#include "VFXMirror.h"
#include "Drawer.h"
#include "Application.h"
#include "mathematics.h"
#include "Primitive.h"
#include "Primitive3DShader.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ImageTarget.h"

const char* VERTEX_SHADER_MIRROR_VS = "MirrorVS.vso";
const char* PIXEL_SHADER_MIRROR_PS = "MirrorPS.pso";

VFXMirror::VFXMirror( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	ApplicationPtr app = Application::getInstance( );

	// 鏡処理を行うための頂点シェーダーとピクセルシェだーを読み込む
	//_vertex_shader = LoadVertexShader( "resource/shader/MirrorVS.vso" );
	_vertex_shader = drawer->createVertexShader( VERTEX_SHADER_MIRROR_VS );
	//_pixel_shader = LoadPixelShader( "resource/shader/MirrorPS.pso" );
	_pixel_shader = drawer->createPixelShader( PIXEL_SHADER_MIRROR_PS );

	_primitive_3d_shader = drawer->createPrimitive3DShader( );

	// 鏡に映る映像の取得に使用するスクリーンの作成
	_width = app->getScreenWidth( );
	_height = app->getScreenHeight( );
	//_handle = MakeScreen( MIRROR_SCREEN_W, MIRROR_SCREEN_H, FALSE );
	_image_target = drawer->createImageTarget( _width, _height );
}

VFXMirror::~VFXMirror( ) {
}

void VFXMirror::draw( const Vector eye, const Vector focus ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	// カメラの設定
	_camera_pos = Vector( ( double )eye.x, ( double )eye.y, ( double )eye.z );
	_camera_target = Vector( ( double )focus.x, ( double )focus.y, ( double )focus.z );

	// 描画先を鏡に映る映像の取得に使用する画像を描画するスクリーンにする
	//SetDrawScreen( _handle );
	//ClearDrawScreen( );
	drawer->setTarget( scope, _image_target );
	drawer->clearScreen( );

	// 鏡に映る映像を描画する際に使用するカメラの設定を行う
	setupCamera( _camera_pos, _camera_target );

	drawer->drawSphere( Vector( 15.5, 1, 15.5 ), 1, 15, Color( 1.0, 0.0, 0.0, 1 ), false );

	// 裏画面を描画対象にする
	//SetDrawScreen( DX_SCREEN_BACK );
	drawer->setTarget( scope );

	// カメラを元に戻す
	//SetCameraNearFar( 10.0f, 200.0f );
	drawer->setNearFar( 10.0, 200.0 );
	//SetCameraPositionAndTarget_UpVecY( _camera_pos, _camera_target );
	drawer->setCameraUp( Vector( 0, 1, 0 ) );
	drawer->setCamera( _camera_pos, _camera_target );
	//SetupCamera_Perspective( 12.0f * DX_PI_F / 180.0f );
	drawer->setCameraPerspective( 12.0 * PI / 180.0 );

	// 描画する
	render( );
}

void VFXMirror::setPos( Vector pos, int width, int depth ) {
	_world_pos[ 0 ] = Vector( ( double )pos.x, ( double )pos.y, ( double )pos.z + depth );
	_world_pos[ 1 ] = Vector( ( double )pos.x + width, ( double )pos.y, ( double )pos.z + depth );
	_world_pos[ 2 ] = Vector( ( double )pos.x, ( double )pos.y, ( double )pos.z );
	_world_pos[ 3 ] = Vector( ( double )pos.x + width, ( double )pos.y, ( double )pos.z );
}

void VFXMirror::setupCamera( Vector CameraEyePos, Vector CameraTargetPos ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	float EyeLength, TargetLength;
	Vector MirrorNormal;
	Vector* MirrorWorldPosP;
	Vector MirrorCameraEyePos, MirrorCameraTargetPos;

	MirrorWorldPosP = _world_pos;

	// 鏡の面の法線を算出
	//MirrorNormal = VNorm( VCross( VSub( MirrorWorldPosP[ 1 ], MirrorWorldPosP[ 0 ] ), VSub( MirrorWorldPosP[ 2 ], MirrorWorldPosP[ 0 ] ) ) );
	Vector vtr = Vector( );
	vtr = MirrorWorldPosP[ 1 ].sub( MirrorWorldPosP[ 0 ] );
	vtr.cross( MirrorWorldPosP[ 2 ].sub( MirrorWorldPosP[ 0 ] ) );
	vtr.normalize( );
	MirrorNormal = vtr;


	////
	// v = CameraTargetPos - MirrorWorldPosP[ 0 ];
	// n = MirrorNormal.normalize( );
	// L = n.dot( v );
	////
	// 鏡の面からカメラの座標までの最短距離、鏡の面からカメラの注視点までの最短距離を算出
	//EyeLength = Plane_Point_MinLength( MirrorWorldPosP[ 0 ], MirrorNormal, CameraEyePos );
	vtr = Vector( );
	Vector v = Vector( );
	v = CameraTargetPos.sub( MirrorWorldPosP[ 0 ] );
	vtr = MirrorNormal.normalize( );
	EyeLength = ( float )vtr.dot( v );

	//TargetLength = Plane_Point_MinLength( MirrorWorldPosP[ 0 ], MirrorNormal, CameraTargetPos );
	vtr = Vector( );
	v = Vector( );
	v = CameraTargetPos.sub( MirrorWorldPosP[ 0 ] );
	vtr = MirrorNormal.normalize( );
	TargetLength = ( float )vtr.dot( v );

	// 鏡に映る映像を描画する際に使用するカメラの座標とカメラの注視点を算出
	//MirrorCameraEyePos = VAdd( CameraEyePos, VScale( MirrorNormal, -EyeLength * 2.0f ) );
	vtr = Vector( );
	vtr = CameraEyePos.add( MirrorNormal.multiply( -EyeLength * 2.0f ) );
	MirrorCameraEyePos = vtr;
	

	//MirrorCameraTargetPos = VAdd( CameraTargetPos, VScale( MirrorNormal, -TargetLength * 2.0f ) );
	vtr = Vector( );
	vtr = CameraTargetPos.add( MirrorNormal.multiply( -TargetLength * 2.0f ) );
	MirrorCameraTargetPos = vtr;

	// 計算で得られたカメラの座標とカメラの注視点の座標をカメラの設定する
	//SetCameraPositionAndTarget_UpVecY( MirrorCameraEyePos, MirrorCameraTargetPos );
	drawer->setCameraUp( Vector( 0, 1, 0 ) );   //???
	drawer->setCamera( MirrorCameraEyePos, MirrorCameraTargetPos );

	// 鏡に映る映像の中での鏡の四隅の座標を算出( 同次座標 )
	for ( int i = 0; i < 4; i++ ) {
		_screen_pos[ i ] = drawer->getScreenPosPlusW( MirrorWorldPosP[ i ] );
	}
}

void VFXMirror::render( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );

	// 鏡の描画に使用する頂点のセットアップ
	for ( int i = 0; i < 4; i++ ) {
		Primitive::VERTEX vtx;
		// 座標をセット
		//_vertex[ i ].pos = _world_pos[ i ];
		vtx.pos = _world_pos[ i ];
		// Diffuse Color をセット
		//_vertex[ i ].dif = GetColorU8( 200, 200, 200, 150 );
		vtx.color = Color( 0.788, 0.788, 0.788, 0.59 );

		Quaternion qtn;
		// 補助座標に鏡に映る視点での射影座標を代入する
		//_vertex[ i ].spos.x = _screen_pos[ i ].x / MIRROR_SCREEN_W;
		qtn.x = _screen_pos[ i ].x / _width;
		//_vertex[ i ].spos.y = _screen_pos[ i ].y / MIRROR_SCREEN_H;
		qtn.y = _screen_pos[ i ].y / _height;
		//_vertex[ i ].spos.z = _screen_pos[ i ].z;
		qtn.z = _screen_pos[ i ].z;
		//_vertex[ i ].spos.w = _screen_pos[ i ].w;
		qtn.w = _screen_pos[ i ].w;
		_primitive_3d_shader->alloc( 2 );
		_primitive_3d_shader->set( i, vtx );
		_primitive_3d_shader->setSpos( qtn );
	}
	//_vertex[ 4 ] = _vertex[ 2 ];
	_primitive_3d_shader->setVTX3D( 4, _primitive_3d_shader->getVTX3D( 2 ) );
	//_vertex[ 5 ] = _vertex[ 1 ];
	_primitive_3d_shader->setVTX3D( 5, _primitive_3d_shader->getVTX3D( 1 ) );

	// 描画ブレンドモードをセット
	//SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 );
	drawer->setBlend( scope, Drawer::BLEND::ALPHA, 1.0 );

	// 使用する頂点シェーダーをセット
	//SetUseVertexShader( _vertex_shader );
	_vertex_shader->useVertexShader( );

	// 使用するピクセルシェーダーをセット
	//SetUsePixelShader( _pixel_shader );
	_pixel_shader->usePixelShader( );

	// 使用するテクスチャをセット
	//SetUseTextureToShader( 0, _handle );
	_pixel_shader->setUseTexture( 0, _image_target );


	// 鏡を描画
	//DrawPolygon3DToShader( _vertex, 2 );
	PrimitivePtr primitive = drawer->createPrimitive( );
	primitive->setNum( 2 );
	_primitive_3d_shader->draw( primitive );

	// 描画ブレンドモードを元に戻す
	//SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
	drawer->setBlend( scope, Drawer::BLEND::NONE, 1.0 );

}
/*


// 座標指定用関数
void ViewerMirror::setPos( VECTOR pos, int width, int depth ) {
	_world_pos[ 0 ] = VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z + depth );
	_world_pos[ 1 ] = VGet( ( float )pos.x + width, ( float )pos.y, ( float )pos.z + depth );
	_world_pos[ 2 ] = VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z );
	_world_pos[ 3 ] = VGet( ( float )pos.x + width, ( float )pos.y, ( float )pos.z );

}


void ViewerMirror::render( ) {

	// 鏡の描画に使用する頂点のセットアップ
	for( int i = 0; i < 4; i ++ )
	{
		// 座標をセット
		_vertex[ i ].pos = _world_pos[ i ];

		// Diffuse Color をセット
		_vertex[ i ].dif = GetColorU8( 200, 200, 200, 150 );

		// 補助座標に鏡に映る視点での射影座標を代入する
		_vertex[ i ].spos.x = _screen_pos[ i ].x / MIRROR_SCREEN_W;
		_vertex[ i ].spos.y = _screen_pos[ i ].y / MIRROR_SCREEN_H;
		_vertex[ i ].spos.z = _screen_pos[ i ].z;
		_vertex[ i ].spos.w = _screen_pos[ i ].w;
	}
	_vertex[ 4 ] = _vertex[ 2 ];
	_vertex[ 5 ] = _vertex[ 1 ];

	// 描画ブレンドモードをセット
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 ) ;

	// 使用する頂点シェーダーをセット
	SetUseVertexShader( _vertex_shader );

	// 使用するピクセルシェーダーをセット
	SetUsePixelShader( _pixel_shader );

	// 使用するテクスチャをセット
	SetUseTextureToShader( 0, _handle );

	// 鏡を描画
	DrawPolygon3DToShader( _vertex, 2 );

	// 描画ブレンドモードを元に戻す
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 ) ;

	// シェーダーを消す
	SetUseVertexShader( -1 );
	SetUsePixelShader( -1 );

}

void ViewerMirror::drawBegin( const Vector eye, const Vector focus ) {
	// カメラの設定
	_camera_pos = VGet( ( float )eye.x, ( float )eye.y, ( float )eye.z );
	_camera_target = VGet( ( float )focus.x, ( float )focus.y, ( float )focus.z );

	// 描画先を鏡に映る映像の取得に使用する画像を描画するスクリーンにする
	SetDrawScreen( _handle );
	ClearDrawScreen( );

	// 鏡に映る映像を描画する際に使用するカメラの設定を行う
	setupCamera( _camera_pos, _camera_target );
}

void ViewerMirror::drawEnd( ) {

	// 裏画面を描画対象にする
	SetDrawScreen( DX_SCREEN_BACK );

	// カメラを元に戻す
	SetCameraNearFar( 10.0f, 200.0f );
	SetCameraPositionAndTarget_UpVecY( _camera_pos, _camera_target );
	SetupCamera_Perspective( 12.0f * DX_PI_F / 180.0f );

	// 描画する
	render( );
}
*/
