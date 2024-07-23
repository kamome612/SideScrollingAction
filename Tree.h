#pragma once
#include "Engine/GameObject.h"
#include "Camera.h"

namespace {
    const int TREE_SIZE = 256;
}
class Tree :
	public GameObject
{
public:
    //�R���X�g���N�^
    Tree(GameObject* parent) :GameObject(parent, "Tree"), tImage_(-1) { ; }

    //�f�X�g���N�^
    ~Tree() {
        if (tImage_ > 0) {
            DeleteGraph(tImage_);
        }
    }

    //������
    void Initialize() override {
        tImage_ = LoadGraph("Assets\\Stage\\tree.png");
        assert(tImage_ > 0);
    }

    //�`��
    void Draw() override {
        int x = (int)transform_.position_.x;
        int y = (int)transform_.position_.y;
        Camera* cam = GetParent()->FindGameObject<Camera>();
        if (cam != nullptr) {
            x -= cam->GetValue();
        }
        DrawRectGraph(x, y, 0, 0, TREE_SIZE, TREE_SIZE, tImage_, TRUE);
    }

    //�ꏊ���Z�b�g����
    void SetPosition(float _x, float _y) {
        transform_.position_.x = _x;
        transform_.position_.y = _y;
    }

private:
    int tImage_;
};

