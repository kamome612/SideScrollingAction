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

    //�ʏ펞�̓���̃A�j���[�V�����Ȃǂ̍X�V
    void UpdateNormal();

    //�`��
    void Draw() override;

    //�ʒu���Z�b�g
    void SetPosition(float _x, float _y);

    //�d�͂̃Z�b�g
    void SetGravity(float _gravity);
private:
    int pImage_; //�v���C���[�̉摜
    float walkSpeed_; //��������(���namespace�̕��ɓ���邩��)
    float gravity_;   //�d��
    float jumpSpeed_; //�W�����v�̑���
    float ground_;    //�n�ʂ̍���
    bool prevSpaceKey_; //�ЂƂO�̃t���[����spaceKey����������
    bool onGround_;     //�n�ʂ̏�ɂ��邩
    float time_;        //�^�C��
    int animType_;//��
    int animFrame_;//�R�}
    int frameCounter_;

    enum State {
        S_Normal = 0,a
    };
    State state_;
};

