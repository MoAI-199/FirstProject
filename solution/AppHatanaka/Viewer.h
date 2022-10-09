#pragma once
#include "smart_ptr.h"
#include "Image.h"
#include "inside.h"

PTR( Player );
PTR( Map );
PTR( Inside );
PTR( Time );
PTR( Score );

class Viewer {
public:
	Viewer( PlayerConstPtr player, MapConstPtr map, InsideConstPtr inside, TimeConstPtr time,ScorePtr score );
	virtual ~Viewer( );
public:
	void draw( ) ;
private:
	void drawPlayer( ) const;
	void drawBackground( )const;
	void drawFrame( ) const;
	void drawInside( );
	void drawStatus( Vector2& pos ,Inside::STATUS num ) ;
	void drawNumber( Vector2 pos, int number );
	void drawNumberRed( Vector2 pos, int number );
	void drawCountNumber( Vector2 pos, int number );
	void drawCounter( );
	void drawCountdown( );
	void drawClearResult( );
	void drawGameOverResult( );
	void drawScore( Vector2 pos, int number );
private:
	ImagePtr _image_player;
	ImagePtr _image_bg;
	ImagePtr _image_frame;
	ImagePtr _image_normal;
	ImagePtr _image_flash;
	ImagePtr _image_lost_1;
	ImagePtr _image_lost_2;
	ImagePtr _image_lost;

	ImagePtr _image_num;
	ImagePtr _image_num_red;
	ImagePtr _image_countdown;
	ImagePtr _image_gray;
	ImagePtr _image_game_ui;
	ImagePtr _image_inside_target;
	ImagePtr _image_count_start;


	ImagePtr _image_result_clear;
	ImagePtr _image_result_gameover;
	ImagePtr _image_result_score_vatule;

	PlayerConstPtr _player;
	MapConstPtr _map;
	InsideConstPtr _inside;
	TimeConstPtr _time;
	ScorePtr _score;
};

