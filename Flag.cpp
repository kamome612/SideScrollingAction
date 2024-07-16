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
		x -= cam->GetValue();
	}
	DrawRectGraph(x, y, 0, animFrame_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE, fImage_, TRUE);
}

void Flag::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
}
