#include "TitleScene.h"
#include "Drawer.h"
#include "define.h"
#include "Mouse.h"

SCENE( TitleScene );

TitleScene::TitleScene( ) {
	DrawerPtr draw = Task::get<Drawer>( );
	_image_title = draw->createImage( "Image/title.png" );
}

TitleScene::~TitleScene( ) {
}

void TitleScene::update( ) {
	draw( );
	start( );
}

void TitleScene::start( ) {
	MousePtr mouse = Task::get< Mouse >( );
	int top_pos_x = TITLE_START_X;
	int top_pos_y = TITLE_START_Y;
	int bottom_pos_x = TITLE_START_X + 484;
	int bottom_pos_y = TITLE_START_Y + 157;

	if ( top_pos_x <= mouse->getPos( ).x &&
		 top_pos_y <= mouse->getPos( ).y &&
		 bottom_pos_x >= mouse->getPos( ).x&&
		 bottom_pos_y >= mouse->getPos( ).y&&
		 mouse->isPushLeftButton( )){
		loadScene( "SelectScene" );
	}
}

void TitleScene::draw( ) {
	_image_title->draw( 0, 0 );
}
