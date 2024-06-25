#pragma once
#include "Engine/GameObject.h"

//ステージを管理するクラス
namespace {
	const int MAP_NUM = 3;
	const int MAP_SIZE = 32;
}

class Stage :
    public GameObject
{
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

	//マップをセットする
	void StageSet();

	//マップの状態をリセットする
	void StageReset();

	void ChangeStage() { mapNo_++; }

	/// <summary>
	/// 右側の点が当たっているか調べる
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <returns>めりこんだ量(ドット)</returns>
	int CollisionRight(int x, int y);

	/// <summary>
	/// 左側の点が当たっているか調べる
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <returns>めり込んだ量(ドット)</returns>
	int CollisionLeft(int x, int y);

	/// <summary>
	/// 下の点が当たっているか調べる
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <returns>めり込んだ量(ドット)</returns>
	int CollisionDown(int x, int y);

	/// <summary>
	/// 上の点が当たっているか調べる
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <returns></returns>
	int CollisionUp(int x, int y);

private:
	int hImage_;
	int* map_;
	int width_;
	int height_;
	int mapNo_;
	bool IsWallBlock(int x, int y);
	bool prevResetKey_;
};

