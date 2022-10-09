#include "Model.h"
#include "Application.h"
#include "DxLib.h"
#include "PixelShader.h"
#include <string>
#include <assert.h>

const int FRAME_INDEX = -1;

Model::Model( ) :
	_adjust_origin( false ),
	_mesh( -1 ),
	_anim( -1 ),
	_time( 0 ) {
	_rot_mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), 0 );
	_trans_mat = Matrix::makeTransformTranslation( Vector( ) );
	_scale_mat = Matrix::makeTransformScaling( Vector( 1, 1, 0 ) );
}


Model::~Model( ) {
	if ( _mesh >= 0 ) {
		MV1DeleteModel( _mesh );
		MV1TerminateCollInfo( _mesh );
	}
}

void Model::load( const char* filename, double ratio ) {
	_mesh = MV1LoadModel( filename );
	if ( _mesh <= 0 ) {
		std::string path = ( std::string )"../" + filename;
		_mesh = MV1LoadModel( path.c_str( ) );
	}

	assert( _mesh > 0 );

	float r = ( float )ratio;
	int num = MV1GetMaterialNum( _mesh );
	for ( int i = 0; i < num; i++ ) {
		MV1SetMaterialDifColor( _mesh, i, GetColorF( 1.0f, 1.0f, 1.0f, r ) );
		MV1SetMaterialAmbColor( _mesh, i, GetColorF( 1.0f, 1.0f, 1.0f, 1.0f ) );
		MV1SetMaterialSpcColor( _mesh, i, GetColorF( 0.0f, 0.0f, 0.0f, 1.0f ) );
		MV1SetMaterialEmiColor( _mesh, i, GetColorF( 0.5f, 0.5f, 0.5f, 1.0f ) );
	}
	MV1_REF_POLYGONLIST ref = MV1GetReferenceMesh( _mesh, FRAME_INDEX, TRUE );
	_max = Vector( ref.MaxPosition.x, ref.MaxPosition.y, ref.MaxPosition.z );
	_min = Vector( ref.MinPosition.x, ref.MinPosition.y, ref.MinPosition.z );

	MV1SetupCollInfo( _mesh, FRAME_INDEX );
}

void Model::setColor( const Color& color ) {
	int num = MV1GetMaterialNum( _mesh );
	for ( int i = 0; i < num; i++ ) {
		MV1SetMaterialDifColor( _mesh, i, GetColorF( ( float )color.r, ( float )color.g, ( float )color.b, ( float )color.a ) );
	}
}

void Model::setTexture( const char* filename, bool semi_trans ) {
	int graph = MV1LoadTexture( filename );
	if ( semi_trans ) {
		MV1SetTextureGraphHandle( _mesh, 0, graph, TRUE );
	} else {
		MV1SetTextureGraphHandle( _mesh, 0, graph, FALSE );
	}
}


void Model::draw( ) const {
	Matrix mat;
	if ( _adjust_origin ) {
		Vector check = ( _max + _min ) * -0.5;
		Matrix adjust = Matrix::makeTransformTranslation( check );
		mat = adjust.multiply( _rot_mat ).multiply( _scale_mat ).multiply( _trans_mat );
	} else {
		mat = _rot_mat.multiply( _scale_mat ).multiply( _trans_mat );
	}
	MATRIX matrix = MGetIdent( );
	for ( int i = 0; i < 4; i++ ) {
		for ( int j = 0; j < 4; j++ ) {
			matrix.m[ i ][ j ] = ( float )mat.data[ i ][ j ];
		}
	}
	MV1SetMatrix( _mesh, matrix );
	//アニメーション設定
	int idx = MV1AttachAnim( _mesh, 0, _anim, TRUE );
	MV1SetAttachAnimTime( _mesh, idx, ( float )_time );
	// ３Ｄモデルの描画
	MV1DrawModel( _mesh );
	MV1DetachAnim( _mesh, idx );
}

//void Model::draw( ShaderPtr shader ) const {
//	// シェーダーを有効にする
//	MV1SetUseOrigShader( TRUE );
//	SetUsePixelShader( shader->_idx );
//
//	draw( );
//
//	// シェーダーのリセット
//	MV1SetUseOrigShader( FALSE );
//	SetUseVertexShader( -1 );
//	SetUsePixelShader( -1 );
//}

void Model::setTrans( const Matrix& matrix ) {
	_trans_mat = matrix;
}

void Model::setRot( const Matrix& matrix ) {
	_rot_mat = matrix;
}

void Model::setScale( const Matrix& matrix ) {
	_scale_mat = matrix;
}

void Model::setAdjust( const bool adjust ) {
	_adjust_origin = adjust;
}

void Model::transferTransform( const Matrix& trans_mat ) {
	_trans_mat = _trans_mat.multiply( trans_mat );
}

void Model::rotateTransform( const Matrix& rot_mat ) {
	_rot_mat = _rot_mat.multiply( rot_mat );
}

void Model::scaleTransform( const Matrix& scale_mat ) {
	_scale_mat = _scale_mat.multiply( scale_mat );
}

double Model::getEndAnimTime( ) const {
	return MV1GetAnimTotalTime( _mesh, 0 );
}

void Model::reset( ) {
	MV1DeleteModel( _mesh );
	_mesh = -1;
	_anim = -1;
	_time = 0;
}

Vector Model::getHitPos( ) const {
	return _hit_pos;
}

Vector Model::getHitNorm( ) const {
	return _hit_norm;
}

bool Model::isHitLine( const Vector& begin, const Vector& end ) {
	//当たり判定にモデルの姿勢を反映させるためにMatrixを入れる必要がある。
	Matrix mat = _rot_mat.multiply( _scale_mat ).multiply( _trans_mat );
	MATRIX matrix = MGetIdent( );
	for ( int j = 0; j < 3 * 3; j++ ) {
		int n = j % 3;
		int m = j / 3;
		matrix.m[ m ][ n ] = ( float )mat.data[ n ][ m ];
	}
	for ( int j = 0; j < 4; j++ ) {
		matrix.m[ j ][ 3 ] = ( float )mat.data[ j ][ 3 ];
		matrix.m[ 3 ][ j ] = ( float )mat.data[ 3 ][ j ];
	}
	MV1SetMatrix( _mesh, matrix );
	//いったん規定値にあわせる
	MV1RefreshCollInfo( _mesh, FRAME_INDEX );
	VECTOR start_pos = VGet( (float)begin.x, (float)begin.y,(float)begin.z );
	VECTOR end_pos = VGet( (float)end.x, (float)end.y, (float)end.z );
	MV1_COLL_RESULT_POLY hit_poly = MV1CollCheck_Line( _mesh, FRAME_INDEX, start_pos, end_pos );
	_hit_pos = Vector( );
	_hit_norm = Vector( );
	if ( hit_poly.HitFlag == 1 ) {
		_hit_pos = Vector( ( double )hit_poly.HitPosition.x, ( double )hit_poly.HitPosition.y, ( double )hit_poly.HitPosition.z );
		_hit_norm = Vector( ( double )hit_poly.Normal.x, ( double )hit_poly.Normal.y, ( double )hit_poly.Normal.z );
	}
	return hit_poly.HitFlag == 1;
}

bool Model::isHitSphere( const Vector& pos, const float radius ) {
	//いったん規定値にあわせる
	MV1RefreshCollInfo( _mesh, FRAME_INDEX );
	VECTOR center_pos = VGet( (float)pos.x, (float)pos.y,(float)pos.z );
	MV1_COLL_RESULT_POLY_DIM hit_poly = MV1CollCheck_Sphere( _mesh, FRAME_INDEX, center_pos, radius );
	_hit_pos = Vector( );
	_hit_norm = Vector( );
	if ( hit_poly.HitNum > 0 ) {
		VECTOR ave = VGet( 0, 0, 0 );
		for ( int i = 0; i < hit_poly.HitNum; i++ ) {
			ave = VAdd( ave, hit_poly.Dim[ i ].HitPosition );
		}
		_hit_pos.x = ave.x / hit_poly.HitNum;
		_hit_pos.y = ave.y / hit_poly.HitNum;
		_hit_pos.z = ave.z / hit_poly.HitNum;
		_hit_norm.x = hit_poly.Dim[ 0 ].Normal.x;
		_hit_norm.y = hit_poly.Dim[ 0 ].Normal.y;
		_hit_norm.z = hit_poly.Dim[ 0 ].Normal.z;
	}
	MV1CollResultPolyDimTerminate( hit_poly );//当たり判定の配列メモリ領域削除
	return hit_poly.HitNum > 0;
}

void Model::setAnimTime( double time ) {
	_time = time;
}

void Model::setAnim( int anim ) {
	_anim = anim;
}

void Model::setUseZBuffer( bool use ) {
	MV1SetUseZBuffer( _mesh, use ? TRUE : FALSE );
}

void Model::setWriteZBuffer( bool writting ) {
	MV1SetWriteZBuffer( _mesh, writting ? TRUE: FALSE );
}
