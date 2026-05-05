// Source - https://stackoverflow.com/a/19040841
// Posted by Mark Lakata, modified by community. See post 'Timeline' for change history
// Retrieved 2026-05-05, License - CC BY-SA 3.0

#include "linreg.h"
#include <stdlib.h>
#include <math.h>


inline static double sqr(double x) {
    return x*x;
}


/**
 * @brief régression linéaire pour des points de coordonnées (x, y).
 * 
 * @param n nombre de points
 * @param x abscisses des points
 * @param y ordonnées des points
 * @param m pente
 * @param b ordonnée à l'origine
 * @param r coeficient de corélation 
 * @return 0 si le calcul s'est déroulé comme prévu
 */
int linreg(int n, const double x[], const double y[], double* m, double* b, double* r){
    double   sumx = 0.0;                      /* sum of x     */
    double   sumx2 = 0.0;                     /* sum of x**2  */
    double   sumxy = 0.0;                     /* sum of x * y */
    double   sumy = 0.0;                      /* sum of y     */
    double   sumy2 = 0.0;                     /* sum of y**2  */

    for (int i=0;i<n;i++){ 
        sumx  += x[i];       
        sumx2 += sqr(x[i]);  
        sumxy += x[i] * y[i];
        sumy  += y[i];      
        sumy2 += sqr(y[i]); 
    } 

    double denom = (n * sumx2 - sqr(sumx));
    if (denom == 0) {
        // singular matrix. can't solve the problem.
        *m = 0;
        *b = 0;
        if (r) *r = 0;
            return 1;
    }

    *m = (n * sumxy  -  sumx * sumy) / denom;
    *b = (sumy * sumx2  -  sumx * sumxy) / denom;
    if (r!=NULL) {
        *r = (sumxy - sumx * sumy / n) /    /* compute correlation coeff */
              sqrt((sumx2 - sqr(sumx)/n) *
              (sumy2 - sqr(sumy)/n));
    }

    return 0; 
}
