#include "OkutyScene.h"
#include "Drawer.h"
#include "Billboard.h"

SCENE( OkutyScene );

const double RADIUS = 2.0;
const double ROT_SPEED = 0.2;
const double TAIL_ROT = 0.05;
const double TAIL_HEIGHT = 0.1;
const int FIRE_NUM = 6;
const int TAIL_NUM = 50;
const double RISE_SPEED = 0.3;
const double HEIGHT = 7.0;

OkutyScene::OkutyScene( ) {
}

OkutyScene::~OkutyScene( ) {
}

void OkutyScene::initialize( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_billboard = drawer->createBillboard( "fire/test_fire2.png" );
	_billboard->setImageCenterPos( 0.5, 0.5 );
	drawer->setCamera( Vector( 0, 30, 40 ), Vector( 0, 5.0, 0 ) );
	_rot = 0;
	_height = 0;
}

void OkutyScene::update( ) {
	_rot += ROT_SPEED;
	_height += RISE_SPEED;
	if ( _height > HEIGHT ) {
		_height = 0;
	}

	for ( int i = 0; i < FIRE_NUM; i++ ) {
		drawFire( i );
	}
}

void OkutyScene::drawFire( int idx ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	drawer->disableZBufferWriting( scope, false );

	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), _rot + PI2 * idx / FIRE_NUM );
	Vector pos = mat.multiply( Vector( RADIUS , _height, 0 ) );
	for ( int i = 0; i < TAIL_NUM; i++ ) {
		drawer->setBlend( scope, Drawer::BLEND::ADD, ( double )i / TAIL_NUM * ( 1.0 - _height / HEIGHT ) );
		Matrix mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), TAIL_ROT );
		pos = mat.multiply( pos );
		pos.y += TAIL_HEIGHT;
		_billboard->setPos( pos );
		_billboard->draw( );
	}
}
