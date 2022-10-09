#pragma once
#include "smart_ptr.h"
#include "mathematics.h"
#include "Color.h"
#include <string>

PTR( Model );
PTR( Shader );

class Model {
public:
	Model( );
	virtual ~Model( );
public:
	/// <summary>
	/// 3Dƒ‚ƒfƒ‹‚Ì•`‰æ
	/// </summary>
	void draw( ) const;
	//void draw( ShaderPtr shader ) const;
	void load( const char* filename, double ratio = 1.0 );
	void setTexture( const char* filename, bool semi_trans = false );
	void transferTransform( const Matrix& trans_mat );
	void rotateTransform( const Matrix& rot_mat );
	void scaleTransform( const Matrix& scale_mat );
	void setTrans( const Matrix& matrix );
	void setRot( const Matrix& matrix );
	void setScale( const Matrix& matrix );
	void setAdjust( const bool adjust );
	void reset( );
	double getEndAnimTime( ) const;
	Vector getHitPos( ) const;
	Vector getHitNorm( ) const;
	bool isHitLine( const Vector& begin, const Vector& end );
	bool isHitSphere( const Vector& pos, const float radius );
	void setAnimTime( double time );
	void setAnim( int anim );
	void setColor( const Color& color );
	void setUseZBuffer( bool use );
	void setWriteZBuffer( bool writting );
private:
	int _mesh;
	int _anim;
	bool _adjust_origin;
	double _time;
	Vector _max;
	Vector _min;
	Vector _hit_pos;
	Vector _hit_norm;
	Matrix _trans_mat;
	Matrix _rot_mat;
	Matrix _scale_mat;
};