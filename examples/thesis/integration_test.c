#include <stdlib.h>

double trapezoidal_integration_double(double a, double b, int n, double f(double)){

	const double d = (b - a) / (double)n;
	double sum = 0;

	sum += (f(a) + f(b)) / 2;

	for(int i = 1; i < n; i++)
	{
		double temp = f(a + i * d);
		sum += temp;
		//printf("%f = %f\n", a + i, temp);
	}

	sum *= d;

	//printf("double summe: %f\n", sum);

	return sum;
}

double trapezoidal_step(double a, double b, int n, double f(double)){

	const double d = (b - a) / n;
	double sum = 0;

	double sample_last = f(a) * d;

	for(int i = 1; i < n + 1; i++)
	{
		double sample_now = f(a + i * d) * d;
		double area = (sample_now - sample_last) / 2;

		sum += sample_last + area;
		sample_last = sample_now;
	}

	//printf("step summe: %f\n", sum);

	return sum;
}

double trapezoidal_twice(double a, double b, int n, double f(double)){

	const double d = (b - a) / (double)n;
	double sum = 0;

	double sample_last = f(a) * d;

	//double results[10];

	double *results = (double*)calloc(n, sizeof(double));

	for(int i = 1; i < n + 1; i++)
	{
		double sample_now = f(a + i * d) * d;
		double area = (sample_now - sample_last) / 2;

		double trapezoid = sample_last + area;

		//results[i - 1] = trapezoid;

		sum += trapezoid;
		results[i - 1] = sum;
		sample_last = sample_now;
		//printf("%i result: %f\n", i - 1, sum);
	}

	//printf("first step summe: %f\n", sum);
	//puts("");
	sum = 0;
	sample_last = results[0] * d;

	for(int i = 1; i < n; i++)
	{
		double sample_now = results[i] * d;
		double area = (sample_now - sample_last) / 2;

		double trapezoid = sample_last + area;

		sum += trapezoid;
		sample_last = sample_now;

		//printf("%i results: %f\n", i, results[i]);
		//printf("%i trapez: %f summe: %f\n", i, trapezoid, sum);
	}

	//printf("step summe: %f\n", sum);

	free(results);

	return sum;
}
