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
  int uniquePaths(int m, int n) {
    d = m;
    r = n;
    fun(1, 1);
    return sum;
  }
  void fun(int r_now, int d_now) {
    if (r_now == r && d_now == d) {
      ++sum;
      return;
    } else {
      auto tmp_sum = _map.find(array<int, 2>{r_now, d_now});
      if (tmp_sum == _map.end()) {
        int _sum = sum;
        if (r_now < r) {
          fun(r_now + 1, d_now);
        };
        if (d_now < d) {
          fun(r_now, d_now + 1);
        };
        _map[array<int, 2>{r_now, d_now}] = sum - _sum;
      } else {
        sum += tmp_sum->second;
        return;
      }
    }
  }
  map<array<int, 2>, int> _map;
  int r = 0;
  int d = 0;
  int sum = 0;
};
void printVector(vector<vector<int>>& vec) {
  for (auto v : vec) {
    cout << "[ ";
    for (auto i : v) {
      cout << i << " ";
    }
    cout << "]" << endl;
  }
}

int main() {
  Solution sol;
  // int res1 = sol.uniquePaths(3, 2);
  int res2 = sol.uniquePaths(19, 13);
  // cout << "Test Case 1 Output: " << res1 << endl;
  cout << "Test Case 2 Output: " << res2 << endl;
  return 0;
}