#include "MoveSystem.h"
#include "Player.h"
#include "inside.h"
#include "Time.h"
#include "Mouse.h"
#include "Define.h"
#include "Score.h"

MoveSystem::MoveSystem( PlayerPtr player, InsidePtr inside, TimePtr time, ScorePtr score ) :
	_player( player ),
	_inside( inside ),
	_time( time ),
	_score( score ) {

}

MoveSystem::~MoveSystem( ) {
}

void MoveSystem::update( ) {
	moveConditions( );
}

void MoveSystem::moveConditions( ) {
	Inside::STATUS status = Inside::STATUS::FLASHING;
	if ( _time->getCountStart( ) < START_TIME ) {
		if ( _player->isClear( ) == false && _player->isGameOver( ) == false && _time->isGameover( ) == false ) {
			_player->move( );
			_inside->changeTarget( _inside->getStatus( _inside->getTarget( ) ) );
			_inside->inside( );
			_time->countGame( );
		}
	}

	if ( _player->isClear( ) ) {
		_score->initialize( );

	}
}


