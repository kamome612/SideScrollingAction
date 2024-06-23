#include "PlayScene.h"
#include "Player.h"
#include "Stage.h"
#include "Camera.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent,"PlayScene"),pPict_(-1)
{
}

void PlayScene::Initialize()
{
	//画像のロード(背景)
	//pPict_ = LoadGraph("Assets\\Picture\\playscene.png");//拾ってきた画像がwindowサイズに合ってなかった(探すこと)
	//assert(pPict_ > 0);
	//Stage* pStage = Instantiate<Stage>(this);
	//pStage->Reset();
	state_ = S_Play;
	Instantiate<Camera>(this);
	Stage* pStage = Instantiate<Stage>(this);
	Instantiate<Player>(this);
	pStage->Reset();
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
