#include "Banner.h"
#include "Ease.h"

Banner::Banner(GameObject* scene)
	:GameObject(scene,"Banner"),bImage_(-1),gameOverTime_(0.0f),gameOverY(200),
	                            transparency_(0.0f),view_(V_Start)
{
}

Banner::~Banner()
{
	if (bImage_ > 0) {
		DeleteGraph(bImage_);
		bImage_ = -1;
	}
}

void Banner::Initialize()
{
	bImage_ = LoadGraph("Assets/Image/font.png");
	//bImage_ = LoadGraph("Assets/Image/font2.png");
	assert(bImage_ > 0);
}

void Banner::Update()
{
	static const float MaxTime = 1.5f;
	if (view_ == ViewID::V_GameOver) {
		//変数を操作
		gameOverY = GetEase<float>(200, 400,gameOverTime_ / MaxTime);
		transparency_ = GetEase<float>(0,255,gameOverTime_ / MaxTime);
		gameOverTime_ += 1.0f / 60.0f;
		if (gameOverTime_ > MaxTime)
			gameOverTime_ = MaxTime;
	}
}

void Banner::Draw()
{
	if (view_ == ViewID::V_Start)
		DrawRectGraph(200, 400, 0, 0, 429, 64, bImage_, TRUE);//スタート
		//DrawRectGraph(200, 400, 0, 0, 256, 64, bImage_, TRUE);//スタート
	else if (view_ == ViewID::V_Clear)
		DrawRectGraph(200, 400, 0, 64, 429, 64, bImage_, TRUE);//クリア
		//DrawRectGraph(200, 400, 0, 64, 256, 64, bImage_, TRUE);//クリア
	else if (view_ == ViewID::V_GameOver) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, transparency_);
		DrawRectGraph(200, gameOverY, 0, 128, 429, 64, bImage_, TRUE);//ゲームオーバー
		//DrawRectGraph(200, gameOverY, 0, 128, 256, 128, bImage_, TRUE);//ゲームオーバー
	}
}

void Banner::View(ViewID id)
{
	view_ = id;
	if (view_ == ViewID::V_GameOver) {
		//変数の初期化
		gameOverY = 200;
		transparency_ = 0;
		gameOverTime_ = 0.0f;
	}
}
