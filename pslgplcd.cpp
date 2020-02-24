#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_map>

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
  Solution(vector<pair<double,double>> points, const vector<vector<int>>& edges)
  {
    cout << "PSLG Point Location with Chain Decomposition Solution object has been invoked" << endl;
    points_ = points;
    edges_ = edges;
    int sz = points_.size();
    vector<int> index_map(sz);
    for (int i = 0; i < sz; ++i)
    {
      index_map[i] = i;
    }

    unordered_map<int,int> m;

    // we will keep both the sorted array and the index map for future
    // reference (we will need to convert edge indices)

    for (int i = 0; i < sz; ++i)
    {
      // find the ith smallest elements from the array
      int max_element_index = i;
      for (int j = i + 1; j < sz; ++j)
      {
        if (points[j].second > points[max_element_index].second)
        {
          max_element_index = j;
        }
      }
      m[max_element_index] = i;
      iter_swap(points.begin() + i, points.begin() + max_element_index);
    }


    cout << "the new array be like: " << endl;
    for (auto i : points)
    {
      cout << i.first << " " << i.second << " | ";
    }
    cout << endl;


    cout << "the conversion looks like: " << endl;
    for (int i = 0; i < sz; ++i)
    {
      cout << m[i] << " ";
    }
    cout << endl;

    // phase #1 ended - preprocessing done
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
