
#ifndef EXAMPLES_THESIS_INTEGRATION_TEST_H_
#define EXAMPLES_THESIS_INTEGRATION_TEST_H_


double trapezoidal_integration_double(double a, double b, int n, double f(double));

double trapezoidal_step(double a, double b, int n, double f(double));

double trapezoidal_twice(double a, double b, int n, double f(double));


#endif /* EXAMPLES_THESIS_INTEGRATION_TEST_H_ */
