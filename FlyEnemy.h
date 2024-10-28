#pragma once
#include "Engine/GameObject.h"
#include "Player.h"
class FlyEnemy :
    public GameObject
{
public:
    //コンストラクタ
    FlyEnemy(GameObject* parent);

    //デストラクタ
    ~FlyEnemy() override;

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

    bool CollideCircle(float x, float y, float r);

    bool InRange(Player* player);

private:
    int eImage_;
    bool onGround_;//地面の上にいるか
    bool underbrock_; //ブロックの下にいるか
    float ground_;    //地面の高さ
    float time_;        //タイム
    int animType_;//状況
    int animFrame_;//コマ
    int frameCounter_;
    float moveTime_;//上下入れ替え時間を測る
    int moveDirection_;//上下入れ替え（1が上、-1が下）
    bool chasingPlayer_;//視界内に入ったか
    float chaseDelayTime_;//追尾遅延時間
    float lastPlayerX_;//Playerの居たX座標
    float lastPlayerY_;//Playerの居たY座標

    enum State {
        S_Normal = 0,
        S_Attack,
    };
    State state_;
};

