#include "Oculus.h"
#include "Application.h"
#include "DxLib.h"

#ifdef OCULUS
#include "Exception.h"
#include "Drawer.h"
#include "ImageTarget.h"
#include "OVR_CAPI_D3D.h"
#include <assert.h>
#include <vector>
#include <array>

#include <d3d11.h>
#endif

const Vector Oculus::FORWARD = Vector( 0, 0, 1 );

#ifdef OCULUS

struct OculusTexture {
    ovrSession session;
	ovrTextureSwapChain texture_chain_rtv;
	ovrTextureSwapChain texture_chain_dsv;
	std::vector< ID3D11RenderTargetView* > tex_rtv;
	std::vector< ID3D11DepthStencilView* > tex_dsv;

	OculusTexture( ) :
	session( nullptr ),
	texture_chain_rtv( nullptr ),
	texture_chain_dsv( nullptr ),
	tex_rtv( ),
	tex_dsv( ) {
	};
	~OculusTexture( ) { };
	void init( ovrSession session_, int size_w, int size_h ) {
		session = session_;
		initRTV( size_w, size_h );
		initDSV( size_w, size_h );
	}

	void initRTV( int size_w, int size_h ) {
		ID3D11Device* device = ( ID3D11Device* )GetUseDirect3D11Device( );
		if ( device == nullptr ) {
			EXCEPTION( true, "ID3D11DeviceがNULLです" );
			return;
		}
		int sample_count = 1;
		//SwapChainの設定
		ovrTextureSwapChainDesc desc = {};
		desc.Type        = ovrTexture_2D;
		desc.Width       = size_w;
		desc.Height      = size_h;
		desc.ArraySize   = 1; //ovrTexture_2Dの場合１を指定する
		desc.MipLevels   = 1; //マルチサンプリングを行う場合１、サブテクスチャーの全セットを生成するには 0
		desc.SampleCount = sample_count; //マルチサンプリングを行う場合2以上、それ以外は1。2以上の場合描画されない
		desc.Format      = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
		desc.MiscFlags   = 0;//ovrTextureMisc_DX_Typeless | ovrTextureMisc_AutoGenerateMips;
		desc.BindFlags   = ovrTextureBind_DX_RenderTarget;
		desc.StaticImage = ovrFalse;

		//SwapChainの作成
		ovrResult result = ovr_CreateTextureSwapChainDX( session, device, &desc, &texture_chain_rtv );
		EXCEPTION( OVR_FAILURE( result ), "SwapChainの作成に失敗しました" );

		//SwapChain内のテクスチャの数の取得
		int texture_count = 0;
		ovr_GetTextureSwapChainLength( session, texture_chain_rtv, &texture_count );

		for ( int i = 0; i < texture_count; i++ ) {
			//SwapChainのID3D11Texture2Dの取得
			ID3D11Texture2D* tex = nullptr;
			ovr_GetTextureSwapChainBufferDX( session, texture_chain_rtv, i, IID_PPV_ARGS( &tex ) );

			//RenderTargetViewの設定
			D3D11_RENDER_TARGET_VIEW_DESC rtvd = { };
			rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			rtvd.ViewDimension = ( sample_count > 1) ? D3D11_RTV_DIMENSION_TEXTURE2DMS
                                                       : D3D11_RTV_DIMENSION_TEXTURE2D;

			//RenderTargetViewの作成
			ID3D11RenderTargetView* rtv;
			HRESULT hr = device->CreateRenderTargetView( tex, &rtvd, &rtv );
			EXCEPTION( hr != ERROR_SUCCESS, "RenderTargetViewの作成に失敗しました" );
		
			tex_rtv.push_back( rtv );
			tex->Release( );
		}
	}

	void initDSV( int size_w, int size_h ) {
		ID3D11Device* device = ( ID3D11Device* )GetUseDirect3D11Device( );
		if ( device == nullptr ) {
			EXCEPTION( true, "ID3D11DeviceがNULLです" );
			return;
		}

		//SwapChainの設定
		ovrTextureSwapChainDesc desc = {};
		desc.Type        = ovrTexture_2D;
		desc.Width       = size_w;
		desc.Height      = size_h;
		desc.ArraySize   = 1; //ovrTexture_2Dの場合１を指定する
		desc.MipLevels   = 1; //マルチサンプリングを行う場合１、サブテクスチャーの全セットを生成するには 0
		desc.SampleCount = 1; //マルチサンプリングを行う場合2以上、それ以外は1。2以上の場合描画されない
		desc.Format      = OVR_FORMAT_D32_FLOAT;
		desc.MiscFlags   = ovrTextureMisc_None;
		desc.BindFlags   = ovrTextureBind_DX_DepthStencil;
		desc.StaticImage = ovrFalse;
	
		//SwapChainの作成
		ovrResult result = ovr_CreateTextureSwapChainDX( session, device, &desc, &texture_chain_dsv );
		if ( !OVR_SUCCESS( result ) || !texture_chain_dsv ) {
			EXCEPTION( true, "SwapChainの作成に失敗しました" );
			return;
		}
		

		//SwapChain内のテクスチャの数の取得
		int texture_count = 0;
		ovr_GetTextureSwapChainLength( session, texture_chain_dsv, &texture_count );

		for ( int i = 0; i < texture_count; i++ ) {
			//SwapChainのID3D11Texture2Dの取得
			ID3D11Texture2D* tex = nullptr;
			ovr_GetTextureSwapChainBufferDX( session, texture_chain_dsv, i, IID_PPV_ARGS( &tex ) );

			D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = { };
			dsvd.Format = DXGI_FORMAT_D32_FLOAT;
			dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvd.Texture2D.MipSlice = 0;

			ID3D11DepthStencilView* dsv;
			HRESULT hr = device->CreateDepthStencilView( tex, &dsvd, &dsv );
			EXCEPTION( hr != ERROR_SUCCESS, "DepthStencilViewの作成に失敗しました" );
			tex_dsv.push_back( dsv );
			tex->Release( );
		}
	}
	
	ovrTextureSwapChain getTextureChainRTV( ) const {
		return texture_chain_rtv;
	}

	ovrTextureSwapChain getTextureChainDSV( ) const {
		return texture_chain_dsv;
	}

	void* getRTV( ) const {
		int idx = 0;
		ovr_GetTextureSwapChainCurrentIndex( session, texture_chain_rtv, &idx );
		return tex_rtv[ idx ];
	}
	
	void* getDSV( ) const {
		int idx = 0;
		ovr_GetTextureSwapChainCurrentIndex( session, texture_chain_dsv, &idx );
		return tex_dsv[ idx ];
	}

	void commit( ) {
		ovr_CommitTextureSwapChain( session, texture_chain_rtv );
		ovr_CommitTextureSwapChain( session, texture_chain_dsv );
	}
};

#endif

#ifdef OCULUS
class OculusImpl {
public:
	OculusImpl( ) :
	eye_render_viewport( ),
	eye_render_texture( ),
	eye_render_pose( ),
	eye_poses( ),
	eye_rots( ),
	now_input_state( ),
	before_input_state( ),
	mirror_texture( nullptr ),
	mirror_target( ),
	hmdDesc( ),
	sessionStatus( ),
	frame_index( 0 ),
	sensor_sample_time( 0 ) {
	};
	~OculusImpl( ) { };
public:
	std::array< ovrRecti, ovrEyeType::ovrEye_Count > eye_render_viewport;
	std::array< std::shared_ptr< OculusTexture >, ovrEyeType::ovrEye_Count > eye_render_texture;
	std::array< ovrPosef, ovrEyeType::ovrEye_Count > eye_render_pose;
	std::array< Vector, ovrEyeType::ovrEye_Count > eye_poses;
	std::array< Matrix, ovrEyeType::ovrEye_Count > eye_rots;
	ovrInputState now_input_state;
	ovrInputState before_input_state;
    ovrSession session;
	ovrMirrorTexture mirror_texture;
	ImageTargetPtr mirror_target;
	ovrHmdDesc hmdDesc;
	ovrSessionStatus sessionStatus;
	int frame_index;
	double sensor_sample_time;
};
#endif

Oculus::Oculus( ) :
_result( CREATE_RESULT_NONE ) {
#ifdef OCULUS
	{//LibOVRの初期化
		ovrInitParams initParams = { ovrInit_RequestVersion | ovrInit_FocusAware, OVR_MINOR_VERSION, NULL, 0, 0 };
		ovrResult oculus_initialize = ovr_Initialize( &initParams );
		if ( OVR_FAILURE( oculus_initialize ) ) {
			_result = CREATE_RESULT_FAIL_OVR_INITIALIZE;
			return;
			EXCEPTION( OVR_FAILURE( oculus_initialize ), "LibOVRの初期化に失敗しました" );
		}
	}

	{//VRセッションへのハンドルを作成
		ovrGraphicsLuid luid;
		_impl = OculusImplPtr( new OculusImpl );
		ovrResult oculus_create = ovr_Create( &_impl->session, &luid );
		//"VRセッションへのハンドルの作成に失敗しました";
		if ( OVR_FAILURE( oculus_create ) ) {
			_result = CREATE_RESULT_FAIL_OVR_CREATE;
			return;
		}
	}
	//HMDに関する情報の取得
	_impl->hmdDesc = ovr_GetHmdDesc( _impl->session );
	
	for ( int eye = 0; eye < ovrEyeType::ovrEye_Count; eye++ ) {
		_impl->hmdDesc.DefaultEyeFov[ eye ].LeftTan  = 0.964f;
		_impl->hmdDesc.DefaultEyeFov[ eye ].RightTan = 0.964f;
	}

	for( int eye = 0; eye < ovrEyeType::ovrEye_Count; eye++ ) {
		//テクスチャのサイズを取得
		ovrSizei idealSize = ovr_GetFovTextureSize( _impl->session, ( ovrEyeType )eye, _impl->hmdDesc.DefaultEyeFov[ eye ], 1.0f );
		//OculusTexture初期化
		_impl->eye_render_texture[ eye ] = std::shared_ptr< OculusTexture >( new OculusTexture( ) );
		_impl->eye_render_texture[ eye ]->init( _impl->session, idealSize.w, idealSize.h );
		//ビューポートの設定
		_impl->eye_render_viewport[ eye ].Pos.x = 0;
		_impl->eye_render_viewport[ eye ].Pos.y = 0;
		_impl->eye_render_viewport[ eye ].Size = idealSize;
	}
	
	{
		//ミラーテクスチャー設定
		ovrMirrorTextureDesc mirrorDesc = { };
		mirrorDesc.Format = ovrTextureFormat::OVR_FORMAT_R8G8B8A8_UNORM;
		mirrorDesc.Width  = _impl->hmdDesc.Resolution.w;
		mirrorDesc.Height = _impl->hmdDesc.Resolution.h;
		mirrorDesc.MirrorOptions = ovrMirrorOption_Default;
		mirrorDesc.MiscFlags = ovrTextureFlags::ovrTextureMisc_None;

		//ミラーテクスチャの作成
		ID3D11Device* device = ( ID3D11Device* )GetUseDirect3D11Device( );
		ovrResult mirror_texture_create = ovr_CreateMirrorTextureWithOptionsDX( _impl->session, device, &mirrorDesc, &_impl->mirror_texture );
		if ( OVR_FAILURE( mirror_texture_create ) ) {
			//const char* check = "ミラーテクスチャの作成に失敗しました";
			_result = CREATE_RESULT_FAIL_OVR_CREATE_MIRORR_TEX;
			return;
		}
		DrawerPtr drawer = Task::get< Drawer >( );
		_impl->mirror_target = drawer->createImageTarget( _impl->hmdDesc.Resolution.w, _impl->hmdDesc.Resolution.h );

	}

	// FloorLevel will give tracking poses where the floor height is 0
	//トラッキング方法の指定
    ovr_SetTrackingOriginType( _impl->session, ovrTrackingOrigin_FloorLevel );
	_result = CREATE_RESULT_SUCCESS;
#endif
}

Oculus::~Oculus( ) {
}

void Oculus::update( ) { 
#ifdef OCULUS
	if ( !isCreated( ) ) {
		return;
	}
	ovr_GetSessionStatus( _impl->session, &_impl->sessionStatus );
	if ( _impl->sessionStatus.ShouldQuit ) {
		return;
	}
	if ( _impl->sessionStatus.ShouldRecenter ) {
		ovr_RecenterTrackingOrigin( _impl->session );
	}
	if ( _impl->sessionStatus.IsVisible ) {
		ovrEyeRenderDesc eyeRenderDesc[ ovrEye_Count ];
		eyeRenderDesc[ 0 ] = ovr_GetRenderDesc( _impl->session, ovrEye_Left , _impl->hmdDesc.DefaultEyeFov[ ovrEye_Left  ] );
		eyeRenderDesc[ 1 ] = ovr_GetRenderDesc( _impl->session, ovrEye_Right, _impl->hmdDesc.DefaultEyeFov[ ovrEye_Right ] );

		ovrPosef HmdToEyePose[ ovrEye_Count ] = {
			eyeRenderDesc[ 0 ].HmdToEyePose,
			eyeRenderDesc[ 1 ].HmdToEyePose
		};
		ovr_GetEyePoses( _impl->session, _impl->frame_index, ovrTrue, HmdToEyePose, _impl->eye_render_pose.data( ), &_impl->sensor_sample_time );
		for ( int i = 0; i < EYE::MAX_EYE; i++ ) {
			EYE eye = ( EYE )i;
			_impl->eye_rots[ eye ] = convOrientToMatrix(
				_impl->eye_render_pose[ eye ].Orientation.x,
				_impl->eye_render_pose[ eye ].Orientation.y,
				_impl->eye_render_pose[ eye ].Orientation.z,
				_impl->eye_render_pose[ eye ].Orientation.w );
			_impl->eye_poses[ eye ] = convOvrVecToVector(
				_impl->eye_render_pose[ eye ].Position.x,
				_impl->eye_render_pose[ eye ].Position.y,
				_impl->eye_render_pose[ eye ].Position.z );
		}
	}
	_impl->before_input_state = _impl->now_input_state;
	ovr_GetInputState( _impl->session, ovrControllerType::ovrControllerType_Touch, &_impl->now_input_state );

#endif
}

bool Oculus::isCreated( ) const {
	return _result == CREATE_RESULT_SUCCESS;
}

void Oculus::flip( ) const {
#ifdef OCULUS
	if ( !_impl->sessionStatus.IsVisible ) {
		return;
	}
	// Initialize our single full screen Fov layer.
	ovrLayerEyeFovDepth ld = { };
	ld.Header.Type = ovrLayerType_EyeFovDepth;
	ld.Header.Flags = 0;

	for ( int eye = 0; eye < ovrEyeType::ovrEye_Count; eye++ ) {
		//描画情報をOculusへ送信
		_impl->eye_render_texture[ eye ]->commit( );
		//描画反映用の情報設定
		ld.ColorTexture[ eye ] = _impl->eye_render_texture[ eye ]->getTextureChainRTV( );
		ld.DepthTexture[ eye ] = _impl->eye_render_texture[ eye ]->getTextureChainDSV( );
		ld.Viewport[ eye ] = _impl->eye_render_viewport[ eye ];
		ld.Fov[ eye ] = _impl->hmdDesc.DefaultEyeFov[ eye ];
		ld.RenderPose[ eye ] = _impl->eye_render_pose[ eye ];
		ld.SensorSampleTime = _impl->sensor_sample_time;
	}
			
	ovrLayerHeader* layers = &ld.Header;
	ovrResult result = ovr_SubmitFrame( _impl->session, _impl->frame_index, nullptr, &layers, 1 );
	EXCEPTION( OVR_FAILURE( result ), "Submitに失敗しました" );// exit the rendering loop if submit returns an error, will retry on ovrError_DisplayLost

	_impl->frame_index++;
	mirror( );
#endif
}

Matrix Oculus::convOrientToMatrix( float orient_x, float orient_y, float orient_z, float orient_w ) const {
	Matrix result = Matrix( );
#ifdef OCULUS

	//右手座標から左手座標への変換
	orient_x *= -1;
	orient_y *= -1;
	orient_w *= -1;
	
	
	//orient_x軸
	result.data[ 0 ][ 0 ] = 1.0f - 2.0f * orient_y * orient_y - 2.0f * orient_z * orient_z;
	result.data[ 0 ][ 1 ] = 2.0f * orient_x * orient_y - 2.0f * orient_w * orient_z;
	result.data[ 0 ][ 2 ] = 2.0f * orient_x * orient_z + 2.0f * orient_w * orient_y;
	result.data[ 0 ][ 3 ] = 0;

 
	//orient_y軸
	result.data[ 1 ][ 0 ] = 2.0f * orient_x * orient_y + 2.0f * orient_w * orient_z;
	result.data[ 1 ][ 1 ] = 1.0f - 2.0f * orient_x * orient_x - 2.0f * orient_z * orient_z;
	result.data[ 1 ][ 2 ] = 2.0f * orient_y * orient_z - 2.0f * orient_w * orient_x;
	result.data[ 1 ][ 3 ] = 0;
	
 
	//orient_z軸
	result.data[ 2 ][ 0 ] = 2.0f * orient_x * orient_z - 2.0f * orient_w * orient_y;
	result.data[ 2 ][ 1 ] = 2.0f * orient_y * orient_z + 2.0f * orient_w * orient_x;
	result.data[ 2 ][ 2 ] = 1.0f - 2.0f * orient_x * orient_x - 2.0f * orient_y * orient_y;
	result.data[ 2 ][ 3 ] = 0;
	
	//w
	result.data[ 3 ][ 0 ] = 0;
	result.data[ 3 ][ 1 ] = 0;
	result.data[ 3 ][ 2 ] = 0;
	result.data[ 3 ][ 3 ] = 1;
#endif
	return result;
}

Vector Oculus::convOvrVecToVector( float x, float y, float z ) const {
	Vector vec = Vector( x, y, z );
	
	//右手座標から左手座標への変換
	vec.z *= -1;
	
	return vec;
}

void Oculus::mirror( ) const {	
#ifdef OCULUS
	//ミラーテクスチャーをコピー----------------------------------------
	int pre_screen = GetDrawScreen( );
	ID3D11Texture2D* tex;
	ovr_GetMirrorTextureBufferDX( _impl->session, _impl->mirror_texture, IID_PPV_ARGS( &tex ) );
	int handle = CreateGraphFromID3D11Texture2D( tex );
	if ( handle >= 0 ) {
		_impl->mirror_target->clear( );
		SetDrawScreen( _impl->mirror_target->getHandle( ) );
		DrawGraph( 0, 0, handle, FALSE );
		SetDrawScreen( pre_screen );
		tex->Release( );
	}
	//---------------------------------------
#endif
}

bool Oculus::isActiveTouch( const TOUCH touch ) const {
#ifdef OCULUS
	if ( touch == TOUCH_LEFT ) {
		return ( _impl->now_input_state.ControllerType & ovrControllerType_LTouch ) != 0;
	}
	if ( touch == TOUCH_RIGHT ) {
		return ( _impl->now_input_state.ControllerType & ovrControllerType_RTouch ) != 0;
	}
#endif
	return false;
}

bool Oculus::isPushButton( const BUTTON button ) const {
#ifdef OCULUS
	ovrButton ovr_button = ( ovrButton )0;
	switch ( button ) {
	case BUTTON_A:    ovr_button = ovrButton_A;     break;
	case BUTTON_B:    ovr_button = ovrButton_B;     break;
	case BUTTON_X:    ovr_button = ovrButton_X;     break;
	case BUTTON_Y:    ovr_button = ovrButton_Y;     break;
	case BUTTON_HOME: ovr_button = ovrButton_Enter; break;
	default:
		break;
	}

	return
		( _impl->before_input_state.Buttons & ovr_button ) == 0 &&
		( _impl->   now_input_state.Buttons & ovr_button ) != 0;

	
#else
	return false;
#endif
}

bool Oculus::isHoldButton( const BUTTON button ) const {
#ifdef OCULUS
	ovrButton ovr_button = ( ovrButton )0;
	switch ( button ) {
	case BUTTON_A:    ovr_button = ovrButton_A;     break;
	case BUTTON_B:    ovr_button = ovrButton_B;     break;
	case BUTTON_X:    ovr_button = ovrButton_X;     break;
	case BUTTON_Y:    ovr_button = ovrButton_Y;     break;
	case BUTTON_HOME: ovr_button = ovrButton_Enter; break;
	default:
		break;
	}

	return
		( _impl->before_input_state.Buttons & ovr_button ) != 0 &&
		( _impl->   now_input_state.Buttons & ovr_button ) != 0;
#else
	return false;
#endif
}

bool Oculus::isOnButton( const ON_BUTTON button ) const {
#ifdef OCULUS
	ovrTouch ovr_touch = ( ovrTouch )0;
	switch ( button ) {
	case ON_BUTTON_A               : ovr_touch = ovrTouch_A;             break;
	case ON_BUTTON_B               : ovr_touch = ovrTouch_B;             break;
	case ON_BUTTON_X               : ovr_touch = ovrTouch_X;             break;
	case ON_BUTTON_Y               : ovr_touch = ovrTouch_Y;             break;
	case ON_BUTTON_LEFT_TRIGGER    : ovr_touch = ovrTouch_LIndexTrigger; break;
	case ON_BUTTON_RIGHT_TRIGGER   : ovr_touch = ovrTouch_RIndexTrigger; break;
	case ON_BUTTON_LEFT_THUMB_PAD  : ovr_touch = ovrTouch_LThumb;        break;
	case ON_BUTTON_RIGHT_THUMB_PAD : ovr_touch = ovrTouch_RThumb;        break;
	case ON_BUTTON_LEFT_THUMB_REST : ovr_touch = ovrTouch_LThumbRest;    break;
	case ON_BUTTON_RIGHT_THUMB_REST: ovr_touch = ovrTouch_RThumbRest;    break;
	default:
		break;
	}
	return ( _impl->now_input_state.Touches & ovr_touch ) != 0;
#else
	return false;
#endif
}

int Oculus::getViewportWidth( ) const {
#ifdef OCULUS
	return _impl->eye_render_viewport[ 0 ].Size.w;
#else
	return 0;
#endif
}

int Oculus::getViewportHeight( ) const {
#ifdef OCULUS
	return _impl->eye_render_viewport[ 0 ].Size.h;
#else
	return 0;
#endif
}

double Oculus::getTriggerDown( const TOUCH touch ) const {
#ifdef OCULUS
	return _impl->now_input_state.IndexTrigger[ touch ];
#else
	return 0;
#endif
}

double Oculus::getHandTriggerDown( const TOUCH touch ) const {
#ifdef OCULUS
	return _impl->now_input_state.HandTrigger[ touch ];
#else
	return 0;
#endif
}

Vector Oculus::getThumbStick( const TOUCH touch ) const {
#ifdef OCULUS
	return Vector( _impl->now_input_state.Thumbstick[ touch ].x, _impl->now_input_state.Thumbstick[ touch ].y );
#else
	return Vector( );
#endif
}

Vector Oculus::getTouchPos( const TOUCH touch ) const {
#ifdef OCULUS
	ovrVector3f pos = ovr_GetTrackingState( _impl->session, ovr_GetTimeInSeconds( ), ovrTrue ).HandPoses[ touch ].ThePose.Position;
	ovrVector3f origin = ovr_GetTrackingState( _impl->session, ovr_GetTimeInSeconds( ), ovrTrue ).CalibratedOrigin.Position;
	return convOvrVecToVector( pos.x, pos.y, pos.z ) - convOvrVecToVector( origin.x, origin.y, origin.z );
#else
	return Vector( );
#endif
}

Matrix Oculus::getTouchRot( const TOUCH touch ) const {
#ifdef OCULUS
	ovrQuatf quatf = ovr_GetTrackingState( _impl->session, ovr_GetTimeInSeconds( ), ovrTrue ).HandPoses[ touch ].ThePose.Orientation;
	return convOrientToMatrix( quatf.x, quatf.y, quatf.z, quatf.w );
#else
	return Matrix( );
#endif
}

Vector Oculus::getHeadPos( ) const {
#ifdef OCULUS
	ovrVector3f pos = ovr_GetTrackingState( _impl->session, ovr_GetTimeInSeconds( ), ovrTrue ).HeadPose.ThePose.Position;
	ovrVector3f origin = ovr_GetTrackingState( _impl->session, ovr_GetTimeInSeconds( ), ovrTrue ).CalibratedOrigin.Position;
	return convOvrVecToVector( pos.x, pos.y, pos.z ) - convOvrVecToVector( origin.x, origin.y, origin.z );
#else
	return Vector( );
#endif
}

Matrix Oculus::getHeadRot( ) const {
#ifdef OCULUS
	ovrQuatf quatf = ovr_GetTrackingState( _impl->session, ovr_GetTimeInSeconds( ), ovrTrue ).HeadPose.ThePose.Orientation;
	return convOrientToMatrix( quatf.x, quatf.y, quatf.z, quatf.w );
#else
	return Matrix( );
#endif
}

Vector Oculus::getEyePos( EYE eye ) const {
#ifdef OCULUS
	return _impl->eye_poses[ eye ];
#else
	return Vector( );
#endif
}

Matrix Oculus::getEyeRot( EYE eye ) const {
#ifdef OCULUS
	return _impl->eye_rots[ eye ];
#else
	return Matrix( );
#endif
}

ImagePtr Oculus::getMirrorImage( ) const {
#ifdef OCULUS
	return _impl->mirror_target;
#else
	return ImagePtr( );
#endif
}

double Oculus::getEyeFov( EYE eye ) const {
#ifdef OCULUS
	return atan( _impl->hmdDesc.DefaultEyeFov[ eye ].LeftTan ) + atan( _impl->hmdDesc.DefaultEyeFov[ eye ].RightTan );
#else
	return 1.5;
#endif
}

double Oculus::getEyeNear( ) const {
	return 0.01;
}

double Oculus::getEyeFar( ) const {
	return 1000.0;
}

bool Oculus::isExistRenderTargetView( EYE eye ) const {
#ifdef OCULUS
	// RenderTargetViewの空きインデックスをidxに取得
	int idx = 0;
	ovrResult result = ovr_GetTextureSwapChainCurrentIndex( _impl->session, _impl->eye_render_texture[ eye ]->texture_chain_rtv, &idx );

	// エラーなら false になる
	return OVR_SUCCESS( result );
#else
	return false;
#endif
}

void Oculus::setTargetScreen( EYE eye ) {
#ifdef OCULUS
	int idx = 0;
	// RenderTargetViewの空きインデックスをidxに取得
	ovrResult result = ovr_GetTextureSwapChainCurrentIndex( _impl->session, _impl->eye_render_texture[ eye ]->texture_chain_rtv, &idx );

	if ( OVR_SUCCESS( result ) ) {
		SetDrawScreen_ID3D11RenderTargetView( 
			_impl->eye_render_texture[ eye ]->tex_rtv[ idx ],
			_impl->eye_render_texture[ eye ]->tex_dsv[ idx ]
		);
	}
#endif
}

Matrix Oculus::createProjectionMatrix( EYE eye ) const {
	Matrix result;
	DxLib::MATRIX dxlib_proj;
	DxLib::CreatePerspectiveFovMatrix( &dxlib_proj, ( float )getEyeFov( eye ), ( float )getEyeNear( ), ( float )getEyeFar( ), 1 );
	for ( int i = 0; i < 4; i++ ) {
		for ( int j = 0; j < 4; j++ ) {
			result.data[ i ][ j ] = dxlib_proj.m[ i ][ j ];
		}
	}
	return result;
}

void Oculus::vibration( TOUCH controllerType, float frequency, float amplitude ) {
#ifdef OCULUS
	ovr_SetControllerVibration(
		_impl->session,
		( controllerType == TOUCH_LEFT ) ? ovrControllerType::ovrControllerType_LTouch : ovrControllerType::ovrControllerType_RTouch,
		frequency,
		amplitude );
#endif // OCULUS
}
