#pragma once
#include "Task.h"
#include <string>
#include <array>

PTR( Log );

class Log : public Task {
public:
	Log( );
	virtual ~Log( );
public:
	void draw( );
	void toggleShow( );
	void off( );
	void chat( const char * format, ... );
	void status( const char * format, ... );
private:
	static const int LINES = 30;
private:
	void update( ) override;
	void drawStatus( );
	void drawChat( );
private:
	std::array< std::string, LINES > _line_chat;
	std::array< std::string, LINES > _line_status;
	int _status_idx;
	bool _show;
};

