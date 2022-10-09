#include "Image.h"
#include "DxLib.h"
#include "Application.h"

Image::Image( ) :
	_handle( -1 ),
	_width( 0 ),
	_height( 0 ) {
}


Image::~Image( ) {
	if ( _handle >= 0 ) {
		DeleteGraph( _handle );
	}
}

bool Image::load( std::string path ) {
	_handle = LoadGraph( path.c_str( ) );
	if ( _handle < 0 ) {
		return false;
	}

	GetGraphSize( _handle, &_width, &_height );

	return true;
}

void Image::draw( int x, int y ) const {
	DrawGraph( x, y, _handle, TRUE );
}

void Image::draw( const Vector2& pos ) const {
	draw( ( int )pos.x, ( int )pos.y );
}

void Image::drawExtend( int x1, int y1, int x2, int y2 ) const {
	DrawExtendGraph( x1, y1, x2, y2, _handle, TRUE );
}

void Image::drawExtend( const Vector2& pos1, const Vector2& pos2 ) const {
	drawExtend(
		( int )pos1.x, ( int )pos1.y,
		( int )pos2.x, ( int )pos2.y );
}

void Image::drawRotation( int x, int y, double angle ) const {
	DrawRotaGraph( x, y, 1.0, angle, _handle, TRUE );
}

void Image::drawRotation( const Vector2& pos, double angle ) const {
	drawRotation( ( int )pos.x, ( int )pos.y, angle );
}

void Image::drawRect( int x, int y, int tx, int ty, int tw, int th ) const {
	DrawRectGraph( x, y, tx, ty, tw, th, _handle, TRUE );
}

void Image::drawRect( const Vector2& pos, int tx, int ty, int tw, int th ) const {
	drawRect( ( int )pos.x, ( int )pos.y, tx, ty, tw, th );
}

void Image::drawRectExtend( int x1, int y1, int x2, int y2, int tx, int ty, int tw, int th ) const {
	DrawRectExtendGraph( x1, y1, x2, y2, tx, ty, tw, th, _handle, TRUE );
}

void Image::drawRectExtend( const Vector2& pos1, const Vector2& pos2, int tx, int ty, int tw, int th ) const {
	drawRectExtend(
		( int )pos1.x, ( int )pos1.y,
		( int )pos2.x, ( int )pos2.y,
		tx, ty, tw, th );
}

void Image::drawRectRotation( int x, int y, double scale, double angle, int tx, int ty, int tw, int th, bool rev_x, bool rev_y ) const {
	DrawRectRotaGraph( x, y, tx, ty, tw, th,
		scale, angle, _handle, TRUE, rev_x, rev_y );
}

void Image::drawRectRotation( const Vector2& pos, double scale, double angle, int tx, int ty, int tw, int th, bool rev_x, bool rev_y ) const {
	drawRectRotation(
		( int )pos.x, ( int )pos.y, scale, angle, 
		tx, ty, tw, th, rev_x, rev_y );
}

void Image::getImageSize( int& width, int& height ) const {
	GetGraphSize( _handle, &width, &height );
}

int Image::getHandle( ) const {
	return _handle;
}

void Image::setHandle( int handle ) {
	_handle = handle;
}

void Image::setWidth( int width ) {
	_width = width;
}

void Image::setHeight( int height ) {
	_height = height;
}

int Image::getWidth( ) const {
	return _width;
}

int Image::getHeight( ) const {
	return _height;
}
