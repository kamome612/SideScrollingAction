#include "Health.h"
#include "Engine/time.h"
#include "Camera.h"

namespace {
	const float IMAGE_SIZE = 48.0f;
	const float FIN_DIS_TIME = 10.0f;
	//const XMFLOAT3 INIT_POS = { 500,580,0 };
}

Health::Health(GameObject* parent)
	:GameObject(parent,"Health"),hImage_(-1),disTime_(0.0f),animFrame_(0),
	animType_(0),time_(0.0f)
{
}

Health::~Health()
{
	if (hImage_ > 0) {
		DeleteGraph(hImage_);
		hImage_ = -1;
	}
}

void Health::Initialize()
{
	hImage_ = LoadGraph("Assets\\Image\\Heal2.png");
	assert(hImage_ >= 0);
}

void Health::Update()
{
	//一定時間経過で点滅して消える
	disTime_ += Time::DeltaTime();
	float tmp = 7;
	if (disTime_ >= tmp) {
		//tmp = disTime_ - 1;
		time_ += Time::DeltaTime();
		if (time_ >= 0.2) {
			time_ = 0;
			animFrame_ = animFrame_ % 3 + 1;
		}
	}
	if (disTime_ > FIN_DIS_TIME) {

		KillMe();
	}
}

void Health::Draw()
{
	
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	DrawRectGraph(x,y+16,animFrame_*IMAGE_SIZE,0,IMAGE_SIZE,IMAGE_SIZE, hImage_, TRUE);

	//当たり判定見るよう
	//DrawCircle( x + IMAGE_SIZE/2, y + IMAGE_SIZE/2+16, 16.0f, GetColor(0, 0, 255), FALSE);
}

void Health::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
}

bool Health::CollideCircle(float x, float y, float r)
{
	float myCenterX = transform_.position_.x + (float)IMAGE_SIZE/2;
	float myCenterY = transform_.position_.y + (float)IMAGE_SIZE/2+16;
	float myR = 16.0f;
	float dx = myCenterX - x;
	float dy = myCenterY - y;
	if ((dx * dx + dy * dy) < ((r + myR) * (r + myR))) {
		return true;
	}
	else {
		return false;
	}
}
