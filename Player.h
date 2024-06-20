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

    //通常時の動作のアニメーションなどの更新
    void UpdateNormal();

    //描画
    void Draw() override;

    //位置をセット
    void SetPosition(float _x, float _y);

    //重力のセット
    void SetGravity(float _gravity);
private:
    int pImage_; //プレイヤーの画像
    float walkSpeed_; //歩く速さ(後でnamespaceの方に入れるかも)
    float gravity_;   //重力
    float jumpSpeed_; //ジャンプの速さ
    float ground_;    //地面の高さ
    bool prevSpaceKey_; //ひとつ前のフレームでspaceKeyを押したか
    bool onGround_;     //地面の上にいるか
    float time_;        //タイム
    int animType_;//状況
    int animFrame_;//コマ
    int frameCounter_; 

    enum State {
        S_Normal = 0,
        //S_Walk,
    };
    State state_;
};

