#include "Billboard.h"
#include "DxLib.h"
#include "Application.h"

Billboard::Billboard( ) :
_cx_ratio( 0 ),
_cy_ratio( 0 ),
_size_ratio( 1 ),
_angle( 0 ),
_trans( false ),
_pos( Vector( ) ),
_image( ImagePtr( ) ) {
}

Billboard::~Billboard( ) { 
}

void Billboard::draw( ) const {
	VECTOR pos = VGet( ( float )_pos.x, ( float )_pos.y, ( float )_pos.z );
	DrawBillboard3D( pos, ( float )_cx_ratio, ( float )_cy_ratio, ( float )_size_ratio, ( float )_angle, _image->getHandle( ), _trans );
}

void Billboard::setImage( ImagePtr image ) {
	_image = image;
}

void Billboard::setPos( const Vector& pos ) {
	_pos = pos;
}

void Billboard::setImageCenterPos( double cx_ratio, double cy_ratio ) {
	_cx_ratio = cx_ratio;
	_cy_ratio = cy_ratio;
}

void Billboard::setImageSize( double size_ratio ) {
	_size_ratio = size_ratio;
}

void Billboard::setImageRotationAngle( double radian ) {
	_angle = radian;
}

void Billboard::setTrans( bool trans ) {
	_trans = trans;
}
