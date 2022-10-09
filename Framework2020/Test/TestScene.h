#pragma once
#include "Scene.h"
#include "Model.h"
#include <vector>
#include <string>

PTR( VFXEdge );
PTR( VFXWater );
PTR( VFXMirror );
PTR( VFXFirePillar );
PTR( Fire );

class TestScene : public Scene {
public:
	TestScene( );
	virtual ~TestScene( );

private:
	void initialize( ) override;
	void update( ) override;
private:
	std::vector< std::string > _files;
	ModelPtr _model;
	Vector _pos;
	VFXEdgePtr _vfx_edge;
	VFXWaterPtr _vfx_water;
	VFXMirrorPtr _vfx_mirror;
	VFXFirePillarPtr _vfx_fire_pillar;
	FirePtr _fire;
	Vector _fire_pos;
};
