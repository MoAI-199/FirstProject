#pragma once
#include "smart_ptr.h"
#include "Primitive.h"

PTR( Primitive2DShader );
PTR( Primitive2DShaderImpl );
PTR( PixelShader );

class Primitive2DShader {
public:
	Primitive2DShader( );
	virtual ~Primitive2DShader( );
public:
	void set( const int& i, Primitive::VERTEX vertex );
	void alloc( const int& num );
	void draw( PixelShaderPtr shader );
	void drawUsPolygon2DToShader( int idx );
private:
	Primitive2DShaderImplPtr _impl;
};

