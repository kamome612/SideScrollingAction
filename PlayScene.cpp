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
	                                ,mapNo_(-1)
{
	SceneManager* scenemanager = (SceneManager*)FindObject("SceneManager");
	prevEnterKey_ = scenemanager->keyFlag_;
}

void PlayScene::Initialize()
{
	////画像のロード
	//pPict_ = LoadGraph("Assets\\Image\\");
	//assert(pPict_ > 0);

	StartSelect();
	Instantiate<Camera>(this);
	pPict_ = LoadGraph("Assets\\Picture\\stageselect.jpg");
	assert(pPict_ > 0);
	//Stage* pStage = Instantiate<Stage>(this);
	//Player* pPlayer = Instantiate<Player>(this);
	//pStage->StageSet();
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
	if (state_ == S_Select) {
		//ステージ選択の画像表示(画像はまだ仮)
		DrawGraph(0, 0, pPict_, FALSE);
		//選ぶ時の三角形の表示
		int tmp = (mapNo_ - 1) * 440;
		DrawTriangle(200 + tmp, 540, 200 + tmp + 20, 570, 200 + tmp - 20, 570,
			GetColor(255, 255, 0), TRUE);
	}
}

void PlayScene::Release()
{
}

void PlayScene::StartSelect()
{
	state_ = S_Select;
	mapNo_ = 1;
}

void PlayScene::UpdateSelect()
{
	//三角とステージの番号を変更するため
	if (mapNo_ < 3) {//右の上限を超えないように
		if (CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_D)) {//右を押したら
			if (!prevRightKey_) {
				mapNo_ += 1;
			}
			prevRightKey_ = true;
		}
		else
			prevRightKey_ = false;
	}

	if (mapNo_ > 1) {//左の上限を超えないように
		if (CheckHitKey(KEY_INPUT_LEFT) || CheckHitKey(KEY_INPUT_A)) {//左を押したら
			if (!prevLeftKey_) {
				mapNo_ -= 1;
			}
			prevLeftKey_ = true;
		}
		else
			prevLeftKey_ = false;

	}

	//エンターでステージを決定
	if (CheckHitKey(KEY_INPUT_RETURN)) {
		if (!prevEnterKey_) {
			StartReady();
			prevEnterKey_ = true;
		}
	}
	else {
		prevEnterKey_ = false;
	}
}

void PlayScene::StartReady()
{
	state_ = S_Ready;
	timer_ = 2.0f;//Readyの表示時間
	//バナーがあったら消しとく
	if (Banner* pBanner = GetParent()->FindGameObject<Banner>()) {
		pBanner->KillMe();
	}
	Stage* pStage = Instantiate<Stage>(this);
	pStage->ChangeStage(mapNo_);
	pStage->StageSet();
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
	if (pStage->GetStageLife() <= 0) {
		StartGameOver();
	}
}

void PlayScene::StartClear()
{
	state_ = S_Clear;
	Banner* pBanner = FindGameObject<Banner>();
	pBanner->View(Banner::ViewID::V_Clear);
	timer_ = 3.0f;
}

void PlayScene::UpdateClear()
{
	timer_ -= Time::DeltaTime();
	if (timer_ <= 0.0f) {
		timer_ = 0.0f;
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_RESULT, true);
	}
	/*if (CheckHitKey(KEY_INPUT_R)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_RESULT,true);
	}*/
}

void PlayScene::StartGameOver()
{
	state_ = S_GameOver;
	Banner* pBanner = FindGameObject<Banner>();
	pBanner->View(Banner::ViewID::V_GameOver);
	timer_ = 3.0f;

}

void PlayScene::UpdateGameOver()
{
	timer_ -= Time::DeltaTime();
	if (timer_ <= 0.0f) {
		timer_ = 0.0f;
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_RESULT, false);
	}
}
