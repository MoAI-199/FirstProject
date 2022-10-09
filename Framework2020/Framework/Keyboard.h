#pragma once

#include "task.h"
#include <string>
#include <map>

PTR( Keyboard );

class Keyboard : public Task {
public:
	/// <summary>
	/// 各キーの数値を文字列へ代入
	/// </summary>
	Keyboard( );
	virtual ~Keyboard( );
public:
	///<summry>
	///GetHitKeyStateAllで複数のキーの入力を取得できるようにしている
	///</summry>
	virtual void update( );
public:
	/// <summary> 指定したキーが押された瞬間の検知 </summary> <param name="key"> 検知するキー </param> <returns> 指定したキーのtrue </returns>
	bool isPushKey( std::string key );
	/// <summary>
	/// 特定のキーが押されている間の検知
	/// </summary>
	/// <param name="key"> 検知するキー </param>
	/// <returns> 指定したキーのtrue </returns>
	bool isHoldKey( std::string key );
	///<summary>
	///文字入力バッファに溜まった文字データから１文字取得する
	///</summary>
	char getInputChar( );
private:
	struct KEYS {
		char buffer[ 256 ];
	};
private:
	KEYS _now_keys;
	KEYS _before_keys;
	std::map< std::string, int > _convert;
};
