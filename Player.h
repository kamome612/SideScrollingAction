#pragma once
#include "Engine/GameObject.h"
class Player :
    public GameObject
{
public:
    //コンストラクタ
    Player(GameObject* parent);
    Player(GameObject* parent, float _gravity);

    //デストラクタ
    ~Player();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //通常時の動作のアニメーションなどの更新
    void UpdateNormal();

    //描画
    void Draw() override;

    //位置をセット
    void SetPosition(int _x, int _y);
private:
    int pImage_;
    float walkSpeed_;
    float gravity_;
    float jumpSpeed_;
    bool prevSpaceKey_;
    bool onGround_;
    enum State {
        S_Normal = 0,
        //S_Walk,
    };

    State state_;
};

