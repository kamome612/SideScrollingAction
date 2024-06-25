#pragma once
#include "Engine/GameObject.h"
class PlayScene :
    public GameObject
{
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	PlayScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	bool canMove() { return state_ == S_Play; }

private:
	//背景(ステージのほうで表示することにするかも)
	int pPict_;
	enum State {
		S_Select = 0,
		S_Ready,   //始まるまで待つ(Readyなどを表示予定)
		S_Play,    //プレイ中
		S_Clear,   //ゲームクリア(ボタン押されるまで待つ予定)
		S_GameOver,//ゲームオーバー(ボタン押されるまで待つ予定)
	};

	State state_;
	float timer_;

	void UpdateSelect();
	void StartReady();
	void UpdateReady();
	void StartPlay();
	void UpdatePlay();
	void UpdateClear();
	void UpdateGameOver();
public:
	void StartSelect();
	void StartGameOver();
};

