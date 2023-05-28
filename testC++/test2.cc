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

class Solution {
 public:
  int minPathSum(vector<vector<int>>& grid) {
    int m = grid.size();
    int n = grid[0].size();
    int dp[m][n];
    dp[0][0] = grid[0][0];
    for (int i = 1; i < n; i++) {
      dp[0][i] = dp[0][i - 1] + grid[0][i];
    }
    for (int i = 1; i < m; i++) {
      dp[i][0] = dp[i - 1][0] + grid[i][0];
    }
    for (int i = 1; i < m; i++) {
      for (int j = 1; j < n; j++) {
        dp[i][j] = min(dp[i - 1][j] + grid[i][j], dp[i][j - 1] + grid[i][j]);
      }
    }
    return dp[m - 1][n - 1];
  }
};

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