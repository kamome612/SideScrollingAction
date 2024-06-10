#include "Player.h"
#include "Engine/time.h"

namespace {
	const XMFLOAT3 INITPOS = { 30,550,0 };//�ŏ��̈ʒu
	const float JUMP_HEIGHT = 64.0f * 4.0f;//�W�����v�̍���
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),pImage_(-1),walkSpeed_(133),gravity_(9.8 / 60.0f),
	                             jumpSpeed_(0.0f), onGround_(true), prevSpaceKey_(false)
{
	//�����ʒu�̒���
	transform_.position_ = INITPOS;
	state_ = S_Normal;
}

Player::Player(GameObject* parent, float _gravity)
	:GameObject(parent, "Player"), pImage_(-1), walkSpeed_(133), jumpSpeed_(0.0f),
	                               onGround_(true),prevSpaceKey_(false)
{
	// �����ʒu�̒���
    transform_.position_ = INITPOS;
	state_ = S_Normal;

	//�X�e�[�W����󂯎�����d�͂̃Z�b�g(��Ŏ󂯓n�����͕ς��邩��)
	gravity_ = _gravity;
}

Player::~Player()
{
	if (pImage_ > 0)
		DeleteGraph(pImage_);
}

void Player::Initialize()
{
	//�Ƃ肠��������aoi���o���Ƃ�(��ŕς���)
	pImage_ = LoadGraph("Assets\\Image\\chara.png");
	assert(pImage_ >= 0);
}

void Player::Update()
{
	switch (state_){
	case 0: 
		UpdateNormal();
		break;
	case 1:
		break;
	default:
		break;
	}
}

void Player::UpdateNormal()
{
	if (CheckHitKey(KEY_INPUT_D)) {//S�L�[�������ƉE�ɐi��
		transform_.position_.x += walkSpeed_ * Time::DeltaTime();
	}
	if (CheckHitKey(KEY_INPUT_A)) {//A�L�[�������ƍ��ɐi��
		transform_.position_.x -= walkSpeed_ * Time::DeltaTime();
	}

	if (CheckHitKey(KEY_INPUT_SPACE)) {//SPASE�L�[�������ƃW�����v
		if (prevSpaceKey_ == false) {
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
	jumpSpeed_ += gravity_;//���x += �����x
	transform_.position_.y += jumpSpeed_; //���W += ���x

	if (transform_.position_.y > INITPOS.y) {
		transform_.position_.y = INITPOS.y;
		onGround_ = true;
	}
}

void Player::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	DrawGraph(x, y, pImage_, TRUE);
}