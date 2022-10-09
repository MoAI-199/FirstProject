#include "VFXFirePillar.h"
#include "Drawer.h"
#include "Billboard.h"

const char* FILE_NAME = "fire/test_fire3.png";
const double RADIUS = 10;

VFXFirePillar::VFXFirePillar( Vector pos ) :
	_center( pos ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	for ( int i = 0; i < NUM_OF_FIRE; i++ ) {
		_billboard[ i ] = drawer->createBillboard( FILE_NAME );
		_billboard[ i ]->setImageCenterPos( 0.5, 0.5 );
		_billboard[ i ]->setTrans( true );
		_billboard[ i ]->setImageSize( 2.5 );
	}
}

VFXFirePillar::~VFXFirePillar( ) {
}

void VFXFirePillar::draw( ) {
	drawFire( RADIUS );
	drawFire( RADIUS - 2.0 );
}

void VFXFirePillar::fireElement( double radius, double height ) {
	//_billboard->setPos( _center );
	Vector2 vtr2 = Vector2( _center.x + radius, _center.z );
	double angle = 360 / NUM_OF_FIRE;
	double y = _center.y + height;
	for ( int i = 0; i < NUM_OF_FIRE; i++ ) {
		vtr2 = vtr2.rotate( angle );
		Vector vtr = Vector( vtr2.x, y, vtr2.y );
		_billboard[ i ]->setPos( vtr );
	}
}

void VFXFirePillar::drawFire( double raidus ) {
	double rad = raidus;
	double num = 1.0;
	for ( int j = 0; j < HEIGHT_OF_FIRE; j++ ) {
		if ( 1 < j && j < 7 ) {
			rad -= num;
			num -= 0.1;
		} else if ( 8 < j && j < 14 ) {
			rad += num;
			num += 0.1;
		}
		fireElement( rad, j * 1.5 );
		for ( int i = 0; i < NUM_OF_FIRE; i++ ) {
			_billboard[ i ]->draw( );
		}
	}
}
