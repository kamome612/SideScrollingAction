#include "ReloadGauge.h"
#include "Camera.h"



ReloadGauge::ReloadGauge(GameObject* parent)
	:GameObject(parent, "ReloadGauge"), hImage_(-1),hImageF_(-1),maxW(50), progress_(0.0f)
{
}

ReloadGauge::~ReloadGauge()
{
	if (hImage_ > 0) {
		DeleteGraph(hImage_);
	}
	if (hImage_ > 0) {
		DeleteGraph(hImageF_);
	}
}

void ReloadGauge::Initialize()
{

	hImage_ = LoadGraph("Assets\\Image\\gauge.jpg");
	assert(hImage_ >= 0);
	hImageF_ = LoadGraph("Assets\\Image\\gaugeF.jpg");
	assert(hImageF_ >= 0);
}

void ReloadGauge::Update()
{
	
}

void ReloadGauge::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;

	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		//y -= cam->GetValueY();
	}
	
	DrawExtendGraph(x - 5, y, x + maxW, y - 5, hImageF_, TRUE);//フレーム

	float currentWidth = maxW * progress_;
	DrawExtendGraph(x -5 , y, x + currentWidth , y - 5, hImage_, TRUE);//ゲージ
	

}

void ReloadGauge::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
}

void ReloadGauge::SetProgress(float progress)
{
	progress_ = progress;
}

