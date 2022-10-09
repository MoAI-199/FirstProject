#include "VFXBlur.h"
#include "ImageTarget.h"
#include "Drawer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Graph.h"
#include "mathematics.h"
#include "Primitive2DShader.h"


VFXBlur::VFXBlur( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	//	GetWindowSize( &SCREEN_W, &SCREEN_H );
//
//	ShaderScreen_W = 1024 * 2;
	ShaderScreen_W = 1024 * 2;
	//	ShaderScreen_H = 1024 * 2;
	ShaderScreen_H = 1024 * 2;
	//
	//	// 通常描画用のテクスチャを作成
	//	ColorMapTexture[ 0 ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
	//
	//	// 通常描画した映像に弱いぼかしを加えた画像を保存するためのテクスチャを作成
	//	ColorMapTexture[ 1 ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
	//
	//	// 通常描画した映像に強いぼかしを加えた画像を保存するためのテクスチャを作成
	//	ColorMapTexture[ 2 ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
	//
	for ( int i = 0; i < 3; i++ ) {
		_color_map_texture[ i ] = drawer->createImageTarget( ShaderScreen_W, ShaderScreen_H );
	}
	//	// 深度値を描画する１チャンネル画像を作成
	//	SetCreateDrawValidGraphChannelNum( 1 );
	_graph = drawer->createGraph( );
	_graph->setGraphChannelNum( 1 );
	//	DepthMapTexture = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
	_depth_map_texture = drawer->createImageTarget( ShaderScreen_W, ShaderScreen_H );
	//	SetCreateDrawValidGraphChannelNum( 0 );
	_graph->setGraphChannelNum( 0 );
	//
	//	
	//	// 加工した画像を保存するためのテクスチャを作成
	//	for ( int i = 0; i < MAX_BLUR; i++ ) {
	//		OldMapTexture[ i ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
	//	}
	for ( int i = 0; i < MAX_BLUR; i++ ) {
		_old_map_texture[ i ] = drawer->createImageTarget( ShaderScreen_W, ShaderScreen_H );
	}
	//
	//	// 深度値描画用の頂点シェーダー読み込み
	//	Depth_NormalMeshVS = LoadVertexShader( "resource/shader/Depth_NormalMeshVS.vso" );
	_depth_normal = drawer->createVertexShader( "resource/shader/Depth_NormalMeshVS.vso" );
	//	Depth_SkinMeshVS   = LoadVertexShader( "resource/shader/Depth_SkinMeshVS.vso" );
	_depth_skin = drawer->createVertexShader( "resource/shader/Depth_SkinMeshVS.vso" );
	//
	//	// 深度値描画用のピクセルシェーダー読み込み
	//	DepthPS            = LoadPixelShader( "resource/shader/DepthPS.pso" );
	_depth_ps = drawer->createPixelShader( "resource/shader/DepthPS.pso" );
	//
	//	// 深度値を使用して被写界深度処理を行うためのピクセルシェーダーを読み込み
	//	DepthOfFieldPS     = LoadPixelShader( "resource/shader/DepthOfFieldPS.pso" );
	_depth_field_ps = drawer->createPixelShader( "resource/shader/DepthOfFieldPS.pso" );

	_primitive_2d = drawer->createPrimitive2DShader( );
	//
	//	// 被写界深度処理用のパラメータを初期化
	_depthOfField_InterpSize = 3000.0f;
	_depthOfField_camera_target = 3200.0f;
	_depthOfField_camera_targetSize = 500.0f;
	//
}

VFXBlur::~VFXBlur( ) {

}

void VFXBlur::drawMotionBlurBegin( ) {
	//
//	// 描画ブレンドモードをセット
//	SetDrawBlendMode( DX_BLENDMODE_PMA_ALPHA, 255 );
//	
//	// 補間範囲とフォーカスが合っている範囲を含めた総距離を算出
//	DepthOfField_TotalSize = DepthOfField_InterpSize * 2.0f + DepthOfField_camera_targetSize;
//
//	// 通常描画用テクスチャにモデルを通常描画
//	DrawScreen( ColorMapTexture[ 0 ], FALSE );
}

void VFXBlur::drawMotionBlurEnd( ) {
	motionBlur( );
}

void VFXBlur::resetMoitonBlur( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	for ( int i = 0; i < MAX_BLUR; i++ ) {
		//SetDrawScreen( OldMapTexture[ i ] );
		drawer->setTarget( scope, _old_map_texture[ i ] );
		//ClearDrawScreen( );
		drawer->clearScreen( );
	}
	//SetDrawScreen( DX_SCREEN_BACK );
	drawer->setTarget( scope );
}

void VFXBlur::draw( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	//	FLOAT4 ParamF;
	Quaternion paramf;
	//	float RightU, BottomV;
	float right_u;
	float bottom_v;
	//	VERTEX2DSHADER Vertex[ 6 ];


			// 被写界深度描画の準備
	{
		// 通常描画の結果にぼかしを加えた画像を保存
		_graph->setGraphFilterTectBlt( _color_map_texture[ 0 ], _color_map_texture[ 1 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, Graph::GRAPH::GAUSS );

		// ぼかしを加えた画像に更にぼかしを加えて保存
		_graph->setGraphFilterTectBlt( _color_map_texture[ 1 ], _color_map_texture[ 2 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, Graph::GRAPH::GAUSS );

		// 深度値描画用テクスチャに深度値を描画
		drawer->setTarget( scope, _depth_map_texture );
	}

	// 描画先を裏画面に変更
//		SetDrawScreen( DX_SCREEN_BACK );
	drawer->setTarget( scope );


	// 被写界深度を施した画像を裏画面に描画
	{
		// 使用するテクスチャをセット
		//SetUseTextureToShader( 0, ColorMapTexture[ 0 ] );
		_depth_field_ps->setUseTexture( 0, _color_map_texture[ 0 ] );
		//SetUseTextureToShader( 1, ColorMapTexture[ 1 ] );
		_depth_field_ps->setUseTexture( 1, _color_map_texture[ 1 ] );
		//SetUseTextureToShader( 2, ColorMapTexture[ 2 ] );
		_depth_field_ps->setUseTexture( 2, _color_map_texture[ 2 ] );
		//SetUseTextureToShader( 3, DepthMapTexture );
		_depth_field_ps->setUseTexture( 3, _color_map_texture[ 3 ] );

		// 被写界深度処理用のピクセルシェーダーをセット
		//SetUsePixelShader( DepthOfFieldPS );
		_depth_field_ps->usePixelShader( );

		// 被写界深度のぼかし無しの開始位置を計算
		//ParamF.x = 100;//DepthOfField_InterpSize / DepthOfField_TotalSize;
		paramf.x = 100;

		// 被写界深度のぼかし無しの終了位置を計算
		//ParamF.y = ParamF.x;//( DepthOfField_InterpSize + DepthOfField_camera_targetSize ) / DepthOfField_TotalSize;
		paramf.y = paramf.x;

		// ピクセルシェーダーの定数にセット
		//SetPSConstF( 0, ParamF );
		_depth_field_ps->setPSConstF( 0, paramf );

		Primitive::VERTEX vtx[ 6 ];

		_primitive_2d->alloc( 2 );

		// 画面全体に描画する頂点の準備
		//Vertex[ 0 ].pos = VGet(					   0.0f,					0.0f, 0.0f );
		vtx[ 0 ].pos = Vector( 0.0, 0.0, 0.0 );
		//Vertex[ 1 ].pos = VGet( ( float )ShaderScreen_W,					0.0f, 0.0f );
		vtx[ 1 ].pos = Vector( ShaderScreen_W, 0.0, 0.0 );
		//Vertex[ 2 ].pos = VGet(					   0.0f, ( float )ShaderScreen_H, 0.0f );
		vtx[ 2 ].pos = Vector( 0.0, ShaderScreen_H, 0.0 );
		//Vertex[ 3 ].pos = VGet( ( float )ShaderScreen_W, ( float )ShaderScreen_H, 0.0f );
		vtx[ 3 ].pos = Vector( ShaderScreen_W, ShaderScreen_H, 0.0 );

		//Vertex[ 0 ].rhw = 1.0f;
		//Vertex[ 1 ].rhw = 1.0f;
		//Vertex[ 2 ].rhw = 1.0f;
		//Vertex[ 3 ].rhw = 1.0f;

		right_u = ( float )ShaderScreen_W / ShaderScreen_W;
		bottom_v = ( float )ShaderScreen_H / ShaderScreen_H;
		vtx[ 0 ].u = 0.0f;
		vtx[ 1 ].u = right_u;
		vtx[ 2 ].u = 0.0f;
		vtx[ 3 ].u = right_u;

		vtx[ 0 ].v = 0.0f;
		vtx[ 1 ].v = 0.0f;
		vtx[ 2 ].v = bottom_v;
		vtx[ 3 ].v = bottom_v;

		vtx[ 4 ] = vtx[ 2 ];
		vtx[ 5 ] = vtx[ 1 ];

		for ( int i = 0; i < 6; i++ ) {
			_primitive_2d->set( i, vtx[ i ] );
		}
		// ポリゴン２枚を使用して画面全体に被写界深度処理を施した３Ｄシーンの描画
		//DrawPolygon2DToShader( Vertex, 2 );
		_primitive_2d->drawUsPolygon2DToShader( 2 );
	}


	//	// 読み込んだシェーダーの解除
	//	SetUsePixelShader( -1 );
	//	SetUseVertexShader( -1 );
	//	MV1SetUseOrigShader( FALSE );
	//
	_depth_field_ps->usePixelShaderEnd( );
	//	// 描画ブレンドモードを元に戻す
	//	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
	drawer->setBlend( scope, Drawer::BLEND::NONE, 1.0 );
	//		
}

void VFXBlur::motionBlur( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	//	FLOAT4 ParamF;
	Quaternion paramf;
	//	float RightU, BottomV;
	float right_u;
	float bottom_v;
	//	VERTEX2DSHADER Vertex[ 6 ];

			// 被写界深度描画の準備
	{
		// 通常描画の結果にぼかしを加えた画像を保存
		//GraphFilterRectBlt( ColorMapTexture[ 0 ], ColorMapTexture[ 1 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, DX_GRAPH_FILTER_GAUSS, 8, 2000 );
		_graph->setGraphFilterTectBlt( _color_map_texture[ 0 ], _color_map_texture[ 1 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, Graph::GRAPH::GAUSS );

		// ぼかしを加えた画像に更にぼかしを加えて保存
		//GraphFilterRectBlt( ColorMapTexture[ 1 ], ColorMapTexture[ 2 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, DX_GRAPH_FILTER_GAUSS, 8, 2000 );
		_graph->setGraphFilterTectBlt( _color_map_texture[ 1 ], _color_map_texture[ 2 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, Graph::GRAPH::GAUSS );

		// 深度値描画用テクスチャに深度値を描画
		//DrawScreen( DepthMapTexture, TRUE );
		drawer->setTarget( scope, _depth_map_texture );
	}

	// 描画先を裏画面に変更
	//SetDrawScreen( OldMapTexture[ 0 ] );
	drawer->setTarget( scope );

	//ClearDrawScreen( );
	drawer->clearScreen( );

	// 被写界深度を施した画像を裏画面に描画
	{
		// 使用するテクスチャをセット
		//SetUseTextureToShader( 0, ColorMapTexture[ 0 ] );
		_depth_field_ps->setUseTexture( 0, _color_map_texture[ 0 ] );
		//SetUseTextureToShader( 1, ColorMapTexture[ 1 ] );
		_depth_field_ps->setUseTexture( 1, _color_map_texture[ 1 ] );
		//SetUseTextureToShader( 2, ColorMapTexture[ 2 ] );
		_depth_field_ps->setUseTexture( 2, _color_map_texture[ 2 ] );
		//SetUseTextureToShader( 3, DepthMapTexture );
		_depth_field_ps->setUseTexture( 3, _depth_map_texture );

		// 被写界深度処理用のピクセルシェーダーをセット
		//SetUsePixelShader( DepthOfFieldPS );
		_depth_field_ps->usePixelShader( );

		// 被写界深度のぼかし無しの開始位置を計算
		paramf.x = 100;//DepthOfField_InterpSize / DepthOfField_TotalSize;

		// 被写界深度のぼかし無しの終了位置を計算
		paramf.y = paramf.x;//( DepthOfField_InterpSize + DepthOfField_camera_targetSize ) / DepthOfField_TotalSize;

		// ピクセルシェーダーの定数にセット
		//SetPSConstF( 0, ParamF );
		_depth_field_ps->setPSConstF( 0, paramf );

		Primitive::VERTEX vtx[ 6 ];

		// 画面全体に描画する頂点の準備
		vtx[ 0 ].pos = Vector( 0.0f, 0.0f, 0.0f );
		vtx[ 1 ].pos = Vector( ShaderScreen_W, 0.0f, 0.0f );
		vtx[ 2 ].pos = Vector( 0.0f, ShaderScreen_H, 0.0f );
		vtx[ 3 ].pos = Vector( ShaderScreen_W, ShaderScreen_H, 0.0f );

		//Vertex[ 0 ].rhw = 1.0f;
		//Vertex[ 1 ].rhw = 1.0f;
		//Vertex[ 2 ].rhw = 1.0f;
		//Vertex[ 3 ].rhw = 1.0f;

		right_u = ( float )ShaderScreen_W / ShaderScreen_W;
		bottom_v = ( float )ShaderScreen_H / ShaderScreen_H;
		vtx[ 0 ].u = 0.0f;
		vtx[ 1 ].u = right_u;
		vtx[ 2 ].u = 0.0f;
		vtx[ 3 ].u = right_u;

		vtx[ 0 ].v = 0.0f;
		vtx[ 1 ].v = 0.0f;
		vtx[ 2 ].v = bottom_v;
		vtx[ 3 ].v = bottom_v;

		vtx[ 4 ] = vtx[ 2 ];
		vtx[ 5 ] = vtx[ 1 ];

		for ( int i = 0; i < 6; i++ ) {
			_primitive_2d->set( i, vtx[ i ] );
		}
		// ポリゴン２枚を使用して画面全体に被写界深度処理を施した３Ｄシーンの描画
		//DrawPolygon2DToShader( Vertex, 2 );
		_primitive_2d->drawUsPolygon2DToShader( 2 );
	}


	//GetDrawScreenGraph( 0, 0, ShaderScreen_W, ShaderScreen_H, OldMapTexture[ 1 ] );
	_graph->getDrawScreenGraph( 0, 0, ShaderScreen_W, ShaderScreen_H, _old_map_texture[ 1 ] );

	for ( int i = 1; i < MAX_BLUR - 1; i++ ) {
		//SetDrawScreen( OldMapTexture[ MAX_BLUR - ( i + 1 ) ] );
		drawer->setTarget( scope, _old_map_texture[ MAX_BLUR - ( i + 1 ) ] );
		//GetDrawScreenGraph( 0, 0, ShaderScreen_W, ShaderScreen_H, OldMapTexture[ MAX_BLUR - i ] );
		_graph->getDrawScreenGraph( 0, 0, ShaderScreen_W, ShaderScreen_H, _old_map_texture[ MAX_BLUR - i ] );
	}

	// 描画先を裏画面に変更
	//SetDrawScreen( DX_SCREEN_BACK );
	drawer->setTarget( scope );

	for ( int i = 1; i < MAX_BLUR; i++ ) {
		//DrawGraph( 0, 0, OldMapTexture[ MAX_BLUR - i ], TRUE );
		_old_map_texture[ MAX_BLUR - i ]->draw( 0, 0 );

	}

	// 読み込んだシェーダーの解除
	//SetUsePixelShader( -1 );
	//SetUseVertexShader( -1 );
	//MV1SetUseOrigShader( FALSE );
	_depth_field_ps->usePixelShaderEnd( );

	// 描画ブレンドモードを元に戻す
	//SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
	drawer->setBlend( scope, Drawer::BLEND::NONE, 1.0 );
}

//// 指定の画面に３Ｄモデルを描画
void VFXBlur::drawScreen( int ScreenGraphHandle, int UseDepthShader ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	//	FLOAT4 ParamF;
	Quaternion paramf;
	//
	//	// 描画対象を変更
	//	SetDrawScreen( ScreenGraphHandle );
	//
	//	// 画面を初期化
	//	ClearDrawScreen( );
	drawer->clearScreen( );
	//
	//	// 深度値を描画するかどうかで処理を分岐
	//	if( UseDepthShader != 0 ) {
	//		// 深度値を描画する場合はオリジナルのシェーダーを使用してモデルを描画する設定にする
	//		MV1SetUseOrigShader( TRUE );
	//
	//		// 深度値を描画するためのピクセルシェーダーをセット
	//		SetUsePixelShader( DepthPS );
	//
	//		// 被写界深度の開始位置を計算
	//		ParamF.x = DepthOfField_camera_target - DepthOfField_camera_targetSize / 2.0f - DepthOfField_InterpSize;
	//
	//		// 被写界深度の終了位置を計算
	//		ParamF.y = DepthOfField_camera_target + DepthOfField_camera_targetSize / 2.0f + DepthOfField_InterpSize;
	//
	//		// 被写界深度の範囲の逆数を計算
	//		ParamF.z = 1.0f / ( ParamF.y - ParamF.x );
	//
	//		// ピクセルシェーダーの定数にセット
	//		SetPSConstF( 0, ParamF );
	//	} else {
	//		// 深度値を描画するわけではない場合はオリジナルのシェーダーは使用しない
	//		MV1SetUseOrigShader( FALSE );
	//	}
	//
	if ( UseDepthShader != 0 ) {
		_depth_ps->usePixelShaderBegin( );
		// 被写界深度の開始位置を計算
		paramf.x = _depthOfField_camera_target - _depthOfField_camera_targetSize / 2.0 - _depthOfField_InterpSize;

		// 被写界深度の終了位置を計算
		paramf.y = _depthOfField_camera_target + _depthOfField_camera_targetSize / 2.0 + _depthOfField_InterpSize;

		// 被写界深度の範囲の逆数を計算
		paramf.z = 1.0f / ( paramf.y - paramf.x );

		// ピクセルシェーダーの定数にセット
		_depth_ps->setPSConstF( 0, paramf );
	} else {
		_depth_ps->noUseOrigShader( );
	}
	//	// 深度値を描画する場合は頂点シェーダーを剛体メッシュ用の深度値描画用頂点シェーダーにする
	//	if( UseDepthShader != 0 ) {
	//		SetUseVertexShader( Depth_NormalMeshVS );
	//	}
	if ( UseDepthShader != 0 ) {
		_depth_normal->useVertexShader( );
	}
	//
	//	// 深度値を描画する場合は頂点シェーダーをスキニングメッシュ用の深度値描画用頂点シェーダーにする
	//	if( UseDepthShader != 0 ) {
	//		SetUseVertexShader( Depth_SkinMeshVS );
	//	}
	if ( UseDepthShader != 0 ) {
		_depth_skin->useVertexShader( );
	}
}

//#include "ViewerBlur.h"
////#include "Camera.h"
//#include <math.h>
//
//ViewerBlur::ViewerBlur( ) {
//	
//	GetWindowSize( &SCREEN_W, &SCREEN_H );
//
//	ShaderScreen_W = 1024 * 2;
//	ShaderScreen_H = 1024 * 2;
//
//	// 通常描画用のテクスチャを作成
//	ColorMapTexture[ 0 ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
//
//	// 通常描画した映像に弱いぼかしを加えた画像を保存するためのテクスチャを作成
//	ColorMapTexture[ 1 ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
//
//	// 通常描画した映像に強いぼかしを加えた画像を保存するためのテクスチャを作成
//	ColorMapTexture[ 2 ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
//
//	// 深度値を描画する１チャンネル画像を作成
//	SetCreateDrawValidGraphChannelNum( 1 );
//	DepthMapTexture = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
//	SetCreateDrawValidGraphChannelNum( 0 );
//
//	
//	// 加工した画像を保存するためのテクスチャを作成
//	for ( int i = 0; i < MAX_BLUR; i++ ) {
//		OldMapTexture[ i ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
//	}
//
//	// 深度値描画用の頂点シェーダー読み込み
//	Depth_NormalMeshVS = LoadVertexShader( "resource/shader/Depth_NormalMeshVS.vso" );
//	Depth_SkinMeshVS   = LoadVertexShader( "resource/shader/Depth_SkinMeshVS.vso" );
//
//	// 深度値描画用のピクセルシェーダー読み込み
//	DepthPS            = LoadPixelShader( "resource/shader/DepthPS.pso" );
//
//	// 深度値を使用して被写界深度処理を行うためのピクセルシェーダーを読み込み
//	DepthOfFieldPS     = LoadPixelShader( "resource/shader/DepthOfFieldPS.pso" );
//
//	// 被写界深度処理用のパラメータを初期化
//	DepthOfField_InterpSize = 3000.0f;
//	DepthOfField_camera_target	    = 3200.0f;
//	DepthOfField_camera_targetSize  = 500.0f;
//
//}


//void ViewerBlur::motionBlur( ) {
//	FLOAT4 ParamF;
//	float RightU, BottomV;
//	VERTEX2DSHADER Vertex[ 6 ];
//
//		// 被写界深度描画の準備
//		{
//			// 通常描画の結果にぼかしを加えた画像を保存
//			GraphFilterRectBlt( ColorMapTexture[ 0 ], ColorMapTexture[ 1 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, DX_GRAPH_FILTER_GAUSS, 8, 2000 );
//
//			// ぼかしを加えた画像に更にぼかしを加えて保存
//			GraphFilterRectBlt( ColorMapTexture[ 1 ], ColorMapTexture[ 2 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, DX_GRAPH_FILTER_GAUSS, 8, 2000 );
//			
//			// 深度値描画用テクスチャに深度値を描画
//			DrawScreen( DepthMapTexture, TRUE );
//		}
//
//		// 描画先を裏画面に変更
//		SetDrawScreen( OldMapTexture[ 0 ] );
//		ClearDrawScreen( );
//
//		// 被写界深度を施した画像を裏画面に描画
//		{
//			// 使用するテクスチャをセット
//			SetUseTextureToShader( 0, ColorMapTexture[ 0 ] );
//			SetUseTextureToShader( 1, ColorMapTexture[ 1 ] );
//			SetUseTextureToShader( 2, ColorMapTexture[ 2 ] );
//			SetUseTextureToShader( 3, DepthMapTexture );
//
//			// 被写界深度処理用のピクセルシェーダーをセット
//			SetUsePixelShader( DepthOfFieldPS );
//
//			// 被写界深度のぼかし無しの開始位置を計算
//			ParamF.x = 100;//DepthOfField_InterpSize / DepthOfField_TotalSize;
//
//			// 被写界深度のぼかし無しの終了位置を計算
//			ParamF.y = ParamF.x;//( DepthOfField_InterpSize + DepthOfField_camera_targetSize ) / DepthOfField_TotalSize;
//
//			// ピクセルシェーダーの定数にセット
//			SetPSConstF( 0, ParamF );
//
//			// 画面全体に描画する頂点の準備
//			Vertex[ 0 ].pos = VGet(				   	   0.0f,					0.0f, 0.0f );
//			Vertex[ 1 ].pos = VGet( ( float )ShaderScreen_W,					0.0f, 0.0f );
//			Vertex[ 2 ].pos = VGet(					   0.0f, ( float )ShaderScreen_H, 0.0f );
//			Vertex[ 3 ].pos = VGet( ( float )ShaderScreen_W, ( float )ShaderScreen_H, 0.0f );
//
//			Vertex[ 0 ].rhw = 1.0f;
//			Vertex[ 1 ].rhw = 1.0f;
//			Vertex[ 2 ].rhw = 1.0f;
//			Vertex[ 3 ].rhw = 1.0f;
//
//			RightU  = ( float )ShaderScreen_W / ShaderScreen_W;
//			BottomV = ( float )ShaderScreen_H / ShaderScreen_H;
//			Vertex[ 0 ].u = 0.0f;
//			Vertex[ 1 ].u = RightU;
//			Vertex[ 2 ].u = 0.0f;
//			Vertex[ 3 ].u = RightU;
//
//			Vertex[ 0 ].v = 0.0f;
//			Vertex[ 1 ].v = 0.0f;
//			Vertex[ 2 ].v = BottomV;
//			Vertex[ 3 ].v = BottomV;
//
//			Vertex[ 4 ] = Vertex[ 2 ];
//			Vertex[ 5 ] = Vertex[ 1 ];
//
//			// ポリゴン２枚を使用して画面全体に被写界深度処理を施した３Ｄシーンの描画
//			DrawPolygon2DToShader( Vertex, 2 );
//		}
//
//		
//		GetDrawScreenGraph( 0, 0, ShaderScreen_W, ShaderScreen_H, OldMapTexture[ 1 ] );
//
//		for ( int i = 1; i < MAX_BLUR - 1; i++ ) {
//			SetDrawScreen( OldMapTexture[ MAX_BLUR - ( i + 1 ) ] );
//			GetDrawScreenGraph( 0, 0, ShaderScreen_W, ShaderScreen_H, OldMapTexture[ MAX_BLUR - i ] );
//		}
//
//		// 描画先を裏画面に変更
//		SetDrawScreen( DX_SCREEN_BACK );
//
//		for ( int i = 1; i < MAX_BLUR; i++ ) {
//			DrawGraph( 0, 0, OldMapTexture[ MAX_BLUR - i ], TRUE );
//		}
//
//	// 読み込んだシェーダーの解除
//	SetUsePixelShader( -1 );
//	SetUseVertexShader( -1 );
//	MV1SetUseOrigShader( FALSE );
//
//	// 描画ブレンドモードを元に戻す
//	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
//		
//}
//
//void ViewerBlur::drawBegin( ) {
//	
//	// 描画ブレンドモードをセット
//	SetDrawBlendMode( DX_BLENDMODE_PMA_ALPHA, 255 );
//	
//	// 補間範囲とフォーカスが合っている範囲を含めた総距離を算出
//	DepthOfField_TotalSize = DepthOfField_InterpSize * 2.0f + DepthOfField_camera_targetSize;
//
//	// 通常描画用テクスチャにモデルを通常描画
//	DrawScreen( ColorMapTexture[ 0 ], FALSE );
//}
//
//void ViewerBlur::drawEnd( ) {
//	draw( );
//}
//
//void ViewerBlur::drawMotionBlurBegin( ) {
//
//	// 描画ブレンドモードをセット
//	SetDrawBlendMode( DX_BLENDMODE_PMA_ALPHA, 255 );
//	
//	// 補間範囲とフォーカスが合っている範囲を含めた総距離を算出
//	DepthOfField_TotalSize = DepthOfField_InterpSize * 2.0f + DepthOfField_camera_targetSize;
//
//	// 通常描画用テクスチャにモデルを通常描画
//	DrawScreen( ColorMapTexture[ 0 ], FALSE );
//}
//
//void ViewerBlur::drawMotionBlurEnd( ) {
//	motionBlur( );
//}
//
//void ViewerBlur::resetMoitonBlur( ) {
//	for ( int i = 0; i < MAX_BLUR; i++ ) {
//		SetDrawScreen( OldMapTexture[ i ] );
//		ClearDrawScreen( );
//	}
//	SetDrawScreen( DX_SCREEN_BACK );
//}