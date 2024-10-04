#include "AttackSkill.h"
#include "Engine/time.h"
#include "Meteorite.h"
#include "Explosion.h"
#include "Camera.h"

namespace {
	const float SPEED_ = 300;
	const float LimitTime_ = 1.5;//消える時間
}

AttackSkill::AttackSkill(GameObject* parent)
	:GameObject(parent,"AttackSkill"),aImage_(-1),BulletTime_(0)
{
}

AttackSkill::~AttackSkill()
{
	if (aImage_ > 0) {
		DeleteGraph(aImage_);
	}
}

void AttackSkill::Initialize()
{
	aImage_ = LoadGraph("Assets/Image/missile.png");
	assert(aImage_ > 0);
}

void AttackSkill::Update()
{
	BulletTime_ += Time::DeltaTime();
	if (BulletTime_ >= LimitTime_)//時間で消える
	{
		KillMe();
	}
	//if (transform_.position_.x < -30) {//画面外に出ていれば消す
	//	KillMe();
	//}
	transform_.position_.x += SPEED_ * Time::DeltaTime();
	//transform_.position_.y -= SPEED_ * Time::DeltaTime();

	std::list<Meteorite*> pMeteos = GetParent()->FindGameObjects<Meteorite>();
	for (Meteorite* pMeteo : pMeteos) {
		if (pMeteo->CollideCircle(transform_.position_.x, transform_.position_.y, 10.0f)) {
			pMeteo->KillMe();
			Explosion* pEx = Instantiate<Explosion>(GetParent());
			pEx->SetPosition(transform_.position_.x - 32.0f, transform_.position_.y - 64.0f);
			KillMe();
		}
	}
}

void AttackSkill::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValue();
	}
	DrawRotaGraph(x,y,1.0,1.6, aImage_, TRUE);
	//DrawRotaGraph(x, y, 1.0, 1.6, aImage_, TRUE);
	//DrawCircle(x,y, 10.0f, GetColor(0, 0, 255), FALSE);
}

void AttackSkill::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y + 21;
}
