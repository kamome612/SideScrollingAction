#include "PlayScene.h"
#include "Player.h"
#include "Stage.h"
#include "Camera.h"
#include "Banner.h"
#include "Engine/time.h"
#include "Engine/SceneManager.h"
#include "ResultScene.h"
#include <random>

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent, "PlayScene"), pPict_(-1), timer_(0.0f),state_(S_Select),fpsTimer_(0.0f)
	                                ,mapNo_(-1),prevRightKey_(false),prevLeftKey_(false)
{
	SceneManager* scenemanager = (SceneManager*)FindObject("SceneManager");
	prevEnterKey_ = scenemanager->keyFlag_;
	fps_ = scenemanager->fps_;
	fpsCount_ = fps_;
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
	sHandle_ = LoadSoundMem("Assets/Sound/SelectSound.mp3");
	eHandle_ = LoadSoundMem("Assets/Sound/EnterSound.mp3");
	cHandle_ = LoadSoundMem("Assets/Sound/ClearSound2.mp3");
	gHandle_ = LoadSoundMem("Assets/Sound/GameOverSound.mp3");
	pHandle_ = LoadSoundMem("Assets/Sound/PlayScene.mp3");
	cgHandle_ = LoadSoundMem("Assets/Sound/ooatari2.mp3");
	ChangeVolumeSoundMem(255 * 70 / 100, sHandle_);
	ChangeVolumeSoundMem(255 * 70 / 100, eHandle_);
	ChangeVolumeSoundMem(255 * 30 / 100, cHandle_);
	ChangeVolumeSoundMem(255 * 80 / 100, gHandle_);
	ChangeVolumeSoundMem(255 * 75 / 100, pHandle_);
	ChangeVolumeSoundMem(255 * 75 / 100, cgHandle_);
	//rHandle_ = LoadSoundMem("Assets/Sound/ReturnSound.mp3");
}

void PlayScene::Update()
{
	SceneManager* scenemanager = (SceneManager*)FindObject("SceneManager");
	//fps確認用
	if (fpsTimer_ >= 1.0f) {
		fpsTimer_ = 0.0f;
		fps_ = fpsCount_;
		scenemanager->fps_ = fps_;
		fpsCount_ = 0;
	}
	fpsTimer_ += Time::DeltaTime();
	fpsCount_++;

	srand(time(NULL));
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
		int tmp = (mapNo_ - 1) * 332;
		int len = 330;
		DrawTriangle(len + tmp, 540, len + tmp + 23, 570, len + tmp - 23, 570,
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
	//コントローラの情報とる
	DINPUT_JOYSTATE input;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	//三角とステージの番号を変更するため
	if (mapNo_ < 3) {//右の上限を超えないように
		if (CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_D)
			|| input.X >= 500 || input.POV[0] == 9000) {//右を押したら
			if (!prevRightKey_) {
				mapNo_ += 1;
				PlaySoundMem(sHandle_, DX_PLAYTYPE_BACK);//選択音を流す
			}
			prevRightKey_ = true;
		}
		else
			prevRightKey_ = false;
	}

	if (mapNo_ > 1) {//左の上限を超えないように
		if (CheckHitKey(KEY_INPUT_LEFT) || CheckHitKey(KEY_INPUT_A)
			|| input.X <= -500 || input.POV[0] == 27000) {//左を押したら
			if (!prevLeftKey_) {
				mapNo_ -= 1;
				PlaySoundMem(sHandle_, DX_PLAYTYPE_BACK);//選択音を流す
			}
			prevLeftKey_ = true;
		}
		else
			prevLeftKey_ = false;
	}

	//エンターでステージを決定
	if (CheckHitKey(KEY_INPUT_RETURN) || (input.Buttons[1] & 0x80) != 0) {
		if (!prevEnterKey_) {
			PlaySoundMem(eHandle_, DX_PLAYTYPE_BACK); //決定音を流す
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
	PlaySoundMem(pHandle_, DX_PLAYTYPE_LOOP);//プレイ中BGMを流す
	state_ = S_Play;
	Banner* pBanner = FindGameObject<Banner>();
	//Banner* pBanner = Instantiate<Banner>(this);
	pBanner->View(Banner::ViewID::V_Nothing);
}

void PlayScene::UpdatePlay()
{
	Stage* pStage = FindGameObject<Stage>();
}

void PlayScene::StartClear()
{
	StopSoundMem(pHandle_);//プレイ中BGMを止める
	state_ = S_Clear;
	Banner* pBanner = FindGameObject<Banner>();
	pBanner->View(Banner::ViewID::V_Clear);
	PlaySoundMem(cHandle_, DX_PLAYTYPE_BACK);//クリア音を流す
	timer_ = 2.0f;
}

void PlayScene::UpdateClear()
{
	timer_ -= Time::DeltaTime();
	if (timer_ <= 0.0f) {
		timer_ = 0.0f;
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_RESULT, true);
		PlaySoundMem(cgHandle_, DX_PLAYTYPE_BACK);//クリアシーン音を流す
	}
	/*if (CheckHitKey(KEY_INPUT_R)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_RESULT,true);
	}*/
}

void PlayScene::StartGameOver()
{
	StopSoundMem(pHandle_);//プレイ中BGMを止める
	state_ = S_GameOver;
	Banner* pBanner = FindGameObject<Banner>();
	pBanner->View(Banner::ViewID::V_GameOver);
	timer_ = 2.0f;

}

void PlayScene::UpdateGameOver()
{
	timer_ -= Time::DeltaTime();
	if (timer_ <= 0.0f) {
		timer_ = 0.0f;
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_RESULT, false);
		PlaySoundMem(gHandle_, DX_PLAYTYPE_BACK);//ゲームオーバ音を流す
	}
}
