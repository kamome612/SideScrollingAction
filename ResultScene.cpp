#include "ResultScene.h"
#include "Engine/SceneManager.h"

ResultScene::ResultScene(GameObject* parent)
	:GameObject(parent,"ResultScene"),rPict_(-1),isClear_(false)
{
}

void ResultScene::Initialize()
{
	if (isClear_) {
		rPict_ = LoadGraph("Assets/Picture/");
	}
	else {
		rPict_ = LoadGraph("Assets/Picture/gameover.png");
	}
	assert(rPict_ > 0);
}

void ResultScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {//�^�C�g���ɖ߂�
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
	else if (CheckHitKey(KEY_INPUT_P)) {//�X�e�[�W�I���ɖ߂�(�v���C�V�[������)
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

void ResultScene::Draw()
{
	DrawGraph(0, 0, rPict_, TRUE);
}

void ResultScene::Release()
{
	if (rPict_ > 0) {
		DeleteGraph(rPict_);
		rPict_ = -1;
	}
}
