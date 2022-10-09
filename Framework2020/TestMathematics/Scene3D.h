#pragma once
#include "Scene.h"

class Scene3D : public Scene {
public:
	Scene3D( );
	virtual ~Scene3D( );
	void initialize( ) override;
	void update( ) override;
private:
	Vector _p;
	Vector _dir;
};

