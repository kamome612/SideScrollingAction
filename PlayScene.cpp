#include "PlayScene.h"
#include "Player.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent,"PlayScene"),pPict_(-1)
{
}

void PlayScene::Initialize()
{
	//�摜�̃��[�h(�w�i)
	//pPict_ = LoadGraph("");//�E���Ă����摜��window�T�C�Y�ɍ����ĂȂ�����(�T������)
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
