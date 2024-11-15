#include "Meteorite.h"
#include "Camera.h"
#include "PlayScene.h"
#include "Engine/time.h"
#include "Explosion.h"
#include "Stage.h"

namespace {
	static const int SCREEN_WIDTH = 1280;
	static const int SCREEN_HEIGHT = 720;
	//const int CHIP_SIZE = 256;
	const float INIT_GRAVITY = 9.8 / 90.0f;
	const int CHIP_SIZE = 128;
	const int MOVE_SPEED = 100;
	const XMFLOAT3 INIT_POS = { 0,0,0 };
}

Meteorite::Meteorite(GameObject* parent)
	:GameObject(parent, "Meteorite"), mImage_(-1), gravity_(INIT_GRAVITY),
	moveSpeed_(MOVE_SPEED), moveType_(0),isDead_(false)
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
	if (mStage->GetMapNo() == 2) {
		gravity_ = 1.62 / 90.0f;
	}
	else if (mStage->GetMapNo() == 3) {
		gravity_ = 3.71 / 90.0f;
	}

	//�X�N���[���ɍ��킹�ē����悤��
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}

	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->canMove())//canMove�Ȃ瓮����
		return;

	if (x < 0 - CHIP_SIZE || y > SCREEN_HEIGHT) {
		//�}�b�v�O�ɏo���炳��Ȃ�
		KillMe();
		return;
	}

	//�n�ʂɓ���������
	bool hitGround = false;
	float hitX, hitY; //覐΂̐�[�̓_
	//float hitXU, hitYU; //覐΂̉���ӂ̓_
	switch (moveType_)//覐΂̎�ނɂ���ĕς���
	{
	case 0://������
		//hitGround = mStage->CollisionDown(transform_.position_.x + CHIP_SIZE / 4.0, transform_.position_.y + CHIP_SIZE / 2.0);
		hitX = transform_.position_.x + CHIP_SIZE / 8.0;
		hitY = transform_.position_.y + CHIP_SIZE / 24.0 * 15.0;
		hitGround = mStage->CollisionDown(hitX, hitY);
		break;
	case 1://�E����
		//hitGround = mStage->CollisionDown(transform_.position_.x + CHIP_SIZE / 4.0 * 3.0, transform_.position_.y + CHIP_SIZE / 2.0);
		hitX = transform_.position_.x + CHIP_SIZE / 8.0 * 7.0;
		hitY = transform_.position_.y + CHIP_SIZE / 24.0 * 15.0;
		hitGround = mStage->CollisionDown(hitX, hitY);
		break;
	case 2://������
		//hitGround = mStage->CollisionDown(transform_.position_.x + CHIP_SIZE / 2.0, transform_.position_.y + CHIP_SIZE / 4 * 3);
		hitX = transform_.position_.x + CHIP_SIZE / 8.0 * 3.0;
		hitY = transform_.position_.y + CHIP_SIZE / 16.0 * 15.0;
		hitGround = mStage->CollisionDown(hitX, hitY);
		break;
	}

	if (hitGround) {//�n�ʂȂǂ̃X�e�[�W�̈ꕔ�ɓ����������
		//KillMe();
		//if (!(x > SCREEN_WIDTH)) {//��ʊO(�E��)�ɂ���Ȃ�X�e�[�W�󂵂��肵�Ȃ��悤��
		//	mStage->BreakGround(transform_.position_.x, transform_.position_.y + CHIP_SIZE / 1.5);
		//	Explosion* mEx = Instantiate<Explosion>(GetParent());
		//	mEx->SetPosition(transform_.position_.x, transform_.position_.y);
		//	//mEx->SetPosition(hitX, hitY);
		//}
		if (!(x > SCREEN_WIDTH ) && !(x < 0)) {//��ʓ��ɂ���Ȃ�X�e�[�W����
			mStage->OneSquareBreak(hitX, hitY);
			/*hitXU = transform_.position_.x + CHIP_SIZE / 4;
			hitYU = transform_.position_.y + CHIP_SIZE / 24 * 17;
			mStage->OneSquareBreak(hitXU, hitYU);*/
			//if (!isDead_) {//�ŏ������������o��
				isDead_ = true;
				Explosion* mEx = Instantiate<Explosion>(GetParent());
				mEx->SetPosition(transform_.position_.x, transform_.position_.y);
			//}
		}
		else {
			KillMe();
		}
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
		x -= cam->GetValueX();
		//y -= cam->GetValueY();
	}

	//if (!isDead_) {
		DrawRectGraph(x, y, 0, 0, CHIP_SIZE, CHIP_SIZE, mImage_, TRUE);
	//}
	//�����蔻����������邽�ߗp
	switch (moveType_)//覐΂̎�ނɂ���ĕ�����
	{
	case 0://������
		DrawCircle(x + CHIP_SIZE / 4, y + CHIP_SIZE / 2, 24.0f, GetColor(0, 0, 255), FALSE);
		DrawCircle(x + CHIP_SIZE / 8, y + CHIP_SIZE / 24 * 15, 2.0f, GetColor(0, 0, 255), TRUE);
		//DrawCircle(x + CHIP_SIZE / 4, y + CHIP_SIZE / 24 * 17, 2.0f, GetColor(0, 0, 255), TRUE);
		break;
	case 1://�E����
		DrawCircle(x + CHIP_SIZE / 4 * 3, y + CHIP_SIZE / 2, 24.0f, GetColor(0, 0, 255), FALSE);
		DrawCircle(x + CHIP_SIZE / 8 * 7, y + CHIP_SIZE / 24 * 15, 2.0f, GetColor(0, 0, 255), TRUE);
		break;
	case 2://������
		DrawCircle(x + CHIP_SIZE / 8 * 3, y + CHIP_SIZE / 4 * 3, 24.0f, GetColor(0, 0, 255), FALSE);
		DrawCircle(x + CHIP_SIZE / 8 * 3, y + CHIP_SIZE / 16 * 15, 2.0f, GetColor(0, 0, 255), TRUE);
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
	float myCenterX = 0.0f;
	float myCenterY = 0.0f;
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
		myCenterX = transform_.position_.x + (float)CHIP_SIZE / 4 * 3 / 2;
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
