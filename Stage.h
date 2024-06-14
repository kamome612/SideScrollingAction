#pragma once
#include "Engine/GameObject.h"

//ステージを管理するクラス
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
	//コンストラクタ
	Stage();

    //デストラクタ
	~Stage();

	//初期化
	void Initialize() override;

    //更新
	void Update() override;
    
	//描画
	void Draw()override;

};

