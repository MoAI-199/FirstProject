#pragma once
#include "smart_ptr.h"
#include<array>

const int MAX_BLUR = 5;

PTR( ImageTarget );
PTR( VertexShader );
PTR( PixelShader );
PTR( Graph );
PTR( Primitive2DShader );

class VFXBlur {
public:
	VFXBlur( );
	virtual ~VFXBlur( );
public:
	void drawMotionBlurBegin( );
	void drawMotionBlurEnd( );
	void resetMoitonBlur( );
private:
	//	void draw( );
	//	void motionBlur( );
	//	void DrawScreen( int ScreenGraphHandle, int UseDepthShader );
	void draw( );
	void motionBlur( );
	void drawScreen( int ScreenGraphHandle, int UseDepthShader );
private:
	//	int SCREEN_W;// 画面の横解像度
//	int SCREEN_H;// 画面の縦解像度
	int ShaderScreen_W;// シェーダー用の画面の横解像度
	int ShaderScreen_H;// シェーダー用の画面の縦解像度
//
//	int Depth_NormalMeshVS;	// 剛体メッシュ用の深度値描画用頂点シェーダーハンドル
	VertexShaderPtr _depth_normal;
//	int Depth_SkinMeshVS;	// スキニングメッシュ用の深度値描画用頂点シェーダーハンドル
	VertexShaderPtr _depth_skin;
//	int DepthPS;			// 深度値描画用ピクセルシェーダーハンドル
	PixelShaderPtr _depth_ps;
//	int DepthOfFieldPS;		// 被写界深度処理用のピクセルシェーダーハンドル
	PixelShaderPtr _depth_field_ps;
	GraphPtr _graph;
	Primitive2DShaderPtr _primitive_2d;
//
	float _depthOfField_InterpSize;	// 最もぼかしが強いところとぼかし無しのところの補間距離
	float _depthOfField_camera_target;		// ぼかし無しの範囲の中心位置
	float _depthOfField_camera_targetSize;	// ぼかし無しの範囲
	float _depthOfField_TotalSize;	// 補間範囲とぼかし無しの範囲の合計距離
//
//	// 深度値描画用テクスチャ
//	int DepthMapTexture;
	ImageTargetPtr _depth_map_texture;
//	// 0:通常描画用テクスチャ
//	// 1:通常描画に弱いぼかしを加えた画像保存用のテクスチャ
//	// 2:通常描画に強いぼかしを加えた画像保存用のテクスチャ
	//int ColorMapTexture[ 3 ];
	std::array< ImageTargetPtr, 3 > _color_map_texture;
//
//	// 加工した画像保存用のテクスチャ
//	int OldMapTexture[ MAX_BLUR ];
	std::array< ImageTargetPtr, MAX_BLUR > _old_map_texture;
};

//#pragma once
//#include "smart_ptr.h"
////#include "DxLib.h"
//
//#include <vector>
//
//PTR( ViewerBlur );
//
//#define MAX_BLUR 5
//
//class ViewerBlur {
//public:
//	ViewerBlur( );
//	virtual ~ViewerBlur( );
//private:
//	void draw( );
//	void motionBlur( );
//	void DrawScreen( int ScreenGraphHandle, int UseDepthShader );
//public:
//	void drawBegin( );
//	void drawEnd( );
//	void drawMotionBlurBegin( );
//	void drawMotionBlurEnd( );
//	void resetMoitonBlur( );
//private:
//	int SCREEN_W;// 画面の横解像度
//	int SCREEN_H;// 画面の縦解像度
//	int ShaderScreen_W;// シェーダー用の画面の横解像度
//	int ShaderScreen_H;// シェーダー用の画面の縦解像度
//
//	int Depth_NormalMeshVS;	// 剛体メッシュ用の深度値描画用頂点シェーダーハンドル
//	int Depth_SkinMeshVS;	// スキニングメッシュ用の深度値描画用頂点シェーダーハンドル
//	int DepthPS;			// 深度値描画用ピクセルシェーダーハンドル
//	int DepthOfFieldPS;		// 被写界深度処理用のピクセルシェーダーハンドル
//
//	float DepthOfField_InterpSize;	// 最もぼかしが強いところとぼかし無しのところの補間距離
//	float DepthOfField_camera_target;		// ぼかし無しの範囲の中心位置
//	float DepthOfField_camera_targetSize;	// ぼかし無しの範囲
//	float DepthOfField_TotalSize;	// 補間範囲とぼかし無しの範囲の合計距離
//
//	// 深度値描画用テクスチャ
//	int DepthMapTexture;
//	// 0:通常描画用テクスチャ
//	// 1:通常描画に弱いぼかしを加えた画像保存用のテクスチャ
//	// 2:通常描画に強いぼかしを加えた画像保存用のテクスチャ
//	int ColorMapTexture[ 3 ];
//
//	// 加工した画像保存用のテクスチャ
//	int OldMapTexture[ MAX_BLUR ];
//};
//
