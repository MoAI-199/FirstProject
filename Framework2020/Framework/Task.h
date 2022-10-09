#pragma once
#include "smart_ptr.h"
#include "Application.h"
#include <string>

PTR( Task );

class Task {
public:
	template< class T > static std::shared_ptr< T > get( ) {
		ApplicationPtr app = Application::getInstance( );
		std::string tag = typeid( T ).name( );
		TaskPtr task = app->getTask( tag );
		return std::dynamic_pointer_cast< T >( task );
	}
	template< class T > static std::shared_ptr< const T > getConst( ) {
		std::string tag = typeid( T ).name( );
		TaskConstPtr task = Application::getInstance( )->getTask( tag );
		return std::dynamic_pointer_cast< const T >( task );
	}
	template< class T > static void create( ) {
		std::string tag = typeid( T ).name( );
		Application::getInstance( )->addTask( tag, TaskPtr( new T ) );
	}
	template< class T > static void create( const std::string& str ) {
		std::string tag = typeid( T ).name( );
		Application::getInstance( )->addTask( tag, TaskPtr( new T( str ) ) );
	}
	template< class T > static void create( TaskPtr task ) {
		std::string tag = typeid( T ).name( );
		Application::getInstance( )->addTask( tag, task );
	}
public:
	Task( ) { }
	virtual ~Task( ) { }
public:
	virtual void update( ) { }
	virtual void initialize( ) { }
	virtual void finalize( ) { }
};

