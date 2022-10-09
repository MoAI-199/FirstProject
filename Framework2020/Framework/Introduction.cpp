#include "Introduction.h"
#include "Exception.h"


Introduction::Introduction( ) {
}


Introduction::~Introduction( ) {
}

void Introduction::start( const std::string& scene ) {
	Application::getInstance( )->setNextSceneName( scene );
}

ScenePtr Introduction::generateScene( const std::string& name ) {
	EXCEPTION( _generate_scene_func.count( name ) == 0, name + " not registered"  );
	return ( *_generate_scene_func[ name ] )( );
}