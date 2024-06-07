#pragma once
#include "Engine/GameObject.h"
class Player :
    public GameObject
{
public:
    //�R���X�g���N�^
    Player(GameObject* parent);

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
    enum State_ {
        S_Normal = 0,
        S_Walk,
    };

    State state_;
};

