#include "TitleScene.h"
#include "Engine/Scenemanager.h"

TitleScene::TitleScene(GameObject* parent)
	:GameObject(parent,"TitleScene"),tPict_(-1)
{
}

void TitleScene::Initialize()
{
	//画像のロード
	tPict_ = LoadGraph("Assets\\Picture\\title.jpg");
	assert(tPict_ >= 0);
}

void TitleScene::Update()
{
	//プレイシーンへの遷移(仮だから後で変える)
	if (CheckHitKey(KEY_INPUT_P)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

void TitleScene::Draw()
{
	//画像の表示
	DrawGraph(0, 0, tPict_, TRUE);
}

void TitleScene::Release()
{
}
