#pragma once
#include "Engine/GameObject.h"
class TitleScene :
    public GameObject
{
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	TitleScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

private:
	int tPict_;
	int select_;        //選択
	bool prevUpKey_;    //前フレームの上キー状態
	bool prevDownKey_;  //前フレームの下キー状態
	bool prevEnterKey_; //前フレームの決定キー状態
	int sHandle_; //選択音ハンドル
	int eHandle_; //決定音ハンドル
	int rHandle_; //戻り音ハンドル
	bool SoundFlag_;//サウンドがなっているか
};

