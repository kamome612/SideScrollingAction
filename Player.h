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

    //弾のリロード
    void Reload();

    //攻撃に移るための準備
    void ReadyAttack(bool &_isType);

    //攻撃
    void Attack(int _angleA,int _angleB);

    //〇ぬ
    void Die();

    //ミサイルアイコンのアニメーション処理
    void Icon();

private:
    int pImage_; //プレイヤーの画像
    int lImage_; //プレイヤーの体力を表す画像
    int dImage_; //体力のダメージ受けた部分の画像
    int mImage_; //残弾数表示用の画像
    int bImage_; //弾表示のバナー
    int sImage_; //シールド持ってる時の画像
    int iImage_; //ミサイルアイテムの画像
    int fImage_;      //ライフバナー
    float gravity_;   //重力
    float jumpSpeed_; //ジャンプの速さ
    float ground_;    //地面の高さ
    bool prevAttackKey_;//ひとつ前のフレームでEキーを押したから
    bool onGround_;     //地面の上にいるか
    bool hitFlag_;      //当った時のフラグ
    float time_;        //タイム
    float invTime_;     //無敵時間
    int animType_;      //状況
    int animFrame_;     //コマ
    int pLife_;         //プレイヤーの体力
    bool isTypeA{ false }; //ミサイル(横向き)
    bool isTypeB{ false }; //〃(斜め上)
    int prevMoveKey_;      //左右どっちのキーが前回押されたか
    int currentNum_;       //現在の弾数
    float reloadTime_;     //リロードの時間
    bool reloading_;       //リロード中か
    bool getShield_;       //シールド持ってるかどうか
    bool getMissileItem_;  //ミサイルアイテムを持ってるかどうか
    float itemTime_;       //ミサイルアイテムの経過時間
    float iTime_;          //ミサイルアイテム用のアニメーションタイム
    int mAnimFrame_;       //ミサイルアイコン用のフレーム
    bool canJump_;

    //ちょっとfps確認用
    int fps_;
    float fpsTimer_;

    enum State {
        S_Normal = 0, //通常時
        S_Move,       //なにかしら動きがある時
        S_Attack,     //攻撃時
        S_Die,        //〇ぬ時
    };

    State state_;
};

