#include "Flag.h"
#include "Camera.h"
#include "Engine/time.h"

namespace {
	const int CHIP_SIZE = 64;
}

Flag::Flag(GameObject* parent)
	:GameObject(parent, "GoalFlag"), fImage_(-1),animFrame_(0),time_(0)
{
}

Flag::~Flag()
{
	if (fImage_ > 0) {
		DeleteGraph(fImage_);
	}
}

void Flag::Initialize()
{
	fImage_ = LoadGraph("Assets\\Stage\\flag.png");
	assert(fImage_ > 0);
}

void Flag::Update()
{
	if (time_ > 0.4f) {
		if (animFrame_ == 1) {
			animFrame_ = 0;
		}
		else {
			animFrame_++;
		}
		time_ = 0.0f;
	}
	time_ += Time::DeltaTime();
}

void Flag::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	DrawRectGraph(x, y, 0, animFrame_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE, fImage_, TRUE);
    //“–‚½‚è”»’è‚ÌˆÊ’uŠm‚©‚ß‚é—p
	//DrawCircle(x + CHIP_SIZE/2, y + CHIP_SIZE /2,
	//	       20.0f, GetColor(75, 0, 130),false);
}

void Flag::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
}

bool Flag::HitFlag(float _x, float _y, float _r)
{
	//x,y,r‚ª‘Šè‚Ì‰~‚Ìî•ñ
	//©•ª‚Ì‰~‚Ìî•ñ
	float myCenterX = transform_.position_.x + (float)CHIP_SIZE / 2;
	float myCenterY = transform_.position_.y + (float)CHIP_SIZE / 2;
	float myR = 24.0f;
	float dx = myCenterX - _x;
	float dy = myCenterY - _y;
	if ((dx * dx + dy * dy) < (_r + myR) * (_r + myR))
		return true;
	else
		return false;
}
