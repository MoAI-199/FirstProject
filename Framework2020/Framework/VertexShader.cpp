#include "VertexShader.h"
#include "DxLib.h"
#include "Exception.h"
#include "Application.h"

VertexShader::VertexShader( const char* filename ) {
	ApplicationPtr app = Application::getInstance( );
	EXCEPTION( !app->isEnabledD3D9( ), "only Direct3D9" );
	_idx = LoadVertexShader( filename );
	EXCEPTION( _idx == -1, "Cant load Pixel Shader File" );
}

VertexShader::~VertexShader( ) {
	DeleteShader( _idx );
}

void VertexShader::useVertexShader( ) {
	SetUseVertexShader( _idx );
}
