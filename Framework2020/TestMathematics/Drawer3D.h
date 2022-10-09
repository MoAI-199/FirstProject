#pragma once

#include "Task.h"
#include "mathematics.h"
//#include "activecf.h"
#include <string>

PTR( Drawer3D );

class Drawer3D : public Task {
public:
	void drawSphere( std::string name, const Vector& pos, double radius );
	void drawLine( std::string name, const Vector& pos1, const Vector& pos2 );
public:
	Drawer3D( );
	virtual ~Drawer3D( );
private:
	void initialize( ) override;
	void update( ) override;
	void controllCamera( );
	void drawGuide( );
private:
	bool _camera_rotation_capturing;
	Vector2 _camera_rotation_origin;
	Vector _camera_pos;
	Vector _camera_dir;
	Vector _guide_length;
};

