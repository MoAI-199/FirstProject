#include "Drawer3D.h"
#include "Drawer.h"
#include "Application.h"
#include "Mouse.h"
#include "Keyboard.h"

Drawer3D::Drawer3D( ) {

}

Drawer3D::~Drawer3D( ) {

}

void Drawer3D::initialize( ) {
	_camera_pos = Vector( 10, 10, 10 );
	_camera_dir = _camera_pos * -1;
	_guide_length = Vector( 10, 10, 10 );
}

void Drawer3D::update( ) {
	controllCamera( );
	drawGuide( );
}

void Drawer3D::controllCamera( ) {
	MousePtr mouse = Task::get< Mouse >( );
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	
	if ( mouse->isHoldRightButton( ) ) {
		if ( !_camera_rotation_capturing ) {
			_camera_rotation_capturing = true;
			_camera_rotation_origin = mouse->getPos( );
		}
		Vector2 pos = mouse->getPos( );
		Vector2 diff = pos - _camera_rotation_origin;

		{

		}
	} else {
		_camera_rotation_capturing = false;
	}
	
	int wheel_value = mouse->getWheelRotValue( );
	if ( wheel_value != 0 ) {
		double length = _camera_pos.getLength( );
		length += wheel_value / 1.0;
		if ( length < 0 ) {
			length = 0;
		}
		_camera_pos = _camera_pos.normalize( ) * length;
	}
}

void Drawer3D::drawGuide( ) {
	DrawerPtr drawer = Task::get< Drawer >( );

	drawer->setCamera( _camera_pos, _camera_pos + _camera_dir );

	// xŽ²
	Vector guide_x( _guide_length.x, 0, 0 );
	drawer->drawLine( guide_x * -1, guide_x, Color( 1.0, 0, 0 ) );
	// yŽ²
	Vector guide_y( 0, _guide_length.y, 0 );
	drawer->drawLine( guide_y * -1, guide_y, Color( 0.0, 0, 1.0 ) );
	// zŽ²
	Vector guide_z( 0, 0, _guide_length.z );
	drawer->drawLine( guide_z * -1, guide_z, Color( 0.0, 1.0, 0.0 ) );
}

void Drawer3D::drawSphere( std::string name, const Vector& pos, double radius ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	drawer->drawSphere( pos, radius, 12, Color( 1, 1, 1 ), false );

	Vector2 screen_pos = drawer->getScreenPos( pos );
	drawer->drawString( ( int )screen_pos.x, ( int )screen_pos.y, name.c_str( ) );
}

void Drawer3D::drawLine( std::string name, const Vector& pos1, const Vector& pos2 ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	drawer->drawLine( pos1, pos2, Color( 1, 1, 1 ) );

	Vector2 screen_pos = drawer->getScreenPos( pos1 );
	drawer->drawString( ( int )screen_pos.x, ( int )screen_pos.y, name.c_str( ) );
}
