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

    //歩くやジャンプのアニメーションなどの更新
    void UpdateMove();

    //攻撃時のアニメーションなどの更新
    void UpdateAttack();

    //死ぬ時のアニメーションなどの更新
    void UpdateDie();

    //描画
    void Draw() override;

    //位置をセット
    void SetPosition(float _x, float _y);

    //重力のセット
    void SetGravity(float _gravity);
private:
    int pImage_; //プレイヤーの画像
    float gravity_;   //重力
    float jumpSpeed_; //ジャンプの速さ
    float ground_;    //地面の高さ
    bool prevAttackKey_;//ひとつ前のフレームでEキーを押したから
    bool onGround_;     //地面の上にいるか
    float time_;        //タイム
    int animType_;//状況
    int animFrame_;//コマ
    int frameCounter_;

    enum State {
        S_Normal = 0,
        S_Move,
        S_Attack,
        S_Die,
    };
    State state_;
};

