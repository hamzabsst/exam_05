#include "vect2.hpp"

vect2::vect2(): x(0), y(0){}
vect2::vect2(int num1 , int num2):x(num1), y(num2){}
vect2::vect2(const vect2 &other): x(other.x), y(other.y){}

vect2 &vect2::operator=(const vect2 &other)
{
    if(this != &other)
    {
        x = other.x;
        y = other.y;
    }
    return *this;
}

int vect2::operator[]( int index)const
{
    if(index == 0)
        return x;
    return y;
}
int &vect2::operator[](int index)
{
    if(index == 0)
        return x;
    return y;  
}
vect2 vect2::operator-()const
{
    vect2 temp;
    temp.x = -x;
    temp.y = -y;
    return temp;
}

vect2 vect2::operator*(int num)const
{
    vect2 temp;
    temp.x = x * num;
    temp.y = y * num;
    return temp;
}
vect2 &vect2::operator*=(int num)
{
    this->x = this->x * num;
    this->y = this->y * num;
    return *this;
}
vect2 &vect2::operator++()
{
    this->x++;
    this->y++;
    return *this;
}
vect2 vect2::operator++(int)
{
    vect2 temp = *this;
    this->x++;
    this->y++;
    return temp;
}
vect2 &vect2::operator--()
{
    this->x--;
    this->y--;
    return *this;
}
vect2 vect2::operator--(int)
{
    vect2 temp = *this;
    this->x--;
    this->y--;
    return temp;
}

vect2 &vect2::operator+=(const vect2 &other)
{
    this->x = this->x + other.x;
    this->y = this->y + other.y;
    return *this;
}
vect2 &vect2::operator-=(const vect2 &other)
{
    this->x = this->x - other.x;
    this->y = this->y - other.y;
    return *this;   
}
vect2 &vect2::operator*=(const vect2 &other)
{
    this->x = this->x * other.x;
    this->y = this->y * other.y;
    return *this;
}

vect2 vect2::operator+(const vect2 &other)const
{
    vect2 temp;
    temp.x = x + other.x;
    temp.y = y + other.y;
    return temp;
}
vect2 vect2::operator-(const vect2 &other)const
{
    vect2 temp;
    temp.x = x - other.x;
    temp.y = y - other.y;
    return temp;
}
vect2 vect2::operator*(const vect2 &other)const
{
    vect2 temp;
    temp.x = x * other.x;
    temp.y = y * other.y;
    return temp;
}
bool vect2::operator==(const vect2 &other)const
{
    return(x == other.x && y == other.y);
}
bool  vect2::operator!=(const vect2 &other)const
{
    return !(*this  == other);
}
vect2::~vect2(){}
vect2 operator*(int num, const vect2 &obj)
{
    vect2 temp(obj);
    temp *= num;
    return temp;
}
std::ostream &operator<<(std::ostream &out , const vect2 &v)
{
    out << "{" << v[0] << ", " << v[1] << "}";

    return out;
}