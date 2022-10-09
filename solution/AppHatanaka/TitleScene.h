#pragma once
#include "Scene.h"
#include "Image.h"


class TitleScene : public Scene{
public:
	TitleScene( );
	virtual ~TitleScene( );
public:
	void update( );
	void start( );
	void draw( );
private:
	ImagePtr _image_title;
};

