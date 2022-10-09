#pragma once
#include "smart_ptr.h"
#include "inside.h"

PTR( Mouse );
PTR( Player );
PTR( Insdie );
PTR( Time );
PTR( Score );
class MoveSystem {
	
public:
	MoveSystem( PlayerPtr player, InsidePtr inside,TimePtr time,ScorePtr score );
	virtual ~MoveSystem( );
public:
	void update( );
	void moveConditions( );
private:
	PlayerPtr _player;
	InsidePtr _inside;
	TimePtr _time;
	ScorePtr _score;
};

