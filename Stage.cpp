#include "Stage.h"
#include "Engine/CsvReader.h"
#include "Player.h"
#include "Meteorite.h"
#include "Camera.h"

namespace {
	//const int CHIP_SIZE = 32;
	const int CHIP_SIZE = 64;
	const int MAP_WIDTH = 1280;
	const int MAP_HEIGHT = 720;
	const int CHIP_NULL = 255;
}

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage")
{
	hImage_ = -1;
	width_ = 0;
	height_ = 0;
	mapNo_ = 1;
	prevResetKey_ = false;
	gPict_ = -1;
	meteoHitCount_ = 0;
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
	if (mapNo_ == 1) {
		DrawGraph(5, -250, gPict_, TRUE);
	}
	else if (mapNo_ == 2) {
		DrawGraph(0, 0, gPict_, TRUE);
	}

	int scroll = 0;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		scroll = cam->GetValue();
	}

	////ステージの描画
	//for (int h = 0; h < height_; h++) {
	//	for (int w = 0; w < width_; w++) {
	//		int chip = map_[h * width_ + w];
	//		DrawRectGraph(w * CHIP_SIZE-scroll, h * CHIP_SIZE, 
	//			          CHIP_SIZE * (chip % 16), CHIP_SIZE * (chip / 16), 
	//			          CHIP_SIZE, CHIP_SIZE, hImage_, TRUE, FALSE);
	//	}
	//}

	//ステージの描画(仮)
	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			int chip = map_[h * width_ + w];
			DrawRectGraph(w * CHIP_SIZE - scroll, h * CHIP_SIZE,
				CHIP_SIZE * (chip % 8), CHIP_SIZE * (chip / 8),
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
	//プレイヤーと隕石がステージのリセットやセット時に消されるように
	if (Player* sPlayer = GetParent()->FindGameObject<Player>()) {
		sPlayer->KillMe();
	}
	if (Meteorite* sMeteo = GetParent()->FindGameObject<Meteorite>()) {
		sMeteo->KillMe();
	}

	Camera* cam = GetParent()->FindGameObject<Camera>();
	cam->SetValue(0);
	static const std::string folder = "Assets/Stage/";

	//画像の読み込み
	std::string n = std::to_string(mapNo_);
	//hImage_ = LoadGraph((folder + "bgchar" + n + ".png").c_str());
	hImage_ = LoadGraph("Assets/Stage/spritesheet_ground.png");
	assert(hImage_ > 0);

	//バックグランドの画像読み込み
	gPict_ = LoadGraph(("Assets/Picture/background" + n + ".png").c_str());
	assert(gPict_ > 0);

	//csvから読み込み
	CsvReader csv;
	//bool ret = csv.Load((folder + "testStage" + n + ".csv").c_str());
	bool ret = csv.Load("Assets/Stage/testStage0.csv");
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
	

	//Mapデータの中で0があれば、Playerの座標を0の位置にする
	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			switch (csv.GetInt(w, h + height_ + 1))
			{
			case 0://player
			{
				//Player* sPlayer = GetParent()->FindGameObject<Player>();
				Player* sPlayer = Instantiate<Player>(GetParent());
				sPlayer ->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
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
			case 15://Meteorite
				Meteorite * sMeteo = Instantiate<Meteorite>(GetParent());
				//sMeteo_ = Instantiate<Meteorite>(this);
				sMeteo->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				break;
			}
		}
	}
}

//void Stage::StageReset()
//{
//	//マップの中になにか入ってたら消す
//	if (map_ != nullptr) {
//		delete[] map_;
//		map_ = nullptr;
//	}
//
//	static const std::string folder = "Assets/Stage/";
//	
//	//画像の読み込み
//	std::string n = std::to_string(mapNo_);
//	//hImage_ = LoadGraph((folder + "bgchar" + n + ".png").c_str());
//	hImage_ = LoadGraph("Assets/Stage/spritesheet_ground.png");
//	assert(hImage_ > 0);
//	//csvから読み込み
//	CsvReader csv;
//	bool ret = csv.Load((folder +"testStage"+ n + ".csv").c_str());
//	//bool ret = csv.Load("Assets/Stage/testStage2.csv");
//	assert(ret);
//	width_ = csv.GetWidth(0);
//	height_ = csv.GetHeight();
//	map_ = new int[height_ * width_];
//
//	for (int h = 0; h < height_; h++) {
//		if (csv.GetString(0, h) == "") {
//			height_ = h;
//			break;
//		}
//		for (int w = 0; w < width_; w++) {
//			map_[h * width_ + w] = csv.GetInt(w, h);
//		}
//	}
//
//	//Mapデータの中で0があれば、Playerの座標を0の位置にする
//	for (int h = 0; h < height_; h++) {
//		for (int w = 0; w < width_; w++) {
//			switch (csv.GetInt(w, h + height_ + 1))
//			{
//			case 0://player
//			{
//				//Player* sPlayer = GetParent()->FindGameObject<Player>();
//				Player* sPlayer = Instantiate<Player>(GetParent());
//				sPlayer->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
//			}
//			case 15://Meteorite
//				Meteorite * sMeteo = Instantiate<Meteorite>(GetParent());
//				sMeteo->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
//				break;
//			}
//		}
//	}
//}

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
		//return x % CHIP_SIZE - 28;
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
		return y % CHIP_SIZE - 25;
	}
	else
		return 0;
}

void Stage::BreakGround(int x, int y)
{
	//隕石の当たった回数を増やす
	meteoHitCount_ += 1;

	int chipX = x / CHIP_SIZE;
	int chipY = y / CHIP_SIZE;

	//destructionChip:壊す基準のチップの場所
	int desChip = chipY * width_ + chipX;
	//destructionRange:壊す範囲
	int desRange = 5;

	//隕石が当たったところらへんを壊す
	for (int i = 0; i <= desRange; i++) {
		map_[desChip - width_ + i] = CHIP_NULL;
		map_[desChip + i] = CHIP_NULL;
		if (i > 0 && i < desRange) {
			map_[desChip + width_ + i] = CHIP_NULL;
		}
	}
}

bool Stage::IsWallBlock(int x, int y)
{
	//地面などの当たり判定のあるチップなのか(ステージごとに変えてもあり)
	int chipX = x / CHIP_SIZE;
	int chipY = y / CHIP_SIZE;
	//switch (map_[chipY * width_ + chipX]) {
	//case 2://土の左上
	//case 3://土の右上
	//case 18://土の左下
	//case 19://土の右下
	//case 32://地面の左上
	//case 33://地面の右上
	//case 48://地面の左下
	//case 49://地面の右下
	//case 160://浮いてる足場(右)の左上
	//case 161://浮いてる足場(右)の右上
	//case 192://浮いてる足場(中央)の左上
	//case 193://浮いてる足場(中央)の右上
	//case 224://浮いてる足場(左)の左上
	//case 225://浮いてる足場(左)の右上
	//	//月の方のマップチップ
	//case 132:
	//case 133:
	//case 148:
	//case 149:
	//case 164:
	//case 165:
	//case 180:
	//case 181:
	//case 196:
	//case 197:
	//case 212:
	//case 213:
	//case 292:
	//case 293:
	//case 324:
	//case 325:
	//case 356:
	//case 357:
	//	return true;
	//}
	//return false;

	switch (map_[chipY * width_ + chipX]) {
	case 8:
	case 1:
		return true;
	}
	return false;
}
