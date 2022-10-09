#pragma once
#include "smart_ptr.h"

PTR( Score );
PTR( Inside );
PTR( Time);
PTR( Player );

class Score {
public:
	Score( InsidePtr inside,TimePtr time, PlayerPtr player );
	virtual ~Score( );
public:
	void initialize( );
	int getScore( );
private:
	void calculation( );
private:
	int _max_score;
	int _time_score;
	int _inside_score;
private:
	InsidePtr _inside;
	TimePtr _time;
	PlayerPtr _player;
};

