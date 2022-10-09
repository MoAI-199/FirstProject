#include "Graph.h"
#include "DxLib.h"
#include "ImageTarget.h"

Graph::Graph( ) {

}

Graph::~Graph( ) {

}

void Graph::setGraphChannelNum( int num ) {
	SetCreateDrawValidGraphChannelNum( num );
}

void Graph::setGraphFilterTectBlt( ImageTargetPtr image_target, ImageTargetPtr image_target2, int src_x1, int src_y1, int src_x2, int src_y2, int dest_x, int dest_y, GRAPH graph ) {
	switch ( graph ) {
	case GRAPH::MONO:
		GraphFilterRectBlt( image_target->getHandle( ), image_target2->getHandle( ), src_x1, src_y1, src_x2, src_y2, dest_x, dest_y, DX_GRAPH_FILTER_MONO, 8, 1000 );
		break;
	case GRAPH::GAUSS:
		GraphFilterRectBlt( image_target->getHandle( ), image_target2->getHandle( ), src_x1, src_y1, src_x2, src_y2, dest_x, dest_y, DX_GRAPH_FILTER_GAUSS, 8, 1000 );
		break;
	case GRAPH::DOWN_SCALE:
		GraphFilterRectBlt( image_target->getHandle( ), image_target2->getHandle( ), src_x1, src_y1, src_x2, src_y2, dest_x, dest_y, DX_GRAPH_FILTER_DOWN_SCALE, 8, 1000 );
		break;
	case GRAPH::BRIGHT_CLIP:
		GraphFilterRectBlt( image_target->getHandle( ), image_target2->getHandle( ), src_x1, src_y1, src_x2, src_y2, dest_x, dest_y, DX_GRAPH_FILTER_BRIGHT_CLIP, 8, 1000 );
		break;
	case GRAPH::HSB:
		GraphFilterRectBlt( image_target->getHandle( ), image_target2->getHandle( ), src_x1, src_y1, src_x2, src_y2, dest_x, dest_y, DX_GRAPH_FILTER_HSB, 8, 1000 );
		break;
	case GRAPH::INVERT:
		GraphFilterRectBlt( image_target->getHandle( ), image_target2->getHandle( ), src_x1, src_y1, src_x2, src_y2, dest_x, dest_y, DX_GRAPH_FILTER_INVERT, 8, 1000 );
		break;
	case GRAPH::LEVEL:
		GraphFilterRectBlt( image_target->getHandle( ), image_target2->getHandle( ), src_x1, src_y1, src_x2, src_y2, dest_x, dest_y, DX_GRAPH_FILTER_LEVEL, 8, 1000 );
		break;
	case GRAPH::TWO_COLOR:
		GraphFilterRectBlt( image_target->getHandle( ), image_target2->getHandle( ), src_x1, src_y1, src_x2, src_y2, dest_x, dest_y, DX_GRAPH_FILTER_TWO_COLOR, 8, 1000 );
		break;
	case GRAPH::GRADIENT_MAP:
		GraphFilterRectBlt( image_target->getHandle( ), image_target2->getHandle( ), src_x1, src_y1, src_x2, src_y2, dest_x, dest_y, DX_GRAPH_FILTER_GRADIENT_MAP, 8, 1000 );
		break;
	}
}

void Graph::getDrawScreenGraph( int x1, int y1, int x2, int y2, ImageTargetPtr image_target ) {
	GetDrawScreenGraph( x1, y1, x2, y2, image_target->getHandle( ) );
}
