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
#include "FlyEnemy.h"
#include "Health.h"
#include "Shield.h"

namespace {
	const float CHIP_SIZE = 64.0f;//キャラの画像サイズ
	const int MAP_HEIGHT = 720;   //高さ
	const float ROBO_WIDTH = 48;  //キャラの横幅
	const XMFLOAT3 INIT_POS = { 30,580,0 };//最初の位置
	const float JUMP_HEIGHT = 64.0f * 3.0f;//ジャンプの高さ
	const float INIT_GRAVITY = 9.8/ 90.0f; //重力
	const float MAX_POS = 400;//カメラが動かずにいける最大の位置
	const int SPEED = 200;    //スピード
	const int R_MARGIN = 24;    //プレイヤーのチップの余白
	const int L_MARGIN = 1;
	const int LIFE_IMAGE_SIZE = 64;//体力画像サイズ
	const int MISSILE_SIZE = 30;   //ミサイル画像サイズ
	const float FINV_TIME = 1.0f;  //無敵が終わる時間
	const int MAX_BULLET = 10;     //ミサイルの発射可能数
	const float INTERVAL = 5.0f;   //リロード時間
	const int MAX_X = 7680;        //X座標のマックス

	//重力メモ:月...1.62,火星...3.71
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), pImage_(-1), gravity_(INIT_GRAVITY),
	jumpSpeed_(0.0f), onGround_(true), time_(0.0f), animType_(0),
	animFrame_(0), prevAttackKey_(false), pLife_(3),
	invTime_(0), hitFlag_(false), lImage_(-1), dImage_(-1), ground_(0),
	prevMoveKey_(0), currentNum_(MAX_BULLET), reloadTime_(0), mImage_(-1),
	bImage_(-1), reloading_(false),getShield_(false)
{
	//初期位置の調整
	transform_.position_ = INIT_POS;
	state_ = S_Normal;
}

Player::~Player()
{
	//中身入っているならちゃんと消す
	if (pImage_ > 0) {
		DeleteGraph(pImage_);
		pImage_ = -1;
	}
	if (lImage_ > 0) {
		DeleteGraph(lImage_);
		lImage_ = -1;
	}
	if (dImage_ > 0) {
		DeleteGraph(dImage_);
		dImage_ = -1;
	}
	if (mImage_ > 0) {
		DeleteGraph(mImage_);
		mImage_ = -1;
	}
	if (bImage_ > 0) {
		DeleteGraph(bImage_);
		bImage_ = -1;
	}
	if (sImage_ > 0) {
		DeleteGraph(sImage_);
		sImage_ = -1;
	}
}

void Player::Initialize()
{
	//プレイヤーの画像の読み込み
	pImage_ = LoadGraph("Assets\\Image\\cyborg.png");
	assert(pImage_ >= 0); 

	//体力画像の読み込み
	lImage_ = LoadGraph("Assets\\Image\\Life4.png");
	assert(lImage_ > 0);

	//体力削られたところに使う画像の読み込み
	dImage_ = LoadGraph("Assets\\Image\\Damage2.png");
	assert(dImage_ > 0);

	//残弾表示に使う画像の読み込み
	mImage_ = LoadGraph("Assets\\Image\\missile.png");
	assert(mImage_ > 0);

	//弾のバナーの読み込み
	bImage_ = LoadGraph("Assets\\Image\\banner.png");
	assert(bImage_ > 0);

	//シールド所持
	sImage_ = LoadGraph("Assets\\Image\\Shield.png");
	assert(sImage_ > 0);
}

void Player::Update()
{
	//プレイシーンから動いていいのかどうかを確認する
	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->canMove())//動いちゃダメの場合
		return;           //なにもしない

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
	case 3: //〇ぬ
		UpdateDie();
		break;
	default:
		break;
	}

	//アニメーションに使うタイムの更新
	time_ += Time::DeltaTime();

	//左右の向きで若干変わる当たり判定の位置の修正
	float colX,colY,colR;
	if (prevMoveKey_ == 0) {
		colX = transform_.position_.x + CHIP_SIZE / 4;
	}
	else {
		colX = transform_.position_.x + CHIP_SIZE / 10 * 3;
	}
	colY = transform_.position_.y + CHIP_SIZE / 5 * 3;
	colR = 20.0f;

	//ステージとの上下の当たり判定
	Stage* pStage = GetParent()->FindGameObject<Stage>();
	if (pStage != nullptr) {
		int pushR = pStage->CollisionDown(transform_.position_.x + CHIP_SIZE / 15 * 7, transform_.position_.y + CHIP_SIZE);
		int pushL = pStage->CollisionDown(transform_.position_.x + CHIP_SIZE / 7, transform_.position_.y + CHIP_SIZE);
		int push = max(pushR, pushL);//２つの足元のめり込みの大きい方
		if (push >= 1) {
			transform_.position_.y -= push - 1;
			jumpSpeed_ = 0.0f;
			onGround_ = true;
		}
		else {
			onGround_ = false;
		}
		/*pushR = pStage->CollisionUp(transform_.position_.x + CHIP_SIZE - R_MARGIN, transform_.position_.y + CHIP_SIZE / 4);
		pushL = pStage->CollisionUp(transform_.position_.x + 9, transform_.position_.y + CHIP_SIZE / 4);*/
		pushR = pStage->CollisionUp(transform_.position_.x + CHIP_SIZE / 15 * 7, transform_.position_.y + CHIP_SIZE / 4);
		pushL = pStage->CollisionUp(transform_.position_.x + CHIP_SIZE / 7, transform_.position_.y + CHIP_SIZE / 4);
		push = max(pushR, pushL);//２つの頭上のめり込みの大きい方
		if (push >= 1) {
			transform_.position_.y += push + 1;
			jumpSpeed_ = 0.0f;
		}
	}

	//隕石との当たり判定
	std::list<Meteorite*> pMeteos = GetParent()->FindGameObjects<Meteorite>();
	for (Meteorite* pMeteo : pMeteos) {
		if (pMeteo->CollideCircle(colX, colY, colR)) {
			pMeteo->KillMe();
			Explosion* pEx = Instantiate<Explosion>(GetParent());
			pEx->SetPosition(transform_.position_.x, transform_.position_.y - CHIP_SIZE / 2);
			if (prevMoveKey_ == 0) {
				animFrame_ = 0;
			}
			else {
				animFrame_ = 5;
			}

			if (getShield_ == false) {
				state_ = S_Die;
			}
			else {
				hitFlag_ = true;
				getShield_ = false;
			}

		}
	}

	//敵との当たり判定
	std::list<Enemy*> pEnemys = GetParent()->FindGameObjects<Enemy>();
	for (Enemy* pEnemy : pEnemys) {
		if (hitFlag_ == false) {
			if (pEnemy->CollideCircle(colX, colY, colR)) {
				if (getShield_ == false) {
					pLife_ -= 1;
				}
				else {
					getShield_ = false;
				}
				hitFlag_ = true;
			}
		}
	}

	//飛んでる敵との当たり判定
	std::list<FlyEnemy*> fEnemys = GetParent()->FindGameObjects<FlyEnemy>();
	for (FlyEnemy* fEnemy : fEnemys) {
		if (hitFlag_ == false) {
			if (fEnemy->CollideCircle(colX, colY, colR)) {
				if (getShield_ == false) {
					pLife_ -= 1;
				}
				else {
					getShield_ = false;
				}
				hitFlag_ = true;
			}
		}
	}

	//回復アイテムとの当たり判定
	std::list<Health*> pHealth = GetParent()->FindGameObjects<Health>();
	for (Health* pHealth : pHealth) {
		if (pHealth->CollideCircle(colX, colY, colR)) {
			//体力が3の時は増やさない
			if (pLife_ < 3) {
				pLife_ += 1;
			}
			pHealth->KillMe();
		}
	}

	//防御アイテムとの当たり判定
	std::list<Shield*> pShield = GetParent()->FindGameObjects<Shield>();
	for (Shield* pShield : pShield) {
		if (pShield->CollideCircle(colX, colY, colR)) {
			getShield_ = true;
			pShield->KillMe();
		}
	}

	//敵が当たったら少しの間無敵になる
	if (hitFlag_ == true) {
		invTime_ += Time::DeltaTime();
		if (invTime_ >= FINV_TIME) {
			hitFlag_ = false;
			invTime_ = 0;
		}
	}

	//ライフが０になったら死ぬ
	if (pLife_ == 0)
	{
		state_ = S_Die;
	}

	//ゴールの旗との当たり判定
	Flag* pFlag = GetParent()->FindGameObject<Flag>();
	if (pFlag->HitFlag(colX,colY,colR)) {
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
	if (transform_.position_.x > MAX_X) {
		transform_.position_.x = MAX_X;
	}

	//ここでカメラ位置の調整
	Camera* cam = GetParent()->FindGameObject<Camera>();
	int x = (int)transform_.position_.x + cam->GetValueX();
	//画面の１/３ぐらいまでくるとカメラが動くように
	if (x > MAX_POS) {
		x = MAX_POS;
		cam->SetValueX((int)transform_.position_.x - x);
	}
	/*if (x < 0) {
		x = 0;
		cam->SetValueX((int)transform_.position_.x - x);
	}
	if (x > 3600) {
		x = 3600;
		cam->SetValueX((int)transform_.position_.x - x);
	}*/

	if (transform_.position_.y <= 0) {
		jumpSpeed_ = 0;
		//transform_.position_.y = 1;
	}
	
	//リロード中なら
	if (reloading_) {
		Reload();
	}
}

void Player::UpdateNormal()
{
	//無敵の時は点滅
	if (hitFlag_) {
		if (prevMoveKey_ == 0) {
			animType_ = 5;
		}
		else {
			animType_ = 15;
		}
	}
	else {
		if (prevMoveKey_ == 0) {
			animType_ = 0;
		}
		else {
			animType_ = 10;
		}
	}

	//アニメーション更新
	if (time_ > 0.2f) {
		if (onGround_) {
			animFrame_ = animFrame_ % 3 + 1;
			time_ = 0.0f;
			/*if (prevMoveKey_ == 0) {
				animFrame_ = animFrame_ % 3 + 1;
				time_ = 0.0f;
			}
			else {
				if (animFrame_ == 2) {
					animFrame_ = 5;
				}
				else {
					animFrame_--;
				}
				time_ = 0.0f;
			}*/
		}
	}

	//コントローラの情報とる
	int x, y;
	DINPUT_JOYSTATE input;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);
	x = input.X / 1000.0f;

	//なんか動くならS_Moveに移動
	if (CheckHitKey(KEY_INPUT_D) || CheckHitKey(KEY_INPUT_A)
		|| CheckHitKey(KEY_INPUT_SPACE) || x > 0.3f || x < -0.3f
		|| ((input.Buttons[0] & 0x80) != 0)) {
		state_ = S_Move;
	}

	//ミサイルを飛ばしての攻撃
	//if (onGround_) {//地面に立っている時しか使えないようにする場合
	if ((CheckHitKey(KEY_INPUT_J) || (input.Buttons[1] & 0x80) != 0) && currentNum_ > 0) {//横に飛ばす
		if (!reloading_) {
			ReadyAttack(isTypeA);//弾の準備
		}
	}
	else {
		prevAttackKey_ = false;
		isTypeA = false;
	}

	if ((CheckHitKey(KEY_INPUT_K) || (input.Buttons[3] & 0x80) != 0) && currentNum_ > 0) {//斜め前に飛ばす
		if (!reloading_) {
			ReadyAttack(isTypeB);//弾の準備
		}
	}
	else {
		prevAttackKey_ = false;
		isTypeB = false;
	}
    //}

	if (CheckHitKey(KEY_INPUT_L) || (input.Buttons[2] & 0x80) != 0)//弾のリロード
	{
		if (currentNum_ != MAX_BULLET && reloading_ != true) {
			reloading_ = true;
		}
	}
}

void Player::UpdateMove()
{
	//無敵の時は点滅
	if (hitFlag_) {
		if (prevMoveKey_ == 0) {
			animType_ = 6;
		}
		else {
			animType_ = 16;
		}
	}
	else {
		if (prevMoveKey_ == 0) {
			animType_ = 1;
		}
		else {
			animType_ = 11;
		}
	}

	//移動量とその初期化
	float moveX, moveY;
	moveX = 0.0f;
	moveY = 0.0f;

	//コントローラ操作(なぜかスティックを左に倒しても左にいかない)
	DINPUT_JOYSTATE input;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);
	//if (input.X < 0) {
	//	moveX += (float)input.X / 1000.0f; //取ってきた値を使えるように小さくする
	//}

	moveX += (float)input.X / 1000.0f * (SPEED * Time::DeltaTime()); //取ってきた値を使えるように小さくする
	
	if (moveX < 0.3f && moveX > -0.3f) {//中心付近の誤差をのぞく
		 moveX = 0.0f;
	}

	//コントローラでも右左の動きを切り替えられるように
	if (moveX > 0) {
		prevMoveKey_ = 0;//右向き
	}
	else if(moveX < 0) {
		prevMoveKey_ = 1;//左向き
	}

	//移動
	if (CheckHitKey(KEY_INPUT_D)) {//Dキーを押すと右に進む
		moveX += SPEED * Time::DeltaTime();
		prevMoveKey_ = 0;//右向き
	}
	else if (CheckHitKey(KEY_INPUT_A)) {//Aキーを押すと左に進む
		moveX -= SPEED * Time::DeltaTime();
		prevMoveKey_ = 1;//左向き
	}
	
	//移動してないなら
	if (moveX == 0) {
		if (onGround_) {            //地面にいるなら
			state_ = S_Normal;      //ノーマルに戻したる
			animFrame_ = 0;
			//if (prevMoveKey_ == 0) {//最終が右向きなら
			//	animFrame_ = 0;
			//}
			//else {//最終が左向きなら
			//	animFrame_ = 5;
			//}
			time_ = 0;
		}
	}

	transform_.position_.x += moveX;//移動量

	if (time_ > 0.15f) {
		if (onGround_) {
			animFrame_ = animFrame_ % 5 + 1;
			time_ = 0.0f;
		}
	}

	//if (prevMoveKey_ == 0) {//右向きなら
	//	//アニメーション
	//	if (time_ > 0.15f) {
	//		if (onGround_) {
	//			animFrame_ = animFrame_ % 5 + 1;
	//			time_ = 0.0f;
	//		}
	//	}
	//}
	//else {//左向きなら
	//	//左向きの歩き
	//	if (time_ > 0.15f) {
	//		if (onGround_) {
	//			if (animFrame_ == 0) {
	//				animFrame_ = 5;
	//			}
	//			else {
	//				animFrame_--;
	//			}
	//			time_ = 0.0f;
	//		}
	//	}
	//}

	Stage* pStage = GetParent()->FindGameObject<Stage>();

	//プレイヤーの右側のステージとの当たり判定
	//int hitX = transform_.position_.x + (CHIP_SIZE - R_MARGIN);//ブロックとプレイヤーの余白をなくすために引く
	//int hitX = transform_.position_.x + CHIP_SIZE / 15 * 7;
	//int hitY = transform_.position_.y + CHIP_SIZE - 1; //そのまま足すと落ちていくから-1
	////int hitY = transform_.position_.y + CHIP_SIZE / 4;
	//if (pStage != nullptr) {
	//	int push = pStage->CollisionRight(hitX, hitY);//右側の当たり判定
	//	transform_.position_.x -= push;//のめりこんでる分戻す
	//}

	//新・右側のステージとの当たり判定
	int pushT, pushB, pushM ,push;
	int hitX = transform_.position_.x + CHIP_SIZE / 15 * 7;
	int hitY = transform_.position_.y + CHIP_SIZE / 4;
	if (pStage != nullptr) {
	    pushT = pStage->CollisionRight(hitX, hitY);//右側の当たり判定
		hitY = transform_.position_.y + CHIP_SIZE - 1;
		pushB = pStage->CollisionRight(hitX, hitY);
		hitY = transform_.position_.y + CHIP_SIZE / 2;
		pushM = pStage->CollisionRight(hitX, hitY);
		push = max(pushT, pushB);
		push = max(push, pushM);
		transform_.position_.x -= push;//のめりこんでる分戻す
	}
	//新・左側のステージとの当たり判定
	hitX = transform_.position_.x + CHIP_SIZE / 7;
	hitY = hitY = transform_.position_.y + CHIP_SIZE / 4;
	if (pStage != nullptr) {
		pushT = pStage->CollisionLeft(hitX, hitY);
		hitY = transform_.position_.y + CHIP_SIZE - 1;
		pushB = pStage->CollisionLeft(hitX, hitY);
		hitY = transform_.position_.y + CHIP_SIZE / 2;
		pushM = pStage->CollisionLeft(hitX, hitY);
		push = max(pushT, pushB);
		push = max(push, pushM);
		transform_.position_.x += push;//のめりこんでる分戻す
	}

	//プレイヤーの左側のステージとの当たり判定
	//hitX = transform_.position_.x + MARGIN;//ブロックとプレイヤーの余白をなくすために足す(なぜかこれだと後ろ下がり続けるとがたがたする)
	//hitX = transform_.position_.x + L_MARGIN;
	//hitX = transform_.position_.x + CHIP_SIZE / 7;
	//hitY = transform_.position_.y + CHIP_SIZE - 1;//そのまま足すと落ちていくから-1
	////hitY = transform_.position_.y + CHIP_SIZE / 4;
	//if (pStage != nullptr) {
	//	int push = pStage->CollisionLeft(hitX, hitY);//左側の当たり判定
	//	transform_.position_.x += push;//のめりこんでる分戻す
	//}

	//if (prevMoveKey_ == 0) {//右向き
	//	//プレイヤーの右側のステージとの当たり判定
	//	int hitX = transform_.position_.x + (CHIP_SIZE - MARGIN);//ブロックとプレイヤーの余白をなくすために引く
	//	int hitY = transform_.position_.y + CHIP_SIZE - 1; //そのまま足すと落ちていくから-1
	//	if (pStage != nullptr) {
	//		int push = pStage->CollisionRight(hitX, hitY);//右側の当たり判定
	//		transform_.position_.x -= push;//のめりこんでる分戻す
	//	}

	//	//プレイヤーの左側のステージとの当たり判定
	//	hitX = transform_.position_.x + MARGIN;//ブロックとプレイヤーの余白をなくすために足す(なぜかこれだと後ろ下がり続けるとがたがたする)
	//	hitY = transform_.position_.y + CHIP_SIZE - 1;//そのまま足すと落ちていくから-1
	//	if (pStage != nullptr) {
	//		int push = pStage->CollisionLeft(hitX, hitY);//左側の当たり判定
	//		transform_.position_.x += push;//のめりこんでる分戻す
	//	}
	//}
	//else {//左向き
	//	//プレイヤーの右側のステージとの当たり判定
	//	int hitX = transform_.position_.x + (CHIP_SIZE -MARGIN);//ブロックとプレイヤーの余白をなくすために引く
	//	int hitY = transform_.position_.y + CHIP_SIZE - 1; //そのまま足すと落ちていくから-1
	//	if (pStage != nullptr) {
	//		int push = pStage->CollisionRight(hitX, hitY);//右側の当たり判定
	//		transform_.position_.x -= push;//のめりこんでる分戻す
	//	}
	//	
	//	//プレイヤーの左側のステージとの当たり判定
	//	hitX = transform_.position_.x + MARGIN;//ブロックとプレイヤーの余白をなくすために足す(なぜかこれだと後ろ下がり続けるとがたがたする)
	//	hitY = transform_.position_.y + CHIP_SIZE - 1;//そのまま足すと落ちていくから-1
	//	if (pStage != nullptr) {
	//		int push = pStage->CollisionLeft(hitX, hitY);//左側の当たり判定
	//		transform_.position_.x += push;//のめりこんでる分戻す
	//	}
	//}

	if (onGround_) {//地面にいるか
		if (CheckHitKey(KEY_INPUT_SPACE) || (input.Buttons[0] & 0x80) != 0) {//SPACEキーを押すとジャンプ
			jumpSpeed_ = -sqrtf(2 * (gravity_)*JUMP_HEIGHT);
			onGround_ = false;//地面にいない
		}
	}

	//ジャンプの時と降りてる時のアニメーションの変更
	if (!onGround_ && jumpSpeed_ < 0) {//地面にいなくてジャンプの上に行く途中なら
		if (prevMoveKey_ == 0) {//右向きなら
			animType_ = 2;
			/*animFrame_ = 0;
			if (time_ > 0.5) {
				animFrame_ = 1;
			}*/
		}
		if (prevMoveKey_ == 1) {//左向きなら
			animType_ = 12;
			/*animFrame_ = 5;
			if (time_ > 0.5) {
				animFrame_ = 4;
			}*/
		}
		animFrame_ = 0;
		if (time_ > 0.5) {
			animFrame_ = 1;
		}
	}
	else if (!onGround_ && jumpSpeed_ >= 0) {//地面にいなくて、ジャンプで下がっているなら
		if (prevMoveKey_ == 0) {//右向きなら
			animType_ = 2;
			/*animFrame_ = 2;
			if (time_ > 0.5) {
				animFrame_ = 3;
			}*/
		}
		if (prevMoveKey_ == 1) {//左向きなら
			animType_ = 12;
			/*animFrame_ = 3;
			if (time_ > 0.5) {
				animFrame_ = 2;
			}*/
		}
		animFrame_ = 2;
		if (time_ > 0.5) {
			animFrame_ = 3;
		}
	}

	jumpSpeed_ += gravity_;//速度 += 重力
	transform_.position_.y += jumpSpeed_; //座標 += 速度

	//ミサイルを飛ばしての攻撃
	//if (onGround_) {//地面に立っている時しか使えないようにする場合
		if ((CheckHitKey(KEY_INPUT_J) || (input.Buttons[1] & 0x80) != 0) && currentNum_ > 0) {//攻撃のキーを押すのと、残弾があるなら
			if (!reloading_) {
				ReadyAttack(isTypeA);//弾の準備
			}
		}
		else {
			prevAttackKey_ = false;
			isTypeA = false;
		}

		if ((CheckHitKey(KEY_INPUT_K) || (input.Buttons[3] & 0x80) != 0) && currentNum_ > 0) {//斜め前に飛ばす
			if (!reloading_) {
				ReadyAttack(isTypeB);//弾の準備
			}
		}
		else {
			prevAttackKey_ = false;
			isTypeB = false;
		}

	//}
	if (CheckHitKey(KEY_INPUT_L) || (input.Buttons[2] & 0x80) != 0)//弾のリロード
	{
		if (currentNum_ != MAX_BULLET && reloading_ != true) {
			reloading_ = true;
		}
	}
}

void Player::UpdateAttack()
{
	if (hitFlag_) {//無敵の時は点滅
		if (prevMoveKey_ == 0) {//右向きなら
			animType_ = 8;
		}
		else {
			animType_ = 18;
		}
	}
	else {
		if (prevMoveKey_ == 0) {//左向きなら
			animType_ = 3;
		}
		else {
			animType_ = 13;
		}
	}

	//アニメーションが最後になったら弾を打ち出す
	if (time_ > 0.05f) {
		if (prevMoveKey_ == 0) {//右向きなら
			if (animFrame_ + 1 == 5)
			{
				Attack(0, 1);
			}
		}
		else {//左向きなら
			if (animFrame_ + 1 == 5)
			{
				Attack(1, 3);
			}
			/*if (animFrame_ - 1 == 0)
			{
				Attack(1, 3);
				animFrame_ = 5;
			}*/
		}
	}

	//アニメーションの更新
	if (time_ > 0.05f) {
		animFrame_ = animFrame_ % 5 + 1;
		time_ = 0.0f;
		//if (prevMoveKey_ == 0) {//右向きなら
		//	animFrame_ = animFrame_ % 5 + 1;
		//	time_ = 0.0f;
		//}
		//else {//左向きなら
		//	if (animFrame_ == 0) {
		//		animFrame_ = 5;
		//	}
		//	else {
		//		animFrame_--;
		//	}
		//	time_ = 0.0f;
		//}
	}

	jumpSpeed_ += gravity_;//速度 += 重力
	transform_.position_.y += jumpSpeed_; //座標 += 速度
}

void Player::UpdateDie()
{
	if (prevMoveKey_ == 0) {
		animType_ = 4;//死ぬ時のアニメーション(右)
	}
	else {
		animType_ = 14;//死ぬときのアニメーション(左)
	}

	//死んだ時に地面にいないなら地面に落としてから〇す
	if (!onGround_) {
		transform_.position_.y += 3.0f;
		//animFrame_ = 1;
		if (prevMoveKey_ == 0) {
			animFrame_ = 1;
		}
		else {
			animFrame_ = 4;
		}
	}
	else {
		//if (animFrame_ + 1 == 6) {//最後のフレームになったら
		//	Die();//〇ぬ
		//	animFrame_ = 0;
		//}
		//if (time_ > 0.2f) {
		//	animFrame_ = animFrame_ % 5 + 1;
		//	time_ = 0.0f;
		//}
		if (prevMoveKey_ == 0) {
			if (animFrame_ + 1 == 6) {//最後のフレームになったら
				Die();//〇ぬ
				animFrame_ = 0;
			}
			if (time_ > 0.2f) {
				animFrame_ = animFrame_ % 5 + 1;
				time_ = 0.0f;
			}
		}
		else {
			if (animFrame_ == 0) {//最後のフレームになったら
				Die();//〇ぬ
				animFrame_ = 5;
			}
			if (time_ > 0.2f) {
				animFrame_--;
				time_ = 0.0f;
			}
		}
	}
}

void Player::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		//y -= cam->GetValueY();
	}
	DrawRectGraph(x, y, animFrame_ * CHIP_SIZE, animType_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE, pImage_, TRUE);
	//当たり判定を見るよう
	//DrawCircle(x + CHIP_SIZE / 4, y + CHIP_SIZE / 5 * 3, 20.0f, GetColor(0, 0, 255), FALSE);
	/*if (prevMoveKey_ == 0) {
		DrawCircle(x + CHIP_SIZE / 4, y + CHIP_SIZE / 5 * 3, 20.0f, GetColor(0, 0, 255), FALSE);
	}
	else {
		DrawCircle(x + CHIP_SIZE / 10 * 3, y + CHIP_SIZE / 5 * 3, 20.0f, GetColor(0, 0, 255), FALSE);
	}*/

	//足元の点
	/*DrawCircle(x + CHIP_SIZE / 15 * 7, y + CHIP_SIZE, 2.0f, GetColor(0, 0, 255), TRUE);
	DrawCircle(x + CHIP_SIZE / 7, y + CHIP_SIZE, 2.0f, GetColor(0, 0, 255), TRUE);
	DrawCircle(x + CHIP_SIZE / 15 * 7, y + CHIP_SIZE / 4,2.0f,GetColor(0,0,255),TRUE);
	DrawCircle(x + CHIP_SIZE / 7, y + CHIP_SIZE / 4, 2.0f, GetColor(0, 0, 255), TRUE);*/
	
	//残弾数がわかりやすいように
	DrawExtendGraph(120, -130, 610, 200, bImage_, TRUE);//バナー

	for (int i = 0; i < currentNum_; i++) {

		DrawGraph((i * MISSILE_SIZE) + 180, 10, mImage_, TRUE);
	}

	//ライフとライフの下に黒くしたライフを描画
	for (int j = 0; j < 3; j++) {
		DrawGraph(LIFE_IMAGE_SIZE * j, 0, dImage_, TRUE);
	}
	for (int i = 0; i < pLife_; i++) {
		DrawGraph(LIFE_IMAGE_SIZE * i, 0, lImage_, TRUE);
	}

	//シールドのアイコン表示位置
	if (getShield_ == true) {
		DrawGraph(10, 80, sImage_, TRUE);
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

void Player::Reload()
{
	//インターバルを超えたら一弾補充
	/*if (reloadTime_ > INTERVAL) {
		if (currentNum_ != MAX_BULLET) {
			currentNum_++;
		}
		reloadTime_ = 0;
	}
	reloadTime_ += Time::DeltaTime();*/

	//インターバルを超えたら、全部補充
	reloadTime_ += Time::DeltaTime();
	if (reloadTime_ > INTERVAL) {
		currentNum_ = MAX_BULLET;
		reloading_ = false;
		reloadTime_ = 0.0f;
	}
}

void Player::ReadyAttack(bool &_isType)
{
	if (prevAttackKey_ == false) {
		if (prevMoveKey_ == 0) {
			animFrame_ = 0;
			animType_ = 3;
		}
		else {
			//animFrame_ = 5;
			animFrame_ = 0;
			animType_ = 13;
		}
		time_ = 0.0f;
		_isType = true;
		state_ = S_Attack;//攻撃の状態に移る
	}
	prevAttackKey_ = true;
}

void Player::Attack(int angleA, int angleB)
{
	AttackSkill* attack = Instantiate<AttackSkill>(GetParent());
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;

	/*if (prevMoveKey_ == 0) {
		attack->SetPosition(x + CHIP_SIZE, y);
	}
	else {
		attack->SetPosition(x, y);
	}*/

	//横に撃つか、斜めに撃つか
	if (isTypeA == true) {//横に撃つ
		attack->SetAngle(XM_PI*angleA);//角度の設定
		if (prevMoveKey_ == 0) {
			attack->SetPosition(x + CHIP_SIZE, y);
		}
		else {
			attack->SetPosition(x, y);
		}
	}
	if (isTypeB == true) {//斜め上に撃つ
		attack->SetAngle(XM_PI / -4 * angleB);//角度を設定
		if (prevMoveKey_ == 0) {
			attack->SetPosition(x + CHIP_SIZE / 5 * 2, y);
		}
		else {
			attack->SetPosition(x + CHIP_SIZE / 5, y);
		}
	}
	currentNum_--;
	time_ = 0.0f;
	animFrame_ = 0;
	if (onGround_) {
		state_ = S_Normal;
	}
	else {
		state_ = S_Move;
	}
}

void Player::Die()
{
	KillMe();//バイバイ
	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	scene->StartGameOver();//ゲームオーバシーンに移行
}
