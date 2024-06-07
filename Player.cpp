#include "Player.h"

namespace {
	const XMFLOAT3 INITPOS = { 30,600,0 };
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),pImage_(-1)
{
	//‰ŠúˆÊ’u‚Ì’²®(‚ ‚Æ‚Å’l‚Ínamespace‚È‚Ç‚Å‚â‚Á‚Æ‚­)
	transform_.position_ = INITPOS;
}

Player::~Player()
{
	if (pImage_ > 0)
		DeleteGraph(pImage_);
}

void Player::Initialize()
{
	//‚Æ‚è‚ ‚¦‚¸¡‚Íaoi‚ðo‚µ‚Æ‚­(Œã‚Å•Ï‚¦‚é)
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