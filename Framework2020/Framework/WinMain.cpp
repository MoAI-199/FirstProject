#include "DxLib.h"
#include "Application.h"
#include "Exception.h"
#include "Mouse.h"
#include "Introduction.h"

void messageBox( Exception& exception ) {
	std::string filename = exception.getFilename( );
	int off = ( int )filename.find_last_of( '\\' );
	if ( off != std::string::npos ) {
		off += 1;
	}


	std::string str_error = exception.getError( );
	std::string str_file = ( std::string )"ファイル：" + filename.substr( off, filename.length( ) );
	std::string str_line = ( std::string )"行：" + std::to_string( exception.getLine( ) );

	int font = 16;
	int bw = ( int )str_error.length( ) * ( font + 2 ) / 2 + 20;
	int bh = 130;
	if ( bw < 450 ) bw = 450;

	int str_error_y = 20;
	int str_file_y = 40;
	int str_line_y = 60;


	//設定
	SetWindowText( "Exception!" );
	SetFontSize( font );
	SetWindowStyleMode( 5 );
	ChangeWindowMode( TRUE );
	SetGraphMode( bw, bh, 32 );
	SetWindowSize( bw, bh );
	SetDrawScreen( DX_SCREEN_BACK );
	SetBackgroundColor( 255, 255, 255 );
	ClearDrawScreen( );

	//描画
	DrawBox( 0, bh / 2 + 10, bw, bh, 0xeeeeee, TRUE );
	DrawBox( bw - 100 - 5, bh - 40 - 5, bw - 10 + 5, bh - 10 + 5, 0xaaaaaa, TRUE );
	DrawBox( bw - 100, bh - 40, bw - 10, bh - 10, 0xffffff, TRUE );

	DrawString ( bw - 64, bh - font - 15, "OK", 0x000000 );
	DrawString ( ( bw - ( int )str_error.length( ) * ( font + 2 ) / 2 ) / 2, str_error_y, str_error.c_str( ), 0x000000 );

	SetFontSize( 12 );
	DrawString( 10, str_file_y, str_file.c_str( ), 0x000000 );
	DrawString( 10, str_line_y, str_line.c_str( ), 0x000000 );

	ScreenFlip( );
	WaitKey( );
}

int WINAPI WinMain( HINSTANCE hInstaece, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	Application::createInstance( );

	try {
		Application::getInstance( )->run( );
	} catch ( Exception& e ) {
		messageBox( e );
	}

	Application::destroyInstance( );

	return 0;
}