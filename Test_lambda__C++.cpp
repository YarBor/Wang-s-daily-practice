#include <list>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
// void abssort(float *x, unsigned n)
// {
//     std::sort(x, x + n,
//               [](float a, float b)-> bool
//               {
//                   return (std::abs(a) < std::abs(b));
//               });
// }
// //
// int main(int argc, char **argv)
// {
//     float x[]{1.1, 1.4, 1.2, 23.5, 123, 3.234, 12.2345};
//     abssort(x, 7);
//     for (auto i : x)
//     {
//         std::cout << i << std::endl;
//     }
// }
// // int main(void)
// // {
// // using namespace std;
// // int num = 100;
// // read only
// // float wang = 12.2;
// // auto fun1 = [num, wang]()
// // { cout << num << wang << endl; };
// // fun1();
// //
// // read and write
// // auto fun2 = [&num](std::string str)
// // {
// // num = 200;
// // cout << num << endl;
// // cout << str << endl;
// // };
// // fun2(string("hello"));
// // num = 200
// // cout << num << endl;
// // return 0;
// // }
int main(int argc, char **argv)
{
    using namespace std;
    list<string> l{string("hello"), string("asdfasdf"), string("qwer")};
    l.push_back("wanggttg");
    l.push_back("wangfr");
    l.push_back("wangde");
    l.push_back("wangws");
    // auto fun1 = bind(
    //     [&](string str)
    //     {if (str.length()<=5) str="wang"; cout << str << endl; },
    //     placeholders::_1);
    // for_each(l.begin(), l.end(), fun1);
    for (auto const i : l)
    {
        cout << i << endl;
    }
    l.sort([](string &s1, string &s2) -> bool
         { return s1 > s2; });
    // sort(l.begin(), l.end());
    // cout << (l[1]>l[2]) << std::endl;
    for (auto const i : l)
    {
        cout << i << endl;
    }
}