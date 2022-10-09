#include "Map.h"
#include <string>
#include "inside.h"
#include "Define.h"

//width:80/height:45/size:16


Map::Map( std::string map ) :
	MAP( map ) {
	_wall_type[ 'f' ] = WALL::FRAME;
	_wall_type[ '1' ] = WALL::INSIDE1;
	_wall_type[ '2' ] = WALL::INSIDE2;
	_wall_type[ '3' ] = WALL::INSIDE3;
	_wall_type[ '4' ] = WALL::INSIDE4;
	_wall_type[ '5' ] = WALL::INSIDE5;
	_wall_type[ '6' ] = WALL::INSIDE6;
	_wall_type[ '7' ] = WALL::INSIDE7;
	_wall_type[ '8' ] = WALL::INSIDE8;
	_wall_type[ '9' ] = WALL::INSIDE9;
	_wall_type[ 't' ] = WALL::INSIDE10;
	_wall_type[ ' ' ] = WALL::NONE;
}

Map::~Map( ) {
}

Map::WALL Map::getWallType( const Vector2& pos ) const {
	int x = ( int )pos.x / WALL_SIZE;
	int y = ( int )pos.y / WALL_SIZE;
	int idx = x + y * MAP_WIDTH;
	char ch = MAP[ idx ];
	if ( _wall_type.count( ch ) == 0 ) {
		return WALL::NONE;
	}
	return _wall_type.at( ch );
}

bool Map::isOverlappedWall( const Vector2& pos ) const {
	int x = ( int )pos.x / WALL_SIZE;
	int y = ( int )pos.y / WALL_SIZE;
	int idx = x + y * MAP_WIDTH;
	char ch = MAP[ idx ];
	return _wall_type.count( ch ) > 0;
}



