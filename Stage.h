#pragma once
#include "Engine/GameObject.h"

//�X�e�[�W���Ǘ�����N���X
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
    int sImage_;//�X�e�[�W���쐬����̂Ɏg���摜
    int* Map_;//�X�e�[�W�̏�������z��
    int width_;
    int height_;
};

