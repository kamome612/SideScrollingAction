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
	Map_ = nullptr;
}

Stage::Stage(GameObject* parent, int _number)
{
	//仮の画像(後で拾ってきた素材に変える)
	//sImage_ = LoadGraph("～,_number");//渡された値に応じたステージデータをロードする
	sImage_ = LoadGraph("Assets\\Image\\bgchar.png");//ステージの画像読み込み
	assert(sImage_ > 0);
	Map_ = nullptr;
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
	//if (Map_ != nullptr) {
	//	delete[] Map_;
	//	Map_ = nullptr;
	//}
	//CsvReader csv;//データを読むクラスのインスタンスを作成
	//bool ret = csv.Load("Assets\\Image\\stage1.csv");
	//assert(ret);
	//width_ = csv.GetWidth();
	//height_ = csv.GetHeight();
	//Map_ = new int[width_ * height_];

	//for (int h = 0; h < height_; h++) {
	//	if (csv.GetString(0, h) == "") {
	//		height_ = h;
	//		break;
	//	}
	//	for (int w = 0; w < width_; w++) {
	//		Map_[h * width_ + w] = csv.GetValue(w, h);
	//	}
	//}
	////Mapデータの中で0があれば、Playerの座標を0の位置にする
	//for (int h = 0; h < height_; h++) {
	//	for (int w = 0; w < width_; w++) {
	//		switch (csv.GetValue(w, h + height_ + 1))
	//		{
	//		case 0://player
	//		{
	//			Player* pplayer = GetParent()->FindGameObject<Player>();
	//			pplayer->SetPosition(w * 32, h * 32);
	//		}
	//		break;
	//		}
	//	}
	//}

	if (Map_ != nullptr) {
		delete[] Map_;
		Map_ = nullptr;
	}
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load("Assets\\Image\\stage1.csv");
	assert(ret);
	width_ = csv.GetWidth();
	height_ = csv.GetHeight();
	Map_ = new int[width_ * height_];//C言語での動的二次元配列の取り方

	for (int h = 0; h < height_; h++) {
		if (csv.GetString(0, h) == "") {
			height_ = h;
			break;
		}
		for (int w = 0; w < width_; w++) {
			Map_[h * width_ + w] = csv.GetValue(w, h);
		}
	}
	//Mapデータの中で0があれば、Playerの座標を0の位置にする
	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			switch (csv.GetValue(w, h + height_ + 1))
			{
			case 0://player
			{
				Player* pplayer = GetParent()->FindGameObject<Player>();
				pplayer->SetPosition(w * 32, h * 32);
			}
			break;
			case 1://bird
			{
				/*Bird* pbird = Instantiate<Bird>(GetParent());
				pbird->SetPosition(w * 32, h * 32);*/
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
			int chip = Map_[y * width_ + x];
			DrawRectGraph(x * 32 - scroll, y * 32, 32 * (chip % 16), 32 * (chip / 16), 32, 32, sImage_, TRUE);
		}
	}
}
