#include "Meteorite.h"

Meteorite::Meteorite(GameObject* parent)
	:GameObject(parent,"Meteorite"),mImage_(-1)
{
}

Meteorite::~Meteorite()
{
}

void Meteorite::Initialize()
{
	mImage_ = LoadGraph("");
	assert(mImage_ > 0);
}

void Meteorite::Update()
{
}

void Meteorite::Draw()
{
	DrawGraph(transform_.position_.x, transform_.position_.y, mImage_, TRUE);
}
