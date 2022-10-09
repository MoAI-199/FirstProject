#include "Drawer2D.h"
#include "Drawer.h"
#include "Application.h"
#include "Mouse.h"
#include "Keyboard.h"

Drawer2D::Drawer2D( ) {

}

Drawer2D::~Drawer2D( ) {

}

void Drawer2D::initialize( ) {
	ApplicationPtr app = Application::getInstance( );
	_origin_x = app->getScreenWidth( ) / 2;
	_origin_y = app->getScreenHeight( ) / 2;
	_scale = 10.0;
	_dir_y = -1;
}

void Drawer2D::update( ) {
	controllGizmo( );
	drawGuide( );
}

void Drawer2D::controllGizmo( ) {
	MousePtr mouse = Task::get< Mouse >( );
	KeyboardPtr keyboard = Task::get< Keyboard >( );

	if ( mouse->isHoldLeftButton( ) ) {
		if ( !_gizmo_translate_capturing ) {
			_gizmo_translate_capturing = true;
			_gizmo_translate_origin = Vector2( _origin_x, _origin_y ) - mouse->getPos( );
		}
		Vector2 origin_pos = mouse->getPos( ) + _gizmo_translate_origin;
		_origin_x = ( int )origin_pos.x;
		_origin_y = ( int )origin_pos.y;
	} else {
		_gizmo_translate_capturing = false;
	}

	if ( keyboard->isPushKey( "NUM+" ) ) {
		_dir_y = 1;
	}
	if ( keyboard->isPushKey( "NUM-" ) ) {
		_dir_y = -1;
	}

	int wheel_value = mouse->getWheelRotValue( );
	if ( wheel_value != 0 ) {
		_scale += wheel_value;
		if ( _scale < 1.0 ) {
			_scale = 1.0;
		}
	}
}

void Drawer2D::drawGuide( ) {
	ApplicationPtr app = Application::getInstance( );
	int screen_width = app->getScreenWidth( );
	int screen_height = app->getScreenHeight( );

	DrawerPtr drawer = Task::get< Drawer >( );
	// xŽ²
	drawer->drawLine( 0, _origin_y, screen_width, _origin_y, Color( 1.0, 0, 0 ) );
	// yŽ²
	drawer->drawLine( _origin_x, 0, _origin_x, screen_height, Color( 0, 0, 1.0 ) );
}

void Drawer2D::drawCircle( std::string name, const Vector2& pos, double radius ) {
	int sx = _origin_x + ( int )( pos.x * _scale );
	int sy = _origin_y + ( int )( pos.y * _scale * _dir_y );

	DrawerPtr drawer = Task::get< Drawer >( );
	drawer->drawCircle( sx, sy, radius * _scale, Color( 1, 1, 1 ), false );

	drawer->drawString( sx, sy, name.c_str( ) );
}

void Drawer2D::drawLine( std::string name, const Vector2& pos1, const Vector2& pos2 ) {
	int sx1 = _origin_x + ( int )( pos1.x * _scale );
	int sy1 = _origin_y + ( int )( pos1.y * _scale * _dir_y );
	int sx2 = _origin_x + ( int )( pos2.x * _scale );
	int sy2 = _origin_y + ( int )( pos2.y * _scale * _dir_y );

	DrawerPtr drawer = Task::get< Drawer >( );
	drawer->drawLine( sx1, sy1, sx2, sy2, Color( 1, 1, 1 ) );

	drawer->drawString( sx1, sy1, name.c_str( ) );
}
