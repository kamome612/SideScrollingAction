#include "Stage.h"
#include "Camera.h"
#include "Player.h"
#include "Engine/CsvReader.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage")
{
	sImage_ = LoadGraph("Assets\\Image\\bgchar.png");//�X�e�[�W�̉摜�ǂݍ���
	assert(sImage_ > 0);
}

Stage::Stage(GameObject* parent, int _number)
{
	//sImage_ = LoadGraph("�`_number");//�n���ꂽ�l�ɉ������X�e�[�W�f�[�^�����[�h����
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
	if (Map_ != nullptr) {
		delete[] Map_;
		Map_ = nullptr;
	}
	CsvReader csv;//�f�[�^��ǂރN���X�̃C���X�^���X���쐬
	bool ret = csv.Load("");
	assert(ret);
	width_ = csv.GetWidth();
	height_ = csv.GetHeight();
	Map_ = new int[width_ * height_];

	for (int h = 0; h < height_; h++) {
		if (csv.GetString(0, h) == "") {
			height_ = h;
			break;
		}
		for (int w = 0; w < width_; w++) {
			Map_[h * width_ + w] = csv.GetValue(w,h);
		}
	}
}

void Stage::Update()
{
}

void Stage::Draw()
{
}
