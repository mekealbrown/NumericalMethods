#include <cmath>
#include <fenv.h>
#include <iomanip>
#include <iostream>
#include <cfenv>
#include <string>

typedef double (*functions)(double);

//test functions
double firstEq(double x) {return 7 - (x * x);}
double secondEq(double x) {return 3 + (2 * x) - (x * x);}
double thirdEq(double x) {return (std::sqrt(4 - (x * x))) - 1;}
double fourthEq(double x) {return std::sin(x);}

void changeRoundMethod(int x, std::string &s)
{
    switch (x) {
        case 0:
            std::fesetround(FE_TONEAREST);
            s = "RN";
            break;
        case 1:
            std::fesetround(FE_UPWARD);
            s = "RU";
            break;
        case 2:
            std::fesetround(FE_DOWNWARD);
            s = "RD";
            break;
        case 3:
            std::fesetround(FE_TOWARDZERO);
            s = "RZ";
            break;
    }
}

//absolute and relative error of each solution
void absRelError(double &x, double &y, char prob, double solution) 
{
    switch (prob) {
        case '1':
            x = std::abs(std::sqrt(7) - solution);
            y = std::abs(x / std::sqrt(7));
            break;
        case '2':
            x = std::abs(3 - solution);
            y = std::abs(x / 3);
            break;
        case '3':
            x = std::abs(std::sqrt(3) * -1 - solution);
            y = std::abs(x / std::sqrt(3) * -1);
            break;
        case '4':
            x = std::abs(2 * M_PI - solution); 
            y = std::abs(x / 2 * M_PI);
            break;
    }
}

//bisection root method algorithm as explained in the wiki page dedicated to it
double bisectionRootMethod(double(*fp)(double func), double lower, double upper, char which, double tol)
{ 
    double root = lower;
    while ((upper - lower) >= tol)
    {
        root = (lower + upper) / 2; //middle point
        if (fp(root) == 0.0)
            break;
        else if (fp(root) * fp(lower) < 0)
            upper = root;
        else 
            lower = root;
    }
    return root;
}
int main()
{
    double (*functions[4])(double){firstEq, secondEq, thirdEq, fourthEq}; //store each function 
    double lowerBound, upperBound, tolerance;
    char whichFunc;
    int precision;
    char rmOnOff;
    std::cout << "Enter which equation to test: "; //1-4 corresponds to the 4 equations in the problem respectively
    std::cin >> whichFunc;
    std::cout << "Tolerance: "; //tolerance refers to how close to zero we get before saying we're close enough
    std::cin >> tolerance;
    std::cout << "Enter the precision: ";   //allow variable rounding precision
    std::cin >> precision;
    std::cout << "Rounding modes switching on? ";  //'y' or 'n'
    std::cin >> rmOnOff;
    switch (whichFunc) {  //set bounds for each equation
        case '1':
            lowerBound = -0.5;
            upperBound = 12;
            break;
        case '2':
            lowerBound = -0.5;
            upperBound = 13;
            break;
        case '3':
            lowerBound = -2;
            upperBound = 1;
            break;
        case '4':
            lowerBound = 4;
            upperBound = 8;
            break;
    }
    std::cout << "                                               Solution                                               " << std::endl 
              << "------------------------------------------------------------------------------------------------------\n" << std::fixed;

    std::string currMethod;
    double solution;
    double absolute, relative;

    if (rmOnOff == 'y') //display solutions with dif rounding modes if turned on
    {    
        for (int i{0}; i < 4; ++i) //print table showing differences in rounding methods
        {
            changeRoundMethod(i, currMethod);
            solution = bisectionRootMethod(functions[((int)whichFunc - '0') - 1], lowerBound, upperBound, whichFunc, tolerance);
            absRelError(absolute, relative, whichFunc, solution);
            std::cout << std::setprecision(precision) << '[' << currMethod << "] : " << solution 
                      << std::setw(20) << std::right << "  absolute error: " << absolute << " relative error: " << relative << '%' << std::endl;
        }
    }
    else 
    {
        solution = bisectionRootMethod(functions[((int)whichFunc - '0') - 1], lowerBound, upperBound, whichFunc, tolerance);
        absRelError(absolute, relative, whichFunc, solution);
        std::cout << std::setprecision(precision) << "solution: " << solution << std::setw(20) << std::right << "  absolute error: " << absolute 
                  << " relative error: " << relative << '%' << std::endl;
    }
    std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
    return 0;
}