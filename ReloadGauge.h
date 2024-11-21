#pragma once
#include "Engine/GameObject.h"
class ReloadGauge
	:public GameObject
{
public:
	//�R���X�g���N�^
	ReloadGauge(GameObject* parent);

	//�f�X�g���N�^
	~ReloadGauge();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw()override;

	//�ʒu�̃Z�b�^�[
	void SetPosition(float _x, float _y);

	void SetProgress(float progress);
private:
	int hImage_;
	int hImageF_;//�t���[��

	float progress_;//��
	float maxW;//�Q�[�W�̉���

	

};



