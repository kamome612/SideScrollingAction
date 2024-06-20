#pragma once
#include "Engine/GameObject.h"
class PlayScene :
    public GameObject
{
private:
	//�w�i(�X�e�[�W�̂ق��ŕ\�����邱�Ƃɂ��邩��)
	int pPict_;
	enum State {
		//S_Ready=0, //�n�܂�܂ő҂�(Ready�Ȃǂ�\���\��)
		S_Play,    //�v���C��
		S_Clear,   //�Q�[���N���A(�{�^���������܂ő҂\��)
		S_Dead,    //�Q�[���I�[�o�[(�{�^���������܂ő҂\��)
	};

	State state_;
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
};

