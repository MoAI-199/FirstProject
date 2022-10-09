#pragma once
#include "Scene.h"

class Scene2D : public Scene {
public:
	Scene2D( );
	virtual ~Scene2D( );
	void initialize( ) override;
	void update( ) override;
private:
	Vector2 _dir;
};

