#pragma once

#include <iostream>
#include <sstream>

std::string addString(const std::string& a, const std::string& b);
class bigint
{
    private:
        std::string str;
    public:
        bigint():str("0"){}
        bigint(unsigned int n)
        {
            std::stringstream ss;
            ss << n;
            str = ss.str();
        }
        bigint(const bigint& other): str(other.str) {}
        bigint& operator=(const bigint& other)
        {
            if (this != &other)
            {
                str = other.str;
            }
            return *this;
        }
        ~bigint(){}
        std::string getStr() const { return this->str; }
        bigint operator+(const bigint &other) const
        {
            bigint res(*this);
            res.str = addString(this->str, other.str);
            return res;
        }
        bigint& operator++()
        {
            this->str = addString(this->str, "1");
            return *this;
        }
        bigint operator++(int)
        {
            bigint temp(*this);
            this->str = addString(this->str, "1");
            return temp;
        }
        bigint& operator+=(const bigint& other)
        {
            this->str = addString(this->str, other.str);
            return *this;
        }

        bool operator<(const bigint& other) const
        {
            if (this->str.size() != other.str.size())
                return this->str.size() < other.str.size();
            return this->str < other.str;
        }
        bool operator==(const bigint& other) const
        {
            if (this->str.size() == other.str.size()
            && this->str == other.str)
            {
                return true;
            }
            return false;
        }
        bool operator>=(const bigint& other) const
        {
            if (*this < other)
                return false;
            return true;
        }
        bool operator>(const bigint& other) const
        {
            if (!(*this < other) && !(*this == other))
            {
                return true;
            }
            return false;
        }
        bool operator<=(const bigint& other) const
        {
            if (*this > other)
                return false;
            return true;
        }
        bool operator!=(const bigint& other) const
        {
            if (!(*this == other))
            {
                return true;
            }
            return false; 
        }
        bigint operator<<(const bigint& other)
        {
            if (this->str == "0")
                return *this;
            unsigned int n = 0;
            std::stringstream ss(other.str);
            ss >> n;    
            bigint temp(*this);
            temp.str.append(n, '0');
            return temp;
        }
        bigint operator>>(const bigint& other)
        {
            unsigned int n = 0;
            std::stringstream ss(other.str);
            ss >> n;    
            if (this->str == "0" || n >= this->str.size())
                return bigint(0);
            bigint temp(*this);
            temp.str.erase(temp.str.size() - n, n);
            return temp;
        }
        bigint& operator<<=(const bigint& other)
        {
            *this = *this << other;
            return *this;
        }

        bigint& operator>>=(const bigint& other)
        {
            *this = *this >> other;
            return *this;
        }
};
std::ostream& operator<<(std::ostream& os, const bigint& other);