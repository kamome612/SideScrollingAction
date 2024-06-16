#include "Stage.h"
#include "Engine/CsvReader.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage"),hImage_(-1),width_(0),height_(0)
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	hImage_ = LoadGraph("Assets/Image/bgchar.png");
	assert(hImage_ > 0);
	map_ = nullptr;
}

void Stage::Update()
{
	if (map_ != nullptr) {
		delete[] map_;
		map_ = nullptr;
	}

	CsvReader csv;
	bool ret = csv.Load("Assets/Image/stage1.csv");
	assert(ret);
	width_ = csv.GetWidth();
	height_ = csv.GetHeight();
	map_ = new int[height_ * width_];

	for (int h = 0; h < height_; h++) {
		if (csv.GetString(0, h) == "") {
			height_ = h;
			break;
		}
		for (int w = 0; w < width_; w++) {
			map_[h * width_ + w] = csv.GetValue(w, h);
		}
	}
}

void Stage::Draw()
{
	/*for (int h = 0; h < MAP_HEIGHT; h++) {
		for (int w = 0; w < MAP_WIDTH; w++) {
			int chip = MapData[h][w];
			DrawRectGraph(w * 32, h * 32, 32 * (chip % 16), 32 * (chip / 16), 32, 32, hImage_, TRUE, FALSE);
		}
	}*/

	for (int h = 0; h < height_; h++) {
		for (int w = 0; w < width_; w++) {
			int chip = map_[h * width_ + w];
			DrawRectGraph(w * 32, h * 32, 32 * (chip % 16), 32 * (chip / 16), 32, 32, hImage_, TRUE, FALSE);
		}
	}
}
