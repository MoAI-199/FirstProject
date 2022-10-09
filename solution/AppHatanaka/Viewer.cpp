#include "Viewer.h"
#include "Player.h"
#include "Map.h"
#include "inside.h"
#include "Time.h"
#include "mathematics.h"
#include "Drawer.h"
#include "Image.h"
#include "Define.h"
#include "Score.h"
#include "ChangeMap.h"

Viewer::Viewer( PlayerConstPtr player, MapConstPtr map, InsideConstPtr inside, TimeConstPtr time, ScorePtr score ) :
	_player( player ),
	_map( map ),
	_inside( inside ),
	_time( time ),
	_score( score ) {
	DrawerPtr draw = Task::get< Drawer >( );
	_image_bg = draw->createImage( "Image/BG.png" );
	_image_player = draw->createImage( "Image/ball.png" );
	_image_frame = draw->createImage( "Image/frame.png" );
	_image_normal = draw->createImage( "Image/normal.png" );
	_image_flash = draw->createImage( "Image/flash.png" );
	_image_lost_1 = draw->createImage( "Image/lost1.png" );
	_image_lost_2 = draw->createImage( "Image/lost2.png" );
	_image_lost = draw->createImage( "Image/lost3.png" );

	_image_num = draw->createImage( "Image/value.png" );
	_image_num_red = draw->createImage( "Image/value_red.png" );
	_image_gray = draw->createImage( "Image/BG_countdown.png" );
	_image_countdown = draw->createImage( "Image/startCount.png" );
	_image_game_ui = draw->createImage( "Image/gameUI.png" );
	_image_inside_target = draw->createImage( "Image/inside_target.png" );
	_image_result_clear = draw->createImage( "Image/clear.png" );
	_image_result_gameover = draw->createImage( "Image/gameover.png" );
	_image_result_score_vatule = draw->createImage( "Image/score_num.png" );
	_image_count_start = draw->createImage( "Image/count_start.png" );
}

Viewer::~Viewer( ) {
}

void Viewer::draw( ) {
	drawBackground( );
	drawFrame( );
	drawInside( );
	drawCounter( );
	drawCountdown( );
	drawPlayer( );
	drawClearResult( );
	drawGameOverResult( );
}

void Viewer::drawPlayer( ) const {
	Vector2 pos = _player->getPos( );
	Vector2 pos1 = pos + Vector2( -IMAGE_PLAYER_SIZE / 2, -IMAGE_PLAYER_SIZE / 2 );
	Vector2 pos2 = pos + Vector2( IMAGE_PLAYER_SIZE / 2, IMAGE_PLAYER_SIZE / 2 );
	_image_player->drawExtend( pos1, pos2 );
}

void Viewer::drawBackground( ) const {
	_image_bg->draw( 0, 0 );
}

void Viewer::drawFrame( ) const {
	for ( int i = 0; i < Map::MAP_WIDTH; i++ ) {
		int map_x = i;
		for ( int j = 0; j < Map::MAP_HEIGHT; j++ ) {
			int map_y = j;

			Vector2 pos( i * Map::WALL_SIZE, j * Map::WALL_SIZE );
			Map::WALL type = _map->getWallType( pos );

			int pos_x = map_x * Map::WALL_SIZE;
			int pos_y = map_y * Map::WALL_SIZE;

			switch ( type ) {
			case Map::WALL::NONE:
				continue;
				break;
			case Map::WALL::FRAME:
				_image_frame->draw( Vector2( pos_x, pos_y ) );
				break;
			}
		}
	}
}

void Viewer::drawInside( ) {
	ChangeMapPtr change = Task::get< ChangeMap >( );
	for ( int i = 0; i < Map::MAP_WIDTH; i++ ) {
		int map_x = i;
		for ( int j = 0; j < Map::MAP_HEIGHT; j++ ) {
			int map_y = j;
			Vector2 pos( i * Map::WALL_SIZE, j * Map::WALL_SIZE );
			Map::WALL type = _map->getWallType( pos );
			int pos_x = map_x * Map::WALL_SIZE;
			int pos_y = map_y * Map::WALL_SIZE;
			Vector2 wall_pos( pos_x, pos_y );

			switch ( type ) {
			case Map::WALL::NONE:
			case Map::WALL::FRAME:
				continue;
				break;
			default:
				Inside::STATUS status = _inside->getStatus( ( int )type );

				if ( status != Inside::STATUS::LOST ) {
					_image_normal->draw( pos );
				} else {
					_image_lost->draw( pos );
				}
				if ( _inside->getTarget( ) == ( int )type ) {
					drawStatus( wall_pos, status );
				}
				break;
			}
		}
	}
}

void Viewer::drawStatus( Vector2& pos, Inside::STATUS num ) {

	switch ( num ) {
	case Inside::STATUS::NORMAL:
		_image_inside_target->draw( pos );
		break;
	case Inside::STATUS::FLASHING:

		if ( _time->isFlashing( ) ) {
			_image_inside_target->draw( pos );
		} else {
			_image_flash->draw( pos );
		}
		break;
	case Inside::STATUS::LOST:

		break;
	}
}

void Viewer::drawNumber( Vector2 pos, int number ) {

	int digit_num = 0;
	int number_per_digit[ 100 ] = { 0 };
	do {
		number_per_digit[ digit_num ] = number % 10;
		digit_num++;
		number /= 10;
	} while ( number > 0 );
	int shift_pos = 0;
	for ( int i = digit_num - 1; i >= 0; i-- ) {

		_image_num->drawRect( pos.x + 40 * shift_pos, pos.y, 40 * number_per_digit[ i ], 0, 40, 80 );

		shift_pos++;
	}
}

void Viewer::drawNumberRed( Vector2 pos, int number ) {
	int digit_num = 0;
	int number_per_digit[ 100 ] = { 0 };
	do {
		number_per_digit[ digit_num ] = number % 10;
		digit_num++;
		number /= 10;
	} while ( number > 0 );
	int shift_pos = 0;
	for ( int i = digit_num - 1; i >= 0; i-- ) {

		_image_num_red->drawRect( pos.x + 40 * shift_pos, pos.y, 40 * number_per_digit[ i ], 0, 40, 80 );

		shift_pos++;
	}
}

void Viewer::drawCountNumber( Vector2 pos, int number ) {
	int digit_num = 0;
	int number_per_digit[ 10 ] = { 0 };
	do {
		number_per_digit[ digit_num ] = number % 10;
		digit_num++;
		number /= 10;
	} while ( number > 0 );
	int shift_pos = 0;
	for ( int i = digit_num - 1; i >= 0; i-- ) {
		_image_countdown->drawRect( pos.x + 240 * shift_pos, pos.y, 244 * number_per_digit[ i ], 0, 240, 360 );
		shift_pos++;
	}
}

void Viewer::drawCounter( ) {
	_image_game_ui->draw( 0, 0 );
	//§ŒÀŽžŠÔ
	if ( _time->getCountGame( ) > 10 ) {
		drawNumber( TIME_COUNTER, _time->getCountGame( ) );
	} else {
		drawNumberRed( TIME_COUNTER, _time->getCountGame( ) );
	}
	//ì‚è•Ï‚¦‰ñ”
	drawNumber( CHANGE_COUNT, _inside->getChangeCount( ) );
}

void Viewer::drawCountdown( ) {
	if ( _time->getCountStart( ) >= 0 ) {
		if ( _time->getCountStart( ) == 0 ) {
			_image_gray->draw( 0, 0 );
			_image_count_start->draw( 0, 0 );
		} else {
			_image_gray->draw( 0, 0 );
			drawCountNumber( SENTER_POS, _time->getCountStart( ) );
		}
	}
}

void Viewer::drawClearResult( ) {
	if ( _player->isClear( ) ) {
		_image_result_clear->draw( 0, 0 );
		drawScore( SCORE_POS, _score->getScore( ) );
	}
}

void Viewer::drawGameOverResult( ) {
	if ( _player->isGameOver( ) || _time->isGameover( ) ) {
		_image_result_gameover->draw( 0, 0 );
	}
}

void Viewer::drawScore( Vector2 pos, int number ) {
	int digit_num = 0;
	int number_per_digit[ 100 ] = { 0 };
	do {
		number_per_digit[ digit_num ] = number % 10;
		digit_num++;
		number /= 10;
	} while ( number > 0 );
	int shift_pos = 0;
	for ( int i = digit_num - 1; i >= 0; i-- ) {
		_image_result_score_vatule->drawRect( pos.x + 60 * shift_pos - 10, pos.y - 10, 60 * number_per_digit[ i ], 0, 60, 100 );
		shift_pos++;
	}
}

