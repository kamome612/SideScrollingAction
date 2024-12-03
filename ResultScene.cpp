#include "ResultScene.h"
#include "Engine/SceneManager.h"
#include "PlayScene.h"

ResultScene::ResultScene(GameObject* parent)
	:GameObject(parent,"ResultScene"),rPict_(-1),isClear_(false),select_(0)
	                                 ,prevUpKey_(false),prevDownKey_(false)
	                                 ,SoundFlag_(false)
{
	SceneManager* scenemanager = (SceneManager*)FindObject("SceneManager");
	isClear_ = scenemanager->clearFlag_;
}

void ResultScene::Initialize()
{
	if (isClear_) {
		rPict_ = LoadGraph("Assets/Picture/clear.jpg");
	}
	else {
		rPict_ = LoadGraph("Assets/Picture/gameover.jpg");
	}
	assert(rPict_ > 0);
	sHandle_ = LoadSoundMem("Assets/Sound/SelectSound.mp3");
	eHandle_ = LoadSoundMem("Assets/Sound/EnterSound.mp3");
	rHandle_ = LoadSoundMem("Assets/Sound/ReturnSound.mp3");
	ChangeVolumeSoundMem(255 * 70 / 100, sHandle_);
	ChangeVolumeSoundMem(255 * 70 / 100, eHandle_);
	ChangeVolumeSoundMem(255 * 70 / 100, rHandle_);
}

void ResultScene::Update()
{
	//�R���g���[���̏��Ƃ�
	DINPUT_JOYSTATE input;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	//����𒴂��Ȃ��悤��
	if (select_ > 0) {
		if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W)
			|| input.Y <= -500 || input.POV[0] == 0) {//�オ�����ꂽ��
			if (!prevUpKey_) {
				select_ -= 1;
				PlaySoundMem(sHandle_, DX_PLAYTYPE_BACK);
			}
			prevUpKey_ = true;
		}
		else
			prevUpKey_ = false;
	}
	//�����𒴂��Ȃ��悤��
	if (select_ < 2) {
		if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S)
			|| input.Y >= 500 || input.POV[0] == 18000) {//���������ꂽ��
			if (!prevDownKey_) {
				select_ += 1;
				PlaySoundMem(sHandle_, DX_PLAYTYPE_BACK);
			}
			prevDownKey_ = true;
		}
		else
			prevDownKey_ = false;
	}

	//�G���^�[�Ō���!
	if (CheckHitKey(KEY_INPUT_RETURN) || (input.Buttons[1] & 0x80) != 0) {
		SoundFlag_ = true;
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		switch (select_) {
		case 0:
			if (SoundFlag_ == true) {
				PlaySoundMem(eHandle_, DX_PLAYTYPE_BACK);
			}
			pSceneManager->ChangeScene(SCENE_ID_PLAY,true);
			break;
		case 1:
			if (SoundFlag_ == true) {
				PlaySoundMem(rHandle_, DX_PLAYTYPE_BACK);
			}
			pSceneManager->ChangeScene(SCENE_ID_TITLE,true);
			break;
		case 2:
			DxLib_End();
		}
	}
}

void ResultScene::Draw()
{
	DrawGraph(0, 0, rPict_, TRUE);
	int tmp = 335 + select_ * 97;
	int len = 400;
	//�I������Ƃ��̎O�p�`��
	DrawTriangle(len, tmp,len - 30, tmp + 20, len - 30, tmp - 20,
		GetColor(255, 255, 0), TRUE);
}

void ResultScene::Release()
{
	if (rPict_ > 0) {
		DeleteGraph(rPict_);
		rPict_ = -1;
	}
}
