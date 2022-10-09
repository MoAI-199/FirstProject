#pragma once

struct Color {
	double r;
	double g;
	double b;
	double a;
	Color( ) :
		r( 1.0 ), g( 1.0 ), b( 1.0 ), a( 1.0 ) {
	}
	Color( double red, double green, double blue, double alpha = 1.0 ) :
		r( red ), g( green ), b( blue ), a( alpha ) {
	}
};
