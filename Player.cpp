#include "Player.h"
#include "Engine/time.h"
#include "Camera.h"
#include "Stage.h"
#include "Meteorite.h"
#include "PlayScene.h"
#include "Explosion.h"
#include "AttackSkill.h"
#include "Flag.h"
#include "Enemy.h"

namespace {
	const float CHIP_SIZE = 64.0f;//�L�����̉摜�T�C�Y
	//const float CHIP_SIZE = 60.0f;//�L�����̉摜�T�C�Y
	const int MAP_HEIGHT = 720; //����
	const float ROBO_WIDTH = 48;
	const XMFLOAT3 INIT_POS = { 30,580,0 };//�ŏ��̈ʒu
	const float JUMP_HEIGHT = 64.0f * 3.0f;//�W�����v�̍���
	const float INIT_GRAVITY = 9.8/ 90.0f;
	const float MAX_POS = 400;
	const int SPEED = 150;
	//const int SPEED = 500;
	const int MARGIN = 14;//�v���C���[�̃`�b�v�̗]��
	const int LIFE_IMAGE_SIZE = 64;//�̗͉摜�T�C�Y
	const float FINV_TIME = 1.0f;//���G���I��鎞��
	const int MAX_BULLET = 10; //�~�T�C���̔��ˉ\��
	//�d�̓���:��...1.62,�ΐ�...3.71
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), pImage_(-1), gravity_(INIT_GRAVITY),
	 jumpSpeed_(0.0f), onGround_(true), time_(0.0f), animType_(0),
	 animFrame_(0), frameCounter_(0),prevAttackKey_(false), pLife_(3),
	 invTime_(0), hitFlag_(false),lImage_(-1),dImage_(-1),ground_(0),
	 prevMoveKey_(0),currentNum_(MAX_BULLET)
{
	//�����ʒu�̒���
	transform_.position_ = INIT_POS;
	state_ = S_Normal;
}

Player::~Player()
{
	//�����Ə���
	if (pImage_ > 0) {
		DeleteGraph(pImage_);
		pImage_ = -1;
	}
	if (lImage_ > 0) {
		DeleteGraph(lImage_);
		lImage_ = -1;
	}
	if (dImage_ > 0) {
		DeleteGraph(dImage_);
		dImage_ = -1;
	}
}

void Player::Initialize()
{
	//�v���C���[�̉摜�̓ǂݍ���
	//pImage_ = LoadGraph("Assets\\Image\\robot.png");
	pImage_ = LoadGraph("Assets\\Image\\cyborg3.png");
	assert(pImage_ >= 0); 

	lImage_ = LoadGraph("Assets\\Image\\Life.png");
	assert(lImage_ > 0);

	dImage_ = LoadGraph("Assets\\Image\\Damege.png");
	assert(dImage_ > 0);
}

void Player::Update()
{

	//�v���C�V�[�����瓮���Ă����̂��ǂ������m�F����
	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->canMove())
		return;

	//�X�e�[�g���g���ĕ��ʂ̏�ԂƍU���̏�ԂȂǂ��Ăѕ�����
	switch (state_){
	case 0: //�m�[�}��
		UpdateNormal();
		break;
	case 1: //��������A�W�����v
		UpdateMove();
		break;
	case 2: //�A�^�b�N
		UpdateAttack();
		break;
	case 3: //�Z��
		UpdateDie();
		break;
	default:
		break;
	}

	//�A�j���[�V�����Ɏg���^�C���̍X�V
	time_ += Time::DeltaTime();

	//�X�e�[�W�Ƃ̏㉺�̓����蔻��
	Stage* pStage = GetParent()->FindGameObject<Stage>();
	if (pStage != nullptr) {
		int pushR = pStage->CollisionDown(transform_.position_.x + CHIP_SIZE - MARGIN, transform_.position_.y + CHIP_SIZE);
		int pushL = pStage->CollisionDown(transform_.position_.x + MARGIN, transform_.position_.y + CHIP_SIZE);
		int push = max(pushR, pushL);//�Q�̑����̂߂荞�݂̑傫����
		if (push >= 1) {
			transform_.position_.y -= push - 1;
			jumpSpeed_ = 0.0f;
			onGround_ = true;
		}
		else {
			onGround_ = false;
		}

		pushR = pStage->CollisionUp(transform_.position_.x + CHIP_SIZE -MARGIN, transform_.position_.y);
		pushL = pStage->CollisionUp(transform_.position_.x + MARGIN, transform_.position_.y);
		push = max(pushR, pushL);//�Q�̑����̂߂荞�݂̑傫����
		if (push >= 1) {
			transform_.position_.y += push + 1;
			jumpSpeed_ = 0.0f;
		}
	}

	//覐΂Ƃ̓����蔻��
	std::list<Meteorite*> pMeteos = GetParent()->FindGameObjects<Meteorite>();
	for (Meteorite* pMeteo : pMeteos) {
		if (pMeteo->CollideCircle(transform_.position_.x + CHIP_SIZE/4, 
			                      transform_.position_.y + CHIP_SIZE/2, 20.0f)) {
			pMeteo->KillMe();
			Explosion* pEx = Instantiate<Explosion>(GetParent());
			pEx->SetPosition(transform_.position_.x, transform_.position_.y - CHIP_SIZE / 2);
			if (prevMoveKey_ == 0) {
				animFrame_ = 0;
			}
			else {
				animFrame_ = 5;
			}
			state_ = S_Die;
			//KillMe();
			//scene->StartGameOver();
		}
	}

	//�G�Ƃ̓����蔻��
	std::list<Enemy*> pEnemys = GetParent()->FindGameObjects<Enemy>();
	for (Enemy* pEnemy : pEnemys) {
		if (hitFlag_ == false) {
			if (pEnemy->CollideCircle(transform_.position_.x + CHIP_SIZE / 4,
				transform_.position_.y + CHIP_SIZE / 2, 20.0f)) {
				//Explosion* pEx = Instantiate<Explosion>(GetParent());
				//pEx->SetPosition(transform_.position_.x, transform_.position_.y - CHIP_SIZE / 2);
				//state_ = S_Die;
				pLife_ -= 1;
				hitFlag_ = true;
			}
		}
	}

	if (hitFlag_ == true) {//�G�����������班���̊Ԗ��G�ɂȂ�
		invTime_ += Time::DeltaTime();
		if (invTime_ >= FINV_TIME) {
			hitFlag_ = false;
			invTime_ = 0;
		}
	}

	if (pLife_ == 0)//���C�t���O�ɂȂ����玀��
	{
		state_ = S_Die;
	}

	//�S�[���̊��Ƃ̓����蔻��
	Flag* pFlag = GetParent()->FindGameObject<Flag>();
	if (pFlag->HitFlag(transform_.position_.x +CHIP_SIZE / 2,
		               transform_.position_.y + CHIP_SIZE / 2,20.0f)) {
		scene->StartClear();
	}

	//��ʊO(��)�ɗ�������Q�[���I�[�o
	if (transform_.position_.y >= MAP_HEIGHT) {
		KillMe();
		scene->StartGameOver();
	}

	//��ʊO(���E)�ɍs���Ȃ��悤��
	if (transform_.position_.x < 0) {
		transform_.position_.x = 0;
	}
	if (transform_.position_.x > 4600) {
		transform_.position_.x = 4600;
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
	if (x > 3800) {
		x = 3800;
		cam->SetValue((int)transform_.position_.x - x);
	}
}

void Player::UpdateNormal()
{
	if (hitFlag_) {//���G�̎��͓_��
		if (prevMoveKey_ == 0) {
			animType_ = 5;
		}
		else{
			animType_ = 15;
		}
	}
	else {
		if (prevMoveKey_ == 0) {
			animType_ = 0;//�������ĂȂ��Ƃ�Idle���
		}
		else {
			animType_ = 10;
		}
	}

	if (time_ > 0.2f) {
		if (onGround_) {
			if (prevMoveKey_ == 0) {
				animFrame_ = animFrame_ % 3 + 1;
				time_ = 0.0f;
			}
			else {
				if (animFrame_ == 2) {
					animFrame_ = 5;
				}
				else {
					animFrame_--;
				}
				time_ = 0.0f;
			}
		}
	}

	int x, y;
	GetJoypadAnalogInput(&x, &y, DX_INPUT_PAD1);
	x = x / 1000.0f;

	if (CheckHitKey(KEY_INPUT_D) || CheckHitKey(KEY_INPUT_A)
		|| CheckHitKey(KEY_INPUT_SPACE) || x > 0.3f || x < -0.3f) {//S_MOVE�ɂ���
		state_ = S_Move;
	}

	//�~�T�C�����΂��Ă̍U��
	if (CheckHitKey(KEY_INPUT_E) && currentNum_ > 0) {//���ɔ�΂�
		ReadyAttack(isTypeA);//�e�̏���
	}
	else {
		prevAttackKey_ = false;
		isTypeA = false;
	}

	if (CheckHitKey(KEY_INPUT_R) && currentNum_ > 0) {//�΂ߑO�ɔ�΂�
		ReadyAttack(isTypeB);//�e�̏���
	}
	else {
		prevAttackKey_ = false;
		isTypeB = false;
	}

	if (CheckHitKey(KEY_INPUT_H))//�e�̃����[�h
	{
		Reload();
	}
}

void Player::UpdateMove()
{
	if (hitFlag_) {//���G�̎��͓_��
		if (prevMoveKey_ == 0) {
			animType_ = 6;
		}
		else {
			animType_ = 16;
		}
	}
	else {
		if (prevMoveKey_ == 0) {
			animType_ = 1;//�������[�V����
		}
		else {
			animType_ = 11;
		}
	}
	Stage* pStage = GetParent()->FindGameObject<Stage>();

	//�ړ��ʂƂ��̏�����
	float moveX, moveY;
	moveX = 0.0f;
	moveY = 0.0f;

	//�R���g���[������(�Ȃ����X�e�B�b�N�����ɓ|���Ă����ɂ����Ȃ�)
	int x, y;
	GetJoypadAnalogInput(&x, &y, DX_INPUT_PAD1);//�X�e�B�b�N�̓|���������Ă���
	if (x < 0) {
		moveX += (float)x / 1000.0f; //����Ă����l���g����悤�ɏ���������
	}

	moveX += (float)x / 1000.0f; //����Ă����l���g����悤�ɏ���������
	
	if (moveX < 0.3f && moveX > -0.3f) {//���S�t�߂̌덷���̂���
		 moveX = 0.0f;
	}

	//�R���g���[���ł��E���̓�����؂�ւ�����悤��
	if (moveX > 0) {
		prevMoveKey_ = 0;//�E����
	}
	else if(moveX < 0) {
		prevMoveKey_ = 1;//������
	}

	//�ړ�
	if (CheckHitKey(KEY_INPUT_D)) {//D�L�[�������ƉE�ɐi��
		moveX += SPEED * Time::DeltaTime();
		prevMoveKey_ = 0;//�E����
	}
	else if (CheckHitKey(KEY_INPUT_A)) {//A�L�[�������ƍ��ɐi��
		moveX -= SPEED * Time::DeltaTime();
		prevMoveKey_ = 1;//������
	}
	
	if (moveX == 0) {
		if (onGround_) {            //�n�ʂɂ���Ȃ�
			state_ = S_Normal;      //�m�[�}���ɖ߂�����
			if (prevMoveKey_ == 0) {//�ŏI���E�����Ȃ�
				animFrame_ = 0;
			}
			else {//�ŏI���������Ȃ�
				animFrame_ = 5;
			}
			time_ = 0;
		}
	}

	transform_.position_.x += moveX;//�ړ���

	if (prevMoveKey_ == 0) {//�E�����Ȃ�
		//�A�j���[�V����
		if (time_ > 0.15f) {
			if (onGround_) {
				animFrame_ = animFrame_ % 5 + 1;
				time_ = 0.0f;
			}
		}
	}
	else {//�������Ȃ�
		//�������̕���
		if (time_ > 0.15f) {
			if (onGround_) {
				if (animFrame_ == 0) {
					animFrame_ = 5;
				}
				else {
					animFrame_--;
				}

				time_ = 0.0f;
			}
		}
	}

	//�v���C���[�̉E���̃X�e�[�W�Ƃ̓����蔻��
	int hitX = transform_.position_.x + (CHIP_SIZE - MARGIN);//�u���b�N�ƃv���C���[�̗]�����Ȃ������߂Ɉ���
	int hitY = transform_.position_.y + CHIP_SIZE - 1; //���̂܂ܑ����Ɨ����Ă�������-1
	if (pStage != nullptr) {
		int push = pStage->CollisionRight(hitX, hitY);//�E���̓����蔻��
		transform_.position_.x -= push;//�̂߂肱��ł镪�߂�
	}
	//�v���C���[�̍����̃X�e�[�W�Ƃ̓����蔻��
	hitX = transform_.position_.x + MARGIN;//�u���b�N�ƃv���C���[�̗]�����Ȃ������߂ɑ���(�Ȃ������ꂾ�ƌ�뉺���葱����Ƃ�����������)
	hitY = transform_.position_.y + CHIP_SIZE - 1;//���̂܂ܑ����Ɨ����Ă�������-1
	if (pStage != nullptr) {
		int push = pStage->CollisionLeft(hitX, hitY);//�����̓����蔻��
		transform_.position_.x += push;//�̂߂肱��ł镪�߂�
	}

	//�ύX�O�̈ړ�
	//if (CheckHitKey(KEY_INPUT_D)) {//D�L�[�������ƉE�ɐi��
	//	moveX += SPEED * Time::DeltaTime();
	//	transform_.position_.x += moveX;//�ړ���
	//	if (time_ > 0.2f) {
	//		if (onGround_) {
	//			animFrame_ = animFrame_ % 8 + 1;
	//			time_ = 0.0f;
	//		}
	//	}
	//	int hitX = transform_.position_.x + 50;
	//	int hitY = transform_.position_.y + CHIP_SIZE -1;
	//	if (pStage != nullptr) {
	//		int push = pStage->CollisionRight(hitX, hitY);
	//		transform_.position_.x -= push;
	//	}
	//}
	//else if (CheckHitKey(KEY_INPUT_A)) {//A�L�[�������ƍ��ɐi��
	//	if (transform_.position_.x > 0) {//����ʒ[�Ŏ~�܂�悤��
	//		moveX -= SPEED * Time::DeltaTime();//�ړ���
	//		transform_.position_.x += moveX;
	//		if (time_ > 0.2f) {
	//			if (onGround_) {
	//				animFrame_ = animFrame_ % 8 + 1;
	//				time_ = 0.0f;
	//			}
	//		}
	//		int hitX = transform_.position_.x + 10;
	//		int hitY = transform_.position_.y + CHIP_SIZE-1;
	//		if (pStage != nullptr) {
	//			int push = pStage->CollisionLeft(hitX, hitY);
	//			transform_.position_.x += push;
	//		}
	//	}
	//}
	//else {
	//	animFrame_ = 0;
	//	frameCounter_ = 0;
	//}

	if (onGround_) {//�n�ʂɂ��邩
		if (CheckHitKey(KEY_INPUT_SPACE)) {//SPACE�L�[�������ƃW�����v
			jumpSpeed_ = -sqrtf(2 * (gravity_)*JUMP_HEIGHT);
			onGround_ = false;//�n�ʂɂ��Ȃ�
		}
	}

	//�W�����v�̎��ƍ~��Ă鎞�̃A�j���[�V�����̕ύX
	if (!onGround_ && jumpSpeed_ < 0) {//�n�ʂɂ��Ȃ��ăW�����v�̏�ɍs���r���Ȃ�
		/*animType_ = 2;
		animFrame_ = 2;*/
		if (prevMoveKey_ == 0) {//�E�����Ȃ�
			animType_ = 2;
			animFrame_ = 0;
			if (time_ > 0.5) {
				animFrame_ = 1;
			}
		}
		if (prevMoveKey_ == 1) {//�������Ȃ�
			animType_ = 12;
			animFrame_ = 5;
			if (time_ > 0.5) {
				animFrame_ = 4;
			}
		}
	}
	else if (!onGround_ && jumpSpeed_ > 0) {//�n�ʂɂ��Ȃ��āA�W�����v�ŉ������Ă���Ȃ�
		/*animType_ = 2;
		animFrame_ = 1;*/
		if (prevMoveKey_ == 0) {//�E�����Ȃ�
			animType_ = 2;
			animFrame_ = 2;
			if (time_ > 0.5) {
				animFrame_ = 3;
			}
		}
		if (prevMoveKey_ == 1) {//�������Ȃ�
			animType_ = 12;
			animFrame_ = 3;
			if (time_ > 0.5) {
				animFrame_ = 2;
			}
		}
	}

	jumpSpeed_ += gravity_;//���x += �d��
	transform_.position_.y += jumpSpeed_; //���W += ���x

	//�~�T�C�����΂��Ă̍U��
	if (CheckHitKey(KEY_INPUT_E) && currentNum_ > 0) {//�U���̃L�[�������̂ƁA�c�e������Ȃ�
		ReadyAttack(isTypeA);//�e�̏���
	}
	else {
		prevAttackKey_ = false;
		isTypeA = false;
	}

	if (CheckHitKey(KEY_INPUT_R) && currentNum_ > 0) {//�΂ߑO�ɔ�΂�
		ReadyAttack(isTypeB);//�e�̏���
	}
	else {
		prevAttackKey_ = false;
		isTypeB = false;
	}

	if (CheckHitKey(KEY_INPUT_H))//�e�̃����[�h
	{
		Reload();
	}
}

void Player::UpdateAttack()
{
	if (hitFlag_) {//���G�̎��͓_��
		if (prevMoveKey_ == 0) {//�E�����Ȃ�
			animType_ = 8;
		}
		else {
			animType_ = 18;
		}
	}
	else {
		if (prevMoveKey_ == 0) {//�������Ȃ�
			animType_ = 3;
		}
		else {
			animType_ = 13;
		}
	}

	//�A�j���[�V�������Ō�ɂȂ�����e��ł��o��
	if (prevMoveKey_ == 0) {//�E�����Ȃ�
		if (animFrame_ + 1 == 5)
		{
			Attack(0,1);
		}
	}
	else{//�������Ȃ�
		if (animFrame_ - 1 == 0)
		{
			Attack(1,3);
			animFrame_ = 5;
		}
	}

	if (prevMoveKey_ == 0) {
		if (time_ > 0.1f) {
			animFrame_ = animFrame_ % 5 + 1;
			time_ = 0.0f;
		}
	}
	else {
		if (time_ > 0.1f) {
			if (animFrame_ == 0) {
				animFrame_ = 5;
			}
			else {
				animFrame_--;
			}
			time_ = 0.0f;
		}
	}
}

void Player::UpdateDie()
{
	if (prevMoveKey_ == 0) {
		animType_ = 4;//���ʎ��̃A�j���[�V����(�E)
	}
	else {
		animType_ = 14;//���ʂƂ��̃A�j���[�V����(��)
	}

	//���񂾎��ɒn�ʂɂ��Ȃ��Ȃ�n�ʂɗ��Ƃ��Ă���Z��
	if (!onGround_) {
		transform_.position_.y += 3.0f;
		if (prevMoveKey_ == 0) {
			animFrame_ = 1;
		}
		else {
			animFrame_ = 4;
		}
	}
	else {
		if (prevMoveKey_ == 0) {
			if (animFrame_ + 1 == 6) {//�Ō�̃t���[���ɂȂ�����
				Die();//�Z��
				animFrame_ = 0;
			}
			if (time_ > 0.2f) {
				animFrame_ = animFrame_ % 5 + 1;
				time_ = 0.0f;
			}
		}
		else {
			if (animFrame_ == 0) {//�Ō�̃t���[���ɂȂ�����
				Die();//�Z��
				animFrame_ = 5;
			}
			if (time_ > 0.2f) {
				animFrame_--;
				time_ = 0.0f;
			}
		}
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
	//DrawRectGraph(x, y, animFrame_ * CHIP_SIZE, animType_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE, pImage_, TRUE);
	DrawRectGraph(x, y, animFrame_ * CHIP_SIZE, animType_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE, pImage_, TRUE);
	//�����蔻�������悤
	//DrawCircle(x + CHIP_SIZE / 4, y + CHIP_SIZE / 2, 20.0f, GetColor(0, 0, 255), FALSE);
	//DrawCircle(x + CHIP_SIZE/2, y + CHIP_SIZE / 2, 20.0f, GetColor(0, 0, 255), FALSE);
	
	//���C�t�ƃ��C�t�̉��ɍ����������C�t��`��
	for (int j = 0; j < 3; j++) {
		DrawGraph(LIFE_IMAGE_SIZE * j, 0, dImage_, TRUE);
	}
	for (int i = 0; i < pLife_; i++) {
		DrawGraph(LIFE_IMAGE_SIZE * i, 0, lImage_, TRUE);
	}
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

void Player::Reload()
{
	currentNum_ = MAX_BULLET;
}

void Player::ReadyAttack(bool &_isType)
{
	if (prevAttackKey_ == false) {
		if (prevMoveKey_ == 0) {
			animFrame_ = 0;
			animType_ = 3;
		}
		else {
			animFrame_ = 5;
			animType_ = 13;
		}
		time_ = 0.0f;
		_isType = true;
		currentNum_ -= 1;
		state_ = S_Attack;//�U���̏�ԂɈڂ�
	}
	prevAttackKey_ = true;
}

void Player::Attack(int angleA, int angleB)
{
	AttackSkill* attack = Instantiate<AttackSkill>(GetParent());
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	attack->SetPosition(x + CHIP_SIZE, y);

	//���Ɍ����A�΂߂Ɍ���
	if (isTypeA == true) {//���Ɍ���
		attack->SetAngle(XM_PI*angleA);//�p�x�̐ݒ�
	}
	if (isTypeB == true) {//�΂ߏ�Ɍ���
		attack->SetAngle(XM_PI / -4 * angleB);//�p�x��ݒ�
	}

	time_ = 0.0f;
	animFrame_ = 0;
	if (onGround_) {
		state_ = S_Normal;
	}
	else {
		state_ = S_Move;
	}
}

void Player::Die()
{
	KillMe();//�o�C�o�C
	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	scene->StartGameOver();//�Q�[���I�[�o�V�[���Ɉڍs
}
