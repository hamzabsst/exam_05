
#ifndef BIGINT_HPP
#define BIGINT_HPP
#include <iostream>
#include <sstream>

class bigint{

    private:
        std::string str;
    public:
        bigint();
        bigint(unsigned int num);
        bigint(const bigint &other);
        bigint &operator=(const bigint &other);

        std::string getstr()const;

        bigint operator+(unsigned int num)const;
        bigint &operator+=(const bignt &other);


        bigint &operator++();
        bigint operator++(int);


        bigint operator<<(unsigned int num)const;
        bigint operator>>(unsigned int num)const;
        bigint &operator<<=(unsigned int num);
        bigint &operator>>=(unsigned int num);


        bigint operator<<(const bignt &other)const;
        bigint operator>>(const bignt &other)const;
        bigint &operator<<=(const bignt &other);
        bigint &operator>>= (const bignt &other);
        
        bool operator==(const bignt &other)const;
        bool operator!=(const bignt &other)const;
        bool operator<(const bignt &other)const;
        bool operator>(const bignt &other)const;
        bool operator<=(const bignt &other)const;
        bool operator>=(const bignt &other)const;
        
        ~bigint();
};

std::ostream &operator<<(std::ostream &out , const bigint &obj);




#endif