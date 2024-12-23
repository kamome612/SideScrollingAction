#include "Enemy.h"
#include "Engine/time.h"
#include "Camera.h"
#include "Stage.h"
#include "Meteorite.h"
#include "PlayScene.h"
#include "Explosion.h"
#include "AttackSkill.h"
#include "Flag.h"


namespace {
	const float CHIP_SIZE = 64.0f;//キャラの画像サイズ
	static const int SCREEN_WIDTH = 1280;
	const XMFLOAT3 INIT_POS = { 200,575,0 };//最初の位置
	const float JUMP_HEIGHT = 64.0f * 3.0f;//ジャンプの高さ
	const float INIT_GRAVITY = 9.8;
	const float MAX_POS = 400;
	const int SPEED = 250;
	//重力メモ:月...1.62,火星...3.71
}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent, "Enemy"), eImage_(-1), gravity_(INIT_GRAVITY),
	jumpSpeed_(0.0f), onGround_(true),
	time_(0.0f), animType_(0), animFrame_(0),moveDirection_(-1),turn_(false)
{
	//初期位置の調整
	transform_.position_ = INIT_POS;
	state_ = S_Normal;
}

Enemy::~Enemy()
{
	if (eImage_ > 0) {
		DeleteGraph(eImage_);
		eImage_ = -1;
	}
}

void Enemy::Initialize()
{
	eImage_ = LoadGraph("Assets\\Image\\Enemy.png");
	assert(eImage_ >= 0);
	transform_.position_ = INIT_POS;
}

void Enemy::Update()
{
	Stage* mStage = GetParent()->FindGameObject<Stage>();

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

	if (x > SCREEN_WIDTH)//画面外(右側)にいるならなにもしない
		return;
	//else if (x < 0 - CHIP_SIZE) {//マップ外に出たらさよなら
	//	KillMe();
	//	return;
	//}

	switch (state_) {
	case 0:
		UpdateNormal();
		break;
	case 1:
		UpdateAttack();
		break;
	default:
		break;
	}

	//アニメーションに使うタイムの更新
	time_ += Time::DeltaTime();

	Stage* pStage = GetParent()->FindGameObject<Stage>();
	if (pStage != nullptr) {
		int pushR = pStage->CollisionDown(transform_.position_.x + 50, transform_.position_.y + CHIP_SIZE);
		int pushL = pStage->CollisionDown(transform_.position_.x +24, transform_.position_.y + CHIP_SIZE);
		int push = max(pushR, pushL);//２つの足元のめり込みの大きい方
		if (push >= 1) {
			transform_.position_.y -= push - 1;
			jumpSpeed_ = 0.0f;
			onGround_ = true;
			//turn_ = false;
		}
		else {
			onGround_ = false;
		}

		pushR = pStage->CollisionUp(transform_.position_.x + 50, transform_.position_.y);
		pushL = pStage->CollisionUp(transform_.position_.x +24, transform_.position_.y);
		push = max(pushR, pushL);//２つの足元のめり込みの大きい方
		if (push >= 1) {
			transform_.position_.y += push + 1;
			jumpSpeed_ = 0.0f;
		}
	}
	//画面外に落ちたら消える
	if (transform_.position_.y >= 730) {
		KillMe();
	}
}

void Enemy::UpdateNormal()
{
	Stage* pStage = GetParent()->FindGameObject<Stage>();

	//if (turn_ == true) {
	//	moveDirection_ *= -1;
	//	animFrame_ = 1;
	//}
	//else {
	//	moveDirection_ *= -1;
	//	animFrame_ = 0;
	//}

	//移動量とその初期化
	float moveX, moveY;
	moveX = 0.0f;
	moveY = 0.0f;


	if (transform_.position_.x < 0) {//左画面端で止まるように
		KillMe();
	}
	
	moveX = SPEED * Time::DeltaTime()*moveDirection_;
	transform_.position_.x += moveX;

	//左の当たり判定と左右反転
	int LhitX = transform_.position_.x+24 ;
	int LhitY = transform_.position_.y + CHIP_SIZE / 2;
	if (pStage != nullptr) {
		int pushL = pStage->CollisionLeft(LhitX, LhitY);
		transform_.position_.x += pushL;
		if (pStage->CollisionLeft(LhitX, LhitY)) {
			moveDirection_ = 1;
			turn_ = true;
		}
		
	}

	//右の当たり判定と左右反転
	int RhitX = transform_.position_.x +50;
	int RhitY = transform_.position_.y + CHIP_SIZE / 2;
	if (pStage != nullptr) {
		int pushR = pStage->CollisionRight(RhitX, RhitY);
		transform_.position_.x -= pushR;
		if (pStage->CollisionRight(RhitX, RhitY)) {
			moveDirection_ = -1;
			turn_ = false;
		}

	}
	
	//左右のアニメーション
	if (time_ > 0.2f) {
		if (turn_ == false) {
			animType_ = 0;
			//moveDirection_ = -1;
			animFrame_ = animFrame_ % 6 + 1;
			time_ = 0.0f;
			
		}
		if (turn_ == true) {
			animType_ = 1;
			//moveDirection_ = 1;
			animFrame_ = animFrame_ % 6 + 1;
			time_ = 0.0f;
			
		}
	}
	
	jumpSpeed_ += gravity_ * 1.6 * Time::DeltaTime();//速度 += 重力
	transform_.position_.y += jumpSpeed_; //座標 += 速度

}

void Enemy::UpdateAttack()
{
}

void Enemy::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		//y -= cam->GetValueY();
	}
	DrawRectGraph(x, y, animFrame_ * CHIP_SIZE, animType_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE, eImage_, TRUE);

	//DrawCircle(x + CHIP_SIZE / 2, y + CHIP_SIZE / 2, 12.0f, GetColor(0, 0, 255), FALSE);

}

void Enemy::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
	ground_ = _y;
}

void Enemy::SetGravity(float _gravity)
{
	gravity_ = _gravity;
}

bool Enemy::CollideCircle(float x, float y, float r)
{
	//x,y,rが相手の円の情報
	//自分の円の情報
	float myCenterX = transform_.position_.x + (float)CHIP_SIZE / 2;
	float myCenterY = transform_.position_.y + (float)CHIP_SIZE / 2;
	float myR = 12.0f;
	float dx = myCenterX - x;
	float dy = myCenterY - y;
	if ((dx * dx + dy * dy) < ((r + myR) * (r + myR))) {
		return true;
	}
	else {
		return false;
	}
}