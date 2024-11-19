#pragma once
#include "Engine/GameObject.h"
class Shield :
	public GameObject
{
public:
	//�R���X�g���N�^
	Shield(GameObject* parent);

	//�f�X�g���N�^
	~Shield();

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


private:
	int sImage_;
	int aImage_;
	float disTime_; //������܂ł̎���
	int animType_;
	int animFrame_;
	float time_;

};

