#include "calculate_distance.h"


/*
https://www.me-systeme.de (Werte stimmen mit anderen Seiten überein)
g zu m/s^2
0.1g = 0.980665
0.2g = 1.96133
0.3g = 2.941995
0.5g = 4.903325
1.0g = 9.80665
2.0g = 19.6133
3.0g = 29.41995
5.0g = 49.03325


*/

//Convert  g to m/s^2
double g_to_m(double g){

	//const double gFactor = 9.80665;

	return g * G_FACTOR;
}

//Convert milli g to m/s^2
double milli_g_to_m(int g){

	//const double gFactor = 0.00980665;

	return g * G_MILLI_FACTOR;
}



/*
calculate_distance
distance = 1/2 * acceleration * time^2

acceleration = m/s^2
time = ms

returns distance in cm
*/
double calculate_distance(double acceleration, int time){
	return acceleration / 2 * time * time / 10000;
}

/*
calculate_distance_g
distance = 1/2 * acceleration * time^2

milli_g = g/1000
time = ms

returns distance in cm
*/
double calculate_distance_g(int milli_g, int time){
	return calculate_distance(milli_g * G_MILLI_FACTOR, time);
}



