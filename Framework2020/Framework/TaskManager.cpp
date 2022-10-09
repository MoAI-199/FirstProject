#include "TaskManager.h"
#include "Task.h"
#include "Exception.h"

TaskManager::TaskManager( ) :
_status( STATUS::INVALID ) {
}


TaskManager::~TaskManager( ) {
}


void TaskManager::addTask( const std::string& tag, TaskPtr task ) {
	EXCEPTION( _status != STATUS::CREATED, "invalid created task" );
	if ( task ) {
		_task_list[ tag ] = task;
	}
}

TaskPtr TaskManager::getTask( const std::string& tag ) const {
	EXCEPTION( _status == STATUS::INVALID, "invalid used task" );
	if ( _task_list.count( tag ) == 0 ) {
		return TaskPtr( );
	}
	return _task_list.at( tag );
}

void TaskManager::update( ) {
	for ( std::pair< std::string, TaskPtr > task : _task_list ) {
		task.second->update( );
	}
}

void TaskManager::initialize( ) {
	for ( std::pair< std::string, TaskPtr > task : _task_list ) {
		task.second->initialize( );
	}
}

void TaskManager::finalize( ) {
	for ( std::pair< std::string, TaskPtr > task : _task_list ) {
		task.second->finalize( );
	}
}

void TaskManager::setStatus( STATUS status ) {
	_status = status;
}
