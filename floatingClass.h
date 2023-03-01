#include <cstdint>
#include <cmath>
#include <bitset>
#include <string>
#include <cstdlib>
#include <iostream>

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

    void add(const FP &other) // this might be easy if I follow this current structure
    {
        if (isInfinity() || isNaN())
            return;
        uint64_t sig_max{1UL << significand_size};
        if ((significand + 2UL) >= sig_max) 
            return;

        bool F{sign};
        uint32_t oth_exp{other.exponent};
        uint64_t oth_sig{other.significand}; 
        if ((significand == oth_sig) && (exponent == oth_exp))
            if (sign != other.sign) {exponent = significand = 0; return;}
        if (significand < oth_sig)
            if (exponent <= oth_exp)
            {
                uint64_t temp1, temp2;
                F = other.sign;
                temp1 = exponent; temp2 = significand;
                exponent = oth_exp; significand = oth_sig;
                oth_exp = temp1; oth_sig = temp2;
            }
        std::cout << "here\n";
        for (int i{0}; i < exponent - oth_exp; ++i)
            oth_sig >>= 1UL;
        uint32_t E{exponent};
        uint64_t S{F == other.sign ? significand + oth_sig : significand - oth_sig};
        std::cout << "Sig " << significand << "\n";
        if (S == 0UL) {exponent = significand = 0; std::cout << "its zero for some reason\n"; return;}
        
        while (true)
        {
            std::cout << "here\n";
            if (((int)log2(S) + 1) > significand_size) {S >>= 1UL; ++E;} //if num of bits is larger than sig size
            else 
                break;
        }
        uint64_t sig_mask{1UL << significand_size};
        while ((S & sig_mask) == 0)
        {
            std::cout << "here\n";
            S <<= 1UL; --E;
        }
        sig_mask >>= 1UL;
        uint32_t ex_offset{(1U << (exponent_size - 1U))};
        sign = F; exponent = E + ex_offset; significand = S & sig_mask;
    }

    void negate() {sign ? sign = false : sign = true;}

    enum Order{BEFORE, EQUAL, AFTER, UNORDERED};

    Order compare(const FP &other)
    {
        if (isZero())
        {
            if (other.isZero())
                return Order::EQUAL;
            return other.sign == false ? Order::BEFORE : Order::AFTER;
        }
        if (isNaN() || other.isNaN())
            return (isNaN() && other.isNaN()) ? Order::EQUAL : Order::UNORDERED;
        
        if (isInfinity() || other.isInfinity())
        {
            if (isInfinity() && other.isInfinity())
                return Order::EQUAL;
            else if (isInfinity())
                return !sign ? Order::AFTER : Order::BEFORE;
            else
                return !other.sign ? Order::BEFORE : Order::AFTER;
        }
        //test "normal" nums
        if (exponent == other.exponent && significand == other.significand)
            return Order::EQUAL;
        else if (exponent == other.exponent)
            return significand > other.significand ? Order::AFTER : Order::BEFORE;
        else  
            return exponent > other.exponent ? Order::AFTER : Order::BEFORE;
    }

    // for testing
    uint64_t    getSignificand()    const { return significand; }
    uint32_t    getExponent()       const { return exponent; }
};