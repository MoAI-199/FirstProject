#include "Sound.h"
#include "Application.h"
#include "Dxlib.h"
#include <string>

Sound::Sound( ) :
_bgm( -1 ) {
}

Sound::~Sound( ) {

}

void Sound::update( ) {
}

bool Sound::isPlayingSE( std::string filename ) {
	if ( _se.count( filename ) == 0 ) {
		return false;
	}

	return CheckSoundMem( _se[ filename ] ) == 1;
}

void Sound::loadSE( std::string filename ) {
	if ( _se.count( filename ) != 0 ) {
		return;
	}

	SetCreateSoundDataType( DX_SOUNDDATATYPE_MEMNOPRESS );

	std::string path = filename;
	int id = LoadSoundMem( path.c_str( ) );
	if ( id < 0 ) {
		path = "../" + path;
		id = LoadSoundMem( path.c_str( ) );
	}
	_se[ filename ] = id;
}

void Sound::playSE( std::string filename, bool loop ) {
	loadSE( filename );

	if ( loop ) {
		PlaySoundMem( _se[ filename ], DX_PLAYTYPE_LOOP );
	} else {
		PlaySoundMem( _se[ filename ], DX_PLAYTYPE_BACK );
	}
}

void Sound::stopSE( std::string filename ) {
	if ( _se.count( filename ) == 0 ) {
		return;
	}

	StopSoundMem( _se[ filename ] );
}

void Sound::playBGM( std::string filename, bool loop ) {
	stopBGM();

	SetCreateSoundDataType( DX_SOUNDDATATYPE_FILE );

	std::string path = filename;
	int id = LoadSoundMem( path.c_str( ) );
	if ( id < 0 ) {
		path = "../" + path;
		id = LoadSoundMem( path.c_str( ) );
	}

	_bgm = id;

	int flg = DX_PLAYTYPE_BACK;
	if ( loop ) {
		flg = DX_PLAYTYPE_LOOP;
	}

	PlaySoundMem( _bgm, flg );
}

void Sound::setVolumeBGM( double volume ) {
	if ( _bgm < 0 ) {
		return;
	}
	ChangeVolumeSoundMem( ( int )( volume * 255 ), _bgm );
}

void Sound::stopBGM( ) {
	if ( _bgm < 0 ) {
		return;
	}
	DeleteSoundMem( _bgm );
	_bgm = -1;
}

void Sound::stopAllSE( ) {
	std::map< std::string, int >::iterator it = _se.begin( );
	while ( it != _se.end( ) ) {
		StopSoundMem( ( *it ).second );
		it++;
	}
}

bool Sound::isPlayingBGM( ) {
	return CheckSoundMem( _bgm ) == 1;
}

int Sound::getPlayingTimeBGM( ) {
	return ( int )GetSoundCurrentTime( _bgm );
}

void Sound::set3DSound( bool flag ) {
	SetCreate3DSoundFlag( flag );
}

void Sound::set3DSoundOneMetre( const double distance ) {
	Set3DSoundOneMetre( ( float )distance );
}

void Sound::set3DSEPosition( const Vector& pos, const std::string& filename ) {
	Set3DPositionSoundMem( VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z ), _se[ filename ] ); 
}

void Sound::set3DBGMPosition( const Vector& pos ) {
	Set3DPositionSoundMem( VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z ), _bgm ); 
}

void Sound::set3DSEValidityRange( double radius, const std::string& filename ) {
	Set3DRadiusSoundMem( ( float )radius, _se[ filename ] );
}

void Sound::set3DBGMValidityRange( double radius ) {
	Set3DRadiusSoundMem( ( float )radius, _bgm );
}

void Sound::set3DSoundListenerPosAndFrontPosAndUpVec( const Vector& pos, const Vector& front_pos, const Vector& up_vector ) {
	VECTOR dx_pos = VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z );
	VECTOR dx_front_pos = VGet( ( float )front_pos.x, ( float )front_pos.y, ( float )front_pos.z );
	VECTOR dx_up_vec = VGet( ( float )up_vector.x, ( float )up_vector.y, ( float )up_vector.z );
	Set3DSoundListenerPosAndFrontPosAndUpVec( dx_pos, dx_front_pos, dx_up_vec );
}

void Sound::set3DSoundListenerVelocity( const Vector& velocity_per_sec ) {
	// à¯êîÇÃíPà ÇÕ1ïbìñÇΩÇËÇÃë¨ìx
	Set3DSoundListenerVelocity( VGet( ( float )velocity_per_sec.x, ( float )velocity_per_sec.y, ( float )velocity_per_sec.z ) );
}
