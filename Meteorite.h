#pragma once
#include "Engine/GameObject.h"
class Meteorite :
    public GameObject
{
private:
	int mImage_;
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

};

