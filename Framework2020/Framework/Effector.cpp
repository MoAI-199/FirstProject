#include "Effector.h"
#include "Application.h"
#include "DxLib.h"
#include "Effect.h"
#include "Exception.h"


#ifndef EFFEKSEER

Effector::Effector( const std::string& directory ) :
_directory( directory ),
_resource_handles( ),
_effekseer_graph( -1 ) {
}

Effector::~Effector( ) {
}

EffectPtr Effector::createEffect( const std::string& directory ) { return EffectPtr( ); }

void Effector::drawEffect( ) const { }

#else

#include "EffekseerForDXLib.h"

#include <assert.h>

const int PARTICLE = 3000; //Effectorの最大パーティクル数

Effector::Effector( const std::string& directory ) :
_directory( directory ),
_resource_handles( ),
_effekseer_graph( -1 ) {
	// DirectX9を使用するようにする。
	// Effectorを使用するには必ず設定する。
	//SetUseDirect3DVersion(DX_DIRECT3D_9);


	// Effectorを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if ( Effkseer_Init( PARTICLE ) == -1 ) {
		return;
	}


	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effectorを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag( FALSE );

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions( );
}

Effector::~Effector( ) {
	Effkseer_End( );
}


EffectPtr Effector::createEffect( const std::string& filename ) {
	if ( _resource_handles.count( filename ) == 0 ) {
		std::string path = _directory + "/" + filename;
		int handle = LoadEffekseerEffect( path.c_str( ) );
		if ( handle < 0 ) {
			path = ( std::string )"../" + path;
			handle = LoadEffekseerEffect( path.c_str( ) );
			EXCEPTION( handle < 0, filename );
		}
		_resource_handles[ filename ] = handle;
	}

	EffectPtr result( new Effect( _resource_handles[ filename ] ) );
	return result;
}

void Effector::drawEffect( ) const {
	CHECKDRAWING
	//DrawGraph( 0, 0, _effekseer_graph, TRUE );
	RenderVertex( );

	// EffectorをDxLibのカメラと同期させる
	Effekseer_Sync3DSetting( );

	// Effectorにより再生中のエフェクトを更新する。
	UpdateEffekseer3D( );

	// Effectorにより再生中のエフェクトを描画する。
	DrawEffekseer3D( );
}

#endif