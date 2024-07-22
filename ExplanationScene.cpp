#include "ExplanationScene.h"
#include "Engine/SceneManager.h"

ExplanationScene::ExplanationScene(GameObject* parent)
	:GameObject(parent, "ExplanationScene"),select_(0),ePict_(-1), prevUpKey_(false), prevDownKey_(false)
{
}

void ExplanationScene::Initialize()
{
	ePict_ = LoadGraph("Assets/Picture/explanation.jpg");
	assert(ePict_ > 0);
	SceneManager* scenemanager = (SceneManager*)FindObject("SceneManager");
	prevEnterKey_ = scenemanager->keyFlag_;
}

void ExplanationScene::Update()
{
	//����𒴂��Ȃ��悤��
	if (select_ == 1) {
		if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W)) {//�オ�����ꂽ��
			if (!prevUpKey_) {
				select_ -= 1;
			}
			prevUpKey_ = true;
		}
		else
			prevUpKey_ = false;
	}
	//�����𒴂��Ȃ��悤��
	if (select_ == 0) {
		if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S)) {//���������ꂽ��
			if (!prevDownKey_) {
				select_ += 1;
			}
			prevDownKey_ = true;
		}
		else
			prevDownKey_ = false;
	}

	//�G���^�[�Ō���!
	if (CheckHitKey(KEY_INPUT_RETURN)) {
		if (!prevEnterKey_) {
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			switch (select_) {
			case 0:
				pSceneManager->ChangeScene(SCENE_ID_PLAY, true);
				break;
			case 1:
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
	//�摜�̕\��
	DrawGraph(0, 0, ePict_, TRUE);
	//�I������Ƃ��̎O�p
	int tmp = 600 + select_ * 60;
	DrawTriangle(1000, tmp, 1000 - 30, tmp + 20, 1000 - 30, tmp - 20,
		GetColor(255, 255, 0), TRUE);
}

void ExplanationScene::Release()
{
}
