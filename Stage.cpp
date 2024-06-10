#include "Stage.h"
#include "Camera.h"
#include "Player.h"
#include "Engine/CsvReader.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage")
{
	sImage_ = LoadGraph("Assets\\Image\\bgchar.png");//ステージの画像読み込み
	assert(sImage_ > 0);
}

Stage::Stage(GameObject* parent, int _number)
{
	//sImage_ = LoadGraph("～_number");//渡された値に応じたステージデータをロードする
}

Stage::~Stage()
{
	if (sImage_ > 0) {
		DeleteGraph(sImage_);
	}
	if (Map_ != nullptr) {
		delete[] Map_;
	}
}

void Stage::Reset()
{
	if (Map_ != nullptr) {
		delete[] Map_;
		Map_ = nullptr;
	}
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load("");
	assert(ret);
	width_ = csv.GetWidth();
	height_ = csv.GetHeight();
	Map_ = new int[width_ * height_];

	for (int h = 0; h < height_; h++) {
		if (csv.GetString(0, h) == "") {
			height_ = h;
			break;
		}
		for (int w = 0; w < width_; w++) {
			Map_[h * width_ + w] = csv.GetValue(w,h);
		}
	}
}

void Stage::Update()
{
}

void Stage::Draw()
{
}
