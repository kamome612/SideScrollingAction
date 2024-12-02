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

    //�e�̃����[�h
    void Reload();

    //�U���Ɉڂ邽�߂̏���
    void ReadyAttack(bool &_isType);

    //�U��
    void Attack(int _angleA,int _angleB);

    //�Z��
    void Die();

    //�~�T�C���A�C�R���̃A�j���[�V��������
    void Icon();

private:
    int pImage_; //�v���C���[�̉摜
    int lImage_; //�v���C���[�̗̑͂�\���摜
    int dImage_; //�̗͂̃_���[�W�󂯂������̉摜
    int mImage_; //�c�e���\���p�̉摜
    int bImage_; //�e�\���̃o�i�[
    int sImage_; //�V�[���h�����Ă鎞�̉摜
    int iImage_; //�~�T�C���A�C�e���̉摜
    int fImage_;      //���C�t�o�i�[
    float gravity_;   //�d��
    float jumpSpeed_; //�W�����v�̑���
    float ground_;    //�n�ʂ̍���
    bool prevAttackKey_;//�ЂƂO�̃t���[����E�L�[������������
    bool onGround_;     //�n�ʂ̏�ɂ��邩
    bool hitFlag_;      //���������̃t���O
    float time_;        //�^�C��
    float invTime_;     //���G����
    int animType_;      //��
    int animFrame_;     //�R�}
    int pLife_;         //�v���C���[�̗̑�
    bool isTypeA{ false }; //�~�T�C��(������)
    bool isTypeB{ false }; //�V(�΂ߏ�)
    int prevMoveKey_;      //���E�ǂ����̃L�[���O�񉟂��ꂽ��
    int currentNum_;       //���݂̒e��
    float reloadTime_;     //�����[�h�̎���
    bool reloading_;       //�����[�h����
    bool getShield_;       //�V�[���h�����Ă邩�ǂ���
    bool getMissileItem_;  //�~�T�C���A�C�e���������Ă邩�ǂ���
    float itemTime_;       //�~�T�C���A�C�e���̌o�ߎ���
    float iTime_;          //�~�T�C���A�C�e���p�̃A�j���[�V�����^�C��
    int mAnimFrame_;       //�~�T�C���A�C�R���p�̃t���[��
    bool canJump_;

    //�������fps�m�F�p
    int fps_;
    float fpsTimer_;

    enum State {
        S_Normal = 0, //�ʏ펞
        S_Move,       //�Ȃɂ����瓮�������鎞
        S_Attack,     //�U����
        S_Die,        //�Z�ʎ�
    };

    State state_;
};

