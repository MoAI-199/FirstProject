#pragma once
#include "Map.h"

PTR( Map1 );

class Map1 : public Map {
public:
	Map1( );
	virtual ~Map1( );
private:
	std::string initializeMap( ) override;
};

