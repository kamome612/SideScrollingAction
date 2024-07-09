#pragma once
#include "Engine/GameObject.h"
class PlayScene :
    public GameObject
{
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	PlayScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	bool canMove() { return state_ == S_Play; }

private:
	int pPict_; //�X�e�[�W�I�����̉摜
	enum State {
		S_Select = 0,
		S_Ready,   //�n�܂�܂ő҂�(Ready�Ȃǂ�\���\��)
		S_Play,    //�v���C��
		S_Clear,   //�Q�[���N���A(�{�^���������܂ő҂\��)
		S_GameOver,//�Q�[���I�[�o�[(�{�^���������܂ő҂\��)
	};

	State state_;
	float timer_;
	bool prevResetKey_;
	bool prevChangeKey_;

	void UpdateSelect();
	void UpdateReady();
	void UpdatePlay();
	void UpdateClear();
	void UpdateGameOver();
public:
	void StartSelect();
	void StartReady();
	void StartPlay();
	void StartGameOver();
};

