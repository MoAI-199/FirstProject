#pragma once
#include "SelectScene.h"
#include "Scene.h"
#include "Image.h"
#include "ChangeMap.h"

PTR( SelectScene );
PTR( ChangeMap );


class SelectScene :  public Scene {
public:
	SelectScene( );
	virtual ~SelectScene( );
public:
	void initialize( ) override;
	void update( ) override;
public:
	void selectStage( );
private:
	ImagePtr _image_select_stage;
};

