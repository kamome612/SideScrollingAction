#include "GameOverScene.h"

GameOverScene::GameOverScene(GameObject* parent)
	:GameObject(parent,"GameOverScene"),gPict_(-1)
{
}

void GameOverScene::Initialize()
{
	gPict_ = LoadGraph("Assets/Picture/gameover.png");
	assert(gPict_ > 0);
}

void GameOverScene::Update()
{
}

void GameOverScene::Draw()
{
	DrawGraph(0, 0, gPict_, TRUE);
}

void GameOverScene::Release()
{
	if (gPict_ > 0) {
		DeleteGraph(gPict_);
		gPict_ = -1;
	}
}
