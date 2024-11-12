#pragma once
#include "Engine/GameObject.h"
#include "Player.h"
#include "Meteorite.h"

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

	//ステージ変更などに使う
	void ChangeStage(int _mapNo) { mapNo_ = _mapNo; }

	//ステージの種類のゲッター
	int GetMapNo() { return mapNo_; }

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

	//隕石が当たった時の地面などの破壊
	void BreakGround(int x, int y);

private:
	int hImage_;        //ステージの素材
	int gPict_;         //背景の画像
	int* map_;          //ステージの情報を入れる
	int width_;         //ステージの横
	int height_;        //ステージの縦
	int mapNo_;         //どのステージかの識別
	bool IsWallBlock(int x, int y);
	float timer_;
};

