#include "Log.h"
#include "Drawer.h"
#include "Application.h"
#include "Keyboard.h"
#include <stdarg.h>

const int PITCH = 20;
const int SIZE = 9;

Log::Log( ) {
	_status_idx = 0;
	off( );
}

Log::~Log( ) {
}

void Log::off( ) {
	_show = false;
}

void Log::update( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( !keyboard ) {
		return;
	}

	if ( keyboard->isHoldKey( "LEFT_CTRL" ) &&
		 keyboard->isHoldKey( "RIGHT_CTRL" ) &&
		 keyboard->isPushKey( "ARROW_RIGHT" ) ) {
		toggleShow( );
	}

}

void Log::draw( ) {
	drawStatus( );
	drawChat( );
}

void Log::drawStatus( ) {
	if ( _show ) {
		DrawerPtr drawer = Task::get< Drawer >( );
		for ( int i = 0; i < _status_idx; i++ ) {
			drawer->drawString( 0, i * PITCH, _line_status[ i ].c_str( ) );
		}
	}
	_status_idx = 0;
}

void Log::drawChat( ) {
	if ( !_show ) {
		return;
	}
	ApplicationPtr app = Application::getInstance( );
	int width = app->getScreenWidth( );
	DrawerPtr drawer = Task::get< Drawer >( );
	for ( int i = 0; i < LINES; i++ ) {
		int length = ( int )_line_chat[ i ].length( );
		drawer->drawString( width - length * SIZE, i * PITCH, _line_chat[ i ].c_str( ) );
	}
}

void Log::toggleShow( ) {
	_show = !_show;
}

void Log::chat( const char * format, ... ) {
	char buf[ 1024 ];
	va_list ap;
	va_start( ap, format );
	vsprintf_s( buf, 1024, format, ap );
	va_end( ap );

	for ( int i = 0; i < LINES - 1; i++ ) {
		_line_chat[ i ] = _line_chat[ i + 1 ];
	}
	_line_chat[ LINES - 1 ] = buf;
}

void Log::status( const char * format, ... ) {
	if ( _status_idx >= LINES ) {
		return;
	}

	char buf[ 1024 ];
	va_list ap;
	va_start( ap, format );
	vsprintf_s( buf, 1024, format, ap );
	va_end( ap );

	_line_status[ _status_idx ] = buf;
	_status_idx++;
}
