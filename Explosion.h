#pragma once
#include "Engine/GameObject.h"
class Explosion :
    public GameObject
{
public:
	//コンストラクタ
	Explosion(GameObject* parent);

	//デストラクタ
	~Explosion();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw()override;
	
	//位置をセット
	void SetPosition(float _x, float _y);

private:
	int eImage_;
	int animFrame_;
	int animColumn_;
	int animLine_;
	float exTime_;
};

