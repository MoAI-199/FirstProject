#pragma once
#include "smart_ptr.h"
#include "Scene.h"
#include "ChangeMap.h"

PTR( Stage1 );
PTR( Player );
PTR( Viewer );
PTR( Map );
PTR( Map1 );
PTR( Map2 );
PTR( Wall );
PTR( Inside );
PTR( Time );
PTR( MoveSystem );
PTR( Score );
PTR( ChangeMap );

class Stage1 : public Scene {
public:
	Stage1( );
	virtual ~Stage1( );
private:
	void initialize( );
	void update( );
	void reset( );
	void selectReset( );
	void selectSelectStage( );
	void selectTitle( );
	void nextStage( );
	
private:
	PlayerPtr _player;
	ViewerPtr _viewer;
	MapPtr _map;
	WallPtr _wall;
	InsidePtr _inside;
	TimePtr _time;
	MoveSystemPtr _move_system;
	ScorePtr _score;
private:
	bool _complete;
};

