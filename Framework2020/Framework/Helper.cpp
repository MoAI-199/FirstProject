#include "Helper.h"

namespace Helper {
	DxLib::VECTOR convVectorToDxlib( const Vector& vec ) {
		return DxLib::VGet(
			( float )vec.x,
			( float )vec.y,
			( float )vec.z );
	}
};
