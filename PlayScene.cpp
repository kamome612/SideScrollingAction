#include "PlayScene.h"
#include "Player.h"
#include "Stage.h"
#include "Camera.h"
#include "Banner.h"
#include "Engine/time.h"
#include "Engine/SceneManager.h"
#include "ResultScene.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent, "PlayScene"), pPict_(-1), timer_(0.0f),state_(S_Select)
	                                , prevResetKey_(false), prevChangeKey_(false)
{
}

void PlayScene::Initialize()
{
	////画像のロード
	//pPict_ = LoadGraph("Assets\\Image\\");
	//assert(pPict_ > 0);

	StartSelect();
	Instantiate<Camera>(this);
	Stage* pStage = Instantiate<Stage>(this);
	//Player* pPlayer = Instantiate<Player>(this);
	pStage->StageSet();
	//Instantiate<Banner>(this);
}

void PlayScene::Update()
{
	switch (state_) {
	case S_Select:UpdateSelect(); break;
	case S_Ready:UpdateReady(); break;
	case S_Play:UpdatePlay(); break;
	case S_Clear: UpdateClear(); break;
	case S_GameOver: UpdateGameOver(); break;
	}
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}

void PlayScene::StartSelect()
{
	state_ = S_Select;
}

void PlayScene::UpdateSelect()
{

	if (CheckHitKey(KEY_INPUT_RETURN)) {
		StartReady();
	}

	//ここで画像を用意してその上にステージ三つぐらいを回す(予定)
	//DrawGraph(0, 0, pPict_, TRUE);
}

void PlayScene::StartReady()
{
	state_ = S_Ready;
	timer_ = 2.0f;//Readyの表示時間
	//Banner* pBanner = FindGameObject<Banner>();
	Banner* pBanner = Instantiate<Banner>(this);
	pBanner->View(Banner::ViewID::V_Start);
}

void PlayScene::UpdateReady()
{
	timer_ -= Time::DeltaTime();
	if (timer_ <= 0.0f) {
		timer_ = 0.0f;
		StartPlay();
	}
}

void PlayScene::StartPlay()
{
	state_ = S_Play;
	Banner* pBanner = FindGameObject<Banner>();
	//Banner* pBanner = Instantiate<Banner>(this);
	pBanner->View(Banner::ViewID::V_Nothing);
}

void PlayScene::UpdatePlay()
{
	Stage* pStage = FindGameObject<Stage>();
	if (CheckHitKey(KEY_INPUT_R)) {
		if (!prevResetKey_) {
			pStage->StageSet();
			StartReady();
		}
		prevResetKey_ = true;
	}
	else
		prevResetKey_ = false;

	if (CheckHitKey(KEY_INPUT_C)) {
		if (!prevChangeKey_) {
			pStage->ChangeStage();
			pStage->StageSet();
			StartReady();
		}
		prevChangeKey_ = true;
	}
	else
		prevChangeKey_ = false;

	if (pStage->GetStageLife() <= 0) {
		StartGameOver();
	}
}

void PlayScene::StartClear()
{
	state_ = S_Clear;
	Banner* pBanner = FindGameObject<Banner>();
	pBanner->View(Banner::ViewID::V_Clear);
}

void PlayScene::UpdateClear()
{
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_RESULT,true);
	}
}

void PlayScene::StartGameOver()
{
	state_ = S_GameOver;
	Banner* pBanner = FindGameObject<Banner>();
	pBanner->View(Banner::ViewID::V_GameOver);
}

void PlayScene::UpdateGameOver()
{
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_RESULT);
	}
}
