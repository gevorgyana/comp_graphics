#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_map>

namespace PSLG_Point_Location {
using namespace std;

class Solution
{
 public:
  Solution(const vector<pair<double,double>>& points, const vector<vector<int>>& edges)
  {
    cout << "PSLG Point Location with Chain Decomposition Solution object has been invoked" << endl;
    cout << "Initial data..." << endl;
    cout << "--- point # | coords (x) | coords(y) ---" << endl;
    for (int i = 0; i < points.size(); ++i)
    {
      cout << i << " ";
      cout << points[i].first << " " << points[i].second << endl;
    }
    cout << "--- edges : index -> [indices] ---" << endl;
    int cur_node = 0;
    for (auto i : edges)
    {
      cout << cur_node++ << " ";
      for (auto j : i)
      {
        cout << " :" << j << " ";
      }
      cout << endl;
    }

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
        if (points_[j].second > points_[max_element_index].second)
        {
          max_element_index = j;
        }
      }

      m[i] = max_element_index;
      iter_swap(points_.begin() + i, points_.begin() + max_element_index);
    }

    cout << "the new array be like: " << endl;
    for (auto i : points_)
    {
      cout << i.first << " " << i.second << " | ";
    }
    cout << endl;


    for (int i = 0; i < sz; ++i)
    {
      if (m[i] != i)
      {
        m[m[i]] = i;
      }
    }

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
  PSLG_Point_Location::Solution s({{0.0, 1.0}, {1.5, 2}, {3.0, 3.0}, {3.0, -1.0}}, // nodes
                                  {{1, 2, 3}, {0, 3}, {0, 3}, {0, 1, 2}}); // edges
  return 0;
}
