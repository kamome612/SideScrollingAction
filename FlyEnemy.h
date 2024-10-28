#pragma once
#include "Engine/GameObject.h"
#include "Player.h"
class FlyEnemy :
    public GameObject
{
public:
    //�R���X�g���N�^
    FlyEnemy(GameObject* parent);

    //�f�X�g���N�^
    ~FlyEnemy() override;

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

    bool CollideCircle(float x, float y, float r);

    bool InRange(Player* player);

private:
    int eImage_;
    bool onGround_;//�n�ʂ̏�ɂ��邩
    bool underbrock_; //�u���b�N�̉��ɂ��邩
    float ground_;    //�n�ʂ̍���
    float time_;        //�^�C��
    int animType_;//��
    int animFrame_;//�R�}
    int frameCounter_;
    float moveTime_;//�㉺����ւ����Ԃ𑪂�
    int moveDirection_;//�㉺����ւ��i1����A-1�����j
    bool chasingPlayer_;//���E���ɓ�������
    float chaseDelayTime_;//�ǔ��x������
    float lastPlayerX_;//Player�̋���X���W
    float lastPlayerY_;//Player�̋���Y���W

    enum State {
        S_Normal = 0,
        S_Attack,
    };
    State state_;
};

