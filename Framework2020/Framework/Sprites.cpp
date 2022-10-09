#include "Sprites.h"
#include "Image.h"
#include "Helper.h"
#include "Exception.h"
#include "Application.h"

const int INDEX[ 6 ] = { 0, 1, 2, 2, 1, 3 };

Sprites::Sprites( int count, ImagePtr image, int num ) :
COUNT( count ),
NUM( num ),
_image( image ),
_count( 0 ) {
	_vertex = ( void * )( new VERTEX2D[ COUNT * 6 ] );
	_width  = _image->getWidth( );
	_height = _image->getHeight( );
	_pitch  = _width / num;
	
	VERTEX2D *vertex = ( VERTEX2D * )_vertex;
	for ( int i = 0; i < COUNT * 6; i++ ) {
		vertex[ i ].pos.x = 0;
		vertex[ i ].pos.y = 0;
		vertex[ i ].pos.z = 0;
		vertex[ i ].rhw = 1.0f;
		vertex[ i ].dif = GetColorU8( 255, 255, 255, 255 );
	}
	for ( int j = 0; j < COUNT; j++ ) {
		for ( int i = 0; i < 6; i++ ) {
			vertex[ j * 6 + i ].u = ( float )( INDEX[ i ] % 2 );
			vertex[ j * 6 + i ].v = ( float )( INDEX[ i ] / 2 ); 
		}
	}
}

Sprites::~Sprites( ) {
	delete _vertex;
}

void Sprites::showOff( int idx ) {
	VERTEX2D *vertex = ( VERTEX2D * )_vertex;
	for ( int i = 0; i < 6; i++ ) {
		vertex[ idx * 6 + i ].pos.x = 0;
		vertex[ idx * 6 + i ].pos.y = 0;
	}
}

void Sprites::clear( ) {
	_count = 0;
}

void Sprites::add( const Vector& pos ) {
	if ( _count >= COUNT ) {
		return;
	}

	double hp = _pitch / 2;
	Vector vec[ 4 ] = {
		Vector( -hp, -hp ),
		Vector(  hp, -hp ),
		Vector( -hp,  hp ),
		Vector(  hp,  hp ),
	};

	VECTOR p[ 4 ];  
	for ( int i = 0; i < 4; i++ ) {
		p[ i ] = Helper::convVectorToDxlib( pos + vec[ i ] );
	}

	VERTEX2D *vertex = ( VERTEX2D * )_vertex;
	for ( int i = 0; i < 6; i++ ) {
		vertex[ _count * 6 + i ].pos = p[ INDEX[ i ] ];
	}

	_count++;
}

void Sprites::add( const Vector& pos, double scale ) {
	if ( _count >= COUNT ) {
		return;
	}

	double hp = _pitch * scale / 2;
	Vector vec[ 4 ] = {
		Vector( -hp, -hp ),
		Vector(  hp, -hp ),
		Vector( -hp,  hp ),
		Vector(  hp,  hp ),
	};

	VECTOR p[ 4 ];  
	for ( int i = 0; i < 4; i++ ) {
		p[ i ] = Helper::convVectorToDxlib( pos + vec[ i ] );
	}

	VERTEX2D *vertex = ( VERTEX2D * )_vertex;
	for ( int i = 0; i < 6; i++ ) {
		vertex[ _count * 6 + i ].pos = p[ INDEX[ i ] ];
	}

	_count++;
}

void Sprites::add( const Vector& pos, double scale, int anime ) {
	if ( _count >= COUNT ) {
		return;
	}

	double hp = _pitch * scale / 2;
	Vector vec[ 4 ] = {
		Vector( -hp, -hp ),
		Vector(  hp, -hp ),
		Vector( -hp,  hp ),
		Vector(  hp,  hp ),
	};
	float u[ 2 ] = {
		( float )( ( ( anime % NUM + 0 ) * _pitch + 0.5 ) / _width  ),
		( float )( ( ( anime % NUM + 1 ) * _pitch - 0.5 ) / _width  ),
	};
	float v[ 2 ] = {
		( float )( ( ( anime / NUM + 0 ) * _pitch + 0.5 ) / _height  ),
		( float )( ( ( anime / NUM + 1 ) * _pitch - 0.5 ) / _height  ),
	};

	VECTOR p[ 4 ];  
	for ( int i = 0; i < 4; i++ ) {
		p[ i ] = Helper::convVectorToDxlib( pos + vec[ i ] );
	}

	VERTEX2D *vertex = ( VERTEX2D * )_vertex;
	for ( int i = 0; i < 6; i++ ) {
		vertex[ _count * 6 + i ].pos = p[ INDEX[ i ] ];
		vertex[ _count * 6 + i ].u = u[ INDEX[ i ] % 2 ];
		vertex[ _count * 6 + i ].v = v[ INDEX[ i ] / 2 ]; 
	}

	_count++;
}

void Sprites::add( const Vector& pos, double scale, int anime, double angle ) {
	if ( _count >= COUNT ) {
		return;
	}

	double hp = _pitch * scale / 2;
	Vector vec[ 4 ] = {
		Vector( -hp, -hp ),
		Vector(  hp, -hp ),
		Vector( -hp,  hp ),
		Vector(  hp,  hp ),
	};
	float u[ 2 ] = {
		( float )( ( ( anime % NUM + 0 ) * _pitch + 0.5 ) / _width  ),
		( float )( ( ( anime % NUM + 1 ) * _pitch - 0.5 ) / _width  ),
	};
	float v[ 2 ] = {
		( float )( ( ( anime / NUM + 0 ) * _pitch + 0.5 ) / _height  ),
		( float )( ( ( anime / NUM + 1 ) * _pitch - 0.5 ) / _height  ),
	};

	Matrix mat_rot = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), angle );
	VECTOR p[ 4 ];  
	for ( int i = 0; i < 4; i++ ) {
		p[ i ] = Helper::convVectorToDxlib( pos + mat_rot.multiply( vec[ i ] ) );
	}

	const int index[ 6 ] = { 0, 1, 2, 2, 1, 3 };
	VERTEX2D *vertex = ( VERTEX2D * )_vertex;
	for ( int i = 0; i < 6; i++ ) {
		vertex[ _count * 6 + i ].pos = p[ index[ i ] ];
		vertex[ _count * 6 + i ].u = u[ index[ i ] % 2 ];
		vertex[ _count * 6 + i ].v = v[ index[ i ] / 2 ]; 
	}

	_count++;
}

void Sprites::addPrimitive( const Primitive& primitive ) {
	if ( _count >= COUNT ) {
		return;
	}

	const int index[ 6 ] = { 0, 1, 2, 2, 1, 3 };
	VERTEX2D *vertex = ( VERTEX2D * )_vertex;
	for ( int i = 0; i < 6; i++ ) {
		vertex[ _count * 6 + i ].pos = Helper::convVectorToDxlib( primitive.vertex[ index[ i ] ].pos );
		vertex[ _count * 6 + i ].dif = GetColorU8(
			( int )( primitive.vertex[ index[ i ] ].color.r * 255 ), 
			( int )( primitive.vertex[ index[ i ] ].color.g * 255 ), 
			( int )( primitive.vertex[ index[ i ] ].color.b * 255 ), 
			( int )( primitive.vertex[ index[ i ] ].color.a * 255 ) );
		vertex[ _count * 6 + i ].u = ( float )primitive.vertex[ index[ i ] ].u;
		vertex[ _count * 6 + i ].v = ( float )primitive.vertex[ index[ i ] ].v; 
	}

	_count++;
}

void Sprites::draw( ) {
	DrawPrimitive2D( ( VERTEX2D * )_vertex, _count * 6, DX_PRIMTYPE_TRIANGLELIST, _image->getHandle( ), TRUE );
}

void Sprites::draw( int count ) {
	DrawPrimitive2D( ( VERTEX2D * )_vertex, count * 6, DX_PRIMTYPE_TRIANGLELIST, _image->getHandle( ), TRUE );
}
