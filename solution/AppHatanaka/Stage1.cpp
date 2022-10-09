#include "Stage1.h"
#include "Player.h"
#include "Map.h"
#include "Map1.h"
#include "Map2.h"

#include "Viewer.h"
#include "inside.h"
#include "Time.h"
#include "MoveSystem.h"
#include "Mouse.h"
#include "Define.h"
#include "Score.h"
#include "ChangeMap.h"
//debug
#include "Keyboard.h"

SCENE( Stage1 );

Stage1::Stage1( ) {
}

Stage1::~Stage1( ) {
}

void Stage1::reset( ) {
	initialize( );
}

void Stage1::initialize( ) {
	ChangeMapPtr change = Task::get< ChangeMap >( );
	_map = Map2Ptr( new Map2( ) );

	switch ( change->getMapNum( ) ) {
	case 1:
		_map = Map1Ptr( new Map1( ) );
		break;
	case 2:
		_map = Map2Ptr( new Map2( ) );
		break;
	}
	_time = TimePtr( new Time( ) );
	_inside = InsidePtr( new Inside( _map ) );
	_player = PlayerPtr( new Player( _map, _inside, _time ) );
	_score = ScorePtr( new Score( _inside, _time, _player ) );
	_move_system = MoveSystemPtr( new MoveSystem( _player, _inside, _time, _score ) );
	_viewer = ViewerPtr( new Viewer( _player, _map, _inside, _time, _score ) );
}

void Stage1::update( ) {
	_player->update( );
	_inside->update( );
	_move_system->update( );

	_viewer->draw( );

	if ( !_player->isClear( ) ) {
		_time->update( );

	}
	selectReset( );
	selectSelectStage( );
	selectTitle( );
	nextStage( );
}


void Stage1::selectReset( ) {
	MousePtr mouse = Task::get< Mouse >( );
	//clearŽž
	int retry_pos_top_x = RESULT_RETRY_POS.x;
	int retry_pos_top_y = RESULT_RETRY_POS.y;
	int retry_pos_bottom_x = RESULT_RETRY_POS.x + 438;
	int retry_pos_bottom_y = RESULT_RETRY_POS.y + 97;

	if ( retry_pos_top_x <= mouse->getPos( ).x &&
		 retry_pos_top_y <= mouse->getPos( ).y &&
		 retry_pos_bottom_x >= mouse->getPos( ).x &&
		 retry_pos_bottom_y >= mouse->getPos( ).y &&
		 mouse->isPushLeftButton( ) &&
		 _player->isClear( ) ) {

		reset( );
	}
	//gameoverŽž
	int retry_top_x_gemeover = GAMEOVER_RESULT_RETRY_POS.x;
	int retry_top_y_gemeover = GAMEOVER_RESULT_RETRY_POS.y;
	int retry_bottom_x_gemeover = GAMEOVER_RESULT_RETRY_POS.x + 584;
	int retry_bottom_y_gemeover = GAMEOVER_RESULT_RETRY_POS.y + 134;
	if ( _player->isGameOver( ) || _time->isGameover( ) ) {

		if ( retry_top_x_gemeover <= mouse->getPos( ).x &&
			 retry_top_y_gemeover <= mouse->getPos( ).y &&
			 retry_bottom_x_gemeover >= mouse->getPos( ).x &&
			 retry_bottom_y_gemeover >= mouse->getPos( ).y &&
			 mouse->isPushLeftButton( ) ) {
			reset( );
		}
	}
}

void Stage1::selectSelectStage( ) {
	MousePtr mouse = Task::get<Mouse>( );
	//clear
	int select_top_x_clear = RESULT_SELECT_STAGE_X;
	int select_top_y_clear = RESULT_SELECT_STAGE_Y;
	int select_bottom_x_clear = RESULT_SELECT_STAGE_X + 709;
	int select_bottom_y_clear = RESULT_SELECT_STAGE_Y + 95;

	if ( select_top_x_clear <= mouse->getPos( ).x &&
		 select_top_y_clear <= mouse->getPos( ).y &&
		 select_bottom_x_clear >= mouse->getPos( ).x &&
		 select_bottom_y_clear >= mouse->getPos( ).y &&
		 mouse->isPushLeftButton( ) &&
		 _player->isClear( ) ) {

		loadScene( "SelectScene" );
	}
	//gameover
	int select_top_x_gameover = GAMEOVER_SELECT_STAGE_X;
	int select_top_y_gameover = GAMEOVER_SELECT_STAGE_Y;
	int select_bottom_x_gameover = GAMEOVER_SELECT_STAGE_X + 941;
	int select_bottom_y_gameover = GAMEOVER_SELECT_STAGE_Y + 145;
	if ( _player->isGameOver( ) || _time->isGameover( ) ) {

		if ( select_top_x_gameover <= mouse->getPos( ).x &&
			 select_top_y_gameover <= mouse->getPos( ).y &&
			 select_bottom_x_gameover >= mouse->getPos( ).x &&
			 select_bottom_y_gameover >= mouse->getPos( ).y &&
			 mouse->isPushLeftButton( ) ) {

			loadScene( "SelectScene" );
		}
	}

}

void Stage1::selectTitle( ) {
	MousePtr mouse = Task::get<Mouse>( );
	//clear
	int select_top_x_clear = RESULT_TITLE_POS_X;
	int select_top_y_clear = RESULT_TITLE_POS_Y;
	int select_bottom_x_clear = RESULT_TITLE_POS_X + 469;
	int select_bottom_y_clear = RESULT_TITLE_POS_Y + 107;

	if ( select_top_x_clear <= mouse->getPos( ).x &&
		 select_top_y_clear <= mouse->getPos( ).y &&
		 select_bottom_x_clear >= mouse->getPos( ).x &&
		 select_bottom_y_clear >= mouse->getPos( ).y &&
		 mouse->isPushLeftButton( ) &&
		 _player->isClear( ) ) {

		loadScene( "TitleScene" );
	}
	//gameover
	int select_top_x_gameover = GAMEOVER_TITLE_POS_X;
	int select_top_y_gameover = GAMEOVER_TITLE_POS_Y;
	int select_bottom_x_gameover = GAMEOVER_TITLE_POS_X + 641;
	int select_bottom_y_gameover = GAMEOVER_TITLE_POS_Y + 137;
	if ( _player->isGameOver( ) || _time->isGameover( ) ) {

		if ( select_top_x_gameover <= mouse->getPos( ).x &&
			 select_top_y_gameover <= mouse->getPos( ).y &&
			 select_bottom_x_gameover >= mouse->getPos( ).x &&
			 select_bottom_y_gameover >= mouse->getPos( ).y &&
			 mouse->isPushLeftButton( ) ) {

			loadScene( "TitleScene" );
		}
	}

}

void Stage1::nextStage( ) {
	MousePtr mouse = Task::get<Mouse>( );
	ChangeMapPtr change = Task::get< ChangeMap >( );
	DrawerPtr draw = Task::get< Drawer >( );
	int select_top_x_next = RESULT_NEXT_X;
	int select_top_y_next = RESULT_NEXT_Y;
	int select_bottom_x_next = RESULT_NEXT_X + 709;
	int select_bottom_y_next = RESULT_NEXT_Y + 103;

	if ( select_top_x_next <= mouse->getPos( ).x &&
		 select_top_y_next <= mouse->getPos( ).y &&
		 select_bottom_x_next >= mouse->getPos( ).x &&
		 select_bottom_y_next >= mouse->getPos( ).y &&
		 mouse->isPushLeftButton( ) &&
		 _player->isClear( ) ) {
		switch ( change->getMapNum( ) ) {
		case 1:
		change->setMapNum( 2 );
		reset( );
			break;
		case 2:
			loadScene( "CompleteScene" );
			break;
		}
	}
}
