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
	/*int MapData[MAP_HEIGHT][MAP_WIDTH] =
	{
		{ 16, 17, 255, 255, 16, 17, 255, 255, 16, 17 } ,
		{ 32, 33, 255, 255, 32, 33, 255, 255, 32, 33 } ,
		{ 255, 255, 16, 17, 255, 255, 16, 17,255, 255 } ,
		{ 255, 255, 32, 33, 255, 255, 32, 33, 255, 255 } ,
		{ 16, 17, 255, 255, 16, 17, 255,255, 16, 17 } ,
		{ 32, 33, 255, 255, 32, 33, 255,255, 32, 33 } ,
		{ 255, 255, 16, 17, 255, 255, 16, 17, 255, 255 } ,
		{ 255, 255, 32, 33, 255, 255, 32, 33, 255, 255 }
	};*/

	int hImage_;
	int* map_;
	int width_;
	int height_;
public:
	//コンストラクタ
	Stage(GameObject* parent);

    //デストラクタ
	~Stage();

	//初期化
	void Initialize() override;

    //更新
	void Update() override;
    
	//描画
	void Draw()override;

};

