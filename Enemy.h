#pragma once
#include "Engine/GameObject.h"
class Enemy :
    public GameObject
{
public:
    //コンストラクタ
    Enemy(GameObject* parent);

    //デストラクタ
    ~Enemy();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //通常時の動作のアニメーションなどの更新
    void UpdateNormal();

    //攻撃時のアニメーションなどの更新
    void UpdateAttack();

    //描画
    void Draw() override;

    //位置をセット
    void SetPosition(float _x, float _y);

    //重力のセット
    void SetGravity(float _gravity); 
    
    bool CollideCircle(float x,float y,float r);
private:
    int eImage_;      //プレイヤーの画像
    float gravity_;   //重力
    float jumpSpeed_; //ジャンプの速さ
    bool onGround_;   //地面の上にいるか
    float time_;      //タイム
    int animType_;    //状況
    int animFrame_;   //コマ
   
    enum State {
        S_Normal = 0, //通常時
        S_Attack,     //攻撃時(今のところ無し)
    };
    State state_;
};

