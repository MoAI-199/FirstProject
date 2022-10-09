#pragma once

#include <math.h>

/// <summary>
/// 円周率
/// </summary>
const double PI  = 3.1415926535897932384626433832795;
/// <summary>
/// 円周
/// </summary>
const double PI2 = PI * 2;

/// <summary>
/// 三角比
/// </summary>
const unsigned int VECTOR_RATIO = 1 << 16;
/// <summary>
/// VECTOR_MARGINのビットの値を設定
/// </summary>
const unsigned int VECTOR_MARGIN = 1 << 15;

/// <summary>
/// 円形
/// </summary>
/// <param name="in_value">値</param>
/// <returns></returns>
inline double round(const double in_value) {
	int in = (int)(in_value * VECTOR_RATIO);
	return (double)in / VECTOR_RATIO;
}

struct Vector {
	double x;
	double y;
	double z;

	/// <summary>
	/// 頂点
	/// </summary>
	Vector()
		: x(0)
		, y(0)
		, z(0) {
	}

	/// <summary> 2Dの円の頂点 </summary>
	/// <param name="inX"> X座標 </param>
	/// <param name="inY"> Y座標 </param>
	/// <param name="inZ"> Z座標 </param>
	Vector(const double inX, const double inY, const double inZ = 0)
		: x(round(inX))
		, y(round(inY))
		, z(round(inZ)) {
	}

	/// <summary>
	/// 全ての頂点データを破棄
	/// </summary>
	virtual ~Vector() {

	}

	// 距離の２乗（計算スピード向上のため）
	/// <summary>
	/// 斜辺の長さ
	/// </summary>
	/// <returns></returns>
	inline double getLength2() const {
		return x * x + y * y + z * z;
	}

	// 距離
	/// <summary>
	/// 距離を取得する
	/// </summary>
	/// <returns></returns>
	inline double getLength() const {
		double s[3] = { fabs(x), fabs(y), fabs(z)};
		double n = s[0];
		for (int i = 1; i < 3; i++) {
			if (n < s[i]) n = s[i];
		}
		if (n == 0.0) {
			return 0;
		}
		double x = this->x / n;
		double y = this->y / n;
		double z = this->z / n;
		return sqrt(x * x + y * y + z * z) * n;
	}

	// 足し算
	inline Vector add(const Vector& inVector) const {
		return Vector(
			x + inVector.x,	
			y + inVector.y,
			z + inVector.z);
	}
	inline Vector operator+(const Vector& inVector) const {
		return add(inVector);
	}
	inline Vector operator+=(const Vector& inVector) {
		*this = *this + inVector;
		return *this;
	}

	// 引き算
	inline Vector sub(const Vector& inVector) const {
		return Vector(
			x - inVector.x,	
			y - inVector.y,
			z - inVector.z);
	}
	inline Vector operator-(const Vector& inVector) const {
		return sub(inVector);
	}
	inline Vector operator-=(const Vector& inVector) {
		*this = *this - inVector;
		return *this;
	}

	// 掛け算
	/// <summary>
	/// 頂点の掛け算
	/// </summary>
	/// <param name="inValue"> 掛ける値 </param>
	inline Vector multiply(const double inValue) const {
		return Vector(
			x * inValue,	
			y * inValue,
			z * inValue);
	}
	inline Vector operator*(const double inValue) const {
		return multiply(inValue);
	}
	inline Vector operator*=(const double inValue) {
		*this = *this * inValue;
		return *this;
	}

	// ベクトル同士の積
	inline Vector product(const Vector& inVector) const {
			double x1 = x * inVector.x;
			double x2 = x * inVector.y; 
			double y1 = y * inVector.x;
			double y2 = y * inVector.y; 

			return Vector( x1 - y2, x2 + y1, 0 );
	}
	// 外積
	inline Vector cross(const Vector& inVector) const {
		Vector v1 = (*this).normalize();
		Vector v2 = inVector.normalize();
		Vector vc(
			v1.y * v2.z - v2.y * v1.z,
			v1.z * v2.x - v2.z * v1.x,
			v1.x * v2.y - v2.x * v1.y);
		return vc.normalize();
	}

	// 内積
	inline double dot(const Vector& inVec) const {
		double x = this->x * inVec.x;
		double y = this->y * inVec.y;
		double z = this->z * inVec.z;
		return x + y + z;
	}

	// 正規化	
	inline Vector normalize() const {
		double norm = getLength();
		if (norm == 0) {
			norm = 1; // 大きさがないので、１で対応
		}
		return Vector(
			x / norm,
			y / norm,
			z / norm);
	}

	// ２ベクトル間の角度（ラジアン）を求める
	// エラーは-1
	inline double angle(const Vector& in_vec) const {
		Vector v1 = normalize();
		Vector v2 = in_vec.normalize();
		double d = v1.dot(v2);

		if (d >= 1) {
			return 0;
		}
		
		if (d <= -1) {
			return PI;
		}

		return acos(d);
	}

	/// <summary>
	/// 原点確認
	/// </summary>
	inline bool isOrigin() const {
		return
			x == 0 &&
			y == 0 &&
			z == 0;
	}

	// イコール
	inline bool operator==(const Vector& inVec) const {
		int sx = (int)(this->x * VECTOR_MARGIN);
		int sy = (int)(this->y * VECTOR_MARGIN);
		int sz = (int)(this->z * VECTOR_MARGIN);
		int dx = (int)(inVec.x * VECTOR_MARGIN);
		int dy = (int)(inVec.y * VECTOR_MARGIN);
		int fz = (int)(inVec.z * VECTOR_MARGIN);
		return
			sx == dx &&
			sy == dy &&
			sz == fz;
	}
	inline bool operator!=(const Vector& inVec) const {
		return !(*this == inVec);
	}
};


struct Vector2 {
	double x;
	double y;

	/// <summary>
	/// 頂点
	/// </summary>
	Vector2( )
		: x( 0 )
		, y( 0 ) {
	}

	/// <summary> 2Dの円の頂点 </summary>
	/// <param name="inX"> X座標 </param>
	/// <param name="inY"> Y座標 </param>
	/// <param name="inZ"> Z座標 </param>
	Vector2( const double inX, const double inY )
		: x( round( inX ) )
		, y( round( inY ) ) {
	}

	/// <summary>
	/// 全ての頂点データを破棄
	/// </summary>
	virtual ~Vector2( ) {

	}

	// 距離の２乗（計算スピード向上のため）
	/// <summary>
	/// 斜辺の長さ
	/// </summary>
	/// <returns></returns>
	inline double getLength2( ) const {
		return x * x + y * y;
	}

	// 距離
	/// <summary>
	/// 距離を取得する
	/// </summary>
	/// <returns></returns>
	inline double getLength( ) const {
		double s[ 2 ] = { fabs( x ), fabs( y ) };
		double n = s[ 0 ];
		for ( int i = 1; i < 2; i++ ) {
			if ( n < s[ i ] ) n = s[ i ];
		}
		if ( n == 0.0 ) {
			return 0;
		}
		double x = this->x / n;
		double y = this->y / n;
		return sqrt( x * x + y * y ) * n;
	}

	// 足し算
	inline Vector2 add( const Vector2& inVector2 ) const {
		return Vector2(
			x + inVector2.x,
			y + inVector2.y );
	}
	inline Vector2 operator+( const Vector2& inVector2 ) const {
		return add( inVector2 );
	}
	inline Vector2 operator+=( const Vector2& inVector2 ) {
		*this = *this + inVector2;
		return *this;
	}

	// 引き算
	inline Vector2 sub( const Vector2& inVector2 ) const {
		return Vector2(
			x - inVector2.x,
			y - inVector2.y );
	}
	inline Vector2 operator-( const Vector2& inVector2 ) const {
		return sub( inVector2 );
	}
	inline Vector2 operator-=( const Vector2& inVector2 ) {
		*this = *this - inVector2;
		return *this;
	}

	// 掛け算
	/// <summary>
	/// 頂点の掛け算
	/// </summary>
	/// <param name="inValue"> 掛ける値 </param>
	inline Vector2 multiply( const double inValue ) const {
		return Vector2(
			x * inValue,
			y * inValue );
	}
	inline Vector2 operator*( const double inValue ) const {
		return multiply( inValue );
	}
	inline Vector2 operator*=( const double inValue ) {
		*this = *this * inValue;
		return *this;
	}

	// 回転
	inline Vector2 rotate( double angle ) const {
		return Vector2(
			x * cos( angle ) - y * sin( angle ),
			x * sin( angle ) + y * cos( angle ) );
	}

	// 外積
	inline double cross( const Vector2& inVec2 ) const {
		return x * inVec2.y - y * inVec2.x;
	}

	// 内積
	inline double dot( const Vector2& inVec2 ) const {
		double x = this->x * inVec2.x;
		double y = this->y * inVec2.y;
		return x + y;
	}

	// 正規化	
	inline Vector2 normalize( ) const {
		double norm = getLength( );
		if ( norm == 0 ) {
			norm = 1; // 大きさがないので、１で対応
		}
		return Vector2(
			x / norm,
			y / norm );
	}

	// ２ベクトル間の角度（ラジアン）を求める
	// エラーは-1
	inline double angle( const Vector2& in_vec2 ) const {
		Vector2 v1( x, y );
		Vector2 v2 = in_vec2;

		double d = v1.dot( v2 );
		double n = v1.getLength( ) * v2.getLength( );
		double cos_theta = d / n;

		return acos( cos_theta );
	}

	/// <summary>
	/// 原点確認
	/// </summary>
	inline bool isOrigin( ) const {
		return
			x == 0 &&
			y == 0;
	}

	// イコール
	inline bool operator==( const Vector2 inVec2 ) const {
		int sx = ( int )( this->x * VECTOR_MARGIN );
		int sy = ( int )( this->y * VECTOR_MARGIN );
		int dx = ( int )( inVec2.x * VECTOR_MARGIN );
		int dy = ( int )( inVec2.y * VECTOR_MARGIN );
		return
			sx == dx &&
			sy == dy;
	}
	inline bool operator!=( const Vector2& inVec2 ) const {
		return !( *this == inVec2 );
	}

};

struct Matrix {
	double data[ 4 ][ 4 ];

	/// <summary>
	/// 行列のデータを作成
	/// </summary>
	Matrix( ) {
		// Matrixが作られたら、単位行列を作成する
		for ( int i = 0; i < 4; i++ )
			for ( int j = 0; j < 4; j++ ) {
				data[ i ][ j ] = 0;
			}
		for ( int i = 0; i < 4; i++ ) {
			data[ i ][ i ] = 1;
		}
	}

	/// <summary>
	/// 行列のデータを破棄
	/// </summary>
	virtual ~Matrix( ) {
	}

	inline static Matrix makeTransformTranslation( const Vector& inTrans ) {
		// 平行移動の変換行列を作る
		Matrix mat;

		mat.data[ 3 ][ 0 ] = inTrans.x;
		mat.data[ 3 ][ 1 ] = inTrans.y;
		mat.data[ 3 ][ 2 ] = inTrans.z;

		return mat;
	}

	inline static Matrix makeTransformTranslation( const Vector2& inTrans ) {
		// 平行移動の変換行列を作る
		Matrix mat;

		mat.data[ 3 ][ 0 ] = inTrans.x;
		mat.data[ 3 ][ 1 ] = inTrans.y;
		mat.data[ 3 ][ 2 ] = 0;

		return mat;
	}

	inline static Matrix makeTransformScaling( const Vector& inScale ) {
		// 拡大縮小の変換行列を作る
		Matrix mat;

		mat.data[ 0 ][ 0 ] = inScale.x;
		mat.data[ 1 ][ 1 ] = inScale.y;
		mat.data[ 2 ][ 2 ] = inScale.z;

		return mat;
	}

	inline static Matrix makeTransformScaling( const Vector2& inScale ) {
		// 拡大縮小の変換行列を作る
		Matrix mat;

		mat.data[ 0 ][ 0 ] = inScale.x;
		mat.data[ 1 ][ 1 ] = inScale.y;
		mat.data[ 2 ][ 2 ] = 1;

		return mat;
	}

	inline static Matrix makeTransformRotation( const double inRadian ) {
		return makeTransformRotation( Vector( 0, 0, 1 ), inRadian );
	}

	inline static Matrix makeTransformRotation( const Vector& inAxis, const double inRadian ) {
		// 任意軸回転の変換行列を作る
		Matrix mat;
		Vector v = inAxis.normalize( );
		double c = ( 1 - cos( inRadian ) );

		mat.data[ 0 ][ 0 ] = v.x * v.x * c + cos( inRadian );
		mat.data[ 0 ][ 1 ] = v.x * v.y * c - v.z * sin( inRadian );
		mat.data[ 0 ][ 2 ] = v.z * v.x * c + v.y * sin( inRadian );

		mat.data[ 1 ][ 0 ] = v.x * v.y * c + v.z * sin( inRadian );
		mat.data[ 1 ][ 1 ] = v.y * v.y * c + cos( inRadian );
		mat.data[ 1 ][ 2 ] = v.y * v.z * c - v.x * sin( inRadian );

		mat.data[ 2 ][ 0 ] = v.z * v.x * c - v.y * sin( inRadian );
		mat.data[ 2 ][ 1 ] = v.y * v.z * c + v.x * sin( inRadian );
		mat.data[ 2 ][ 2 ] = v.z * v.z * c + cos( inRadian );

		return mat;
	}

	inline Matrix multiply( const Matrix& inMat ) const {
		Matrix mat;
		for ( int n = 0; n < 4; n++ ) {
			for ( int m = 0; m < 4; m++ ) {
				mat.data[ n ][ m ] = 0;
				for ( int i = 0; i < 4; i++ ) {
					mat.data[ n ][ m ] += data[ n ][ i ] * inMat.data[ i ][ m ];
				}
			}
		}
		return mat;
	}

	inline Matrix operator*( const Matrix& inMat ) const {
		return this->multiply( inMat );
	}

	inline Vector multiply( const Vector& inVector ) const {
		return Vector(
			data[ 0 ][ 0 ] * inVector.x + data[ 1 ][ 0 ] * inVector.y + data[ 2 ][ 0 ] * inVector.z + data[ 3 ][ 0 ] * 1,
			data[ 0 ][ 1 ] * inVector.x + data[ 1 ][ 1 ] * inVector.y + data[ 2 ][ 1 ] * inVector.z + data[ 3 ][ 1 ] * 1,
			data[ 0 ][ 2 ] * inVector.x + data[ 1 ][ 2 ] * inVector.y + data[ 2 ][ 2 ] * inVector.z + data[ 3 ][ 2 ] * 1 );
	}

	inline Vector operator*( const Vector& inVector ) const {
		return this->multiply( inVector );
	}

	inline Vector2 multiply( const Vector2& inVector ) const {
		return Vector2(
			data[ 0 ][ 0 ] * inVector.x + data[ 1 ][ 0 ] * inVector.y + data[ 2 ][ 0 ] * 1 + data[ 3 ][ 0 ] * 1,
			data[ 0 ][ 1 ] * inVector.x + data[ 1 ][ 1 ] * inVector.y + data[ 2 ][ 1 ] * 1 + data[ 3 ][ 1 ] * 1 );
	}

	inline Vector2 operator*( const Vector2& inVector ) const {
		return this->multiply( inVector );
	}
};

struct Quaternion {
	double x;
	double y;
	double z;
	double w;

	inline Quaternion makeTransformRotation( const Vector& dir, double angle ) {
		Vector v = dir.normalize( );
		x = v.x * sin( angle / 2 );
		y = v.y * sin( angle / 2 );
		z = v.z * sin( angle / 2 );
		w =       cos( angle / 2 );
	}
};
