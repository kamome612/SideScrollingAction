#include "Player.h"

namespace {
	const XMFLOAT3 INITPOS = { 30,550,0 };
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),pImage_(-1)
{
	//�����ʒu�̒���(���ƂŒl��namespace�Ȃǂł���Ƃ�)
	transform_.position_ = INITPOS;
	state_ = S_Stop;
}

Player::~Player()
{
	if (pImage_ > 0)
		DeleteGraph(pImage_);
}

void Player::Initialize()
{
	//�Ƃ肠��������aoi���o���Ƃ�(��ŕς���)
	pImage_ = LoadGraph("Assets\\Image\\chara.png");
	assert(pImage_ >= 0);
}

void Player::Update()
{
	switch (state_){
	case 0: 
		UpdateNormal();
		break;
	case 1:
		UpdateWalk();
		break;
	default:
		break;
	}
}

void Player::UpdateNormal()
{
	/*if (InputKey(KEY_INPUT_S)) {
		tranform_.position_.x += 0.1;
	}
	if (InputKey(KEY_INPUT_A))
	{
		transform_.position_.x -= 0.1;
	}*/
}

void Player::UpdateWalk()
{
	if (InputKey(KEY_INPUT_S)) {//S�L�[�������ƉE�ɐi��
		tranform_.position_.x += 0.1;
	}
	if (InputKey(KEY_INPUT_A)) {//A�L�[�������ƍ��ɐi��
		transform_.position_.x -= 0.1;
	}
}

void Player::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	DrawGraph(x, y, pImage_, TRUE);
}