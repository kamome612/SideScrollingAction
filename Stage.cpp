#include "Stage.h"
#include "Engine/CsvReader.h"
#include "Player.h"
#include "Camera.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage"),hImage_(-1),width_(0),height_(0)
{
}

Stage::~Stage()
{
	if (hImage_ < 0) {
		DeleteGraph(hImage_);
	}
	if (map_ != nullptr) {
		delete[] map_;
	}
}

void Stage::Initialize()
{
	//�摜�̓ǂݍ���
	hImage_ = LoadGraph("Assets/Image/bgchar.png");
	assert(hImage_ > 0);
	map_ = nullptr;
}

void Stage::Update()
{
	//R�����ꂽ�烊�Z�b�g
	if (CheckHitKey(KEY_INPUT_R))
		Reset();
}

void Stage::Draw()
{
	int scroll = 0;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		scroll = cam->GetValue();
	}

	//�X�e�[�W�̕`��
	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			int chip = map_[h * width_ + w];
			DrawRectGraph(w * 32-scroll, h * 32, 32 * (chip % 16), 32 * (chip / 16), 32, 32, hImage_, TRUE, FALSE);
		}
	}
}

void Stage::Reset()
{
	//�}�b�v�̒��ɂȂɂ������Ă������
	if (map_ != nullptr) {
		delete[] map_;
		map_ = nullptr;
	}

	//csv����ǂݍ���
	CsvReader csv;
	bool ret = csv.Load("Assets/Image/stage1.csv");
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
				Player* pPlayer = GetParent()->FindGameObject<Player>();
				pPlayer->SetPosition(w * 32, h * 32);
				break;
			}
			}
		}
	}
}

int Stage::CollisionRight(int x, int y)
{
	if (IsWallBlock(x, y)) {
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return x % 32 + 1;
	}
	else
		return 0;
}

int Stage::CollisionLeft(int x, int y)
{
	if (IsWallBlock(x, y)) {
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return x % 32 - 1;
	}
	else
		return 0;
}

int Stage::CollisionDown(int x, int y)
{
	if (IsWallBlock(x, y)) {
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return y % 32 + 1;
	}
	else
		return 0;
}

int Stage::CollisionUp(int x, int y)
{
	if (IsWallBlock(x, y)) {
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return y % 32 - 1;
	}
	else
		return 0;
}

bool Stage::IsWallBlock(int x, int y)
{
	int chipX = x / 32;
	int chipY = y / 32;
	switch (map_[chipY * width_ + chipX]) {
	case 16:
	case 17:
	case 18:
	case 19:
	case 32:
	case 33:
	case 34:
	case 35:
		return true;
	}
	return false;
}