#pragma once

#include "smart_ptr.h"
#include "mathematics.h"
#include <string>
#include <map>
#include <vector>

PTR( Application );
PTR( Binary );
PTR( Introduction );
PTR( TaskManager );
PTR( Task );
PTR( Scene );

class Application {
public:
	Application( );
	virtual ~Application( );
public:
	static void createInstance( );
	static void destroyInstance( );
	static ApplicationPtr getInstance( );
public:
	void run( );
	void addTask( const std::string& tag, TaskPtr task );
	TaskPtr getTask( const std::string& tag );
	int getWindowWidth( ) const;
	int getWindowHeight( ) const;
	int getScreenWidth( ) const;
	int getScreenHeight( ) const;
	bool isWindowed( ) const;
	bool isEnabledD3D9( ) const;
	bool isEnabledShaderVersion( const int vnum ) const;
	int getNowCount( ) const;
	bool loadBinary( std::string filename, BinaryPtr binary );
	void saveBinary( std::string filename, BinaryPtr binary );
	std::string inputString( int sx, int sy );
	void terminate( );
	void changeWindow( bool windowed );
	void setWindowSize( int width, int height );
	void setScreenSize( int width, int height );
	void setNextSceneName( const std::string& name );
	void setFPS( unsigned int fps );
	unsigned int getFPS( ) const;
	double getActualFPS( ) const;
	Vector getCursor( int mouse_x, int mouse_y ) const;
	void fetchFilename( const char* format, std::vector< std::string >& files );
private:
	bool initialize( );
	void finalize( );
	void initSettings( );
	void updateScene( );
	void flip( );
	void waitForFlipping( );
private:
	static ApplicationPtr _instance;
	TaskManagerPtr _scene_task_mgr;
	TaskManagerPtr _global_task_mgr;
	bool _enable_scene_task_mgr;
	IntroductionPtr _introduction;
	std::string _next_scene_name;
	ScenePtr _scene;
	bool _terminating;
	int _screen_width;
	int _screen_height;
	int _window_width;
	int _window_height;
	bool _windowed;
	int _flipping_refresh_count;
	int _flipping_start_time;
	unsigned int _fps;
	unsigned int _fps_count;
	unsigned int _fps_start_time;
	double _actual_fps;
	bool _using_d3d9;
};
