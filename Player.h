#pragma once
#include "Engine/GameObject.h"
class Player :
    public GameObject
{
public:
    //コンストラクタ
    Player(GameObject* parent);

    //デストラクタ
    ~Player();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    void UpdateNormal();

    void UpdateWalk();

    //描画
    void Draw() override;
private:
    int pImage_;
    enum State_ {
        S_Normal = 0,
        S_Walk,
    };

    State state_;
};

