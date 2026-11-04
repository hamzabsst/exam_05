#include "bigint.hpp"

static std::string addStrings(const std::string& a, const std::string& b)
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
bigint::bigint(): str("0"){}

bigint::bigint(unsigned int num)
{
    std::stringstream ss;
    ss << num;
    str = ss.str();
}
bigint::bigint(const bigint &other):str(other.str){}

std::string bigint::getStr()const
{
    return(this->str);
}

bigint &bigint::operator=(const bigint &other)
{
    if(this != &other)
        str = other.str;
    return *this;
}
bigint bigint::operator+(const bigint &other)const
{
    bigint k(*this);
    k += other;
    return(k);
}
bigint &bigint::operator+=(const bigint &other)
{
    this->str = addStrings(this->str , other.str);
    return *this;
}

bigint &bigint::operator++()
{
    return *this += 1;
}
bigint bigint::operator++(int)
{
    bigint tmp = *this;
    ++(*this);
    return tmp;
}

static unsigned int stringToint(const std::string& str)
{
    std::stringstream ss(str);
    unsigned int res;
    ss >> res;
    return res;
}

bigint bigint::operator<<(const bigint &other)const
{
    unsigned int n = stringToint(other.str);
    if (this->str == "0")
        return *this;
    bigint k(*this);
    k.str.append(n, '0');
    return k;
}
bigint bigint::operator>>(const bigint &other)const
{   
    unsigned int n = stringToint(other.str);
    if(n >= str.size())
        return bigint(0);
    bigint k(*this);
    k.str.erase( k.str.size() - n , n );
    return k;
 
}
bigint &bigint::operator>>=(const bigint &other)
{ 
    *this  =  *this >> other;
    return *this;
}
bigint &bigint::operator<<=(const bigint &other)
{
   
    *this  =  *this << other;
    return *this;
}

bool bigint::operator==(const bigint &other)const
{
     return this->str == other.str;
}
bool bigint::operator!=(const bigint &other)const
{
    return (!(*this == other));
}
bool bigint::operator<(const bigint &other)const
{
    if (str.size() != other.str.size())
        return str.size() < other.str.size();
    return str < other.str;
}
bool bigint::operator>(const bigint &other)const
{
    return other < *this;
}
    
bool bigint::operator<=(const bigint& other) const
{
    return !(*this > other);
}

bool bigint::operator>=(const bigint& other) const
{
    return !(*this < other);
}
std::ostream& operator<<(std::ostream& os, const bigint& obj)
{
    os << obj.getStr();
    return os;
}
