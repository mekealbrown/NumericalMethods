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
        significand = 1UL;
        //exponent = 0;
        exponent = 0 | (1UL << e) - 1UL; 
        //exponent -= 1UL;
    };                

    //Ill test (52, 11, 4) first
    FP(int s,int e,int x)
    {
        significand_size = s;
        exponent_size = e;
        sign = x > 0 ? 0 : 1;
        sign == 0 ? 0 : x *= -1;
        exponent = 1U << (e - 1U);
        //set exponent
        uint64_t temp{static_cast<uint32_t>(x)};
        uint32_t ex_mask{(1U << e) - 1U};
        while (temp != 1U)
        {
            temp >>= 1U;
            ++exponent;
        }
        if (exponent >= ex_mask) //set INF 
        {
            exponent = ex_mask;
            significand = 1UL;
            return;
        }
        //set significand
        significand = static_cast<uint64_t>(x);
        temp = static_cast<uint64_t>(x);
        uint64_t max_sig{1UL << s};
        uint64_t sig_mask{(1UL << s) - 1UL};
        while ((temp & max_sig) == 0Ul)
            temp <<= 1Ul;
        significand = (temp & sig_mask);
    };
    FP(int s,int e,std::string x);

    bool isNaN()       {return exponent >= (1 << exponent_size) - 1UL && significand != 0;}
    bool isZero()      {return exponent == 0 && significand == 0;}
    bool isInfinity()  {return exponent >= (exponent << (exponent_size + 1UL)) - 1UL && significand == 0;}
    bool isPositive()  {return !sign;}
    bool isNormal()    {return exponent != 0 && exponent != (exponent | (exponent_size + 1UL)) - 1UL && isNaN() != true;} 
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
        
        uint64_t sig_mask{1UL << (significand_size - 1UL)};
        while (sig_mask != 1)
        {
            (significand & sig_mask) == 0 ? result += "0" : result += "1";
            sig_mask >>= 1UL;
        }
        

        while (true)
        {
            if (*(result.end() - 1) == '1')
                break;
            result.erase(result.end() - 1);
        }

        uint32_t ex_offset{1U << (exponent_size - 1U)};
        result += "b" + std::to_string(exponent - ex_offset);
        return result;
    }

    bool        addOne(); // if possible add 1

    // for testing
    uint64_t    getSignificand()    const { return significand; }
    uint32_t    getExponent()       const { return exponent; }
};