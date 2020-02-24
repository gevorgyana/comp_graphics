#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

/**
 * Google C++ style guide compliant

 * This algorithm has been constructed after reading B10-699,
 * all credits to the authors!
 */

namespace PSLG_Point_Location {
using namespace std;

class Solution
{
 public:
  Solution(const vector<pair<double,double>>& points, const vector<vector<int>>& edges)
  {
    points_ = points;
    edges_ = edges;

    // sort with respect to y axis
    sort(points_.begin(), points_.end(), [](pair<double,double> left, pair<double,double> right)
                                      {return left.second > right.second;});

    int sz = points_.size();

    for (auto p : points_)
    {
      cout << p.first << " " << p.second << " ";
    }
  }

 private:

  struct RowList {
    vector<int> vertex_indices;
  };

  struct EdgeList {
    vector<int> vertex_indices;
  };

  vector<vector<int>> edges_;
  vector<pair<double,double>> points_;
};

} // ns PSLG_Point_Location

int main()
{
  PSLG_Point_Location::Solution s({{-1.0, 2.0}, {0.0, 1.0}}, {});
  return 0;
}
