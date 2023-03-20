// #include <fstream>
// #include <iostream>
// #include <string>
// #include <vector>
// #include <list>
// #include <iterator>
// #include <algorithm>

// using namespace std;

// int main(int argc, char **argv)
// {
//     // istream_iterator
//     ifstream in(*(argv+1));
//     ofstream out("a");
//     ostream_iterator<string> outs(out);
//     istream_iterator<string> int_iter(in); // 从cin读取int
//     istream_iterator<string> eofs;           // istream尾后迭代器
//     vector<string> vec(int_iter,eofs);
//     list<string> eof{""}; 
//     list<string> l{"wang","yi","bo","hhhh"};
//     // auto it = eof.begin();
//     copy(l.begin(),l.end(),it);
//     // front_inserter ll(l);

//     // for(auto i :vec)
//     // {
//     //     cout << i <<endl;
//     // }
//     copy(vec.begin(), vec.end(),outs);
//     copy(eof.begin(), eof.end(),outs);
//     for(auto i:eof)
//     {
//         cout<<i<<endl;
//     }
// }