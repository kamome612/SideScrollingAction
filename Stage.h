#pragma once
#include "Engine/GameObject.h"

//�X�e�[�W���Ǘ�����N���X
namespace {
	const int MAP_NUM = 3;
	const int MAP_SIZE = 32;
}

class Stage :
    public GameObject
{
public:
	//�R���X�g���N�^
	Stage(GameObject* parent);

    //�f�X�g���N�^
	~Stage();

	//������
	void Initialize() override;

    //�X�V
	void Update() override;
    
	//�`��
	void Draw()override;

	//�}�b�v���Z�b�g����
	void StageSet();

	//�}�b�v�̏�Ԃ����Z�b�g����
	void StageReset();

	void ChangeStage() { mapNo_++; }

	/// <summary>
	/// �E���̓_���������Ă��邩���ׂ�
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <returns>�߂肱�񂾗�(�h�b�g)</returns>
	int CollisionRight(int x, int y);

	/// <summary>
	/// �����̓_���������Ă��邩���ׂ�
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <returns>�߂荞�񂾗�(�h�b�g)</returns>
	int CollisionLeft(int x, int y);

	/// <summary>
	/// ���̓_���������Ă��邩���ׂ�
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <returns>�߂荞�񂾗�(�h�b�g)</returns>
	int CollisionDown(int x, int y);

	/// <summary>
	/// ��̓_���������Ă��邩���ׂ�
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <returns></returns>
	int CollisionUp(int x, int y);

private:
	int hImage_;
	int* map_;
	int width_;
	int height_;
	int mapNo_;
	bool IsWallBlock(int x, int y);
	bool prevResetKey_;
};

