#pragma once
#include "smart_ptr.h"
#include "mathematics.h"
#include <vector>

PTR( Billboard );

class Fire {
public:
	Fire( const Vector& pos );
	virtual ~Fire( );
public:
	void draw( );
private:
	void createFire( Vector pos );
private:
	std::vector< BillboardPtr > _fire;
};

