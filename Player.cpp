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
	//�d�̓���:��...1.62,�ΐ�...3.71
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),pImage_(-1),gravity_(INIT_GRAVITY),
	                             jumpSpeed_(0.0f), onGround_(true),
	                             time_(0.0f),animType_(0),animFrame_(0),
	                             prevAttackKey_(false), pLife_(3), invTime_(0), hitFlag_(false)
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
}

void Player::Initialize()
{
	//�v���C���[�̉摜�̓ǂݍ���
	//pImage_ = LoadGraph("Assets\\Image\\robot.png");
	pImage_ = LoadGraph("Assets\\Image\\cyborg.png");
	assert(pImage_ >= 0); 

	hImage_ = LoadGraph("Assets\\Image\\Life.png");
	assert(hImage_ > 0);
}

void Player::Update()
{

	//�v���C�V�[�����瓮���Ă����̂��ǂ������m�F����
	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->canMove())
		return;

	//������ƃv���C���[�̍��W���m�F�������Ƃ��p
	if (CheckHitKey(KEY_INPUT_O)) {
		int tmp = (int)transform_.position_.x;
		int tmp2 = (int)transform_.position_.y;
		printfDx("(%d,%d)", tmp,tmp2);
	}

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
	case 3: //����
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
			animFrame_ = 0;
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
		animType_ = 5;
	}
	else {
		animType_ = 0;//�������ĂȂ��Ƃ�Idle���
	}

	if (time_ > 0.2f) {
		if (onGround_) {
			animFrame_ = animFrame_ % 3 + 1;
			time_ = 0.0f;
		}
	}

	if (CheckHitKey(KEY_INPUT_D) || CheckHitKey(KEY_INPUT_A)
		|| CheckHitKey(KEY_INPUT_SPACE)) {//S_MOVE�ɂ���
		state_ = S_Move;
	}

	//�~�T�C�����΂��Ă̍U��
	if (CheckHitKey(KEY_INPUT_E)) {//���ɔ�΂�
		if (prevAttackKey_ == false) {
			animFrame_ = 0;
			time_ = 0.0f;
			isTypeA = true;
			state_ = S_Attack;//�U���̏�ԂɈڂ�
		}
		prevAttackKey_ = true;
	}
	else {
		prevAttackKey_ = false;
		isTypeA = false;
	}

	if (CheckHitKey(KEY_INPUT_R)) {//�΂ߑO�ɔ�΂�
		if (prevAttackKey_ == false) {
			animFrame_ = 0;
			time_ = 0.0f;
			isTypeB = true;
			state_ = S_Attack;//�U���̏�ԂɈڂ�
		}
		prevAttackKey_ = true;

	}
	else {
		prevAttackKey_ = false;
		isTypeB = false;
	}
}

void Player::UpdateMove()
{
	if (hitFlag_) {//���G�̎��͓_��
		animType_ = 6;
	}
	else {
		animType_ = 1;//�������[�V����
	}
	Stage* pStage = GetParent()->FindGameObject<Stage>();

	//�ړ��ʂƂ��̏�����
	float moveX, moveY;
	moveX = 0.0f;
	moveY = 0.0f;

	//�R���g���[������(�Ȃ����X�e�B�b�N�����ɓ|���Ă����ɂ����Ȃ�)
	//int x, y;
	//GetJoypadAnalogInput(&x, &y, DX_INPUT_PAD1);
	//moveX += (float)x / 1000.0f;
	//if (moveX < 0.3f) {//���S�t�߂̌덷���̂���
	//	 moveX = 0.0f;
	//}

	//�ړ�
	if (CheckHitKey(KEY_INPUT_D)) {//D�L�[�������ƉE�ɐi��
		moveX += SPEED * Time::DeltaTime();
	}
	else if (CheckHitKey(KEY_INPUT_A)) {//A�L�[�������ƍ��ɐi��
		moveX -= SPEED * Time::DeltaTime();
	}
	else {
		time_ = 0;
		animFrame_ = 0;
		frameCounter_ = 0;
		if (onGround_) {
			state_ = S_Normal;
		}
	}

	transform_.position_.x += moveX;//�ړ���

	if (time_ > 0.15f) {
		if (onGround_) {
			animFrame_ = animFrame_ % 5 + 1;
			time_ = 0.0f;
		}
	}

	//�v���C���[�̉E���̃X�e�[�W�Ƃ̓����蔻��
	int hitX = transform_.position_.x + (CHIP_SIZE - MARGIN);//�u���b�N�ƃv���C���[�̗]�����Ȃ������߂Ɉ���
	int hitY = transform_.position_.y + CHIP_SIZE - 1; //���̂܂ܑ����Ɨ����Ă�������-1
	if (pStage != nullptr) {
		int push = pStage->CollisionRight(hitX, hitY);
		transform_.position_.x -= push;
	}
	//�v���C���[�̍����̃X�e�[�W�Ƃ̓����蔻��
	hitX = transform_.position_.x + MARGIN;//�u���b�N�ƃv���C���[�̗]�����Ȃ������߂ɑ���(�Ȃ������ꂾ�ƌ�뉺���葱����Ƃ�����������)
	hitY = transform_.position_.y + CHIP_SIZE - 1;//���̂܂ܑ����Ɨ����Ă�������-1
	if (pStage != nullptr) {
		int push = pStage->CollisionLeft(hitX, hitY);
		transform_.position_.x += push;
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
	if (!onGround_ && jumpSpeed_ < 0) {
		/*animType_ = 2;
		animFrame_ = 2;*/
		animType_ = 2;
		animFrame_ = 0;
		if (time_ > 0.5) {
			animFrame_ = 1;
		}
	}
	else if (!onGround_ && jumpSpeed_ > 0) {
		/*animType_ = 2;
		animFrame_ = 1;*/
		animType_ = 2;
		animFrame_ = 2;
		if (time_ > 0.5) {
			animFrame_ = 3;
		}
	}

	jumpSpeed_ += gravity_;//���x += �d��
	transform_.position_.y += jumpSpeed_; //���W += ���x

	//�~�T�C�����΂��Ă̍U��
	if (CheckHitKey(KEY_INPUT_E)) {
		if (prevAttackKey_ == false) {
			animFrame_ = 0;
			time_ = 0.0f;
			state_ = S_Attack;//�U���̏�ԂɈڂ�
		}
		prevAttackKey_ = true;
	}
	else {
		prevAttackKey_ = false;
	}
}

void Player::UpdateAttack()
{
	
	if (hitFlag_) {//���G�̎��͓_��
		animType_ = 7;
	}
	else {
		animType_ = 3;//�U�����
	}

	if (animFrame_ + 1 == 5)
	{
		AttackSkill* attack = Instantiate<AttackSkill>(GetParent());
		int x = (int)transform_.position_.x;
		int y = (int)transform_.position_.y;
		attack->SetPosition(x + CHIP_SIZE, y);

		//���Ɍ����A�΂߂Ɍ���
		if (isTypeA == true) {
			attack->SetAngle(0);

		}
		if (isTypeB == true) {
			attack->SetAngle(XM_PI / -4);//�p�x��ݒ�
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

	if (time_ > 0.1f) {
		/*animFrame_ = (animFrame_ + 1) % 6;
		time_ = 0.0f;*/
		animFrame_ = animFrame_ % 5 + 1;
		time_ = 0.0f;
	}
}

void Player::UpdateDie()
{
	animType_ = 4;
	if (animFrame_ + 1 == 6) {
		KillMe();
		animFrame_ = 0;
		PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
		scene->StartGameOver();
	}
	if (time_ > 0.2f) {
		animFrame_ = animFrame_ % 5 + 1;
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
	//DrawRectGraph(x, y, animFrame_ * CHIP_SIZE, animType_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE, pImage_, TRUE);
	DrawRectGraph(x, y, animFrame_ * CHIP_SIZE, animType_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE, pImage_, TRUE);
	//�����蔻�������悤
	//DrawCircle(x + CHIP_SIZE / 4, y + CHIP_SIZE / 2, 20.0f, GetColor(0, 0, 255), FALSE);
	//DrawCircle(x + CHIP_SIZE/2, y + CHIP_SIZE / 2, 20.0f, GetColor(0, 0, 255), FALSE);
	for (int i = 0; i < pLife_; i++) {
		DrawGraph(LIFE_IMAGE_SIZE * i, 0, hImage_, TRUE);
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