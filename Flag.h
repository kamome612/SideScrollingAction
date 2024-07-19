#pragma once
#include "Engine/GameObject.h"
class Flag :
    public GameObject
{
public:
    //コンストラクタ
    Flag(GameObject* parent);

    //デストラクタ
    ~Flag();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //場所をセットする
    void SetPosition(float _x, float _y);

    //当たり判定
    bool HitFlag(float _x, float _y, float _r);

private:
    int fImage_;
    int animFrame_;
    float time_;
};

