#ifndef SALES_DATA_H
#define SALES_DATA_H
#include <string>
#include <iostream>
class Sales_data{
	// 为Sales_data的非成员函数所做的友元声明
	friend Sales_data add(const Sales_data &lhs, const Sales_data &rhs );
	friend std::istream &read(std::istream &is, Sales_data &item);
	friend std::ostream &print(std::ostream &os, const Sales_data &item);
	// 类User可以访问此类的私有部分
	//friend class User; 
	public:

		Sales_data() = default;
		Sales_data(const std::string &s):bookNo(s){}
		Sales_data(const std::string &s,unsigned n,double p):bookNo(s),units_sold(n),revenue(p*n){}
		Sales_data(std::istream &is);
		// 返回对像的ISBN编号
		std::string isbn() const {return this->bookNo;}
		// 用于将一个Sales_data加到另一个对象上
		Sales_data &combine(const Sales_data &item);
	private:
		// 定义在类内部的函数是隐式的内联
		double avg_price() const{return units_sold ? revenue/units_sold : 0;}
		// 书号
		std::string bookNo;
    	// 卖出去的册数
		unsigned units_sold = 0;
		// 总销售额
		double revenue = 0.0;
};
// 非成员接口函数：友元函数
// 执行两个Sales_data的对象的加法
Sales_data add(const Sales_data &lhs, const Sales_data &rhs );
// 将数据从istream读入到Sales_data对象中
std::istream &read(std::istream &is, Sales_data &item);//将对象的内容打印到给定的流中
// 将Sales_data对象的值输入到ostream中
std::ostream &print(std::ostream &os, const Sales_data &item);

#endif 
