#include "Option.h"
#include "Application.h"
#include "Exception.h"
#include <fstream>

Option::Option( std::string filepath ) {
	load( filepath );
}


Option::~Option( ) {
}

void Option::update( ) {
}

const int Option::getData( const std::string& tag ) const {
	std::string msg = "not found tag:" + tag;
	char * err = new char[ msg.length( ) + 1 ] ( );
	strcpy_s( err, msg.length( ) + 1, msg.c_str( ) );
	EXCEPTION( _data.count( tag ) == 0, err );
	
	return _data.at( tag );
}

void Option::load( const std::string& filepath ) {
	std::string str = filepath;
	std::ifstream ifs = std::ifstream( str.c_str( ) );

	if ( ifs.fail( ) ) {
		str = "../" + str;
		ifs = std::ifstream( str.c_str( ) );
		if ( ifs.fail( ) ) {
			return;
		}
	}
	
	std::string line;
	while ( std::getline( ifs, line ) ) {
		size_t size = line.find( ',' );
		if ( size == std::string::npos ) {
			continue;
		}
		std::string tag = line.substr( 0, size );
		int value = std::atoi( line.substr( size + 1, line.length( ) ).c_str( ) );
		_data[ tag ] = value;
	}
	ifs.close( );
}
