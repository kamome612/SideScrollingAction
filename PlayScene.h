#pragma once
#include "Engine/GameObject.h"
class PlayScene :
    public GameObject
{
	//�w�i(�X�e�[�W�̂ق��ŕ\�����邱�Ƃɂ��邩��)
	int pPict_;
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
};

