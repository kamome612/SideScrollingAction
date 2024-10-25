#pragma once
#include "Engine/GameObject.h"
#include "Camera.h"

namespace {
    const int ROCKET_SIZE = 320;
}

class Rocket :
    public GameObject
{
public:
    //�R���X�g���N�^
    Rocket(GameObject* parent) :GameObject(parent, "Rocket"), rImage_(-1) { ; }

    //�f�X�g���N�^
    ~Rocket() {
        if (rImage_ > 0) {
            DeleteGraph(rImage_);
        }
    }

    //������
    void Initialize() override {
        rImage_ = LoadGraph("Assets\\Stage\\rocket.png");
        assert(rImage_ > 0);
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
        DrawRectGraph(x, y, 0, 0, ROCKET_SIZE, ROCKET_SIZE, rImage_, TRUE);
    }

    //�ꏊ���Z�b�g����
    void SetPosition(float _x, float _y) {
        transform_.position_.x = _x;
        transform_.position_.y = _y;
    }

private:
    int rImage_;
};
