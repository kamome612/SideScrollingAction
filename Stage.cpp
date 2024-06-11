#include "Stage.h"
#include "Camera.h"
#include "Player.h"
#include "Engine/CsvReader.h"
#include <assert.h>

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage")
{
	//���̉摜(��ŏE���Ă����f�ނɕς���)
	sImage_ = LoadGraph("Assets\\Image\\bgchar.png");//�X�e�[�W�̉摜�ǂݍ���
	assert(sImage_ > 0);
	Map_ = nullptr;
}

Stage::Stage(GameObject* parent, int _number)
{
	//���̉摜(��ŏE���Ă����f�ނɕς���)
	//sImage_ = LoadGraph("�`,_number");//�n���ꂽ�l�ɉ������X�e�[�W�f�[�^�����[�h����
	sImage_ = LoadGraph("Assets\\Image\\bgchar.png");//�X�e�[�W�̉摜�ǂݍ���
	assert(sImage_ > 0);
	Map_ = nullptr;
}

Stage::~Stage()
{
	if (sImage_ > 0) {
		DeleteGraph(sImage_);
	}
	if (Map_ != nullptr) {
		delete[] Map_;
	}
}

void Stage::Reset()
{
	//if (Map_ != nullptr) {
	//	delete[] Map_;
	//	Map_ = nullptr;
	//}
	//CsvReader csv;//�f�[�^��ǂރN���X�̃C���X�^���X���쐬
	//bool ret = csv.Load("Assets\\Image\\stage1.csv");
	//assert(ret);
	//width_ = csv.GetWidth();
	//height_ = csv.GetHeight();
	//Map_ = new int[width_ * height_];

	//for (int h = 0; h < height_; h++) {
	//	if (csv.GetString(0, h) == "") {
	//		height_ = h;
	//		break;
	//	}
	//	for (int w = 0; w < width_; w++) {
	//		Map_[h * width_ + w] = csv.GetValue(w, h);
	//	}
	//}
	////Map�f�[�^�̒���0������΁APlayer�̍��W��0�̈ʒu�ɂ���
	//for (int h = 0; h < height_; h++) {
	//	for (int w = 0; w < width_; w++) {
	//		switch (csv.GetValue(w, h + height_ + 1))
	//		{
	//		case 0://player
	//		{
	//			Player* pplayer = GetParent()->FindGameObject<Player>();
	//			pplayer->SetPosition(w * 32, h * 32);
	//		}
	//		break;
	//		}
	//	}
	//}

	if (Map_ != nullptr) {
		delete[] Map_;
		Map_ = nullptr;
	}
	CsvReader csv;//�f�[�^��ǂރN���X�̃C���X�^���X���쐬
	bool ret = csv.Load("Assets\\Image\\stage1.csv");
	assert(ret);
	width_ = csv.GetWidth();
	height_ = csv.GetHeight();
	Map_ = new int[width_ * height_];//C����ł̓��I�񎟌��z��̎���

	for (int h = 0; h < height_; h++) {
		if (csv.GetString(0, h) == "") {
			height_ = h;
			break;
		}
		for (int w = 0; w < width_; w++) {
			Map_[h * width_ + w] = csv.GetValue(w, h);
		}
	}
	//Map�f�[�^�̒���0������΁APlayer�̍��W��0�̈ʒu�ɂ���
	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			switch (csv.GetValue(w, h + height_ + 1))
			{
			case 0://player
			{
				Player* pplayer = GetParent()->FindGameObject<Player>();
				pplayer->SetPosition(w * 32, h * 32);
			}
			break;
			case 1://bird
			{
				/*Bird* pbird = Instantiate<Bird>(GetParent());
				pbird->SetPosition(w * 32, h * 32);*/
			}
			break;
			}
		}
	}
}

void Stage::Update()
{
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

	for (int y = 0; y < height_; y++) {
		for (int x = 0; x < width_; x++) {
			int chip = Map_[y * width_ + x];
			DrawRectGraph(x * 32 - scroll, y * 32, 32 * (chip % 16), 32 * (chip / 16), 32, 32, sImage_, TRUE);
		}
	}
}
