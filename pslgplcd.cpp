#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <cmath>
#include <numeric>

/**
todo regularization, but it will bring even more complexity, first finish with a regular graph
 */

namespace PSLG_Point_Location {
using namespace std;


// not tested
int lca(int i, int j, int lvl)
{
  // we know i is the minimum element
  // try to reach one level more, and see what happens
  // will we reach what we want to in 1 elvelation?

  // we need to know the direction

  int sign = 1;
  int rem = i / pow(2, lvl);
  int k = 1;
  while(k != rem)
  {
    k += 2;
    sign *= -1;
  }

  // move one step up
  int move_diff = pow(2, lvl);

  int i_ = sign * move_diff;

  // if j is inside
  int i_range_r = i_;
  int i_range_l = i_;
  int l = 0;
  while (l != lvl + 2) // or 1 - check
  {
    i_range_r += pow(2, l);
    i_range_l -= pow(2, l);
    ++l;
  }

  if (i_range_l <= j && i_range_r >= j)
    return i_;
  return lca(i_, j, lvl + 1);
}

//not tested
int LCA(int i, int j)
{
  // find the level of both elements in the tree
  int level_i = 0;
  int two_p_level_i = 1;
  while (i % two_p_level_i == i)
  {
    two_p_level_i *= 2;
    ++level_i;
  }

  int level_j = 0;
  int two_p_level_j = 1;
  while (j % two_p_level_j == 0)
  {
    two_p_level_j *= 2;
    ++level_j;
  }

  if (level_i < level_j)
    return lca(i, j, level_i);
  return lca(j, i, level_j);
}

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

    // okay, it has been tested up to this point

    // this is generally dangerous to do, but okay here
    auto& pref = points_;

    for (int i = 0; i < rows.size(); ++i)
    {
      sort(rows[i].begin(), rows[i].end(), [i, pref](int n, int m) {
          double atan2n = atan2(pref[n].first - pref[i].first, pref[n].second - pref[i].second),
              atan2m = atan2(pref[m].first - pref[i].first, pref[m].second - pref[i].second);

          //experimental
          //return atan2n < atan2m;
          return pref[n].first < pref[m].first;
        });
    }

    for (int i = 0; i < cols.size(); ++i)
    {
      sort(cols[i].begin(), cols[i].end(), [i, pref](int n, int m) {
          double atan2n = atan2(pref[i].first - pref[n].first, pref[i].second - pref[n].second),
              atan2m = atan2(pref[i].first - pref[m].first, pref[i].second - pref[m].second);

          // experimental
          // return atan2n > atan2m;
          return pref[n].first > pref[m].first;
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
     * I is the cardinality of incoming edges, really is the size of column list
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

    // debug statement
    for (int i = 0; i < cols.size(); ++i)
    {
      cout << i << " ## " << endl;
      for (int j = 0; j < cols[i].size(); ++j)
      {
        cout << cols[i][j] << endl;
      }
    }
    cout << "----" << endl;

    for (int i = sz - 2; i > 0; --i)
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
        cout << "before processing the weight of edge i <- cols[.][0] = " << e2weight[make_pair(cols[i][0], i)] << endl;
        e2weight[make_pair(cols[i][0], i)] = w_out - cols[i].size() + 1;
        cout << "after processing the weight of edge i <- cols[.][0] = " << e2weight[make_pair(cols[i][0], i)] << endl;
      }
    }

    // tested until this point!
    // first pass finished


    // for (int i = 0; i < rows[0].size(); ++i)
    // {
    //   cout << rows[0][i] << " ";
    // }

    // cout << "-=-=-=-=-" << endl;

    map<pair<int,int>,int> Imin, Imax, Rc, Lc;

    // todo give these boys more meaningful names, but it is easier to
    // follow the original naming convention from the article with them now
    int A = 0, r = 1, L = 0, R = 0; // L and R are for marking the
    // outer regions if we take into account currnt part of the figure limited by 2 chains; initially these point to 0 - 0 is the name of the outer region out figure lies in

    // r is the index of the regions we are about to mark; we maintain 1 such instance in the inner loop, it is enough


    // todo document it! A means the distance between the bois, it is needed at the very beginning; checkthe first stpe of the algorithm to see what happens if we set it to 0! it needs to be set to 1 at the very beginning


    // udpte: A si 0 at the very beginning, it is the index os the leftmost edge, so 0-based

    // todo fix the below loop, it can be rewritten more concisely
    int w_in = 0;
    for (int k = 0; k < rows[0].size(); ++k)
    {
      w_in += (e2weight[make_pair(0, rows[0][k])]);
    }

    int i = 0;
    while (i < sz - 1)
    {
      int w_out = 0;

      for (int k = 0; k < rows[i].size(); ++k)
      {
        w_out += (e2weight[make_pair(i, rows[i][k])]);
      }

      int a = w_in - w_out;
      for (int j = 0; j < rows[i].size(); ++j)
      {
        pair<int,int> e = make_pair(i, rows[i][j]);
        Imin[e] = A;
        Imax[e] = A + a + e2weight[e] - 1;
        cout << "pred : " <<Imin[e] << " " << Imax[e] << ": is given an edge " << "(" << e.first << " " << e.second << endl;
        // int c = pred(Imin[e], Imax[e]);
        // link(c, e); // assign e to chaing c
        if (j == 0)
        {
          Lc[e] = L;
        } else
        {
          Lc[e] = r;
          ++r;
        }
        if (j == rows[i].size() - 1)
        {
          Rc[e] = R;
        } else
        {
          Rc[e] = r;
        }
        a = 0;
        A = Imax[e] + 1;
      }
      ++i; --r;

      w_in = 0;
      for (int k = 0; k < cols[i].size(); ++k)
      {
        w_in += e2weight[make_pair(cols[i][k], i)];
      }

      cout << w_in << endl;

      pair<int,int> d1 = make_pair(cols[i][0], i);
      R = Rc[d1];
      pair<int,int> d2 = make_pair(cols[i][cols[i].size() - 1], i);


      cout << "debug; " << d1.first << " " << d1.second << endl;
      cout << "debug; " << d2.first << " " << d2.second << endl;


      L = Lc[d2];
      A = Imin[d2];
    }
    // supposedly, this is the end of the algorithm

    // need to test it more extensively, but to thispoints seems correct
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

  // this is the example from the article; needs to be modified as it it not regular!
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
