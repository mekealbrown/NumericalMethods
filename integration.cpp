#include <cmath>
#include <fenv.h>
#include <iomanip>
#include <iostream>
#include <cfenv>
#include <string>

typedef double (*functions)(double);

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
            x = std::abs(18 - solution);
            y = x / 18;
            break;
        case '2':
            x = std::abs((32. / 3.) - solution);
            y = x / (32. / 3.);
            break;
        case '3':
            x = std::abs((M_PI / 4.) - solution);
            y = x / M_PI;
            break;
        case '4':
            x = std::abs(1. - solution); 
            y = x / 1.;
            break;
    }
}

//test functions
double firstEq(double x) {return 7 - (x * x);}
double secondEq(double x) {return 3 + (2 * x) - (x * x);}
double thirdEq(double x) {return std::sqrt(1 - (x * x));}
double fourthEq(double x) {return std::sin(x);}

double riemannSum(double(*fp)(double func), double lower, double upper, double subInt, char which, char rsVersion)
{ 
    double sum{0.0};
    switch (rsVersion) {
        case 'l':   //left rule
            for (int i{0}; i < subInt - ((upper - lower) / subInt); ++i)
                sum += fp(lower + (i * (upper - lower) / subInt));
            break;
        case 'r':   //right rule
            for (int i{0}; i < subInt; ++i)
                sum += fp(lower + ((i + 1) * (upper - lower) / subInt));
            break;
        case 'm':   //midpoint rule
            for (int i{0}; i < (subInt - ((upper - lower) / subInt / 2)); ++i)
                sum += fp(lower + (i + 0.5) * (upper - lower) / subInt) * ((upper - lower) / subInt);
            return sum;
        case 't':   //trapazoidal rule
            sum = fp(lower) + fp(upper);
            for (int i{0}; i < subInt; ++i)
                sum += 2 * fp(lower + i * (upper - lower) / subInt);
            return sum * (0.5 * (upper - lower) / subInt);
    }
    return sum * ((upper - lower) / subInt);
}
int main()
{
    double (*functions[4])(double){firstEq, secondEq, thirdEq, fourthEq}; //store each function 
    double lowerBound, upperBound, subInterval;
    char whichFunc, rsVersion;
    int precision;
    std::cout << "Enter a lower, upper bound, and subinterval: " << std::endl;
    std::cin >> lowerBound >> upperBound >> subInterval;
    std::cout << "Enter which equation and Riemann Sum rule to use: " << std::endl; //1-4 corresponds to the 4 equations in the problem respectively
    std::cin >> whichFunc >> rsVersion;
    std::cout << "Enter the precision: " << std::endl;   //allow variable rounding precision
    std::cin >> precision;
    std::cout << std::endl << std::fixed;

    std::string currMethod;
    double solution;
    double absolute, relative;
    for (int i{0}; i < 4; ++i) //print table showing differences in rounding methods
    {
        changeRoundMethod(i, currMethod);
        solution = riemannSum(functions[((int)whichFunc - '0') - 1], lowerBound, upperBound, subInterval, whichFunc , rsVersion);
        absRelError(absolute, relative, whichFunc, solution);
        std::cout << std::setprecision(precision) << '[' << currMethod << "] : " << solution 
                  << std::setw(20) << std::right << "  absolute error: " << absolute << " relative error: " << relative << "%" << std::endl;
    }
    std::cout << std::endl;
    return 0;
}