#include "PlayScene.h"
#include "Player.h"
#include "Stage.h"
#include "Camera.h"
#include "Banner.h"
#include "Engine/time.h"
#include "Engine/SceneManager.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent, "PlayScene"), pPict_(-1), timer_(0.0f)
	                                , prevResetKey_(false), prevChangeKey_(false)
{
}

void PlayScene::Initialize()
{
	//‰æ‘œ‚Ìƒ[ƒh(”wŒi)
	//pPict_ = LoadGraph("Assets\\Picture\\playscene.png");//E‚Á‚Ä‚«‚½‰æ‘œ‚ªwindowƒTƒCƒY‚É‡‚Á‚Ä‚È‚©‚Á‚½(’T‚·‚±‚Æ)
	//assert(pPict_ > 0);
	//Stage* pStage = Instantiate<Stage>(this);
	//pStage->Reset();
	//state_ = S_Play;
	Instantiate<Camera>(this);
	Stage* pStage = Instantiate<Stage>(this);
	Player* pPlayer = Instantiate<Player>(this);
	pStage->StageSet();
	Instantiate<Banner>(this);
	StartReady();
}

void PlayScene::Update()
{
	switch (state_) {
	case S_Ready:UpdateReady(); break;
	case S_Play:UpdatePlay(); break;
	case S_Clear: UpdateClear(); break;
	case S_GameOver: UpdateGameOver(); break;
	}
}

void PlayScene::Draw()
{
	//DrawGraph(0, 0, pPict_, FALSE);
}

void PlayScene::Release()
{
}

void PlayScene::StartSelect()
{
	state_ = S_Select();
}

void PlayScene::UpdateSelect()
{
}

void PlayScene::StartReady()
{
	state_ = S_Ready;
	timer_ = 2.0f;//Ready‚Ì•\¦ŠÔ
	Banner* pBanner = FindGameObject<Banner>();
	pBanner->View(Banner::ViewID::V_Start);
	//Player‚Ì‰Šú‰»
	//“G‚Ì‰Šú‰»
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

}

void PlayScene::UpdateClear()
{
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
		pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
	}
}
