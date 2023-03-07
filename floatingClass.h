#include <cstdint>
#include <cmath>
#include <bitset>
#include <string>
#include <cstdlib>
#include <iostream>

#define DEBUG(a,b) std::cout << "sig " << a << " other sig " << b << "\n";


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
        exponent = (1U << e) - 1U; 
        sign = false;
    };                

    FP(const FP &other)
    {
        significand_size = other.significand_size;
        exponent_size = other.exponent_size;
        significand = other.significand;
        exponent = other.exponent;
        sign = other.sign;
    }

    FP(int s,int e,int x)
    {
        if (x == 0)
        {
            exponent_size = e; 
            significand_size = s;
            exponent = significand = 0;
            sign = false;
            return;
        }
        significand_size = s;
        exponent_size = e;
        sign = x > 0 ? 0 : 1;
        sign == 0 ? 0 : x *= -1;
        exponent = 1U << (e - 1U);
        //set exponent
        uint64_t temp{static_cast<uint32_t>(x)};
        uint32_t ex_mask{(1U << e) - 1U};
        while (temp > 1U){ temp >>= 1U; ++exponent;}
        if (exponent >= ex_mask) //set INF 
        {
            exponent = ex_mask;
            significand = 0UL;
            return;
        }
        //set significand
        significand = static_cast<uint64_t>(x);
        temp = static_cast<uint64_t>(x);
        uint64_t max_sig{1UL << s};
        uint64_t sig_mask{(1UL << s) - 1UL};
        while (temp >= (1UL << (s + 1UL)))
            temp >>= 1UL;
        while ((temp & max_sig) == 0Ul)
            temp <<= 1Ul;

        significand = (temp & sig_mask);
    }
    FP(int s,int e,std::string x)
    {
        significand_size = s;
        exponent_size = e;
        if (x[0] != '+' && x[0] != '-') 
        {
            significand = 1; 
            exponent = (1U << e) - 1U; 
            return;
        }
        sign = (x[0] == '+' ? false : true);
        x.erase(0, 1);
        if (x[0] == '0') {significand = exponent = 0; return;}
        else if (x[0] == 'N') {significand = 1; exponent = (1U << e) - 1U; return;}
        else if (x[0] == 'I') {significand = 0; exponent = (1U << e) - 1U; return;}
        else if (x[0] != '1') {significand = 1; exponent = (1U << e) - 1U; return;}
        x.erase(0, 1);
        if (x[0] != '.') {significand = 1; exponent = (1U << e) - 1U; return;}
        x.erase(0, 1);
        std::string temp{""};
        int count{0};
        while (x[0] == '1' || x[0] == '0')
        {
            x[0] == '0' ? ++count : count = 0;
            temp += x[0];
            x.erase(0, 1);
        }
        if (count != 0)
            temp.erase (temp.find_last_not_of('0') + 1, std::string::npos );

        if (x[0] != 'b') {significand = 1; exponent = (1U << e) - 1U; return;}
        x.erase(0, 1);
        if (x[0] != '+' && x[0] != '-') {significand = 1; exponent = (1U << e) - 1U; return;}
        bool ex_sign;
        ex_sign = x[0] == '+' ? false : true;
        x.erase(0, 1);
        if (!std::isdigit(x[0])) {significand = 1; exponent = (1U << e) - 1U; return;}
        exponent = std::stoi(x);
        uint32_t ex_offset{(1U << (exponent_size - 1U))};
        ex_sign ? exponent = ex_offset - exponent : exponent = ex_offset + exponent;
        if (temp.length() >= 1) 
        {
            significand =  std::stoi(temp, 0, 2); //denotes conversion of a binary num
            significand <<= (significand_size - temp.length());
        }
        else
            significand = 0;
    }

    bool isNaN()       const {return exponent >= (1U << exponent_size) - 1UL && significand != 0UL;}
    bool isZero()      const {return exponent == 0U && significand == 0U;}
    bool isInfinity()  const {return exponent >= ((1U << exponent_size) - 1U) && significand == 0U && !isZero();}
    bool isPositive()  const {return !sign;}
    bool isNormal()    const {return exponent != 0 && !isNaN();} 
    bool isSubnormal() const {return exponent == 0 && significand > 0 && !isNaN();}

    std::string to_string()
    {
        std::string result{};
        if (isNaN())  return sign ? "-" : "+" + std::string("NAN");
        else if (isInfinity()) return sign ? "-" : "+" + std::string("INF");
        else if (isZero()) return sign ? "-" : "+" + std::string("0");
        isPositive() ? result += "+" : result += "-";
        isNormal() ? result += "1." : result += "0.";
        
        uint64_t sig_mask{1UL << (significand_size - 1UL)};
        while (sig_mask)
        {
            (significand & sig_mask) == 0 ? result += "0" : result += "1";
            sig_mask >>= 1UL;
        }
        while (true)
        {
            if (*(result.end() - 1) == '1' || *(result.end() - 1) == '.')
                break;
            result.erase(result.end() - 1);
        }
        
        int ex_offset{1 << (exponent_size - 1)};
        ex_offset >= 0 ? result += "b+" : result += "b-";
        return result + std::to_string(exponent - ex_offset);
    }

    void add( FP &other) //change back to const ....
    {
        
        
        //uint64_t sig_max{1UL << significand_size};
        //if ((significand + 2UL) >= sig_max) 
        //    return;

        bool F{sign};
        uint32_t oth_exp{other.exponent};
        uint64_t oth_sig{other.significand}; 
       // DEBUG(significand, oth_sig);
        if (isInfinity() || isNaN())
            return;
        else if (isZero())
        {
            exponent = oth_exp;
            significand = oth_sig;
            sign = other.sign;
            return;
        }
        else if (other.isZero()) {return;}
        else if ((significand == oth_sig) && (exponent == oth_exp))
        {
            if (sign != other.sign) {exponent = significand = 0; return;}
        }
        else
        {
            DEBUG(significand, oth_sig);
            significand |= (1UL << (significand_size + 1));  //concatenate hidden bit 
            oth_sig |= (1UL << (other.significand_size + 1)); 
            std::cout << to_string() << " " << other.to_string() << "\n";
            std::cout << "\n";
            uint32_t ex_diff;
            ex_diff = (exponent >= oth_exp ? exponent - oth_exp : oth_exp - exponent);
            while (--ex_diff)
            {
                if (exponent < oth_exp) {exponent <<= 1U; significand <<= 1UL;}
                else {oth_exp <<= 1U; oth_sig <<= 1UL;}
            }

            if (sign == other.sign)
                significand += oth_sig;
            else if (!sign && significand > oth_sig)
                significand -= oth_sig;
            else if (!sign && significand < oth_sig)
                significand -= (oth_sig - significand);
            else if (!other.sign && significand > oth_sig)
                significand -= oth_sig;
            else if (!other.sign && significand < oth_sig)
                significand -= (oth_sig - significand);
            //check msb position, if greater than sig size, shift right and increase exp
            int msb_pos{(int)(log2(significand))};
            while (true)
            {
                if (msb_pos > significand_size) {significand >>= 1UL; ++exponent;}
                else
                    break;
                --msb_pos;
            }
            significand >>= 1UL;
            //remove hidden bit, shift right and decrease exp??
        }








       // 
       // if (other.sign)
       // {
       //     //DEBUG(significand, oth_sig);
       //     if (significand >= oth_sig)
       //         significand -= oth_sig;
       //     else 
       //     {
       //         //DEBUG(significand, oth_sig); 
       //         significand -= (oth_sig - significand); //sign = true;
       //     }
       // }
       // else
       //     significand += oth_sig;
//
       // //gotta figure out how to change the exponent up or down
//     //   std::cout << "sig " << significand << "\n";
       // uint64_t min_sig{1UL << (significand_size - 2)};
       // min_sig += 1UL;
       // if (exponent < oth_exp)
       //     exponent = oth_exp;
       // else if (min_sig > significand)
       //     --exponent;
       // 
    }

    void negate() {sign ? sign = false : sign = true;}

    enum Order{BEFORE, EQUAL, AFTER, UNORDERED};

    Order  compare(const FP &other) 
    { 
        if (isNaN() || other.isNaN()) return UNORDERED;
        if (isInfinity() && other.isInfinity()) 
        {
            if (sign == other.sign) return EQUAL;
            else if (sign) return BEFORE;
            else return AFTER;
        }
        if (isZero() && other.isZero()) return EQUAL;

        if (sign != other.sign) 
        {
            if (sign) return BEFORE; 
            else return AFTER;   
        }
        if (exponent < other.exponent) 
        {
            if (sign) return AFTER; 
            else return BEFORE;         
        } 
        else if (exponent > other.exponent) 
        {
            if (sign) return BEFORE;    
            else return AFTER;     
        }
        if (significand < other.significand) 
        {
            if (sign) return AFTER; 
            else return BEFORE;         
        } 
        else if (significand > other.significand) 
        {
            if (sign) return BEFORE;    
            else return AFTER;      
        }
        return EQUAL;
    }
    

    // for testing
    uint64_t    getSignificand()    const { return significand; }
    uint32_t    getExponent()       const { return exponent; }
};