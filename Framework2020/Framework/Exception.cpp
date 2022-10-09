#include "Exception.h"

Exception::Exception( ) :
_error( nullptr ),
_file( nullptr ),
_line( 0 ) {
}

Exception::Exception( std::string error, const char* file, int line ) :
_error( error ),
_file( file ),
_line( line ) {
}

Exception::~Exception( ) {
}

const std::string Exception::getError( ) const{
	return _error;
}

const std::string Exception::getFilename( ) const{
	return _file;
}

const int Exception::getLine( ) const {
	return _line;
}
