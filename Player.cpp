#include "Player.h"
#include "Engine/time.h"
#include "Camera.h"
#include "Stage.h"
#include "Meteorite.h"
#include "PlayScene.h"
#include "Explosion.h"
#include "AttackSkill.h"

namespace {
	const XMFLOAT3 INIT_POS = { 30,575,0 };//�ŏ��̈ʒu
	const float JUMP_HEIGHT = 64.0f * 4.0f;//�W�����v�̍���
	const float INIT_GRAVITY = 9.8/ 60.0f;
	//�d�̓���:��...1.62,�ΐ�...3.71,���z274

}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),pImage_(-1),walkSpeed_(150),gravity_(INIT_GRAVITY),
	                             jumpSpeed_(0.0f), onGround_(true), prevSpaceKey_(false),
	                             time_(0.0f),animType_(0),animFrame_(0)
{
	//�����ʒu�̒���
	transform_.position_ = INIT_POS;
	state_ = S_Normal;
}

Player::~Player()
{
	if (pImage_ > 0) {
		DeleteGraph(pImage_);
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

	Stage* pStage = GetParent()->FindGameObject<Stage>();
	if (pStage != nullptr) {
		//(50,64)��(14,64)������
		int pushR = pStage->CollisionDown(transform_.position_.x + 50, transform_.position_.y + 64);
		int pushL = pStage->CollisionDown(transform_.position_.x + 14, transform_.position_.y + 64);
		int push = max(pushR, pushL);//�Q�̑����̂߂荞�݂̑傫����
		if (push >= 1) {
			transform_.position_.y -= push - 1;
			jumpSpeed_ = 0.0f;
			onGround_ = true;
		}
		else {
			onGround_ = false;
		}

		//(50,64)��(14,64)������
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
		if (pMeteo->CollideCircle(transform_.position_.x + 32.0f, transform_.position_.y + 32.0f, 20.0f)) {
			//scene->StartDead();
			//�����ɔ����̃G�t�F�N�g����ꂽ�炢����...
			pMeteo->KillMe();
			Explosion* pEx = Instantiate<Explosion>(GetParent());
			pEx->SetPosition(transform_.position_.x, transform_.position_.y - 32.0f);
		}
	}

	//�����ŃJ�����ʒu�̒���
	Camera* cam = GetParent()->FindGameObject<Camera>();
	int x = (int)transform_.position_.x - cam->GetValue();
	if (x > 400) {
		x = 400;
		cam->SetValue((int)transform_.position_.x - x);
	}
	if (x < 0) {
		x = 0;
		cam->SetValue((int)transform_.position_.x + x);
	}
}

void Player::UpdateNormal()
{
	Stage* pStage = GetParent()->FindGameObject<Stage>();
	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->canMove())
		return;

	if (CheckHitKey(KEY_INPUT_D)) {//D�L�[�������ƉE�ɐi��
		transform_.position_.x += walkSpeed_ * Time::DeltaTime();
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
			transform_.position_.x -= walkSpeed_ * Time::DeltaTime();
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

	if (CheckHitKey(KEY_INPUT_SPACE)) {//SPASE�L�[�������ƃW�����v
		if (prevSpaceKey_ == false) {//�O��̃t���[����space�������ĂȂ��Ƃ�����
			if (onGround_) {
				jumpSpeed_ = -sqrtf(2 * (gravity_)*JUMP_HEIGHT);
				onGround_ = false;
			}
		}
		prevSpaceKey_ = true;
	}
	else {
		prevSpaceKey_ = false;
	}
	jumpSpeed_ += gravity_;//���x += �d��
	transform_.position_.y += jumpSpeed_; //���W += ���x

	if (CheckHitKey(KEY_INPUT_E)) {
		time_ = 0.0f;
		state_ = S_Attack;
	}

	//if (pStage != nullptr) {
	//	//(50,64)��(14,64)������
	//	int pushR = pStage->CollisionDown(transform_.position_.x + 50, transform_.position_.y + 64);
	//	int pushL = pStage->CollisionDown(transform_.position_.x + 14, transform_.position_.y + 64);
	//	int push = max(pushR, pushL);//�Q�̑����̂߂荞�݂̑傫����
	//	if (push >= 1) {
	//		transform_.position_.y -= push - 1;
	//		jumpSpeed_ = 0.0f;
	//		onGround_ = true;
	//	}
	//	else {
	//		onGround_ = false;
	//	}

	//	//(50,64)��(14,64)������
	//	pushR = pStage->CollisionUp(transform_.position_.x + 50, transform_.position_.y);
	//	pushL = pStage->CollisionUp(transform_.position_.x + 14, transform_.position_.y);
	//	push = max(pushR, pushL);//�Q�̑����̂߂荞�݂̑傫����
	//	if (push >= 1) {
	//		transform_.position_.y += push + 1;
	//		jumpSpeed_ = 0.0f;
	//	}
	//}

	//std::list<Meteorite*> pMeteos = GetParent()->FindGameObjects<Meteorite>();
	//for (Meteorite* pMeteo : pMeteos) {
	//	if (pMeteo->CollideCircle(transform_.position_.x + 32.0f, transform_.position_.y + 32.0f, 20.0f)) {
	//		//scene->StartDead();
	//		//�����ɔ����̃G�t�F�N�g����ꂽ�炢����...
	//		pMeteo->KillMe();
	//		Explosion* pEx = Instantiate<Explosion>(GetParent());
	//		pEx->SetPosition(transform_.position_.x, transform_.position_.y - 32.0f);
	//	}
	//}

	////�����ŃJ�����ʒu�̒���
	//Camera* cam = GetParent()->FindGameObject<Camera>();
	//int x = (int)transform_.position_.x - cam->GetValue();
	//if (x > 400) {
	//	x = 400;
	//	cam->SetValue((int)transform_.position_.x - x);
	//}
	//if (x < 0) {
	//	x = 0;
	//	cam->SetValue((int)transform_.position_.x + x);
	//}

	//�A�j���[�V�����Ɏg���^�C���̍X�V
	time_ += Time::DeltaTime();
}

void Player::UpdateAttack()
{
	animType_ = 1;
	if (animFrame_ + 1 == 3) {
		animType_ = 0;
		state_ = S_Normal;
	}
	if (time_ > 0.3f) {
		animFrame_ = (animFrame_ + 1) % 3;
		time_ = 0.0f;
	}
	if (animFrame_ == 2) {
		AttackSkill* attack = Instantiate<AttackSkill>(GetParent());
		attack->SetPosition(transform_.position_.x + 64.0f, transform_.position_.y + 64.0f/2.0f);
	}
	time_ += Time::DeltaTime();
}

void Player::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValue();
	}
	DrawRectGraph(x, y, animFrame_ * 64, animType_ * 64, 64, 64, pImage_, TRUE);
	DrawCircle(x + 32.0f, y + 32.0f, 20.0f,GetColor(0,0,255), FALSE);
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