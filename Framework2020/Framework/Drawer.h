#pragma once
#include "Task.h"
#include "mathematics.h"
#include "Color.h"
#include <string>
#include <array>

PTR( Drawer );
PTR( Image );
PTR( SoftImage );
PTR( Movie );
PTR( ImageTarget );
PTR( Billboard );
PTR( Sprites );
PTR( PixelShader );
PTR( VertexShader );
PTR( Primitive );
PTR( Primitive2DShader );
PTR( Primitive3DShader );
PTR( Graph );

class Drawer : public Task {
public:
	Drawer( );
	virtual ~Drawer( );
public:
	enum class BLEND {
		NONE,
		ALPHA,
		ADD,
		SUB,
		INV,
		MUL,
	};
	class Scope {
		friend class Drawer;
	private:
		Scope( );
		void onBlend( );
		void onBright( );
		void onTarget( );
		void onWriteZBuffer( );
		void onUseZBuffer( );
	public:
		~Scope( );
	private:
		bool _on_blend;
		bool _on_bright;
		bool _on_target;
		bool _on_write_z_buffer;
		bool _on_use_z_buffer;
	};
public: //make
	/// <summary>
	/// 画像を作成
	/// </summary>
	/// <param name="filename">ファイルの名前</param>
	/// <returns>true or false</returns>
	ImagePtr createImage( const char* filename ) const;
	/// <summary> ソフト画像データの保存クラス作成 </summary>
	/// <param name="filename"> ファイルアドレス</param>
	/// <returns> ソフト画像データ保存クラス </returns>
	SoftImagePtr createSoftImage( const char* filename ) const;
	ImagePtr createImageFromSoftImage( SoftImagePtr soft_image ) const;
	/// <summary>
	/// 画像の目標を作成
	/// </summary>
	/// <param name="width">横幅</param>
	/// <param name="height">立幅</param>
	/// <returns></returns>
	ImageTargetPtr createImageTarget( int width, int height ) const;
	/// <summary> 映像データの保存クラス作成 </summary>
	/// <param name="filename"> ファイルアドレス </param>
	/// <param name="alpha"> true or false </param>
	/// <returns> 映像データ保存クラス </returns>
	MoviePtr createMovie( const char* filename, bool alpha = false ) const;
	/// <summary> ビルボードデータの保存クラス作成 </summary>
	/// <param name="filename"> ファイルアドレス </param>
	/// <returns> ビルボードデータ保存クラス </returns>
	BillboardPtr createBillboard( const char* filename ) const;
	BillboardPtr createBillboard( ) const;
	/// <summary> スプライトデータの保存クラス作成 </summary>
	/// <param name="count"> カウント </param>
	/// <param name="filename"> ファイルアドレス </param>
	/// <param name="num"> NUM </param>
	/// <returns> スプライトデータ保存クラス </returns>
	SpritesPtr createSprites( int count, const char* filename, int num ) const;

	PixelShaderPtr createPixelShader( const char* filename ) const;
	VertexShaderPtr createVertexShader( const char* filename ) const;
	PrimitivePtr createPrimitive( bool buffer = false ) const;
	Primitive2DShaderPtr createPrimitive2DShader( );
	Primitive3DShaderPtr createPrimitive3DShader( );
	GraphPtr createGraph( );
public: //2D
	/// <summary>円の描画</summary>
	///<param name="pos">座標</param>
	///<param name="radius">半径</param>
	///<param name="color">色</param>
	///<param name="fill">塗り</param>
	void drawCircle( int x, int y, const double radius, const Color& color, bool fill ) const;
	void drawCircle( const Vector2& pos, const double radius, const Color& color, bool fill ) const;
	/// <summary> 描画する文字列のフォントのサイズをセットする </summary>
	/// <param name="size"> 描画するフォントのサイズ </param>
	void setStringSize( int size ) const;
    /// <summary> 文字の入力 </summary>
	/// <param name="x"> 文字のX座標 </param>
	/// <param name="y"> 文字のY座標 </param>
	/// <param name="string"> 入力したい文字 </param>
	/// <param name="..."> 可変長 </param>
	void drawString( int x, int y, const char* string, ... ) const;
	void drawString( const Vector2& pos, const char* string, ... ) const;
	/// <summary> 線の描画 </summary>
	///<param name="x1"> 線端のX座標 </param> 
	///<param name="y1"> 線端のY座標 </param>
	///<param name="x2"> 逆側の線端のX座標 </param>
	///<param name="y2"> 逆側の線端のY座標 </param>
	///<param name="color"> 色 </param>
	void drawLine( int x1, int y1, int x2, int y2, const Color& color ) const;
	void drawLine( const Vector2& pos1, const Vector2& pos2, const Color& color ) const;
	/// <summary> 四角の描画 </summary>
	///<param name="x1"> 四角の左上X座標 </param>
	///<param name="y1"> 四角の左上Y座標 </param>
	///<param name="x2"> 四角の右下X座標 </param>
	///<param name="y2"> 四角の右下y座標 </param>
	///<param name="color"> 四角の色 </param>
	///<param name="fill"> 四角を枠線だけで描画するかどうか </param>
	void drawBox( int x1, int y1, int x2, int y2, const Color& color, bool fill ) const;
	void drawBox( const Vector2& pos1, const Vector2& pos2, const Color& color, bool fill ) const;
	///<summary>ピクセルを描画する</summary>
	///<param name="x"> ピクセルのX座標 </param>
	///<param name="y"> ピクセルのY座標 </param>
	///<param name="color"> ピクセルの色 </param>
	void drawPixel( int x, int y, const Color& color ) const;
	void drawPixel( const Vector2& pos, const Color& color ) const;
	/// <summary>
	/// 扇形の描画
	/// </summary>
	/// <param name="x"> X座標 </param>
	/// <param name="y"> Y座標 </param>
	/// <param name="image"> 描画する円の画像 </param>
	/// <param name="ratio"> 比率 </param>
	void drawSector( int x, int y, ImageConstPtr image, double ratio ) const;
	/// <summary> ピクセルの色の取得 </summary>
	/// <param name="x"> 取得したいX座標 </param>
	/// <param name="y"> 取得したいY座標 </param>
	/// <returns> 取得した色 </returns>
	Color getPixelColor( int x, int y ) const;
public: //3D
	/// <summary> 球の描画 </summary>
	/// <param name="pos"> 球の中心座標 </param>
	/// <param name="radius"> 球の半径 </param>
	/// <param name="polygon_num"> 球を形成するためのポリゴンの細かさ </param>
	/// <param name="color"> 球の色 </param>
	/// <param name="fill"> 塗りつぶすor塗りつぶさない </param>
	void drawSphere( const Vector& pos, const double radius, int polygon_num, const Color& color, bool fill ) const;
	/// <summary>線の描画</summary>
	/// <param name="pos1">最初の点</param>
	/// <param name="pos2">最終の点</param>
	/// <param name="color">線の色</param>
	void drawLine( const Vector& pos1, const Vector& pos2, const Color& color ) const;
	/// <summary> カメラの座標を上げる </summary>
	/// <param name="up"> 上げる数値 </param>
	void setCameraUp( const Vector& up );
	/// <summary> カメラの位置、注視点の設定 </summary> 
	/// <param name="pos">　カメラの位置座標　</param>
	/// <param name="target"> カメラの注視するもの、部分 </param>
	void setCamera( const Vector& pos, const Vector& target );
	/// <summary> カメラの行列の設定 </summary>
	/// <param name="view"> ビュー行列 </param>
	/// <param name="proj"> 射影行列 </param>
	void setCamera( const Matrix& view, const Matrix& proj );
	/// <summary>
	/// カメラの中心を取得
	/// </summary>
	/// <param name="x">カメラのX座標を取得</param>
	/// <param name="y">カメラのY座標を取得</param>
	void setCameraCenter( double x, double y );
	/// <summary> カメラの視点をセットする </summary>
	/// <param name="fov">視野の広さ</param>
	void setCameraPerspective( double fov );
	/// <summary> カメラの呼び出し </summary>
	void initCameraSetting( ) const;
	/// <summary>
	/// 霧を取得
	/// </summary>
	/// <param name="start">霧の初めの座標</param>
	/// <param name="end">霧の終わりの座標</param>
	/// <param name="red">赤色</param>
	/// <param name="green">緑色</param>
	/// <param name="blue">青色</param>
	void setFog( double start, double end, int red, int green, int blue );
	/// <summary>
	///	ライトの位置と方向の設定
	/// </summary>
	/// <param name="pos">ライトの位置の値</param>
	/// <param name="dir">ライトの方向の値</param>
	void setLight( const Vector& pos, const Vector& dir );
	/// <summary>　使うライトの設定 </summary>
	/// <param name="on">ライトのON/OFF</param>
	void setUseLight( bool on );
	/// <summary>
	/// カメラの手前クリップ距離と奥クリップ距離の設定
	/// </summary>
	/// <param name="near">カメラの手前クリップ距離の値</param>
	/// <param name="far">カメラの奥クリップ距離の値</param> 
	void setNearFar( double near, double far );
	/// <summary>
	/// カメラが画面内にあるかどうかを判別する
	/// </summary>
	/// <param name="pos">カメラで捉えたいものの位置</param>
	/// <returns>true or false</returns>
	bool isInCamera( const Vector& pos ) const;
	/// <summary>
	/// ワールドの座標を取得する
	/// </summary>
	/// <param name="screen_pos"> スクリーン座標 </param>
	/// <returns>true or false</returns>
	Vector getWorldPos( const Vector& screen_pos ) const;
	/// <summary>
	/// スクリーンの座標を取得する
	/// </summary>
	Vector2 getScreenPos( const Vector& world_pos ) const;

	Quaternion getScreenPosPlusW( const Vector& world_pos ) const;
	/// <summary>
	/// カメラの座標を取得する
	/// </summary>
	Vector getCameraPos( ) const;
	/// <summary>
	/// カメラの注視点の取得
	/// </summary>
	/// <returns> カメラの注視点の値を返す </returns>
	Vector getCameraTarget( ) const;
	/// <summary>
	/// カメラのズームの値を返す
	/// </summary>
	/// <returns>ズームの値</returns>
	Vector getCameraUp( ) const;
	/// <summary> カメラの設定 </summary>
	/// <param name="camera_pos"> 座標 </param>
	/// <param name="camera_rot"> 角度（回転） </param>
	/// <returns> カメラ設定データの保存クラス </returns>
	Matrix createViewMatrix( const Vector& camera_pos, const Matrix& camera_rot ) const;
	/// <summary> 射影行列の作成 </summary>
	/// <returns> 行列 </returns>
	Matrix createProjectionMatrix( ) const;
public: // common
	/// <summary> 画面を初期化 </summary>
	void clearScreen( ) const;
public: // scope
	/// <summary>
	/// Scope作成
	/// </summary>
	/// <returns>生成されたScope</returns>
	Scope createScope( ) const;
	/// <summary>Ｚバッファを使用するかどうかを設定する </summary>
	/// <param name="write_z_buffer">zbuffer</param>
	void disableZBufferWriting( Scope& scope, bool write_z_buffer );
	void disableZBufferUsing( Scope& scope, bool write_z_buffer );
	/// <summary>
	///	描画先指定
	/// </summary>
	/// <param name="target">描画対象(無記入でBackScreen)</param>
	void setTarget( Scope& scope, ImageTargetPtr target = ImageTargetPtr( ) ) const;
	/// <summary>
	/// ブレンド設定
	/// </summary>
	/// <param name="scope">scope</param>
	/// <param name="blend">ブレンドタイプ</param>
	/// <param name="ratio">ブレンドレシオ</param>
	void setBlend( Scope& scope, BLEND blend, double ratio ) const;
	/// <summary>
	/// ブライト設定
	/// </summary>
	/// <param name="scope">scope</param>
	/// <param name="r">赤成分レシオ</param>
	/// <param name="g">緑成分レシオ</param>
	/// <param name="b">青成分レシオ</param>
	void setBright( Scope& scope, double r, double g, double b ) const;
	bool isOutterOfScreen( const Vector& pos );
	void polygon( Vector v1, Vector v2, Vector v3, int r, int g, int b );
private:
	void update( ) override;
private:
	Vector _light_pos;
	Vector _light_dir;
	bool _light;
	Vector _camera_pos;
	Vector _camera_target;
	Vector _camera_up;
	double _fov;
	double _near;
	double _far;
	int _screen_width;
	int _screen_height;
};

