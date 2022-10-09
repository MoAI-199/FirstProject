#include "Effect.h"
#ifndef EFFEKSEER
Effect::Effect( int resource_handle ) { }
Effect::~Effect( ) { }
void Effect::stop( ) const { }
void Effect::setTransform( const Vector& pos, const double size, const Vector& rotate ) { }
#else
#include "EffekseerForDXLib.h"

Effect::Effect( int resource_handle ) {
	_play_handle = PlayEffekseer3DEffect( resource_handle );
}

Effect::~Effect( ) {
}

void Effect::stop( ) const {
	StopEffekseer3DEffect( _play_handle );
}

void Effect::setTransform( const Vector& pos, const double size, const Vector& rotate ) {
	SetScalePlayingEffekseer3DEffect( _play_handle,
		( float )size, ( float )size, ( float )size );
	SetRotationPlayingEffekseer3DEffect( _play_handle,
		( float )rotate.x, ( float )rotate.y, ( float )rotate.z );
	SetPosPlayingEffekseer3DEffect( _play_handle,
		( float )pos.x, ( float )pos.y, ( float )pos.z );
}
#endif