#pragma once
#include "mathematics.h"
#include "Task.h"
#include <string>

PTR( Oculus );
PTR( OculusImpl );
PTR( ImageTarget );
PTR( Image );

class Oculus : public Task {
public:
	static const Vector FORWARD;
public:
	Oculus( );
	virtual ~Oculus( );
public:
	enum EYE {
		EYE_LEFT,
		EYE_RIGHT,
		MAX_EYE,
	};
	enum TOUCH {
		TOUCH_LEFT,
		TOUCH_RIGHT,
		MAX_TOUCH
	};
	enum BUTTON {
		BUTTON_A,
		BUTTON_B,
		BUTTON_X,
		BUTTON_Y,
		BUTTON_HOME,
		MAX_BUTTON
	};

	enum ON_BUTTON {
		ON_BUTTON_A,
		ON_BUTTON_B,
		ON_BUTTON_X,
		ON_BUTTON_Y,
		ON_BUTTON_LEFT_TRIGGER,
		ON_BUTTON_RIGHT_TRIGGER,
		ON_BUTTON_LEFT_THUMB_PAD,
		ON_BUTTON_RIGHT_THUMB_PAD,
		ON_BUTTON_LEFT_THUMB_REST,
		ON_BUTTON_RIGHT_THUMB_REST,
		MAX_ON_BUTTON
	};
	enum CREATE_RESULT {
		CREATE_RESULT_NONE,
		CREATE_RESULT_SUCCESS,
		CREATE_RESULT_FAIL_OVR_CREATE,
		CREATE_RESULT_FAIL_OVR_INITIALIZE,
		CREATE_RESULT_FAIL_OVR_CREATE_MIRORR_TEX,
		MAX_CREATE_RESULT,
	};
public:
	bool isCreated( ) const;
	void flip( ) const;
	bool isActiveTouch( TOUCH touch ) const;
	bool isPushButton( BUTTON button ) const;
	bool isHoldButton( BUTTON button ) const;
	bool isOnButton( ON_BUTTON button ) const;
	int getViewportWidth( ) const;
	int getViewportHeight( ) const;
	double getTriggerDown( TOUCH touch ) const;
	double getHandTriggerDown( TOUCH touch ) const;
	Vector getThumbStick( TOUCH touch ) const;
	Vector getTouchPos( TOUCH touch ) const;
	Matrix getTouchRot( TOUCH touch ) const;
	Vector getHeadPos( ) const;
	Matrix getHeadRot( ) const;
	Vector getEyePos( EYE eye ) const;
	Matrix getEyeRot( EYE eye ) const;
	ImagePtr getMirrorImage( ) const;
	double getEyeFov( EYE eye ) const;
	double getEyeNear( ) const;
	double getEyeFar( ) const;
	bool isExistRenderTargetView( EYE eye ) const;
	void setTargetScreen( EYE eye );
	Matrix createProjectionMatrix( EYE eye ) const;
	void vibration( TOUCH controllerType, float frequency, float amplitude );
private:
	void update( ) override;
	Matrix convOrientToMatrix( float orient_x, float orient_y, float orient_z, float orient_w ) const;
	Vector convOvrVecToVector( float x, float y, float z ) const;
	void mirror( ) const;
private:
	OculusImplPtr _impl;
	CREATE_RESULT _result;
};

