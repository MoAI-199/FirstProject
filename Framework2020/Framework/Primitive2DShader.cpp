#include "Primitive2DShader.h"
#include "DxLib.h"
#include "mathematics.h"
#include "PixelShader.h"

class Primitive2DShaderImpl {
public:
	VERTEX2DSHADER* _vertex2d;
};

Primitive2DShader::Primitive2DShader( ) {
	_impl = Primitive2DShaderImplPtr( new Primitive2DShaderImpl );
	_impl->_vertex2d = NULL;
	//	// 頂点位置
	//	Vector pos[ 4 ] = {
	//		Vector(		0,		0 ),
	//		Vector(	width,		0 ),
	//		Vector(		0, height ),
	//		Vector(	width, height ),
	//	};
	//	
	//	// ポリゴンの形成
	//	Vector vertex[ 6 ];
	//	vertex[ 0 ] = pos[ 0 ];
	//	vertex[ 1 ] = pos[ 1 ];
	//	vertex[ 2 ] = pos[ 2 ];
	//	
	//	vertex[ 3 ] = pos[ 1 ];
	//	vertex[ 4 ] = pos[ 3 ];
	//	vertex[ 5 ] = pos[ 2 ];
	//	
	//	// 頂点データのセット
	//	int u[ 6 ] = { 0, 1, 0, 1, 1, 0 };
	//	int v[ 6 ] = { 0, 0, 1, 0, 1, 1 };
}

Primitive2DShader::~Primitive2DShader( ) {

}

void Primitive2DShader::set( const int& i, Primitive::VERTEX vertex ) {
	VERTEX2DSHADER vtx;
	vtx.pos = VGet( ( float )vertex.pos.x, ( float )vertex.pos.y, ( float )vertex.pos.z );
	vtx.dif = GetColorU8(
		( int )( vertex.color.r * 255 ),
		( int )( vertex.color.g * 255 ),
		( int )( vertex.color.b * 255 ),
		( int )( vertex.color.a * 255 ) );
	vtx.spc = GetColorU8( 0, 0, 0, 0 );
	vtx.u = ( float )vertex.u;
	vtx.v = ( float )vertex.v;
	vtx.su = 0.0f;
	vtx.sv = 0.0f;
	vtx.rhw = 1.0f;
	_impl->_vertex2d[ i ] = vtx;
}

void Primitive2DShader::alloc( const int& num ) {
	if ( _impl->_vertex2d ) {
		delete[ ] _impl->_vertex2d;
	}
	_impl->_vertex2d = new VERTEX2DSHADER[ num * 3 ];
}

void Primitive2DShader::draw( PixelShaderPtr shader ) {
	SetUsePixelShader( shader->_idx );

	DrawPrimitive2DToShader( _impl->_vertex2d, 6, DX_PRIMTYPE_TRIANGLELIST );

	MV1SetUseOrigShader( FALSE );
	SetUsePixelShader( -1 );
}

void Primitive2DShader::drawUsPolygon2DToShader( int idx ) {
	DrawPolygon2DToShader( _impl->_vertex2d, idx );
}
