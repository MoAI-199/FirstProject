/*
* １年次に制作したコードになります。
* なので、今の自分なら「こう書くな」というコメントをいれています。
* 他のプロジェクトと比べて自分の成長度を知っていただけたらと思います。
* */

#pragma once
#include "smart_ptr.h"
#include "mathematics.h"
#include "Map.h"
#include "inside.h"

PTR( Player );
PTR( Map );
PTR( Inside );
PTR( Time );

class Player {
public:
	Player( MapPtr map, InsidePtr inside ,TimePtr time);
	virtual ~Player( );
public:
	Vector2 getPos( ) const;
	bool isClear( ) const;
	bool isGameOver( ) const;
	bool isScore( ) const;
public:
	/// <summary>
	/// 更新
	/// </summary>
	void update( ) ;
	/// <summary>
	/// 初期座標設定
	/// </summary>
	void startPos( );
	/// <summary>
	/// プレイヤーの動作
	/// 改善点：publicで行う必要が無い。なぜupdate関数で呼ばないのか謎である。
	/// </summary>
	void move( );
private:
	/// <summary>
	/// 外枠とのあたり判定を行っている
	/// 改善点：Playerクラスであたり判定をやる理由が不明
	/// </summary>
	void frameCollision( );
	/*内側の壁とのあたり判定をまとめている。
	* 改善点：コーディングルール「関数は動詞から始める」に則っていない。
	* 解決案はcollisionInside()に変える。*/
	void insideCollision( );
	/// <summary>
	/// クリアの条件を満たしたらクリアする
	/// 改善点：１関数１機能とするのが理想なので、条件式は別のisClearなどと別の関数にするのがよい。
	/// </summary>
	void clear( );
	/// <summary>
	/// 移動制限
	/// </summary>
	void moveLimit( );
	/// <summary>
	/// ブロック１つ１つのあたり判定を取っている。
	/// </summary>
	/// <param name="type">壁の種類</param>
	/// <param name="status">壁の状態</param>
	void collision( Map::WALL type, Inside::STATUS status );
private:
	Vector2 _pos;
	MapPtr _map;
	InsidePtr _inside;
	TimePtr _time;
private:
	bool _clear;
	bool _hit;
	bool _gameover;
	bool _score;
};

