#pragma once
#include "Engine/GameObject.h"
#include "Player.h"
#include "Meteorite.h"

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

	//�X�e�[�W�ύX�ȂǂɎg��
	void ChangeStage(int _mapNo) { mapNo_ = _mapNo; }

	//�X�e�[�W�̎�ނ̃Q�b�^�[
	int GetMapNo() { return mapNo_; }

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

	//覐΂������������̒n�ʂȂǂ̔j��
	void BreakGround(int x, int y);

private:
	int hImage_;        //�X�e�[�W�̑f��
	int gPict_;         //�w�i�̉摜
	int* map_;          //�X�e�[�W�̏�������
	int width_;         //�X�e�[�W�̉�
	int height_;        //�X�e�[�W�̏c
	int mapNo_;         //�ǂ̃X�e�[�W���̎���
	bool IsWallBlock(int x, int y);
	float timer_;
};

