#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <cmath>
#include <numeric>

/*
 * todo after refactoring support verbose flags
**/

/**
todo regularization, but it will bring even more complexity, first finish with a regular graph
 */

namespace PSLG_Point_Location {
using namespace std;

/**
   tree used for reference

        4
     2     6
    1 3   5 7

    p(x) - max power of 2 in x

    if p(x) = p_, it means that x is located on level p_ from
    the bottom (bottom is level 0)

    leading number l(x) = (2 ** p(x))

    if we divide the whole row by the leading number l,
    we get 1, 3, ... sequence;

    then scroll all the possible numbers from this range to eventully
    meet the desired x

    this will take O(current height), where height changes from log2(n) to 1,
    with step = -2

 */

// find the max power of 2 in @x
int p2(int x)
{
  int p = 0;
  int pp = 1;
  while (x % static_cast<int>(pow(2, pp)) == 0)
  {
    ++p;
    pp *= 2;
  }
  return p;
}

int left_from_edge(pair<int,int> from, pair<int,int> to, pair<int,int> checked)
{
  /**
   * from -> to
   *
   */

  // todo check that it works on all quadrants

  // construct the line passing thru from and to points
  // then, if we continue moving along this line and stay at x point equal to that of the point
  // being checked, what is out y value? maybe it is higher, or lower? check and return the result
  double slope = (to.second - from.second) / (to.first - from.first); // delta(y) / delta(x)
  double a = from.second - slope * from.first; // a(y value) = from.y - slope * from.x;

  int supposed_y = (a + checked.first * slope);


  return (supposed_y < checked.second);
}


/************very big todo check direction functions***/
int right_from_edge(pair<int,int> from, pair<int,int> to, pair<int,int> checked)
{

  /**
   * from -> to
   *
   */

  // todo check that it works on all quadrants

  // construct the line passing thru from and to points
  // then, if we continue moving along this line and stay at x point equal to that of the point
  // being checked, what is out y value? maybe it is higher, or lower? check and return the result
  double slope = (to.second - from.second) / (to.first - from.first); // delta(y) / delta(x)
  double a = from.second - slope * from.first; // a(y value) = from.y - slope * from.x;

  int supposed_y = (a + checked.first * slope);


  return (supposed_y > checked.second);
}

/*
 * move left in the standard heirarchy
 * todo test
**/
int left(int y)
{
  /*
   * in this case,
   *   y
   * x
   * return x

   * y covers exactly (2^(p2(y)) elements) (excluding itself, i.e. strong inclusion)
   * x is the top element in the left half of them
  **/

  return y - (y / 2);
}

/*
 * move right in the standard heirarchy
 * todo test
**/
int right(int y)
{
  return y + (y / 2);
}

/**
 * returns the parent of x in standard search heirarchy
 */
int move(int x)
{
  // currently iterated thru p2-th level of the tree
  int standard_x = pow(2, p2(x));
  int next_x = standard_x;
  int to_the_right = true;
  while (next_x != x)
  {
    cout << next_x << endl;
    to_the_right = !to_the_right;
    next_x += standard_x * 2;
  }

  return x + standard_x  * ((to_the_right ? 1 : -1 ));
}

int lca(int l, int r)
{
  ++l; ++r; // we can only work with 1-based indices
  // cout << "survived to this points 1" << endl;
  // cout << l <<  " " << r << endl;

  int p2_ = p2(l);

  // cout << "survived to this point 2" << endl;
  // cout << p2_ << endl;

  while ((l - (pow(2, p2(l)) - 1) > r) || (l + (pow(2, p2(l)) - 1) < r))
    l = move(l);

  // cout << "stopped executing at point 3" << endl;
  return l;
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

    /*
      adding chains to the chain structure fixme hardcoded 10
    **/

    // update A will also be responsible for all edge indices!
    // update A is the index of the current leftmost edge +-?

    vector<vector<pair<int,int>>> chains(10, vector<pair<int,int>>());

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
        cout << "pred : " << Imin[e] << " " << Imax[e] << ": is given an edge " << "("
             << e.first << " " << e.second << endl;

        // --- +- experimental

        // c must be decreased by 1, see the docs fir lca
        int c = lca(Imin[e], Imax[e]);

        /**
           short reminder:
           the fact Imin(e) is the minimal chain index that contains this edge e;
                    Imax(e) is the maximal one;

           lca(l, r) here means that before we search for chain r, we search for chain l;

           1 - 2 - 3 I
           1 - 2 - 4 J
           these are 2 chains
           it may be confusing tha the parent of I is J, but is is chosed to be this way
           deliberatey

           indeed, one of them will be missing, todo maybe i need to add both of them as predecessors?
         */

        cout << "lca checking edge: " << e.first << " and " << e.second << endl;
        cout << "lca of (is)" << Imin[e] << " and " << Imax[e] << " is " << c - 1 << endl;
        // link(c, e); // assign e to chain c
        chains[c - 1].push_back(e);

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

    // end of preprocessing
    // need to test it more extensively, but to thispoints seems correct
    cout<< "Printing the chain structure" << endl;

    int vi = 0;

    for (auto v : chains)
    {
      cout << "Chain number " << vi++ << " is consituted by the following edges" << endl;
      for (auto& j : v)
      {
        cout <<  j.first << "->" << j.second << endl;
      }
      cout << endl;
    }

    /**
       small comment:
       the chain tree has an implicit root;
       on top of it is the chain that has the maximum index (see examples below)
           4 <- chain 4 is on top
         2
       1  3

       we always start searching from this chain
       it is by definiton the top edge in the tree induced by binary searching
       procedure

       first we select the chain we are going to discriminate the point against

     */

    /*
     * large todo - need to check how it works
    **/

    cout << "chain tree has been constructed; searching now..." << endl;
    // large todo make this double-based, now the code works with integers only
    pair<int,int> checked_point(1, 2);

    // filter the point according to its height todo
    // check that the point indeed lies in between the max and min y-coords

    // todo fix magic numbers
    int min_y_coord = 100,
        max_y_coord = -100;

    for (int i = 0; i < sz; ++i)
    {
      if (min_y_coord > points_[i].second)
      {
        min_y_coord = points_[i].second;
      }

      if (max_y_coord < points_[i].second)
      {
        max_y_coord = points_[i].second;
      }
    }

    cout << "min coord (y) is " << min_y_coord << endl;
    cout << "max coord (y) is " << max_y_coord << endl;

    // large todo move this out of the constructor - this code does not belong here...

    // think about the non-strict check...
    if (checked_point.second <= min_y_coord | checked_point.second >= max_y_coord)
      cout << "the given point lies outside of the planar straight line graph" <<
          endl;

    // todo the tree that is constructed stores more slots than
    // needed now(hardcoded 10); fix that;

    int j = 3; // this is hardcoded for now; but later will be assigned to
               // get_root_chain(max_chain_index); actually this
               // function needs only the the number of
               // chains, it then finds the lca of 2 boundary elements
               // which happens to be the root element

    // this value should be computed somewhere, i am not doing it now, but it needs
    // to be done somewhere above when we handle chains, or maybe in another place,
    // need to think about it
    int mi = 4; // this is the number of chains(1 + the index of the last one!)

    // todo rename these; these two are responsible for binary search process;
    // they are the left and right boundary chains currently considered respectively
    int l = -1;  // first chain's index - 1;
    int ri = mi; // last index + 1 really

    int region_mark; // stores the mark that points to the region where the
                     // points lies

    while (l + 1 != ri)
    {
      cout << "current chain is " << j << endl;

      if (l >= j)
      {
        cout << "moving to the right from " << j << " to " << right(j) << endl;
        j = right(j);
        continue;
      }

      if (ri <= j)
      {
        cout << "moving to the left from " << j << " to " << left(j) << endl;
        j = left(j);
        continue;
      }

      pair<int,int> chosen_edge; // stores the index that leads to the
                                 // first and last parts of
                                 // the chosen edge

      cout << "checking in this chain : " << endl;
      for (auto &e : chains[j])
      {
        cout << e.first << " -> " << e.second << " (indices to vetices)" << endl;
      }

      // iterate over all the elements from j-th chain
      for (auto& e : chains[j])
      {
        cout << "checking if the checked point lies between this edge: " <<
            e.first << " -> " << e.second << endl;
        bool found = (checked_point.second <= (points_[e.first].second) &&
                      checked_point.second >= (points_[e.second].second));
        if (found)
        {// if the tested point lies inside the y coords of the chosen edge
         // there is one according to the invariant that we have protected above
          chosen_edge = e;
          cout << "the checked_point's y value:  "
               << checked_point.second << " lies "
               << "inside the edge: " << "from " << chosen_edge.first
               << " to " << chosen_edge.second << endl;
          cout << "in other words, " << checked_point.second << " <= " <<
              (points_[e.first].second) << " and " << chosen_edge.second <<
              " >= " << (points_[e.second].second) << endl;
          break;
        }

        cout << "tried to check with edge "<< e.first << "->" << e.second
             << ", moving on to the next one" << endl;
      }

      // now we got the edge, need to check if it is on the left side or the right one
      if (right_from_edge(points_[(chosen_edge.first)],
                          points_[(chosen_edge.second)], checked_point))
      {
        cout << "the tested points lies to the right side of the chosed edge" << endl;
        l = Imax[chosen_edge];
        cout << "new rightmost index is " << l << endl;
        cout << "region mark update " << region_mark << " -> " <<
            Rc[chosen_edge] << endl;
        region_mark = Rc[chosen_edge];
      }
      else if (left_from_edge(points_[(chosen_edge.first)],
                              points_[(chosen_edge.second)], checked_point))
      {
        cout << "the tested point lies to the left side of the chosed edge" << endl;
        ri = Imin[chosen_edge];
        cout << "new rightmost index is " << ri << endl;
        cout << "region mark update " << region_mark << " -> " <<
            Lc[chosen_edge] << endl;
        region_mark = Lc[chosen_edge];
      }
      else // else if lies on the edge and we cannot really provide an answer - return
      {
        // do nothing, this should not happen
        // for now
        throw std::runtime_error("not implemented");
      }
    }

  }


 private:
  vector<pair<double,double>> points_;
  vector<vector<int>> edges_;
};
} // ns PSLG_Point_Location

int main()
{
  // std::cout << PSLG_Point_Location::lca(0, 1) << std::endl;


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
