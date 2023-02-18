#include <cstdint>
#include <string>

class FP {
    uint64_t    significand;        // trailing
    uint32_t    exponent;
    bool        sign;
    uint8_t     significand_size;
    uint8_t     exponent_size;
public:
    FP(int s,int e) //NAN
    {
        significand_size = s;
        exponent_size = e;
        significand = 1;
        exponent = UINT32_MAX;
    };                

    //Ill test (52, 11, 4) first
    FP(int s,int e,int x)
    {
        significand_size = s;
        exponent_size = e;
        sign = x > 0 ? 0 : 1;
        
    };
    FP(int s,int e,std::string x);

    bool isNaN() {return exponent == UINT32_MAX && significand > 0;}
    bool isZero() {return exponent == 0 && significand == 0;}
    bool isInfinity() {return exponent == UINT32_MAX && significand == 0;}
    bool isPositive() {return !sign;}
    bool isNormal() {return exponent > 0;}
    bool isSubnormal() {return !isNormal();}

    std::string to_string();

    bool        addOne(); // if possible add 1

    // for testing
    uint64_t    getSignificand()    const { return significand; }
    uint32_t    getExponent()       const { return exponent; }
};