#include "Stage.h"
#include "Engine/CsvReader.h"
#include "Player.h"
#include "Meteorite.h"
#include "Camera.h"
#include "AttackSkill.h"
#include "Flag.h"
#include "Tree.h"
#include "Cloud.h"
#include "Rocket.h"
#include "Meteo.h"
#include "Enemy.h"
#include "MeteoSpawner.h"
#include "FlyEnemy.h"

namespace {
	const int CHIP_SIZE = 32;
	//const int LIFE_IMAGE_SIZE = 64;
	const int MAP_WIDTH = 1280;
	const int MAP_HEIGHT = 720;
	const int CHIP_NULL = -1;
}

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage")
{
	hImage_ = -1;
	width_ = 0;
	height_ = 0;
	mapNo_ = 1;
	gPict_ = -1;
	map_ = nullptr;
}

Stage::~Stage()
{
	if (hImage_ < 0) {
		DeleteGraph(hImage_);
		hImage_ = -1;
	}

	if (gPict_ < 0) {
		DeleteGraph(gPict_);
		gPict_ = -1;
	}

	if (map_ != nullptr) {
		delete[] map_;
	}
}

void Stage::Initialize()
{
	map_ = nullptr;
}

void Stage::Update()
{
}

void Stage::Draw()
{
	switch (mapNo_) {
	case 1:
		DrawGraph(0, -240, gPict_, TRUE);
		break;
	case 2:
	case 3:
		DrawGraph(0, 0, gPict_, TRUE);
	}

	int scrollX = 0;
	int scrollY = 0;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		scrollX = cam->GetValueX();
		scrollY = cam->GetValueY();
	}

	//ステージの描画
	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			int chip = map_[h * width_ + w];

			/*DrawRectGraph(w * CHIP_SIZE-scrollX, h * CHIP_SIZE - scrollY, 
				          CHIP_SIZE * (chip % 16), CHIP_SIZE * (chip / 16), 
				          CHIP_SIZE, CHIP_SIZE, hImage_, TRUE, FALSE);*/

			//新しい地面用
			DrawRectGraph(w * CHIP_SIZE - scrollX, h * CHIP_SIZE,
				CHIP_SIZE * (chip % 26), CHIP_SIZE * (chip / 26),
				CHIP_SIZE, CHIP_SIZE, hImage_, TRUE, FALSE);
		}
	}
}

void Stage::StageSet()
{
	//マップの中になにか入ってたら消す
	if (map_ != nullptr) {
		delete[] map_;
		map_ = nullptr;
	}
	//プレイヤーや隕石、ミサイルがステージのリセットやセット時に消されるように
	if (Player* sPlayer = GetParent()->FindGameObject<Player>()) {
		sPlayer->KillMe();
	}
	if (Meteorite* sMeteo = GetParent()->FindGameObject<Meteorite>()) {
		std::list<Meteorite*> meteos = GetParent()->FindGameObjects<Meteorite>();
		for (Meteorite* m : meteos) {
			m->KillMe();
		}
	}
	if (AttackSkill* sAttack = GetParent()->FindGameObject<AttackSkill>()) {
		sAttack->KillMe();
	}

	Camera* cam = GetParent()->FindGameObject<Camera>();
	cam->SetValueX(0);
	cam->SetValueY(-32); //ここで全体のカメラの高さを変えられる
	static const std::string folder = "Assets/Stage/";

	//ステージ素材の画像の読み込み
	std::string n = std::to_string(mapNo_);
	//hImage_ = LoadGraph((folder + "bgchar" + n + ".png").c_str());
	//hImage_ = LoadGraph("Assets/Stage/spritesheet_ground.png");
	//hImage_ = LoadGraph("Assets/Stage/ground2.png");

	if (mapNo_ != 3) {
		//hImage_ = LoadGraph((folder + "ground3.png").c_str());
		
		//新しい地面用
		hImage_ = LoadGraph((folder + "ground.png").c_str());
	}
	else {
		hImage_ = LoadGraph((folder + "ground2.png").c_str());
	}
	assert(hImage_ > 0);

	//バックグランドの画像読み込み
	gPict_ = LoadGraph(("Assets/Picture/background" + n + ".png").c_str());
	assert(gPict_ > 0);

	//csvから読み込み
	CsvReader csv;
	//bool ret = csv.Load((folder + "Stage" + n + ".csv").c_str());

	//新しい地面用
	bool ret = csv.Load((folder + "test3.csv").c_str());
	assert(ret);

	//csvで読んだステージの横と縦を取る
	width_ = csv.GetWidth(0);
	height_ = csv.GetHeight();

	map_ = new int[height_ * width_];

	for (int h = 0; h < height_; h++) {
		if (csv.GetString(0, h) == "") {//空行を飛ばす
			height_ = h;
			break;
		}
		for (int w = 0; w < width_; w++) {
			map_[h * width_ + w] = csv.GetInt(w, h);
		}
	}
	

	//旗や木などのオブジェクトの設置
	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			switch (csv.GetInt(w, h + height_ + 1))
			{
			case 1://flag
			{
				Flag* sFlag = Instantiate<Flag>(GetParent());
				sFlag->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				break;
			}
			case 14://Tree
			{
				Tree* sTree = Instantiate<Tree>(GetParent());
				sTree->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				break;
			}
			case 30://Cloud
			{
				Cloud* sCloud = Instantiate<Cloud>(GetParent());
				sCloud->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				break;
			}
			}
		}
	}

	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			switch (csv.GetInt(w, h + height_ + 1))
			{
			case 0://player
			{
				//Player* sPlayer = GetParent()->FindGameObject<Player>();
				Player* sPlayer = Instantiate<Player>(GetParent());
				sPlayer->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				//とりあえずのマップ変更用
				switch (mapNo_)
				{
				case 2:
					sPlayer->SetGravity(1.62 / 90.0f);
					break;
				case 3:
					sPlayer->SetGravity(3.71 / 90.0f);
					break;
				default:
					break;
				}
				break;
			}
			case 5: //Enemy
			{
				Enemy* sEnemy = Instantiate<Enemy>(GetParent());
				sEnemy->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				if (mapNo_ == 2) {
					sEnemy->SetGravity(1.62 / 90.0f);
				}
				else if (mapNo_ == 3) {
					sEnemy->SetGravity(3.71 / 90.0f);
				}
				break;
			}
			case 10: //FlyEnemy
			{
				FlyEnemy* fEnemy = Instantiate<FlyEnemy>(GetParent());
				fEnemy->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				break;
			}
			case 15://Meteorite左向き
			{
				Meteorite* sMeteo = Instantiate<Meteorite>(GetParent());

				sMeteo->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				/*if (mapNo_ == 2) {
					sMeteo->SetGravity(1.62 / 90.0f);
				}
				else if (mapNo_ == 3) {
					sMeteo->SetGravity(3.71 / 90.0f);
				}*/
				sMeteo->SetMoveType(0);
				break;
			}
			case 16://Meteorite右向き
			{
				Meteorite* sMeteo = Instantiate<Meteorite>(GetParent());

				sMeteo->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				/*if (mapNo_ == 2) {
					sMeteo->SetGravity(1.62 / 90.0f);
				}
				else if (mapNo_ == 3) {
					sMeteo->SetGravity(3.71 / 90.0f);
				}*/
				sMeteo->SetMoveType(1);
				break;
			}
			case 17://Meteorite下向き
			{
				Meteorite* sMeteo = Instantiate<Meteorite>(GetParent());

				sMeteo->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				/*if (mapNo_ == 2) {
					sMeteo->SetGravity(1.62 / 90.0f);
				}
				else if (mapNo_ == 3) {
					sMeteo->SetGravity(3.71 / 90.0f);
				}*/
				sMeteo->SetMoveType(2);
				break;
			}
			case 20://MeteoSpawner
				MeteoSpawner * meteoS = Instantiate<MeteoSpawner>(GetParent());
				meteoS->SetPosition(w* CHIP_SIZE, h* CHIP_SIZE);
				break;
			}
		}
	}
}


int Stage::CollisionRight(int x, int y)
{
	if (IsWallBlock(x, y)) {
		//当たっているので、めり込んだ量を返す
		return x % CHIP_SIZE + 1;
	}
	else
		return 0;
}

int Stage::CollisionLeft(int x, int y)
{
	if (IsWallBlock(x, y)) {
		//当たっているので、めり込んだ量を返す
		return (CHIP_SIZE - x % CHIP_SIZE) + 1;
	}
	else
		return 0;
}

int Stage::CollisionDown(int x, int y)
{
	if (IsWallBlock(x, y)) {
		//当たっているので、めり込んだ量を返す
		return y % CHIP_SIZE + 1;
	}
	else
		return 0;
}

int Stage::CollisionUp(int x, int y)
{
	if (IsWallBlock(x, y)) {
		//当たっているので、めり込んだ量を返す
		return (CHIP_SIZE - y % CHIP_SIZE) + 1;
	}
	else
		return 0;
}

void Stage::BreakGround(int x, int y)
{
	int chipX = x / CHIP_SIZE;
	int chipY = y / CHIP_SIZE;

	//destructionChip:壊す基準のチップの場所
	int desChip = chipY * width_ + chipX;
	//destructionRange:壊す範囲
	int desRange = 3;
	int desRangeY = 7;

	//隕石が当たったところらへんを壊す

	for (int i = 0; i < 6; i++) {
		map_[desChip + width_ * i] = CHIP_NULL;
		if (i < 4) {
			map_[desChip + width_ * i + 1] = CHIP_NULL;
			map_[desChip + width_ * i - 1] = CHIP_NULL;
			if (i < 2) {
				map_[desChip + width_ * i - 2] = CHIP_NULL;
				map_[desChip + width_ * i + 2] = CHIP_NULL;
			}
		}
	}
}

bool Stage::IsWallBlock(int x, int y)
{
	//地面などの当たり判定のあるチップなのか(ステージごとに変えてもあり)
	int chipX = x / CHIP_SIZE;
	int chipY = y / CHIP_SIZE;
	switch (map_[chipY * width_ + chipX]) {
	case 2://土の左上
	case 3://土の右上
	case 18://土の左下
	case 19://土の右下
	case 32://地面の左上
	case 33://地面の右上
	case 48://地面の左下
	case 49://地面の右下
	case 160://浮いてる足場(右)の左上
	case 161://浮いてる足場(右)の右上
	case 192://浮いてる足場(中央)の左上
	case 193://浮いてる足場(中央)の右上
	case 224://浮いてる足場(左)の左上
	case 225://浮いてる足場(左)の右上
	case 520://橋の左
	case 521://橋の右
		//月の方のマップチップ
	case 134://地中の左上
	case 135://地中の右上
	case 150://地中の左下
	case 151://地中の右下
	case 164://地面(表面)の左上
	case 165://地面(表面)の右上
	case 180://地面(表面)の左下
	case 181://地面(表面)の右下
	case 292://浮いてる足場(右)の左上
	case 293://〃の右上
	case 324://浮いてる足場(中央)の左上
	case 325://〃の右上
	case 356://浮いてる足場(左)の左上
	case 357://〃の右上
		//火星の方のマップチップ
	case 4://地面(表面)の左上
	case 5://〃の右上
	case 20://〃の左下
	case 21://〃の右下
	case 0://地中の左上
	case 1://〃の右上
	case 16://〃の左下
	case 17://〃の右下
	case 64://地中2の左上
	case 65://〃の右上
	case 80://〃の左下
	case 81://〃の右下
		//新しい地面の当たり判定
	case 46:
	case 47:
		return true;
	}
	return false;
}
