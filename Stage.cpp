#include "Stage.h"
#include "Engine/CsvReader.h"
#include "Player.h"
#include "Meteorite.h"
#include "Camera.h"

namespace {
	const int CHIP_SIZE = 32;
	//const int CHIP_SIZE = 64;
	const int LIFE_IMAGE_SIZE = 64;
	const int MAP_WIDTH = 1280;
	const int MAP_HEIGHT = 720;
	const int CHIP_NULL = 255;
}

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage")
{
	hImage_ = -1;
	lImage_ = -1;
	width_ = 0;
	height_ = 0;
	mapNo_ = 1;
	prevResetKey_ = false;
	gPict_ = -1;
	stageLife_ = 0;
	map_ = nullptr;
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

	if (lImage_ < 0) {
		DeleteGraph(lImage_);
		lImage_ = -1;
	}

	if (map_ != nullptr) {
		delete[] map_;
	}
}

void Stage::Initialize()
{
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

	//�X�e�[�W�̗̑͂̕\��
	for (int i = 0; i < stageLife_; i++) {
		DrawGraph(LIFE_IMAGE_SIZE * i, 0, lImage_, TRUE);
	}

}

void Stage::StageSet()
{
	//�}�b�v�̒��ɂȂɂ������Ă������
	if (map_ != nullptr) {
		delete[] map_;
		map_ = nullptr;
	}
	//�v���C���[��覐΂��X�e�[�W�̃��Z�b�g��Z�b�g���ɏ������悤��
	if (Player* sPlayer = GetParent()->FindGameObject<Player>()) {
		sPlayer->KillMe();
	}
	if (Meteorite* sMeteo = GetParent()->FindGameObject<Meteorite>()) {
		std::list<Meteorite*> meteos = GetParent()->FindGameObjects<Meteorite>();
		for (Meteorite* m : meteos) {
			m->KillMe();
		}
	}

	Camera* cam = GetParent()->FindGameObject<Camera>();
	cam->SetValue(0);
	static const std::string folder = "Assets/Stage/";

	//�X�e�[�W�f�ނ̉摜�̓ǂݍ���
	std::string n = std::to_string(mapNo_);
	//hImage_ = LoadGraph((folder + "bgchar" + n + ".png").c_str());
	hImage_ = LoadGraph("Assets/Stage/spritesheet_ground.png");
	//hImage_ = LoadGraph("Assets/Stage/tilesheet_complete.png");
	assert(hImage_ > 0);

	//�o�b�N�O�����h�̉摜�ǂݍ���
	gPict_ = LoadGraph(("Assets/Picture/background" + n + ".png").c_str());
	assert(gPict_ > 0);

	//�̗͂������摜�̓ǂݍ���
	lImage_ = LoadGraph(("Assets/Image/planet"+ n + ".png").c_str());
	assert(lImage_ > 0);

	//csv����ǂݍ���
	CsvReader csv;
	bool ret = csv.Load((folder + "testStage" + n + ".csv").c_str());
	//bool ret = csv.Load("Assets/Stage/testStage.csv");
	assert(ret);

	//csv�œǂ񂾃X�e�[�W�̉��Əc�����
	width_ = csv.GetWidth(0);
	height_ = csv.GetHeight();

	map_ = new int[height_ * width_];

	for (int h = 0; h < height_; h++) {
		if (csv.GetString(0, h) == "") {//��s���΂�
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
				//Player* sPlayer = GetParent()->FindGameObject<Player>();
				Player* sPlayer = Instantiate<Player>(GetParent());
				sPlayer ->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				//�Ƃ肠�����̃}�b�v�ύX�p
				switch (mapNo_)
				{
				case 2:
					sPlayer->SetGravity(1.62 / 90.0f);
					stageLife_ = 3;;
					break;
				case 3:
					sPlayer->SetGravity(3.71 / 90.0f);
					stageLife_ = 3;
					break;
				default:
					stageLife_ = 5;
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
		//return x % CHIP_SIZE - 28;
		return (CHIP_SIZE - x % CHIP_SIZE) + 1;
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
	//覐΂̓��������񐔂𑝂₷
	stageLife_ -= 1;

	int chipX = x / CHIP_SIZE;
	int chipY = y / CHIP_SIZE;

	//destructionChip:�󂷊�̃`�b�v�̏ꏊ
	int desChip = chipY * width_ + chipX;
	//destructionRange:�󂷔͈�
	int desRange = 5;

	//覐΂����������Ƃ����ւ����
	for (int i = 0; i <= desRange; i++) {
		map_[desChip - width_ + i] = CHIP_NULL;
		map_[desChip + i] = CHIP_NULL;
		map_[desChip + width_ + i] = CHIP_NULL;
		if (i > 0 && i < desRange) {
			map_[desChip + width_*2 + i] = CHIP_NULL;
		}
	}
}

bool Stage::IsWallBlock(int x, int y)
{
	//�n�ʂȂǂ̓����蔻��̂���`�b�v�Ȃ̂�(�X�e�[�W���Ƃɕς��Ă�����)
	int chipX = x / CHIP_SIZE;
	int chipY = y / CHIP_SIZE;
	switch (map_[chipY * width_ + chipX]) {
	case 2://�y�̍���
	case 3://�y�̉E��
	case 18://�y�̍���
	case 19://�y�̉E��
	case 32://�n�ʂ̍���
	case 33://�n�ʂ̉E��
	case 48://�n�ʂ̍���
	case 49://�n�ʂ̉E��
	case 160://�����Ă鑫��(�E)�̍���
	case 161://�����Ă鑫��(�E)�̉E��
	case 192://�����Ă鑫��(����)�̍���
	case 193://�����Ă鑫��(����)�̉E��
	case 224://�����Ă鑫��(��)�̍���
	case 225://�����Ă鑫��(��)�̉E��
		//���̕��̃}�b�v�`�b�v
	case 134://�n���̍���
	case 135://�n���̉E��
	case 150://�n���̍���
	case 151://�n���̉E��
	case 164://�n��(�\��)�̍���
	case 165://�n��(�\��)�̉E��
	case 180://�n��(�\��)�̍���
	case 181://�n��(�\��)�̉E��
	case 292://�����Ă鑫��(�E)�̍���
	case 293://�V�̉E��
	case 324://�����Ă鑫��(����)�̍���
	case 325://�V�̉E��
	case 356://�����Ă鑫��(��)�̍���
	case 357://�V�̉E��
		//�ΐ��̕��̃}�b�v�`�b�v
		return true;
	}
	return false;
}
