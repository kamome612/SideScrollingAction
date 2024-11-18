#pragma once
#include "Engine/GameObject.h"
class Health:
	public GameObject
{
public:
	//コンストラクタ
	Health(GameObject* parent);

	//デストラクタ
	~Health();

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


private:
	int hImage_;
	float disTime_; //消えるまでの時間
	int animType_;
	int animFrame_;
	float time_;
};

