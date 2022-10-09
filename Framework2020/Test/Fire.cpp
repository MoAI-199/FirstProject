#include "Fire.h"
#include "Drawer.h"
#include "Billboard.h"
#include <string>


Fire::Fire( const Vector& pos ) {
	createFire( pos );
}

Fire::~Fire( ) {
}

void Fire::draw( ) {
	for ( int i = 0; i < _fire.size( ); i++ ) {
		_fire[ i ]->draw( );
	}
}

void Fire::createFire( Vector pos ) {
	using namespace std;
	DrawerPtr drawer = Task::get< Drawer >( );
	for ( int i = 0; i < 5; i++ ) {
		string num = to_string( i );
		string fileName = "fire/fire_0" + num + ".png";
		BillboardPtr billboard = drawer->createBillboard( fileName.c_str( ) );
		billboard->setPos( pos );
		billboard->setImageCenterPos( 0.5, 0.5 );
		billboard->setTrans( true );
		billboard->setImageSize( 2 );
		_fire.push_back( billboard );
	}
}
