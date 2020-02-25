#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>
#include <functional>

namespace
{
using namespace std;
// test # 1 sort the edges by their slope, take qudrants into account
void atan_sort_test()
{
  // the set of points we will work with
  vector<pair<double,double>> points_{{1, 2}, {2, 1}, {1, 1}, {2, 2}};
  cout << "points to be tested" << endl;
  for (auto i : points_)
  {
    cout << i.first << " " << i.second << endl;
  }
  cout << "---" << endl;

  /**
   * according to the application, there can only be
   * 1) edges coming from above to a given point,
   * 2) or there can be edges coming down from the point;
   * in other words, the edges are oriented towards -inf on y-axis;
   * "row list" is used to store outcoming egdes, let's test that first
   */

  vector<int> row_list{0, 1, 2, 3};
  pair<double,double> start_point{0, 0}; // it is below the points_ ...

  start_point = make_pair(1.5, 3);

  sort(row_list.begin(), row_list.end(), [start_point, &points_](int n, int m) {
      double atan2n = atan2(points_[n].first - start_point.first, points_[n].second - start_point.second),
          atan2m = atan2(points_[m].first - start_point.first, points_[m].second - start_point.second);
      return atan2n < atan2m;
    });


  cout << "the sorted sequence" << endl;
  for (auto i : row_list)
  {
    cout << i << " ";
  }
  cout << endl;
}


vector<function<void()>> tests = {atan_sort_test};

} // anonymous ns

int main()
{
  for (int i = 0; i < tests.size(); ++i)
  {
    cout << "running test # " << i << endl;
    tests[i]();
    cout << "test #" << i << " has finished" << endl;
  }
  return 0;
}
