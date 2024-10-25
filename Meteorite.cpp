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
	//画像の読み込み
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

	//スクロールに合わせて動くように
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}

	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->canMove())//canMoveなら動かす
		return;

	if (x < 0 - CHIP_SIZE || y > SCREEN_HEIGHT) {
		//マップ外に出たらさよなら
		KillMe();
		return;
	}

	bool isGround = false;
	switch (moveType_)//隕石の種類によって変える
	{
	case 0://左向き
	    isGround = mStage->CollisionDown(transform_.position_.x + CHIP_SIZE / 4.0, transform_.position_.y + CHIP_SIZE / 2.0);
		break;
	case 1://右向き
		isGround = mStage->CollisionDown(transform_.position_.x + CHIP_SIZE / 4.0 * 3.0, transform_.position_.y + CHIP_SIZE / 2.0);
		break;
	case 2://下向き
		isGround = mStage->CollisionDown(transform_.position_.x + CHIP_SIZE / 2.0, transform_.position_.y + CHIP_SIZE / 4 * 3);
		break;
	}

	if (isGround) {//地面などのステージの一部に当たったらさようなら
		KillMe();
		if (!(x > SCREEN_WIDTH)) {//画面外(右側)にいるならステージ壊したりしないように
			mStage->BreakGround(transform_.position_.x, transform_.position_.y + CHIP_SIZE / 1.5);
			Explosion* mEx = Instantiate<Explosion>(GetParent());
			mEx->SetPosition(transform_.position_.x, transform_.position_.y);
		}
	}

	//新・移動処理(採用)
	moveSpeed_ += gravity_;
	if (moveType_ == 0) {//左向き
		transform_.position_.x -= moveSpeed_ * Time::DeltaTime();
		transform_.position_.y += moveSpeed_ * 0.75 * Time::DeltaTime();
	}
	else if (moveType_ == 1) {//右向き
		transform_.position_.x += moveSpeed_ * Time::DeltaTime();
		transform_.position_.y += moveSpeed_ * 0.75 * Time::DeltaTime();
	}
	else {//下向き
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
		y -= cam->GetValueY();
	}
	DrawRectGraph(x, y, 0, 0, CHIP_SIZE, CHIP_SIZE, mImage_, TRUE);

	//当たり判定を可視化するため用
	switch (moveType_)//隕石の種類によって分ける
	{
	case 0://左向き
		DrawCircle(x + CHIP_SIZE / 4, y + CHIP_SIZE / 2, 24.0f, GetColor(0, 0, 255), FALSE);
		break;
	case 1://右向き
		DrawCircle(x + CHIP_SIZE / 4 * 3, y + CHIP_SIZE / 2, 24.0f, GetColor(0, 0, 255), FALSE);
		break;
	case 2://下向き
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
	//x,y,rが相手の円の情報
	//自分の円の情報
	float myCenterX = 0.0f;
	float myCenterY = 0.0f;
	switch (moveType_) {//隕石の種類によって分ける
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
	//新・画像読み込み
	if (moveType_ == 0) {//左向きの画像
		mImage_ = LoadGraph("Assets/Image/meteo0.png");
		assert(mImage_ > 0);
	}
	else if (moveType_ == 1) {//右向きの画像
		mImage_ = LoadGraph("Assets/Image/meteo1.png");
		assert(mImage_ > 0);
	}
	else {//下向きの画像
		mImage_ = LoadGraph("Assets/Image/meteo2.png");
		assert(mImage_ > 0);
	}
}
