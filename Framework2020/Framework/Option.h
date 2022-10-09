#pragma once

#include "Task.h"
#include <string>
#include <map>

PTR( Option );

class Option : public Task {
public:
	Option( std::string filepath );
	virtual ~Option( );
public:
	void load( const std::string& filepath );
	const int getData( const std::string& tag ) const;
private:
	void update( ) override;
private:
private:
	std::map< std::string, int > _data;
};

