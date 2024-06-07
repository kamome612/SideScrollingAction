#pragma once
#include "Engine/GameObject.h"
class PlayScene :
    public GameObject
{
	//背景(ステージのほうで表示することにするかも)
	int pPict_;
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
};

