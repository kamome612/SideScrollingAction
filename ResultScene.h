#pragma once
#include "Engine/GameObject.h"

//クリアやゲームオーバなどのリザルトシーンを管理するクラス
class ResultScene :
	public GameObject
{
public:
	//コンストラクタ
	// 引数:parent 親オブジェクト(SceneManager)
	ResultScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//クリアの時にisClear_をtrueにする
	void ChangeResult() { isClear_ = true; }

private:
	bool isClear_;
	int rPict_;
};

