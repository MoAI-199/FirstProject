#pragma once

#include "smart_ptr.h"
#include "mathematics.h"
#include "Color.h"
#include <string>

PTR( Primitive );
PTR( PrimitiveImpl );
PTR( Image );

class Primitive {
	friend class Drawer;
public:
	/// <summary>
	/// class ModelImplで生成された変数の初期化処理
	/// </summary>
	/// <summary>
	/// _vertexが呼び出されていたらなかの配列を削除する
	/// </summary>
	virtual ~Primitive( );
public:
	struct VERTEX {
		Vector pos;
		double u;
		double v;
		Vector norm;
		Color color;
		VERTEX( ) { }
		VERTEX(
			Vector pos_,
			double u_,
			double v_,
			Vector norm_ = Vector( ),
			Color color_ = Color( ) ) {
				pos = pos_;
				u = u_;
				v = v_;
				color = color_;
				norm = norm_;
		}
	};
public:
	/// <summary>
	/// 3Dモデル描画
	/// </summary>
	void draw( );
	/// <summary>
	/// 3Dモデルの座標設定
	/// </summary>
	/// <param name="mat">二次元配列の要素</param>
	void draw( const Matrix& mat );
	/// <summary>
	/// 画像のハンドルを取得
	/// </summary>
	/// <param name="image">画像のハンドル</param>
	void setImage( ImageConstPtr image );
	/// <summary>
	/// 値を配分する
	/// </summary>
	/// <param name="polygon_num">変更されない値</param>
	void alloc( const int& polygon_num );
	/// <summary>
	/// 値を_polygon_numにセット
	/// </summary>
	/// <param name="num">変更されない値</param>
	void setNum( const int& num );
	/// <summary>
	/// _polygon_numに指定された値を返す
	/// </summary>
	/// <returns></returns>
	int  getNum( ) const;
	/// <summary>
	/// 3Dポリゴンに要素をセット
	/// </summary>
	/// <param name="n">配列の値</param>
	/// <param name="vertex">構造体の頂点配列</param>
	void set( const int& n, const VERTEX& vertex ); // n * 3
	/// <summary>
	/// ロード
	/// </summary>
	/// <param name="filename">読み込み先のファイル名</param>
	/// <returns></returns>
	bool load( const std::string& filename );
	/// <summary>
	/// セーブ
	/// </summary>
	/// <param name="filename">読み込み先のファイル名</param>
	void save( const std::string& filename );
	/// <summary>
	/// リセット
	/// </summary>
	void reset( );
	/// <summary>
	/// ポリゴン位置の×計算
	/// </summary>
	/// <param name="mat">ポリゴンの座標</param>
	void multiply( const Matrix& mat );
	/// <summary> 色の設定 </summary>
	/// <param name="color"> 色の値 </param>
	void setColor( const Color& color );
		/// <summary>
	/// 色の値を取得
	/// </summary>
	/// <param name="vertex_idx">配列の構造体</param>
	/// <param name="color">色の構造体</param>
	void setColor( const int& vertex_idx, const Color& color );
	/// <summary>
	/// モデルの頂点を取得する
	/// </summary>
	/// <param name="idx">モデルの頂点</param>
	/// <returns></returns>
	VERTEX getVertex( const int& idx ) const;
	/// <summary>
	/// ポイントの値取得
	/// </summary>
	/// <param name="idx">ポイントの座標値</param>
	/// <returns></returns>
	Vector getPoint( const int& idx ) const;
	/// <summary>
	/// モデルのポイントを設定する
	/// </summary>
	/// <param name="vertex_idx">ポイントの頂点</param>
	/// <param name="pos">ポイントの座標</param>
	/// <returns></returns>
	void setPoint( const int& vertex_idx, const Vector& pos );
	/// <summary>
	/// ポリゴンの法線を描画
	/// </summary>
	/// <param name="vertex_idx">ポリゴンの法線の値</param>
	/// <param name="norm">ポリゴンのベクトルの座標</param>
	void setNorm( const int& vertex_idx, const Vector& norm );

	void setTrans( bool trans );

	ImageConstPtr getImage( );
protected:
	Primitive( bool buffer );
private:
	/// <summary>
	/// モデルの値を実装
	/// </summary>
	/// <returns>_implの値</returns>
	PrimitiveImplConstPtr getModelImpl( ) const;
private:
	PrimitiveImplPtr _impl;
	ImageConstPtr _image;
	bool _buffer;
	bool _buffer_changed;
	bool _trans;
};

