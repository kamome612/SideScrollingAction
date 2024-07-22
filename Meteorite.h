#pragma once
#include "Engine/GameObject.h"
class Meteorite :
    public GameObject
{
public:
	//コンストラクタ
	Meteorite(GameObject* parent);

	//デストラクタ
	~Meteorite();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw()override;

	//位置のセッター
	void SetPosition(float _x, float _y);

	//円の当たり判定をする
	bool CollideCircle(float x, float y, float r);

	//重力をセット
	void SetGravity(float _g) { gravity_ = _g; }
private:
	int mImage_;
	float gravity_;
	float moveSpeed_;
};

