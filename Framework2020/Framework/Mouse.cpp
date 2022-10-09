#include "Mouse.h"
#include "Application.h"
#include "DxLib.h"
#include "Image.h"

Mouse::Mouse( ) {
	_key[ MOUSE_LEFT_BUTTON ] = 0;
	_key[ MOUSE_RIGHT_BUTTON ] = 0;
	_wheel_value = 0;
}

Mouse::~Mouse( ) {
}

void Mouse::update( ) {
	int x = 0;
	int y = 0;
	GetMousePoint( &x, &y );
	_pos = Vector2( x, y );

	int mouse = GetMouseInput( );
	if ( mouse & MOUSE_INPUT_LEFT ) {
		_key[ MOUSE_LEFT_BUTTON ]++;
		if ( _key[ MOUSE_LEFT_BUTTON ] > 2 ) {
			_key[ MOUSE_LEFT_BUTTON ] = 2;
		}
	} else {
		_key[ MOUSE_LEFT_BUTTON ] = 0;
	}

	if ( mouse & MOUSE_INPUT_RIGHT ) {
		_key[ MOUSE_RIGHT_BUTTON ]++;
		if ( _key[ MOUSE_RIGHT_BUTTON ] > 2 ) {
			_key[ MOUSE_RIGHT_BUTTON ] = 2;
		}
	} else {
		_key[ MOUSE_RIGHT_BUTTON ] = 0;
	}

	_wheel_value = GetMouseWheelRotVol( );
}

void Mouse::setDispFlag( bool flag ) {
	SetMouseDispFlag( flag );
}

Vector2 Mouse::getPos( ) const {

	return _pos;
}

void Mouse::setPos( const Vector2& pos ) {
	_pos = pos;
	SetMousePoint( ( int )_pos.x, ( int )_pos.y );
}

int Mouse::getWheelRotValue( ) const {
	return _wheel_value;
}

bool Mouse::isPushRightButton( ) const {
	bool result = false;
	if ( _key[ MOUSE_RIGHT_BUTTON ] == 1 ) {
		result = true;
	}
	return result;
}

bool Mouse::isHoldRightButton( ) const {
	bool result = false;
	if ( _key[ MOUSE_RIGHT_BUTTON ] >= 1 ) {
		result = true;
	}
	return result;
}

bool Mouse::isPushLeftButton( ) const {
	bool result = false;
	if ( _key[ MOUSE_LEFT_BUTTON ] == 1 ) {
		result = true;
	}
	return result;
}

bool Mouse::isHoldLeftButton( ) const {
	bool result = false;
	if ( _key[ MOUSE_LEFT_BUTTON ] >= 1 ) {
		result = true;
	}
	return result;
}
