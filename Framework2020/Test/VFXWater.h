#pragma once
#include "smart_ptr.h"
#include "mathematics.h"
#include "Primitive.h"
#include <vector>

PTR( VFXWater );
PTR( Primitive );
PTR( Primitive3DShader );
PTR( PixelShader );
PTR( VertexShader );

class VFXWater {
public:
	VFXWater( );
	virtual ~VFXWater( );
public:
	void draw( );
private:
	void addPos( int x, int y, int z );
	void drawShader( );
	void drawNoneShader( );
	void loadImage( const char* filename );
	// �U���̐ݒ�
	void setAmplitude( double index );
	// �����̑��x
	void setPeriodSpeed( double speed );
	// �g�̍쐬
	void create( );
	// �������[�m��
	void allocate( );
	// �e�N�X�`���[�̐ݒ�
	void setPlank( int num, const Vector* vertex );
	void initialize( );
	void setVertexRegister( );
	void setPixelRegister( );
	void setTexture( );
private:
	struct Parameter {
		float period;		// ����
		float amplitude;	// �U��

		Parameter( ) {
			period = 0;
			amplitude = 0.2f;
		}
	};
private:
	PrimitivePtr _model_water;
	Primitive::VERTEX _vertex;
	Primitive3DShaderPtr _primitive_3dshader;
	PixelShaderPtr _pixel_shader;
	VertexShaderPtr _vertex_shader;
	std::vector< Vector > _array;
	Parameter _parameter;
	bool _shaderVersion;
	int _size_x;
	int _size_y;
	float _time;
	float _speed;
};