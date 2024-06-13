#include "Stage.h"
#include "Camera.h"
#include "Player.h"
#include "Engine/CsvReader.h"
#include <assert.h>

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage")
{
	//仮の画像(後で拾ってきた素材に変える)
	sImage_ = LoadGraph("Assets\\Image\\bgchar.png");//ステージの画像読み込み
	assert(sImage_ > 0);
	map_ = nullptr;
}

Stage::Stage(GameObject* parent, int _number)
{
	//仮の画像(後で拾ってきた素材に変える)
	//sImage_ = LoadGraph("～,_number");//渡された値に応じたステージデータをロードする
	sImage_ = LoadGraph("Assets\\Image\\bgchar.png");//ステージの画像読み込み
	assert(sImage_ > 0);
	map_ = nullptr;
}

Stage::~Stage()
{
	if (sImage_ > 0) {
		DeleteGraph(sImage_);
	}
	if (map_ != nullptr) {
		delete[] map_;
	}
}

void Stage::Reset()
{
	if (map_ != nullptr) {
		delete[] map_;
		map_ = nullptr;
	}
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load("Assets/image/stage1.csv");
	assert(ret);
	width_ = csv.GetWidth();
	height_ = csv.GetHeight();
	map_ = new int[width_ * height_];

	for (int h = 0; h < height_; h++) {
		if (csv.GetString(0, h) == "") {
			height_ = h;
			break;
		}
		for (int w = 0; w < width_; w++) {
			map_[h * width_ + w] = csv.GetValue(w, h);
		}
	}
	//mapデータの中で0があれば、playerの座標を0の位置にする
	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			switch (csv.GetValue(w, h + height_ + 1))
			{
			case 0://player
			{
				Player* pPlayer = GetParent()->FindGameObject<Player>();
				pPlayer->SetPosition(w * 32, h * 32);
			}
			break;
			}
		}
	}
}

void Stage::Update()
{
	if (CheckHitKey(KEY_INPUT_R))
		Reset();
}

void Stage::Draw()
{
	int scroll = 0;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		scroll = cam->GetValue();
	}

	for (int y = 0; y < height_; y++) {
		for (int x = 0; x < width_; x++) {
			int chip = map_[y * width_ + x];
			DrawRectGraph(x * 32 - scroll, y * 32, 32 * (chip % 16), 32 * (chip / 16), 32, 32, sImage_, TRUE);
		}
	}
}
