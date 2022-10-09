#include "Wall.h"
#include "Player.h"
#include "Map.h"
#include "Log.h"
#include "Drawer.h"

Wall::Wall( MapPtr map, PlayerPtr player ) :
	_map( map ),
	_player( player ) {
}

Wall::~Wall( ) {
}

void Wall::update( ) {
	collision( );
}

void Wall::collision( ) {
	/*Vector2 player_top = _player->getPos( ) - Vector2(40,40);
	Vector2 player_bottom = _player->getPos( ) + Vector2( 40, 40 );
	Vector2 wall_top;
	Vector2 wall_bottom;
	DrawerPtr draw = Task::get<Drawer>( );

	for ( int i = 0; i < Map::MAP_WIDTH; i++ ) {
		for ( int j = 0; j < Map::MAP_HEIGHT; j++ ) {

			Map::WALL object = _map->getWallType( i, j );
			int pos_x = i * Map::WALL_SIZE;
			int pos_y = j * Map::WALL_SIZE;

			switch ( object ) {
			case Map::WALL::NONE:
				continue;
				break;
			case Map::WALL::FRAME:
				wall_top = Vector2( pos_x, pos_y );
				wall_bottom = Vector2( pos_x + 16, pos_y + 16 );
				break;
			}

		}
	}

	if ( wall_top.getLength( ) <= player_bottom.getLength( ) &&
		 wall_bottom.getLength( ) >= player_top.getLength( ) ) {
		LogPtr log = Task::get< Log >( );
		log->status( "Hit" );
	} else {
		LogPtr log = Task::get< Log >( );
		log->status( "NoHit" );
	}*/
}

void Wall::createWall( ) {

}
