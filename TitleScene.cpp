#include "TitleScene.h"
#include "Engine/Scenemanager.h"

TitleScene::TitleScene(GameObject* parent)
	:GameObject(parent,"TitleScene"),tPict_(-1),select_(0)
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
	/*if (CheckHitKey(KEY_INPUT_P)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}*/

	//上限を超えないように
	if (select_ > 0) {
		if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W)) {//上が押されたら
			if (!prevUpKey_) {
				select_ -= 1;
			}
			prevUpKey_ = true;
		}
		else
			prevUpKey_ = false;
	}
	//下限を超えないように
	if (select_ < 2) {
		if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S)) {//下が押されたら
			if (!prevDownKey_) {
				select_ += 1;
			}
			prevDownKey_ = true;
		}
		else
			prevDownKey_ = false;
	}

	//エンターで決定!
	if (CheckHitKey(KEY_INPUT_RETURN)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		switch (select_) {
		case 0:
			pSceneManager->ChangeScene(SCENE_ID_PLAY,true);
			break;
		case 1:
			pSceneManager->ChangeScene(SCENE_ID_TITLE);
			break;
		case 2:
			DxLib_End();
		}
	}
}

void TitleScene::Draw()
{
	//画像の表示
	DrawGraph(0, 0, tPict_, TRUE);
	//選択するときの三角
	int tmp = 440 + select_ * 60;
	DrawTriangle(480, tmp, 480 - 30, tmp + 20, 480 - 30, tmp - 20,
		GetColor(255, 255, 0), TRUE);
}

void TitleScene::Release()
{
}
