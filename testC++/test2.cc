#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

#include <cassert>
#include <iostream>

std::ostream &operator<<(std::ostream &out, const std::vector<vector<int>> &a)
{
    for (vector<int> const &aa : a)
    {
        for (int aaa : aa)
        {
            cout << aaa << " ";
        }
        cout << endl;
    }
    return cout << endl;
}

class Solution
{
  public:
    vector<vector<int>> subsets(vector<int> &nums)
    {
        for (int a = 0; a < nums.size(); ++a)
        {
            fun(nums, a);
        }
        return std::move(result);
    }
    void fun(vector<int> &result, int i)
    {
    }

    vector<vector<int>> result;
};

// #define assert(X) std::cout << X << std::endl

vector<vector<int>> fun(vector<int> &result, int n)
{
    vector<vector<int>> res; // 用于存储所有生成的排列
    if (n == 0)
    { // 如果排列长度为0，则返回一个只包含空向量的向量
        res.push_back({});
        return res;
    }
    for (int i = 0; i < n; ++i)
    {
        swap(result[i],
             result[n - 1]); // 将第i个元素放到最后一个位置上，即未处理的序列的最后一位
        auto sub_res = fun(result, n - 1); // 递归地生成剩余元素的所有排列
        for (auto &vec : sub_res)
        {                                 // 遍历子问题的所有解
            vec.push_back(result[n - 1]); // 将已经放在最后一位的元素添加到当前解中
            res.push_back(vec);           // 将当前解添加到结果中
        }
        swap(result[i], result[n - 1]); // 恢复原序列
    }
    return res; // 返回所有排列
}
int main()
{
    vector<int> res{0, 1, 2, 3};
    vector<vector<int>> &&a = fun(res, 4);
    sort(a.begin(), a.end());
    cout << a << endl;
}

// int main() { cout << minDistance("11", "1lilil"); }

// class Solution {
//  public:
//   int uniquePaths(int m, int n) {
//     d = m - 1;
//     r = n - 1;
//     fun(0, 0,0);
//     return sum;
//   }
//   void fun(int r_now, int d_now , int Sum_now) {
//     if (r_now == r && d_now == d) {
//       // ++sum;
//       Sum_now+=
//       return;
//     } else {
//       auto tmp_sum = _map.find(array<int, 2>{r_now, d_now});
//       if (tmp_sum == _map.end()) {
//         int _sum = sum;
//         if (r_now < r) {
//           fun(r_now + 1, d_now);
//         };
//         if (d_now < d) {
//           fun(r_now, d_now + 1);
//         };
//         _map[array<int, 2>{r_now, d_now}] = sum - _sum;
//       } else {
//         sum += tmp_sum->second;
//         return;
//       }
//     }
//   }
//   map<array<int, 2>, int> _map;
//   int r = 0;
//   int d = 0;
//   int sum = 0;
// };
// void printVector(vector<vector<int>>& vec) {
//   for (auto v : vec) {
//     cout << "[ ";
//     for (auto i : v) {
//       cout << i << " ";
//     }
//     cout << "]" << endl;
//   }
// }

// int main() {
//   Solution sol;
//   // int res1 = sol.uniquePaths(3, 2);
//   int res2 = sol.uniquePaths(19, 13);
//   // cout << "Test Case 1 Output: " << res1 << endl;
//   cout << "Test Case 2 Output: " << res2 << endl;
//   return 0;
// }