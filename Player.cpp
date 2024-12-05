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
#include "FlyEnemy.h"
#include "Health.h"
#include "Shield.h"
#include "MissileItem.h"
#include "ReloadGauge.h"

namespace {
	const float CHIP_SIZE = 64.0f;//�L�����̉摜�T�C�Y
	const int MAP_HEIGHT = 720;   //����
	const float ROBO_WIDTH = 48;  //�L�����̉���
	const XMFLOAT3 INIT_POS = { 30,580,0 };//�ŏ��̈ʒu
	const float JUMP_HEIGHT = 64.0f * 2.5f;//�W�����v�̍���
	const float JUMP_SPEED = 10;
	const float INIT_GRAVITY = 9.8; //�d��
	const float MAX_POS = 400;//�J�������������ɂ�����ő�̈ʒu
	const int SPEED = 250;    //�X�s�[�h
	const int R_MARGIN = 24;    //�v���C���[�̃`�b�v�̗]��
	const int L_MARGIN = 1;
	const int IMAGE_SIZE = 64;//�̗͉摜�T�C�Y
	//const int LIFE_IMAGE_SIZE = 64;//�̗͉摜�T�C�Y
	const int MISSILE_SIZE = 30;   //�~�T�C���摜�T�C�Y
	const float FINV_TIME = 1.0f;  //���G���I��鎞��
	const int MAX_BULLET = 10;     //�~�T�C���̔��ˉ\��
	const float INTERVAL = 3.0f;   //�����[�h����
	const int MAX_X = 7680;        //X���W�̃}�b�N�X
	const float FIN_ITEM_TIME = 10.0f; //�~�T�C���A�C�e�����I��鎞��

	//�d�̓���:��...1.62,�ΐ�...3.71
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), pImage_(-1), gravity_(INIT_GRAVITY),
	jumpSpeed_(0.0f), onGround_(true), time_(0.0f), animType_(0),
	animFrame_(0), prevAttackKey_(false), pLife_(3),fpsCount_(0),
	invTime_(0), hitFlag_(false), lImage_(-1), dImage_(-1), ground_(0),
	prevMoveKey_(0), currentNum_(MAX_BULLET), reloadTime_(0), mImage_(-1),
	bImage_(-1), reloading_(false), getShield_(false), sImage_(-1), iImage_(-1),
	getMissileItem_(false), itemTime_(0.0f), mAnimFrame_(0), iTime_(0.0f),
	fps_(0), fpsTimer_(0.0f), fImage_(-1), canJump_(true), eSound_(-1), jTime_(0),
	mSound_(-1), rSound_(-1), SEJump_(-1), SEItem_(-1), SEDamage_(-1), prevY_(0)
{
	//�����ʒu�̒���
	transform_.position_ = INIT_POS;
	state_ = S_Normal;
}

Player::~Player()
{
	//���g�����Ă���Ȃ炿���Ə���
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
	if (mImage_ > 0) {
		DeleteGraph(mImage_);
		mImage_ = -1;
	}
	if (bImage_ > 0) {
		DeleteGraph(bImage_);
		bImage_ = -1;
	}
	if (sImage_ > 0) {
		DeleteGraph(sImage_);
		sImage_ = -1;
	}
	if (iImage_ > 0) {
		DeleteGraph(iImage_);
		iImage_ = -1;
	}
	if (fImage_ > 0) {
		DeleteGraph(fImage_);
		fImage_ = -1;
	}
}

void Player::Initialize()
{
	//�v���C���[�̉摜�̓ǂݍ���
	pImage_ = LoadGraph("Assets\\Image\\cyborg.png");
	assert(pImage_ >= 0); 

	//�̗͉摜�̓ǂݍ���
	lImage_ = LoadGraph("Assets\\Image\\Life4.png");
	assert(lImage_ > 0);

	//�̗͍��ꂽ�Ƃ���Ɏg���摜�̓ǂݍ���
	dImage_ = LoadGraph("Assets\\Image\\Damage2.png");
	assert(dImage_ > 0);

	//�c�e�\���Ɏg���摜�̓ǂݍ���
	mImage_ = LoadGraph("Assets\\Image\\missile.png");
	assert(mImage_ > 0);

	//�e�̃o�i�[�̓ǂݍ���
	bImage_ = LoadGraph("Assets\\Image\\banner.png");
	assert(bImage_ > 0);

	//�V�[���h����
	sImage_ = LoadGraph("Assets\\Image\\Shield.png");
	assert(sImage_ > 0);

	//�~�T�C���A�C�e������
	iImage_ = LoadGraph("Assets\\Image\\Missile_Icon.png");
	assert(iImage_ > 0);

	//���C�t�o�i�[
	fImage_ = LoadGraph("Assets\\Image\\LifeFrame.png");
	assert(fImage_ > 0);

	//�������̃��[�h
	eSound_ = LoadSoundMem("Assets/Sound/Explosion.mp3");
	assert(eSound_ > 0);

	//�~�T�C�����̃��[�h
	mSound_ = LoadSoundMem("Assets/Sound/Missile.mp3");
	assert(mSound_ > 0);

	//�����[�h���̃��[�h
	rSound_ = LoadSoundMem("Assets/Sound/Reload.mp3");
	assert(rSound_ > 0);

	//�W�����v���̃��[�h
	SEJump_ = LoadSoundMem("Assets/Sound/jump.mp3");
	assert(SEJump_ > 0);

	//�A�C�e���擾�̌��ʉ����[�h
	SEItem_ = LoadSoundMem("Assets/Sound/Item.mp3");
	assert(SEItem_ > 0);

	//�_���[�W���̃��[�h
	SEDamage_ = LoadSoundMem("Assets/Sound/damage.mp3");
	assert(SEDamage_ > 0);
}

void Player::Update()
{
	//�v���C�V�[�����瓮���Ă����̂��ǂ������m�F����
	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->canMove())//��������_���̏ꍇ
		return;           //�Ȃɂ����Ȃ�

	//fps�m�F�p
	if (fpsTimer_ >= 1.0f) {
		fpsTimer_ = 0.0f;
		fps_ = fpsCount_;
		fpsCount_ = 0;
	}
	fpsTimer_ += Time::DeltaTime();
	fpsCount_++;

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

	//���E�̌����Ŏ኱�ς�铖���蔻��̈ʒu�̏C��
	float colX,colY,colR;
	if (prevMoveKey_ == 0) {
		colX = transform_.position_.x + CHIP_SIZE / 4;
	}
	else {
		colX = transform_.position_.x + CHIP_SIZE / 10 * 3;
	}
	colY = transform_.position_.y + CHIP_SIZE / 5 * 3;
	colR = 20.0f;

	//�X�e�[�W�Ƃ̏㉺�̓����蔻��
	Stage* pStage = GetParent()->FindGameObject<Stage>();
	if (pStage != nullptr) {
		int pushR = pStage->CollisionDown(transform_.position_.x + CHIP_SIZE / 15 * 7, transform_.position_.y + CHIP_SIZE);
		int pushL = pStage->CollisionDown(transform_.position_.x + CHIP_SIZE / 7, transform_.position_.y + CHIP_SIZE);
		int push = max(pushR, pushL);//�Q�̑����̂߂荞�݂̑傫����
		if (push >= 1) {
			transform_.position_.y -= push - 1;
			jumpSpeed_ = 0.0f;
			onGround_ = true;
		}
		else {
			onGround_ = false;
		}
		/*pushR = pStage->CollisionUp(transform_.position_.x + CHIP_SIZE - R_MARGIN, transform_.position_.y + CHIP_SIZE / 4);
		pushL = pStage->CollisionUp(transform_.position_.x + 9, transform_.position_.y + CHIP_SIZE / 4);*/
		pushR = pStage->CollisionUp(transform_.position_.x + CHIP_SIZE / 15 * 7, transform_.position_.y + CHIP_SIZE / 4);
		pushL = pStage->CollisionUp(transform_.position_.x + CHIP_SIZE / 7, transform_.position_.y + CHIP_SIZE / 4);
		push = max(pushR, pushL);//�Q�̓���̂߂荞�݂̑傫����
		if (push >= 1) {
			transform_.position_.y += push + 1;
			jumpSpeed_ = 0.0f;
		}
	}

	//覐΂Ƃ̓����蔻��
	std::list<Meteorite*> pMeteos = GetParent()->FindGameObjects<Meteorite>();
	for (Meteorite* pMeteo : pMeteos) {
		if (pMeteo->CollideCircle(colX, colY, colR)) {
			pMeteo->KillMe();
			Explosion* pEx = Instantiate<Explosion>(GetParent());
			pEx->SetPosition(transform_.position_.x, transform_.position_.y - CHIP_SIZE / 2);
			if (prevMoveKey_ == 0) {
				animFrame_ = 0;
			}
			else {
				animFrame_ = 5;
			}

			if (getShield_ == false) {
				state_ = S_Die;
			}
			else {
				hitFlag_ = true;
				getShield_ = false;
			}
			PlaySoundMem(eSound_, DX_PLAYTYPE_BACK);
			// ���ʂ̐ݒ�
			ChangeVolumeSoundMem(255 * 50 / 100, eSound_);
		}
	}

	//�G�Ƃ̓����蔻��
	std::list<Enemy*> pEnemys = GetParent()->FindGameObjects<Enemy>();
	for (Enemy* pEnemy : pEnemys) {
		if (hitFlag_ == false) {
			if (pEnemy->CollideCircle(colX, colY, colR)) {
				if (getShield_ == false) {
					pLife_ -= 1;
				}
				else {
					getShield_ = false;
				}
				hitFlag_ = true;
				PlaySoundMem(SEDamage_, DX_PLAYTYPE_BACK);
			}
		}
	}

	//���ł�G�Ƃ̓����蔻��
	std::list<FlyEnemy*> fEnemys = GetParent()->FindGameObjects<FlyEnemy>();
	for (FlyEnemy* fEnemy : fEnemys) {
		if (hitFlag_ == false) {
			if (fEnemy->CollideCircle(colX, colY, colR)) {
				if (getShield_ == false) {
					pLife_ -= 1;
				}
				else {
					getShield_ = false;
				}
				hitFlag_ = true;
				PlaySoundMem(SEDamage_, DX_PLAYTYPE_BACK);
			}
		}
	}

	//�񕜃A�C�e���Ƃ̓����蔻��
	std::list<Health*> pHealth = GetParent()->FindGameObjects<Health>();
	for (Health* pHealth : pHealth) {
		if (pHealth->CollideCircle(colX, colY, colR)) {
			//�̗͂�3�̎��͑��₳�Ȃ�
			if (pLife_ < 3) {
				pLife_ += 1;
			}
			PlaySoundMem(SEItem_, DX_PLAYTYPE_BACK);
			pHealth->KillMe();
		}
	}

	//�h��A�C�e���Ƃ̓����蔻��
	std::list<Shield*> pShield = GetParent()->FindGameObjects<Shield>();
	for (Shield* pShield : pShield) {
		if (pShield->CollideCircle(colX, colY, colR)) {
			getShield_ = true;
			PlaySoundMem(SEItem_, DX_PLAYTYPE_BACK);
			pShield->KillMe();
		}
	}

	//�~�T�C���A�C�e���Ƃ̓����蔻��
	std::list<MissileItem*> pMissileItem = GetParent()->FindGameObjects<MissileItem>();
	for (MissileItem* pMissileItem : pMissileItem) {
		if (pMissileItem->CollideCircle(colX, colY, colR)) {
			getMissileItem_ = true;
			currentNum_ = MAX_BULLET;
			PlaySoundMem(SEItem_, DX_PLAYTYPE_BACK);
			pMissileItem->KillMe();
		}
	}
	//�~�T�C���A�C�e�����Ƃ������̏���
	if (getMissileItem_ == true) {
		Icon();
	}

	//�G�����������班���̊Ԗ��G�ɂȂ�
	if (hitFlag_ == true) {
		invTime_ += Time::DeltaTime();
		if (invTime_ >= FINV_TIME) {
			hitFlag_ = false;
			invTime_ = 0;
		}
	}

	//���C�t���O�ɂȂ����玀��
	if (pLife_ == 0)
	{
		state_ = S_Die;
	}

	//�S�[���̊��Ƃ̓����蔻��
	Flag* pFlag = GetParent()->FindGameObject<Flag>();
	if (pFlag->HitFlag(colX,colY,colR)) {
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
	if (transform_.position_.x > MAX_X) {
		transform_.position_.x = MAX_X;
	}

	//�����ŃJ�����ʒu�̒���
	Camera* cam = GetParent()->FindGameObject<Camera>();
	int x = (int)transform_.position_.x + cam->GetValueX();
	//��ʂ̂P/�R���炢�܂ł���ƃJ�����������悤��
	if (x > MAX_POS) {
		x = MAX_POS;
		cam->SetValueX((int)transform_.position_.x - x);
	}
	/*if (x < 0) {
		x = 0;
		cam->SetValueX((int)transform_.position_.x - x);
	}
	if (x > 3600) {
		x = 3600;
		cam->SetValueX((int)transform_.position_.x - x);
	}*/

	//if (transform_.position_.y <= 0) {
	//	jumpSpeed_ = 0;
	//	//transform_.position_.y = 1;
	//}
	
	//�����[�h���Ȃ�
	if (reloading_) {
		Reload();
	}
}

void Player::UpdateNormal()
{
	//���G�̎��͓_��
	if (hitFlag_) {
		if (prevMoveKey_ == 0) {
			animType_ = 5;
		}
		else {
			animType_ = 15;
		}
	}
	else {
		if (prevMoveKey_ == 0) {
			animType_ = 0;
		}
		else {
			animType_ = 10;
		}
	}

	//�A�j���[�V�����X�V
	if (time_ > 0.2f) {
		if (onGround_) {
			animFrame_ = animFrame_ % 3 + 1;
			time_ = 0.0f;
		}
	}

	//�R���g���[���̏��Ƃ�
	int x, y;
	DINPUT_JOYSTATE input;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);
	x = input.X / 1000.0f;

	//�Ȃ񂩓����Ȃ�S_Move�Ɉړ�
	if (CheckHitKey(KEY_INPUT_D) || CheckHitKey(KEY_INPUT_A)
		|| CheckHitKey(KEY_INPUT_SPACE) || x > 0.3f || x < -0.3f
		|| ((input.Buttons[0] & 0x80) != 0) || !onGround_) {
		state_ = S_Move;
	}

	//�~�T�C���U���i�������j
	if ((CheckHitKey(KEY_INPUT_J) || (input.Buttons[1] & 0x80) != 0) && currentNum_ > 0) {
		if (!reloading_) {
			PlaySoundMem(mSound_, DX_PLAYTYPE_BACK);
			// ���ʂ̐ݒ�
			ChangeVolumeSoundMem(255 * 40 / 100, mSound_);
			//ChangeVolumeSoundMem(255 * 20 / 100, mSound_);
			ReadyAttack(isTypeA);//�e�̏���
		}
	}
	else {
		prevAttackKey_ = false;
		isTypeA = false;
	}

	//�~�T�C���U���i�΂ߑO�����j
	if ((CheckHitKey(KEY_INPUT_K) || (input.Buttons[3] & 0x80) != 0) && currentNum_ > 0) {
		if (!reloading_) {
			PlaySoundMem(mSound_, DX_PLAYTYPE_BACK);
			// ���ʂ̐ݒ�
			ChangeVolumeSoundMem(255 * 40 / 100, mSound_);
			//ChangeVolumeSoundMem(255 * 20 / 100, mSound_);
			ReadyAttack(isTypeB);//�e�̏���
		}
	}
	else {
		prevAttackKey_ = false;
		isTypeB = false;
	}

	//�e�̃����[�h
	if (CheckHitKey(KEY_INPUT_L) || (input.Buttons[2] & 0x80) != 0)
	{
		if (currentNum_ != MAX_BULLET && reloading_ != true) {
			reloading_ = true;
		}
	}
}

void Player::UpdateMove()
{
	//���G�̎��͓_��
	if (hitFlag_) {
		if (prevMoveKey_ == 0) {
			animType_ = 6;
		}
		else {
			animType_ = 16;
		}
	}
	else {
		if (prevMoveKey_ == 0) {
			animType_ = 1;
		}
		else {
			animType_ = 11;
		}
	}

	//�ړ��ʂƂ��̏�����
	float moveX, moveY;
	moveX = 0.0f;
	moveY = 0.0f;

	//�R���g���[������(�Ȃ����X�e�B�b�N�����ɓ|���Ă����ɂ����Ȃ�)
	DINPUT_JOYSTATE input;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	//����Ă����l���g����悤�ɏ���������
	moveX += (float)input.X / 1000.0f * (SPEED * Time::DeltaTime());

	//���S�t�߂̌덷���̂���
	if (moveX < 0.3f && moveX > -0.3f) {
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
	
	//�ړ����ĂȂ��Ȃ�
	if (moveX == 0) {
		if (onGround_) {            //�n�ʂɂ���Ȃ�
			state_ = S_Normal;      //�m�[�}���ɖ߂�����
			animFrame_ = 0;
			time_ = 0;
		}
	}

	transform_.position_.x += moveX;//�ړ���

	if (time_ > 0.15f) {
		if (onGround_) {
			animFrame_ = animFrame_ % 5 + 1;
			time_ = 0.0f;
		}
	}

	Stage* pStage = GetParent()->FindGameObject<Stage>();

	//�V�E�E���̃X�e�[�W�Ƃ̓����蔻��
	int pushT, pushB, pushM ,push;
	int hitX = transform_.position_.x + CHIP_SIZE / 15 * 7;
	int hitY = transform_.position_.y + CHIP_SIZE / 4;
	if (pStage != nullptr) {
	    pushT = pStage->CollisionRight(hitX, hitY);//�E���̓����蔻��
		hitY = transform_.position_.y + CHIP_SIZE - 1;
		pushB = pStage->CollisionRight(hitX, hitY);
		hitY = transform_.position_.y + CHIP_SIZE / 2;
		pushM = pStage->CollisionRight(hitX, hitY);
		push = max(pushT, pushB);
		push = max(push, pushM);
		transform_.position_.x -= push;//�̂߂肱��ł镪�߂�
	}
	//�V�E�����̃X�e�[�W�Ƃ̓����蔻��
	hitX = transform_.position_.x + CHIP_SIZE / 7;
	hitY = hitY = transform_.position_.y + CHIP_SIZE / 4;
	if (pStage != nullptr) {
		pushT = pStage->CollisionLeft(hitX, hitY);
		hitY = transform_.position_.y + CHIP_SIZE - 1;
		pushB = pStage->CollisionLeft(hitX, hitY);
		hitY = transform_.position_.y + CHIP_SIZE / 2;
		pushM = pStage->CollisionLeft(hitX, hitY);
		push = max(pushT, pushB);
		push = max(push, pushM);
		transform_.position_.x += push;//�̂߂肱��ł镪�߂�
	}

    //�n�ʂɂ��邩
	if (onGround_) {
		//SPACE�L�[�������ƃW�����v
		if (CheckHitKey(KEY_INPUT_SPACE) || (input.Buttons[0] & 0x80) != 0) {
			//jumpSpeed_ = -6.47;
			jumpSpeed_ = -sqrtf(2 * (gravity_ / (fps_ * 0.5))*JUMP_HEIGHT);
			//jumpSpeed_ = -sqrtf(2 * gravity_ * JUMP_HEIGHT);
			PlaySoundMem(SEJump_, DX_PLAYTYPE_BACK);
			onGround_ = false;//�n�ʂɂ��Ȃ�
			canJump_ = false;
			jTime_ = 0;
			prevY_ = transform_.position_.y;
		}
	}


	//�W�����v�̎��ƍ~��Ă鎞�̃A�j���[�V�����̕ύX
	if (!onGround_ && jumpSpeed_ < 0) {//�n�ʂɂ��Ȃ��ăW�����v�̏�ɍs���r���Ȃ�
		if (prevMoveKey_ == 0) {//�E�����Ȃ�
			animType_ = 2;
			if (time_ > 0.5) {
				animFrame_ = 1;
			}
			else {
				animFrame_ = 0;
			}
		}
		else if (prevMoveKey_ == 1) {//�������Ȃ�
			animType_ = 12;
			if (time_ > 0.5) {
				animFrame_ = 2;
			}
			else {
				animFrame_ = 3;
			}
		}
	}
	else if (!onGround_ && jumpSpeed_ >= 0) {//�n�ʂɂ��Ȃ��āA�W�����v�ŉ������Ă���Ȃ�
		if (prevMoveKey_ == 0) {//�E�����Ȃ�
			animType_ = 2;
			if (time_ > 0.5) {
				animFrame_ = 3;
			}
			else {
				animFrame_ = 2;
			}
		}
		else if (prevMoveKey_ == 1) {//�������Ȃ�
			animType_ = 12;
			if (time_ > 0.5) {
				animFrame_ = 0;
			}
			else {
				animFrame_ = 1;
			}
		}
	}
	/*jTime_ += Time::DeltaTime();
	transform_.position_.y = 0.5 * gravity_ * jTime_ * jTime_ - jumpSpeed_ * jTime_ + prevY_;*/
	jumpSpeed_ += gravity_ * 1.6 * Time::DeltaTime();//���x += �d��
	transform_.position_.y += jumpSpeed_; //���W += ���x

	/*if (jTime_ > 1.0f) {
		canJump_ = true;
	}

	if (jTime_ <= 1.0f && !canJump_) {
		jTime_ += Time::DeltaTime();
	}
	else if(canJump_ && jTime_ > 0) {
		jTime_ -= Time::DeltaTime();
	}
	jumpSpeed_ = jTime_ * JUMP_HEIGHT;
	transform_.position_.y = ground_ - jumpSpeed_;*/

	/*if (!canJump_) {
		transform_.position_.y = ground_ - jumpSpeed_;
	}
	else {
		transform_.position_.y += jumpSpeed_;
	}*/

	//�W�����v�Ń}�b�v�O�ɂłȂ��悤��
	if (transform_.position_.y <= 0) {
		transform_.position_.y = 0;
		jumpSpeed_ = 0;
	}

	//�~�T�C���U��(������)
	if ((CheckHitKey(KEY_INPUT_J) || (input.Buttons[1] & 0x80) != 0) && currentNum_ > 0) {//�U���̃L�[�������̂ƁA�c�e������Ȃ�
		if (!reloading_) {
			PlaySoundMem(mSound_, DX_PLAYTYPE_BACK);
			// ���ʂ̐ݒ�
			ChangeVolumeSoundMem(255 * 40 / 100, mSound_);
			//ChangeVolumeSoundMem(255 * 20 / 100, mSound_);
			ReadyAttack(isTypeA);//�e�̏���
		}
	}
	else {
		prevAttackKey_ = false;
		isTypeA = false;
	}

	//�~�T�C���U��(�΂ߑO����)
	if ((CheckHitKey(KEY_INPUT_K) || (input.Buttons[3] & 0x80) != 0) && currentNum_ > 0) {//�΂ߑO�ɔ�΂�
		if (!reloading_) {
			PlaySoundMem(mSound_, DX_PLAYTYPE_BACK);
			// ���ʂ̐ݒ�
			ChangeVolumeSoundMem(255 * 40 / 100, mSound_);
			//ChangeVolumeSoundMem(255 * 20 / 100, mSound_);
			ReadyAttack(isTypeB);//�e�̏���
		}
	}
	else {
		prevAttackKey_ = false;
		isTypeB = false;
	}

    //�e�̃����[�h
	if (CheckHitKey(KEY_INPUT_L) || (input.Buttons[2] & 0x80) != 0)
	{
		if (currentNum_ != MAX_BULLET && reloading_ != true) {
			reloading_ = true;
		}
	}
}

void Player::UpdateAttack()
{
	//���G�̎��͓_��
	if (hitFlag_) {
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
	if (time_ > 0.05f) {
		if (prevMoveKey_ == 0) {//�E�����Ȃ�
			if (animFrame_ + 1 == 5)
			{
				Attack(0, 1);
			}
		}
		else {//�������Ȃ�
			if (animFrame_ + 1 == 5)
			{
				Attack(1, 3);
			}
		}
	}

	//�A�j���[�V�����̍X�V
	if (time_ > 0.05f) {
		animFrame_ = animFrame_ % 5 + 1;
		time_ = 0.0f;
	}

	//jumpSpeed_ += gravity_ * 1.6 * Time::DeltaTime();//���x += �d��
	//transform_.position_.y += jumpSpeed_; //���W += ���x
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
		//animFrame_ = 1;
		if (prevMoveKey_ == 0) {
			animFrame_ = 1;
		}
		else {
			animFrame_ = 4;
		}
	}
	else {
		if (prevMoveKey_ == 0) {
			//�Ō�̃t���[���ɂȂ�����
			if (animFrame_ + 1 == 6) {
				Die();//�Z��
				animFrame_ = 0;
			}
			if (time_ > 0.2f) {
				animFrame_ = animFrame_ % 5 + 1;
				time_ = 0.0f;
			}
		}
		else {
			//�Ō�̃t���[���ɂȂ�����
			if (animFrame_ == 0) {
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
		x -= cam->GetValueX();
		//y -= cam->GetValueY();
	}
	DrawRectGraph(x, y, animFrame_ * CHIP_SIZE, animType_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE, pImage_, TRUE);
	//�����蔻�������悤
	//DrawCircle(x + CHIP_SIZE / 4, y + CHIP_SIZE / 5 * 3, 20.0f, GetColor(0, 0, 255), FALSE);
	/*if (prevMoveKey_ == 0) {
		DrawCircle(x + CHIP_SIZE / 4, y + CHIP_SIZE / 5 * 3, 20.0f, GetColor(0, 0, 255), FALSE);
	}
	else {
		DrawCircle(x + CHIP_SIZE / 10 * 3, y + CHIP_SIZE / 5 * 3, 20.0f, GetColor(0, 0, 255), FALSE);
	}*/

	//�����̓_
	/*DrawCircle(x + CHIP_SIZE / 15 * 7, y + CHIP_SIZE, 2.0f, GetColor(0, 0, 255), TRUE);
	DrawCircle(x + CHIP_SIZE / 7, y + CHIP_SIZE, 2.0f, GetColor(0, 0, 255), TRUE);
	DrawCircle(x + CHIP_SIZE / 15 * 7, y + CHIP_SIZE / 4,2.0f,GetColor(0,0,255),TRUE);
	DrawCircle(x + CHIP_SIZE / 7, y + CHIP_SIZE / 4, 2.0f, GetColor(0, 0, 255), TRUE);*/
	
	//�c�e���̕\��
	int lenX = 870;//�o�i�[��X�ړ�
	DrawExtendGraph(lenX, -135, lenX + 490, 200, bImage_, TRUE);//�o�i�[

	for (int i = 0; i < currentNum_; i++) {
		DrawGraph((i * MISSILE_SIZE) + lenX + 65, 10, mImage_, TRUE);
	}

	//���C�t�̘g
	int lenF = -120;//x�ړ�
	DrawExtendGraph(lenF, -175, lenF + 510, 245, fImage_, TRUE);

	//���C�t�ƃ��C�t�̉��ɍ����������C�t��`��
	int lenYff = 95;//�n�[�g�t���[��������x�ړ�
	int lenY = 36;   //Y�ړ�
	for (int j = 0; j < 3; j++) {
		//DrawGraph((IMAGE_SIZE * j) + 40, 30, dImage_, TRUE);
		DrawExtendGraph((IMAGE_SIZE * j) + lenYff, lenY, (IMAGE_SIZE * j) + lenYff - 60, lenY + 45, dImage_, TRUE);
	}

	for (int i = 0; i < pLife_; i++) {
		//DrawGraph((IMAGE_SIZE * i) + 40, 30, lImage_, TRUE);
		DrawExtendGraph((IMAGE_SIZE * i) + 100, lenY, (IMAGE_SIZE * i) - 40, lenY + 45, lImage_, TRUE);
	}

	//�V�[���h�̃A�C�R���\���ʒu
	if (getShield_ == true) {
		DrawGraph(10, 100, sImage_, TRUE);
	}

	//�~�T�C���A�C�e���̃A�C�R���\���ʒu
	if (getMissileItem_ == true) {
		DrawRectGraph(10, 180, mAnimFrame_ * IMAGE_SIZE, 0, IMAGE_SIZE, IMAGE_SIZE, iImage_, TRUE);
	}

	//fps�`��
	DrawFormatString(0, 0, GetColor(0, 0, 0), "FPS:%d", fps_);
	DrawFormatString(0, 60, GetColor(0, 0, 0), "Y:%f", transform_.position_.y);
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
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;

	//�C���^�[�o���𒴂������e��[
	/*if (reloadTime_ > INTERVAL) {
		if (currentNum_ != MAX_BULLET) {
			currentNum_++;
		}
		reloadTime_ = 0;
	}
	reloadTime_ += Time::DeltaTime();*/

	//�C���^�[�o���𒴂�����A�S����[
	reloadTime_ += Time::DeltaTime();
	ReloadGauge* RG = GetParent()->FindGameObject<ReloadGauge>();
	if (RG == nullptr) {
		RG = Instantiate<ReloadGauge>(GetParent());
	}
	RG->SetPosition(x, y);
	RG->SetProgress(reloadTime_ / INTERVAL);

	if (reloadTime_ > INTERVAL) {
		currentNum_ = MAX_BULLET;
		PlaySoundMem(rSound_, DX_PLAYTYPE_BACK);
		// ���ʂ̐ݒ�
		ChangeVolumeSoundMem(255 * 50 / 100, rSound_);
		reloading_ = false;
		reloadTime_ = 0.0f;
		RG->KillMe();
	}
}

void Player::Icon()
{
	//��莞�Ԍo�߂œ_�ł��ď�����
	itemTime_ += Time::DeltaTime();
	float tmp = 7.0f;
	if (itemTime_ >= tmp) {
		//tmp = disTime_ - 1;
		iTime_ += Time::DeltaTime();
		if (iTime_ >= 0.2) {
			iTime_ = 0;
			mAnimFrame_ = mAnimFrame_ % 3 + 1;
		}
	}
	if (itemTime_ >=  FIN_ITEM_TIME) {
		itemTime_ = 0.0f;
		mAnimFrame_ = 0;
		getMissileItem_ = false;
	}
}

void Player::ReadyAttack(bool &_isType)
{
	if (prevAttackKey_ == false) {
		if (prevMoveKey_ == 0) {
			animFrame_ = 0;
			animType_ = 3;
		}
		else {
			animFrame_ = 0;
			animType_ = 13;
		}
		time_ = 0.0f;
		_isType = true;
		state_ = S_Attack;//�U���̏�ԂɈڂ�
	}
	prevAttackKey_ = true;
}

void Player::Attack(int angleA, int angleB)
{
	AttackSkill* attack = Instantiate<AttackSkill>(GetParent());
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;

	//���Ɍ����A�΂߂Ɍ���
	if (isTypeA == true) {//���Ɍ���
		attack->SetAngle(XM_PI*angleA);//�p�x�̐ݒ�
		if (prevMoveKey_ == 0) {
			attack->SetPosition(x + CHIP_SIZE, y);
		}
		else {
			attack->SetPosition(x, y);
		}
	}
	if (isTypeB == true) {//�΂ߏ�Ɍ���
		attack->SetAngle(XM_PI / -4 * angleB);//�p�x��ݒ�
		if (prevMoveKey_ == 0) {
			attack->SetPosition(x + CHIP_SIZE / 5 * 2, y);
		}
		else {
			attack->SetPosition(x + CHIP_SIZE / 5, y);
		}
	}

	//�~�T�C���A�C�e�����Ƃ�����e������
	if (getMissileItem_ == false) {
		currentNum_--;
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
