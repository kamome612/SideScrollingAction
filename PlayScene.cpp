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
	//�摜�̃��[�h(�w�i)
	//pPict_ = LoadGraph("Assets\\Picture\\universe.jpg");//�E���Ă����摜��window�T�C�Y�ɍ����ĂȂ�����(�T������)
	//assert(pPict_ > 0);
	//Stage* pStage = Instantiate<Stage>(this);
	//pStage->Reset();
	Instantiate<Camera>(this);
	Stage* pStage = Instantiate<Stage>(this);
	pStage->Reset();
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
