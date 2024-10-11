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

    //������W�����v�̃A�j���[�V�����Ȃǂ̍X�V
    void UpdateMove();

    //�U�����̃A�j���[�V�����Ȃǂ̍X�V
    void UpdateAttack();

    //���ʎ��̃A�j���[�V�����Ȃǂ̍X�V
    void UpdateDie();

    //�`��
    void Draw() override;

    //�ʒu���Z�b�g
    void SetPosition(float _x, float _y);

    //�d�͂̃Z�b�g
    void SetGravity(float _gravity);
private:
    int pImage_; //�v���C���[�̉摜
    int lImage_; //�v���C���[�̗̑͂�\���摜
    int dImage_; //�̗͂̃_���[�W�󂯂������̉摜
    float gravity_;   //�d��
    float jumpSpeed_; //�W�����v�̑���
    float ground_;    //�n�ʂ̍���
    bool prevAttackKey_;//�ЂƂO�̃t���[����E�L�[������������
    bool onGround_;     //�n�ʂ̏�ɂ��邩
    bool hitFlag_; //���������̃t���O
    float time_;        //�^�C��
    float invTime_; //���G����
    int animType_;//��
    int animFrame_;//�R�}
    int frameCounter_;
    int pLife_;//�v���C���[�̗̑�
    bool isTypeA{ false };
    bool isTypeB{ false };
    int prevMoveKey_;//���E�ǂ����̃L�[���O�񉟂��ꂽ��

    enum State {
        S_Normal = 0,
        S_Move,
        S_Attack,
        S_Die,
    };
    State state_;
};

