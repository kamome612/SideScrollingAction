#pragma once
#include "Engine/GameObject.h"
class Player :
    public GameObject
{
public:
    //�R���X�g���N�^
    Player(GameObject* parent);
    Player(GameObject* parent, float _gravity);

    //�f�X�g���N�^
    ~Player();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    void UpdateNormal();

    void UpdateWalk();

    //�`��
    void Draw() override;
private:
    int pImage_;
    float walkSpeed_;
    float gravity_;
    float jumpSpeed_;
    bool prevSpaceKey_;
    bool onGround_;
    enum State {
        S_Normal = 0,
        //S_Walk,
    };

    State state_;
};

