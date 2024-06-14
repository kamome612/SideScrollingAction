#include "Stage.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage"),hImage_(-1)
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	hImage_ = LoadGraph("Assets/bgchar.png");
	assert(hImage_ > 0);
}

void Stage::Update()
{
}

void Stage::Draw()
{
	for (int h = 0; h < MAP_HEIGHT; h++) {
		for (int w = 0; w < MAP_WIDTH; w++) {
			int chip = MapData[w][h];
			DrawRectGraph(w * 32, h * 32, 32 * (chip & 16), 32 * (chip / 16), 32, 32, hImage_, TURE, FALSE)
		}
	}
}
