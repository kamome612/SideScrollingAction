#include "PlayScene.h"
#include "Player.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent,"PlayScene"),pPict_(-1)
{
}

void PlayScene::Initialize()
{
	//‰æ‘œ‚Ìƒ[ƒh(”wŒi)
	//pPict_ = LoadGraph("");//E‚Á‚Ä‚«‚½‰æ‘œ‚ªwindowƒTƒCƒY‚É‡‚Á‚Ä‚È‚©‚Á‚½(’T‚·‚±‚Æ)
	//assert(pPict_ > 0);
	Instantiate<Player>(this);
}

void PlayScene::Update()
{
}

void PlayScene::Draw()
{
	DrawGraph(0, 0, pPict_, FALSE);
}

void PlayScene::Release()
{
}
