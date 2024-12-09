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
	int pPict_; //ステージ選択時の画像
	enum State {
		S_Select = 0,
		S_Ready,   //始まるまで待つ(Readyなどを表示予定)
		S_Play,    //プレイ中
		S_Clear,   //ゲームクリア(ボタン押されるまで待つ予定)
		S_GameOver,//ゲームオーバー(ボタン押されるまで待つ予定)
	};

	State state_;
	float timer_;
	int mapNo_;  //ステージを指定するときに使う
	bool prevRightKey_;
	bool prevLeftKey_;
	int sHandle_; //選択音ハンドル
	int eHandle_; //決定音ハンドル
	int rHandle_; //戻り音ハンドル
	int cHandle_; //クリア音ハンドル
	int gHandle_; //ゲームオーバー音ハンドル
	int pHandle_; //ゲームプレイ音ハンドル

	//ちょっとfps確認用
	int fpsCount_;
	float fpsTimer_;

	void UpdateSelect();
	void UpdateReady();
	void UpdatePlay();
	void UpdateClear();
	void UpdateGameOver();
public:
	void StartSelect();
	void StartReady();
	void StartPlay();
	void StartClear();
	void StartGameOver();
	bool prevEnterKey_;
	int fps_;
};

