#pragma once
#include "mathematics.h"
#include "smart_ptr.h"
#include "Image.h"

PTR( Image );

class Billboard {
public:
	Billboard( );
	virtual ~Billboard( );
public:
	void draw( ) const;
	void setImage( ImagePtr image );
	void setPos( const Vector& pos );
	void setImageCenterPos( double cx, double cy );
	void setImageSize( double size );
	void setImageRotationAngle( double radian );
	void setTrans( bool trans );
private:
	double _cx_ratio;
	double _cy_ratio;
	double _size_ratio;
	double _angle;
	bool _trans;
	Vector _pos;
	ImagePtr _image;
};

