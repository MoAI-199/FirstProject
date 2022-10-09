#pragma once
#include "smart_ptr.h"
#include "TaskManager.h"
#include "Application.h"
#include <string>
#include <array>

PTR( Scene );
PTR( Task );
PTR( TaskManager );

class Scene {
public:
	Scene( );
	virtual ~Scene( );
public:
	virtual void initialize( ) { }
	virtual void finalize( ) { }
	virtual void update( ) = 0;
protected:
	void loadScene( const std::string& name );
};


#define SCENE( NAME ) \
ScenePtr generate##NAME( ) { \
	return ScenePtr( new NAME ); \
}