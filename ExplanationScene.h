#pragma once
#include "Engine/GameObject.h"
class ExplanationScene :
    public GameObject
{
public:
	//�R���X�g���N�^
	ExplanationScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
private:
	int ePict_;
	int select_;
	bool prevUpKey_;
	bool prevDownKey_;
	bool prevEnterKey_;
	int sHandle_; //�I�����n���h��
	int eHandle_; //���艹�n���h��
	int rHandle_; //�߂艹�n���h��
	bool SoundFlag_; //�T�E���h���Ȃ��Ă��邩
};

