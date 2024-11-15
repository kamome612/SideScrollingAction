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

	//隕石の落ちるタイプをセットする(画像もここでセットするか)
	void SetMoveType(int _type);

	bool isDead_;     //〇んでるか
private:
	int mImage_;
	float gravity_;   //重力
	float moveSpeed_; //動く速さ
	int moveType_;    //向きの種類
};

