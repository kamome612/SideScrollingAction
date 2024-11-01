#pragma once
#include "Engine/GameObject.h"
class Enemy :
    public GameObject
{
public:
    //�R���X�g���N�^
    Enemy(GameObject* parent);

    //�f�X�g���N�^
    ~Enemy();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�ʏ펞�̓���̃A�j���[�V�����Ȃǂ̍X�V
    void UpdateNormal();

    //�U�����̃A�j���[�V�����Ȃǂ̍X�V
    void UpdateAttack();

    //�`��
    void Draw() override;

    //�ʒu���Z�b�g
    void SetPosition(float _x, float _y);

    //�d�͂̃Z�b�g
    void SetGravity(float _gravity); 
    
    bool CollideCircle(float x,float y,float r);
private:
    int eImage_;      //�v���C���[�̉摜
    float gravity_;   //�d��
    float jumpSpeed_; //�W�����v�̑���
    bool onGround_;   //�n�ʂ̏�ɂ��邩
    float time_;      //�^�C��
    int animType_;    //��
    int animFrame_;   //�R�}
   
    enum State {
        S_Normal = 0, //�ʏ펞
        S_Attack,     //�U����(���̂Ƃ��떳��)
    };
    State state_;
};

