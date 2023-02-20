#include <cstdint>
#include <cmath>
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
        exponent |= 1UL << e; 
        exponent -= 1UL;
    };                

    //Ill test (52, 11, 4) first
    FP(int s,int e,int x)
    {
        significand_size = s;
        exponent_size = e;
        sign = x > 0 ? 0 : 1;
        sign == 0 ? 0 : x *= -1;
        exponent |= (1 << e - 1);  //exponent offset

        uint32_t exp_mask{(1U << e) - 1U};
        uint64_t sig_mask{(1UL << s) - 1UL};

        uint64_t temp{static_cast<uint64_t>(x)};
        while (temp > 0)
        {
            temp >>= 1UL; 
            if (temp == 0)
                break;
            temp > 0UL ? ++exponent : --exponent;
        } 
        if (exponent >= exp_mask)
        {
            exponent = (exponent << e + 1U) - 1U; 
            significand = 0;
        }
        
        temp = static_cast<uint64_t>(x);
        uint64_t max_sig{1UL << s + 1};
        while ((temp & max_sig) == 0)
            temp <<= 1;

        significand = ~max_sig & temp; //gets trailing significand
    };
    FP(int s,int e,std::string x);

    bool isNaN()       {return exponent >= (exponent | exponent_size + 1UL) - 1UL && significand != 0;}
    bool isZero()      {return exponent == 0 && significand == 0;}
    bool isInfinity()  {return exponent >= (exponent << exponent_size + 1) - 1UL && significand == 0;}
    bool isPositive()  {return !sign;}
    bool isNormal()    {return exponent != 0 && exponent != (exponent | exponent_size + 1UL) - 1UL && isNaN() != true;} 
    bool isSubnormal() {return exponent == 0 && significand > 0 && !isNaN();}

    std::string to_string()
    {
        std::string result{};
        if (isNaN())
        {
            isPositive() ? result += "+" : result += "-";
            result += "NAN";
            return result;
        }
        else if (isInfinity())
        {
            isPositive() ? result += "+" : result += "-";
            result += "INF";
            return result;
        }
        else if (isZero())
        {
            isPositive() ? result += "+" : result += "-";
            result += "0";
            return result;
        }
        isPositive() ? result += "+" : result += "-";
        isNormal() ? result += "1." : result += "0.";
        
        uint64_t sig_cp{significand}; uint64_t sig_cp_mask{1UL << significand_size};
        uint32_t ex_offset{};
        ex_offset |= (1UL << exponent_size - 1UL);
        uint32_t ex_size{exponent - ex_offset};
 
        while (ex_size--)
        {
            (sig_cp & sig_cp_mask) == 0 ? result += "0" : result += "1";
            sig_cp_mask >>= 1UL;
        }
        
        result += "b" + std::to_string(exponent - ex_offset);
        return result;
    }

    bool        addOne(); // if possible add 1

    // for testing
    uint64_t    getSignificand()    const { return significand; }
    uint32_t    getExponent()       const { return exponent; }
};