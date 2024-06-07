#include "Player.h"

namespace {
	const XMFLOAT3 INITPOS = { 30,600,0 };
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),pImage_(-1)
{
	//�����ʒu�̒���(���ƂŒl��namespace�Ȃǂł���Ƃ�)
	transform_.position_ = INITPOS;
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
}

void Player::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	DrawGraph(x, y, pImage_, TRUE);
}