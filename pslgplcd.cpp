#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <cmath>

/**
todo regularization, but it will bring even more complexity, first finish with a regular graph
 */

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

    vector<vector<int>> rows(sz);
    vector<vector<int>> cols(sz);

    cout << "correct edges..." << endl;

    map<pair<int,int>,int> e2weight;

    // check that we are doing the correct thing
    // check what is inside the first element's in sorted order lis of outcoming edges

    // now when we need to process an edge, we convert u and v to m[u] and m[v] respectively
    for (int i = 0; i < edges_.size(); ++i)
    {
      cout << "from " << m[i] << " (old " << i << ") to [";
      for (int v = 0; v < edges_[i].size(); ++v)
      {
        if (points_[m[i]].second > points_[m[edges_[i][v]]].second
            || (points_[m[i]].second == points_[m[edges_[i][v]]].second && m[i] < m[edges_[i][v]]))
        {
          cout << m[v] << " (old " << v << ")" << " ";
          // respect that edge
          rows[m[i]].push_back(m[edges_[i][v]]);
          cols[m[edges_[i][v]]].push_back(m[i]);
          e2weight[make_pair(m[i], m[edges_[i][v]])] = 1;
        }
      }
      cout << "]" << endl;
    }

    // this is generally dangerous to do, but okay here
    auto& pref = points_;

    for (int i = 0; i < rows.size(); ++i)
    {
      sort(rows[i].begin(), rows[i].end(), [i, pref](int n, int m) {
          double atan2n = atan2(pref[n].first - pref[i].first, pref[n].second - pref[i].second),
              atan2m = atan2(pref[m].first - pref[i].first, pref[m].second - pref[i].second);
          return atan2n < atan2m;
        });
    }

    for (int i = 0; i < cols.size(); ++i)
    {
      sort(cols[i].begin(), cols[i].end(), [i, pref](int n, int m) {
          double atan2n = atan2(pref[i].first - pref[n].first, pref[i].second - pref[n].second),
              atan2m = atan2(pref[i].first - pref[m].first, pref[i].second - pref[m].second);
          return atan2n < atan2m;
        });
    }

    /**
     * in the original article, the following structure is given
     * for the edge and point vertex format:

     * vertex node format:
     *   values: # I
     *   ptrs: pred, succ, col, row

     * in this program, pred and succ are really indices in the outer vector of nodes;
     * col and row are the indices of the inner array of edges, each vector has
     * 2 associated vectors of (row and cilumn lists respectively);
     * I is the cardinality of incoming edges, really is the size of [row/column] list
     * # is point index in the sorted array of points

     * edge node foramt:
     *   value: weight
     *   ptrs: up, down, col next, row next

     * ptrs up and down are really the idnex and associated values of vector at that index
     * next ptrs are indices too, they are need fortraversing only;

     * to sum up, there are 2 vector for each point - edge list and row list,
     * inside, indices to the sorted structure are stored, filtered thru m[.]
     */
    // phase #1 ended - preprocessing done


    /** this piece of code has not been tested yet*/
    for (int i = sz - 2; i >= 0; --i)
    {
      cout << "debug statement; iter #" << i << endl;

      int w_out = 0; // sum of weights in row list of v_{i} -- same as
      // sum of weights of all edges coming down from v_{i}
      for (int j = 0; j < rows[i].size(); ++j)
      {
        w_out += e2weight[make_pair(i,rows[i][j])];
      }

      cout << "debug statement; w_out" << w_out << endl;

      if (w_out > cols[i].size())
      {
        cout << "debug statement; w_out > cols[i].size()" << endl;
        cout << "debug statement; cols[.].size() = " << cols[i].size() << endl;
        cout << "before processing the weight of edge i -> cols[.][0] = " << e2weight[make_pair(i, cols[i][0])];
        e2weight[make_pair(i, cols[i][0])] = w_out - cols[i].size() + 1;
        cout << "after processing the weight of edge i -> cols[.][0] = " << e2weight[make_pair(i, cols[i][0])];
      }
    }
  }

 private:
  vector<pair<double,double>> points_;
  vector<vector<int>> edges_;
  struct EdgeNodeFormat
  {
    int up;
    int down;
    int col_next;
    int row_next;
  };
  struct VertexNodeFormat
  {
    int pred;
    int succ;
    int col;
    int row;
  };
};
} // ns PSLG_Point_Location

int main()
{

  // this is the example from the article needs to be modified as it it not regular!
  PSLG_Point_Location::Solution s({{1, 1}, {4,2}, {3,4}, {3,3}},
                                  {{2, 3, 1}, {0, 2, 3}, {0, 1, 3}, {0, 1,2}});
  /*
  PSLG_Point_Location::Solution s({{0.0, 3.0}, {1.5, 2}, {3.0, 1.0}, {3.0, 4.0}},
                                  {});
  */

  /*
  PSLG_Point_Location::Solution s({{0.0, 1.0}, {1.5, 2}, {3.0, 3.0}, {3.0, -1.0}}, // nodes
                                  {{1, 2, 3}, {0, 3}, {0, 3}, {0, 1, 2}}); // edges
  */
  return 0;
}
