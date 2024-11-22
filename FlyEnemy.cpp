#include "FlyEnemy.h"
#include "Engine/time.h"
#include "Camera.h"
#include "Stage.h"
#include "PlayScene.h"
#include "Explosion.h"
#include "AttackSkill.h"

namespace {
	const float CHIP_SIZE = 96.0f;//キャラの画像サイズ
	const float PLAYER_CHIP_SIZE = 64.0f;//Playerの画像サイズ
	static const int SCREEN_WIDTH = 1280;
	const XMFLOAT3 INIT_POS = { 0,0,0 };//最初の位置
	const float MAX_POS = 400;
	const int FLY_SPEED = 175;//飛ぶスピード
	const int FLY_HEIGHT =300;//飛ぶ高さ
	const float MOVE_INTERVAL = 2.0f;//上下入れ替え時間
	const int X_MARGIN = 24;//Xの余白
	const int Y_MARGIN = 32;//Yの余白
	const float SEARCH_RANGE = 300.0f;//索敵範囲
}

FlyEnemy::FlyEnemy(GameObject* parent) 
	:GameObject(parent,"FlyEnemy"), eImage_(-1),moveTime_(0), 
	moveDirection_(-1),time_(0.0f), onGround_(false),
	animType_(0),animFrame_(0),chasingPlayer_(false),
	chaseDelayTime_(0.0f),lastPlayerX_(0.0f),lastPlayerY_(0.0),
	underbrock_(false),prevPosX_(0.0f),prevPosY_(0.0f)
{
	//初期位置の調整
	transform_.position_ = INIT_POS;
	state_ = S_Normal;
}

FlyEnemy::~FlyEnemy()
{
	if (eImage_ > 0) {
		DeleteGraph(eImage_);
		eImage_ = -1;
	}
}

void FlyEnemy::Initialize()
{
	eImage_ = LoadGraph("Assets\\Image\\FlyEnemy3.png");
	assert(eImage_ >= 0);
	transform_.position_ = INIT_POS;
}

void FlyEnemy::Update()
{

	//スクロールに合わせて動くように
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		//y -= cam->GetValueY();
	}

	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->canMove())//canMoveなら動かす
		return;

	if (x > SCREEN_WIDTH)//画面外(右側)にいるならなにもしない
		return;

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

	//ステージとの上下の当たり判定
	Stage* pStage = GetParent()->FindGameObject<Stage>();
	if (pStage != nullptr) {
		int pushR = pStage->CollisionDown(transform_.position_.x + CHIP_SIZE / 2 - 10, transform_.position_.y + CHIP_SIZE);
		int pushL = pStage->CollisionDown(transform_.position_.x + 14, transform_.position_.y + CHIP_SIZE);
		int push = max(pushR, pushL);//２つの足元のめり込みの大きい方
		if (push >= 1) {
			transform_.position_.y -= push - 1;
			onGround_ = true;
		}

		pushR = pStage->CollisionUp(transform_.position_.x + CHIP_SIZE / 2 - 10, transform_.position_.y + CHIP_SIZE /2);
		pushL = pStage->CollisionUp(transform_.position_.x + 14, transform_.position_.y + CHIP_SIZE / 2);
		push = max(pushR, pushL);//２つの足元のめり込みの大きい方
		if (push >= 1) {
			transform_.position_.y += push + 1;
			underbrock_ = true;
		}
		else if(push == 0) {
			underbrock_ = false;
		}
	}

	//Stage* pStage = GetParent()->FindGameObject<Stage>();
	//新・右側のステージとの当たり判定
	int pushT, pushB, pushM, push;
	int hitX = transform_.position_.x + CHIP_SIZE / 2 - 10;
	int hitY = transform_.position_.y + CHIP_SIZE / 2 + 6;
	if (pStage != nullptr) {
		pushT = pStage->CollisionRight(hitX, hitY);//右側の当たり判定
		hitY = transform_.position_.y + CHIP_SIZE - 1;
		pushB = pStage->CollisionRight(hitX, hitY);
		hitY = transform_.position_.y + CHIP_SIZE / 4 * 3;
		pushM = pStage->CollisionRight(hitX, hitY);
		push = max(pushT, pushB);
		push = max(push, pushM);
		transform_.position_.x -= push;//のめりこんでる分戻す
	}
	//新・左側のステージとの当たり判定
	hitX = transform_.position_.x + 14;
	hitY = transform_.position_.y + CHIP_SIZE / 2 + 6;
	if (pStage != nullptr) {
		pushT = pStage->CollisionLeft(hitX, hitY);
		hitY = transform_.position_.y + CHIP_SIZE - 1;
		pushB = pStage->CollisionLeft(hitX, hitY);
		hitY = transform_.position_.y + CHIP_SIZE / 4 * 3;
		pushM = pStage->CollisionLeft(hitX, hitY);
		push = max(pushT, pushB);
		push = max(push, pushM);
		transform_.position_.x += push;//のめりこんでる分戻す
	}

	//前フレームの位置を持っておく
	prevPosX_ = transform_.position_.x;
	prevPosY_ = transform_.position_.y;
}

void FlyEnemy::UpdateNormal()
{

	animType_ = 0;//飛行モーション
	Stage* pStage = GetParent()->FindGameObject<Stage>();

	Player* player = GetParent()->FindGameObject<Player>();

	//上下入れ替え時間の計測と入れ替えの判定（地面に当たった時も入れ替える）
	moveTime_ += Time::DeltaTime();
	if (moveTime_ >= MOVE_INTERVAL || transform_.position_.y < 0) {
		moveTime_ = 0.0f;
		moveDirection_ *= -1;
	}

	//地面に当たったら上下入れ替え
	if (onGround_==true || underbrock_ == true) {

		moveTime_ = 0.0f;
		moveDirection_ *= -1;
		onGround_ = false;
	}

	//Playerが視界内に入ったらS_Attackに
	if (InRange(player)) {
		state_ = S_Attack;
	}

	//移動量とその初期化
	float moveX, moveY;
	moveX = 0.0f;
	moveY = 0.0f;

	//moveDirection_をかけて上下入れ替え
	//moveX = FLY_SPEED * Time::DeltaTime();//移動量
	moveY = FLY_SPEED * Time::DeltaTime()*moveDirection_;//移動量

	//transform_.position_.x -= moveX;
	transform_.position_.y -= moveY;

	if (transform_.position_.x < 0 - CHIP_SIZE) {//左画面端で消える
		KillMe();
	}

	//アニメーション
	time_ += Time::DeltaTime();
	if (time_ > 0.1f) {
		animFrame_ = animFrame_ % 5 + 1;
		time_ = 0.0f;
	}
}

void FlyEnemy::UpdateAttack()
{
	Player* player = GetParent()->FindGameObject<Player>();

	//if (player == nullptr) return;

	//// Playerの位置更新間隔
	//chaseDelayTime_ += Time::DeltaTime();
	//if (chaseDelayTime_ >= 0.5f) {  // 0.5秒ごとに位置を更新
	//	lastPlayerX_ = player->GetPosition().x + (float)PLAYER_CHIP_SIZE / 4;
	//	lastPlayerY_ = player->GetPosition().y + (float)PLAYER_CHIP_SIZE / 2;
	//	chaseDelayTime_ = 0.0f;  // 更新したらカウントをリセット
	//}

	////自分の中心位置
	//float myCenterX = transform_.position_.x + (float)CHIP_SIZE / 2 - X_MARGIN;
	//float myCenterY = transform_.position_.y + (float)CHIP_SIZE / 2 + Y_MARGIN;

	////Playerが視界内にいたら追尾
	//if (InRange(player)) {
	//	float moveX = 0.0f;
	//	float moveY = 0.0f;

	//	if (underbrock_ == true && transform_.position_.y == prevPosY_) {
	//		if (animType_ == 1) {
	//			moveX = FLY_SPEED * Time::DeltaTime();
	//			//animType_ = 1;
	//		}
	//		else {
	//			moveX = -FLY_SPEED * Time::DeltaTime();
	//			//animType_ = 0;
	//		}
	//	}
	//	else {
	//		//Playerがいた場所の中心に向かって移動
	//		if (lastPlayerX_ > myCenterX) {
	//			moveX = FLY_SPEED * Time::DeltaTime();
	//			animType_ = 1;
	//		}
	//		else {
	//			moveX = -FLY_SPEED * Time::DeltaTime();
	//			animType_ = 0;
	//		}
	//		if (lastPlayerY_ > myCenterY) {
	//			moveY = FLY_SPEED * Time::DeltaTime();
	//		}
	//		else {
	//			moveY = -FLY_SPEED * Time::DeltaTime();
	//		}
	//	}

	//	transform_.position_.x += moveX;
	//	transform_.position_.y += moveY;
	//}
	//else {
	//	//視界外に出たらS_Normalに
	//	state_ = S_Normal;
	//}

	if (player == nullptr) return;
	chaseDelayTime_ += Time::DeltaTime();
	if (chaseDelayTime_ >= 0.5f) {  // 0.5秒ごとに位置を更新
		lastPlayerX_ = player->GetPosition().x + (float)PLAYER_CHIP_SIZE / 4;
		lastPlayerY_ = player->GetPosition().y + (float)PLAYER_CHIP_SIZE / 2;
		chaseDelayTime_ = 0.0f;  //更新したらカウントをリセット
	}

	float myCenterX = transform_.position_.x + (float)CHIP_SIZE / 2 - X_MARGIN;
	float myCenterY = transform_.position_.y + (float)CHIP_SIZE / 2 + Y_MARGIN;

	if (InRange(player)) {
		float moveX = 0.0f;
		float moveY = 0.0f;

		if (lastPlayerX_ > myCenterX) {
			moveX = FLY_SPEED * Time::DeltaTime();
			animType_ = 1;
		}
		else {
			moveX = -FLY_SPEED * Time::DeltaTime();
			animType_ = 0;
		}
		if (lastPlayerY_ > myCenterY) {
			moveY = FLY_SPEED * Time::DeltaTime();
		}
		else {
			moveY = -FLY_SPEED * Time::DeltaTime();
		}

		transform_.position_.x += moveX;
		transform_.position_.y += moveY;
		if (transform_.position_.y < 0) {
			transform_.position_.y = 0;
		}
	}
	else {
		//視界外に出たらS_Normalに
		state_ = S_Normal;
	}

	time_ += Time::DeltaTime();
	//アニメーション
	if (time_ > 0.1f) {
		animFrame_ = animFrame_ % 5 + 1;
		time_ = 0.0f;
	}
}

void FlyEnemy::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		//y -= cam->GetValueY();
	}
	DrawRectGraph(x, y, animFrame_ * CHIP_SIZE, animType_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE, eImage_, TRUE);
	
	//当たり判定見るよう
	//DrawCircle( x + CHIP_SIZE/2-X_MARGIN, y + CHIP_SIZE/2+Y_MARGIN, 16.0f, GetColor(0, 0, 255), FALSE);
	
	//地面との当たり判定見るよう
	//DrawBox(x + 14, y + CHIP_SIZE / 2 + 6, x + CHIP_SIZE / 2 - 10, y + CHIP_SIZE, GetColor(0, 0, 0), FALSE);
}

void FlyEnemy::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
}

bool FlyEnemy::CollideCircle(float x, float y, float r)
{
	//x,y,rが相手の円の情報
	//自分の円の情報
	float myCenterX = transform_.position_.x + (float)CHIP_SIZE / 2-X_MARGIN;
	float myCenterY = transform_.position_.y + (float)CHIP_SIZE/2+Y_MARGIN;
	float myR = 16.0f;
	float dx = myCenterX - x;
	float dy = myCenterY - y;
	if ((dx * dx + dy * dy) < ((r + myR) * (r + myR))) {
		return true;
	}
	else {
		return false;
	}
}

//視界内にいるか
bool FlyEnemy::InRange(Player* player)
{
	//自分の中心位置
	float myCenterX = transform_.position_.x + (float)CHIP_SIZE / 2 - X_MARGIN;
	float myCenterY = transform_.position_.y + (float)CHIP_SIZE / 2 + Y_MARGIN;

	//Playerの中心位置
	float playerCenterX = player->GetPosition().x + (float)PLAYER_CHIP_SIZE / 4;
	float playerCenterY = player->GetPosition().y + (float)PLAYER_CHIP_SIZE / 2;

	//プレイヤーとの距離
	float dx = myCenterX - playerCenterX;
	float dy = myCenterY - playerCenterY;
	float distance = sqrt(dx * dx + dy * dy);

	//視界内にいるか返す
	return(distance < SEARCH_RANGE);
}
