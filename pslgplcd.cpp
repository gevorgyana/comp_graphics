#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>

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


    // cannot use unordered map with pair - try to compile and see why
    map<pair<double,double>,int> p;
    for (int i = 0; i < sz; ++i)
    {
      p[points_[i]] = i;
    }

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

      iter_swap(points_.begin() + i, points_.begin() + max_element_index);
    }

    cout << "the new array: " << endl;
    for (auto i : points_)
    {
      cout << i.first << " " << i.second << " | ";
    }
    cout << endl;


    vector<int> m(sz);
    for (int i = 0; i < sz; ++i)
    {
      m[p[points_[i]]] = i;
    }

    cout << "the conversion looks like: " << endl;
    for (int i = 0; i < sz; ++i)
    {
      cout << m[i] << " ";
    }
    cout << endl;


    // now when we need to process and edge, we convert u and v to m[u] and m[v] respectively


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
  PSLG_Point_Location::Solution s({{0.0, 3.0}, {1.5, 2}, {3.0, 1.0}, {3.0, 4.0}}, // nodes
                                  {}); // edges
  /*
  PSLG_Point_Location::Solution s({{0.0, 1.0}, {1.5, 2}, {3.0, 3.0}, {3.0, -1.0}}, // nodes
                                  {{1, 2, 3}, {0, 3}, {0, 3}, {0, 1, 2}}); // edges
  */
  return 0;
}
