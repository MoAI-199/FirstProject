#include "VFXBlur.h"
#include "ImageTarget.h"
#include "Drawer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Graph.h"
#include "mathematics.h"
#include "Primitive2DShader.h"


VFXBlur::VFXBlur( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	//	GetWindowSize( &SCREEN_W, &SCREEN_H );
//
//	ShaderScreen_W = 1024 * 2;
	ShaderScreen_W = 1024 * 2;
	//	ShaderScreen_H = 1024 * 2;
	ShaderScreen_H = 1024 * 2;
	//
	//	// �ʏ�`��p�̃e�N�X�`�����쐬
	//	ColorMapTexture[ 0 ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
	//
	//	// �ʏ�`�悵���f���Ɏア�ڂ������������摜��ۑ����邽�߂̃e�N�X�`�����쐬
	//	ColorMapTexture[ 1 ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
	//
	//	// �ʏ�`�悵���f���ɋ����ڂ������������摜��ۑ����邽�߂̃e�N�X�`�����쐬
	//	ColorMapTexture[ 2 ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
	//
	for ( int i = 0; i < 3; i++ ) {
		_color_map_texture[ i ] = drawer->createImageTarget( ShaderScreen_W, ShaderScreen_H );
	}
	//	// �[�x�l��`�悷��P�`�����l���摜���쐬
	//	SetCreateDrawValidGraphChannelNum( 1 );
	_graph = drawer->createGraph( );
	_graph->setGraphChannelNum( 1 );
	//	DepthMapTexture = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
	_depth_map_texture = drawer->createImageTarget( ShaderScreen_W, ShaderScreen_H );
	//	SetCreateDrawValidGraphChannelNum( 0 );
	_graph->setGraphChannelNum( 0 );
	//
	//	
	//	// ���H�����摜��ۑ����邽�߂̃e�N�X�`�����쐬
	//	for ( int i = 0; i < MAX_BLUR; i++ ) {
	//		OldMapTexture[ i ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
	//	}
	for ( int i = 0; i < MAX_BLUR; i++ ) {
		_old_map_texture[ i ] = drawer->createImageTarget( ShaderScreen_W, ShaderScreen_H );
	}
	//
	//	// �[�x�l�`��p�̒��_�V�F�[�_�[�ǂݍ���
	//	Depth_NormalMeshVS = LoadVertexShader( "resource/shader/Depth_NormalMeshVS.vso" );
	_depth_normal = drawer->createVertexShader( "resource/shader/Depth_NormalMeshVS.vso" );
	//	Depth_SkinMeshVS   = LoadVertexShader( "resource/shader/Depth_SkinMeshVS.vso" );
	_depth_skin = drawer->createVertexShader( "resource/shader/Depth_SkinMeshVS.vso" );
	//
	//	// �[�x�l�`��p�̃s�N�Z���V�F�[�_�[�ǂݍ���
	//	DepthPS            = LoadPixelShader( "resource/shader/DepthPS.pso" );
	_depth_ps = drawer->createPixelShader( "resource/shader/DepthPS.pso" );
	//
	//	// �[�x�l���g�p���Ĕ�ʊE�[�x�������s�����߂̃s�N�Z���V�F�[�_�[��ǂݍ���
	//	DepthOfFieldPS     = LoadPixelShader( "resource/shader/DepthOfFieldPS.pso" );
	_depth_field_ps = drawer->createPixelShader( "resource/shader/DepthOfFieldPS.pso" );

	_primitive_2d = drawer->createPrimitive2DShader( );
	//
	//	// ��ʊE�[�x�����p�̃p�����[�^��������
	_depthOfField_InterpSize = 3000.0f;
	_depthOfField_camera_target = 3200.0f;
	_depthOfField_camera_targetSize = 500.0f;
	//
}

VFXBlur::~VFXBlur( ) {

}

void VFXBlur::drawMotionBlurBegin( ) {
	//
//	// �`��u�����h���[�h���Z�b�g
//	SetDrawBlendMode( DX_BLENDMODE_PMA_ALPHA, 255 );
//	
//	// ��Ԕ͈͂ƃt�H�[�J�X�������Ă���͈͂��܂߂����������Z�o
//	DepthOfField_TotalSize = DepthOfField_InterpSize * 2.0f + DepthOfField_camera_targetSize;
//
//	// �ʏ�`��p�e�N�X�`���Ƀ��f����ʏ�`��
//	DrawScreen( ColorMapTexture[ 0 ], FALSE );
}

void VFXBlur::drawMotionBlurEnd( ) {
	motionBlur( );
}

void VFXBlur::resetMoitonBlur( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	for ( int i = 0; i < MAX_BLUR; i++ ) {
		//SetDrawScreen( OldMapTexture[ i ] );
		drawer->setTarget( scope, _old_map_texture[ i ] );
		//ClearDrawScreen( );
		drawer->clearScreen( );
	}
	//SetDrawScreen( DX_SCREEN_BACK );
	drawer->setTarget( scope );
}

void VFXBlur::draw( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	//	FLOAT4 ParamF;
	Quaternion paramf;
	//	float RightU, BottomV;
	float right_u;
	float bottom_v;
	//	VERTEX2DSHADER Vertex[ 6 ];


			// ��ʊE�[�x�`��̏���
	{
		// �ʏ�`��̌��ʂɂڂ������������摜��ۑ�
		_graph->setGraphFilterTectBlt( _color_map_texture[ 0 ], _color_map_texture[ 1 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, Graph::GRAPH::GAUSS );

		// �ڂ������������摜�ɍX�ɂڂ����������ĕۑ�
		_graph->setGraphFilterTectBlt( _color_map_texture[ 1 ], _color_map_texture[ 2 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, Graph::GRAPH::GAUSS );

		// �[�x�l�`��p�e�N�X�`���ɐ[�x�l��`��
		drawer->setTarget( scope, _depth_map_texture );
	}

	// �`���𗠉�ʂɕύX
//		SetDrawScreen( DX_SCREEN_BACK );
	drawer->setTarget( scope );


	// ��ʊE�[�x���{�����摜�𗠉�ʂɕ`��
	{
		// �g�p����e�N�X�`�����Z�b�g
		//SetUseTextureToShader( 0, ColorMapTexture[ 0 ] );
		_depth_field_ps->setUseTexture( 0, _color_map_texture[ 0 ] );
		//SetUseTextureToShader( 1, ColorMapTexture[ 1 ] );
		_depth_field_ps->setUseTexture( 1, _color_map_texture[ 1 ] );
		//SetUseTextureToShader( 2, ColorMapTexture[ 2 ] );
		_depth_field_ps->setUseTexture( 2, _color_map_texture[ 2 ] );
		//SetUseTextureToShader( 3, DepthMapTexture );
		_depth_field_ps->setUseTexture( 3, _color_map_texture[ 3 ] );

		// ��ʊE�[�x�����p�̃s�N�Z���V�F�[�_�[���Z�b�g
		//SetUsePixelShader( DepthOfFieldPS );
		_depth_field_ps->usePixelShader( );

		// ��ʊE�[�x�̂ڂ��������̊J�n�ʒu���v�Z
		//ParamF.x = 100;//DepthOfField_InterpSize / DepthOfField_TotalSize;
		paramf.x = 100;

		// ��ʊE�[�x�̂ڂ��������̏I���ʒu���v�Z
		//ParamF.y = ParamF.x;//( DepthOfField_InterpSize + DepthOfField_camera_targetSize ) / DepthOfField_TotalSize;
		paramf.y = paramf.x;

		// �s�N�Z���V�F�[�_�[�̒萔�ɃZ�b�g
		//SetPSConstF( 0, ParamF );
		_depth_field_ps->setPSConstF( 0, paramf );

		Primitive::VERTEX vtx[ 6 ];

		_primitive_2d->alloc( 2 );

		// ��ʑS�̂ɕ`�悷�钸�_�̏���
		//Vertex[ 0 ].pos = VGet(					   0.0f,					0.0f, 0.0f );
		vtx[ 0 ].pos = Vector( 0.0, 0.0, 0.0 );
		//Vertex[ 1 ].pos = VGet( ( float )ShaderScreen_W,					0.0f, 0.0f );
		vtx[ 1 ].pos = Vector( ShaderScreen_W, 0.0, 0.0 );
		//Vertex[ 2 ].pos = VGet(					   0.0f, ( float )ShaderScreen_H, 0.0f );
		vtx[ 2 ].pos = Vector( 0.0, ShaderScreen_H, 0.0 );
		//Vertex[ 3 ].pos = VGet( ( float )ShaderScreen_W, ( float )ShaderScreen_H, 0.0f );
		vtx[ 3 ].pos = Vector( ShaderScreen_W, ShaderScreen_H, 0.0 );

		//Vertex[ 0 ].rhw = 1.0f;
		//Vertex[ 1 ].rhw = 1.0f;
		//Vertex[ 2 ].rhw = 1.0f;
		//Vertex[ 3 ].rhw = 1.0f;

		right_u = ( float )ShaderScreen_W / ShaderScreen_W;
		bottom_v = ( float )ShaderScreen_H / ShaderScreen_H;
		vtx[ 0 ].u = 0.0f;
		vtx[ 1 ].u = right_u;
		vtx[ 2 ].u = 0.0f;
		vtx[ 3 ].u = right_u;

		vtx[ 0 ].v = 0.0f;
		vtx[ 1 ].v = 0.0f;
		vtx[ 2 ].v = bottom_v;
		vtx[ 3 ].v = bottom_v;

		vtx[ 4 ] = vtx[ 2 ];
		vtx[ 5 ] = vtx[ 1 ];

		for ( int i = 0; i < 6; i++ ) {
			_primitive_2d->set( i, vtx[ i ] );
		}
		// �|���S���Q�����g�p���ĉ�ʑS�̂ɔ�ʊE�[�x�������{�����R�c�V�[���̕`��
		//DrawPolygon2DToShader( Vertex, 2 );
		_primitive_2d->drawUsPolygon2DToShader( 2 );
	}


	//	// �ǂݍ��񂾃V�F�[�_�[�̉���
	//	SetUsePixelShader( -1 );
	//	SetUseVertexShader( -1 );
	//	MV1SetUseOrigShader( FALSE );
	//
	_depth_field_ps->usePixelShaderEnd( );
	//	// �`��u�����h���[�h�����ɖ߂�
	//	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
	drawer->setBlend( scope, Drawer::BLEND::NONE, 1.0 );
	//		
}

void VFXBlur::motionBlur( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	//	FLOAT4 ParamF;
	Quaternion paramf;
	//	float RightU, BottomV;
	float right_u;
	float bottom_v;
	//	VERTEX2DSHADER Vertex[ 6 ];

			// ��ʊE�[�x�`��̏���
	{
		// �ʏ�`��̌��ʂɂڂ������������摜��ۑ�
		//GraphFilterRectBlt( ColorMapTexture[ 0 ], ColorMapTexture[ 1 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, DX_GRAPH_FILTER_GAUSS, 8, 2000 );
		_graph->setGraphFilterTectBlt( _color_map_texture[ 0 ], _color_map_texture[ 1 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, Graph::GRAPH::GAUSS );

		// �ڂ������������摜�ɍX�ɂڂ����������ĕۑ�
		//GraphFilterRectBlt( ColorMapTexture[ 1 ], ColorMapTexture[ 2 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, DX_GRAPH_FILTER_GAUSS, 8, 2000 );
		_graph->setGraphFilterTectBlt( _color_map_texture[ 1 ], _color_map_texture[ 2 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, Graph::GRAPH::GAUSS );

		// �[�x�l�`��p�e�N�X�`���ɐ[�x�l��`��
		//DrawScreen( DepthMapTexture, TRUE );
		drawer->setTarget( scope, _depth_map_texture );
	}

	// �`���𗠉�ʂɕύX
	//SetDrawScreen( OldMapTexture[ 0 ] );
	drawer->setTarget( scope );

	//ClearDrawScreen( );
	drawer->clearScreen( );

	// ��ʊE�[�x���{�����摜�𗠉�ʂɕ`��
	{
		// �g�p����e�N�X�`�����Z�b�g
		//SetUseTextureToShader( 0, ColorMapTexture[ 0 ] );
		_depth_field_ps->setUseTexture( 0, _color_map_texture[ 0 ] );
		//SetUseTextureToShader( 1, ColorMapTexture[ 1 ] );
		_depth_field_ps->setUseTexture( 1, _color_map_texture[ 1 ] );
		//SetUseTextureToShader( 2, ColorMapTexture[ 2 ] );
		_depth_field_ps->setUseTexture( 2, _color_map_texture[ 2 ] );
		//SetUseTextureToShader( 3, DepthMapTexture );
		_depth_field_ps->setUseTexture( 3, _depth_map_texture );

		// ��ʊE�[�x�����p�̃s�N�Z���V�F�[�_�[���Z�b�g
		//SetUsePixelShader( DepthOfFieldPS );
		_depth_field_ps->usePixelShader( );

		// ��ʊE�[�x�̂ڂ��������̊J�n�ʒu���v�Z
		paramf.x = 100;//DepthOfField_InterpSize / DepthOfField_TotalSize;

		// ��ʊE�[�x�̂ڂ��������̏I���ʒu���v�Z
		paramf.y = paramf.x;//( DepthOfField_InterpSize + DepthOfField_camera_targetSize ) / DepthOfField_TotalSize;

		// �s�N�Z���V�F�[�_�[�̒萔�ɃZ�b�g
		//SetPSConstF( 0, ParamF );
		_depth_field_ps->setPSConstF( 0, paramf );

		Primitive::VERTEX vtx[ 6 ];

		// ��ʑS�̂ɕ`�悷�钸�_�̏���
		vtx[ 0 ].pos = Vector( 0.0f, 0.0f, 0.0f );
		vtx[ 1 ].pos = Vector( ShaderScreen_W, 0.0f, 0.0f );
		vtx[ 2 ].pos = Vector( 0.0f, ShaderScreen_H, 0.0f );
		vtx[ 3 ].pos = Vector( ShaderScreen_W, ShaderScreen_H, 0.0f );

		//Vertex[ 0 ].rhw = 1.0f;
		//Vertex[ 1 ].rhw = 1.0f;
		//Vertex[ 2 ].rhw = 1.0f;
		//Vertex[ 3 ].rhw = 1.0f;

		right_u = ( float )ShaderScreen_W / ShaderScreen_W;
		bottom_v = ( float )ShaderScreen_H / ShaderScreen_H;
		vtx[ 0 ].u = 0.0f;
		vtx[ 1 ].u = right_u;
		vtx[ 2 ].u = 0.0f;
		vtx[ 3 ].u = right_u;

		vtx[ 0 ].v = 0.0f;
		vtx[ 1 ].v = 0.0f;
		vtx[ 2 ].v = bottom_v;
		vtx[ 3 ].v = bottom_v;

		vtx[ 4 ] = vtx[ 2 ];
		vtx[ 5 ] = vtx[ 1 ];

		for ( int i = 0; i < 6; i++ ) {
			_primitive_2d->set( i, vtx[ i ] );
		}
		// �|���S���Q�����g�p���ĉ�ʑS�̂ɔ�ʊE�[�x�������{�����R�c�V�[���̕`��
		//DrawPolygon2DToShader( Vertex, 2 );
		_primitive_2d->drawUsPolygon2DToShader( 2 );
	}


	//GetDrawScreenGraph( 0, 0, ShaderScreen_W, ShaderScreen_H, OldMapTexture[ 1 ] );
	_graph->getDrawScreenGraph( 0, 0, ShaderScreen_W, ShaderScreen_H, _old_map_texture[ 1 ] );

	for ( int i = 1; i < MAX_BLUR - 1; i++ ) {
		//SetDrawScreen( OldMapTexture[ MAX_BLUR - ( i + 1 ) ] );
		drawer->setTarget( scope, _old_map_texture[ MAX_BLUR - ( i + 1 ) ] );
		//GetDrawScreenGraph( 0, 0, ShaderScreen_W, ShaderScreen_H, OldMapTexture[ MAX_BLUR - i ] );
		_graph->getDrawScreenGraph( 0, 0, ShaderScreen_W, ShaderScreen_H, _old_map_texture[ MAX_BLUR - i ] );
	}

	// �`���𗠉�ʂɕύX
	//SetDrawScreen( DX_SCREEN_BACK );
	drawer->setTarget( scope );

	for ( int i = 1; i < MAX_BLUR; i++ ) {
		//DrawGraph( 0, 0, OldMapTexture[ MAX_BLUR - i ], TRUE );
		_old_map_texture[ MAX_BLUR - i ]->draw( 0, 0 );

	}

	// �ǂݍ��񂾃V�F�[�_�[�̉���
	//SetUsePixelShader( -1 );
	//SetUseVertexShader( -1 );
	//MV1SetUseOrigShader( FALSE );
	_depth_field_ps->usePixelShaderEnd( );

	// �`��u�����h���[�h�����ɖ߂�
	//SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
	drawer->setBlend( scope, Drawer::BLEND::NONE, 1.0 );
}

//// �w��̉�ʂɂR�c���f����`��
void VFXBlur::drawScreen( int ScreenGraphHandle, int UseDepthShader ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	//	FLOAT4 ParamF;
	Quaternion paramf;
	//
	//	// �`��Ώۂ�ύX
	//	SetDrawScreen( ScreenGraphHandle );
	//
	//	// ��ʂ�������
	//	ClearDrawScreen( );
	drawer->clearScreen( );
	//
	//	// �[�x�l��`�悷�邩�ǂ����ŏ����𕪊�
	//	if( UseDepthShader != 0 ) {
	//		// �[�x�l��`�悷��ꍇ�̓I���W�i���̃V�F�[�_�[���g�p���ă��f����`�悷��ݒ�ɂ���
	//		MV1SetUseOrigShader( TRUE );
	//
	//		// �[�x�l��`�悷�邽�߂̃s�N�Z���V�F�[�_�[���Z�b�g
	//		SetUsePixelShader( DepthPS );
	//
	//		// ��ʊE�[�x�̊J�n�ʒu���v�Z
	//		ParamF.x = DepthOfField_camera_target - DepthOfField_camera_targetSize / 2.0f - DepthOfField_InterpSize;
	//
	//		// ��ʊE�[�x�̏I���ʒu���v�Z
	//		ParamF.y = DepthOfField_camera_target + DepthOfField_camera_targetSize / 2.0f + DepthOfField_InterpSize;
	//
	//		// ��ʊE�[�x�͈̔͂̋t�����v�Z
	//		ParamF.z = 1.0f / ( ParamF.y - ParamF.x );
	//
	//		// �s�N�Z���V�F�[�_�[�̒萔�ɃZ�b�g
	//		SetPSConstF( 0, ParamF );
	//	} else {
	//		// �[�x�l��`�悷��킯�ł͂Ȃ��ꍇ�̓I���W�i���̃V�F�[�_�[�͎g�p���Ȃ�
	//		MV1SetUseOrigShader( FALSE );
	//	}
	//
	if ( UseDepthShader != 0 ) {
		_depth_ps->usePixelShaderBegin( );
		// ��ʊE�[�x�̊J�n�ʒu���v�Z
		paramf.x = _depthOfField_camera_target - _depthOfField_camera_targetSize / 2.0 - _depthOfField_InterpSize;

		// ��ʊE�[�x�̏I���ʒu���v�Z
		paramf.y = _depthOfField_camera_target + _depthOfField_camera_targetSize / 2.0 + _depthOfField_InterpSize;

		// ��ʊE�[�x�͈̔͂̋t�����v�Z
		paramf.z = 1.0f / ( paramf.y - paramf.x );

		// �s�N�Z���V�F�[�_�[�̒萔�ɃZ�b�g
		_depth_ps->setPSConstF( 0, paramf );
	} else {
		_depth_ps->noUseOrigShader( );
	}
	//	// �[�x�l��`�悷��ꍇ�͒��_�V�F�[�_�[�����̃��b�V���p�̐[�x�l�`��p���_�V�F�[�_�[�ɂ���
	//	if( UseDepthShader != 0 ) {
	//		SetUseVertexShader( Depth_NormalMeshVS );
	//	}
	if ( UseDepthShader != 0 ) {
		_depth_normal->useVertexShader( );
	}
	//
	//	// �[�x�l��`�悷��ꍇ�͒��_�V�F�[�_�[���X�L�j���O���b�V���p�̐[�x�l�`��p���_�V�F�[�_�[�ɂ���
	//	if( UseDepthShader != 0 ) {
	//		SetUseVertexShader( Depth_SkinMeshVS );
	//	}
	if ( UseDepthShader != 0 ) {
		_depth_skin->useVertexShader( );
	}
}

//#include "ViewerBlur.h"
////#include "Camera.h"
//#include <math.h>
//
//ViewerBlur::ViewerBlur( ) {
//	
//	GetWindowSize( &SCREEN_W, &SCREEN_H );
//
//	ShaderScreen_W = 1024 * 2;
//	ShaderScreen_H = 1024 * 2;
//
//	// �ʏ�`��p�̃e�N�X�`�����쐬
//	ColorMapTexture[ 0 ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
//
//	// �ʏ�`�悵���f���Ɏア�ڂ������������摜��ۑ����邽�߂̃e�N�X�`�����쐬
//	ColorMapTexture[ 1 ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
//
//	// �ʏ�`�悵���f���ɋ����ڂ������������摜��ۑ����邽�߂̃e�N�X�`�����쐬
//	ColorMapTexture[ 2 ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
//
//	// �[�x�l��`�悷��P�`�����l���摜���쐬
//	SetCreateDrawValidGraphChannelNum( 1 );
//	DepthMapTexture = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
//	SetCreateDrawValidGraphChannelNum( 0 );
//
//	
//	// ���H�����摜��ۑ����邽�߂̃e�N�X�`�����쐬
//	for ( int i = 0; i < MAX_BLUR; i++ ) {
//		OldMapTexture[ i ] = MakeScreen( ShaderScreen_W, ShaderScreen_H, TRUE );
//	}
//
//	// �[�x�l�`��p�̒��_�V�F�[�_�[�ǂݍ���
//	Depth_NormalMeshVS = LoadVertexShader( "resource/shader/Depth_NormalMeshVS.vso" );
//	Depth_SkinMeshVS   = LoadVertexShader( "resource/shader/Depth_SkinMeshVS.vso" );
//
//	// �[�x�l�`��p�̃s�N�Z���V�F�[�_�[�ǂݍ���
//	DepthPS            = LoadPixelShader( "resource/shader/DepthPS.pso" );
//
//	// �[�x�l���g�p���Ĕ�ʊE�[�x�������s�����߂̃s�N�Z���V�F�[�_�[��ǂݍ���
//	DepthOfFieldPS     = LoadPixelShader( "resource/shader/DepthOfFieldPS.pso" );
//
//	// ��ʊE�[�x�����p�̃p�����[�^��������
//	DepthOfField_InterpSize = 3000.0f;
//	DepthOfField_camera_target	    = 3200.0f;
//	DepthOfField_camera_targetSize  = 500.0f;
//
//}


//void ViewerBlur::motionBlur( ) {
//	FLOAT4 ParamF;
//	float RightU, BottomV;
//	VERTEX2DSHADER Vertex[ 6 ];
//
//		// ��ʊE�[�x�`��̏���
//		{
//			// �ʏ�`��̌��ʂɂڂ������������摜��ۑ�
//			GraphFilterRectBlt( ColorMapTexture[ 0 ], ColorMapTexture[ 1 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, DX_GRAPH_FILTER_GAUSS, 8, 2000 );
//
//			// �ڂ������������摜�ɍX�ɂڂ����������ĕۑ�
//			GraphFilterRectBlt( ColorMapTexture[ 1 ], ColorMapTexture[ 2 ], 0, 0, ShaderScreen_W, ShaderScreen_H, 0, 0, DX_GRAPH_FILTER_GAUSS, 8, 2000 );
//			
//			// �[�x�l�`��p�e�N�X�`���ɐ[�x�l��`��
//			DrawScreen( DepthMapTexture, TRUE );
//		}
//
//		// �`���𗠉�ʂɕύX
//		SetDrawScreen( OldMapTexture[ 0 ] );
//		ClearDrawScreen( );
//
//		// ��ʊE�[�x���{�����摜�𗠉�ʂɕ`��
//		{
//			// �g�p����e�N�X�`�����Z�b�g
//			SetUseTextureToShader( 0, ColorMapTexture[ 0 ] );
//			SetUseTextureToShader( 1, ColorMapTexture[ 1 ] );
//			SetUseTextureToShader( 2, ColorMapTexture[ 2 ] );
//			SetUseTextureToShader( 3, DepthMapTexture );
//
//			// ��ʊE�[�x�����p�̃s�N�Z���V�F�[�_�[���Z�b�g
//			SetUsePixelShader( DepthOfFieldPS );
//
//			// ��ʊE�[�x�̂ڂ��������̊J�n�ʒu���v�Z
//			ParamF.x = 100;//DepthOfField_InterpSize / DepthOfField_TotalSize;
//
//			// ��ʊE�[�x�̂ڂ��������̏I���ʒu���v�Z
//			ParamF.y = ParamF.x;//( DepthOfField_InterpSize + DepthOfField_camera_targetSize ) / DepthOfField_TotalSize;
//
//			// �s�N�Z���V�F�[�_�[�̒萔�ɃZ�b�g
//			SetPSConstF( 0, ParamF );
//
//			// ��ʑS�̂ɕ`�悷�钸�_�̏���
//			Vertex[ 0 ].pos = VGet(				   	   0.0f,					0.0f, 0.0f );
//			Vertex[ 1 ].pos = VGet( ( float )ShaderScreen_W,					0.0f, 0.0f );
//			Vertex[ 2 ].pos = VGet(					   0.0f, ( float )ShaderScreen_H, 0.0f );
//			Vertex[ 3 ].pos = VGet( ( float )ShaderScreen_W, ( float )ShaderScreen_H, 0.0f );
//
//			Vertex[ 0 ].rhw = 1.0f;
//			Vertex[ 1 ].rhw = 1.0f;
//			Vertex[ 2 ].rhw = 1.0f;
//			Vertex[ 3 ].rhw = 1.0f;
//
//			RightU  = ( float )ShaderScreen_W / ShaderScreen_W;
//			BottomV = ( float )ShaderScreen_H / ShaderScreen_H;
//			Vertex[ 0 ].u = 0.0f;
//			Vertex[ 1 ].u = RightU;
//			Vertex[ 2 ].u = 0.0f;
//			Vertex[ 3 ].u = RightU;
//
//			Vertex[ 0 ].v = 0.0f;
//			Vertex[ 1 ].v = 0.0f;
//			Vertex[ 2 ].v = BottomV;
//			Vertex[ 3 ].v = BottomV;
//
//			Vertex[ 4 ] = Vertex[ 2 ];
//			Vertex[ 5 ] = Vertex[ 1 ];
//
//			// �|���S���Q�����g�p���ĉ�ʑS�̂ɔ�ʊE�[�x�������{�����R�c�V�[���̕`��
//			DrawPolygon2DToShader( Vertex, 2 );
//		}
//
//		
//		GetDrawScreenGraph( 0, 0, ShaderScreen_W, ShaderScreen_H, OldMapTexture[ 1 ] );
//
//		for ( int i = 1; i < MAX_BLUR - 1; i++ ) {
//			SetDrawScreen( OldMapTexture[ MAX_BLUR - ( i + 1 ) ] );
//			GetDrawScreenGraph( 0, 0, ShaderScreen_W, ShaderScreen_H, OldMapTexture[ MAX_BLUR - i ] );
//		}
//
//		// �`���𗠉�ʂɕύX
//		SetDrawScreen( DX_SCREEN_BACK );
//
//		for ( int i = 1; i < MAX_BLUR; i++ ) {
//			DrawGraph( 0, 0, OldMapTexture[ MAX_BLUR - i ], TRUE );
//		}
//
//	// �ǂݍ��񂾃V�F�[�_�[�̉���
//	SetUsePixelShader( -1 );
//	SetUseVertexShader( -1 );
//	MV1SetUseOrigShader( FALSE );
//
//	// �`��u�����h���[�h�����ɖ߂�
//	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 );
//		
//}
//
//void ViewerBlur::drawBegin( ) {
//	
//	// �`��u�����h���[�h���Z�b�g
//	SetDrawBlendMode( DX_BLENDMODE_PMA_ALPHA, 255 );
//	
//	// ��Ԕ͈͂ƃt�H�[�J�X�������Ă���͈͂��܂߂����������Z�o
//	DepthOfField_TotalSize = DepthOfField_InterpSize * 2.0f + DepthOfField_camera_targetSize;
//
//	// �ʏ�`��p�e�N�X�`���Ƀ��f����ʏ�`��
//	DrawScreen( ColorMapTexture[ 0 ], FALSE );
//}
//
//void ViewerBlur::drawEnd( ) {
//	draw( );
//}
//
//void ViewerBlur::drawMotionBlurBegin( ) {
//
//	// �`��u�����h���[�h���Z�b�g
//	SetDrawBlendMode( DX_BLENDMODE_PMA_ALPHA, 255 );
//	
//	// ��Ԕ͈͂ƃt�H�[�J�X�������Ă���͈͂��܂߂����������Z�o
//	DepthOfField_TotalSize = DepthOfField_InterpSize * 2.0f + DepthOfField_camera_targetSize;
//
//	// �ʏ�`��p�e�N�X�`���Ƀ��f����ʏ�`��
//	DrawScreen( ColorMapTexture[ 0 ], FALSE );
//}
//
//void ViewerBlur::drawMotionBlurEnd( ) {
//	motionBlur( );
//}
//
//void ViewerBlur::resetMoitonBlur( ) {
//	for ( int i = 0; i < MAX_BLUR; i++ ) {
//		SetDrawScreen( OldMapTexture[ i ] );
//		ClearDrawScreen( );
//	}
//	SetDrawScreen( DX_SCREEN_BACK );
//}