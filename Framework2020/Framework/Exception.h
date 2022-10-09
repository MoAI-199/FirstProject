#pragma once

#include <string>

#define EXCEPTION( bool_fail, char_error ) {\
	if ( bool_fail ) {\
	_STL_ASSERT( false, "Exception Assert" );\
		throw Exception( char_error, __FILE__, __LINE__ );\
	}\
}

class Exception {
public:
	Exception( );
	Exception( std::string error, const char* file, int line );
	virtual ~Exception( );
public:
	const std::string getError( ) const;
	const std::string getFilename( ) const;
	const int getLine( ) const;
private:
	std::string _error;
	std::string _file;
	int _line;
};

