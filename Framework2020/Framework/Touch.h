#pragma once

#include "Task.h"
#include "mathematics.h"
#include <string>
#include <vector>

PTR( Touch );

class Touch : public Task {
public:
	struct Data {
		Vector pos;
		int id;
		Data( ) { }
		Data( Vector pos_, int id_ ) : pos( pos_ ), id( id_ ) { }
	};
public:
	Touch( );
	virtual ~Touch( );
public:
	std::vector< Data > getData( ) const;
private:
	void update( );
private:
	std::vector< Data > _data;
};
