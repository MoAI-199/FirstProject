#include "Scene.h"

Scene::Scene( ) {
}


Scene::~Scene( ) {
}

void Scene::loadScene( const std::string& name ) {
	ApplicationPtr app = Application::getInstance( );
	app->setNextSceneName( name );
}
