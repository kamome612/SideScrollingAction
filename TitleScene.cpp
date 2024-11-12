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
	SceneManager* scenemanager = (SceneManager*)FindObject("SceneManager");
	prevEnterKey_ = scenemanager->keyFlag_;
}

void TitleScene::Update()
{
	//コントローラの情報とる
	DINPUT_JOYSTATE input;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	//上限を超えないように
	if (select_ > 0) {
		if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W)
			|| input.Y <= -500 || input.POV[0] == 0) {//上が押されたら
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
		if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S) 
			|| input.Y >= 500 || input.POV[0] == 18000) {//下が押されたら
			if (!prevDownKey_) {
				select_ += 1;
			}
			prevDownKey_ = true;
		}
		else
			prevDownKey_ = false;
	}

	//エンターで決定!
	if (CheckHitKey(KEY_INPUT_RETURN)|| (input.Buttons[1] & 0x80) != 0) {
		if (!prevEnterKey_) {
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			switch (select_) {
			case 0:
				pSceneManager->ChangeScene(SCENE_ID_PLAY, true);
				break;
			case 1:
				pSceneManager->ChangeScene(SCENE_ID_EXPLANATION,true);
				break;
			case 2:
				DxLib_End();
			}
		}
		prevEnterKey_ = true;
	}
	else {
		prevEnterKey_ = false;
	}
}

void TitleScene::Draw()
{
	//画像の表示
	DrawGraph(0, 0, tPict_, TRUE);

	//選択するときの三角描画
	int tmp = 410 + select_ * 110;
	int len = 430;
	DrawTriangle(len, tmp, len - 30, tmp + 20, len - 30, tmp - 20,
		GetColor(255, 255, 0), TRUE);
}

void TitleScene::Release()
{
	if (tPict_ > 0) {
		DeleteGraph(tPict_);
	}
}
