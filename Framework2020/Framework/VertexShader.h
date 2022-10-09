#pragma once
#include "smart_ptr.h"

PTR( VertexShader );

class VertexShader {
	friend class Drawer;
public:
	virtual ~VertexShader( );
public:
	void useVertexShader( );
protected:
	VertexShader( const char* filename );
private:
	int _idx;
};

