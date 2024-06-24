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
	//pPict_ = LoadGraph("Assets\\Picture\\playscene.png");//�E���Ă����摜��window�T�C�Y�ɍ����ĂȂ�����(�T������)
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
	switch (state_) {
	case S_Ready:UpdateReady(); break;
	case S_Play:UpdatePlay(); break;
	case S_Clear: UpdateClear(); break;
	case S_GameOver: UpdateGameOver(); break;
	}
}

void PlayScene::Draw()
{
	//DrawGraph(0, 0, pPict_, FALSE);
}

void PlayScene::Release()
{
}

void PlayScene::StartReady()
{
	state_ = S_Ready;
}

void PlayScene::UpdateReady()
{
}

void PlayScene::StartPlay()
{
	state_ = S_Play;
}

void PlayScene::UpdatePlay()
{
}

void PlayScene::UpdateClear()
{
}

void PlayScene::StartGameOver()
{
	state_ = S_GameOver;
}

void PlayScene::UpdateGameOver()
{

}
