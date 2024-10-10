#pragma once
#include "Engine/GameObject.h"
class MeteoSpawner :
    public GameObject
{
public:
	//コンストラクタ
	MeteoSpawner(GameObject* parent);

	//デストラクタ
	~MeteoSpawner();

	//更新
	void Update() override;

	//描画(使うかは微妙)
	void Draw() override;

	//位置のセッター
	void SetPosition(float _x, float _y);
private:
	float timer_;
	float interval = 5.0f;
};

