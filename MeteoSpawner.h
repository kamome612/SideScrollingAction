#pragma once
#include "Engine/GameObject.h"
class MeteoSpawner :
    public GameObject
{
public:
	//�R���X�g���N�^
	MeteoSpawner(GameObject* parent);

	MeteoSpawner(GameObject* parent, int _type);

	//�f�X�g���N�^
	~MeteoSpawner();

	//�X�V
	void Update() override;

	//�`��(�g�����͔���)
	void Draw() override;

	//�ʒu�̃Z�b�^�[
	void SetPosition(float _x, float _y);

	//�^�C�v�w��
	void SetType(int _type) { type_ = _type; }
private:
	float timer_;
	float interval = 5.0f;
	int type_;
};

