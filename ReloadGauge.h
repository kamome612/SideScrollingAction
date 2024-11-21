#pragma once
#include "Engine/GameObject.h"
class ReloadGauge
	:public GameObject
{
public:
	//コンストラクタ
	ReloadGauge(GameObject* parent);

	//デストラクタ
	~ReloadGauge();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw()override;

	//位置のセッター
	void SetPosition(float _x, float _y);

	void SetProgress(float progress);
private:
	int hImage_;
	int hImageF_;//フレーム

	float progress_;//状況
	float maxW;//ゲージの横幅

	

};



