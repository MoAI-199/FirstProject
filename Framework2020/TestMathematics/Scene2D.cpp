#include "Scene2D.h"
#include "Drawer2D.h"
#include "Log.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Application.h"

SCENE( Scene2D );

Scene2D::Scene2D( ) {
	_dir = Vector2( 10, 10 );

}

Scene2D::~Scene2D( ) {

}

void Scene2D::initialize( ) {
	Task::create< Drawer2D >( );

	LogPtr log = Task::get< Log >( );
	log->toggleShow( );
}

void Scene2D::update( ) {

	//Matrix2 mat = Matrix2::makeTransformRotation( 0.01 );
	//_dir = mat * _dir;

	//Mat = rot * scale * trans;

	KeyboardPtr keyboard = Task::get< Keyboard >( );
	Drawer2DPtr drawer2D = Task::get< Drawer2D >( );
	MousePtr mouse = Task::get< Mouse >( );
	ApplicationPtr app = Application::getInstance( );
	int width = app->getScreenWidth( );
	int height = app->getScreenHeight( );
	Vector2 mp = mouse->getPos( );
	mp += Vector2( -width / 2, -height / 2 );
	mp.y *= -1;
	mp *= 0.1;
	drawer2D->drawLine( "mouse", mp, Vector2( ) );
	drawer2D->drawLine( "dir", _dir, Vector2( ) );

	double n = _dir.cross( mp );

	LogPtr log = Task::get< Log >( );
	log->status( "dir * mouse : %lf", n );

	double rot = 0.01;
	if ( n < 0 ) {
		rot *= -1;
	}
	_dir = _dir.rotate( rot );

	if ( keyboard->isPushKey( "F1" ) ) {
		loadScene( "Scene3D" );
	}
}
