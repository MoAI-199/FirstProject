#pragma once
#include "Scene.h"

PTR( Image );

class CompleteScene : public Scene {
public:
	CompleteScene( );
	virtual ~CompleteScene( );
private:
	void update( );
	void selectTitle( );
private:
	ImagePtr _image_rizarut;
};

