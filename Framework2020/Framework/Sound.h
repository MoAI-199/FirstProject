#pragma once

#include "Task.h"
#include "smart_ptr.h"
#include <map>
#include <string>
#include <array>

PTR( Sound );

class Sound : public Task {
public:
	Sound( );
	virtual ~Sound( );
public:
	void update( );
	bool isPlayingSE( std::string filename );
	void playSE( std::string filename, bool loop = false );
	void stopSE( std::string filename );
	void loadSE( std::string filename );
	void stopAllSE( );
	void playBGM( std::string filename, bool loop = true );
	void stopBGM( );
	void setVolumeBGM( double volume );
	void set3DSound( bool flag );
	void set3DSoundOneMetre( double distance );
	void set3DSEPosition( const Vector& pos, const std::string& filename );
	void set3DBGMPosition( const Vector& pos );
	void set3DSEValidityRange( double radius, const std::string& filename );
	void set3DBGMValidityRange( double radius );
	void set3DSoundListenerPosAndFrontPosAndUpVec( const Vector& pos, const Vector& front_pos, const Vector& up_vector );
	void set3DSoundListenerVelocity( const Vector& velocity_per_sec );
	bool isPlayingBGM( );
	int getPlayingTimeBGM( );
private:
	std::map< std::string, int > _se;
	int _bgm;
};

