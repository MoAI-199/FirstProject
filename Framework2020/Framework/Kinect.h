#pragma once
#include "Task.h"
#include "mathematics.h"
#include "Color.h"
#include <string>

PTR( Kinect );
PTR( KinectImpl );

class Kinect : public Task {
public:
	static const int DEPTH_WIDTH = 512;
	static const int DEPTH_HEIGHT = 424;
	static const int COLOR_WIDTH = 1920;
	static const int COLOR_HEIGHT = 1080;
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
	Kinect( );
	virtual ~Kinect( );
public:
	void update( );
	Color getColor( int x, int y ) const;
	double getDepth( int x, int y ) const;
	bool updateColor( );
	bool updateDepth( );
	bool isExistJointPoint( int body_index, JOINT joint );
	Vector getJointPoint( int body_index, JOINT joint );
	Vector getJointDepthPoint( int body_index, JOINT joint );
	Vector getJointColorPoint( int body_index, JOINT joint );
	HAND_STATE getHandState( int body_index, HAND_TYPE type );
	void convPosDepthToColor( int depth_x, int depth_y, int& color_x, int& color_y ) const;
private:
	KinectImplPtr _impl;
};

