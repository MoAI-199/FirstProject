#include "Looper.h"
#include "DxLib.h"
#include "Exception.h"
#include "Sound.h"

Looper::Looper( ) {
	for ( int i = 0; i < 16 * 4; i++ ) {
		for ( int j = 0; j < YUTA_NUM; j++ ) {
			_yuta[ j ].note[ i ] = false;
		}
	}
	_yuta_num = 0;
}

Looper::~Looper( ) {
}

void Looper::stamp( ) {
	_stamp_on_beats = _on_beats;
	_stamp_bpm      = _bpm;
	_stamp_beats    = _beats;
	_stamp_ratio    = _ratio;
	_stamp_time     = _time;
}

void Looper::stopAll( ) {
	_yuta_num = 0;
}

void Looper::play( std::string filename ) {
	for ( int i = 0; i < _yuta_num; i++ ) {
		if ( _yuta[ i ].filename == filename ) {
			return;
		}
	}

	std::string path = _directory + "yuta/" + filename;
	int handle = FileRead_open( path.c_str( ) );
	if ( handle == 0 ) {
		path = "../" + path;
		handle = FileRead_open( path.c_str( ) );
		if ( handle == 0 ) {
			EXCEPTION( true, "Looper ERROR : cant open file" );
		}
	}
	EXCEPTION( _yuta_num >= YUTA_NUM, "Looper ERROR : yuta capacity full" );

	_yuta[ _yuta_num ].filename = filename;

	char buf[ 256 ];

	FileRead_gets( buf, 256, handle );
	_yuta[ _yuta_num ].instrument = buf;

	for ( int j = 0; j < 4; j++ ) {
		FileRead_gets( buf, 256, handle );
		for ( int i = 0; i < 16; i++ ) {
			_yuta[ _yuta_num ].note[ i + j * 16 ] = ( buf[ i ] == '*' );
		}
	}
	
	_yuta[ _yuta_num ].ready = true;

	_yuta_num++;
}

void Looper::stop( std::string filename ) {
	int idx = -1;
	for ( int i = 0; i < _yuta_num; i++ ) {
		if ( _yuta[ i ].filename == filename ) {
			idx = i;
		}
	}
	if ( idx == -1 ) {
		return;
	}
	_yuta_num--;
	for ( int i = idx; i < _yuta_num; i++ ) {
		_yuta[ i ] = _yuta[ i + 1 ];
	}
}

void Looper::initialize( ) {
	_bpm = 100;
	_beats = 0;
	_time = 0;
	_ratio = 0;
	_last_count = GetNowCount( );
}

void Looper::begin( ) {
	_beginning = true;
}

void Looper::update( ) {
	int now_count = GetNowCount( );
	int count = now_count - _last_count;
	_real_time = count / 1000.0;
	_on_beats = false;

	if ( _bpm == 0 ) {
		_last_count = now_count;
		_time = 0;
		_ratio = 0;
		return;
	}

	double pitch = ( 60 * 1000 ) / ( _bpm * 4 );
	_time = count / pitch;

	if ( _beginning ) {
		_beginning  = false;
		_on_beats = true;
		_time = 1.0;
		_ratio = 0;
		_beats = 0;
		playSoundOnBeat( );
	} else if ( _time >= 1.0 ) {
		_on_beats = true;
		_time = 1.0;
		_ratio = 0;
		_beats++;
		playSoundOnBeat( );
	} else {
		_ratio += _time;
		if ( _ratio >= 1.0 ) {
			_on_beats = true;
			_ratio -= 1.0;
			_beats++;
			playSoundOnBeat( );
		}
	}

	_last_count = now_count;
}

bool Looper::isOnBeats( ) const {
	return _stamp_on_beats;
}

double Looper::getRealTime( ) const {
	return _real_time;
}

double Looper::getTime( ) const {
	return _stamp_time;
}

int Looper::getBeats( ) const {
	return _stamp_beats;
}

int Looper::getYutaNum( ) const {
	return _yuta_num;
}

double Looper::getRatio( ) const {
	return _stamp_ratio;
}

int Looper::getBPM( ) const {
	return _stamp_bpm;
}

void Looper::setBPM( int bpm ) {
	_bpm = bpm;
	if ( _bpm < 0 ) {
		_bpm = 0;
	}
}

void Looper::setDirectory( std::string path ) {
	_directory = path;
}

std::string Looper::getFilename( int idx ) const {
	return _yuta[ idx ].filename;
}

std::string Looper::getInstrument( int idx ) const {
	return _yuta[ idx ].instrument;
}

bool Looper::isNote( int yuta_idx, int note_idx ) const {
	return _yuta[ yuta_idx ].note[ note_idx ];
}

void Looper::playSoundOnBeat( ) {
	SoundPtr sound = Task::get< Sound >( );
	for ( int i = 0; i < _yuta_num; i++ ) {
		if ( _beats % 32 == 0 ) {
			_yuta[ i ].ready = false;
		}
		if ( _yuta[ i ].ready ) {
			continue;
		}
		if ( !_yuta[ i ].note[ _beats % 64 ] ) {
			continue;
		}
		std::string path = _directory + "instrument/" + _yuta[ i ].instrument;
		sound->playSE( path.c_str( ) );
	}
}
