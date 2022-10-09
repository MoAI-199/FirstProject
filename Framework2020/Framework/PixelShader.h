#pragma once
#include "smart_ptr.h"
#include "Color.h"
#include "mathematics.h"

PTR( PixelShader );
PTR( Image );
PTR( Primitive );

class PixelShader {
	friend class Drawer;
	friend class Model;
	friend class Primitive2DShader;
public:
	virtual ~PixelShader( );
protected:
	PixelShader( const char* filename );
public:
	void usePixelShaderBegin( );
	void usePixelShader( );
	void usePixelShaderEnd( );
	void noUseOrigShader( );
	void setUseTexture( int idx, ImageConstPtr image );
	void setColorToPSConstF( int constantIndex, Color color );
	void setPSConstF( int idx, Quaternion qtn );
	void setPSConstSF( int constantIndex, float  param );
private:
	int _idx;
};

