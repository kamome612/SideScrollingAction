#pragma once
#include "Engine/GameObject.h"
class MeteoSpawner :
    public GameObject
{
public:
	//�R���X�g���N�^
	MeteoSpawner(GameObject* parent);

	//�f�X�g���N�^
	~MeteoSpawner();

	//�X�V
	void Update() override;

	//�`��(�g�����͔���)
	void Draw() override;

	//�ʒu�̃Z�b�^�[
	void SetPosition(float _x, float _y);
private:
	float timer_;
	float interval = 5.0f;
};

