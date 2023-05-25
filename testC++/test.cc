#include <iostream>
#include <vector>
#include <list>
#include <array>
#include "test2.cc"
#include <queue>
#include <iterator>
int * function (int arg,int a =10)
{
    int * wang = new int ;
    *wang = arg+a;
    return wang;
}
int main(int argc, char **argv)
{   
    int * wang = function(10,100);
    std::cout <<*wang <<std::endl;
    delete wang;
    // std::vector<int> v;
    // std::list<int> List{1, 2, 3, 4, 5, 6};
    // List.push_back(1);
    // for (auto &a : List)
    // {
    //     std::cout << a << " " << std::endl;
    // }

    // auto it = List.begin();
	// while(1)
	// {
	// 	if(*it != 3)
	// 		break;
	// }
    // List.insert(it, 10);
    // for (auto &a : List)
    // {
    //     std::cout << a << " " << std::endl;
    // }

    // List.sort();
    // for (auto &a : List)
    // {
    //     std::cout << a << " " << std::endl;
    // }
}
