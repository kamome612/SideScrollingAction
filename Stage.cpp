#include "Stage.h"
#include "Engine/CsvReader.h"
#include "Player.h"
#include "Meteorite.h"
#include "Camera.h"

namespace {
	const int CHIP_SIZE = 32;
	const int MAP_WIDTH = 1280;
	const int MAP_HEIGHT = 720;
}

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage"),hImage_(-1),width_(0),height_(0),
	                            mapNo_(1),prevResetKey_(false),gPict_(-1)
{
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
	////画像の読み込み
	//hImage_ = LoadGraph("Assets/Image/bgchar.png");
	//assert(hImage_ > 0);
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

	//ステージの描画
	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			int chip = map_[h * width_ + w];
			DrawRectGraph(w * CHIP_SIZE-scroll, h * CHIP_SIZE, 
				          CHIP_SIZE * (chip % 16), CHIP_SIZE * (chip / 16), 
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
	bool ret = csv.Load("Assets/Stage/testStage.csv");
	assert(ret);

	width_ = csv.GetWidth(0);
	height_ = csv.GetHeight();
	map_ = new int[height_ * width_];

	for (int h = 0; h < height_; h++) {
		if (csv.GetString(0, h) == "") {
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
				Player* sPlayer = GetParent()->FindGameObject<Player>();
				//sPlayer_ = Instantiate<Player>(this);
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
			case 15://Meteorite
				Meteorite * sMeteo = Instantiate<Meteorite>(GetParent());
				//sMeteo_ = Instantiate<Meteorite>(this);
				sMeteo->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
				break;
			}
		}
	}
}

void Stage::StageReset()
{
	//マップの中になにか入ってたら消す
	if (map_ != nullptr) {
		delete[] map_;
		map_ = nullptr;
	}

	static const std::string folder = "Assets/Stage/";
	
	//画像の読み込み
	std::string n = std::to_string(mapNo_);
	//hImage_ = LoadGraph((folder + "bgchar" + n + ".png").c_str());
	hImage_ = LoadGraph("Assets/Stage/spritesheet_ground.png");
	assert(hImage_ > 0);
	//csvから読み込み
	CsvReader csv;
	bool ret = csv.Load((folder +"testStage"+ n + ".csv").c_str());
	//bool ret = csv.Load("Assets/Stage/testStage2.csv");
	assert(ret);
	width_ = csv.GetWidth(0);
	height_ = csv.GetHeight();
	map_ = new int[height_ * width_];

	for (int h = 0; h < height_; h++) {
		if (csv.GetString(0, h) == "") {
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
				Player* sPlayer = GetParent()->FindGameObject<Player>();
				sPlayer->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
			}
			case 15://Meteorite
				Meteorite * sMeteo = Instantiate<Meteorite>(GetParent());
				sMeteo->SetPosition(w * CHIP_SIZE, h * CHIP_SIZE);
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
		return x % CHIP_SIZE - 28;
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

//仮で作る
int Stage::StageHitCheck(float x, float y, float* moveX, float* moveY)
{
	//afterX,afterY
	float afX, afY;

	// 移動量を足す
	afX = x + *moveX;
	afY = y + *moveY;

	// 当たり判定のあるブロックに当たっているかチェック
	if (GetChipParam(afX, afY) == 1)
	{
		//blockのleftX,topY,rightX,bottomYの座標
		float blx, bty, brx, bby;

		// 当たっていたら壁から離す処理を行う

		// ブロックの上下左右の座標を算出
		blx = (float)((int)afX / CHIP_SIZE) * CHIP_SIZE;        // 左辺の X 座標
		brx = (float)((int)afX / CHIP_SIZE + 1) * CHIP_SIZE;    // 右辺の X 座標

		bty = (float)((int)afY / CHIP_SIZE) * CHIP_SIZE;        // 上辺の Y 座標
		bby = (float)((int)afY / CHIP_SIZE + 1) * CHIP_SIZE;    // 下辺の Y 座標

		// 上辺に当たっていた場合
		if (*moveY > 0.0F)
		{
			// 移動量を補正する
			*moveY = bty - y - 1.0F;

			// 上辺に当たったと返す
			return 3;
		}

		// 下辺に当たっていた場合
		if (*moveY < 0.0F)
		{
			// 移動量を補正する
			*moveY = bby - y + 1.0F;

			// 下辺に当たったと返す
			return 4;
		}

		// 左辺に当たっていた場合
		if (*moveX > 0.0F)
		{
			// 移動量を補正する
			*moveX = blx - x - 1.0F;

			// 左辺に当たったと返す
			return 1;
		}

		// 右辺に当たっていた場合
		if (*moveX < 0.0F)
		{
			// 移動量を補正する
			*moveX = brx - x + 1.0F;

			// 右辺に当たったと返す
			return 2;
		}

		// ここに来たら適当な値を返す
		return 4;
	}

	// どこにも当たらなかったと返す
	return 0;
}


bool Stage::IsWallBlock(int x, int y)
{
	int chipX = x / CHIP_SIZE;
	int chipY = y / CHIP_SIZE;
	//if (mapNo_ == 1) {
		switch (map_[chipY * width_ + chipX]) {
		case 0:
		case 1:
		case 10:
		case 11:
		case 16:
		case 17:
		case 32:
		case 33:
		case 34:
		case 35:
		case 64:
		case 65:
	    //月の方のマップチップ
		case 132:
		case 133:
		case 148:
		case 149:
		case 164:
		case 165:
		case 180:
		case 181:
		case 196:
		case 197:
		case 212:
		case 213:
		case 292:
		case 293:
		case 324:
		case 325:
		case 356:
		case 357:
			return true;
		}
		return false;
	//}
}

// マップチップの値を取得する関数
int Stage::GetChipParam(float _x, float _y)
{
	int x, y;

	// 整数値へ変換
	x = (int)_x / CHIP_SIZE;
	y = (int)_y / CHIP_SIZE;

	// マップからはみ出ていたら 0 を返す
	if (x >= MAP_WIDTH || y >= MAP_HEIGHT || x < 0 || y < 0) return 0;

	// 指定の座標に該当するマップの情報を返す
	return map_[y * width_  + x];
}
