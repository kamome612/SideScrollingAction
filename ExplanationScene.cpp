#include "ExplanationScene.h"
#include "Engine/SceneManager.h"
ExplanationScene::ExplanationScene(GameObject* parent)
	:GameObject(parent, "ExplanationScene"),select_(0),ePict_(-1), 
	 prevUpKey_(false), prevDownKey_(false),prevEnterKey_(false),SoundFlag_(false)
{
}

void ExplanationScene::Initialize()
{
	ePict_ = LoadGraph("Assets/Picture/explanation.jpg");
	assert(ePict_ > 0);
	SceneManager* scenemanager = (SceneManager*)FindObject("SceneManager");
	prevEnterKey_ = scenemanager->keyFlag_;
	sHandle_ = LoadSoundMem("Assets/Sound/SelectSound.mp3");
	eHandle_ = LoadSoundMem("Assets/Sound/EnterSound.mp3");
	rHandle_ = LoadSoundMem("Assets/Sound/ReturnSound.mp3");
	ChangeVolumeSoundMem(255 * 70 / 100, sHandle_);
	ChangeVolumeSoundMem(255 * 70 / 100, eHandle_);
	ChangeVolumeSoundMem(255 * 70 / 100, rHandle_);
}

void ExplanationScene::Update()
{
	//コントローラの情報とる
	DINPUT_JOYSTATE input;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	//上限を超えないように
	if (select_ == 1) {
		if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W)
			|| input.Y <= -500 || input.POV[0] == 0) {//上が押されたら
			if (!prevUpKey_) {
				select_ -= 1;
				PlaySoundMem(sHandle_, DX_PLAYTYPE_BACK);
			}
			prevUpKey_ = true;
		}
		else
			prevUpKey_ = false;
	}
	//下限を超えないように
	if (select_ == 0) {
		if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S)
			|| input.Y >= 500 || input.POV[0] == 18000) {//下が押されたら
			if (!prevDownKey_) {
				select_ += 1;
				PlaySoundMem(sHandle_, DX_PLAYTYPE_BACK);
			}
			prevDownKey_ = true;
		}
		else
			prevDownKey_ = false;
	}

	//エンターで決定!
	if (CheckHitKey(KEY_INPUT_RETURN) || ((input.Buttons[1] & 0x80) != 0)) {
		SoundFlag_ = true;
		if (!prevEnterKey_) {
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			switch (select_) {
			case 0:
				if (SoundFlag_ == true) {
					PlaySoundMem(eHandle_, DX_PLAYTYPE_BACK);
				}
				pSceneManager->ChangeScene(SCENE_ID_PLAY, true);
				break;
			case 1:
				if (SoundFlag_ == true) {
					PlaySoundMem(rHandle_, DX_PLAYTYPE_BACK);
				}
				pSceneManager->ChangeScene(SCENE_ID_TITLE,true);
				break;
			default:
				break;
			}
		}
		prevEnterKey_ = true;
	}
	else {
		prevEnterKey_ = false;
	}
}

void ExplanationScene::Draw()
{
	//画像の表示
	DrawGraph(0, 0, ePict_, TRUE);
	//選択するときの三角
	int tmp = 600 + select_ * 60;
	DrawTriangle(1000, tmp, 1000 - 30, tmp + 20, 1000 - 30, tmp - 20,
		GetColor(255, 255, 0), TRUE);
}

void ExplanationScene::Release()
{
}