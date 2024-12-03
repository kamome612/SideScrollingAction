#pragma once
#include "Engine/GameObject.h"
class AttackSkill :
    public GameObject
{
public:
    //�R���X�g���N�^
    AttackSkill(GameObject* parent);

    //�f�X�g���N�^
    ~AttackSkill();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�ʒu���Z�b�g
    void SetPosition(float _x, float _y);

    //�p�x�Z�b�g
    void SetAngle(float _angle);
private:
    int aImage_;
    float BulletTime_;
    float angle_;
    int Sound_;
};

