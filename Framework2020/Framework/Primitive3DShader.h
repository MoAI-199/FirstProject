#pragma once
#include "smart_ptr.h"
#include "Primitive.h"
#include "mathematics.h"
#include "DxLib.h"

PTR( Primitive )
PTR( Primitive3DShader )
PTR( Primitive3DShaderImpl )
PTR( PixelShader )
PTR( VertexShader )

class Primitive3DShader {
public:
	Primitive3DShader( );
	virtual ~Primitive3DShader( );
public:
	void draw( PrimitivePtr primitive );
	void set( const int& i, Primitive::VERTEX& vertex );
	void alloc( const int& num );
	void setVSConstSF( int idx, float param );
	void setSpos( Quaternion qtn );
	void setScreenW( int screen_width );
	void setScreenH( int screen_height );
	VERTEX3DSHADER getVTX3D( const int& i );
	void setVTX3D( const int& i,VERTEX3DSHADER vtx3d );
private:
	Primitive3DShaderImplPtr _impl;
	Quaternion _spos;
	int _screen_width;
	int _screen_height;
};

