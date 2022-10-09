#pragma once
#include "Map.h"

PTR( Map2 );

class Map2 : public Map {
public:
	Map2( );
	virtual ~Map2( );
private:
	std::string initializeMap( ) override;
};

