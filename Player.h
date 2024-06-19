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
    int pImage_;
    float walkSpeed_;
    float gravity_;
    float jumpSpeed_;
    float ground_;
    bool prevSpaceKey_;
    bool onGround_;
    float time_ = 0.0;
    int animType_;//��
    int animFrame_;//�R�}
    int frameCounter_;

    enum State {
        S_Normal = 0,
        //S_Walk,
    };
    State state_;
};

