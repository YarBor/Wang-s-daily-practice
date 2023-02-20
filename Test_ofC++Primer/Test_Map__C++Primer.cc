#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <algorithm>

using namespace std;

// int main(int argc, char** argv)
// {
// map<int,std::string> book{{1001,"world"},{1002,"2023"}};
// book.insert(make_pair(10003,"wang"));
// book.insert ({1999,string("worlds")});
// auto i =book.begin();
// while(i != book.end())
// {
// cout<<i->first<<" "<<(i++)->second<<endl;
// }
// book.find(1001)->second = "worlds";
// cout << book.find(1001)->second << endl;
// }

int main(int argc, char **argv)
{
set<string> c{"wang", "worlds"};
    vector<string> v({"jjjjjj", "hhhhh"});
    // c.insert(v.begin(),v.end());
    // copy(v.begin(), v.end(), inserter(c,c.begin()));
    // copy(v.begin(), v.end(), back_inserter(c));
    copy(c.begin(), c.end(), inserter(v, v.begin()++));
    // copy(c.begin(), c.end(), back_inserter(v));

    for (auto i : c)
    {
        cout << i << endl;
    }
    cout << endl;
    for (auto i : v)
    {
        cout << i << endl;
    }
}