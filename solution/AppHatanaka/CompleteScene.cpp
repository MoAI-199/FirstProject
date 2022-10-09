#include "CompleteScene.h"
#include "Image.h"
#include "Drawer.h"
#include "Mouse.h"
#include "Define.h"

SCENE( CompleteScene );

CompleteScene::CompleteScene( ) {
	DrawerPtr draw = Task::get< Drawer >( );
	_image_rizarut = draw->createImage( "Image/complete.png" );
}

CompleteScene::~CompleteScene( ) {
}

void CompleteScene::update( ) {
	_image_rizarut->draw( 0,0 );
	selectTitle( );
}

void CompleteScene::selectTitle( ) {
	MousePtr mouse = Task::get<Mouse>( );

	int complete_top_x_title = COMPLETE_TITLE_X;
	int complete_top_y_title = COMPLETE_TITLE_Y;
	int complete_bottom_x_title = COMPLETE_TITLE_X + 941;
	int complete_bottom_y_title = COMPLETE_TITLE_Y + 147;

	if ( complete_top_x_title <= mouse->getPos( ).x &&
		 complete_top_y_title <= mouse->getPos( ).y &&
		 complete_bottom_x_title >= mouse->getPos( ).x &&
		 complete_bottom_y_title >= mouse->getPos( ).y &&
		 mouse->isPushLeftButton( ) ) {

		loadScene( "TitleScene" );
	}
}
