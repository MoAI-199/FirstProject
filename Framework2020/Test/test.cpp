#include "Introduction.h"
#include "Drawer.h"
#include "Log.h"
#include "Application.h"

void Introduction::procedure( ) {
	Task::create< Drawer >( );

	REGIST( TestScene );
	REGIST( OkutyScene );
	//start( "TestScene" );
	start( "OkutyScene" );
}