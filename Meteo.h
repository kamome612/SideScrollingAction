#pragma once
#include "Engine/GameObject.h"
#include "Camera.h"

namespace {
    const int METEO_SIZE = 64;
}

class Meteo :
    public GameObject
{
public:
    //�R���X�g���N�^
    Meteo(GameObject* parent) :GameObject(parent, "Meteo"), mImage_(-1) { ; }

    //�f�X�g���N�^
    ~Meteo() {
        if (mImage_ > 0) {
            DeleteGraph(mImage_);
        }
    }

    //������
    void Initialize() override {
        mImage_ = LoadGraph("Assets\\Stage\\meteo.png");
        assert(mImage_ > 0);
    }

    //�`��
    void Draw() override {
        int x = (int)transform_.position_.x;
        int y = (int)transform_.position_.y;
        Camera* cam = GetParent()->FindGameObject<Camera>();
        if (cam != nullptr) {
            x -= cam->GetValueX();
            y -= cam->GetValueY();
        }
        DrawRectGraph(x, y, 0, 0, METEO_SIZE, METEO_SIZE, mImage_, TRUE);
    }

    //�ꏊ���Z�b�g����
    void SetPosition(float _x, float _y) {
        transform_.position_.x = _x;
        transform_.position_.y = _y;
    }

private:
    int mImage_;

};