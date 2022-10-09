#include "Touch.h"
#include "Application.h"
#include "DxLib.h"


Touch::Touch( ) {

}

Touch::~Touch( ) {
	_data.clear( );
}

void Touch::update( ) {
	_data.clear( );
	int count = 0;
	while ( true ) {
		int x = 0;
		int y = 0;
		int id = 0;
		if ( GetTouchInput( count, &x, &y, &id, NULL ) == -1 ) {
			break;
		}
		_data.push_back( Data( Vector( x, y ), id ) );
		count++;
	}
}

std::vector< Touch::Data > Touch::getData( ) const {
	return _data;
}

