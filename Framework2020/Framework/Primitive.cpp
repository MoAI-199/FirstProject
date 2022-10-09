#include "Primitive.h"
#include "DxLib.h"
#include "Image.h"

class PrimitiveImpl {
public:
	VERTEX3D *_vertex;
	unsigned int _num;
	int _buffer_handle;
};

Primitive::Primitive( bool buffer ) {
	_impl = PrimitiveImplPtr( new PrimitiveImpl );
	_buffer = buffer;
	_impl->_vertex = NULL;
	_impl->_num = 0;
	_impl->_buffer_handle = -1;

	_trans = false;
}

Primitive::~Primitive( ) {
	reset( );
}

void Primitive::alloc( const int& Primitive_num ) {
	reset( );

	_impl->_num = Primitive_num;
	_impl->_vertex      = new VERTEX3D[ Primitive_num * 3 ];

	if ( _buffer ) {
		_impl->_buffer_handle = CreateVertexBuffer( Primitive_num * 3, DX_VERTEX_TYPE_NORMAL_3D );
	}
}

void Primitive::setNum( const int& num ) {
	_impl->_num = num;
}

void Primitive::draw( const Matrix& mat ) {
	MATRIX dx_mat;
	for ( int i = 0; i < 4; i++ ) {
		for ( int j = 0; j < 4; j++ ) {
			dx_mat.m[ i ][ j ] = ( float )mat.data[ i ][ j ];
		}
	}

	SetTransformToWorld( &dx_mat );
	draw( );

	MATRIX ident = MGetIdent( );
	SetTransformToWorld( &ident );
}

void Primitive::setTrans( bool trans ) {
	_trans = trans;
}

ImageConstPtr Primitive::getImage( ) {
	return _image;
}

void Primitive::draw( ) {
	if ( _image == ImageConstPtr( ) ) {
		return;
	}

	if ( _impl->_buffer_handle >= 0 && _buffer_changed ) {
		SetVertexBufferData( 0, _impl->_vertex, _impl->_num * 3, _impl->_buffer_handle );
	}
	_buffer_changed = false;

	if ( _impl->_buffer_handle < 0 ) {
		DrawPolygon3D( _impl->_vertex, _impl->_num, _image->getHandle( ), _trans );
	} else {
		DrawPolygon3D_UseVertexBuffer( _impl->_buffer_handle, _image->getHandle( ), _trans );
	}
}

void Primitive::setImage( ImageConstPtr image ) {
	_image = image;
}

void Primitive::set( const int& n, const VERTEX& vertex ) {
	VERTEX3D vtx;
	vtx.pos  = VGet( ( float )vertex.pos.x, ( float )vertex.pos.y, ( float )vertex.pos.z );
	vtx.norm = VGet( ( float )vertex.norm.x, ( float )vertex.norm.y, ( float )vertex.norm.z );
	vtx.dif  = GetColorU8(
		( int )( vertex.color.r * 255 ),
		( int )( vertex.color.g * 255 ),
		( int )( vertex.color.b * 255 ),
		( int )( vertex.color.a * 255 ));
	vtx.spc  = GetColorU8( 0, 0, 0, 0 );
	vtx.u    = ( float )vertex.u;
	vtx.v    = ( float )vertex.v;
	vtx.su   = 0.0f;
	vtx.sv   = 0.0f;

	_impl->_vertex[ n ] = vtx;

	_buffer_changed = true;
}

Primitive::VERTEX Primitive::getVertex( const int& idx ) const {
	VERTEX vertex;
	vertex.pos   = Vector( _impl->_vertex[ idx ].pos.x, _impl->_vertex[ idx ].pos.y, _impl->_vertex[ idx ].pos.z );
	vertex.norm  = Vector( _impl->_vertex[ idx ].norm.x, _impl->_vertex[ idx ].norm.y, _impl->_vertex[ idx ].norm.z );
	vertex.u     = _impl->_vertex[ idx ].u;
	vertex.v     = _impl->_vertex[ idx ].v;
	vertex.color = Color( _impl->_vertex[ idx ].dif.r, _impl->_vertex[ idx ].dif.g, _impl->_vertex[ idx ].dif.b, _impl->_vertex[ idx ].dif.a );
	return vertex;
}

bool Primitive::load( const std::string& filename ) {
	std::string str = filename;
	int fh = FileRead_open( str.c_str( ) );
	if ( fh <= 0 ) {
		str = "../" + str;
		fh = FileRead_open( str.c_str( ) );
		if (  fh <= 0 ) {
			return false;
		}
	}

	unsigned int Primitive_num = 0;
	FileRead_read( &Primitive_num, sizeof( unsigned int ), fh );

	alloc( Primitive_num );

	FileRead_read( _impl->_vertex, sizeof( DxLib::VERTEX3D ) * ( Primitive_num * 3 ), fh );

	FileRead_close( fh );

	_buffer_changed = true;
	return true;
}

void Primitive::save( const std::string& filename ) {
	FILE *fp;
	errno_t err = fopen_s( &fp, filename.c_str( ), "wb" );
	if ( err != 0 ) {
		return;
	}

	fwrite( &_impl->_num, sizeof( unsigned int ), 1, fp );
	fwrite( _impl->_vertex, sizeof( DxLib::VERTEX3D ) * ( _impl->_num * 3 ), 1, fp );

	fclose( fp );
}

PrimitiveImplConstPtr Primitive::getModelImpl( ) const {
	return _impl;
}

Vector Primitive::getPoint( const int& idx ) const {
	return Vector(
		_impl->_vertex[ idx ].pos.x,
		_impl->_vertex[ idx ].pos.y,
		_impl->_vertex[ idx ].pos.z );
}

void Primitive::setPoint( const int& vertex_idx, const Vector& pos ) {
	_impl->_vertex[ vertex_idx ].pos = VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z );

	_buffer_changed = true;
}

void Primitive::setNorm( const int& vertex_idx, const Vector& norm ) {
	_impl->_vertex[ vertex_idx ].norm = VGet( ( float )norm.x, ( float )norm.y, ( float )norm.z );

	_buffer_changed = true;
}

int Primitive::getNum( ) const {
	return _impl->_num;
}

void Primitive::reset( ) {
	if ( _impl->_vertex ) {
		delete[ ] _impl->_vertex;
	}
	if ( _impl->_buffer_handle >= 0 ) {
		DeleteVertexBuffer( _impl->_buffer_handle );
	}

	_impl->_vertex = NULL;
	_impl->_num = 0;
	_impl->_buffer_handle = -1;
}

void Primitive::setColor( const Color& color ) {
	int vertex_num = _impl->_num * 3;
	for ( int i = 0; i < vertex_num; i++ ) {
		VERTEX vertex = getVertex( i );
		vertex.color = color;
		set( i, vertex );
	}

	_buffer_changed = true;
}

void Primitive::setColor( const int& vertex_idx, const Color& color ) {
	_impl->_vertex[ vertex_idx ].dif.r = ( int )( color.r * 255 );
	_impl->_vertex[ vertex_idx ].dif.g = ( int )( color.g * 255 );
	_impl->_vertex[ vertex_idx ].dif.b = ( int )( color.b * 255 );
	_impl->_vertex[ vertex_idx ].dif.a = ( int )( color.a * 255 );

	_buffer_changed = true;
}

//モデル編集用
void Primitive::multiply( const Matrix& mat ) {
	int vertex_num = _impl->_num * 3;
	for ( int i = 0; i < vertex_num; i++ ) {
		Vector pos(
			_impl->_vertex[ i ].pos.x,
			_impl->_vertex[ i ].pos.y,
			_impl->_vertex[ i ].pos.z );

		pos = mat.multiply( pos );

		_impl->_vertex[ i ].pos.x = ( float )pos.x;
		_impl->_vertex[ i ].pos.y = ( float )pos.y;
		_impl->_vertex[ i ].pos.z = ( float )pos.z;
	}

	_buffer_changed = true;
}

/*
void Primitive::mergeModel( PrimitiveConstPtr model ) {
	if ( model == NULL ) {
		return;
	}

	PrimitiveImplConstPtr merge_impl = model->getModelImpl( );
	int Primitive_num = _impl->_num + merge_impl->_num;
	VERTEX3D* vertex = new VERTEX3D[ sizeof( VERTEX3D ) * Primitive_num * 3 ];

	// this のモデルをコピー
	int base_count = _impl->_num * 3;
	for ( int i = 0; i < base_count; i++ ) {
		vertex[ i ] = _impl->_vertex[ i ];
	}
	// modelのモデルをコピー
	int marge_count = merge_impl->_num * 3;
	for ( int i = 0; i < marge_count; i++ ) {
		int idx = _impl->_num * 3 + i;
		vertex[ idx ] = merge_impl->_vertex[ i ];
	}

	if ( _impl->_vertex ) {
		delete [] _impl->_vertex;
	}
	_impl->_vertex = vertex;
	_impl->_num = Primitive_num;
}
*/
