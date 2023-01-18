#include <cmath>
#include <fenv.h>
#include <iomanip>
#include <iostream>
#include <cfenv>

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

void absRelError(long double &x, long double &y, int prob, long double solution) //absolute and relative error of each solution
{
    switch (prob) {
        case 0:
            x = 18. - solution;
            y = x / 18.;
            break;
        case 1:
            x = (32. / 3.) - solution;
            y = x / (32. / 3.);
            break;
        case 2:
            x = (3.14159265359 / 4.) - solution;
            y = x / 3.14159265359;
            break;
        case 3:
            x = 1. - solution;
            y = x / 1.;
            break;
    }
}

long double firstEq(double x) {return 7 - (x * x);}

long double secondEq(double x) {return 3 + (2 * x) - (x * x);}

long double thirdEq(double x) {return std::sqrt(1 - (x * x));}

long double fourthEq(double x) {return std::sin(x);}

long double riemannSum(double lower, double upper, double subInt, char which, char rsVersion)
{
    double dX{(upper - lower) / subInt};
    double sum{0.0};
    switch (rsVersion) {
        case 'l':   //left rule
            for (int i{0}; i < subInt - dX; ++i)
            {
                if (which == '1') {sum += firstEq(lower + (i * dX));}
                else if (which == '2') {sum += secondEq(lower + (i * dX));}
                else if (which == '3') {sum += thirdEq(lower + (i * dX));}
                else if (which == '4') {sum += fourthEq(lower + (i * dX));}
            }
            break;
        case 'r':   //right rule
            for (int i{0}; i < subInt; ++i)
            {
                if (which == '1') {sum += firstEq(lower + ((i + 1) * dX));}
                else if (which == '2') {sum += secondEq(lower + ((i + 1) * dX));}
                else if (which == '3') {sum += thirdEq(lower + ((i + 1) * dX));}
                else if (which == '4') {sum += fourthEq(lower + ((i + 1) * dX));}
            }
            break;
        case 'm':   //midpoint rule
            for (int i{0}; i < (subInt - (dX / 2)); ++i)
            {
                if (which == '1') {sum += firstEq(lower + (i + 0.5) * dX) * dX;}
                else if (which == '2') {sum += secondEq(lower + (i + 0.5) * dX) * dX;}
                else if (which == '3') {sum += thirdEq(lower + (i + 0.5) * dX) * dX;}
                else if (which == '4') {sum += fourthEq(lower + (i + 0.5) * dX) * dX ;}
            }
            return sum;
        case 't':   //trapazoidal rule
            if (which == '1') {sum = firstEq(lower) + firstEq(upper);}
            else if (which == '2') {sum = secondEq(lower) + secondEq(upper);}
            else if (which == '3') {sum = thirdEq(lower) + thirdEq(upper);}
            else if (which == '4') {sum = fourthEq(lower) + fourthEq(upper);}
            for (int i{0}; i < subInt; ++i)
            {
                if (which == '1') {sum += 2 * firstEq(lower + i * dX);}
                else if (which == '2') {sum += 2 * secondEq(lower + i * dX);}
                else if (which == '3') {sum += 2 * thirdEq(lower + i * dX);}
                else if (which == '4') {sum += 2 * fourthEq(lower + i * dX);}
            }
            return sum * (0.5 * dX);
    }
    
    return sum * dX;
}
int main()
{
    long double lowerBound, upperBound, subInterval;
    char whichFunc, rsVersion;
    int precision;
    std::cout << "Enter a lower, upper bound, and subinterval: " << std::endl;
    std::cin >> lowerBound >> upperBound >> subInterval;
    std::cout << "Enter which equation and Riemann Sum rule to use: " << std::endl; //1-4 corresponds to the 4 equations in the problem respectively
    std::cin >> whichFunc >> rsVersion;
    std::cout << "Enter the precision: " << std::endl;   //allow variable rounding precision
    std::cin >> precision;
    std::cout.precision(precision);
    long double solution{riemannSum(lowerBound, upperBound, subInterval, whichFunc , rsVersion)};

    std::cout << std::endl;
    std::string currMethod;
    long double absolute, relative;
    for (int i{0}; i < 4; ++i) //print table showing differences in rounding methods
    {
        changeRoundMethod(i, currMethod);
        absRelError(absolute, relative, i, solution);
        std::cout << '[' << currMethod << "] : " << solution << std::setw(20) << std::right << "  absolute error: " << absolute << " relative error: " << relative << "%" << std::endl;
    }
    std::cout << std::endl;
}