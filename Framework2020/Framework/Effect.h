#pragma once
#include "smart_ptr.h"
#include "mathematics.h"

PTR( Effect );

class Effect {
public:
	Effect( int resource_handle );
	virtual ~Effect( );

public:
	/// <summary>
	/// 3Dエフェクトの描画を停止する
	/// </summary>
	void stop( ) const;
	/// <summary> エフェクトの値設定 </summary>
	/// <param name="pos"> 座標 </param>
	/// <param name="size"> 多きさ（1.0） </param>
	/// <param name="rotate"> 回転 </param>
	void setTransform( const Vector& pos, const double size = 1.0, const Vector& rotate = Vector( ) );

private:
	int _play_handle;
};

