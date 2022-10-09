#include "VFXEdge.h"
#include "Drawer.h"
#include "Primitive2DShader.h"
#include "ImageTarget.h"
#include "Model.h"
#include "PixelShader.h"
#include "Primitive.h"

const char* SHADER_SINGLE_COLOR_FILENAME = "SingleColor_PS.pso";
const char* SHADER_LAPLACIAN_FILTER      = "LaplacianFilter_PS.pso";

VFXEdge::VFXEdge( ModelPtr model ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	ApplicationPtr app = Application::getInstance( );

	_model = model;

	// �V�F�[�_�[�t�@�C���̃��[�h
	_shader_single_color = drawer->createPixelShader( SHADER_SINGLE_COLOR_FILENAME );
	_shader_laplacian_filter = drawer->createPixelShader( SHADER_LAPLACIAN_FILTER );
	
	// �E�B���h�E�T�C�Y�擾
	int width = app->getScreenWidth( );
	int height = app->getScreenHeight( );
	_screen_single_color = drawer->createImageTarget( width, height );
	_screen_laplacian_filter = drawer->createImageTarget( width, height );

	// ���f���̍쐬
	_primitive_2d_shader = drawer->createPrimitive2DShader( );
	_primitive_2d_shader->alloc( 2 );
	set( width, height );
}

VFXEdge::~VFXEdge( ) {

}
#if 0
void VFXEdge::draw( ) {
	// ���f���̕\��
	_model->setTrans( Matrix::makeTransformTranslation( Vector( -10, 0, 0 ) ) );
	_model->draw( );
}
#else
void VFXEdge::set( int width, int height ) {
	// ���_�ʒu
	Vector pos[ 4 ] = {
		Vector(		0,		0 ),
		Vector(	width,		0 ),
		Vector(		0, height ),
		Vector(	width, height ),
	};
	
	// �|���S���̌`��
	//Vector vertex[ 6 ];
	Primitive::VERTEX vtx[ 6 ];
	vtx[ 0 ].pos = pos[ 0 ];
	vtx[ 1 ].pos = pos[ 1 ];
	vtx[ 2 ].pos = pos[ 2 ];
	
	vtx[ 3 ].pos = pos[ 1 ];
	vtx[ 4 ].pos = pos[ 3 ];
	vtx[ 5 ].pos = pos[ 2 ];
	
	// ���_�f�[�^�̃Z�b�g
	int u[ 6 ] = { 0, 1, 0, 1, 1, 0 };
	int v[ 6 ] = { 0, 0, 1, 0, 1, 1 };
	for ( int i = 0; i < 6; i++ ) {
		vtx[ i ].color = Color( 1.0, 1.0, 1.0, 1.0 );
		vtx[ i ].u = u[ i ];
		vtx[ i ].v = v[ i ];
		_primitive_2d_shader->set( i, vtx[ i ] );
	}
}

void VFXEdge::draw( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope = drawer->createScope( );
	
	// �X�N���[��1�̐ݒ�
	drawer->setTarget( scope, _screen_single_color );
	drawer->clearScreen( );
	
	// ���f���̕\��
	_shader_single_color->usePixelShaderBegin( );
	_model->setTrans( Matrix::makeTransformTranslation( Vector( -10, 0, 0 ) ) );
	_model->draw(  );
	_shader_single_color->usePixelShaderEnd( );

	// �X�N���[��2�̐ݒ�
	drawer->setTarget( scope, _screen_laplacian_filter );
	drawer->clearScreen( );

	// SetUseTextureToShader( 0, _screenId[ SCREEN_1 ] );		// �X�N���[��1���摜�Ƃ��ēn��
	_shader_laplacian_filter->setUseTexture( 0, _screen_single_color );
	
	// SetPSConstF( 1, F4Get( _color._r / 255.0f, _color._g / 255.0f, _color._b / 255.0f,  _color._a / 255.0f ) );		// �G�b�W�J���[�̃Z�b�g
	static int alpha = 0;
	alpha = ( alpha + 1 ) % 50;
	int a = abs( alpha - 25 ) * 10;
	_shader_laplacian_filter->setColorToPSConstF( 1, Color( 1.0, 1.0, 0, 1.0 ) );
	
	// SetPSConstSF( 0, ( float )_size );		// �G�b�W�T�C�Y�̃Z�b�g
	_shader_laplacian_filter->setPSConstSF( 0, 2.0f );
	
	// DrawPrimitive2DToShader( _screenModel, 6, DX_PRIMTYPE_TRIANGLELIST );
	_primitive_2d_shader->draw( _shader_laplacian_filter );
	
	// �o�b�N�X�N���[���ɖ߂�
	//SetDrawScreen( DX_SCREEN_BACK );
	drawer->setTarget( scope );

	//SetCameraNearFar( 10.0f, 200.0f );
	//drawer->setNearFar( 10.0f, 200.0f );
	
	//SetupCamera_Perspective( 12.0f * DX_PI_F / 180.0f );
	//drawer->setCameraPerspective( 12.0f * PI / 180.0  );
	
	// �G�b�W�̕\��
	//DrawGraph( 0, 0, _screenId[ SCREEN_2 ], TRUE );	
	_screen_laplacian_filter->draw( Vector2( ) );
	
	// ���f���̕\��
	_model->setTrans( Matrix::makeTransformTranslation( Vector( -10, 0, 0 ) ) );
	_model->draw( );
}
#endif

//void ViewerEdge::drawBegin( ) {
//	// �X�N���[��1�̐ݒ�
//	SetDrawScreen( _screenId[ SCREEN_1 ] );
//	ClearDrawScreen( );
//
//	// �V�F�[�_�[�̃Z�b�g
//	MV1SetUseOrigShader( TRUE );
//	SetUsePixelShader( _shaderFiles[ SHADER_SINGLE_COLOR ] );
//}
//
//void ViewerEdge::drawEnd( ) {
//	// �X�N���[��2�̃Z�b�g
//	SetDrawScreen( _screenId[ SCREEN_2 ] );
//	ClearDrawScreen( );
//
//	// �`��
//	SetUsePixelShader( _shaderFiles[ SHADER_LAPLACIAN_FILTER ] );
//	SetUseTextureToShader( 0, _screenId[ SCREEN_1 ] );		// �X�N���[��1���摜�Ƃ��ēn��
//	SetPSConstF( 1, F4Get( _color._r / 255.0f, _color._g / 255.0f, _color._b / 255.0f,  _color._a / 255.0f ) );		// �G�b�W�J���[�̃Z�b�g
//	SetPSConstSF( 0, ( float )_size );		// �G�b�W�T�C�Y�̃Z�b�g
//	DrawPrimitive2DToShader( _screenModel, 6, DX_PRIMTYPE_TRIANGLELIST );
//	
//	MV1SetUseOrigShader( FALSE );
//	
//	// �V�F�[�_�[�̃��Z�b�g
//	SetUseVertexShader( -1 );
//	SetUsePixelShader( -1 );
//
//	// �o�b�N�X�N���[���ɖ߂�
//	SetDrawScreen( DX_SCREEN_BACK );
//	SetCameraNearFar( 10.0f, 200.0f );
//	SetupCamera_Perspective( 12.0f * DX_PI_F / 180.0f );
//
//	// �G�b�W�̕\��
//	DrawGraph( 0, 0, _screenId[ SCREEN_2 ], TRUE );		
//}
