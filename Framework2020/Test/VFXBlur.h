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
	//	int SCREEN_W;// ��ʂ̉��𑜓x
//	int SCREEN_H;// ��ʂ̏c�𑜓x
	int ShaderScreen_W;// �V�F�[�_�[�p�̉�ʂ̉��𑜓x
	int ShaderScreen_H;// �V�F�[�_�[�p�̉�ʂ̏c�𑜓x
//
//	int Depth_NormalMeshVS;	// ���̃��b�V���p�̐[�x�l�`��p���_�V�F�[�_�[�n���h��
	VertexShaderPtr _depth_normal;
//	int Depth_SkinMeshVS;	// �X�L�j���O���b�V���p�̐[�x�l�`��p���_�V�F�[�_�[�n���h��
	VertexShaderPtr _depth_skin;
//	int DepthPS;			// �[�x�l�`��p�s�N�Z���V�F�[�_�[�n���h��
	PixelShaderPtr _depth_ps;
//	int DepthOfFieldPS;		// ��ʊE�[�x�����p�̃s�N�Z���V�F�[�_�[�n���h��
	PixelShaderPtr _depth_field_ps;
	GraphPtr _graph;
	Primitive2DShaderPtr _primitive_2d;
//
	float _depthOfField_InterpSize;	// �ł��ڂ����������Ƃ���Ƃڂ��������̂Ƃ���̕�ԋ���
	float _depthOfField_camera_target;		// �ڂ��������͈̔͂̒��S�ʒu
	float _depthOfField_camera_targetSize;	// �ڂ��������͈̔�
	float _depthOfField_TotalSize;	// ��Ԕ͈͂Ƃڂ��������͈̔͂̍��v����
//
//	// �[�x�l�`��p�e�N�X�`��
//	int DepthMapTexture;
	ImageTargetPtr _depth_map_texture;
//	// 0:�ʏ�`��p�e�N�X�`��
//	// 1:�ʏ�`��Ɏア�ڂ������������摜�ۑ��p�̃e�N�X�`��
//	// 2:�ʏ�`��ɋ����ڂ������������摜�ۑ��p�̃e�N�X�`��
	//int ColorMapTexture[ 3 ];
	std::array< ImageTargetPtr, 3 > _color_map_texture;
//
//	// ���H�����摜�ۑ��p�̃e�N�X�`��
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
//	int SCREEN_W;// ��ʂ̉��𑜓x
//	int SCREEN_H;// ��ʂ̏c�𑜓x
//	int ShaderScreen_W;// �V�F�[�_�[�p�̉�ʂ̉��𑜓x
//	int ShaderScreen_H;// �V�F�[�_�[�p�̉�ʂ̏c�𑜓x
//
//	int Depth_NormalMeshVS;	// ���̃��b�V���p�̐[�x�l�`��p���_�V�F�[�_�[�n���h��
//	int Depth_SkinMeshVS;	// �X�L�j���O���b�V���p�̐[�x�l�`��p���_�V�F�[�_�[�n���h��
//	int DepthPS;			// �[�x�l�`��p�s�N�Z���V�F�[�_�[�n���h��
//	int DepthOfFieldPS;		// ��ʊE�[�x�����p�̃s�N�Z���V�F�[�_�[�n���h��
//
//	float DepthOfField_InterpSize;	// �ł��ڂ����������Ƃ���Ƃڂ��������̂Ƃ���̕�ԋ���
//	float DepthOfField_camera_target;		// �ڂ��������͈̔͂̒��S�ʒu
//	float DepthOfField_camera_targetSize;	// �ڂ��������͈̔�
//	float DepthOfField_TotalSize;	// ��Ԕ͈͂Ƃڂ��������͈̔͂̍��v����
//
//	// �[�x�l�`��p�e�N�X�`��
//	int DepthMapTexture;
//	// 0:�ʏ�`��p�e�N�X�`��
//	// 1:�ʏ�`��Ɏア�ڂ������������摜�ۑ��p�̃e�N�X�`��
//	// 2:�ʏ�`��ɋ����ڂ������������摜�ۑ��p�̃e�N�X�`��
//	int ColorMapTexture[ 3 ];
//
//	// ���H�����摜�ۑ��p�̃e�N�X�`��
//	int OldMapTexture[ MAX_BLUR ];
//};
//
