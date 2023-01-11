#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>

template <typename T>
void pentiumDiv()
{
    T data =  8392067./13082905.;
    std::cout << "Pentium division bug: " << std::setprecision(20) << data;
}

template <typename T>
void convergence()
{
    T u{2}; T v{-4}; T w;
    for (int i{3}; i < std::numeric_limits<int>::max(); ++i)
    {
        w = 111 - 1130 / v + 3000 / (v * u);
        u = v;
        v = w;
        if (v == 100)
        {
            std::cout << "convergence bug iteration: " << i;
            break;
        }
    }
}

template <typename T>
void interest()
{

    T account = 1.71828182845904523536028747135; 
    T start{account};
    int i;
    for (i = 1; i <= 25; i++)
        {
            account = i*account - 1;
            if (account > start)
            {
                std::cout << "interest bug answer: " << i + 1;
                break;
            }
        }
}

template <typename T>
void rump()
{
    T a = 77617;
    T b = 33096;
    T b2, b4, b6, b8, a2, firstexpr, f;
    b2 = b * b;
    b4 = b2 * b2;
    b6 = b4 * b2;
    b8 = b4 * b4;
    a2 = a * a;
    firstexpr = (11 * a2 * b2) - b6 - (121 * b4) - 2;
    f = (333.75 * b6) + (a2 * firstexpr) + (5.5 * b8) + (a / (2.0 * b));
    std::cout << "Rump bug solution: " << std::setprecision(20) << f;
}

int main()
{
    pentiumDiv<float>(); std::cout << " *float*\n";
    pentiumDiv<double>(); std::cout << " *double*\n";
    pentiumDiv<long double>(); std::cout << " *long double*\n\n";
    convergence<float>(); std::cout << " *float*\n";
    convergence<double>(); std::cout << " *double*\n";
    convergence<long double>(); std::cout << " *long double*\n\n";
    interest<float>(); std::cout << " *float*\n";
    interest<double>(); std::cout << " *double*\n";
    interest<long double>(); std::cout << " *long double*\n\n";
    rump<float>(); std::cout << " *float*\n";
    rump<double>(); std::cout << " *double*\n";
    rump<long double>(); std::cout << " *long double*\n";
}