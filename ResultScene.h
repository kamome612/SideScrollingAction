#pragma once
#include "Engine/GameObject.h"

//�N���A��Q�[���I�[�o�Ȃǂ̃��U���g�V�[�����Ǘ�����N���X
class ResultScene :
	public GameObject
{
public:
	//�R���X�g���N�^
	// ����:parent �e�I�u�W�F�N�g(SceneManager)
	ResultScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//�N���A�̎���isClear_��true�ɂ���
	void ChangeResult() { isClear_ = true; }

private:
	bool isClear_; //�N���A���ۂ�
	int rPict_;
	int select_;   //�I��
	bool prevUpKey_;   //�O�t���[���̏�L�[���
	bool prevDownKey_; //�O�t���[���̉��L�[���
};

