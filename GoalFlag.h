#pragma once
#include "Engine/GameObject.h"

//ゴールの判定に使う旗を管理するクラス
class GoalFlag :
    public GameObject
{
public:
    //コンストラクタ
    GoalFlag(GameObject* parent);

    //デストラクタ
    ~GoalFlag();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;
private:
    int gfImage_;
};

