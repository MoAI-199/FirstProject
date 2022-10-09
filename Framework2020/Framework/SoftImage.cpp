#include "SoftImage.h"
#include "DxLib.h"

SoftImage::SoftImage( ) :
_handle( -1 ) {
}


SoftImage::~SoftImage( ) {
	if ( _handle >= 0 ) {
		DeleteSoftImage( _handle );
	}
}

bool SoftImage::load( std::string path ) {
	_handle = LoadSoftImage( path.c_str( ) );
	if ( _handle < 0 ) {
		return false;
	}

	GetGraphSize( _handle, &_width, &_height );

	return true;
}

void SoftImage::extractPixel( int x, int y, int& r, int& g, int& b, int& a ) {
	GetPixelSoftImage( _handle, x, y, &r, &g, &b, &a);
}
