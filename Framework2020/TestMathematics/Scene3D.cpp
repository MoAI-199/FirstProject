#include "Scene3D.h"
#include "Drawer3D.h"
#include "Log.h"
#include "Keyboard.h"

SCENE( Scene3D );

Scene3D::Scene3D( ) {
	_dir = Vector( 2, 0, 0 );
}

Scene3D::~Scene3D( ) {

}

void Scene3D::initialize( ) {
	Task::create< Drawer3D >( );

	LogPtr log = Task::get< Log >( );
	log->toggleShow( );
}

void Scene3D::update( ) {
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	Drawer3DPtr drawer3D = Task::get< Drawer3D >( );

	if ( keyboard->isHoldKey( "ARROW_LEFT" ) ) _p.x += -1;
	if ( keyboard->isHoldKey( "ARROW_RIGHT" ) ) _p.x += +1;
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) _p.y += -1;
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) _p.y += +1;

	Vector p1( 0, 0, 0 );
	Vector p2( 1, 1, 0 );
	Vector p3( 1, 0, 1 );

	drawer3D->drawSphere( "p\'", _p, 0.1 );
	drawer3D->drawLine( "p1", p1, p2 );
	drawer3D->drawLine( "p2", p2, p3 );
	drawer3D->drawLine( "p3", p3, p1 );

	// _dir = _dir.rotate( 0.01 );
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), 0.01 );

	_dir = mat * _dir;

	drawer3D->drawLine( "dir", _dir, Vector() );

	if ( keyboard->isPushKey( "F1" ) ) {
		loadScene( "Scene2D" );
	}
}
