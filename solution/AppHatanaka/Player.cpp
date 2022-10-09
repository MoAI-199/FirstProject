#include "Player.h"
#include "Mouse.h"
#include "Map.h"
#include "inside.h"
#include "time.h"
#include "ChangeMap.h"

const double START_X_1 = 40.0;
const double START_Y_1 = 440.0 + 40.0;
const double START_X_2 = 40.0;
const double START_Y_2 = 280.0 + 40.0;

const double SPEED = 10.0;
const double RADIUS = 40;
const int CLEAR_LINE = 1880;
const int LEFT_LIMIT = 40;
const int RIGHT_LIMIT = 1880;
const int TOP_LIMIT = 40;
const int BOTTOM_LIMIT = 1040;


Player::Player( MapPtr map, InsidePtr inside, TimePtr time ) :
	_map( map ),
	_inside( inside ),
	_time( time ),
	_clear( false ),
	_hit( false ),
	_score( false ) {
	ChangeMapPtr change = Task::get< ChangeMap >( );
	switch ( change->getMapNum( ) ) {
	case 1:
		_pos = Vector2( START_X_1, START_Y_1 );
		break;
	case 2:
		_pos = Vector2( START_X_2, START_Y_2 );
		break;
	}

}


Player::~Player( ) {

}

void Player::update( ) {
	startPos( );
	frameCollision( );
	insideCollision( );
	clear( );
	moveLimit( );
}

void Player::startPos( ) {
	if ( _time->getCountStart( ) == 0 ) {
		MousePtr mouse = Task::get< Mouse >( );
		mouse->setPos( _pos );
	}
}

void Player::move( ) {
	MousePtr mouse = Task::get< Mouse >( );
	Vector2 mouse_pos = mouse->getPos( );
	Vector2 vec = mouse_pos - _pos;
	_pos += vec.normalize( ) * SPEED;

	if ( vec.getLength( ) < SPEED ) {
		_pos = mouse_pos;
	}
}

void Player::frameCollision( ) {

	Vector2 top_pos = _pos + Vector2( 0, -RADIUS );
	Vector2 bottom_pos = _pos + Vector2( 0, RADIUS );
	Vector2 left_pos = _pos + Vector2( -RADIUS, 0 );
	Vector2 right_pos = _pos + Vector2( RADIUS, 0 );

	if ( _map->getWallType( top_pos ) == Map::WALL::FRAME ||
		 _map->getWallType( bottom_pos ) == Map::WALL::FRAME ||
		 _map->getWallType( right_pos ) == Map::WALL::FRAME ||
		 _map->getWallType( left_pos ) == Map::WALL::FRAME ) {
		_hit = true;
	}

}

void Player::insideCollision( ) {
	
	ChangeMapPtr change = Task::get< ChangeMap >( );
	switch ( change->getMapNum( ) ) {
	case 1:
		collision( Map::WALL::INSIDE1, _inside->getStatus( 0 ) );
		collision( Map::WALL::INSIDE2, _inside->getStatus( 1 ) );
		collision( Map::WALL::INSIDE3, _inside->getStatus( 2 ) );
		collision( Map::WALL::INSIDE4, _inside->getStatus( 3 ) );
		collision( Map::WALL::INSIDE5, _inside->getStatus( 4 ) );
		collision( Map::WALL::INSIDE6, _inside->getStatus( 5 ) );
		collision( Map::WALL::INSIDE7, _inside->getStatus( 6 ) );
		break;
	case 2:
		collision( Map::WALL::INSIDE1, _inside->getStatus( 0 ) );
		collision( Map::WALL::INSIDE2, _inside->getStatus( 1 ) );
		collision( Map::WALL::INSIDE3, _inside->getStatus( 2 ) );
		collision( Map::WALL::INSIDE4, _inside->getStatus( 3 ) );
		collision( Map::WALL::INSIDE5, _inside->getStatus( 4 ) );
		collision( Map::WALL::INSIDE6, _inside->getStatus( 5 ) );
		collision( Map::WALL::INSIDE7, _inside->getStatus( 6 ) );
		collision( Map::WALL::INSIDE8, _inside->getStatus( 7 ) );
		collision( Map::WALL::INSIDE9, _inside->getStatus( 8 ) );
		collision( Map::WALL::INSIDE10, _inside->getStatus( 9 ) );
		break;
	}
}

void Player::clear( ) {
	Vector2 right_pos = _pos + Vector2( RADIUS, 0 );
	if ( !isGameOver( ) ) {
		if ( _pos.x >= CLEAR_LINE ) {
			_clear = true;
			_score = true;
		}
	}
}

void Player::moveLimit( ) {
	if ( _pos.x <= LEFT_LIMIT ) {
		_pos.x = LEFT_LIMIT;
	}

	if ( _pos.x >= RIGHT_LIMIT ) {
		_pos.x = RIGHT_LIMIT;
	}

	if ( _pos.y <= TOP_LIMIT ) {
		_pos.y = TOP_LIMIT;
	}
	if ( _pos.y >= BOTTOM_LIMIT ) {
		_pos.y = BOTTOM_LIMIT;
	}
}

void Player::collision( Map::WALL type, Inside::STATUS status ) {

	Vector2 top_pos = _pos + Vector2( 0, -RADIUS );
	Vector2 bottom_pos = _pos + Vector2( 0, RADIUS );
	Vector2 left_pos = _pos + Vector2( -RADIUS, 0 );
	Vector2 right_pos = _pos + Vector2( RADIUS, 0 );

	if ( _map->getWallType( top_pos ) == type ||
		 _map->getWallType( bottom_pos ) == type ||
		 _map->getWallType( right_pos ) == type ||
		 _map->getWallType( left_pos ) == type ) {
		if ( status != Inside::STATUS::LOST ) {
			_hit = true;
		}
	}
}

bool Player::isClear( ) const {
	return _clear;
}

Vector2 Player::getPos( ) const {
	return _pos;
}

bool Player::isGameOver( ) const {
	return _hit;
}

bool Player::isScore( ) const {
	return _score;
}


