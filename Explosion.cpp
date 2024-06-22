#include "Explosion.h"
#include "Engine/time.h"
#include "Camera.h"

namespace{
	const int CHIP_SIZE = 256;
}

Explosion::Explosion(GameObject* parent)
	:GameObject(parent,"Explosion"),eImage_(-1),animFrame_(0),animColumn_(0),
	                                animLine_(0),exTime_(0.0f)
{
}

Explosion::~Explosion()
{
}

void Explosion::Initialize()
{
	eImage_ = LoadGraph("Assets/Image/explosion2.png");
	assert(eImage_ > 0);
}

void Explosion::Update()
{
	if (exTime_ > 0.01) {
		animColumn_++;
		exTime_ = 0.0;
		if (animColumn_ == 8) {
			animColumn_ = 0;
			animLine_++;
		}
	}
	exTime_ += Time::DeltaTime();
	
	if (animLine_ == 8) {
		KillMe();
	}
}

void Explosion::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValue();
	}
	DrawRectGraph(x - CHIP_SIZE/3, y -CHIP_SIZE/5, animFrame_ * CHIP_SIZE, animLine_ * CHIP_SIZE,
		          CHIP_SIZE, CHIP_SIZE, eImage_, TRUE);
	
}

void Explosion::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
}
