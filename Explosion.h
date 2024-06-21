#pragma once
#include "Engine/GameObject.h"
class Explosion :
    public GameObject
{
private:
	int eImage_;
	int animFrame_;
	int animColumn_;
	int animLine_;
	float exTime_;
public:
	//�R���X�g���N�^
	Explosion(GameObject* parent);

	//�f�X�g���N�^
	~Explosion();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw()override;
	
	//�ʒu���Z�b�g
	void SetPosition(float _x, float _y);
};

