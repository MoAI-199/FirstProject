#pragma once

#include "Task.h"
#include "mathematics.h"
#include <string>

PTR( Drawer2D );

class Drawer2D : public Task {
public:
	void drawCircle( std::string name, const Vector2& pos, double radius );
	void drawLine( std::string name, const Vector2& pos1, const Vector2& pos2 );
public:
	Drawer2D( );
	virtual ~Drawer2D( );
private:
	void initialize( ) override;
	void update( ) override;
	void controllGizmo( );
	void drawGuide( );
private:
	int _origin_x;
	int _origin_y;
	bool _gizmo_translate_capturing;
	Vector2 _gizmo_translate_origin;
	int _dir_y;
	double _scale;
};

