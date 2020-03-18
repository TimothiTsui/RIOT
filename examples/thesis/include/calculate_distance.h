
#ifndef EXAMPLES_THESIS_CALCULATE_DISTANCE_H_
#define EXAMPLES_THESIS_CALCULATE_DISTANCE_H_

#define G_FACTOR 9.80665
#define G_MILLI_FACTOR 0.00980665

//Convert g to meters
double g_to_m(double g);
double milli_g_to_m(int g);

//Calculate distance
double calculate_distance(double acceleration, int time);

double calculate_distance_g(int milli_g, int time);


#endif /* EXAMPLES_THESIS_CALCULATE_DISTANCE_H_ */
