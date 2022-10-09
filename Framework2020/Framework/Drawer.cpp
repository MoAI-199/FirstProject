#include "Drawer.h"
#include "Application.h"
#include "DxLib.h"
#include "Image.h"
#include "Movie.h"
#include "ImageTarget.h"
#include "Billboard.h"
#include "Helper.h"
#include "SoftImage.h"
#include "Sprites.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "Exception.h"
#include "Primitive.h"
#include "Primitive2DShader.h"
#include "Primitive3DShader.h"
#include "Graph.h"
#include <assert.h>

PTR( Primitive );

DxLib::VECTOR convVector( const Vector& pos ) {
	return VGet( float( pos.x ), float( pos.y ), float( pos.z ) );
}

unsigned int convColorValue( const Color& color ) {
	return GetColor(
		( int )( color.r * 255 ),
		( int )( color.g * 255 ),
		( int )( color.b * 255 ) );
}

Drawer::Scope::Scope( ) {
	_on_blend = false;
	_on_bright = false;
	_on_target = false;
	_on_write_z_buffer = false;
	_on_use_z_buffer = false;
}

void Drawer::Scope::onBlend( ) {
	_on_blend = true;
}

void Drawer::Scope::onBright( ) {
	_on_bright = true;
}

void Drawer::Scope::onTarget( ) {
	_on_target = true;
}

void Drawer::Scope::onWriteZBuffer( ) {
	_on_write_z_buffer = true;
}

void Drawer::Scope::onUseZBuffer( ) {
	_on_use_z_buffer = true;
}

Drawer::Scope::~Scope( ) {
	if ( _on_write_z_buffer ) {
		DxLib::SetWriteZBuffer3D( TRUE );
	}
	if ( _on_use_z_buffer ) {
		DxLib::SetUseZBuffer3D( TRUE );
	}

	if ( _on_blend ) {
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
	}
	if ( _on_bright ) {
		SetDrawBright( 255, 255, 255 );
	}
	if ( _on_target ) {
		SetDrawScreen( DX_SCREEN_BACK );
		DrawerPtr drawer = Task::get< Drawer >( );
		drawer->initCameraSetting( );
	}
}

Drawer::Drawer( ) {
	ApplicationPtr app = Application::getInstance( );
	_fov = 12.0 * DX_PI_F / 180.0;
	_near = 1.6;
	_far = 200.0;
	_camera_up = Vector( 0, 1, 0 );
	_camera_pos = Vector( 0, 0, -10 );
	_camera_target = Vector( );
	_screen_width = app->getScreenWidth( );
	_screen_height = app->getScreenHeight( );

	initCameraSetting( );
}

Drawer::Scope Drawer::createScope( ) const {
	return Scope( );
}

Drawer::~Drawer( ) {
}

void Drawer::update( ) {
}

void Drawer::drawCircle( int x, int y, const double radius, const Color& color, bool fill ) const {
	DrawCircle( x, y, ( int )radius, convColorValue( color ), fill ? 1 : 0 );
}

void Drawer::drawSphere( const Vector& pos, const double radius, int polygon_num, const Color& color, bool fill ) const {
	DrawSphere3D( convVector( pos ), ( float )radius, polygon_num, convColorValue( color ), 0, fill ? 1 : 0 );
}

ImagePtr Drawer::createImage( const char* filename ) const {
	ImagePtr result;
	std::string path = filename;
	ImagePtr image = ImagePtr( new Image );
	if ( image->load( path ) ) {
		result = image;
	}
	if ( !result ) {
		path = "../" + path;
		if ( image->load( path ) ) {
			result = image;
		}
	}

#ifdef _DEBUG
	EXCEPTION( !result, "ERROR[Drawer::createImage] fail loaded file" );
	return result;
#else
	return image;
#endif
}

SoftImagePtr Drawer::createSoftImage( const char* filename ) const {
	SoftImagePtr result;
	std::string path = filename;
	SoftImagePtr soft_image = SoftImagePtr( new SoftImage );
	if ( soft_image->load( path ) ) {
		result = soft_image;
	}
	if ( !result ) {
		path = "../" + path;
		if ( soft_image->load( path ) ) {
			result = soft_image;
		}
	}

	return result;
}

ImagePtr Drawer::createImageFromSoftImage( SoftImagePtr soft_image ) const {
	ImagePtr result = ImagePtr( );

	ImagePtr image = ImagePtr( new Image );
	int handle = CreateGraphFromSoftImage( soft_image->_handle );
	if ( handle >= 0 ) {
		result = image;
	}

	return result;
}

ImageTargetPtr Drawer::createImageTarget( int width, int height ) const {
	ImageTargetPtr image_target = ImageTargetPtr( new ImageTarget );
	image_target->create( width, height );
	return image_target;
}

MoviePtr Drawer::createMovie( const char* filename, bool alpha ) const {
	MoviePtr result = MoviePtr( );
	std::string path = filename;
	MoviePtr movie = MoviePtr( new Movie );
	if ( movie->load( path, alpha ) ) {
		result = movie;
	}
	if ( !result ) {
		path = "../" + path;
		if ( movie->load( path, alpha ) ) {
			result = movie;
		}
	}

	return result;
}

BillboardPtr Drawer::createBillboard( const char* filename ) const {
	BillboardPtr result;
	BillboardPtr billboard = BillboardPtr( new Billboard );

	if ( ImagePtr image = createImage( filename ) ) {
		billboard->setImage( image );
		result = billboard;
	}

#ifdef _DEBUG
	EXCEPTION( !result, "ERROR[Drawer::createBillboard] fail loaded file" );
	return result;
#else
	return billboard;
#endif
}

BillboardPtr Drawer::createBillboard( ) const {
	return BillboardPtr( new Billboard );
}


SpritesPtr Drawer::createSprites( int count, const char* filename, int num ) const {
	ImagePtr image = createImage( filename );
	if ( !image ) {
		return SpritesPtr( );
	}
	return SpritesPtr( new Sprites( count, image, num ) );
}

PixelShaderPtr Drawer::createPixelShader( const char * filename ) const {
	return PixelShaderPtr( new PixelShader( filename ) );
}

VertexShaderPtr Drawer::createVertexShader( const char* filename ) const { 
	return VertexShaderPtr( new VertexShader( filename ) );
}

PrimitivePtr Drawer::createPrimitive( bool buffer ) const {
	return PrimitivePtr( new Primitive( buffer ) );
}

Primitive2DShaderPtr Drawer::createPrimitive2DShader( ) {
	return Primitive2DShaderPtr( new Primitive2DShader( ) );
}

Primitive3DShaderPtr Drawer::createPrimitive3DShader( ) {
	return Primitive3DShaderPtr( new Primitive3DShader( ) );
}

GraphPtr Drawer::createGraph( ) {
	return GraphPtr( new Graph( ) );
}

void Drawer::clearScreen( ) const {
	ClearDrawScreen( );
}

void Drawer::drawLine( int x1, int y1, int x2, int y2, const Color& color ) const {
	DrawLine( x1, y1, x2, y2, convColorValue( color ) );
}

void Drawer::drawLine( const Vector& pos1, const Vector& pos2, const Color& color ) const {
	VECTOR p1 = Helper::convVectorToDxlib( pos1 );
	VECTOR p2 = Helper::convVectorToDxlib( pos2 );
	DrawLine3D( p1, p2, convColorValue( color ) );
}

/// <summary>
/// 四角を描画
/// </summary>
///<param name = "x1"> 四角の左上X座標 < / param>
///<param name = "y1"> 四角の左上Y座標 < / param>
///<param name = "x2"> 四角の右下X座標 < / param>
///<param name = "y2"> 四角の右下Y座標 < / param>
///<param name = "color"> 四角の色 < / param>
///<param name = "fill"> 四角を線だけで描画するかどうか < / param>
void Drawer::drawBox( int x1, int y1, int x2, int y2, const Color& color, bool fill ) const {
	DrawBox( x1, y1, x2, y2, convColorValue( color ), fill ? TRUE : FALSE );
}

void Drawer::drawPixel( int x, int y, const Color& color ) const {
	DrawPixel( x, y, convColorValue( color ) );
}

void Drawer::drawSector( int x, int y, ImageConstPtr image, double ratio ) const {
	DrawCircleGauge( x, y, ratio * 100.0, image->getHandle( ) );
}

void Drawer::setStringSize( int size ) const {
	SetFontSize( size );
}

void Drawer::drawString( int x, int y, const char* string, ... ) const {
	char buf[ 1024 ];
	va_list ap;
	va_start( ap, string );
	vsprintf_s( buf, 1000, string, ap );
	va_end( ap );

	DrawString( x + 1, y + 1, buf, 0x000000 );
	DrawString( x + 0, y + 0, buf, 0xFFFFFF );
}

void Drawer::setCameraUp( const Vector& up ) {
	_camera_up = up;
}

void Drawer::setCamera( const Vector& pos, const Vector& target ) {
	_camera_pos = pos;
	_camera_target = target;
	DxLib::VECTOR dx_pos = Helper::convVectorToDxlib( _camera_pos );
	DxLib::VECTOR dx_target = Helper::convVectorToDxlib( _camera_target );
	DxLib::VECTOR dx_up = Helper::convVectorToDxlib( _camera_up );
	SetCameraPositionAndTargetAndUpVec( dx_pos, dx_target, dx_up );
}

void Drawer::setCamera( const Matrix& view, const Matrix& proj ) {
	DxLib::MATRIX dx_view;
	DxLib::MATRIX dx_proj;
	for ( int i = 0; i < 4; i++ ) {
		for ( int j = 0; j < 4; j++ ) {
			dx_view.m[ i ][ j ] = ( float )view.data[ i ][ j ];
			dx_proj.m[ i ][ j ] = ( float )proj.data[ i ][ j ];
		}
	}
	SetCameraViewMatrix( dx_view );
	SetupCamera_ProjectionMatrix( dx_proj );
}


void Drawer::setCameraCenter( double x, double y ) {
	SetCameraScreenCenter( ( float )x, ( float )y );
}

void Drawer::setCameraPerspective( double fov ) {
	_fov = fov;
	SetupCamera_Perspective( ( float )_fov );
}

void Drawer::setLight( const Vector& pos, const Vector& dir ) {
	_light_pos = pos;
	_light_dir = dir;
	SetLightPosition( Helper::convVectorToDxlib( pos ) );
	SetLightDirection( Helper::convVectorToDxlib( dir ) );
}

bool Drawer::isInCamera( const Vector& pos ) const {
	DxLib::VECTOR dx_pos = Helper::convVectorToDxlib( pos );
	return ( CheckCameraViewClip( dx_pos ) == FALSE ); // TRUE：画面外　FALSE：画面内が返ってくる
}

void Drawer::initCameraSetting( ) const {
	{
		DxLib::VECTOR dx_pos = Helper::convVectorToDxlib( _camera_pos );
		DxLib::VECTOR dx_target = Helper::convVectorToDxlib( _camera_target );
		DxLib::VECTOR dx_up = Helper::convVectorToDxlib( _camera_up );
		SetCameraPositionAndTargetAndUpVec( dx_pos, dx_target, dx_up );
	}
	{
		DxLib::VECTOR dx_pos = Helper::convVectorToDxlib( _light_pos );
		DxLib::VECTOR dx_dir = Helper::convVectorToDxlib( _light_dir );
		SetLightPosition( dx_pos );
		SetLightDirection( dx_dir );
	}

	SetupCamera_Perspective( ( float )_fov );
	SetCameraNearFar( ( float )_near, ( float )_far );
}

void Drawer::setFog( double start, double end, int red, int green, int blue ) {
	SetFogEnable( TRUE );
	SetFogColor( red, green, blue );
	SetFogStartEnd( ( float )start, ( float )end );
}

Vector Drawer::getWorldPos( const Vector& screen_pos ) const {
	VECTOR worldf = ConvScreenPosToWorldPos( Helper::convVectorToDxlib( screen_pos ) );
	return Vector( worldf.x, worldf.y, worldf.z );
}

//スクリーン座標とは無関係の値の場合は原点を返す
Vector2 Drawer::getScreenPos( const Vector& world_pos ) const {
	VECTOR screenf = ConvWorldPosToScreenPos( Helper::convVectorToDxlib( world_pos ) );
	Vector2 result = Vector2( screenf.x, screenf.y );
	if ( screenf.z <= 0.0f || screenf.z >= 1.0f ) {
		result = Vector2( );
	}
	return result;
}

//スクリーン座標とは無関係の値の場合は原点を返す
Quaternion Drawer::getScreenPosPlusW( const Vector& world_pos ) const {
	FLOAT4 f = ConvWorldPosToScreenPosPlusW( Helper::convVectorToDxlib( world_pos ) );
	Quaternion result;
	result.x = ( double )f.x;
	result.y = ( double )f.y;
	result.z = ( double )f.z;
	result.w = ( double )f.w;
	return result;
}

Vector Drawer::getCameraPos( ) const {
	return _camera_pos;
}

Vector Drawer::getCameraTarget( ) const {
	return _camera_target;
}

Vector Drawer::getCameraUp( ) const {
	return _camera_up;
}

void Drawer::setNearFar( double camera_near, double camera_far ) {
	_near = camera_near;
	_far = camera_far;
	SetCameraNearFar( ( float )_near, ( float )_far );
}

Matrix Drawer::createViewMatrix( const Vector& camera_pos, const Matrix& camera_rot ) const {
	Matrix result;
	Vector forward = camera_rot * Vector( 0, 0, 1 );
	Vector camera_up = camera_rot * Vector( 0, 1, 0 );

	DxLib::VECTOR pos = Helper::convVectorToDxlib( camera_pos );
	DxLib::VECTOR focus = Helper::convVectorToDxlib( camera_pos + forward );
	DxLib::VECTOR up = Helper::convVectorToDxlib( camera_up );
	DxLib::MATRIX m;
	DxLib::CreateLookAtMatrix( &m, &pos, &focus, &up );
	for ( int i = 0; i < 4; i++ ) {
		for ( int j = 0; j < 4; j++ ) {
			result.data[ i ][ j ] = m.m[ i ][ j ];
		}
	}
	return result;
}

Matrix Drawer::createProjectionMatrix( ) const {
	Matrix result;
	DxLib::MATRIX dxlib_proj;
	DxLib::CreatePerspectiveFovMatrix( &dxlib_proj, ( float )_fov, ( float )_near, ( float )_far, 1 );
	for ( int i = 0; i < 4; i++ ) {
		for ( int j = 0; j < 4; j++ ) {
			result.data[ i ][ j ] = dxlib_proj.m[ i ][ j ];
		}
	}
	return result;
}

void Drawer::disableZBufferWriting( Scope& scope, bool write_z_buffer ) {
	scope.onWriteZBuffer( );
	DxLib::SetWriteZBuffer3D( write_z_buffer ? TRUE : FALSE );
}

void Drawer::disableZBufferUsing( Scope& scope, bool use_z_buffer ) {
	scope.onUseZBuffer( );
	DxLib::SetUseZBuffer3D( use_z_buffer ? TRUE : FALSE );
}

void Drawer::setTarget( Scope& scope, ImageTargetPtr target ) const {
	scope.onTarget( );
	if ( target ) {
		SetDrawScreen( target->getHandle( ) );
	} else {
		SetDrawScreen( DX_SCREEN_BACK );
	}
	initCameraSetting( );
}

void Drawer::setBlend( Scope& scope, BLEND blend, double ratio ) const {
	scope.onBlend( );
	int param = ( int )( 255 * ratio );
	switch ( blend ) {
	case BLEND::NONE:
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, param );
		break;
	case BLEND::ALPHA:
		SetDrawBlendMode( DX_BLENDMODE_PMA_ALPHA, param );
		break;
	case BLEND::ADD:
		SetDrawBlendMode( DX_BLENDMODE_PMA_ADD, param );
		break;
	case BLEND::SUB:
		SetDrawBlendMode( DX_BLENDMODE_PMA_SUB, param );
		break;
	case BLEND::INV:
		SetDrawBlendMode( DX_BLENDMODE_PMA_INVSRC, param );
		break;
	case BLEND::MUL:
		SetDrawBlendMode( DX_BLENDMODE_MUL, param );
		break;
	}
}

void Drawer::setBright( Scope& scope, double r, double g, double b ) const {
	scope.onBright( );
	SetDrawBright(
		( int )( r * 255 ),
		( int )( g * 255 ),
		( int )( b * 255 ) );
}

void Drawer::setUseLight( bool on ) {
	_light = on;
	SetUseLighting( _light );
	SetLightEnable( _light );
}

Color Drawer::getPixelColor( int x, int y ) const {
	int cr = GetPixel( x, y );
	int r;
	int g;
	int b;
	GetColor2( cr, &r, &g, &b );
	return Color( r / 255.0, g / 255.0, b / 255.0 );
}

bool Drawer::isOutterOfScreen( const Vector& pos ) {
	VECTOR screen_pos = ConvWorldPosToScreenPos( VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z ) );
	if ( screen_pos.z <= 0 || screen_pos.z >= 1.0 ) {
		return true;
	}
	const double BUFF = 50.0;
	if ( screen_pos.x < -BUFF || screen_pos.x > _screen_width + BUFF ) {
		return true;
	}
	if ( screen_pos.y < -BUFF || screen_pos.y > _screen_height + BUFF ) {
		return true;
	}
	return false;
}

void Drawer::polygon( Vector v1, Vector v2, Vector v3, int r, int g, int b ) {
	COLOR_U8 color = GetColorU8( r, g, b, 255 );
	VERTEX3D vertex[ 3 ];
	vertex[ 0 ].pos = VGet( ( float )v1.x, ( float )v1.y, ( float )v1.z );
	vertex[ 0 ].norm = VGet( 0.0f, 0.0f, -1.0f );
	vertex[ 0 ].dif = color;
	vertex[ 0 ].spc = GetColorU8( 0, 0, 0, 0 );
	vertex[ 0 ].u = 0.0f;
	vertex[ 0 ].v = 0.0f;
	vertex[ 0 ].su = 0.0f;
	vertex[ 0 ].sv = 0.0f;
	vertex[ 1 ].pos = VGet( ( float )v2.x, ( float )v2.y, ( float )v2.z );
	vertex[ 1 ].norm = VGet( 0.0f, 0.0f, -1.0f );
	vertex[ 1 ].dif = color;
	vertex[ 1 ].spc = GetColorU8( 0, 0, 0, 0 );
	vertex[ 1 ].u = 0.0f;
	vertex[ 1 ].v = 0.0f;
	vertex[ 1 ].su = 0.0f;
	vertex[ 1 ].sv = 0.0f;
	vertex[ 2 ].pos = VGet( ( float )v3.x, ( float )v3.y, ( float )v3.z );
	vertex[ 2 ].norm = VGet( 0.0f, 0.0f, -1.0f );
	vertex[ 2 ].dif = color;
	vertex[ 2 ].spc = GetColorU8( 0, 0, 0, 0 );
	vertex[ 2 ].u = 0.0f;
	vertex[ 2 ].v = 0.0f;
	vertex[ 2 ].su = 0.0f;
	vertex[ 2 ].sv = 0.0f;
	DrawPolygon3D( vertex, 1, DX_NONE_GRAPH, FALSE );
}