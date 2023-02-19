#include "Sales_data.h"

using namespace std;
int main()
{
	string name = "BOOKONE";
	Sales_data book(name,18,2.6);
	Sales_data bookOne(cin);
	print(cout,book);
	print(cout,bookOne);
}
