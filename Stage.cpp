#include "Stage.h"
#include "Engine/CsvReader.h"
#include "Player.h"
#include "Meteorite.h"
#include "Camera.h"

namespace {
	const int CHIP_SIZE = 32;
	const int MAP_WIDTH = 1280;
	const int MAP_HEIGHT = 720;
	const int CHIP_NULL = 255;
}

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage"),hImage_(-1),width_(0),height_(0),
	                            mapNo_(1),prevResetKey_(false),gPict_(-1)
{
}

Stage::~Stage()
{
	if (hImage_ < 0) {
		DeleteGraph(hImage_);
		hImage_ = -1;
	}

	if (gPict_ < 0) {
		DeleteGraph(gPict_);
		gPict_ = -1;
	}
	if (map_ != nullptr) {
		delete[] map_;
	}
}

void Stage::Initialize()
{
	////�摜�̓ǂݍ���
	//hImage_ = LoadGraph("Assets/Image/bgchar.png");
	//assert(hImage_ > 0);
	map_ = nullptr;
}

void Stage::Update()
{
}

void Stage::Draw()
{
	if (mapNo_ == 1) {
		DrawGraph(5, -250, gPict_, TRUE);
	}
	else if (mapNo_ == 2) {
		DrawGraph(0, 0, gPict_, TRUE);
	}

	int scroll = 0;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		scroll = cam->GetValue();
	}

	//�X�e�[�W�̕`��
	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			int chip = map_[h * width_ + w];
			DrawRectGraph(w * CHIP_SIZE-scroll, h * CHIP_SIZE, 
				          CHIP_SIZE * (chip % 16), CHIP_SIZE * (chip / 16), 
				          CHIP_SIZE, CHIP_SIZE, hImage_, TRUE, FALSE);
		}
	}
}

void Stage::StageSet()
{
	//�}�b�v�̒��ɂȂɂ������Ă������
	if (map_ != nullptr) {
		delete[] map_;
		map_ = nullptr;
	}

	Camera* cam = GetParent()->FindGameObject<Camera>();
	cam->SetValue(0);
	static const std::string folder = "Assets/Stage/";

	//�摜�̓ǂݍ���
	std::string n = std::to_string(mapNo_);
	//hImage_ = LoadGraph((folder + "bgchar" + n + ".png").c_str());
	hImage_ = LoadGraph("Assets/Stage/spritesheet_ground.png");
	assert(hImage_ > 0);

	//�o�b�N�O�����h�̉摜�ǂݍ���
	gPict_ = LoadGraph(("Assets/Picture/background" + n + ".png").c_str());
	assert(gPict_ > 0);

	//csv����ǂݍ���
	CsvReader csv;
	//bool ret = csv.Load((folder + "testStage" + n + ".csv").c_str());
	bool ret = csv.Load("Assets/Stage/testStage.csv");
	assert(ret);

	width_ = csv.GetWidth(0);
	height_ = csv.GetHeight();
	map_ = new int[height_ * width_];

	for (int h = 0; h < height_; h++) {
		if (csv.GetString(0, h) == "") {
			height_ = h;
			break;
		}
		for (int w = 0; w < width_; w++) {
			map_[h * width_ + w] = csv.GetInt(w, h);
		}
	}
	

	//Map�f�[�^�̒���0������΁APlayer�̍��W��0�̈ʒu�ɂ���
	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			switch (csv.GetInt(w, h + height_ + 1))
			{
			case 0://player
			{
				Player* sPlayer = GetParent()->FindGameObject<Player>();
				//sPlayer_ = Instantiate<Player>(this);
				sPlayer->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				//�Ƃ肠�����̃}�b�v�ύX�p
				switch (mapNo_)
				{
				case 2:
					sPlayer->SetGravity(1.62 / 90.0f);
					break;
				case 3:
					sPlayer->SetGravity(3.71 / 90.0f);
					break;
				default:
					break;
				}
				break;
			}
			case 15://Meteorite
				Meteorite * sMeteo = Instantiate<Meteorite>(GetParent());
				//sMeteo_ = Instantiate<Meteorite>(this);
				sMeteo->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				break;
			}
		}
	}
}

void Stage::StageReset()
{
	//�}�b�v�̒��ɂȂɂ������Ă������
	if (map_ != nullptr) {
		delete[] map_;
		map_ = nullptr;
	}

	static const std::string folder = "Assets/Stage/";
	
	//�摜�̓ǂݍ���
	std::string n = std::to_string(mapNo_);
	//hImage_ = LoadGraph((folder + "bgchar" + n + ".png").c_str());
	hImage_ = LoadGraph("Assets/Stage/spritesheet_ground.png");
	assert(hImage_ > 0);
	//csv����ǂݍ���
	CsvReader csv;
	bool ret = csv.Load((folder +"testStage"+ n + ".csv").c_str());
	//bool ret = csv.Load("Assets/Stage/testStage2.csv");
	assert(ret);
	width_ = csv.GetWidth(0);
	height_ = csv.GetHeight();
	map_ = new int[height_ * width_];

	for (int h = 0; h < height_; h++) {
		if (csv.GetString(0, h) == "") {
			height_ = h;
			break;
		}
		for (int w = 0; w < width_; w++) {
			map_[h * width_ + w] = csv.GetInt(w, h);
		}
	}

	//Map�f�[�^�̒���0������΁APlayer�̍��W��0�̈ʒu�ɂ���
	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			switch (csv.GetInt(w, h + height_ + 1))
			{
			case 0://player
			{
				Player* sPlayer = GetParent()->FindGameObject<Player>();
				sPlayer->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
			}
			case 15://Meteorite
				Meteorite * sMeteo = Instantiate<Meteorite>(GetParent());
				sMeteo->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				break;
			}
		}
	}
}

int Stage::CollisionRight(int x, int y)
{
	if (IsWallBlock(x, y)) {
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return x % CHIP_SIZE + 1;
	}
	else
		return 0;
}

int Stage::CollisionLeft(int x, int y)
{
	if (IsWallBlock(x, y)) {
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return x % CHIP_SIZE - 28;
	}
	else
		return 0;
}

int Stage::CollisionDown(int x, int y)
{
	if (IsWallBlock(x, y)) {
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return y % CHIP_SIZE + 1;
	}
	else
		return 0;
}

int Stage::CollisionUp(int x, int y)
{
	if (IsWallBlock(x, y)) {
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return y % CHIP_SIZE - 25;
	}
	else
		return 0;
}

void Stage::BreakGround(int x, int y)
{
	int chipX = x / CHIP_SIZE;
	int chipY = y / CHIP_SIZE;

	//destructionChip:�󂷊�̃`�b�v�̏ꏊ
	int desChip = chipY * width_ + chipX;
	//destructionRamge:�󂷔͈�
	int desRange = 5;
	for (int i = 0; i <= desRange; i++) {
		map_[desChip + i] = CHIP_NULL;
		if (i > 0 && i < desRange) {
			map_[desChip + width_ + i] = CHIP_NULL;
		}
	}
}

bool Stage::IsWallBlock(int x, int y)
{
	int chipX = x / CHIP_SIZE;
	int chipY = y / CHIP_SIZE;
	switch (map_[chipY * width_ + chipX]) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 10:
	case 11:
	case 16:
	case 17:
	case 32:
	case 33:
	case 48:
	case 49:
	case 34:
	case 35:
	case 64:
	case 65:
		//���̕��̃}�b�v�`�b�v
	case 132:
	case 133:
	case 148:
	case 149:
	case 164:
	case 165:
	case 180:
	case 181:
	case 196:
	case 197:
	case 212:
	case 213:
	case 292:
	case 293:
	case 324:
	case 325:
	case 356:
	case 357:
		return true;
	}
	return false;
}
