#pragma once
#include "Engine/GameObject.h"

//�X�e�[�W���Ǘ�����N���X
namespace {
	const int MAP_SIZE = 32;
	const int MAP_WIDTH = 10;
	const int MAP_HEIGHT = 8;
}

class Stage :
    public GameObject
{
	int MapData[MAP_HEIGHT][MAP_WIDTH] =
	{
		{ 18, 19, 255, 255, 18, 19, 255, 255, 18, 19 } ,
		{ 34, 35, 255, 255, 34, 35, 255, 255, 34, 35 } ,
		{ 255, 255, 18, 19, 255, 255, 18, 19,255, 255 } ,
		{ 255, 255, 34, 35, 255, 255,34, 35, 255, 255 } ,
		{ 18, 19, 255, 255, 18, 19, 255,255, 18, 19 } ,
		{ 34, 35, 255, 255,34, 35, 255, 255, 34, 35 } ,
		{ 255, 255, 18, 19, 255, 255, 18, 19, 255, 255 } ,
		{ 255, 255, 34, 35,255, 255, 34, 35, 255, 255 }
	};

	int hImage_;
public:
	//�R���X�g���N�^
	Stage();

    //�f�X�g���N�^
	~Stage();

	//������
	void Initialize() override;

    //�X�V
	void Update() override;
    
	//�`��
	void Draw()override;

};

