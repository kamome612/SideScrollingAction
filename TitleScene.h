#pragma once
#include "Engine/GameObject.h"
class TitleScene :
    public GameObject
{
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	TitleScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

private:
	int tPict_;
	int select_;        //�I��
	bool prevUpKey_;    //�O�t���[���̏�L�[���
	bool prevDownKey_;  //�O�t���[���̉��L�[���
	bool prevEnterKey_; //�O�t���[���̌���L�[���
	int sHandle_; //�I�����n���h��
	int eHandle_; //���艹�n���h��
	int rHandle_; //�߂艹�n���h��
	bool SoundFlag_;//�T�E���h���Ȃ��Ă��邩
};

