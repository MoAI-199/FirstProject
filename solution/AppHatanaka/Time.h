#pragma once
#include "Drawer.h"

PTR( Image );

class Time {
public:
	Time( );
	virtual ~Time( );
public:
	void update( );
	void startUpdate( );
	void countGame( );
	void countStart( );
public:
	bool isFlashing( ) const;
	int getCountGame( ) const;
	int getCountStart( ) const;
	bool isGameover( ) const;

private:
	int _game_count;
	int _start_count;
	bool _game_over;
private:
	ImagePtr _image;
};

