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
	void update( ) ;
	void startPos( );
	void move( );
private:
	void frameCollision( );
	void insideCollision( );
	void clear( );
	void moveLimit( );
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

