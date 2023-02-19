#include "Sales_data.h"
Sales_data::Sales_data(std::istream &is)
{
	read(is,*this);
}

// 用于将一个Sales_data加到另一个对象上
Sales_data& Sales_data::combine(const Sales_data &item)
{
	this->units_sold += item.units_sold;
	this->revenue += item.revenue;
	return *this;
}

// 执行两个Sales_data的对象的加法
Sales_data add(const Sales_data &lhs, const Sales_data &rhs )
{
	Sales_data sum = lhs;
	sum.combine(rhs);
	return sum;
}

// 将数据从istream读入到Sales_data对象中
std::istream &read(std::istream &is, Sales_data &item)
{
	double price = 0;
	is >> item.bookNo >> item.units_sold >> price;
	item.revenue = price*item.units_sold;
	return is;
}

// 将Sales_data对象的值输入到ostream中
std::ostream &print(std::ostream &os, const Sales_data &item)
{
	os << item.isbn() << " " << item.units_sold << " " << item.revenue << " " << item.avg_price()<<std::endl;
	return os;
}
