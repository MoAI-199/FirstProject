#pragma once
#include "Task.h"
#include <string>
#include <array>

PTR( Looper );

class Looper : public Task {
public:
    Looper( );
    virtual ~Looper( );
public:
	static const int YUTA_NUM = 5;
public: // write
    void play( std::string filename );
	void stop( std::string filename );
	void setDirectory(std::string path);
	void setBPM( int bpm );
	void stamp( );
	void stopAll( );
public: // read only
	int getBPM( ) const;
	int getBeats( ) const;
	double getRatio( ) const;
	double getTime( ) const;
	double getRealTime( ) const;
	std::string getInstrument( int idx ) const;
	std::string getFilename( int idx ) const;
	bool isNote( int yuta_idx, int note_idx ) const;
	int getYutaNum( ) const;
	bool isOnBeats( ) const;
	void begin( );
private:
	struct YUTA {
		std::string filename;
		std::string instrument;
		std::array< bool, 16 * 4 > note;
		bool ready;
	};
private:
	void initialize( ) override;
	void update( ) override;
	void playSoundOnBeat( );
private:
	std::string _directory;
	int _last_count;
	std::array< YUTA, YUTA_NUM > _yuta;
	int _yuta_num;
	bool _beginning;
	bool _on_beats;
	int _bpm;
	int _beats;
	double _ratio;
	double _time;
	double _real_time;
	bool   _stamp_on_beats;
	int    _stamp_bpm;
	int    _stamp_beats;
	double _stamp_ratio;
	double _stamp_time;
};

