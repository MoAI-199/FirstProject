#pragma once
#include "smart_ptr.h"
#include "mathematics.h"
#include < array >
#include <map>
#include <string>

class Map {
public:
	Map( std::string map );
	virtual ~Map( );
public:
	static const int MAP_WIDTH = 120;
	static const int MAP_HEIGHT = 68;
	static const int WALL_SIZE = 16;

	enum class WALL {
		INSIDE1,
		INSIDE2,
		INSIDE3,
		INSIDE4,
		INSIDE5,
		INSIDE6,
		INSIDE7,
		INSIDE8,
		INSIDE9,
		INSIDE10,
		NONE,
		FRAME

	};

public:
	WALL getWallType( const Vector2& pos ) const;
	bool isOverlappedWall( const Vector2& pos ) const;
	std::array<WALL, 10 > _inside_num;
protected:
	virtual	std::string initializeMap( ) = 0;
	const std::string MAP;
private:
	std::map< char, WALL > _wall_type;
};