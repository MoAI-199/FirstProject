#pragma once

#include "smart_ptr.h"
#include "mathematics.h"
#include "Drawer.h"
#include <array>

PTR( Image );

class Sprites {
	friend class Drawer;
public:
	struct Primitive {
		struct Vertex {
			Vector pos;
			Color color;
			double u;
			double v;
		};
		std::array< Vertex, 4 > vertex;
	};
public:
	virtual ~Sprites( );
	void showOff( int idx );
	void add( const Vector& pos );
	void add( const Vector& pos, double scale );
	void add( const Vector& pos, double scale, int anime );
	void add( const Vector& pos, double scale, int anime, double angle );
	void draw( );
	void draw( int count );
	void clear( );
	void addPrimitive( const Primitive& primitive );
protected:
	Sprites( int count, ImagePtr image, int num );
private:
	const int COUNT;
	const int NUM;
	ImagePtr _image;
	void* _vertex;
	int _width;
	int _height;
	int _pitch;
	int _count;
};

