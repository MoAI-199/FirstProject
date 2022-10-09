#include "Time.h"
#include "Drawer.h"
#include "Image.h"

const int TIME = 60 * 30;
const int FLASH_INTERVAL = 8;
const int FLASH_FPS = 15;
const int START_COUNTDOWN_COUNT = 4 * 30;

Time::Time( ) {
	_game_count = TIME;
	_start_count = START_COUNTDOWN_COUNT;
	_game_over = false;
}

Time::~Time( ) {
}

void Time::update( ) {
	countStart( );
	
}

void Time::startUpdate( ) {
	countStart( );
}

void Time::countGame( ) {
	_game_count--;
	if ( _game_count <= 0 ) {
		_game_over = true;
	}
}

void Time::countStart( ) {
	_start_count--;
}


bool Time::isFlashing( ) const {
	int timing = _game_count % FLASH_FPS;
	return timing <= FLASH_INTERVAL;
}

int Time::getCountGame( ) const {
	return _game_count / 30;
}

int Time::getCountStart( ) const {
	return _start_count / 30;
}

bool Time::isGameover( ) const {
	return _game_over;
}
