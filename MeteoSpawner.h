#pragma once
#include "Engine/GameObject.h"
class MeteoSpawner :
    public GameObject
{
public:
	//コンストラクタ
	MeteoSpawner(GameObject* parent);

	MeteoSpawner(GameObject* parent, int _type);

	//デストラクタ
	~MeteoSpawner();

	//更新
	void Update() override;

	//描画(使うかは微妙)
	void Draw() override;

	//位置のセッター
	void SetPosition(float _x, float _y);

	//タイプ指定
	void SetType(int _type) { type_ = _type; }
private:
	float timer_;
	float interval = 5.0f;
	int type_;
};

