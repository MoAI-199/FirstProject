#pragma once
#include "smart_ptr.h"

PTR( ImageTarget );
PTR( Graph );

class Graph {
public:
	Graph( );
	virtual ~Graph( );
public:
	enum class GRAPH {
		MONO,
		GAUSS,
		DOWN_SCALE,
		BRIGHT_CLIP,
		HSB,
		INVERT,
		LEVEL,
		TWO_COLOR,
		GRADIENT_MAP,
	};
public:
	void setGraphChannelNum( int num );
	void setGraphFilterTectBlt( ImageTargetPtr image_target, ImageTargetPtr image_target2, int src_x1, int src_y1, int src_x2, int src_y2, int dest_x, int dest_y, GRAPH graph );
	void getDrawScreenGraph( int x1, int y1, int x2, int y2, ImageTargetPtr image_target );
};
