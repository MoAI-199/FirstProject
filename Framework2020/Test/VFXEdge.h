#pragma once
#include "smart_ptr.h"

PTR( ImageTarget );
PTR( Primitive2DShader );
PTR( VFXEdge );
PTR( Model );
PTR( PixelShader );

class VFXEdge {
	friend class VFXEdgeScope;
public:
	VFXEdge( ModelPtr model );
	virtual ~VFXEdge( );
public:
	void draw( );
private:
	void set( int width, int height );
private:
	ModelPtr _model;
	PixelShaderPtr _shader_single_color;
	PixelShaderPtr _shader_laplacian_filter;
	ImageTargetPtr _screen_single_color;
	ImageTargetPtr _screen_laplacian_filter;
	Primitive2DShaderPtr _primitive_2d_shader;
};

