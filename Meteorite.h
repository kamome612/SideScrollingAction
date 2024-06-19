#pragma once
#include "Engine/GameObject.h"
class Meteorite :
    public GameObject
{
private:
	int mImage_;
public:
	//�R���X�g���N�^
	Meteorite(GameObject* parent);

	//�f�X�g���N�^
	~Meteorite();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw()override;

};

