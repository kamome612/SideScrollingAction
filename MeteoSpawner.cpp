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
		int type;
		Meteorite* MR = Instantiate<Meteorite>(GetParent());
		switch (type_)
		{
		case 0:
			//Meteorite * MR = Instantiate<Meteorite>(GetParent());
			MR->SetPosition(x, y);
			//�O��ނ��烉���_���őI��
			type = rand() % 3;
			MR->SetMoveType(type);
			break;
		case 1:
			//Meteorite * MR = Instantiate<Meteorite>(GetParent());
			MR->SetPosition(x, y);
			//�O��ނ��烉���_���őI��
			type = 0;
			MR->SetMoveType(type);
			break;
		case 2:
			//Meteorite * MR = Instantiate<Meteorite>(GetParent());
			MR->SetPosition(x, y);
			//�O��ނ��烉���_���őI��
			type = 1;
			MR->SetMoveType(type);
			break;
		case3:
			//Meteorite * MR = Instantiate<Meteorite>(GetParent());
			MR->SetPosition(x, y);
			//�O��ނ��烉���_���őI��
			type = 2;
			MR->SetMoveType(type);
			break;
		default:
			break;
		}
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
