/**
 * adapté d'un code par emreerdin (https://github.com/emreerdin/SimpleLinearRegressioninC/tree/main)
 * (MIT licence)
 */

#include <stdio.h>
#include <stdlib.h>

double CalculateMean(const double *coords, int length);
double CalculateSlope(const double *x, const double *y, int length, double xMean, double yMean);
double CalculateIntercept(double slope, double xMean, double yMean);
double MeanSquaredError(const double *x, const double *y, int length, double slope, double intercept);

int linreg(int nbPoints, const double x[], const double y[], double *pente, double *y0, double *error)
{
	double xMean = CalculateMean(x, nbPoints);
    double yMean = CalculateMean(y, nbPoints);

    *pente = CalculateSlope(x, y, nbPoints, xMean, yMean);
	*y0 = CalculateIntercept(*pente, xMean, yMean);
    *error = MeanSquaredError(x, y, nbPoints, *pente, *y0);

	return 0;
}

double CalculateMean(const double *coords, int length)
{
	double sum = 0;
	int i;
	
	for(i=0; i<length; i++)		
		sum += coords[i];

	return sum/length;	
}


double CalculateSlope(const double *x, const double *y, int length, double xMean, double yMean)
{
	double nom = 0;
	double den = 0;
	int i;
	
	for(i=0; i<length; i++)
	{
		nom += (x[i] - xMean) * (y[i] - yMean);
		den += (x[i] - xMean) * (x[i] - xMean);
	}

    if(den == 0)
        return 0.;

    return nom / den;
}


double CalculateIntercept(double slope, double xMean, double yMean)
{	
	return yMean - (slope * xMean);	
}


double MeanSquaredError(const double *x, const double *y, int length, double slope, double intercept)
{
	double sum = 0;
	int i;
	double yPred = 0;
	double error = 0;

    for(i=0; i<length; i++)
    {
		yPred = (slope * x[i]) + intercept;
		error = y[i] - yPred;
		sum += error * error;
	}

	return sum/length;
}

