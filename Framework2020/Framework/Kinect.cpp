#include "Kinect.h"
#include "Application.h"

#ifdef KINECT
////////////////////////////////////////////////////////////////////
//
// KinectImpl ( with Kinect )
//
////////////////////////////////////////////////////////////////////
#include <kinect.h>
#include <assert.h>
#include <array>
#include <map>

#pragma comment( lib, "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/lib/x64/kinect20.lib" )

const int SKIP_COUNT = 5;

class KinectImpl {
public:
	KinectImpl( ) { }
	virtual ~KinectImpl( ) { }
public:
	void initialize( ) {
		initializeKinectSensor( );
		initializeBodyReader( );
		initializeColorReader( );
		initializeDepthReader( );
		errorCheck( _kinect->get_CoordinateMapper( &_mapper ) );
	}
	void finalize( ) {
		delete _data_color;
		delete _data_depth;
		_kinect->Close( );
		_kinect->Release( );
		_bodyFrameReader->Release( );
		_color_source->Release( );
		_color_reader->Release( );
		_depth_source->Release( );
		_depth_reader->Release( );
	}
public:
	bool isExistJointPoint( int body_index, Kinect::JOINT joint ) {
		return _joints[ body_index ][ conv( joint ) ].TrackingState != TrackingState_NotTracked;
	}

	Vector getJointPoint( int body_index, Kinect::JOINT joint ) {
		CameraSpacePoint pos = _joints[ body_index ][ conv( joint ) ].Position;
		return Vector( pos.X, pos.Y, pos.Z );
	}
	Vector getJointDepthPoint( int body_index, Kinect::JOINT joint ) {
		DepthSpacePoint point;
		_mapper->MapCameraPointToDepthSpace( _joints[ body_index ][ conv( joint ) ].Position, &point );
		return Vector( point.X, point.Y );
	}
	Vector getJointColorPoint( int body_index, Kinect::JOINT joint ) {
		ColorSpacePoint point;
		_mapper->MapCameraPointToColorSpace( _joints[ body_index ][ conv( joint ) ].Position, &point );
		return Vector( point.X, point.Y );
	}

	Kinect::HAND_STATE getHandState( int body_index, Kinect::HAND_TYPE type ) {
		return convHandStateForKinect( _hand_state[ body_index ][ type ] );
	};


	void convPosDepthToColor( int depth_x, int depth_y, int& color_x, int& color_y ) const {
		DepthSpacePoint depth_point;
		depth_point.X = ( float )depth_x;
		depth_point.Y = ( float )depth_y;
		ColorSpacePoint color_point;
		int idx = ( depth_x + depth_y * Kinect::DEPTH_WIDTH );
		UINT16 depth = _data_depth[ idx ];
		_mapper->MapDepthPointToColorSpace( depth_point, depth, &color_point );
		color_x = ( int )color_point.X;
		color_y = ( int )color_point.Y;
	}

	void updateBodyFrame( ) {

		IBodyFrame* body_frame;
		HRESULT result = _bodyFrameReader->AcquireLatestFrame( &body_frame );
		if ( result != S_OK ) {

			return;
		}

		for ( int i = 0; i < BODY_COUNT; i++ ) {
			for ( int j = 0; j < JointType::JointType_Count; j++ ) {
				_joints[ i ][ j ].TrackingState = TrackingState_NotTracked;
			}
		}

		IBody* bodies[ BODY_COUNT ] = {nullptr};
		body_frame->GetAndRefreshBodyData( BODY_COUNT, bodies );
		

		//ボディデータに変換
		for ( int i = 0; i < BODY_COUNT; i++ ) {
			if ( bodies[ i ] == nullptr ) {
				continue;
			}

			BOOLEAN tracked = false;
			bodies[ i ]->get_IsTracked( &tracked );
			if ( !tracked ) {
				continue;
			}

			Joint joints[ JointType::JointType_Count ];
			bodies[ i ]->GetJoints( JointType::JointType_Count, joints );

			for ( int j = 0; j < JointType::JointType_Count; j++ ) {
				_joints[ i ][ joints[ j ].JointType ] = joints[ j ];

				// 手の状態の取得
				if ( ( JointType )j == JointType::JointType_HandLeft ) {
					bodies[ i ]->get_HandLeftState( &_hand_state[ i ][ Kinect::HAND_TYPE_LEFT ] );
				}
				if ( ( JointType )j == JointType::JointType_HandRight ) {
					bodies[ i ]->get_HandRightState( &_hand_state[ i ][ Kinect::HAND_TYPE_RIGHT ] );
				}
			}
		}

		body_frame->Release( );
	}

	Color getColor( int x, int y ) const {
		int idx = ( x + y * Kinect::COLOR_WIDTH ) * 4;
		return Color(
			_data_color[ idx + 2 ] / 255.0,
			_data_color[ idx + 1 ] / 255.0,
			_data_color[ idx + 0 ] / 255.0 );
	}

	double getDepth( int x, int y ) const {
		if ( x < 0 || x >= Kinect::DEPTH_WIDTH ||
			 y < 0 || y >= Kinect::DEPTH_HEIGHT ) {
			return 0;
		}
		int idx = ( x + y * Kinect::DEPTH_WIDTH );
		return _data_depth[ idx ];
	}

	bool updateColor( ) {
		IColorFrame* color_frame = nullptr;
		HRESULT hResult = _color_reader->AcquireLatestFrame( &color_frame );
		if ( hResult != S_OK ) {
			return false;
		}

		const unsigned int BUFFERSIZE = Kinect::COLOR_WIDTH * Kinect::COLOR_HEIGHT * 4 * sizeof( unsigned char );
		hResult = color_frame->CopyConvertedFrameDataToArray( BUFFERSIZE, reinterpret_cast< BYTE* >( _data_color ), ColorImageFormat_Bgra );
		color_frame->Release( );
		
		return true;
	}

	bool updateDepth( ) {
		IDepthFrame* depth_frame = nullptr;
		HRESULT hResult = _depth_reader->AcquireLatestFrame( &depth_frame );
		if ( hResult != S_OK ) {
			return false;
		}

		const unsigned int BUFFERSIZE = Kinect::DEPTH_WIDTH * Kinect::DEPTH_HEIGHT;
		hResult = depth_frame->CopyFrameDataToArray( BUFFERSIZE, reinterpret_cast< UINT16 * >( _data_depth ) );
		depth_frame->Release( );
		
		return true;
	}

private:
	JointType conv( Kinect::JOINT joint ) {
		switch ( joint ) {
		case Kinect::JOINT_HEAD:
			return JointType_Head;
		case Kinect::JOINT_SPINE_SHOULDER:
			return JointType_SpineMid;

		case Kinect::JOINT_SHOULDER_RIGHT:
			return JointType_ShoulderRight;
		case Kinect::JOINT_SHOULDER_LEFT:
			return JointType_ShoulderLeft;

		case Kinect::JOINT_HAND_RIGHT:
			return JointType_HandRight;
		case Kinect::JOINT_HAND_LEFT:
			return JointType_HandLeft;
		case Kinect::JOINT_FOOT_RIGHT:
			return JointType_FootRight;
		case Kinect::JOINT_FOOT_LEFT:
			return JointType_FootLeft;
		}

		return JointType_Head;
	}

	Kinect::HAND_STATE convHandStateForKinect( HandState hand_state ) {
		Kinect::HAND_STATE state = Kinect::HAND_STATE_UNKNOWN;
		switch ( hand_state ) {
		case HandState_Unknown   : state = Kinect::HAND_STATE_UNKNOWN   ; break;
		case HandState_NotTracked: state = Kinect::HAND_STATE_NOTTRACKED; break;
		case HandState_Open      : state = Kinect::HAND_STATE_OPEN      ; break;
		case HandState_Closed    : state = Kinect::HAND_STATE_CLOSE     ; break;
		case HandState_Lasso     : state = Kinect::HAND_STATE_LASSO     ; break;

		default:
			break;
		}

		return state;
	}

	void initializeKinectSensor( ) {
		errorCheck( ::GetDefaultKinectSensor( &_kinect ) );//センサーの取得
		errorCheck( _kinect->Open( ) );// Kinectセンサーを動作させる

		// Kinectセンサーが動いたかどうか状態を取得する
		BOOLEAN isOpen = false;
		errorCheck( _kinect->get_IsOpen( &isOpen ) );
		if ( !isOpen ) {
			errorCheck( S_FALSE );
		}
	}
	void initializeBodyReader( ) {
		IBodyFrameSource* bodyFrameSource = nullptr;
		errorCheck( _kinect->get_BodyFrameSource( &bodyFrameSource ) );
		errorCheck( bodyFrameSource->OpenReader( &_bodyFrameReader ) );
	}

	void initializeColorReader( ) {
		errorCheck( _kinect->get_ColorFrameSource( &_color_source ) );
		errorCheck( _color_source->OpenReader( &_color_reader ) );

		_data_color = new unsigned char[ Kinect::COLOR_WIDTH * Kinect::COLOR_HEIGHT * 4 ];

	}

	void initializeDepthReader( ) {
		errorCheck( _kinect->get_DepthFrameSource( &_depth_source ) );
		errorCheck( _depth_source->OpenReader( &_depth_reader ) );

		_data_depth = new unsigned short[ Kinect::DEPTH_WIDTH * Kinect::DEPTH_HEIGHT ];	
	}

	bool errorCheck( HRESULT result ) {
		assert( S_OK == result );
		return S_OK == result;
	}



private:
	IKinectSensor* _kinect;
	IBodyFrameReader* _bodyFrameReader;
	IColorFrameSource* _color_source;
	IColorFrameReader* _color_reader;
	IDepthFrameSource* _depth_source;
	IDepthFrameReader* _depth_reader;
	ICoordinateMapper* _mapper;
	Joint _joints[ BODY_COUNT ][ JointType::JointType_Count ];
	HandState _hand_state[ BODY_COUNT ][ Kinect::HAND_TYPE_MAX ];
	unsigned char * _data_color;
	unsigned short * _data_depth;
};

#else

////////////////////////////////////////////////////////////////////
//
// KinectImpl ( without Kinect )
//
////////////////////////////////////////////////////////////////////
class KinectImpl {
public:
	KinectImpl( ) { }
	virtual ~KinectImpl( ) { }
	void initialize( ) { }
	void finalize( ) { }
	void updateBodyFrame( ) { }
	bool isExistJointPoint( int, Kinect::JOINT ) { return false; }
	Vector getJointPoint( int, Kinect::JOINT ) { return Vector( ); }
	Vector getJointColorPoint( int, Kinect::JOINT ) { return Vector( ); }
	Vector getJointDepthPoint( int, Kinect::JOINT ) { return Vector( ); }
	Kinect::HAND_STATE getHandState( int, Kinect::HAND_TYPE ) { return Kinect::HAND_STATE_UNKNOWN; }
	Color getCoor( int, int ) { return Color( ); }
	bool updateColor( ) { return false;  }
	bool updateDepth( ) { return false;  }
	Color getColor( int x, int y ) const { return Color( ); }
	double getDepth( int x, int y ) const { return 1; }
	void convPosDepthToColor( int depth_x, int depth_y, int& color_x, int& color_y ) const { }
};

#endif


////////////////////////////////////////////////////////////////////
//
// Kinect
//
////////////////////////////////////////////////////////////////////

Kinect::Kinect( ) {
	_impl = KinectImplPtr( new KinectImpl( ) );
	_impl->initialize( );
}

Kinect::~Kinect( ) {
	_impl->finalize( );
}

void Kinect::update( ) {
	_impl->updateDepth( );
}

Color Kinect::getColor( int x, int y ) const {
	return _impl->getColor( x, y );
}

bool Kinect::updateColor( ) {
	return _impl->updateColor( );
}

double Kinect::getDepth( int x, int y ) const {
	return _impl->getDepth( x, y );

}

bool Kinect::updateDepth( ) {
	return _impl->updateDepth( );
}

bool Kinect::isExistJointPoint( int body_index, JOINT joint ) {
	return _impl->isExistJointPoint( body_index, joint );
}

Vector Kinect::getJointPoint( int body_index, JOINT joint ) {
	return _impl->getJointPoint( body_index, joint );
}


Vector Kinect::getJointDepthPoint( int body_index, Kinect::JOINT joint ) {
	return _impl->getJointDepthPoint( body_index, joint );
}

Vector Kinect::getJointColorPoint( int body_index, Kinect::JOINT joint ) {
	return _impl->getJointColorPoint( body_index, joint );
}

Kinect::HAND_STATE Kinect::getHandState( int body_index, Kinect::HAND_TYPE type ) {
	return _impl->getHandState( body_index, type );
}

void Kinect::convPosDepthToColor( int depth_x, int depth_y, int& color_x, int& color_y ) const {
	_impl->convPosDepthToColor( depth_x, depth_y, color_x, color_y );
}