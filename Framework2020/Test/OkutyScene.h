#pragma once
#include "Scene.h"

PTR( Billboard );

class OkutyScene : public Scene {
public:
	OkutyScene( );
	virtual ~OkutyScene( );
private:
	void initialize( ) override;
	void update( ) override;
private:
	void drawFire( int idx );
private:
	BillboardPtr _billboard;
	double _rot;
	double _height;
};

