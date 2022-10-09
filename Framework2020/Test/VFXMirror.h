#pragma once
#include "smart_ptr.h"
#include "mathematics.h"

PTR( PixelShader );
PTR( VertexShader );
PTR( ImageTarget );
PTR( Primitive3DShader );

class VFXMirror {
public:
	VFXMirror( );
	virtual ~VFXMirror( );
public:
	void draw( const Vector eye, const Vector focus );
	void setPos( Vector pos, int width, int depth );
	void drawBegin( const Vector eye, const Vector focus );
	void drawEnd( );
private:
	void setupCamera( Vector CameraEyePos, Vector CameraTargetPos );
	void render( );
private:
	VertexShaderPtr _vertex_shader;
	PixelShaderPtr _pixel_shader;
	Primitive3DShaderPtr _primitive_3d_shader;
	ImageTargetPtr _image_target;
	Vector _world_pos[ 4 ];
	Quaternion _screen_pos[ 4 ];
	int _width;
	int _height;
	Vector _camera_pos;
	Vector _camera_target;
};

