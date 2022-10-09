#pragma once
#include <string>
#include <map>
#include "Application.h"

PTR( Scene );

class Introduction {
public:
	Introduction( );
	virtual ~Introduction( );
public:
	void procedure( );
	ScenePtr generateScene( const std::string& name );
private:
	void start( const std::string& scene );
private:
	std::map< std::string, ScenePtr ( * )( ) > _generate_scene_func;
};

#define REGIST( NAME ) \
extern ScenePtr generate##NAME( ); \
_generate_scene_func[ #NAME ] = generate##NAME;
