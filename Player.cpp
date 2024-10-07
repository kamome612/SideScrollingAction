#include "Player.h"
#include "Engine/time.h"
#include "Camera.h"
#include "Stage.h"
#include "Meteorite.h"
#include "PlayScene.h"
#include "Explosion.h"
#include "AttackSkill.h"
#include "Flag.h"
#include "Enemy.h"

namespace {
	const float CHIP_SIZE = 64.0f;//キャラの画像サイズ
	//const float CHIP_SIZE = 60.0f;//キャラの画像サイズ
	const int MAP_HEIGHT = 720; //高さ
	const float ROBO_WIDTH = 48;
	const XMFLOAT3 INIT_POS = { 30,580,0 };//最初の位置
	const float JUMP_HEIGHT = 64.0f * 3.0f;//ジャンプの高さ
	const float INIT_GRAVITY = 9.8/ 90.0f;
	const float MAX_POS = 400;
	const int SPEED = 150;
	//const int SPEED = 500;
	const int MARGIN = 14;//プレイヤーのチップの余白
	const int LIFE_IMAGE_SIZE = 64;//体力画像サイズ
	const float FINV_TIME = 1.0f;//無敵が終わる時間
	//重力メモ:月...1.62,火星...3.71
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),pImage_(-1),gravity_(INIT_GRAVITY),
	                             jumpSpeed_(0.0f), onGround_(true),
	                             time_(0.0f),animType_(0),animFrame_(0),
	                             prevAttackKey_(false), pLife_(3), invTime_(0), hitFlag_(false)
{
	//初期位置の調整
	transform_.position_ = INIT_POS;
	state_ = S_Normal;
}

Player::~Player()
{
	//ちゃんと消す
	if (pImage_ > 0) {
		DeleteGraph(pImage_);
		pImage_ = -1;
	}
}

void Player::Initialize()
{
	//プレイヤーの画像の読み込み
	//pImage_ = LoadGraph("Assets\\Image\\robot.png");
	pImage_ = LoadGraph("Assets\\Image\\cyborg.png");
	assert(pImage_ >= 0); 

	hImage_ = LoadGraph("Assets\\Image\\Life.png");
	assert(hImage_ > 0);
}

void Player::Update()
{

	//プレイシーンから動いていいのかどうかを確認する
	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->canMove())
		return;

	//ちょっとプレイヤーの座標を確認したいとき用
	if (CheckHitKey(KEY_INPUT_O)) {
		int tmp = (int)transform_.position_.x;
		int tmp2 = (int)transform_.position_.y;
		printfDx("(%d,%d)", tmp,tmp2);
	}

	//ステートを使って普通の状態と攻撃の状態などを呼び分ける
	switch (state_){
	case 0: //ノーマル
		UpdateNormal();
		break;
	case 1: //歩いたり、ジャンプ
		UpdateMove();
		break;
	case 2: //アタック
		UpdateAttack();
		break;
	case 3: //死ぬ
		UpdateDie();
		break;
	default:
		break;
	}

	//アニメーションに使うタイムの更新
	time_ += Time::DeltaTime();

	//ステージとの上下の当たり判定
	Stage* pStage = GetParent()->FindGameObject<Stage>();
	if (pStage != nullptr) {
		int pushR = pStage->CollisionDown(transform_.position_.x + CHIP_SIZE - MARGIN, transform_.position_.y + CHIP_SIZE);
		int pushL = pStage->CollisionDown(transform_.position_.x + MARGIN, transform_.position_.y + CHIP_SIZE);
		int push = max(pushR, pushL);//２つの足元のめり込みの大きい方
		if (push >= 1) {
			transform_.position_.y -= push - 1;
			jumpSpeed_ = 0.0f;
			onGround_ = true;
		}
		else {
			onGround_ = false;
		}

		pushR = pStage->CollisionUp(transform_.position_.x + CHIP_SIZE -MARGIN, transform_.position_.y);
		pushL = pStage->CollisionUp(transform_.position_.x + MARGIN, transform_.position_.y);
		push = max(pushR, pushL);//２つの足元のめり込みの大きい方
		if (push >= 1) {
			transform_.position_.y += push + 1;
			jumpSpeed_ = 0.0f;
		}
	}

	//隕石との当たり判定
	std::list<Meteorite*> pMeteos = GetParent()->FindGameObjects<Meteorite>();
	for (Meteorite* pMeteo : pMeteos) {
		if (pMeteo->CollideCircle(transform_.position_.x + CHIP_SIZE/4, 
			                      transform_.position_.y + CHIP_SIZE/2, 20.0f)) {
			pMeteo->KillMe();
			Explosion* pEx = Instantiate<Explosion>(GetParent());
			pEx->SetPosition(transform_.position_.x, transform_.position_.y - CHIP_SIZE / 2);
			animFrame_ = 0;
			state_ = S_Die;
			//KillMe();
			//scene->StartGameOver();
		}
	}

	//敵との当たり判定
	std::list<Enemy*> pEnemys = GetParent()->FindGameObjects<Enemy>();
	for (Enemy* pEnemy : pEnemys) {
		if (hitFlag_ == false) {
			if (pEnemy->CollideCircle(transform_.position_.x + CHIP_SIZE / 4,
				transform_.position_.y + CHIP_SIZE / 2, 20.0f)) {
				//Explosion* pEx = Instantiate<Explosion>(GetParent());
				//pEx->SetPosition(transform_.position_.x, transform_.position_.y - CHIP_SIZE / 2);
				//state_ = S_Die;
				pLife_ -= 1;
				hitFlag_ = true;
			}
		}
	}

	if (hitFlag_ == true) {//敵が当たったら少しの間無敵になる
		invTime_ += Time::DeltaTime();
		if (invTime_ >= FINV_TIME) {
			hitFlag_ = false;
			invTime_ = 0;
		}
	}

	if (pLife_ == 0)//ライフが０になったら死ぬ
	{
		state_ = S_Die;
	}

	//ゴールの旗との当たり判定
	Flag* pFlag = GetParent()->FindGameObject<Flag>();
	if (pFlag->HitFlag(transform_.position_.x +CHIP_SIZE / 2,
		               transform_.position_.y + CHIP_SIZE / 2,20.0f)) {
		scene->StartClear();
	}

	//画面外(下)に落ちたらゲームオーバ
	if (transform_.position_.y >= MAP_HEIGHT) {
		KillMe();
		scene->StartGameOver();
	}

	//画面外(左右)に行かないように
	if (transform_.position_.x < 0) {
		transform_.position_.x = 0;
	}
	if (transform_.position_.x > 4600) {
		transform_.position_.x = 4600;
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
	if (x > 3800) {
		x = 3800;
		cam->SetValue((int)transform_.position_.x - x);
	}
}

void Player::UpdateNormal()
{
	if (hitFlag_) {//無敵の時は点滅
		animType_ = 5;
	}
	else {
		animType_ = 0;//何もしてないときIdle状態
	}

	if (time_ > 0.2f) {
		if (onGround_) {
			animFrame_ = animFrame_ % 3 + 1;
			time_ = 0.0f;
		}
	}

	if (CheckHitKey(KEY_INPUT_D) || CheckHitKey(KEY_INPUT_A)
		|| CheckHitKey(KEY_INPUT_SPACE)) {//S_MOVEにする
		state_ = S_Move;
	}

	//ミサイルを飛ばしての攻撃
	if (CheckHitKey(KEY_INPUT_E)) {//横に飛ばす
		if (prevAttackKey_ == false) {
			animFrame_ = 0;
			time_ = 0.0f;
			isTypeA = true;
			state_ = S_Attack;//攻撃の状態に移る
		}
		prevAttackKey_ = true;
	}
	else {
		prevAttackKey_ = false;
		isTypeA = false;
	}

	if (CheckHitKey(KEY_INPUT_R)) {//斜め前に飛ばす
		if (prevAttackKey_ == false) {
			animFrame_ = 0;
			time_ = 0.0f;
			isTypeB = true;
			state_ = S_Attack;//攻撃の状態に移る
		}
		prevAttackKey_ = true;

	}
	else {
		prevAttackKey_ = false;
		isTypeB = false;
	}
}

void Player::UpdateMove()
{
	if (hitFlag_) {//無敵の時は点滅
		animType_ = 6;
	}
	else {
		animType_ = 1;//歩くモーション
	}
	Stage* pStage = GetParent()->FindGameObject<Stage>();

	//移動量とその初期化
	float moveX, moveY;
	moveX = 0.0f;
	moveY = 0.0f;

	//コントローラ操作(なぜかスティックを左に倒しても左にいかない)
	//int x, y;
	//GetJoypadAnalogInput(&x, &y, DX_INPUT_PAD1);
	//moveX += (float)x / 1000.0f;
	//if (moveX < 0.3f) {//中心付近の誤差をのぞく
	//	 moveX = 0.0f;
	//}

	//移動
	if (CheckHitKey(KEY_INPUT_D)) {//Dキーを押すと右に進む
		moveX += SPEED * Time::DeltaTime();
	}
	else if (CheckHitKey(KEY_INPUT_A)) {//Aキーを押すと左に進む
		moveX -= SPEED * Time::DeltaTime();
	}
	else {
		time_ = 0;
		animFrame_ = 0;
		frameCounter_ = 0;
		if (onGround_) {
			state_ = S_Normal;
		}
	}

	transform_.position_.x += moveX;//移動量

	if (time_ > 0.15f) {
		if (onGround_) {
			animFrame_ = animFrame_ % 5 + 1;
			time_ = 0.0f;
		}
	}

	//プレイヤーの右側のステージとの当たり判定
	int hitX = transform_.position_.x + (CHIP_SIZE - MARGIN);//ブロックとプレイヤーの余白をなくすために引く
	int hitY = transform_.position_.y + CHIP_SIZE - 1; //そのまま足すと落ちていくから-1
	if (pStage != nullptr) {
		int push = pStage->CollisionRight(hitX, hitY);
		transform_.position_.x -= push;
	}
	//プレイヤーの左側のステージとの当たり判定
	hitX = transform_.position_.x + MARGIN;//ブロックとプレイヤーの余白をなくすために足す(なぜかこれだと後ろ下がり続けるとがたがたする)
	hitY = transform_.position_.y + CHIP_SIZE - 1;//そのまま足すと落ちていくから-1
	if (pStage != nullptr) {
		int push = pStage->CollisionLeft(hitX, hitY);
		transform_.position_.x += push;
	}

	//変更前の移動
	//if (CheckHitKey(KEY_INPUT_D)) {//Dキーを押すと右に進む
	//	moveX += SPEED * Time::DeltaTime();
	//	transform_.position_.x += moveX;//移動量
	//	if (time_ > 0.2f) {
	//		if (onGround_) {
	//			animFrame_ = animFrame_ % 8 + 1;
	//			time_ = 0.0f;
	//		}
	//	}
	//	int hitX = transform_.position_.x + 50;
	//	int hitY = transform_.position_.y + CHIP_SIZE -1;
	//	if (pStage != nullptr) {
	//		int push = pStage->CollisionRight(hitX, hitY);
	//		transform_.position_.x -= push;
	//	}
	//}
	//else if (CheckHitKey(KEY_INPUT_A)) {//Aキーを押すと左に進む
	//	if (transform_.position_.x > 0) {//左画面端で止まるように
	//		moveX -= SPEED * Time::DeltaTime();//移動量
	//		transform_.position_.x += moveX;
	//		if (time_ > 0.2f) {
	//			if (onGround_) {
	//				animFrame_ = animFrame_ % 8 + 1;
	//				time_ = 0.0f;
	//			}
	//		}
	//		int hitX = transform_.position_.x + 10;
	//		int hitY = transform_.position_.y + CHIP_SIZE-1;
	//		if (pStage != nullptr) {
	//			int push = pStage->CollisionLeft(hitX, hitY);
	//			transform_.position_.x += push;
	//		}
	//	}
	//}
	//else {
	//	animFrame_ = 0;
	//	frameCounter_ = 0;
	//}

	if (onGround_) {//地面にいるか
		if (CheckHitKey(KEY_INPUT_SPACE)) {//SPACEキーを押すとジャンプ
			jumpSpeed_ = -sqrtf(2 * (gravity_)*JUMP_HEIGHT);
			onGround_ = false;//地面にいない
		}
	}

	//ジャンプの時と降りてる時のアニメーションの変更
	if (!onGround_ && jumpSpeed_ < 0) {
		/*animType_ = 2;
		animFrame_ = 2;*/
		animType_ = 2;
		animFrame_ = 0;
		if (time_ > 0.5) {
			animFrame_ = 1;
		}
	}
	else if (!onGround_ && jumpSpeed_ > 0) {
		/*animType_ = 2;
		animFrame_ = 1;*/
		animType_ = 2;
		animFrame_ = 2;
		if (time_ > 0.5) {
			animFrame_ = 3;
		}
	}

	jumpSpeed_ += gravity_;//速度 += 重力
	transform_.position_.y += jumpSpeed_; //座標 += 速度

	//ミサイルを飛ばしての攻撃
	if (CheckHitKey(KEY_INPUT_E)) {
		if (prevAttackKey_ == false) {
			animFrame_ = 0;
			time_ = 0.0f;
			state_ = S_Attack;//攻撃の状態に移る
		}
		prevAttackKey_ = true;
	}
	else {
		prevAttackKey_ = false;
	}
}

void Player::UpdateAttack()
{
	
	if (hitFlag_) {//無敵の時は点滅
		animType_ = 7;
	}
	else {
		animType_ = 3;//攻撃状態
	}

	if (animFrame_ + 1 == 5)
	{
		AttackSkill* attack = Instantiate<AttackSkill>(GetParent());
		int x = (int)transform_.position_.x;
		int y = (int)transform_.position_.y;
		attack->SetPosition(x + CHIP_SIZE, y);

		//横に撃つか、斜めに撃つか
		if (isTypeA == true) {
			attack->SetAngle(0);

		}
		if (isTypeB == true) {
			attack->SetAngle(XM_PI / -4);//角度を設定
		}

		time_ = 0.0f;
		animFrame_ = 0;
		if (onGround_) {
			state_ = S_Normal;
		}
		else {
			state_ = S_Move;
		}
	}

	if (time_ > 0.1f) {
		/*animFrame_ = (animFrame_ + 1) % 6;
		time_ = 0.0f;*/
		animFrame_ = animFrame_ % 5 + 1;
		time_ = 0.0f;
	}
}

void Player::UpdateDie()
{
	animType_ = 4;
	if (animFrame_ + 1 == 6) {
		KillMe();
		animFrame_ = 0;
		PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
		scene->StartGameOver();
	}
	if (time_ > 0.2f) {
		animFrame_ = animFrame_ % 5 + 1;
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
	//DrawRectGraph(x, y, animFrame_ * CHIP_SIZE, animType_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE, pImage_, TRUE);
	DrawRectGraph(x, y, animFrame_ * CHIP_SIZE, animType_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE, pImage_, TRUE);
	//当たり判定を見るよう
	//DrawCircle(x + CHIP_SIZE / 4, y + CHIP_SIZE / 2, 20.0f, GetColor(0, 0, 255), FALSE);
	//DrawCircle(x + CHIP_SIZE/2, y + CHIP_SIZE / 2, 20.0f, GetColor(0, 0, 255), FALSE);
	for (int i = 0; i < pLife_; i++) {
		DrawGraph(LIFE_IMAGE_SIZE * i, 0, hImage_, TRUE);
	}
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