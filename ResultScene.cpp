#include "ResultScene.h"
#include "Engine/SceneManager.h"
#include "PlayScene.h"

ResultScene::ResultScene(GameObject* parent)
	:GameObject(parent,"ResultScene"),rPict_(-1),isClear_(false)
{
	SceneManager* scenemanager = (SceneManager*)FindObject("SceneManager");
	isClear_ = scenemanager->tmp_;
}

void ResultScene::Initialize()
{
	if (isClear_) {
		rPict_ = LoadGraph("Assets/Picture/clear.png");
	}
	else {
		rPict_ = LoadGraph("Assets/Picture/gameover.png");
	}
	assert(rPict_ > 0);
}

void ResultScene::Update()
{
	if (CheckHitKey(KEY_INPUT_R)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
		//PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
		//scene->StartReady();
	}
	else if (CheckHitKey(KEY_INPUT_T)) {//タイトルに戻る
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
	else if (CheckHitKey(KEY_INPUT_P)) {//ステージ選択に戻る(プレイシーン内の)
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
