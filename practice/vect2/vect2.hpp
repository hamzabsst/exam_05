#ifndef VECT2_HPP
#define VECT2_HPP
#include <iostream>
class vect2 
{
    private:
        int x;
        int y;
    public:
        vect2();
        vect2(int num1 , int num2);
        vect2(const vect2 &other);
        vect2 &operator=(const vect2 &other);

        int operator[]( int index)const;
        int &operator[](int index);
        
        vect2 operator-()const;

        vect2 operator*(int num)const;
        vect2 &operator*=(int num);

        vect2 &operator++();
        vect2 operator++(int);
        vect2 &operator--();
        vect2 operator--(int);


        vect2 &operator+=(const vect2 &other);
        vect2 &operator-=(const vect2 &other);
        vect2 &operator*=(const vect2 &other);

        
        vect2 operator+(const vect2 &other)const;
        vect2 operator-(const vect2 &other)const;
        vect2 operator*(const vect2 &other)const;


        bool operator==(const vect2 &other)const;
        bool  operator!=(const vect2 &other)const;
        ~vect2();

};
vect2 operator*(int num, const vect2 &obj);

std::ostream &operator<<(std::ostream &out , const vect2 &ob);

#endif