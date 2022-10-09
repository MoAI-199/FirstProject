#pragma once
#include "Task.h"
#include "mathematics.h"
#include "smart_ptr.h"
#include <string>

PTR( Gesture );
PTR( GestureImpl );

class Gesture : public Task {
public:
	static const int MAX_BODY = 6;
	enum JOINT {
		JOINT_HEAD,
		JOINT_SPINE_SHOULDER,
		JOINT_SHOULDER_RIGHT,
		JOINT_SHOULDER_LEFT,
		JOINT_HAND_RIGHT,
		JOINT_HAND_LEFT,
		JOINT_FOOT_RIGHT,
		JOINT_FOOT_LEFT,
		MAX_JOINT,
		JOINT_NONE,
	};

	enum HAND_STATE {
		HAND_STATE_UNKNOWN,
		HAND_STATE_NOTTRACKED,
		HAND_STATE_OPEN,
		HAND_STATE_CLOSE,
		HAND_STATE_LASSO, // ó÷Ç¡Ç©ÅH
	};

	enum HAND_TYPE {
		HAND_TYPE_LEFT,
		HAND_TYPE_RIGHT,
		HAND_TYPE_MAX
	};
public:
	static std::string getTag( ) { return "GESTURE"; };
	static GesturePtr getTask( );
public:
	Gesture( );
	virtual ~Gesture( );
public:
	void update( );
	bool isExistJointPoint( int body_index, JOINT joint );
	Vector getJointPoint( int body_index, JOINT joint );
	Vector getJointDepthPoint( int body_index, JOINT joint );
	Vector getJointColorPoint( int body_index, JOINT joint );
	HAND_STATE getHandState( int body_index, HAND_TYPE type );
private:
	GestureImplPtr _impl;
};

