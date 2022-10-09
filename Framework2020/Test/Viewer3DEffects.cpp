#include "Viewer3DEffects.h"
#include "Billboard.h"
#include "Drawer.h"
#include <vector>

class Viewer3DEffectPrimitive {
public:
	Viewer3DEffectPrimitive( Viewer3DEffects::Effect data ) {
		_count = 0;
		_data = data;
	}
	virtual ~Viewer3DEffectPrimitive( ) {

	}
public:
	void update( ) {
		_count++;
	}
	Viewer3DEffects::Effect getData( ) const {
		return _data;
	}
	int getCount( ) const {
		return _count;
	}
private:
	int _count;
	Viewer3DEffects::Effect _data;
};

class Viewer3DEffectAnimation {
public:
	Viewer3DEffectAnimation( ) {

	}
	virtual ~Viewer3DEffectAnimation( ) {

	}
	void add( std::vector< BillboardPtr > billboard ) {
		_billboard.push_back( billboard );
	}
	int getMaxCount( ) const {
		return ( int )_billboard.size( );
	}
	void draw3DEffect( int count, const Viewer3DEffects::Effect& data, const Vector& pos ) {
		DrawerPtr drawer = Task::get< Drawer >( );
		Drawer::Scope scope = drawer->createScope( );
		Vector2 vtr2 = Vector2( pos.x, pos.z );
		double angle = 360 / 64;
		double y = pos.y;
		for ( int i = 0; i < _billboard[ count ].size( ); i++ ) {
			vtr2 = vtr2.rotate( angle );
			Vector vtr = Vector( vtr2.x, y, vtr2.y );
			_billboard[ count ][ i ]->setTrans( true );
			_billboard[ count ][ i ]->setImageCenterPos( 0.5, 0.5 );
			_billboard[ count ][ i ]->setImageSize( 2 );
			_billboard[ count ][ i ]->setPos( vtr );
			_billboard[ count ][ i ]->draw( );
		}
		drawer->setBlend( scope, Drawer::BLEND::NONE, 1.0 );
	}
private:
	std::vector< std::vector< BillboardPtr > > _billboard;
};

Viewer3DEffects::Viewer3DEffects( ) {
	createEffects( );
}

Viewer3DEffects::~Viewer3DEffects( ) {

}

void Viewer3DEffects::draw( ) {
	addEffects( );
	updateEffects( );
}

void Viewer3DEffects::addEffects( ) {
	int num = 1;
	for ( int i = 0; i < num; i++ ) {
		Viewer3DEffectPrimitivePtr prim( new Viewer3DEffectPrimitive( Viewer3DEffects::Effect::fire ) );
		_primitive.push_back( prim );
	}
}

void Viewer3DEffects::updateEffects( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	drawer->setBlend( scope, Drawer::BLEND::ADD, 1.0 );
	drawer->disableZBufferUsing( scope, false );

	std::list< Viewer3DEffectPrimitivePtr >::iterator iter = _primitive.begin( );
	while ( iter != _primitive.end( ) ) {
		Viewer3DEffectPrimitivePtr target = *iter;
		target->update( );

	}
}

void Viewer3DEffects::createEffects( ) {

}

bool Viewer3DEffects::drawEffects( int count ) {
	return false;
}
