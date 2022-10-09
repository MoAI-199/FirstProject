#include "Introduction.h"
#include "Drawer.h"
#include "Application.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "ChangeMap.h"
#include "Player.h"


void Introduction::procedure( ) {
	Task::create< Drawer >( );
	Task::create< Mouse >( );
	Task::create< Keyboard >( );
	Task::create< ChangeMap >( );
	
	ApplicationPtr app = Application::getInstance( );
	app->setFPS( 30 );

	MousePtr mouse = Task::get< Mouse >( );
	mouse->setDispFlag( true );

	REGIST( Stage1 );
	REGIST( SelectScene );
	REGIST( TitleScene );
	REGIST( CompleteScene );

	start( "TitleScene" );
	
}
