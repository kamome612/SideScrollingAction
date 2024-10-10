#include "MeteoSpawner.h"
#include "Camera.h"
#include <assert.h>
#include "Meteorite.h"
#include "Engine/time.h"
#include <random>

MeteoSpawner::MeteoSpawner(GameObject* parent)
	:GameObject(parent,"MeteoSpawner"),timer_(0.0f)
{
}

MeteoSpawner::~MeteoSpawner()
{
}

void MeteoSpawner::Update()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	if (timer_ > interval )
	{
		Meteorite* MR = Instantiate<Meteorite>(GetParent());
		MR->SetPosition(x, y);
		//三種類からランダムで選択
		int type = rand() % 3;
		MR->SetMoveType(type);
		timer_ = 0.0f;
	}
	timer_ += Time::DeltaTime();
}

//一応置いておく使わないようなら後から消す
void MeteoSpawner::Draw()
{
}

void MeteoSpawner::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
}
