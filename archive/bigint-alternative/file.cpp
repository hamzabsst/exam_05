#include "bigint.hpp"

// ---------- HELPER ----------
// كنجمعو زوج strings ديال الأرقام بحال فالمدرسة (من اليمين لليسار)
static std::string addStrings(const std::string& a, const std::string& b)
{
    std::string result;
    int i = a.size() - 1;
    int j = b.size() - 1;
    int carry = 0;

    while (i >= 0 || j >= 0 || carry)
    {
        int digitA = (i >= 0) ? a[i--] - '0' : 0;
        int digitB = (j >= 0) ? b[j--] - '0' : 0;
        int sum = digitA + digitB + carry;
        result.insert(result.begin(), (sum % 10) + '0');
        carry = sum / 10;
    }
    return result;
}

// ---------- CONSTRUCTORS ----------
bigint::bigint() : str("0") {}

bigint::bigint(unsigned int num) : str(std::to_string(num)) {}

bigint::bigint(const bigint& other) : str(other.str) {}

bigint& bigint::operator=(const bigint& other)
{
    if (this != &other)
        str = other.str;
    return *this;
}

// ---------- GETTER ----------
const std::string& bigint::getStr() const
{
    return str;
}

// ---------- ARITHMETIC ----------
bigint bigint::operator+(const bigint& other) const
{
    bigint result;
    result.str = addStrings(this->str, other.str);
    return result;
}

bigint& bigint::operator+=(const bigint& other)
{
    str = addStrings(str, other.str);
    return *this;
}

// ---------- INCREMENT ----------
bigint& bigint::operator++()
{
    *this += bigint(1);
    return *this;
}

bigint bigint::operator++(int)
{
    bigint tmp = *this;
    ++(*this);
    return tmp;
}

// ---------- SHIFT (DECIMAL) ----------
bigint bigint::operator<<(unsigned int n) const
{
    if (str == "0")
        return *this;
    bigint result = *this;
    result.str.append(n, '0');
    return result;
}

bigint bigint::operator>>(unsigned int n) const
{
    if (n >= str.size())
        return bigint(0);
    bigint result = *this;
    result.str.erase(result.str.size() - n, n);
    return result;
}

bigint& bigint::operator<<=(unsigned int n)
{
    *this = *this << n;
    return *this;
}

bigint& bigint::operator>>=(unsigned int n)
{
    *this = *this >> n;
    return *this;
}

// ---------- COMPARISON ----------
bool bigint::operator==(const bigint& other) const
{
    return str == other.str;
}

bool bigint::operator!=(const bigint& other) const
{
    return !(*this == other);
}

bool bigint::operator<(const bigint& other) const
{
    // إيلا الأطوال مختالفين، الأطول هو الأكبر
    if (str.size() != other.str.size())
        return str.size() < other.str.size();
    // نفس الطول → مقارنة lexicographique كافية حيت الأرقام '0'-'9' مرتبين
    return str < other.str;
}

bool bigint::operator>(const bigint& other) const
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

// ---------- OUTPUT ----------
std::ostream& operator<<(std::ostream& os, const bigint& obj)
{
    os << obj.getStr();
    return os;
}