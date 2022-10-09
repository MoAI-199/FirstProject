#pragma once
#include "Image.h"
#include <string>

class ImageTarget : public Image {
	friend class Drawer;
public:
	/// <summary>
	/// 処理なし
	/// </summary>
	virtual ~ImageTarget( );
public:
	/// <summary>
	/// ターゲットのセーブ
	/// </summary>
	/// <param name="path">ファイルのアクセスパス</param>
	void save( std::string path );
protected:
	/// <summary>
	/// 処理なし
	/// </summary>
	ImageTarget( );
private:
	/// <summary>
	/// スクリーンを作る
	/// </summary>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	void create( int width, int height );
};

