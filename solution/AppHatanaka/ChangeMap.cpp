#include "ChangeMap.h"

ChangeMap::ChangeMap( ) {
    _map_num = 0;
}

ChangeMap::~ChangeMap( ) {
}

void ChangeMap::setMapNum( int num ) {
    _map_num = num;
}

int ChangeMap::getMapNum( ) {
    return _map_num;
}
