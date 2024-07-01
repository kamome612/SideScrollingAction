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

    //こっちに変えるかも
    void UpdateNormal2();
    int CharMove(float* _x, float* _y, float *_downSP,
        float _moveX, float _moveY, char*_jumpFlag,Stage _pStage);

    //攻撃時のアニメーションなどの更新
    void UpdateAttack();

    //描画
    void Draw() override;

    //位置をセット
    void SetPosition(float _x, float _y);

    //重力のセット
    void SetGravity(float _gravity);
private:
    int pImage_; //プレイヤーの画像
    //float walkSpeed_; //歩く速さ(後でnamespaceの方に入れるかも)
    float gravity_;   //重力
    float jumpSpeed_; //ジャンプの速さ
    float ground_;    //地面の高さ
    bool prevSpaceKey_; //ひとつ前のフレームでspaceキーを押したか
    bool prevAttackKey_;//ひとつ前のフレームでEキーを押したから
    bool onGround_;     //地面の上にいるか
    float time_;        //タイム
    int animType_;//状況
    int animFrame_;//コマ
    int frameCounter_;

    enum State {
        S_Normal = 0,
        S_Attack,
    };
    State state_;
};

