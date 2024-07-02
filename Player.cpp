#include "Player.h"
#include "Engine/time.h"
#include "Camera.h"
#include "Stage.h"
#include "Meteorite.h"
#include "PlayScene.h"
#include "Explosion.h"
#include "AttackSkill.h"

namespace {
	const float CHIP_SIZE = 64.0f;//�L�����̉摜�T�C�Y
	const XMFLOAT3 INIT_POS = { 30,575,0 };//�ŏ��̈ʒu
	const float JUMP_HEIGHT = 64.0f * 3.0f;//�W�����v�̍���
	const float INIT_GRAVITY = 9.8/ 90.0f;
	const float MAX_POS = 400;
	const int SPEED = 150;
	//�d�̓���:��...1.62,�ΐ�...3.71,���z274

}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),pImage_(-1),gravity_(INIT_GRAVITY),
	                             jumpSpeed_(0.0f), onGround_(true),
	                             time_(0.0f),animType_(0),animFrame_(0),
	                             prevAttackKey_(false)
{
	//�����ʒu�̒���
	transform_.position_ = INIT_POS;
	state_ = S_Normal;
}

Player::~Player()
{
	if (pImage_ > 0) {
		DeleteGraph(pImage_);
		pImage_ = -1;
	}
}

void Player::Initialize()
{
	//�Ƃ肠��������aoi���o���Ƃ�(��ŕς���)
	pImage_ = LoadGraph("Assets\\Image\\aoi.png");
	assert(pImage_ >= 0); 
}

void Player::Update()
{
	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->canMove())
		return;

	switch (state_){
	case 0: 
		UpdateNormal();
		break;
	case 1:
		UpdateAttack();
		break;
	default:
		break;
	}
	//�A�j���[�V�����Ɏg���^�C���̍X�V
	time_ += Time::DeltaTime();

	Stage* pStage = GetParent()->FindGameObject<Stage>();
	if (pStage != nullptr) {
		int pushR = pStage->CollisionDown(transform_.position_.x + 50, transform_.position_.y + CHIP_SIZE);
		int pushL = pStage->CollisionDown(transform_.position_.x + 14, transform_.position_.y + CHIP_SIZE);
		int push = max(pushR, pushL);//�Q�̑����̂߂荞�݂̑傫����
		if (push >= 1) {
			transform_.position_.y -= push - 1;
			jumpSpeed_ = 0.0f;
			onGround_ = true;
		}
		else {
			onGround_ = false;
		}

		pushR = pStage->CollisionUp(transform_.position_.x + 50, transform_.position_.y);
		pushL = pStage->CollisionUp(transform_.position_.x + 14, transform_.position_.y);
		push = max(pushR, pushL);//�Q�̑����̂߂荞�݂̑傫����
		if (push >= 1) {
			transform_.position_.y += push + 1;
			jumpSpeed_ = 0.0f;
		}
	}

	std::list<Meteorite*> pMeteos = GetParent()->FindGameObjects<Meteorite>();
	for (Meteorite* pMeteo : pMeteos) {
		if (pMeteo->CollideCircle(transform_.position_.x + CHIP_SIZE/2, 
			                      transform_.position_.y + CHIP_SIZE/2, 20.0f)) {
			pMeteo->KillMe();
			Explosion* pEx = Instantiate<Explosion>(GetParent());
			pEx->SetPosition(transform_.position_.x, transform_.position_.y - CHIP_SIZE / 2);
			KillMe();
			scene->StartGameOver();
		}
	}

	//�����ŃJ�����ʒu�̒���
	Camera* cam = GetParent()->FindGameObject<Camera>();
	int x = (int)transform_.position_.x - cam->GetValue();
	if (x > MAX_POS) {
		x = MAX_POS;
		cam->SetValue((int)transform_.position_.x - x);
	}
	if (x < 0) {
		x = 0;
		cam->SetValue((int)transform_.position_.x + x);
	}
}

void Player::UpdateNormal()
{
	animType_ = 0;//�������[�V����
	Stage* pStage = GetParent()->FindGameObject<Stage>();

	//�ړ��ʂƂ��̏�����
	float moveX, moveY;
	moveX = 0.0f;
	moveY = 0.0f;

	if (CheckHitKey(KEY_INPUT_D)) {//D�L�[�������ƉE�ɐi��
		moveX += SPEED * Time::DeltaTime();
		transform_.position_.x += moveX;//�ړ���
		if (time_ > 0.2f) {
			animFrame_ = (animFrame_ + 1) % 4;
			time_ = 0.0f;
		}
		int hitX = transform_.position_.x + 50;
		int hitY = transform_.position_.y + 63;
		if (pStage != nullptr) {
			int push = pStage->CollisionRight(hitX, hitY);
			transform_.position_.x -= push;
		}
	}
	else if (CheckHitKey(KEY_INPUT_A)) {//A�L�[�������ƍ��ɐi��
		if (transform_.position_.x > 0) {//����ʒ[�Ŏ~�܂�悤��
			moveX += SPEED * Time::DeltaTime();//�ړ���
			transform_.position_.x -= moveX;
			if (time_ > 0.2f) {
				animFrame_ = (animFrame_ + 1) % 4;
				time_ = 0.0f;
			}
			int hitX = transform_.position_.x + 10;
			int hitY = transform_.position_.y + 63;
			if (pStage != nullptr) {
				int push = pStage->CollisionLeft(hitX, hitY);
				transform_.position_.x += push;
			}
		}
	}
	else {
		animFrame_ = 0;
		frameCounter_ = 0;
	}

	if (onGround_) {//�n�ʂɂ��邩
		if (CheckHitKey(KEY_INPUT_SPACE)) {//SPACE�L�[�������ƃW�����v
			jumpSpeed_ = -sqrtf(2 * (gravity_)*JUMP_HEIGHT);
			onGround_ = false;//�n�ʂɂ��Ȃ�
		}
	}

	jumpSpeed_ += gravity_;//���x += �d��
	transform_.position_.y += jumpSpeed_; //���W += ���x

	if (CheckHitKey(KEY_INPUT_E)) {
		if (onGround_) {
			if (prevAttackKey_ == false) {
				time_ = 0.0f;
				state_ = S_Attack;
			}
			prevAttackKey_ = true;
		}
	}
	else {
		prevAttackKey_ = false;
	}
}

void Player::UpdateNormal2()
{
	animType_ = 0;//�������[�V����
	Stage* pStage = GetParent()->FindGameObject<Stage>();

	//�ړ��ʂƂ��̏�����
	float moveX, moveY;
	moveX = 0.0f;
	moveY = 0.0f;

	if (CheckHitKey(KEY_INPUT_D)) {//D�L�[�������ƉE�ɐi��
		moveX += SPEED * Time::DeltaTime();
	}
	else if (CheckHitKey(KEY_INPUT_A)) {//A�L�[�������ƍ��ɐi��
		if (transform_.position_.x > 0) {//����ʒ[�Ŏ~�܂�悤��
			moveX -= SPEED * Time::DeltaTime();
		}
	}
	else {
		animFrame_ = 0;
		frameCounter_ = 0;
	}

	if (onGround_) {//�n�ʂɂ���Ƃ�����
		if (CheckHitKey(KEY_INPUT_SPACE)) {//SPACE�L�[�������ƃW�����v
			jumpSpeed_ = -sqrtf(2 * (gravity_)*JUMP_HEIGHT);
			onGround_ = false;
		}
	}
	//��������
	jumpSpeed_ += gravity_;

	//�������x�Ɉړ��ʂ�������
	moveY = jumpSpeed_;

	//�ړ��ʂ����ƂɃL�����N�^�̍��W���ړ�
	CharMove(&transform_.position_.x, &transform_.position_.y,
		&jumpSpeed_, moveX, moveY, &onGround_);

	if (CheckHitKey(KEY_INPUT_E)) {
		if (onGround_) {
			if (prevAttackKey_ == false) {
				time_ = 0.0f;
				state_ = S_Attack;
			}
			prevAttackKey_ = true;
		}
	}
	else {
		prevAttackKey_ = false;
	}
}

int Player::CharMove(float* _x, float* _y, float *_downSP, 
	float _moveX, float _moveY, bool* _jumpFlag)
{
	Stage pStage = GetParent()->FindGameObject<Stage>();
	float Dummy = 0.0F;

	// �L�����N�^�̍���A�E��A�����A�E�������������蔻��̂���
	// �}�b�v�ɏՓ˂��Ă��邩���ׁA�Փ˂��Ă�����␳����

	// �悸�㉺�ړ����������Ń`�F�b�N
	// �����̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
	if (pStage.StageHitCheck(*_x, *_y + CHIP_SIZE, &Dummy, &_moveY) == 3)
		*_downSP = 0.0F;

	// �E���̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
	if (pStage.StageHitCheck(*_x + CHIP_SIZE, *_y + CHIP_SIZE, &Dummy, &_moveY) == 3)
		*_downSP = 0.0F;

	// ����̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
	if (pStage.StageHitCheck(*_x, *_y, &Dummy, &_moveY) == 4)
		*_downSP *= -1.0F;

	// �E��̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
	if (pStage.StageHitCheck(*_x + CHIP_SIZE, *_y, &Dummy, &_moveY) == 4)
		*_downSP *= -1.0F;

	// �㉺�ړ����������Z
	*_y += _moveY;

	// ��ɍ��E�ړ����������Ń`�F�b�N
	// �����̃`�F�b�N
    pStage.StageHitCheck(*_x, *_y + CHIP_SIZE, &_moveX, &Dummy);

	// �E���̃`�F�b�N
	pStage.StageHitCheck(*_x + CHIP_SIZE, *_y + CHIP_SIZE, &_moveX, &Dummy);

	// ����̃`�F�b�N
	pStage.StageHitCheck(*_x, *_y, &_moveX, &Dummy);

	// �E��̃`�F�b�N
	pStage.StageHitCheck(*_x + CHIP_SIZE, *_y - CHIP_SIZE, &_moveX, &Dummy);

		// ���E�ړ����������Z
		*_x += _moveX;

	// �ڒn����
	// �L�����N�^�̍����ƉE���̉��ɒn�ʂ����邩���ׂ�
	if (pStage.GetChipParam(*_x - CHIP_SIZE * 0.5F, *_y + CHIP_SIZE * 0.5F + 1.0F) == 0 &&
		pStage.GetChipParam(*_y + CHIP_SIZE * 0.5F, *_y + CHIP_SIZE * 0.5F + 1.0F) == 0)
	{
		// ���ꂪ����������W�����v���ɂ���
		*_jumpFlag = TRUE;
	}
		else
		{
			// ���ꂪ�݂�����ڒn���ɂ���
			*_jumpFlag = FALSE;
		}

	// �I��
	return 0;
}

void Player::UpdateAttack()
{
	animType_ = 1;//�������ǍU�����[�V����
	if (animFrame_ + 1 == 3)
	{
		AttackSkill* attack = Instantiate<AttackSkill>(GetParent());
		int x = (int)transform_.position_.x;
		int y = (int)transform_.position_.y;
		attack->SetPosition(x + CHIP_SIZE, y);
		time_ = 0.0f;
		state_ = S_Normal;
	}
	if (time_ > 0.3f) {
		animFrame_ = (animFrame_ + 1) % 3;
		time_ = 0.0f;
	}
}

void Player::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValue();
	}
	DrawRectGraph(x, y, animFrame_ * CHIP_SIZE, animType_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE, pImage_, TRUE);
	//�����蔻�������悤
	DrawCircle(x + CHIP_SIZE/2, y + CHIP_SIZE / 2, 20.0f, GetColor(0, 0, 255), FALSE);
}

void Player::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
	ground_ = _y;
}

void Player::SetGravity(float _gravity)
{
	gravity_ = _gravity;
}