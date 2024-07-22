#pragma once
#include "Engine/GameObject.h"
class ExplanationScene :
    public GameObject
{
public:
	//コンストラクタ
	ExplanationScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
private:
	int ePict_;
	int select_;
	bool prevUpKey_;
	bool prevDownKey_;
	bool prevEnterKey_;
};

