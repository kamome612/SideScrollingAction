#include "AttackSkill.h"
#include "Engine/time.h"
#include "Meteorite.h"
#include "Explosion.h"
#include "Camera.h"
#include "Enemy.h"
#include "Stage.h"
#include "FlyEnemy.h"
#include "Health.h"
#include "Shield.h"
#include "MissileItem.h"
#include <random>

namespace {
	const float SPEED_ = 600;
	const float LimitTime_ = 1.5f;
	const float CHIP_SIZE = 64.0f;//キャラの画像サイズ
}

AttackSkill::AttackSkill(GameObject* parent)
	:GameObject(parent,"AttackSkill"),aImage_(-1),BulletTime_(0),angle_(0)
{
}

AttackSkill::~AttackSkill()
{
	if (aImage_ > 0) {
		DeleteGraph(aImage_);
	}
}

void AttackSkill::Initialize()
{
	//画像のロード
	aImage_ = LoadGraph("Assets/Image/missile.png");
	assert(aImage_ > 0);
	//音のロード
	aSound_ = LoadSoundMem("Assets/Sound/Explosion.mp3");
	assert(aSound_ != -1);
	ChangeVolumeSoundMem(255 * 40 / 100, aSound_);
}

void AttackSkill::Update()
{
	//ミサイルの生存時間
	BulletTime_ += Time::DeltaTime();
	if (BulletTime_ >= LimitTime_)
	{
		KillMe();
	}
	if (transform_.position_.y < -30) {//画面外に出ていれば消す
		KillMe();
	}
	transform_.position_.x += cos(angle_) * SPEED_ * Time::DeltaTime();
	transform_.position_.y += sin(angle_) * SPEED_ * Time::DeltaTime();

	//隕石との当たり判定
	std::list<Meteorite*> pMeteos = GetParent()->FindGameObjects<Meteorite>();
	for (Meteorite* pMeteo : pMeteos) {
		if (pMeteo->CollideCircle(transform_.position_.x, transform_.position_.y+32.0f, 10.0f)) {
			pMeteo->KillMe();
			Explosion* pEx = Instantiate<Explosion>(GetParent());
			pEx->SetPosition(transform_.position_.x-32.0, transform_.position_.y-64.0);
			PlaySoundMem(aSound_, DX_PLAYTYPE_BACK);
			// 音量の設定
			//ChangeVolumeSoundMem(255 * 40 / 100, aSound_);
			KillMe();
		}
	}

	//地面の敵との当たり判定
	std::list<Enemy*> pEnemys = GetParent()->FindGameObjects<Enemy>();
	for (Enemy* pEnemy : pEnemys) {
		if (pEnemy->CollideCircle(transform_.position_.x, transform_.position_.y+32.0f,10.0f)) {
			pEnemy->KillMe();

			//弾が当たったらランダムでアイテムをドロップ
			//ここをいじって確率を変える。
			int type = rand() % 10;
			if (type == 1 || type == 5) {
				Health* pHealth = Instantiate<Health>(GetParent());
				pHealth->SetPosition(transform_.position_.x, transform_.position_.y);
			}
			if (type == 2) {
				Shield* pShield = Instantiate<Shield>(GetParent());
				pShield->SetPosition(transform_.position_.x, transform_.position_.y);
			}
			Explosion* pEx = Instantiate<Explosion>(GetParent());
			pEx->SetPosition(transform_.position_.x - 32.0f, transform_.position_.y - 64.0f);
			PlaySoundMem(aSound_, DX_PLAYTYPE_BACK);
			// 音量の設定
			//ChangeVolumeSoundMem(255 * 30 / 100, aSound_);
			KillMe();
		}
	}

	//浮いてる敵との当たり判定
	std::list<FlyEnemy*> fEnemys = GetParent()->FindGameObjects<FlyEnemy>();
	for (FlyEnemy* fEnemy : fEnemys) {
		if (fEnemy->CollideCircle(transform_.position_.x, transform_.position_.y + 32.0f, 10.0f)) {
			fEnemy->KillMe();
			//弾が当たったらランダムでアイテムをドロップ
			// //ここをいじって確率を変える。
			int type = rand() % 15;
			if (type == 0) {
				MissileItem* pMissileItem = Instantiate<MissileItem>(GetParent());
				pMissileItem->SetPosition(transform_.position_.x, transform_.position_.y);
			}
			if (type == 1 || type == 5) {
				Health* pHealth = Instantiate<Health>(GetParent());
				pHealth->SetPosition(transform_.position_.x, transform_.position_.y);
			}
			if (type == 2) {
				Shield* pShield = Instantiate<Shield>(GetParent());
				pShield->SetPosition(transform_.position_.x, transform_.position_.y);
			}
			Explosion* pEx = Instantiate<Explosion>(GetParent());
			pEx->SetPosition(transform_.position_.x - 32.0f, transform_.position_.y - 64.0f);
			PlaySoundMem(aSound_, DX_PLAYTYPE_BACK);
			// 音量の設定
			//ChangeVolumeSoundMem(255 * 30 / 100, aSound_);
			KillMe();
		}
	}

	//ステージとの当たり判定
	Stage* pStage = GetParent()->FindGameObject<Stage>();
	bool isHitLeft = pStage->CollisionLeft(transform_.position_.x, transform_.position_.y + CHIP_SIZE / 1.5);
	bool isHitRight = pStage->CollisionRight(transform_.position_.x, transform_.position_.y + CHIP_SIZE / 1.5);
	if (isHitLeft || isHitRight) {
		KillMe();
	}
}

void AttackSkill::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y + 32.0f;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		//y -= cam->GetValueY();
	}
	DrawRotaGraph(x, y, 1.0, angle_ - 80.1, aImage_, TRUE);//角度は適当に調整した
}

void AttackSkill::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
}

void AttackSkill::SetAngle(float _angle)
{
	angle_ = _angle;
}