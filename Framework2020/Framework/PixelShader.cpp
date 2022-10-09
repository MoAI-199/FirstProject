#include "PixelShader.h"
#include "DxLib.h"
#include "Exception.h"
#include "Application.h"
#include "ImageTarget.h"

PixelShader::PixelShader( const char* filename ) {
	ApplicationPtr app = Application::getInstance( );
	EXCEPTION( !app->isEnabledD3D9( ), "only Direct3D9" );
	_idx = LoadPixelShader( filename );
	EXCEPTION( _idx == -1, "Cant load Pixel Shader File" );
}

void PixelShader::usePixelShaderBegin( ) {
	// シェーダーを有効にする
	MV1SetUseOrigShader( TRUE );
	usePixelShader( );
}

void PixelShader::usePixelShader( ) {
	SetUsePixelShader( _idx );
}

void PixelShader::usePixelShaderEnd( ) {
	// シェーダーのリセット
	noUseOrigShader( );
	SetUseVertexShader( -1 );
	SetUsePixelShader( -1 );
}

void PixelShader::noUseOrigShader( ) {
	MV1SetUseOrigShader( FALSE );
}

PixelShader::~PixelShader( ) {
	DeleteShader( _idx );
}

void PixelShader::setUseTexture( int idx, ImageConstPtr image ) {
	SetUseTextureToShader( idx, image->getHandle( ) );
}

void PixelShader::setColorToPSConstF( int idx, Color color ) {
	SetPSConstF( idx, F4Get( ( float )color.r, ( float )color.g, ( float )color.b, ( float )color.a ) );
}

void PixelShader::setPSConstF( int idx, Quaternion qtn ) {
	FLOAT4 f;
	f.x = ( float )qtn.x;
	f.y = ( float )qtn.y;
	f.z = ( float )qtn.z;
	f.w = ( float )qtn.w;

	SetPSConstF( idx, f );
}

void PixelShader::setPSConstSF( int idx, float param ) {
	SetPSConstSF( idx, param );
}