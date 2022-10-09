#include "VFXWater.h"
#include "Drawer.h"
#include "Primitive.h"
#include "Primitive3DShader.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "Application.h"

const char* IMAGE_CAMP = "water_camp.png";
const char* IMAGE_LABYRINTH = "water_labyrinth.png";
const char* IMAGE_DEATHPARADE = "water_deathparade.png";


VFXWater::VFXWater( ) {
	// 仮でシェーダー無しに設定
	//_shaderVersion = true;

	DrawerPtr drawer = Task::get< Drawer >( );
	ApplicationPtr app = Application::getInstance( );
	_primitive_3dshader = drawer->createPrimitive3DShader( );
	_model_water = drawer->createPrimitive( );
	_model_water->setTrans( true );


	// プログラマブルシェーダーモデルの確認
	//if ( GetValidShaderVersion( ) >= 200 ) {
	//	_shaderVersion = true;
	//	initialize( );
	//} else {
	//	_shaderVersion = false;
	//}

	if ( app->isEnabledShaderVersion( 2 ) ) {
		_shaderVersion = true;
		initialize( );
	} else {
		_shaderVersion = false;
	}

	loadImage( IMAGE_LABYRINTH );

	//for ( int i = 0; i < 100; i++ ) {
	//	for ( int j = 0; j < 100; j++ ) {
	//		int height = map->getWaterHeight( i, j );
	//		if ( height > 0 ) {
	//			addPos( i, height, j );
	//		}
	//	}
	//}
	//create( );

	const int field_size = 30;
	for ( int i = 0; i < field_size; i++ ) {
		for ( int j = 0; j < field_size; j++ ) {
			int height = 1;
			addPos( i, height, j );
		}
	}
	create( );
}

VFXWater::~VFXWater( ) {

}

void VFXWater::draw( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );

	// 透過度の設定を有効にする(毎フレーム呼ぶ)
	//SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 );
	drawer->setBlend( scope, Drawer::BLEND::ALPHA, 1.0 );


	if ( _shaderVersion ) {
		drawShader( );		// シェーダあり
	} else {
		drawNoneShader( );	// シェーダなし
	}

	// 透過度の設定を有効にする(毎フレーム呼ぶ)
	//SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	drawer->setBlend( scope, Drawer::BLEND::NONE, 0.0 );

}

void VFXWater::addPos( int x, int y, int z ) {
	_array.push_back( Vector( x, y, z ) );
}

void VFXWater::drawShader( ) {
	//使用する頂点シェーダーをセット
	//SetUseVertexShader( _vertexFile );
	_vertex_shader->useVertexShader( );
	//SetUsePixelShader( _pixelFile );
	_pixel_shader->usePixelShader( );

	//レジスターのセット
	setVertexRegister( );
	setPixelRegister( );

	//テクスチャーのセット
	setTexture( );

	//描画
	//DrawPolygon3DToShader( _sVertex, _polygon_num );	
	////シェーダーファイルのリセット
	//SetUseVertexShader( -1 );
	//SetUsePixelShader( -1 );
	_primitive_3dshader->draw( _model_water );
}

void VFXWater::drawNoneShader( ) {
	_model_water->draw( );
}

void VFXWater::loadImage( const char* filename ) {
	//_model = LoadGraph( str );
	ImagePtr image = Task::get< Drawer >( )->createImage( filename );
	_model_water->setImage( image );
}

void VFXWater::setAmplitude( double index ) {
	_parameter.amplitude = ( float )index;
}

void VFXWater::setPeriodSpeed( double speed ) {
	_speed = ( float )speed;
}

void VFXWater::create( ) {

	allocate( );

	_model_water->setNum( ( int )_array.size( ) * 2 );

	for ( int i = 0; i < ( int )_array.size( ); i++ ) {
		Vector plankPos[ 4 ];
		Vector vertex[ 6 ];

		double x = _array.at( i ).x;
		double y = _array.at( i ).y;
		double z = _array.at( i ).z;

		// 位置
		plankPos[ 0 ] = Vector( x + 0, y, z + 0 );
		plankPos[ 1 ] = Vector( x + 0, y, z + 1 );
		plankPos[ 2 ] = Vector( x + 1, y, z + 0 );
		plankPos[ 3 ] = Vector( x + 1, y, z + 1 );

		// 板ポリゴンの形成
		vertex[ 0 ] = plankPos[ 0 ];
		vertex[ 1 ] = plankPos[ 1 ];
		vertex[ 2 ] = plankPos[ 2 ];

		vertex[ 3 ] = plankPos[ 1 ];
		vertex[ 4 ] = plankPos[ 3 ];
		vertex[ 5 ] = plankPos[ 2 ];

		setPlank( i * 6, vertex );
	}

	// 登録した配列を削除
	_array.clear( );
}

void VFXWater::allocate( ) {
	if ( _shaderVersion ) {
		//if ( _sVertex != NULL ) delete[ ] _sVertex;
		//_sVertex = new VERTEX3DSHADER[ ( int )_array.size( ) * 6 ];
		_primitive_3dshader->alloc( ( int )_array.size( ) * 2 );

	} else {
		//if ( _vertex != NULL ) delete[ ] _vertex;
		//_vertex = new VERTEX3D[ ( int )_array.size( ) * 6 ];
		_model_water->alloc( ( int )_array.size( ) * 2 );
	}
}

void VFXWater::setPlank( int num, const Vector* vertex ) {
	for ( int i = 0; i < 6; i++ ) {

		// テクスチャーUV
		int u[ 6 ] = { 0, 1, 0, 1, 1, 0 };
		int v[ 6 ] = { 0, 0, 1, 0, 1, 1 };

		if ( _shaderVersion ) {
			//_sVertex[ num + i ].pos  = VGet( ( float )vertex[ i ].x, ( float )vertex[ i ].y, ( float )vertex[ i ].z );
			//_sVertex[ num + i ].norm = VGet( 0.0f, 0.0f, 0.0f );
			//_sVertex[ num + i ].dif  = GetColorU8( 255, 255, 255, 255 );
			//_sVertex[ num + i ].spc  = GetColorU8( 0, 0, 0, 0 );
			//_sVertex[ num + i ].u    = ( float )u[ i ];
			//_sVertex[ num + i ].v    = ( float )v[ i ];
			//_sVertex[ num + i ].su   = 0.0f;
			//_sVertex[ num + i ].sv   = 0.0f;
			Primitive::VERTEX vtx;
			vtx.pos = vertex[ i ];
			vtx.norm = Vector( 0.0, 0.0, 0.0 );
			vtx.color = Color( 1.0, 1.0, 1.0, 1.0 );
			vtx.u = u[ i ];
			vtx.v = v[ i ];

			_primitive_3dshader->set( num + i, vtx );
		} else {
			//_vertex[ num + i ].pos  = VGet( ( float )vertex[ i ].x, ( float )vertex[ i ].y, ( float )vertex[ i ].z );
			//_vertex[ num + i ].norm = VGet( 0.0f, 0.0f, 0.0f );
			//_vertex[ num + i ].dif  = GetColorU8( 255, 255, 255, 255 );
			//_vertex[ num + i ].spc  = GetColorU8( 0, 0, 0, 0 );
			//_vertex[ num + i ].u    = ( float )u[ i ];
			//_vertex[ num + i ].v    = ( float )v[ i ];
			//_vertex[ num + i ].su   = 0.0f;
			//_vertex[ num + i ].sv   = 0.0f;
			Primitive::VERTEX vtx;
			vtx.pos = vertex[ i ];
			vtx.norm = Vector( 0.0, 0.0, 0.0 );
			vtx.color = Color( 1.0, 1.0, 1.0, 1.0 );
			vtx.u = u[ i ];
			vtx.v = v[ i ];

			_model_water->set( num + i, vtx );
		}
	}
}

void VFXWater::initialize( ) {
	ApplicationPtr app = Application::getInstance( );
	DrawerPtr drawer = Task::get< Drawer >( );

	//_vertexFile = LoadVertexShader( "resource/shader/WaveVS.vso" );	// 頂点シェーダーを読み込む
	_vertex_shader = drawer->createVertexShader(  "WaveVS.vso" );
	//_pixelFile = LoadPixelShader( "resource/shader/WavePS.pso" );	// ピクセルシェーダーを読み込む
	_pixel_shader = drawer->createPixelShader( "WavePS.pso" );

	//GetWindowSize( &_sizeX, &_sizeY );
	_size_x = app->getWindowWidth( );
	_size_y = app->getScreenHeight( );

	_time = 0.0f;

	_speed = 1;
}

void VFXWater::setVertexRegister( ) {
	//_parameter.period += 0.01f * _speed;
	_parameter.period += 0.01f * _speed;
	//_parameter.period = fmod( _parameter.period, ( float )PI2 );
	_parameter.period = fmod( _parameter.period, ( float )PI2 );
	//SetVSConstSF( 0, _parameter.period );
	_primitive_3dshader->setVSConstSF( 0, _parameter.period );
	//SetVSConstSF( 1, _parameter.amplitude );
	_primitive_3dshader->setVSConstSF( 1, _parameter.amplitude );
}

void VFXWater::setPixelRegister( ) {
	//_time += 0.01f;
	_time += 0.01f;
	//SetPSConstSF( 0, _time );
	_pixel_shader->setPSConstSF( 0, _time );
	//SetPSConstSF( 1, ( float )_sizeX );
	_pixel_shader->setPSConstSF( 1, ( float )_size_x );
	//SetPSConstSF( 2, ( float )_sizeY );
	_pixel_shader->setPSConstSF( 2, ( float )_size_y );
}

void VFXWater::setTexture( ) {
	//if ( _model != -1 ) {
	//	SetPSConstSF( 3, ( float )TRUE );
	//} else {
	//	SetPSConstSF( 3, ( float )FALSE );
	//}
	if ( _model_water ) {
		_pixel_shader->setPSConstSF( 3, ( float )true );
	} else {
		_pixel_shader->setPSConstSF( 3, ( float )false );
	}

	//SetUseTextureToShader( 0, _model );
	_pixel_shader->setUseTexture( 0, _model_water->getImage( ) );
}


/*

ViewerWater::ViewerWater( MapConstPtr map, unsigned char state ) {
//: _sVertex( NULL )
//, _vertex( NULL ) {

	_model_water = PolygonPtr( new Polygon );

	{
	// シェーダー移行前の仮設定
	_shaderVersion = false;
	}

	// プログラマブルシェーダーモデルの確認
	//if ( GetValidShaderVersion( ) >= 200 ) {
	//	_shaderVersion = true;
	//	initialize( );
	//} else {
	//	_shaderVersion = false;
	//}
	_polygon_num = 0;

	switch ( state ) {
	case STATE_CAMP:
		loadImage( "resource/image/map/water_camp.png" );
		break;
	case STATE_LABYRINTH:
		loadImage( "resource/image/map/water_labyrinth.png" );
		break;
	case STATE_DEATHPARADE:
		loadImage( "resource/image/map/water_deathparade.png" );
		break;
	}

	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			int height = map->getWaterHeight( i, j );
			if ( height > 0 ) {
				addPos( i, height, j );
			}
		}
	}
	create( );
}

ViewerWater::~ViewerWater( ) {
	//delete[ ] _vertex;
}

void ViewerWater::draw( ) {
	//SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 );	// 透過度の設定を有効にする(毎フレーム呼ぶ)

	if ( _shaderVersion ) {
		//drawShader( );		// シェーダあり
	} else {
		drawNoneShader( );	// シェーダなし
	}

	//SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );	// 透過度の設定を有効にする(毎フレーム呼ぶ)
}

// 出現位置の指定
void ViewerWater::addPos( int x, int y, int z ) {
	_array.push_back( Vector( x, y, z ) );
}

// 波の作成
void ViewerWater::create( ) {

	allocate( );	// メモリー確保

	_polygon_num = ( int )_array.size( ) * 2;	// ポリゴン数

	for ( int i = 0; i < ( int )_array.size( ); i++ ) {
		Vector plankPos[ 4 ];
		Vector vertex[ 6 ];

		double x = _array.at( i ).x;
		double y = _array.at( i ).y;
		double z = _array.at( i ).z;

		// 位置
		plankPos[ 0 ] = Vector( x + 0, y, z + 0 );
		plankPos[ 1 ] = Vector( x + 0, y, z + 1 );
		plankPos[ 2 ] = Vector( x + 1, y, z + 0 );
		plankPos[ 3 ] = Vector( x + 1, y, z + 1 );

		// 板ポリゴンの形成
		vertex[ 0 ] = plankPos[ 0 ];
		vertex[ 1 ] = plankPos[ 1 ];
		vertex[ 2 ] = plankPos[ 2 ];

		vertex[ 3 ] = plankPos[ 1 ];
		vertex[ 4 ] = plankPos[ 3 ];
		vertex[ 5 ] = plankPos[ 2 ];

		setPlank( i * 6, vertex );
	}

	_array.clear( );	// 登録した配列を削除
}

// 振幅の設定
void ViewerWater::setAmplitude( double index ) {
	_parameter.amplitude = ( float )index;
}

// 周期の速度
void ViewerWater::setPeriodSpeed( double speed ) {
	_speed = ( float )speed;
}

// 使用する画像をロード
void ViewerWater::loadImage( const char* str ) {
	//_model = LoadGraph( str );
	ImagePtr image = Task::get< Drawer >( )->createImage( str );
	_model_water->setImage( image );
}

void ViewerWater::initialize( ) {
	//_vertexFile = LoadVertexShader( "resource/shader/WaveVS.vso" );	// 頂点シェーダーを読み込む

	//_pixelFile = LoadPixelShader( "resource/shader/WavePS.pso" );	// ピクセルシェーダーを読み込む

	//GetWindowSize( &_sizeX, &_sizeY );

	_time = 0.0f;

	_speed = 1;
}

void ViewerWater::setPlank( int num, const Vector* vertex ) {
	for ( int i = 0; i < 6; i++ ) {

		// テクスチャーUV
		int u[ 6 ] = { 0, 1, 0, 1, 1, 0 };
		int v[ 6 ] = { 0, 0, 1, 0, 1, 1 };

		if ( _shaderVersion ) {
			//_sVertex[ num + i ].pos  = VGet( ( float )vertex[ i ].x, ( float )vertex[ i ].y, ( float )vertex[ i ].z );
			//_sVertex[ num + i ].norm = VGet( 0.0f, 0.0f, 0.0f );
			//_sVertex[ num + i ].dif  = GetColorU8( 255, 255, 255, 255 );
			//_sVertex[ num + i ].spc  = GetColorU8( 0, 0, 0, 0 );
			//_sVertex[ num + i ].u    = ( float )u[ i ];
			//_sVertex[ num + i ].v    = ( float )v[ i ];
			//_sVertex[ num + i ].su   = 0.0f;
			//_sVertex[ num + i ].sv   = 0.0f;
		} else {
			Polygon::VERTEX vtx;
			vtx.color = Color( 1.0, 1.0, 1.0, 1.0 );
			vtx.norm = Vector( 0.0, 0.0, 0.0 );
			vtx.pos = *vertex;
			vtx.u = u[ i ];
			vtx.v = v[ i ];

			_model_water->set( i, vtx );
			//_vertex[ num + i ].pos  = VGet( ( float )vertex[ i ].x, ( float )vertex[ i ].y, ( float )vertex[ i ].z );
			//_vertex[ num + i ].norm = VGet( 0.0f, 0.0f, 0.0f );
			//_vertex[ num + i ].dif  = GetColorU8( 255, 255, 255, 255 );
			//_vertex[ num + i ].spc  = GetColorU8( 0, 0, 0, 0 );
			//_vertex[ num + i ].u    = ( float )u[ i ];
			//_vertex[ num + i ].v    = ( float )v[ i ];
			//_vertex[ num + i ].su   = 0.0f;
			//_vertex[ num + i ].sv   = 0.0f;
		}
	}
}

void ViewerWater::setVertexRegister( ) {
	//_parameter.period += 0.01f * _speed;
	//_parameter.period = fmod( _parameter.period, ( float )PI2 );
	//SetVSConstSF( 0, _parameter.period );
	//SetVSConstSF( 1, _parameter.amplitude );
}

void ViewerWater::setPixelRegister( ) {
	//_time += 0.01f;
	//SetPSConstSF( 0, _time );
	//SetPSConstSF( 1, ( float )_sizeX );
	//SetPSConstSF( 2, ( float )_sizeY );
}

void ViewerWater::setTexture( ) {
	//if ( _model != -1 ) {
	//	SetPSConstSF( 3, ( float )TRUE );
	//} else {
	//	SetPSConstSF( 3, ( float )FALSE );
	//}
	//SetUseTextureToShader( 0, _model );
}
void ViewerWater::drawNoneShader( ) {
	//if ( _model != -1 ) {
	//	DrawPolygon3D( _vertex ,_polygon_num, _model, TRUE );
	//} else {
	//	DrawPolygon3D( _vertex ,_polygon_num, DX_NONE_GRAPH, TRUE );
	//}
	_model_water->draw( );
}

void ViewerWater::drawShader( ) {
	// 使用する頂点シェーダーをセット
	//SetUseVertexShader( _vertexFile );
	//SetUsePixelShader( _pixelFile );

	// レジスターのセット
	setVertexRegister( );
	setPixelRegister( );

	// テクスチャーのセット
	setTexture( );

	//　描画
	//DrawPolygon3DToShader( _sVertex, _polygon_num );

	// シェーダーファイルのリセット
	//SetUseVertexShader( -1 );
	//SetUsePixelShader( -1 );
}

void ViewerWater::allocate( ) {
	if ( _shaderVersion ) {
		//if ( _sVertex != NULL ) delete[ ] _sVertex;
		//_sVertex = new VERTEX3DSHADER[ ( int )_array.size( ) * 6 ];
	} else {
		//if ( _vertex != NULL ) delete[ ] _vertex;
		//_vertex = new VERTEX3D[ ( int )_array.size( ) * 6 ];
		_model_water->alloc( 2 );
	}
}

*/