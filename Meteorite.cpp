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
	:GameObject(parent,"Meteorite"),mImage_(-1),gravity_(INIT_GRAVITY),moveSpeed_(MOVE_SPEED)
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
	mImage_ = LoadGraph("Assets/Image/meteo.png");
	assert(mImage_ > 0);
	transform_.position_ = INIT_POS;
}

void Meteorite::Update()
{
	Stage* mStage = GetParent()->FindGameObject<Stage>();

	//スクロールに合わせて動くように
	int x = (int)transform_.position_.x;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValue();
	}

	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->canMove())//canMoveなら動かす
		return;

	if (x > SCREEN_WIDTH)//画面外(右側)にいるならなにもしない
		return;
	else if (x < 0 - CHIP_SIZE) {//マップ外に出たらさよなら
		KillMe();
		return;
	}

	bool isGround = mStage->CollisionDown(transform_.position_.x + CHIP_SIZE / 4.0, transform_.position_.y+ CHIP_SIZE/1.5);
	if (isGround) {//地面などのステージの一部に当たったらさようなら
		KillMe();
		mStage->BreakGround(transform_.position_.x, transform_.position_.y + CHIP_SIZE / 1.5);
		Explosion*mEx = Instantiate<Explosion>(GetParent());
		mEx->SetPosition(transform_.position_.x, transform_.position_.y);
	}

	//移動の処理
	moveSpeed_ += gravity_;
	/*transform_.position_.x -= MOVE_SPEED * Time::DeltaTime();
	transform_.position_.y += MOVE_SPEED * Time::DeltaTime();*/
	transform_.position_.x -= moveSpeed_ * Time::DeltaTime();
	transform_.position_.y += moveSpeed_ * Time::DeltaTime();
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

	//当たり判定を可視化するため用
	//DrawCircle(x + CHIP_SIZE/4, y + CHIP_SIZE/2, 24.0f, GetColor(0, 0, 255), FALSE);
	//DrawPixel(transform_.position_.x, transform_.position_.y + CHIP_SIZE /1.5, GetColor(0, 0, 0));
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
	float myCenterX = transform_.position_.x + (float)CHIP_SIZE / 4;
	float myCenterY = transform_.position_.y + (float)CHIP_SIZE / 2;
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
