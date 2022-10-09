#pragma once
#include <string>
#include <map>
#include "smart_ptr.h"

PTR( Task );

class TaskManager {
public:
	TaskManager( );
	virtual ~TaskManager( );
public:
	enum class STATUS {
		INVALID,
		USED,
		CREATED,
	};
public:
	void addTask( const std::string& tag, TaskPtr task );
	TaskPtr getTask( const std::string& tag ) const;
	void initialize( );
	void finalize( );
	void update( );
	void setStatus( STATUS status );
private:
	std::map< std::string, TaskPtr > _task_list;
	STATUS _status;
};

