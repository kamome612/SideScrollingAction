#include "Meteorite.h"
#include "Camera.h"
#include "PlayScene.h"
#include "Engine/time.h"
#include "Explosion.h"
#include "Stage.h"

namespace {
	static const int SCREEN_WIDTH = 1280;
	//const int CHIP_SIZE = 256;
	const float INIT_GRAVITY = 9.8 / 90.0f;
	const int CHIP_SIZE = 128;
	const int MOVE_SPEED = 100;
	const XMFLOAT3 INIT_POS = { 0,0,0 };
}

Meteorite::Meteorite(GameObject* parent)
	:GameObject(parent, "Meteorite"), mImage_(-1), gravity_(INIT_GRAVITY),
	moveSpeed_(MOVE_SPEED), moveType_(0)
{
}

Meteorite::~Meteorite()
{
	if (mImage_ > 0) {
		DeleteGraph(mImage_);
		mImage_ = -1;
	}
}

void Meteorite::Initialize()
{
	//�摜�̓ǂݍ���
	mImage_ = LoadGraph("Assets/Image/meteo0.png");
	assert(mImage_ > 0);

	transform_.position_ = INIT_POS;
}

void Meteorite::Update()
{
	Stage* mStage = GetParent()->FindGameObject<Stage>();

	//�X�N���[���ɍ��킹�ē����悤��
	int x = (int)transform_.position_.x;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValue();
	}

	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->canMove())//canMove�Ȃ瓮����
		return;

	if (x > SCREEN_WIDTH)//��ʊO(�E��)�ɂ���Ȃ�Ȃɂ����Ȃ�
		return;
	//else if (x < 0 - CHIP_SIZE) {//�}�b�v�O�ɏo���炳��Ȃ�
	//	KillMe();
	//	return;
	//}

	bool isGround = mStage->CollisionDown(transform_.position_.x + CHIP_SIZE / 4.0, transform_.position_.y + CHIP_SIZE / 1.5);
	if (isGround) {//�n�ʂȂǂ̃X�e�[�W�̈ꕔ�ɓ��������炳�悤�Ȃ�
		KillMe();
		mStage->BreakGround(transform_.position_.x, transform_.position_.y + CHIP_SIZE / 1.5);
		Explosion* mEx = Instantiate<Explosion>(GetParent());
		mEx->SetPosition(transform_.position_.x, transform_.position_.y);
	}

	//�V�E�ړ�����(�̗p)
	moveSpeed_ += gravity_;
	if (moveType_ == 0) {//������
		transform_.position_.x -= moveSpeed_ * Time::DeltaTime();
		transform_.position_.y += moveSpeed_ * 0.75 * Time::DeltaTime();
	}
	else if (moveType_ == 1) {//�E����
		transform_.position_.x += moveSpeed_ * Time::DeltaTime();
		transform_.position_.y += moveSpeed_ * 0.75 * Time::DeltaTime();
	}
	else {//������
		transform_.position_.y += moveSpeed_ * 0.75 * Time::DeltaTime();
	}
}

void Meteorite::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValue();
	}
	DrawRectGraph(x, y, 0, 0, CHIP_SIZE, CHIP_SIZE, mImage_, TRUE);

	//�����蔻����������邽�ߗp
	switch (moveType_)//覐΂̎�ނɂ���ĕ�����
	{
	case 0://������
		DrawCircle(x + CHIP_SIZE / 4, y + CHIP_SIZE / 2, 24.0f, GetColor(0, 0, 255), FALSE);
		break;
	case 1://�E����
		DrawCircle(x + CHIP_SIZE / 4 * 3, y + CHIP_SIZE / 2, 24.0f, GetColor(0, 0, 255), FALSE);
		break;
	case 2://������
		DrawCircle(x + CHIP_SIZE / 2, y + CHIP_SIZE / 4 * 3, 24.0f, GetColor(0, 0, 255), FALSE);
		break;
	default:
		break;
	}
}

void Meteorite::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
}

bool Meteorite::CollideCircle(float x, float y, float r)
{
	//x,y,r������̉~�̏��
	//�����̉~�̏��
	float myCenterX;
	float myCenterY;
	switch (moveType_) {//覐΂̎�ނɂ���ĕ�����
	case 0:
		myCenterX = transform_.position_.x + (float)CHIP_SIZE / 4;
		myCenterY = transform_.position_.y + (float)CHIP_SIZE / 2;
		break;
	case 1:
		myCenterX = transform_.position_.x + (float)CHIP_SIZE / 4 * 3;
		myCenterY = transform_.position_.y + (float)CHIP_SIZE / 2;
		break;
	case 2:
		myCenterX = transform_.position_.x + (float)CHIP_SIZE / 2;
		myCenterY = transform_.position_.y + (float)CHIP_SIZE / 4 * 3;
		break;

	}

	float myR = 24.0f;
	float dx = myCenterX - x;
	float dy = myCenterY - y;
	if ((dx * dx + dy * dy) < ((r + myR) * (r + myR))) {
		return true;
	}
	else {
		return false;
	}
}

void Meteorite::SetMoveType(int _type)
{
	moveType_ = _type;
	//�V�E�摜�ǂݍ���
	if (moveType_ == 0) {//�������̉摜
		mImage_ = LoadGraph("Assets/Image/meteo0.png");
		assert(mImage_ > 0);
	}
	else if (moveType_ == 1) {//�E�����̉摜
		mImage_ = LoadGraph("Assets/Image/meteo1.png");
		assert(mImage_ > 0);
	}
	else {//�������̉摜
		mImage_ = LoadGraph("Assets/Image/meteo2.png");
		assert(mImage_ > 0);
	}
}
