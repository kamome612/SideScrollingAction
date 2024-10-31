#include "Stage.h"
#include "Engine/CsvReader.h"
#include "Player.h"
#include "Meteorite.h"
#include "Camera.h"
#include "AttackSkill.h"
#include "Flag.h"
#include "Tree.h"
#include "Cloud.h"
#include "Rocket.h"
#include "Meteo.h"
#include "Enemy.h"
#include "MeteoSpawner.h"
#include "FlyEnemy.h"

namespace {
	const int CHIP_SIZE = 32;
	//const int LIFE_IMAGE_SIZE = 64;
	const int MAP_WIDTH = 1280;
	const int MAP_HEIGHT = 720;
	const int CHIP_NULL = -1;
}

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage")
{
	hImage_ = -1;
	width_ = 0;
	height_ = 0;
	mapNo_ = 1;
	gPict_ = -1;
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
	switch (mapNo_) {
	case 1:
		DrawGraph(0, -240, gPict_, TRUE);
		break;
	case 2:
	case 3:
		DrawGraph(0, 0, gPict_, TRUE);
	}

	int scrollX = 0;
	int scrollY = 0;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		scrollX = cam->GetValueX();
		scrollY = cam->GetValueY();
	}

	//�X�e�[�W�̕`��
	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			int chip = map_[h * width_ + w];

			/*DrawRectGraph(w * CHIP_SIZE-scrollX, h * CHIP_SIZE - scrollY, 
				          CHIP_SIZE * (chip % 16), CHIP_SIZE * (chip / 16), 
				          CHIP_SIZE, CHIP_SIZE, hImage_, TRUE, FALSE);*/

			//�V�����n�ʗp
			DrawRectGraph(w * CHIP_SIZE - scrollX, h * CHIP_SIZE,
				CHIP_SIZE * (chip % 26), CHIP_SIZE * (chip / 26),
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
	//�v���C���[��覐΁A�~�T�C�����X�e�[�W�̃��Z�b�g��Z�b�g���ɏ������悤��
	if (Player* sPlayer = GetParent()->FindGameObject<Player>()) {
		sPlayer->KillMe();
	}
	if (Meteorite* sMeteo = GetParent()->FindGameObject<Meteorite>()) {
		std::list<Meteorite*> meteos = GetParent()->FindGameObjects<Meteorite>();
		for (Meteorite* m : meteos) {
			m->KillMe();
		}
	}
	if (AttackSkill* sAttack = GetParent()->FindGameObject<AttackSkill>()) {
		sAttack->KillMe();
	}

	Camera* cam = GetParent()->FindGameObject<Camera>();
	cam->SetValueX(0);
	cam->SetValueY(-32); //�����őS�̂̃J�����̍�����ς�����
	static const std::string folder = "Assets/Stage/";

	//�X�e�[�W�f�ނ̉摜�̓ǂݍ���
	std::string n = std::to_string(mapNo_);
	//hImage_ = LoadGraph((folder + "bgchar" + n + ".png").c_str());
	//hImage_ = LoadGraph("Assets/Stage/spritesheet_ground.png");
	//hImage_ = LoadGraph("Assets/Stage/ground2.png");

	if (mapNo_ != 3) {
		//hImage_ = LoadGraph((folder + "ground3.png").c_str());
		
		//�V�����n�ʗp
		hImage_ = LoadGraph((folder + "ground.png").c_str());
	}
	else {
		hImage_ = LoadGraph((folder + "ground2.png").c_str());
	}
	assert(hImage_ > 0);

	//�o�b�N�O�����h�̉摜�ǂݍ���
	gPict_ = LoadGraph(("Assets/Picture/background" + n + ".png").c_str());
	assert(gPict_ > 0);

	//csv����ǂݍ���
	CsvReader csv;
	//bool ret = csv.Load((folder + "Stage" + n + ".csv").c_str());

	//�V�����n�ʗp
	bool ret = csv.Load((folder + "test3.csv").c_str());
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
	

	//����؂Ȃǂ̃I�u�W�F�N�g�̐ݒu
	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			switch (csv.GetInt(w, h + height_ + 1))
			{
			case 1://flag
			{
				Flag* sFlag = Instantiate<Flag>(GetParent());
				sFlag->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				break;
			}
			case 14://Tree
			{
				Tree* sTree = Instantiate<Tree>(GetParent());
				sTree->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				break;
			}
			case 30://Cloud
			{
				Cloud* sCloud = Instantiate<Cloud>(GetParent());
				sCloud->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				break;
			}
			}
		}
	}

	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			switch (csv.GetInt(w, h + height_ + 1))
			{
			case 0://player
			{
				//Player* sPlayer = GetParent()->FindGameObject<Player>();
				Player* sPlayer = Instantiate<Player>(GetParent());
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
			case 5: //Enemy
			{
				Enemy* sEnemy = Instantiate<Enemy>(GetParent());
				sEnemy->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				if (mapNo_ == 2) {
					sEnemy->SetGravity(1.62 / 90.0f);
				}
				else if (mapNo_ == 3) {
					sEnemy->SetGravity(3.71 / 90.0f);
				}
				break;
			}
			case 10: //FlyEnemy
			{
				FlyEnemy* fEnemy = Instantiate<FlyEnemy>(GetParent());
				fEnemy->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				break;
			}
			case 15://Meteorite������
			{
				Meteorite* sMeteo = Instantiate<Meteorite>(GetParent());

				sMeteo->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				/*if (mapNo_ == 2) {
					sMeteo->SetGravity(1.62 / 90.0f);
				}
				else if (mapNo_ == 3) {
					sMeteo->SetGravity(3.71 / 90.0f);
				}*/
				sMeteo->SetMoveType(0);
				break;
			}
			case 16://Meteorite�E����
			{
				Meteorite* sMeteo = Instantiate<Meteorite>(GetParent());

				sMeteo->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				/*if (mapNo_ == 2) {
					sMeteo->SetGravity(1.62 / 90.0f);
				}
				else if (mapNo_ == 3) {
					sMeteo->SetGravity(3.71 / 90.0f);
				}*/
				sMeteo->SetMoveType(1);
				break;
			}
			case 17://Meteorite������
			{
				Meteorite* sMeteo = Instantiate<Meteorite>(GetParent());

				sMeteo->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				/*if (mapNo_ == 2) {
					sMeteo->SetGravity(1.62 / 90.0f);
				}
				else if (mapNo_ == 3) {
					sMeteo->SetGravity(3.71 / 90.0f);
				}*/
				sMeteo->SetMoveType(2);
				break;
			}
			case 20://MeteoSpawner
				MeteoSpawner * meteoS = Instantiate<MeteoSpawner>(GetParent());
				meteoS->SetPosition(w* CHIP_SIZE, h* CHIP_SIZE);
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
		return (CHIP_SIZE - y % CHIP_SIZE) + 1;
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
	//destructionRange:�󂷔͈�
	int desRange = 3;
	int desRangeY = 7;

	//覐΂����������Ƃ����ւ����

	for (int i = 0; i < 6; i++) {
		map_[desChip + width_ * i] = CHIP_NULL;
		if (i < 4) {
			map_[desChip + width_ * i + 1] = CHIP_NULL;
			map_[desChip + width_ * i - 1] = CHIP_NULL;
			if (i < 2) {
				map_[desChip + width_ * i - 2] = CHIP_NULL;
				map_[desChip + width_ * i + 2] = CHIP_NULL;
			}
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
	case 520://���̍�
	case 521://���̉E
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
	case 4://�n��(�\��)�̍���
	case 5://�V�̉E��
	case 20://�V�̍���
	case 21://�V�̉E��
	case 0://�n���̍���
	case 1://�V�̉E��
	case 16://�V�̍���
	case 17://�V�̉E��
	case 64://�n��2�̍���
	case 65://�V�̉E��
	case 80://�V�̍���
	case 81://�V�̉E��
		//�V�����n�ʂ̓����蔻��
	case 46:
	case 47:
		return true;
	}
	return false;
}
