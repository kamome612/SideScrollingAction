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
		//�O��ނ��烉���_���őI��
		int type = rand() % 3;
		MR->SetMoveType(type);
		timer_ = 0.0f;
	}
	timer_ += Time::DeltaTime();
}

//�ꉞ�u���Ă����g��Ȃ��悤�Ȃ�ォ�����
void MeteoSpawner::Draw()
{
}

void MeteoSpawner::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
}