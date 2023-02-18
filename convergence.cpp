#include <cmath>
#include <fenv.h>
#include <iomanip>
#include <iostream>
#include <cfenv>
#include <math.h>
#include <string>
#include <vector>
#include <limits>

/*
    To print tabular results for each iteration,
    comment out the printing after the loop and uncomment the printing in the loop.
*/

//define function pointers
typedef double (*functions)(double);
typedef double (*derivative)(double);

//test functions
double firstEq(double x) {return 7 - std::pow(x,2);}
double secondEq(double x) {return 3 + (2 * x) - std::pow(x,2);}
double thirdEq(double x) {return (std::sqrt(4 - std::pow(x,2))) - 1;}
double fourthEq(double x) {return std::sin(x);}
double fifthEq(double x) {return 2 * std::pow(x,3) - 4 * std::pow(x,2) + 3 * x;}

//derivatives for test functions for use in Newtons Method
double firstPrime(double x) {return -2 * x;}
double secondPrime(double x) {return 2 - (2 * x);}
double thirdPrime(double x) {return (x / (std::sqrt(4 - std::pow(x,2)))) * -1;}
double fourthPrime(double x) {return std::cos(x);}
double fifthPrime(double x) {return 6 * std::pow(x,2) - 8 * x + 3;}

//  tabular printing for ease of data visualization
void print(int iter, double trueSol, double sol, double error) 
{
    std::cout << std::fixed << std::setprecision(15);
    std::cout << iter << "|" << trueSol << " |" <<std::setfill(' ') << std::setw(15) << sol << "    |" << std::setfill(' ') << std::setw(15)
                  << std::abs(error) << std::endl;
    std::cout << std::setfill('-') << std::setw(68) << "\n";
}

//solution for original equation
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

//returns 
void bitsPrecision(int &x, char prob, double solution) 
{
    double error;
    switch (prob) {
        case '1':
            error = std::abs(std::sqrt(7) - solution);
            x = (-std::ilogb(error) - 1);
            break;
        case '2':
            error = std::abs(3 - solution);
            x = -std::ilogb(error) - 1;
            break;
        case '3':
            error = std::abs(std::sqrt(3) * -1 - solution);
            x = -std::ilogb(error) - 1;
            break;
        case '4':
            error = std::abs(2 * M_PI - solution); 
            x = -std::ilogb(error) - 1;
            break;
        case '5':
            error = 0 - solution;
            x = -std::ilogb(error) - 1;
    }
}

//bisection root finding method algorithm as explained in the wiki page dedicated to it
void bisectionsolutionMethod(double(*fp)(double func), double lower, double upper, int &iterations, double &solution, int &bits, char which, double sol)
{ 
    std::cout << "Bisection" << std::endl;
    std::cout << "i |" << "x |               |" << "f(x) |               |" << "precision" << std::endl;
    std::cout << std::setfill('-') << std::setw(68) << "\n";
    solution = lower;
    iterations = 0;
    double avg{0};
    while (upper != lower && iterations < 100)
    {
        solution = (lower + upper) / 2; //middle point
        if (signbit(fp(solution)) == signbit(fp(lower)))
            lower = solution;
        else 
            upper = solution;
        ++iterations;
        bitsPrecision(bits, which, solution);
        if (bits == 2147483647)
        {
            print(iterations, sol, solution, bits);
            std::cout << std::endl;
            std::cerr << "Out of bounds...\n\n";
            break;
        }
        print(iterations, sol, solution, bits);
        std::cout << std::endl;
    }
    //print(iterations, sol, solution, avg / iterations);
    //std::cout << std::endl;
}

//false position root finding method
void regulaFalsi(double(*fp)(double func), double lower, double upper, int &iterations, double &solution, char which, int &bits, double sol)
{
    std::cout << "False Position" << std::endl;
    std::cout << "i |" << "x |               |" << "f(x) |               |" << "precision" << std::endl;
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
        bitsPrecision(bits, which, solution);
        if (bits == 2147483647)
        {
            print(iterations, sol, solution, bits);
            std::cout << std::endl;
            std::cerr << "Out of bounds...\n\n";
            break;
        }
        print(iterations, sol, solution, bits);
        std::cout << std::endl;
    }
    while(upper != lower && iterations < 100);
    //print(iterations, sol, solution, avg / iterations);
 //   std::cout << std::endl;
}

//Illinois root finding algorithm 
void illinois(double(*fp)(double func), double lower, double upper, int &iterations, double &solution, char which, int &bits, double sol) 
{
    std::cout << "Illinois" << std::endl;
    std::cout << "i |" << "x |               |" << "f(x) |               |" << "precision\n";
    std::cout << std::setfill('-') << std::setw(68) << "\n";
    double c, fc;
    int n, side = 0;
    int max_inter{15};
    iterations = 0;
    double avg{0};
    double fa = fp(lower);
    double fb = fp(upper);

    while (upper != lower && iterations < 100)
    {
        solution = (fa * upper - fb * lower) / (fa - fb);
        fc = fp(solution);
        if (fc * fb > 0) 
        {
            upper = solution; fb = fc;
            if (side == -1)
               fa /= 2;
            side = -1;
        } 
        else if (fa * fc > 0) 
        {
           lower = solution; fa = fc;
           if (side == +1)
              fb /= 2;
           side = +1;
        } 
        
        ++iterations;
        bitsPrecision(bits, which, solution);
        if (bits == 2147483647)
        {
            print(iterations, sol, solution, bits);
            std::cout << std::endl;
            std::cerr << "Out of bounds...\n\n";
            break;
        }
        print(iterations, sol, solution, bits);
        std::cout << std::endl;
    } 
    //print(iterations, sol, solution, avg / iterations);
    //std::cout << std::endl;
}  

//Newton's Method
void newtons_method(double(*fp)(double func), double(*fp1)(double func), double guess, int &iterations, double &solution, char which, int &bits, double sol)
{
    std::cout << "Newton's Method" << std::endl;
    std::cout << "i |" << "x |               |" << "f(x) |               |" << "precision\n";
    std::cout << std::setfill('-') << std::setw(68) << "\n";
    double avg{0}, f0, prime;
    iterations = 0;

    do
	{
		prime = fp1(guess);
		f0 = fp(guess);
		solution = guess - f0/prime;
		guess = solution;
		
		++iterations;
        bitsPrecision(bits, which, solution);
        if (bits == 2147483647)
        {
            print(iterations, sol, solution, bits);
            std::cout << std::endl;
            std::cerr << "Out of bounds...\n\n";
            break;
        }
        print(iterations, sol, solution, bits);
        std::cout << std::endl;

	} while(iterations < 100); //need to find a better terminating condition than this smh

    //print(iterations, sol, solution, avg / iterations);
    //std::cout << std::endl;     
} 

//secant method
void secant(double(*fp)(double func), double lower, double upper, int &iterations, double &solution, char which, int &bits, double sol)
{
    std::cout << "Secant Method" << std::endl;
    std::cout << "i |" << "x |               |" << "f(x) |               |" << "precision\n";
    std::cout << std::setfill('-') << std::setw(68) << "\n";
    iterations = 0;
    double avg{0};
    while (iterations < 100)
    {
        solution = upper - fp(upper) * (upper - lower) / (fp(upper) - fp(lower));
        lower = upper;
        upper = solution;
        ++iterations;

        bitsPrecision(bits, which, solution);
        if (bits == 2147483647)  
        {
            print(iterations, sol, solution, bits);
            std::cout << std::endl;
            std::cerr << "Out of bounds...\n\n";
            break;
        }
        print(iterations, sol, solution, bits);
        std::cout << std::endl;
    }
    //print(iterations, sol, solution, avg / iterations);
    //std::cout << std::endl; 
}


int main()
{
    double (*functions[5])(double){firstEq, secondEq, thirdEq, fourthEq, fifthEq}; //store each function 
    double (*derivative[5])(double){firstPrime, secondPrime, thirdPrime, fourthPrime, fifthPrime};
    double lowerBound, upperBound, solution, trueSol; //trueSol is actual value of root
    char whichFunc;
    int precision, iterations, bPrec;
    std::cout << "Enter which equation to test: "; //1-5 corresponds to the 5 equations in the problem respectively
    std::cin >> whichFunc;
    assignBounds(whichFunc, lowerBound, upperBound); 
    solutions(whichFunc, trueSol); //get trueSol to equal solution for equation we're testing
    bisectionsolutionMethod(functions[((int)whichFunc - '0') - 1], lowerBound, upperBound, iterations, solution, bPrec, whichFunc, trueSol);
    regulaFalsi(functions[((int)whichFunc - '0') - 1], lowerBound, upperBound, iterations, solution, whichFunc, bPrec, trueSol);
    illinois(functions[((int)whichFunc - '0') - 1], lowerBound, upperBound, iterations, solution, whichFunc, bPrec, trueSol);
    newtons_method(functions[((int)whichFunc - '0') - 1], derivative[((int)whichFunc - '0') - 1], upperBound, iterations, solution, whichFunc, bPrec, trueSol);
    secant(functions[((int)whichFunc - '0') - 1], lowerBound, upperBound, iterations, solution, whichFunc, bPrec, trueSol);

    return 0;
}