#pragma once
#include "Engine/GameObject.h"
#include "Camera.h"

namespace {
	const int CLOUD_SIZE = 203;
}

class Cloud :
	public GameObject
{
public:
    //�R���X�g���N�^
    Cloud(GameObject* parent) :GameObject(parent, "Cloud"), cImage_(-1) { ; }

    //�f�X�g���N�^
    ~Cloud() {
        if (cImage_ > 0) {
            DeleteGraph(cImage_);
        }
    }

    //������
    void Initialize() override {
        cImage_ = LoadGraph("Assets\\Stage\\cloud.png");
        assert(cImage_ > 0);
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
        DrawRectGraph(x, y, 0, 0, CLOUD_SIZE, CLOUD_SIZE, cImage_, TRUE);
    }

    //�ꏊ���Z�b�g����
    void SetPosition(float _x, float _y) {
        transform_.position_.x = _x;
        transform_.position_.y = _y;
    }

private:
    int cImage_;
};