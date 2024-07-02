#include "Player.h"
#include "Engine/time.h"
#include "Camera.h"
#include "Stage.h"
#include "Meteorite.h"
#include "PlayScene.h"
#include "Explosion.h"
#include "AttackSkill.h"

namespace {
	const float CHIP_SIZE = 64.0f;//キャラの画像サイズ
	const XMFLOAT3 INIT_POS = { 30,575,0 };//最初の位置
	const float JUMP_HEIGHT = 64.0f * 3.0f;//ジャンプの高さ
	const float INIT_GRAVITY = 9.8/ 90.0f;
	const float MAX_POS = 400;
	const int SPEED = 150;
	//重力メモ:月...1.62,火星...3.71,太陽274

}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),pImage_(-1),gravity_(INIT_GRAVITY),
	                             jumpSpeed_(0.0f), onGround_(true),
	                             time_(0.0f),animType_(0),animFrame_(0),
	                             prevAttackKey_(false)
{
	//初期位置の調整
	transform_.position_ = INIT_POS;
	state_ = S_Normal;
}

Player::~Player()
{
	if (pImage_ > 0) {
		DeleteGraph(pImage_);
		pImage_ = -1;
	}
}

void Player::Initialize()
{
	//とりあえず今はaoiを出しとく(後で変える)
	pImage_ = LoadGraph("Assets\\Image\\aoi.png");
	assert(pImage_ >= 0); 
}

void Player::Update()
{
	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->canMove())
		return;

	switch (state_){
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
		int pushL = pStage->CollisionDown(transform_.position_.x + 14, transform_.position_.y + CHIP_SIZE);
		int push = max(pushR, pushL);//２つの足元のめり込みの大きい方
		if (push >= 1) {
			transform_.position_.y -= push - 1;
			jumpSpeed_ = 0.0f;
			onGround_ = true;
		}
		else {
			onGround_ = false;
		}

		pushR = pStage->CollisionUp(transform_.position_.x + 50, transform_.position_.y);
		pushL = pStage->CollisionUp(transform_.position_.x + 14, transform_.position_.y);
		push = max(pushR, pushL);//２つの足元のめり込みの大きい方
		if (push >= 1) {
			transform_.position_.y += push + 1;
			jumpSpeed_ = 0.0f;
		}
	}

	std::list<Meteorite*> pMeteos = GetParent()->FindGameObjects<Meteorite>();
	for (Meteorite* pMeteo : pMeteos) {
		if (pMeteo->CollideCircle(transform_.position_.x + CHIP_SIZE/2, 
			                      transform_.position_.y + CHIP_SIZE/2, 20.0f)) {
			pMeteo->KillMe();
			Explosion* pEx = Instantiate<Explosion>(GetParent());
			pEx->SetPosition(transform_.position_.x, transform_.position_.y - CHIP_SIZE / 2);
			KillMe();
			scene->StartGameOver();
		}
	}

	//ここでカメラ位置の調整
	Camera* cam = GetParent()->FindGameObject<Camera>();
	int x = (int)transform_.position_.x - cam->GetValue();
	if (x > MAX_POS) {
		x = MAX_POS;
		cam->SetValue((int)transform_.position_.x - x);
	}
	if (x < 0) {
		x = 0;
		cam->SetValue((int)transform_.position_.x + x);
	}
}

void Player::UpdateNormal()
{
	animType_ = 0;//歩くモーション
	Stage* pStage = GetParent()->FindGameObject<Stage>();

	//移動量とその初期化
	float moveX, moveY;
	moveX = 0.0f;
	moveY = 0.0f;

	if (CheckHitKey(KEY_INPUT_D)) {//Dキーを押すと右に進む
		moveX += SPEED * Time::DeltaTime();
		transform_.position_.x += moveX;//移動量
		if (time_ > 0.2f) {
			animFrame_ = (animFrame_ + 1) % 4;
			time_ = 0.0f;
		}
		int hitX = transform_.position_.x + 50;
		int hitY = transform_.position_.y + 63;
		if (pStage != nullptr) {
			int push = pStage->CollisionRight(hitX, hitY);
			transform_.position_.x -= push;
		}
	}
	else if (CheckHitKey(KEY_INPUT_A)) {//Aキーを押すと左に進む
		if (transform_.position_.x > 0) {//左画面端で止まるように
			moveX += SPEED * Time::DeltaTime();//移動量
			transform_.position_.x -= moveX;
			if (time_ > 0.2f) {
				animFrame_ = (animFrame_ + 1) % 4;
				time_ = 0.0f;
			}
			int hitX = transform_.position_.x + 10;
			int hitY = transform_.position_.y + 63;
			if (pStage != nullptr) {
				int push = pStage->CollisionLeft(hitX, hitY);
				transform_.position_.x += push;
			}
		}
	}
	else {
		animFrame_ = 0;
		frameCounter_ = 0;
	}

	if (onGround_) {//地面にいるか
		if (CheckHitKey(KEY_INPUT_SPACE)) {//SPACEキーを押すとジャンプ
			jumpSpeed_ = -sqrtf(2 * (gravity_)*JUMP_HEIGHT);
			onGround_ = false;//地面にいない
		}
	}

	jumpSpeed_ += gravity_;//速度 += 重力
	transform_.position_.y += jumpSpeed_; //座標 += 速度

	if (CheckHitKey(KEY_INPUT_E)) {
		if (onGround_) {
			if (prevAttackKey_ == false) {
				time_ = 0.0f;
				state_ = S_Attack;
			}
			prevAttackKey_ = true;
		}
	}
	else {
		prevAttackKey_ = false;
	}
}

void Player::UpdateNormal2()
{
	animType_ = 0;//歩くモーション
	Stage* pStage = GetParent()->FindGameObject<Stage>();

	//移動量とその初期化
	float moveX, moveY;
	moveX = 0.0f;
	moveY = 0.0f;

	if (CheckHitKey(KEY_INPUT_D)) {//Dキーを押すと右に進む
		moveX += SPEED * Time::DeltaTime();
	}
	else if (CheckHitKey(KEY_INPUT_A)) {//Aキーを押すと左に進む
		if (transform_.position_.x > 0) {//左画面端で止まるように
			moveX -= SPEED * Time::DeltaTime();
		}
	}
	else {
		animFrame_ = 0;
		frameCounter_ = 0;
	}

	if (onGround_) {//地面にいるときだけ
		if (CheckHitKey(KEY_INPUT_SPACE)) {//SPACEキーを押すとジャンプ
			jumpSpeed_ = -sqrtf(2 * (gravity_)*JUMP_HEIGHT);
			onGround_ = false;
		}
	}
	//落下処理
	jumpSpeed_ += gravity_;

	//落下速度に移動量を加える
	moveY = jumpSpeed_;

	//移動量をもとにキャラクタの座標を移動
	CharMove(&transform_.position_.x, &transform_.position_.y,
		&jumpSpeed_, moveX, moveY, &onGround_);

	if (CheckHitKey(KEY_INPUT_E)) {
		if (onGround_) {
			if (prevAttackKey_ == false) {
				time_ = 0.0f;
				state_ = S_Attack;
			}
			prevAttackKey_ = true;
		}
	}
	else {
		prevAttackKey_ = false;
	}
}

int Player::CharMove(float* _x, float* _y, float *_downSP, 
	float _moveX, float _moveY, bool* _jumpFlag)
{
	Stage pStage = GetParent()->FindGameObject<Stage>();
	float Dummy = 0.0F;

	// キャラクタの左上、右上、左下、右下部分が当たり判定のある
	// マップに衝突しているか調べ、衝突していたら補正する

	// 先ず上下移動成分だけでチェック
	// 左下のチェック、もしブロックの上辺に着いていたら落下を止める
	if (pStage.StageHitCheck(*_x, *_y + CHIP_SIZE, &Dummy, &_moveY) == 3)
		*_downSP = 0.0F;

	// 右下のチェック、もしブロックの上辺に着いていたら落下を止める
	if (pStage.StageHitCheck(*_x + CHIP_SIZE, *_y + CHIP_SIZE, &Dummy, &_moveY) == 3)
		*_downSP = 0.0F;

	// 左上のチェック、もしブロックの下辺に当たっていたら落下させる
	if (pStage.StageHitCheck(*_x, *_y, &Dummy, &_moveY) == 4)
		*_downSP *= -1.0F;

	// 右上のチェック、もしブロックの下辺に当たっていたら落下させる
	if (pStage.StageHitCheck(*_x + CHIP_SIZE, *_y, &Dummy, &_moveY) == 4)
		*_downSP *= -1.0F;

	// 上下移動成分を加算
	*_y += _moveY;

	// 後に左右移動成分だけでチェック
	// 左下のチェック
    pStage.StageHitCheck(*_x, *_y + CHIP_SIZE, &_moveX, &Dummy);

	// 右下のチェック
	pStage.StageHitCheck(*_x + CHIP_SIZE, *_y + CHIP_SIZE, &_moveX, &Dummy);

	// 左上のチェック
	pStage.StageHitCheck(*_x, *_y, &_moveX, &Dummy);

	// 右上のチェック
	pStage.StageHitCheck(*_x + CHIP_SIZE, *_y - CHIP_SIZE, &_moveX, &Dummy);

		// 左右移動成分を加算
		*_x += _moveX;

	// 接地判定
	// キャラクタの左下と右下の下に地面があるか調べる
	if (pStage.GetChipParam(*_x - CHIP_SIZE * 0.5F, *_y + CHIP_SIZE * 0.5F + 1.0F) == 0 &&
		pStage.GetChipParam(*_y + CHIP_SIZE * 0.5F, *_y + CHIP_SIZE * 0.5F + 1.0F) == 0)
	{
		// 足場が無かったらジャンプ中にする
		*_jumpFlag = TRUE;
	}
		else
		{
			// 足場が在ったら接地中にする
			*_jumpFlag = FALSE;
		}

	// 終了
	return 0;
}

void Player::UpdateAttack()
{
	animType_ = 1;//仮だけど攻撃モーション
	if (animFrame_ + 1 == 3)
	{
		AttackSkill* attack = Instantiate<AttackSkill>(GetParent());
		int x = (int)transform_.position_.x;
		int y = (int)transform_.position_.y;
		attack->SetPosition(x + CHIP_SIZE, y);
		time_ = 0.0f;
		state_ = S_Normal;
	}
	if (time_ > 0.3f) {
		animFrame_ = (animFrame_ + 1) % 3;
		time_ = 0.0f;
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
	DrawRectGraph(x, y, animFrame_ * CHIP_SIZE, animType_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE, pImage_, TRUE);
	//当たり判定を見るよう
	DrawCircle(x + CHIP_SIZE/2, y + CHIP_SIZE / 2, 20.0f, GetColor(0, 0, 255), FALSE);
}

void Player::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
	ground_ = _y;
}

void Player::SetGravity(float _gravity)
{
	gravity_ = _gravity;
}