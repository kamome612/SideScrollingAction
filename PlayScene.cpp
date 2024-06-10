#include "PlayScene.h"
#include "Player.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent,"PlayScene"),pPict_(-1)
{
}

void PlayScene::Initialize()
{
	//画像のロード(背景)
	//pPict_ = LoadGraph("Assets\\Picture\\universe.jpg");//拾ってきた画像がwindowサイズに合ってなかった(探すこと)
	//assert(pPict_ > 0);
	Instantiate<Player>(this);
}

void PlayScene::Update()
{
}

void PlayScene::Draw()
{
	//DrawGraph(0, 0, pPict_, FALSE);
}

void PlayScene::Release()
{
}
