#include "Primitive3DShader.h"
#include "DxLib.h"
#include "PixelShader.h"
#include "VertexShader.h"

class Primitive3DShaderImpl {
public:
	VERTEX3DSHADER* _vertex3d;
};

Primitive3DShader::Primitive3DShader( ) :
_screen_width( 0 ),
_screen_height( 0 ){
	_impl = Primitive3DShaderImplPtr( new Primitive3DShaderImpl );
	_impl->_vertex3d = NULL;
}

Primitive3DShader::~Primitive3DShader( ) {

}

void Primitive3DShader::draw( PrimitivePtr primitive ) {
	//　描画
	DrawPolygon3DToShader( _impl->_vertex3d, primitive->getNum( ) );
	// シェーダーファイルのリセット
	SetUseVertexShader( -1 );
	SetUsePixelShader( -1 );
}

void Primitive3DShader::set( const int& i, Primitive::VERTEX& vertex ) {
	VERTEX3DSHADER vtx;
	vtx.pos  = VGet( 
		( float )vertex.pos.x, 
		( float )vertex.pos.y,
		( float )vertex.pos.z );
	vtx.norm = VGet( 
		( float )vertex.norm.x,
		( float )vertex.norm.y,
		( float )vertex.norm.z );
	vtx.dif  = GetColorU8( 
	    ( int )(vertex.color.r * 255 ),
		( int )(vertex.color.g * 255 ),
		( int )(vertex.color.b * 255 ),
		( int )(vertex.color.a * 255 ) );
	if ( _screen_width ) {
		vtx.spos.x = ( float )( _spos.x / _screen_width );
		vtx.spos.y = ( float )( _spos.y / _screen_height );
		vtx.spos.z = ( float )_spos.z;
		vtx.spos.w = ( float )_spos.w;
	}
	vtx.spc  = GetColorU8( 0, 0, 0, 0 );
	vtx.u    = ( float )vertex.u;
	vtx.v    = ( float )vertex.v;
	vtx.su   = 0.0f;
	vtx.sv   = 0.0f;
	_impl->_vertex3d[ i ] = vtx;
}

void Primitive3DShader::alloc( const int& num ) {
	if ( _impl->_vertex3d ) {
		delete[ ] _impl->_vertex3d;
	}
	_impl->_vertex3d = new VERTEX3DSHADER[ num * 3 ];
}

void Primitive3DShader::setVSConstSF( int idx, float param ) {
	SetVSConstSF( idx, param );
}

void Primitive3DShader::setSpos( Quaternion qtn ) {
	_spos.x = qtn.x;
	_spos.y = qtn.y;
	_spos.z = qtn.z;
	_spos.w = qtn.w;
}

void Primitive3DShader::setScreenW( int screen_width ) {
	_screen_width = screen_width;
}

void Primitive3DShader::setScreenH( int screen_height ) {
	_screen_height = screen_height;
}

//強制転換ミスがあり、直接＝方がいい
VERTEX3DSHADER Primitive3DShader::getVTX3D( const int& i ) {
	return _impl->_vertex3d[ i ];
}

void Primitive3DShader::setVTX3D( const int& i, VERTEX3DSHADER vtx3d ) {
	_impl->_vertex3d[ i ] = vtx3d;
}