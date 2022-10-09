#include "SelectScene.h"
#include "Drawer.h"
#include "Image.h"
#include "Player.h"
#include "Mouse.h"
#include "Define.h"
#include "ChangeMap.h"


SCENE( SelectScene );

SelectScene::SelectScene( ) {
	DrawerPtr draw = Task::get< Drawer >( );
	_image_select_stage = draw->createImage( "Image/select_scene.png" );
}

SelectScene::~SelectScene( ) {
}

void SelectScene::initialize( ) {


}

void SelectScene::update( ) {
	_image_select_stage->draw( 0, 0 );
	selectStage( );
}

void SelectScene::selectStage( ) {
	MousePtr mouse = Task::get< Mouse >( );
	ChangeMapPtr change = Task::get< ChangeMap >( );
	int stage1_top_pos_x = SELECT_SCENE_STAGE1_POS_X;
	int stage1_top_pos_y = SELECT_SCENE_STAGE1_POS_Y;
	int stage1_bottom_pos_x = SELECT_SCENE_STAGE1_POS_X + 898;
	int stage1_bottom_pos_y = SELECT_SCENE_STAGE1_POS_Y + 465;

	if ( stage1_top_pos_x <= mouse->getPos( ).x &&
		 stage1_top_pos_y <= mouse->getPos( ).y &&
		 stage1_bottom_pos_x >= mouse->getPos( ).x &&
		 stage1_bottom_pos_y >= mouse->getPos( ).y &&
		 mouse->isPushLeftButton( ) ) {
		change->setMapNum( 1 );
		loadScene( "Stage1" );
	}
	
	int stage2_top_pos_x = SELECT_SCENE_STAGE2_POS_X;
	int stage2_top_pos_y = SELECT_SCENE_STAGE2_POS_Y;
	int stage2_bottom_pos_x = SELECT_SCENE_STAGE2_POS_X + 898;
	int stage2_bottom_pos_y = SELECT_SCENE_STAGE2_POS_Y + 465;

	if ( stage2_top_pos_x <= mouse->getPos( ).x &&
		 stage2_top_pos_y <= mouse->getPos( ).y &&
		 stage2_bottom_pos_x >= mouse->getPos( ).x &&
		 stage2_bottom_pos_y >= mouse->getPos( ).y &&
		 mouse->isPushLeftButton( ) ) {
		change->setMapNum( 2 );
		loadScene( "Stage1" );
	}
}