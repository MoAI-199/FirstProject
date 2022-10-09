#pragma once
#include "smart_ptr.h"
#include <list>
#include <map>

PTR( Billboard );
PTR( Viewer3DEffectPrimitive );
PTR( Viewer3DEffectAnimation );

class Viewer3DEffects {
public:
	enum class Effect {
		fire,
	};
public:
	Viewer3DEffects( );
	virtual ~Viewer3DEffects( );
public:
	void draw( );
private:
	void addEffects( );
	void updateEffects( );
	void createEffects( );
	bool drawEffects( int count );
private:
	std::list< Viewer3DEffectPrimitivePtr > _primitive;
	std::map< Effect, Viewer3DEffectAnimationPtr > _animation;
};

