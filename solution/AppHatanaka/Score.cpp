#include "Score.h"
#include "Time.h"
#include "Player.h"

Score::Score( InsidePtr inside, TimePtr time, PlayerPtr player ) :
	_inside( inside ),
	_time( time ),
	_player( player ) {

	_max_score = 0;
	

}

Score::~Score( ) {
}

void Score::initialize( ) {
	calculation( );
}

void Score::calculation( ) {
	if ( _player->isClear( ) ) {
		//����
		int cleartime = _time->getCountGame( );
		int time_score =  cleartime * 100;
		//�ύX��
		int lost_count = _inside->getLost( ) ;
		int inside_score = 4000 - lost_count * 400;
		_max_score = time_score + inside_score;
	}
}

int Score::getScore( ) {
	return _max_score;
}
