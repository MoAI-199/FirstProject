#include "TestScene.h"
#include "Drawer.h"
#include "Application.h"
#include "Log.h"
#include "VFXEdge.h"
#include "VFXWater.h"
#include "VFXMirror.h"
#include "VFXFirePillar.h"
#include "Fire.h"

SCENE( TestScene );

TestScene::TestScene( ) {

}

TestScene::~TestScene( ) {

}

void TestScene::initialize( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	drawer->setUseLight( false );

	_model = ModelPtr( new Model );
	_model->load( "player_summoner_wait.mv1" );
	_model->setScale( Matrix::makeTransformScaling(	Vector( 0.1, 0.1, 0.1 ) ) );

	_vfx_edge = VFXEdgePtr( new VFXEdge( _model ) );
	_vfx_water = VFXWaterPtr( new VFXWater( ) );
	_vfx_mirror = VFXMirrorPtr( new VFXMirror );
	
	// model
	_pos = Vector( 0, 10, -150 );
	_fire_pos = Vector( 1, 0, 0 );
	
	// water
	//_pos = Vector( 0, 15, -80 );

	drawer->setCamera( Vector( 30, 0, 0 ), Vector( ) );
	//_pos = Vector( 0, 15, -10 );

}

void TestScene::update( ) {
	DrawerPtr drawer = Task::get< Drawer >( );

	// model
	//Matrix mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), 0.01 );
	//_pos = mat.multiply( _pos );
	drawer->setCamera( _pos, Vector( 0, 10, 0 ) );

	// water
	//Matrix mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), 0.01 );
	//_pos = mat.multiply( _pos );
	//drawer->setCamera( _pos, Vector( 15, 1, 15 ) );

	// モデル
	_model->setTrans( Matrix::makeTransformTranslation( Vector( 10, 0, 0 ) ) );
	//_model->draw( );
	Matrix max = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), 0.5 );
	_fire_pos = max.multiply( _fire_pos );
	//_fire = FirePtr( new Fire( pos ) );
	_vfx_fire_pillar = VFXFirePillarPtr( new VFXFirePillar( _fire_pos ) );
	Drawer::Scope scope = drawer->createScope( );
	drawer->setBlend( scope, Drawer::BLEND::ADD, 1.0 );
	_vfx_fire_pillar->draw( );
	//_fire->draw( );
	
	// エッジモデル
	//_vfx_edge->draw( );


	// ウォーター
	//_vfx_water->draw( );
	//drawer->setCamera( Vector( 0, 15, 0 ), Vector( 15, 1, 15 ) );
	////drawer->drawSphere( Vector( 15.5, 1, 15.5 ), 1, 15, Color( 1.0, 0.0, 0.0, 1 ), false );

	// ミラー
	//static Vector pos = drawer->getCameraPos( );
	//pos.x += 1;
	//
	//drawer->setCamera( pos, Vector( ) );

	//drawer->drawSphere( Vector( ), 1, 15, Color( 1.0, 0.0, 0.0, 1 ), false );
}