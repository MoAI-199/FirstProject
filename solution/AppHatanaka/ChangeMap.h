#pragma once
#include "smart_ptr.h"
#include "Task.h"

PTR( ChangeMap );

class ChangeMap : public Task {
public :
	ChangeMap( );
	virtual ~ChangeMap( );
public:
	void setMapNum( int num );
	int getMapNum( );
private:
	int _map_num;
};

