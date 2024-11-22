#include "FlyEnemy.h"
#include "Engine/time.h"
#include "Camera.h"
#include "Stage.h"
#include "PlayScene.h"
#include "Explosion.h"
#include "AttackSkill.h"

namespace {
	const float CHIP_SIZE = 96.0f;//�L�����̉摜�T�C�Y
	const float PLAYER_CHIP_SIZE = 64.0f;//Player�̉摜�T�C�Y
	static const int SCREEN_WIDTH = 1280;
	const XMFLOAT3 INIT_POS = { 0,0,0 };//�ŏ��̈ʒu
	const float MAX_POS = 400;
	const int FLY_SPEED = 175;//��ԃX�s�[�h
	const int FLY_HEIGHT =300;//��ԍ���
	const float MOVE_INTERVAL = 2.0f;//�㉺����ւ�����
	const int X_MARGIN = 24;//X�̗]��
	const int Y_MARGIN = 32;//Y�̗]��
	const float SEARCH_RANGE = 300.0f;//���G�͈�
}

FlyEnemy::FlyEnemy(GameObject* parent) 
	:GameObject(parent,"FlyEnemy"), eImage_(-1),moveTime_(0), 
	moveDirection_(-1),time_(0.0f), onGround_(false),
	animType_(0),animFrame_(0),chasingPlayer_(false),
	chaseDelayTime_(0.0f),lastPlayerX_(0.0f),lastPlayerY_(0.0),
	underbrock_(false),prevPosX_(0.0f),prevPosY_(0.0f)
{
	//�����ʒu�̒���
	transform_.position_ = INIT_POS;
	state_ = S_Normal;
}

FlyEnemy::~FlyEnemy()
{
	if (eImage_ > 0) {
		DeleteGraph(eImage_);
		eImage_ = -1;
	}
}

void FlyEnemy::Initialize()
{
	eImage_ = LoadGraph("Assets\\Image\\FlyEnemy3.png");
	assert(eImage_ >= 0);
	transform_.position_ = INIT_POS;
}

void FlyEnemy::Update()
{

	//�X�N���[���ɍ��킹�ē����悤��
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		//y -= cam->GetValueY();
	}

	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->canMove())//canMove�Ȃ瓮����
		return;

	if (x > SCREEN_WIDTH)//��ʊO(�E��)�ɂ���Ȃ�Ȃɂ����Ȃ�
		return;

	switch (state_) {
	case 0:
		UpdateNormal();
		break;
	case 1:
		UpdateAttack();
		break;
	default:
		break;
	}

	//�X�e�[�W�Ƃ̏㉺�̓����蔻��
	Stage* pStage = GetParent()->FindGameObject<Stage>();
	if (pStage != nullptr) {
		int pushR = pStage->CollisionDown(transform_.position_.x + CHIP_SIZE / 2 - 10, transform_.position_.y + CHIP_SIZE);
		int pushL = pStage->CollisionDown(transform_.position_.x + 14, transform_.position_.y + CHIP_SIZE);
		int push = max(pushR, pushL);//�Q�̑����̂߂荞�݂̑傫����
		if (push >= 1) {
			transform_.position_.y -= push - 1;
			onGround_ = true;
		}

		pushR = pStage->CollisionUp(transform_.position_.x + CHIP_SIZE / 2 - 10, transform_.position_.y + CHIP_SIZE /2);
		pushL = pStage->CollisionUp(transform_.position_.x + 14, transform_.position_.y + CHIP_SIZE / 2);
		push = max(pushR, pushL);//�Q�̑����̂߂荞�݂̑傫����
		if (push >= 1) {
			transform_.position_.y += push + 1;
			underbrock_ = true;
		}
		else if(push == 0) {
			underbrock_ = false;
		}
	}

	//Stage* pStage = GetParent()->FindGameObject<Stage>();
	//�V�E�E���̃X�e�[�W�Ƃ̓����蔻��
	int pushT, pushB, pushM, push;
	int hitX = transform_.position_.x + CHIP_SIZE / 2 - 10;
	int hitY = transform_.position_.y + CHIP_SIZE / 2 + 6;
	if (pStage != nullptr) {
		pushT = pStage->CollisionRight(hitX, hitY);//�E���̓����蔻��
		hitY = transform_.position_.y + CHIP_SIZE - 1;
		pushB = pStage->CollisionRight(hitX, hitY);
		hitY = transform_.position_.y + CHIP_SIZE / 4 * 3;
		pushM = pStage->CollisionRight(hitX, hitY);
		push = max(pushT, pushB);
		push = max(push, pushM);
		transform_.position_.x -= push;//�̂߂肱��ł镪�߂�
	}
	//�V�E�����̃X�e�[�W�Ƃ̓����蔻��
	hitX = transform_.position_.x + 14;
	hitY = transform_.position_.y + CHIP_SIZE / 2 + 6;
	if (pStage != nullptr) {
		pushT = pStage->CollisionLeft(hitX, hitY);
		hitY = transform_.position_.y + CHIP_SIZE - 1;
		pushB = pStage->CollisionLeft(hitX, hitY);
		hitY = transform_.position_.y + CHIP_SIZE / 4 * 3;
		pushM = pStage->CollisionLeft(hitX, hitY);
		push = max(pushT, pushB);
		push = max(push, pushM);
		transform_.position_.x += push;//�̂߂肱��ł镪�߂�
	}

	//�O�t���[���̈ʒu�������Ă���
	prevPosX_ = transform_.position_.x;
	prevPosY_ = transform_.position_.y;
}

void FlyEnemy::UpdateNormal()
{

	animType_ = 0;//��s���[�V����
	Stage* pStage = GetParent()->FindGameObject<Stage>();

	Player* player = GetParent()->FindGameObject<Player>();

	//�㉺����ւ����Ԃ̌v���Ɠ���ւ��̔���i�n�ʂɓ���������������ւ���j
	moveTime_ += Time::DeltaTime();
	if (moveTime_ >= MOVE_INTERVAL || transform_.position_.y < 0) {
		moveTime_ = 0.0f;
		moveDirection_ *= -1;
	}

	//�n�ʂɓ���������㉺����ւ�
	if (onGround_==true || underbrock_ == true) {

		moveTime_ = 0.0f;
		moveDirection_ *= -1;
		onGround_ = false;
	}

	//Player�����E���ɓ�������S_Attack��
	if (InRange(player)) {
		state_ = S_Attack;
	}

	//�ړ��ʂƂ��̏�����
	float moveX, moveY;
	moveX = 0.0f;
	moveY = 0.0f;

	//moveDirection_�������ď㉺����ւ�
	//moveX = FLY_SPEED * Time::DeltaTime();//�ړ���
	moveY = FLY_SPEED * Time::DeltaTime()*moveDirection_;//�ړ���

	//transform_.position_.x -= moveX;
	transform_.position_.y -= moveY;

	if (transform_.position_.x < 0 - CHIP_SIZE) {//����ʒ[�ŏ�����
		KillMe();
	}

	//�A�j���[�V����
	time_ += Time::DeltaTime();
	if (time_ > 0.1f) {
		animFrame_ = animFrame_ % 5 + 1;
		time_ = 0.0f;
	}
}

void FlyEnemy::UpdateAttack()
{
	Player* player = GetParent()->FindGameObject<Player>();

	//if (player == nullptr) return;

	//// Player�̈ʒu�X�V�Ԋu
	//chaseDelayTime_ += Time::DeltaTime();
	//if (chaseDelayTime_ >= 0.5f) {  // 0.5�b���ƂɈʒu���X�V
	//	lastPlayerX_ = player->GetPosition().x + (float)PLAYER_CHIP_SIZE / 4;
	//	lastPlayerY_ = player->GetPosition().y + (float)PLAYER_CHIP_SIZE / 2;
	//	chaseDelayTime_ = 0.0f;  // �X�V������J�E���g�����Z�b�g
	//}

	////�����̒��S�ʒu
	//float myCenterX = transform_.position_.x + (float)CHIP_SIZE / 2 - X_MARGIN;
	//float myCenterY = transform_.position_.y + (float)CHIP_SIZE / 2 + Y_MARGIN;

	////Player�����E���ɂ�����ǔ�
	//if (InRange(player)) {
	//	float moveX = 0.0f;
	//	float moveY = 0.0f;

	//	if (underbrock_ == true && transform_.position_.y == prevPosY_) {
	//		if (animType_ == 1) {
	//			moveX = FLY_SPEED * Time::DeltaTime();
	//			//animType_ = 1;
	//		}
	//		else {
	//			moveX = -FLY_SPEED * Time::DeltaTime();
	//			//animType_ = 0;
	//		}
	//	}
	//	else {
	//		//Player�������ꏊ�̒��S�Ɍ������Ĉړ�
	//		if (lastPlayerX_ > myCenterX) {
	//			moveX = FLY_SPEED * Time::DeltaTime();
	//			animType_ = 1;
	//		}
	//		else {
	//			moveX = -FLY_SPEED * Time::DeltaTime();
	//			animType_ = 0;
	//		}
	//		if (lastPlayerY_ > myCenterY) {
	//			moveY = FLY_SPEED * Time::DeltaTime();
	//		}
	//		else {
	//			moveY = -FLY_SPEED * Time::DeltaTime();
	//		}
	//	}

	//	transform_.position_.x += moveX;
	//	transform_.position_.y += moveY;
	//}
	//else {
	//	//���E�O�ɏo����S_Normal��
	//	state_ = S_Normal;
	//}

	if (player == nullptr) return;
	chaseDelayTime_ += Time::DeltaTime();
	if (chaseDelayTime_ >= 0.5f) {  // 0.5�b���ƂɈʒu���X�V
		lastPlayerX_ = player->GetPosition().x + (float)PLAYER_CHIP_SIZE / 4;
		lastPlayerY_ = player->GetPosition().y + (float)PLAYER_CHIP_SIZE / 2;
		chaseDelayTime_ = 0.0f;  //�X�V������J�E���g�����Z�b�g
	}

	float myCenterX = transform_.position_.x + (float)CHIP_SIZE / 2 - X_MARGIN;
	float myCenterY = transform_.position_.y + (float)CHIP_SIZE / 2 + Y_MARGIN;

	if (InRange(player)) {
		float moveX = 0.0f;
		float moveY = 0.0f;

		if (lastPlayerX_ > myCenterX) {
			moveX = FLY_SPEED * Time::DeltaTime();
			animType_ = 1;
		}
		else {
			moveX = -FLY_SPEED * Time::DeltaTime();
			animType_ = 0;
		}
		if (lastPlayerY_ > myCenterY) {
			moveY = FLY_SPEED * Time::DeltaTime();
		}
		else {
			moveY = -FLY_SPEED * Time::DeltaTime();
		}

		transform_.position_.x += moveX;
		transform_.position_.y += moveY;
		if (transform_.position_.y < 0) {
			transform_.position_.y = 0;
		}
	}
	else {
		//���E�O�ɏo����S_Normal��
		state_ = S_Normal;
	}

	time_ += Time::DeltaTime();
	//�A�j���[�V����
	if (time_ > 0.1f) {
		animFrame_ = animFrame_ % 5 + 1;
		time_ = 0.0f;
	}
}

void FlyEnemy::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		//y -= cam->GetValueY();
	}
	DrawRectGraph(x, y, animFrame_ * CHIP_SIZE, animType_ * CHIP_SIZE, CHIP_SIZE, CHIP_SIZE, eImage_, TRUE);
	
	//�����蔻�茩��悤
	//DrawCircle( x + CHIP_SIZE/2-X_MARGIN, y + CHIP_SIZE/2+Y_MARGIN, 16.0f, GetColor(0, 0, 255), FALSE);
	
	//�n�ʂƂ̓����蔻�茩��悤
	//DrawBox(x + 14, y + CHIP_SIZE / 2 + 6, x + CHIP_SIZE / 2 - 10, y + CHIP_SIZE, GetColor(0, 0, 0), FALSE);
}

void FlyEnemy::SetPosition(float _x, float _y)
{
	transform_.position_.x = _x;
	transform_.position_.y = _y;
}

bool FlyEnemy::CollideCircle(float x, float y, float r)
{
	//x,y,r������̉~�̏��
	//�����̉~�̏��
	float myCenterX = transform_.position_.x + (float)CHIP_SIZE / 2-X_MARGIN;
	float myCenterY = transform_.position_.y + (float)CHIP_SIZE/2+Y_MARGIN;
	float myR = 16.0f;
	float dx = myCenterX - x;
	float dy = myCenterY - y;
	if ((dx * dx + dy * dy) < ((r + myR) * (r + myR))) {
		return true;
	}
	else {
		return false;
	}
}

//���E���ɂ��邩
bool FlyEnemy::InRange(Player* player)
{
	//�����̒��S�ʒu
	float myCenterX = transform_.position_.x + (float)CHIP_SIZE / 2 - X_MARGIN;
	float myCenterY = transform_.position_.y + (float)CHIP_SIZE / 2 + Y_MARGIN;

	//Player�̒��S�ʒu
	float playerCenterX = player->GetPosition().x + (float)PLAYER_CHIP_SIZE / 4;
	float playerCenterY = player->GetPosition().y + (float)PLAYER_CHIP_SIZE / 2;

	//�v���C���[�Ƃ̋���
	float dx = myCenterX - playerCenterX;
	float dy = myCenterY - playerCenterY;
	float distance = sqrt(dx * dx + dy * dy);

	//���E���ɂ��邩�Ԃ�
	return(distance < SEARCH_RANGE);
}
