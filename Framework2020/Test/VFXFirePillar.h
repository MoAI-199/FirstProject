#pragma once
#include "smart_ptr.h"
#include "mathematics.h"
#include <array>
#include <vector>

PTR( VFXFirePillar );
PTR( Billboard );

const int NUM_OF_FIRE = 64;
const int HEIGHT_OF_FIRE = 16;

class VFXFirePillar {
public:
	VFXFirePillar( Vector pos );
	virtual ~VFXFirePillar( );
public:
	void draw( );
	void fireElement( double radius, double height );
	void drawFire( double raidus );
private:
	std::array< BillboardPtr, NUM_OF_FIRE > _billboard;
	//std::vector< std::array< BillboardPtr, NUM_OF_FIRE > > _draw;
	double _angle;
	Vector _center;
};

