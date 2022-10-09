#include "Introduction.h"
#include "Scene2D.h"
#include "Scene3D.h"
#include "Drawer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Log.h"

void Introduction::procedure( ) {
	Task::create< Drawer >( );
	Task::create< Mouse >( );
	Task::create< Keyboard >( );
	Task::create< Log >( );

	REGIST( Scene2D );
	REGIST( Scene3D );

	//start( "Scene3D" );
	start( "Scene2D" );
}
