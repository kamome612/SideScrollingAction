#pragma once
#include "Engine/GameObject.h"
class Flag :
    public GameObject
{
public:
    //�R���X�g���N�^
    Flag(GameObject* parent);

    //�f�X�g���N�^
    ~Flag();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�ꏊ���Z�b�g����
    void SetPosition(float _x, float _y);

    //�����蔻��
    bool HitFlag(float _x, float _y, float _r);

private:
    int fImage_;
    int animFrame_;
    float time_;
};

