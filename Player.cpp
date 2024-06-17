#include "Player.h"
#include "Engine/time.h"
#include "Camera.h"
#include "Stage.h"

namespace {
	const XMFLOAT3 INITPOS = { 30,575,0 };//最初の位置
	const float JUMP_HEIGHT = 64.0f * 4.0f;//ジャンプの高さ
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),pImage_(-1),walkSpeed_(150),gravity_(9.8 / 60.0f),
	                             jumpSpeed_(0.0f), onGround_(true), prevSpaceKey_(false)
{
	//初期位置の調整
	transform_.position_ = INITPOS;
	state_ = S_Normal;
}

Player::Player(GameObject* parent, float _gravity)
	:GameObject(parent, "Player"), pImage_(-1), walkSpeed_(150), jumpSpeed_(0.0f),
	                               onGround_(true),prevSpaceKey_(false)
{
	// 初期位置の調整
    transform_.position_ = INITPOS;
	state_ = S_Normal;

	//ステージから受け取った重力のセット(後で受け渡し方は変えるかも)
	gravity_ = _gravity;
}

Player::~Player()
{
	if (pImage_ > 0)
		DeleteGraph(pImage_);
}

void Player::Initialize()
{
	//とりあえず今はaoiを出しとく(後で変える)
	pImage_ = LoadGraph("Assets\\Image\\chara.png");
	assert(pImage_ >= 0);
}

void Player::Update()
{
	switch (state_){
	case 0: 
		UpdateNormal();
		break;
	case 1:
		break;
	default:
		break;
	}
}

void Player::UpdateNormal()
{
	Stage* pStage = GetParent()->FindGameObject<Stage>();
	if (CheckHitKey(KEY_INPUT_D)) {//Dキーを押すと右に進む
		transform_.position_.x += walkSpeed_ * Time::DeltaTime();
		int hitX = transform_.position_.x + 50;
		int hitY = transform_.position_.y + 63;
		if (pStage != nullptr) {
			int push = pStage->CollisionRight(hitX, hitY);
			transform_.position_.x -= push;
		}
	}
	if (CheckHitKey(KEY_INPUT_A)) {//Aキーを押すと左に進む
		if (transform_.position_.x > 0) {//左画面端で止まるように
			transform_.position_.x -= walkSpeed_ * Time::DeltaTime();
			int hitX = transform_.position_.x - 50;
			int hitY = transform_.position_.y + 63;
			if (pStage != nullptr) {
				int push = pStage->CollisionLeft(hitX, hitY);
				transform_.position_.x -= push;
			}
		}
	}

	if (CheckHitKey(KEY_INPUT_SPACE)) {//SPASEキーを押すとジャンプ
		if (prevSpaceKey_ == false) {
			if (onGround_) {
				jumpSpeed_ = -sqrtf(2 * (gravity_)*JUMP_HEIGHT);
				onGround_ = false;
			}
		}
		prevSpaceKey_ = true;
	}
	else {
		prevSpaceKey_ = false;
	}
	jumpSpeed_ += gravity_;//速度 += 加速度
	transform_.position_.y += jumpSpeed_; //座標 += 速度

	if (pStage != nullptr) {
		//(50,64)と(14,64)も見る
		int pushR = pStage->CollisionDown(transform_.position_.x + 50, transform_.position_.y + 64);
		int pushL = pStage->CollisionDown(transform_.position_.x + 14, transform_.position_.y + 64);
		int push = max(pushR, pushL);//２つの足元のめり込みの大きい方
		if (push >= 1) {
			transform_.position_.y -= push - 1;
			jumpSpeed_ = 0.0f;
			onGround_ = true;
		}
		else {
			onGround_ = false;
		}
	}

	if (transform_.position_.y > INITPOS.y) {
		transform_.position_.y = INITPOS.y;
		onGround_ = true;
	}

	//ここでカメラ位置の調整
	Camera* cam = GetParent()->FindGameObject<Camera>();
	int x = (int)transform_.position_.x - cam->GetValue();
	if (x > 400) {
		x = 400;
		cam->SetValue((int)transform_.position_.x - x);
	}
	if (x < 0) {
		x = 0;
		cam->SetValue((int)transform_.position_.x + x);
	}
}

void Player::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValue();
	}
	DrawGraph(x, y, pImage_, TRUE);
}

void Player::SetPosition(int _x, int _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
}