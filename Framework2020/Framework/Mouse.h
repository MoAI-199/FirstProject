#pragma once
#include "smart_ptr.h"
#include "Task.h"
#include "mathematics.h"
#include <string>

PTR( Mouse );

enum MOUSE {
	MOUSE_LEFT_BUTTON,
	MOUSE_RIGHT_BUTTON,
	MAX_MOUSE,
};

class Mouse : public Task {
public:
	Mouse( );
	virtual ~Mouse( );
public:
	void update( );
	void setDispFlag( bool flag );
	Vector2 getPos( ) const;
	void setPos( const Vector2& pos );
	int getWheelRotValue( ) const;
	bool isPushRightButton( ) const;
	bool isHoldRightButton( ) const;
	bool isPushLeftButton( ) const;
	bool isHoldLeftButton( ) const;
private:
	int _wheel_value;
	Vector2 _pos;
	int _key[ MAX_MOUSE ];
};
