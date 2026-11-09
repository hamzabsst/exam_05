#include "bigint.hpp"

std::string addString(const std::string& a, const std::string& b)
{
    std::string res;

    int i = a.size()-1;
    int j = b.size()-1;
    int carry = 0;
    while(i >= 0 || j >= 0 || carry)
    {
        int digitA = 0;
        int digitB = 0;

        if(i >= 0)
        {
            digitA = a[i]- '0';
            i--;
        }
        if(j >= 0)
        {
            digitB = b[j] - '0';
            j--;
        }
        int sum = digitA + digitB + carry;
        res.insert(res.begin(), (sum % 10 ) + '0');
        carry = sum/10;
    }
    return res;
}

std::ostream& operator<<(std::ostream& os, const bigint& obj)
{
    os << obj.getStr();
    return os;
}
