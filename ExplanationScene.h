#pragma once
#include "Engine/GameObject.h"
class ExplanationScene :
    public GameObject
{
public:
	//�R���X�g���N�^
	ExplanationScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
private:
	int ePict_;
	int select_;
	bool prevUpKey_;
	bool prevDownKey_;
	bool prevEnterKey_;
};

