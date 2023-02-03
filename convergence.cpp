#include <cmath>
#include <fenv.h>
#include <iomanip>
#include <iostream>
#include <cfenv>
#include <math.h>
#include <string>
#include <vector>

/*
    In each root finding function, at the bottom of the main loop is a 
    section that contains a call to 'print' and then a standard out for endl.
    After the loop is a section of the exact same code except for the last
    parameter in 'print' being different. The current setup calculated the average
    absolute error and passes that to print. To print tabular results for each iteration,
    comment out the printing after the loop and uncomment the printing in the loop.
*/



typedef double (*functions)(double);

//test functions
double firstEq(double x) {return 7 - (x * x);}
double secondEq(double x) {return 3 + (2 * x) - (x * x);}
double thirdEq(double x) {return (std::sqrt(4 - (x * x))) - 1;}
double fourthEq(double x) {return std::sin(x);}
double fifthEq(double x) {return 2 * (x * x * x) - 4 * (x * x) + 3 * x;}

/*  tabular printing for ease of data visualization
    may take an average for 'error' to print mean of error once
*/
void print(int iter, double trueSol, double sol, double error) 
{
    std::cout << std::fixed << std::setprecision(15);
    std::cout << iter << "|" << trueSol << " |" <<std::setfill(' ') << std::setw(15) << sol << "    |" << std::setfill(' ') << std::setw(15)
                  << std::abs(error) << std::endl;
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
    std::cout << "Bisection" << std::endl;
    std::cout << "i |" << "x |               |" << "f(x) |               |" << "error" << std::endl;
    std::cout << std::setfill('-') << std::setw(68) << "\n";
    solution = lower;
    iterations = 0;
    double avg{0};
    while ((upper - lower) >= tol)
    {
        solution = (lower + upper) / 2; //middle point
        if (fp(solution) == 0.0)
        {
            ++iterations;
            absError(error, which, solution);
            print(iterations, sol, solution, error);
            std::cout << std::endl;
            break;
        }
        else if (signbit(fp(solution)) == signbit(fp(lower)))
            lower = solution;
        else 
            upper = solution;
        ++iterations;
        absError(error, which, solution);
        avg += error;
        //print(iterations, sol, solution, error);
        //std::cout << std::endl;
    }
    print(iterations, sol, solution, avg / iterations);
    std::cout << std::endl;
}

//false position root finding method
void regulaFalsi(double(*fp)(double func), double lower, double upper, double tol, int &iterations, double &solution, char which, double &error, double sol)
{
    std::cout << "False Position" << std::endl;
    std::cout << "i |" << "x |               |" << "f(x) |               |" << "error" << std::endl;
    std::cout << std::setfill('-') << std::setw(68) << "\n";
    double hold, f0{fp(lower)}, f1{fp(upper)}; 
    iterations = 0;
    double avg{0};
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
        avg += error;
        //print(iterations, sol, solution, error);
        //std::cout << std::endl;
    }
    while(fabs(hold) > tol);
    print(iterations, sol, solution, avg / iterations);
    std::cout << std::endl;
}

//Illinois root finding algorithm 
void illinois(double(*fp)(double func), double lower, double upper, double tol, int &iterations, double &solution, char which, double &error, double sol) 
{
    std::cout << "Illinois" << std::endl;
    std::cout << "i |" << "x |               |" << "f(x) |               |" << "error\n";
    std::cout << std::setfill('-') << std::setw(68) << "\n";
    double c, fc;
    int n, side = 0;
    int max_inter{15};
    iterations = 0;
    double avg{0};
    /* starting values at endpoints of interval */
    double fa = fp(lower);
    double fb = fp(upper);

    for (n = 0; n < max_inter; n++) 
    {
         solution = (fa * upper - fb * lower) / (fa - fb);
         if (fabs(upper - lower) < tol * fabs(upper + lower))
            break;
         fc = fp(solution);

         if (fc * fb > 0) 
         {
             upper = solution; fb = fc;
             if (side == -1)
                fa /= 2;
             side = -1;
         } else if (fa * fc > 0) {
            /* fc and fa have same sign, copy c to a */
            lower = solution; fa = fc;
            if (side == +1)
               fb /= 2;
            side = +1;
         } else {
            /* fc * f_ very small (looks like zero) */
            break;
         }
         ++iterations;
         absError(error, which, solution);
         avg += error;
         //print(iterations, sol, solution, error);
         //std::cout << std::endl;
     } 
     print(iterations, sol, solution, avg / iterations);
     std::cout << std::endl;
}  



int main()
{
    double (*functions[5])(double){firstEq, secondEq, thirdEq, fourthEq, fifthEq}; //store each function 
    double lowerBound, upperBound, tolerance, absolute, solution, trueSol; //trueSol is actual value of root 
    char whichFunc;
    int precision, iterations;
    std::cout << "Enter which equation to test: "; //1-5 corresponds to the 5 equations in the problem respectively
    std::cin >> whichFunc;
    std::cout << "Tolerance: "; //tolerance refers to how close to zero we get before saying we're close enough
    std::cin >> tolerance;
    assignBounds(whichFunc, lowerBound, upperBound); 
    solutions(whichFunc, trueSol); //get trueSol to equal solution for equation we're testing
    bisectionsolutionMethod(functions[((int)whichFunc - '0') - 1], lowerBound, upperBound, tolerance, iterations, solution, absolute, whichFunc, trueSol);
    regulaFalsi(functions[((int)whichFunc - '0') - 1], lowerBound, upperBound,tolerance, iterations, solution, whichFunc, absolute, trueSol);
    illinois(functions[((int)whichFunc - '0') - 1], lowerBound, upperBound, tolerance, iterations, solution, whichFunc, absolute, trueSol);
    
    return 0;
}