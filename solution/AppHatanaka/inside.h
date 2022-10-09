#pragma once
#include "smart_ptr.h"
#include <array>

PTR( Inside );
PTR( Map );

class Inside {
public:
	Inside( MapPtr map );
	virtual ~Inside( );
public:
	enum class STATUS {
		NORMAL,
		FLASHING,
		LOST
	};
public:
	void update( );
	void inside( );
	void changeTarget( STATUS status );
	STATUS changeStatus( STATUS num );
public:
	bool isClear( )const;
	bool isOverlappedStatus( ) const;
	int getTarget( ) const;
	int getChangeCount( )const;
	STATUS getStatus( int num ) const;
	int getLost( );
private:
	MapPtr _map;
private:
	int _change_count;
	int _target_num;
	int _lost_count;
	static const int INSIDE_NUM = 10;
	std::array<STATUS, INSIDE_NUM> _status;
	int _max_inside;

};

