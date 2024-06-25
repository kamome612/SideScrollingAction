#pragma once
#include "Engine/GameObject.h"

class Banner :public GameObject
{
public:
	Banner(GameObject* scene);
	~Banner();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	enum ViewID {
		V_Start = 0,
		V_Clear,
		V_GameOver,
		V_Nothing,
	};
	void View(ViewID id);
private:
	int bImage_;
	ViewID view_;
	//ゲームオーバー表示用
	float transparency_;//透明度
	float gameOverY;//ゲームオーバーの字のY座標
	float gameOverTime_;
};
