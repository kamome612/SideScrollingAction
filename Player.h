#pragma once
#include "Engine/GameObject.h"
class Player :
    public GameObject
{
    int pImage_;
public:
    //コンストラクタ
    Player(GameObject* parent);

    //デストラクタ
    ~Player();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;
};

