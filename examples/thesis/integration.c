#include "integration.h"
#include <stdio.h>

double trapezoidal_twice_array_2(int n, double dataArr[]){

	const double d = 1;

	double sum = 0;
	double sum_integral = 0;

	double sample_last = dataArr[0] * d;
	double sample_last_integral = 0;

//	for(int i = 0; i < 100; i++)
//	{
//		printf("array: %i, val: %f\n", i, 2.5);
//	}

	for(int i = 1; i < n + 1; i++)
	{
		double sample_now = dataArr[i] * d;
		double area = (sample_now - sample_last) / 2;

		double trapezoid = sample_last + area;


		sum += trapezoid;
		sample_last = sample_now;
		//printf("%i result: %f\n", i - 1, sum);

		if(i == 1){

			sample_last_integral = sum * d;

		}else{

			double sample_now_integral = sum * d;
			double area_integral = (sample_now_integral - sample_last_integral) / 2;

			double trapezoid_integral = sample_last_integral + area_integral;

			sum_integral += trapezoid_integral;
			sample_last_integral = sample_now_integral;
		}
	}

	//printf("step summe: %i\n", (int)sum_integral);

	return sum;
}



double trapezoidal_twice_array_integer(int n, int dataArr[]){

	const int d = 1;

	int sum = 0;
	int sum_integral = 0;

	//average test
//	int avg = 0;
//	for(int i = 0; i < n; i++)
//	{
//		avg += dataArr[i];
//	}
//	avg /= n;

	int sample_last = dataArr[0] * d;
	int sample_last_integral = 0;

	for(int i = 1; i < n + 1; i++)
	{
		int sample_now = dataArr[i] * d;
		int area = (sample_now - sample_last) >> 1;

		int trapezoid = sample_last + area;


		sum += trapezoid;
		sample_last = sample_now;
		//printf("%i result: %f\n", i - 1, sum);

		if(i == 1){

			sample_last_integral = sum * d;

		}else{

			int sample_now_integral = sum * d;
			int area_integral = (sample_now_integral - sample_last_integral) >> 1;

			int trapezoid_integral = sample_last_integral + area_integral;

			sum_integral += trapezoid_integral;
			sample_last_integral = sample_now_integral;
		}
	}

	//printf("step summe int: %i\n", sum_integral);
	//printf("step summe dub: %f\n", (sum_integral * 0.0980665));

	//Bei 100 samples für cm
	int distance = (int)((sum_integral * 9.80665) / 100000);

	//Bei 10 samples für cm
	//int distance = (int)((sum_integral * 9.80665) / 1000);

	//printf("step summe dub: %i\n", (int)sum_integral);

	//return sum;
	return distance;
}
