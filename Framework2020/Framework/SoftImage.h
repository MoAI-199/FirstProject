#pragma once

#include <string>

class SoftImage {
	friend class Drawer;
public:
	virtual ~SoftImage( );
protected:
	SoftImage( );
private:
	bool load( std::string path );
	void extractPixel( int x, int y, int& r, int& g, int& b, int& a );
private:
	int _handle;
	int _width;
	int _height;
};

