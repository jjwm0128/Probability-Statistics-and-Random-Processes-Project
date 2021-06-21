#ifndef EX4_11_PROJECT
#define EX4_11_PROJECT

#define PI 3.141592

double* x_generator(double v, double p, double x[], int size);
double* n_generator(double n[], int size);
double SimpsonIntegration(double a, double b, double h, double v, bool zero_to_one_error);
double Given_function(double y, double v, bool zero_to_one_error);
void Ex_Error_Of_Given_Receiver(double v, double p);

#endif