#include "Player.h"

namespace {
	const XMFLOAT3 INITPOS = { 30,550,0 };
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),pImage_(-1)
{
	//初期位置の調整(あとで値はnamespaceなどでやっとく)
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
	//とりあえず今はaoiを出しとく(後で変える)
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
	if (InputKey(KEY_INPUT_S)) {//Sキーを押すと右に進む
		tranform_.position_.x += 0.1;
	}
	if (InputKey(KEY_INPUT_A)) {//Aキーを押すと左に進む
		transform_.position_.x -= 0.1;
	}
}

void Player::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	DrawGraph(x, y, pImage_, TRUE);
}