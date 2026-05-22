/**
 * borrowed from emreerdin (https://github.com/emreerdin/SimpleLinearRegressioninC/tree/main)
 * under MIT licence
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct{
	
	double x;
	double y;
} DataSet;

double CalculateXMean(DataSet dataSet[], int length);
double CalculateYMean(DataSet dataSet[], int length);
double CalculateSlope(DataSet dataSet[], int length, double xMean, double yMean);
double CalculateIntercept(double slope, double xMean, double yMean);
double MeanSquaredError(DataSet dataSet[], int length, double slope, double intercept);

DataSet* convertieEnDataSet(const double x[], const double y[], int nbPoints)
{
    DataSet *data = malloc(nbPoints * sizeof(DataSet));

    for(int i = 0 ; i < nbPoints ; ++i)
        data[i] = (DataSet){x[i], y[i]};

    return data;
}

int linreg(int nbPoints, const double x[], const double y[], double *pente, double *y0, double *error)
{
	DataSet *dataSet = convertieEnDataSet(x, y, nbPoints);

	double xMean = CalculateXMean(dataSet, nbPoints);
    double yMean = CalculateYMean(dataSet, nbPoints);

    *pente = CalculateSlope(dataSet, nbPoints, xMean, yMean);
	*y0 = CalculateIntercept(*pente, xMean, yMean);
    *error = MeanSquaredError(dataSet, nbPoints, *pente, *y0);

	free(dataSet);

	return 0;
}

double CalculateXMean(DataSet dataSet[], int length){
	
	double sum = 0;
	int i;
	
	for(i=0; i<length; i++){
		
		sum += dataSet[i].x;
		
	}
	
	return sum/length;	
}

double CalculateYMean(DataSet dataSet[], int length){
	
	double sum = 0;
	int i;
	
	for(i=0; i<length; i++){
		
		sum += dataSet[i].y;
		
	}
	
	return sum/length;
}


double CalculateSlope(DataSet dataSet[], int length, double xMean, double yMean){
	
	
	double nom = 0;
	double den = 0;
	int i;
	
	for(i=0; i<length; i++){
		
		nom += (dataSet[i].x - xMean) * (dataSet[i].y - yMean);
		den += (dataSet[i].x - xMean) * (dataSet[i].x - xMean);
	}
	
    if(den == 0)
        return 0.;

    return nom / den;
}


double CalculateIntercept(double slope, double xMean, double yMean){
	
	return yMean - (slope * xMean);
	
}


double MeanSquaredError(DataSet dataSet[], int length, double slope, double intercept)
{
	double sum = 0;
	int i;
	double yPred = 0;
	double error = 0;

    for(i=0; i<length; i++)
    {
		yPred = (slope*dataSet[i].x) + intercept;
		error = dataSet[i].y - yPred;
		sum += error * error;
	}

	return sum/length;
}









