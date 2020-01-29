
# pragma once
# include "Common.hpp"
# include "Item.h"

// ゲームシーン
class Game : public MyApp::Scene
{
private:
	// 経過フレーム
	int32 flame = 0;

	// オブジェクトのサイズ
	static constexpr Size size = Size(80, 80);

	// オブジェクトの生成周期
	static constexpr int32 obj_time_slow   = 45;
	static constexpr int32 obj_time_midium = 40;
	static constexpr int32 obj_time_fast   = 30;



	// 自機の速さ
	static constexpr double player_speed = 5.0;
	static constexpr double boost = 10.0;
	static constexpr double debuff = 3.5;

	void object_init();

	// オブジェクトの配列
	// 0 : 単位
	// 1 : ラーメン
	// 2 : 酒
	// 3 : 卒論
	// 4 : 魔材
	Array<Item> item;
	   
	// 自機の当たり判定
	Rect m_player = Rect(300, 500, 80, 80);

	// バフデバフの効果時間を確認する
	Stopwatch effectTime;

	// ブースト中か否か
	bool isBoost = false;

	// デバフ中か否か
	bool isDebuff = false;

	// テクスチャ
	Texture player;
	Texture credit;
	Texture ramen;
	Texture beer;
	Texture thesis;
	Texture monster;

	Texture heart;
	Texture skull;

	// スコア
	int32 m_score = 0;

	// プレイヤのライフ
	int32 life = 3;
public:

	Game(const InitData& init);

	void update() override;

	void draw() const override;
};