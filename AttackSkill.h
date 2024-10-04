#pragma once
#include "Engine/GameObject.h"
class AttackSkill :
    public GameObject
{
public:
    //コンストラクタ
    AttackSkill(GameObject* parent);

    //デストラクタ
    ~AttackSkill();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //位置をセット
    void SetPosition(float _x, float _y);
private:
    int aImage_;
    float BulletTime_;
};

