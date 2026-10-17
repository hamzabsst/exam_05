#include "set.hpp"

set::set(const searchable_bag & s_bag): bag(s_bag);

bool set::has(int value)const
{
	return(bag.has(value));
}
void set::insert(int value)
{
	bag.insert(value);
}

void set::insert(int *data, int value)
{
	bag.insert(data , value);
}
void set::print() const
{
	bag.print();
}
void set::clear()
{
	bag.clear();
}

const set::searchable_bag get_bag()const
{
	return(this->bag);
}

set::~set{}