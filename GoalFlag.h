#pragma once
#include "Engine/GameObject.h"

//�S�[���̔���Ɏg�������Ǘ�����N���X
class GoalFlag :
    public GameObject
{
public:
    //�R���X�g���N�^
    GoalFlag(GameObject* parent);

    //�f�X�g���N�^
    ~GoalFlag();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;
private:
    int gfImage_;
};

