#pragma once
#include <string>
#include "smart_ptr.h"
#include "mathematics.h"
#include "Color.h"

PTR( Image );

class Image {
	friend class Drawer;
public:
	enum BLEND {
		BLEND_NONE,
		BLEND_ALPHA,
		BLEND_ADD,
		BLEND_SUB,
		MAX_BLEND,
	};
public:
	virtual ~Image( );
	void draw( int x, int y ) const;
	void draw( const Vector2& pos ) const;
	void drawExtend( int x1, int y1, int x2, int y2 ) const;
	void drawExtend( const Vector2& pos1, const Vector2& pos2 ) const;
	void drawRotation( int x, int y, double angle ) const;
	void drawRotation( const Vector2& pos, double angle ) const;
	void drawRect( int x, int y, int tx, int ty, int tw, int th ) const;
	void drawRect( const Vector2& pos, int tx, int ty, int tw, int th ) const;
	void drawRectExtend( int x1, int y1, int x2, int y2, int tx, int ty, int tw, int th ) const;
	void drawRectExtend( const Vector2& pos1, const Vector2& pos2, int tx, int ty, int tw, int th ) const;
	void drawRectRotation( int x, int y, double scale, double angle, int tx, int ty, int tw, int th, bool rev_x = false, bool rev_y = false ) const;
	void drawRectRotation( const Vector2& pos1, double scale, double angle, int tx, int ty, int tw, int th, bool rev_x = false, bool rev_y = false ) const;
	void getImageSize( int& width, int& height ) const;
	int getHandle( ) const;
	int getWidth( ) const;
	int getHeight( ) const;
protected:
	Image( );
	void setHandle( int handle );
	void setWidth( int width );
	void setHeight( int height );
private:
	bool load( std::string path );
private:
	int _width;
	int _height;
	int _handle;
	int _sx1;
	int _sy1;
	int _sx2;
	int _sy2;
	int _tx;
	int _ty;
	int _tw;
	int _th;
	BLEND _blend;
	double _ratio;
};