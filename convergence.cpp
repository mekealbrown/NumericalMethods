#include <cmath>
#include <fenv.h>
#include <iomanip>
#include <iostream>
#include <cfenv>
#include <math.h>
#include <string>
#include <vector>

typedef double (*functions)(double);

//test functions
double firstEq(double x) {return 7 - (x * x);}
double secondEq(double x) {return 3 + (2 * x) - (x * x);}
double thirdEq(double x) {return (std::sqrt(4 - (x * x))) - 1;}
double fourthEq(double x) {return std::sin(x);}
double fifthEq(double x) {return 2 * (x * x * x) - 4 * (x * x) + 3 * x;}

//tabular printing for ease of data visualization
void print(int iter, double trueSol, double sol, double error) 
{
    std::cout << std::fixed << std::setprecision(15);
    std::cout << iter << " |" << trueSol << " |" <<std::setfill(' ') << std::setw(15) << sol << "    |" << std::setfill(' ') << std::setw(15)
                  << error << std::endl;
    std::cout << std::setfill('-') << std::setw(68) << "\n";
}

//set bounds for each equation
void solutions(char c, double &val) 
{
    switch (c) {  
        case '1':
            val = sqrt(7);
            break;
        case '2':
            val = 3;
            break;
        case '3':
            val = sqrt(3) * -1;
            break;
        case '4':
            val = 2 * M_PI;
            break;
        case '5':
            val = 0;
    }
}

//assign lower and upper bounds based on which function we're using
void assignBounds(char c, double &lower, double &upper)
{
    switch (c) {  //set bounds for each equation
        case '1':
            lower = -0.5;
            upper = 12;
            break;
        case '2':
            lower = -0.5;
            upper = 13;
            break;
        case '3':
            lower = -2;
            upper = 1;
            break;
        case '4':
            lower = 4;
            upper = 8;
            break;
        case '5':
            lower = -1;
            upper = 1;
    }
}

//absolute and relative error of each solution
void absError(double &x, char prob, double solution) 
{
    switch (prob) {
        case '1':
            x = std::abs(std::sqrt(7) - solution);
            break;
        case '2':
            x = std::abs(3 - solution);
            break;
        case '3':
            x = std::abs(std::sqrt(3) * -1 - solution);
            break;
        case '4':
            x = std::abs(2 * M_PI - solution); 
            break;
        case '5':
            x = 0 - solution;
    }
}

//bisection root finding method algorithm as explained in the wiki page dedicated to it
void bisectionsolutionMethod(double(*fp)(double func), double lower, double upper, double tol, int &iterations, double &solution, double &error, char which, double sol)
{ 
    solution = lower;
    iterations = 0;
    while ((upper - lower) >= tol)
    {
        solution = (lower + upper) / 2; //middle point
        if (fp(solution) == 0.0)
            break;
        else if (fp(solution) * fp(lower) < 0)
            upper = solution;
        else 
            lower = solution;
        ++iterations;
        absError(error, which, solution);
        print(iterations, sol, solution, error);
        std::cout << std::endl;
    }
}

//false position root finding method
void regulaFalsi(double(*fp)(double func), double lower, double upper, double tol, int &iterations, double &solution, char which, double &error, double sol)
{
    double hold, f0{fp(lower)}, f1{fp(upper)}; 
    iterations = 0;
    do  
    {
        solution = lower - (lower - upper) * f0 / (f0 - f1);
        hold = fp(solution);
        if (f0 * hold < 0)
        {    upper = solution; 
            f1 = hold;
        }
        else 
        {
            lower = solution;
            f0 = hold;
        }
        ++iterations;
        absError(error, which, solution);
        print(iterations, sol, solution, error);
        std::cout << std::endl;
    }
    while(fabs(hold) > tol);
}

//Illinois root finding algorithm 
void illinois(double(*fp)(double func), double lower, double upper, double tol, int &iterations, double &solution, char which, double &error, double sol) 
{
    iterations = 0;
    double a{1}, fa = fp(a);
    double  b{2}, fb = fp(b);

    do
    {
        solution = (a * fb - b * fa) / (fb - fa);
        double fc = fp(solution);
        
        if(signbit(fc) != signbit(fa))
        {
            b = a; 
            fb = fa;
        }
        else
            fb *= 0.5;

        a = solution; fa = fc;
        
        if(fabs(fc) < tol) 
            break;

        ++iterations;
        absError(error, which, solution);
        print(iterations, sol, solution, error);
        std::cout << std::endl;
    }
    while(fabs(b - a) > tol);   
}


int main()
{
    double (*functions[5])(double){firstEq, secondEq, thirdEq, fourthEq, fifthEq}; //store each function 
    double lowerBound, upperBound, tolerance, absolute, solution, trueSol;
    char whichFunc;
    int precision, iterations;
    std::cout << "Enter which equation to test: "; //1-5 corresponds to the 5 equations in the problem respectively
    std::cin >> whichFunc;
    std::cout << "Tolerance: "; //tolerance refers to how close to zero we get before saying we're close enough
    std::cin >> tolerance;
    assignBounds(whichFunc, lowerBound, upperBound); 
    solutions(whichFunc, trueSol); //get trueSol to equal solution for equation we're testing

    std::cout << "Bisection\n";
    std::cout << "i |" << "x |               |" << "f(x) |               |" << "error\n"; // << std::endl;
    std::cout << std::setfill('-') << std::setw(68) << "\n";
    bisectionsolutionMethod(functions[((int)whichFunc - '0') - 1], lowerBound, upperBound, tolerance, iterations, solution, absolute, whichFunc, trueSol);
    std::cout << "False Position\n";
    std::cout << "i |" << "x |               |" << "f(x) |               |" << "error\n"; // << std::endl;
    std::cout << std::setfill('-') << std::setw(68) << "\n";
    regulaFalsi(functions[((int)whichFunc - '0') - 1], lowerBound, upperBound,tolerance, iterations, solution, whichFunc, absolute, trueSol);
    std::cout << "Illinois\n";
    std::cout << "i |" << "x |               |" << "f(x) |               |" << "error\n"; // << std::endl;
    std::cout << std::setfill('-') << std::setw(68) << "\n";
    illinois(functions[((int)whichFunc - '0') - 1], lowerBound, upperBound, tolerance, iterations, solution, whichFunc, absolute, trueSol);
    return 0;
}