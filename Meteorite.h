#pragma once
#include "Engine/GameObject.h"
class Meteorite :
    public GameObject
{
public:
	//�R���X�g���N�^
	Meteorite(GameObject* parent);

	//�f�X�g���N�^
	~Meteorite();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw()override;

	//�ʒu�̃Z�b�^�[
	void SetPosition(float _x, float _y);

	//�~�̓����蔻�������
	bool CollideCircle(float x, float y, float r);

	//�d�͂��Z�b�g
	void SetGravity(float _g) { gravity_ = _g; }

	//覐΂̗�����^�C�v���Z�b�g����
	void SetMoveType(int _type) { moveType_ = _type; }
private:
	int mImage_;
	float gravity_;
	float moveSpeed_;
	int moveType_;
};

