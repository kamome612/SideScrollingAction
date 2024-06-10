#pragma once
#include "Engine/GameObject.h"

//ステージを管理するクラス
class Stage :
    public GameObject
{
public:
    Stage(GameObject* parent);
    Stage(GameObject* parent, int _number);
    ~Stage();
    void Reset();
    void Update() override;
    void Draw() override;
private:
    int sImage_;//ステージを作成するのに使う画像
    int* Map_;//ステージの情報を入れる配列
    int width_;
    int height_;
};

