#include "inside.h"
#include "Keyboard.h"
#include "Map.h"
#include "Define.h"
#include "ChangeMap.h"

Inside::Inside( MapPtr map ) :
	_target_num( 0 ),
	_change_count( 5 ),
	_map( map ) {
	for ( int i = 0; i < INSIDE_NUM; i++ ) {
		_status[ i ] = STATUS::NORMAL;
	}
	_lost_count = 0;
	_max_inside = 9;

}



Inside::~Inside( ) {
}

void Inside::update( ) {

}

void Inside::changeTarget( STATUS status ) {
	KeyboardPtr key = Task::get< Keyboard >( );
	ChangeMapPtr change = Task::get< ChangeMap >( );

	switch ( change->getMapNum( ) ) {
	case 1:
		 _max_inside = 6;
		break;
	case 2:
		_max_inside = 9;
		break;
	}

	if ( Inside::STATUS::FLASHING != status ) {
		if ( key->isPushKey( "D" ) ) {
			
			if ( _target_num < _max_inside ) {

				_target_num++;
			}
		}

		if ( key->isPushKey( "A" ) ) {

			if ( _target_num > 0 ) {

				_target_num--;
			}
		}
	}


}

Inside::STATUS Inside::changeStatus( STATUS status ) {
	KeyboardPtr key = Task::get< Keyboard >( );

	switch ( status ) {
	case STATUS::NORMAL:
		if ( key->isPushKey( "W" ) ) {
			status = STATUS::FLASHING;
		}
		break;
	case STATUS::FLASHING:
		if ( key->isPushKey( "W" ) ) {
			status = STATUS::NORMAL;
		}

		if ( _change_count > 0 ) {
			if ( key->isPushKey( "S" ) ) {
				status = STATUS::LOST;
				_change_count--;
				_lost_count++;
			}
		}

		break;
	case STATUS::LOST:
		break;

	}
	return status;
}

void Inside::inside( ) {
	_status[ _target_num ] = changeStatus( _status[ _target_num ] );

}

bool Inside::isClear( ) const {
	return false;
}

int Inside::getTarget( ) const {
	return _target_num;
}

int Inside::getChangeCount( ) const {
	return _change_count;
}

bool Inside::isOverlappedStatus( ) const {
	return false;
}

Inside::STATUS Inside::getStatus( int num ) const {
	return _status[ num ];
}

int Inside::getLost( ) {
	return _lost_count;
}

