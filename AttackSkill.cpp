#include "AttackSkill.h"
#include "Engine/time.h"
#include "Meteorite.h"
#include "Explosion.h"

namespace {
	const float SPEED_ = 150;
}

AttackSkill::AttackSkill(GameObject* parent)
	:GameObject(parent,"AttackSkill"),aImage_(-1)
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
	if (transform_.position_.y > -30) {//画面外に出ていなければ
		transform_.position_.x += SPEED_ * Time::DeltaTime();
		transform_.position_.y -= SPEED_ * Time::DeltaTime();
	}
	else {
		KillMe();
	}

	std::list<Meteorite*> pMeteos = GetParent()->FindGameObjects<Meteorite>();
	for (Meteorite* pMeteo : pMeteos) {
		if (pMeteo->CollideCircle(transform_.position_.x, transform_.position_.y, 24.0f)) {
			//scene->StartDead();
			//ここに爆発のエフェクト入れれたらいいな...
			pMeteo->KillMe();
			Explosion* pEx = Instantiate<Explosion>(GetParent());
			pEx->SetPosition(transform_.position_.x, transform_.position_.y - 32.0f);
			KillMe();
		}
	}
}

void AttackSkill::Draw()
{
	DrawRotaGraph(transform_.position_.x, transform_.position_.y,1.0,0.78, aImage_, TRUE);
	DrawCircle(transform_.position_.x, transform_.position_.y, 24.0f, GetColor(0, 0, 255), FALSE);
}

void AttackSkill::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
}
