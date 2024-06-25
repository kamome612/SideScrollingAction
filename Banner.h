#pragma once
#include "Engine/GameObject.h"

class Banner :public GameObject
{
public:
	Banner(GameObject* scene);
	~Banner();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	enum ViewID {
		V_Start = 0,
		V_Clear,
		V_GameOver,
		V_Nothing,
	};
	void View(ViewID id);
private:
	int bImage_;
	ViewID view_;
	//�Q�[���I�[�o�[�\���p
	float transparency_;//�����x
	float gameOverY;//�Q�[���I�[�o�[�̎���Y���W
	float gameOverTime_;
};
