#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <cmath>
#include <numeric>
#include <cstdio>

/*
 * TODO after refactoring support verbose flags
 * TODO regularization will bring even more complexity, first finish other todos
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

// find the max power of 2 in x
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

/**
 * as all the edges are directed to the bottom, the following functions work properly:
 * left_from_edge() and right_from_egde()
 */

int left_from_edge(pair<int,int> from, pair<int,int> to, pair<int,int> checked)
{
  cout << "testing left_from_edge with the following parameters" << endl;
  cout << "from " <<  from.first << " " << from.second << " -> " << to.first <<
      to.second << "against this point : " << checked.first << " "
       << checked.second << endl;

  // TODO fix this barbarian cast magic
  double slope = 1.0 * (to.second - from.second) / (to.first - from.first);
  cout << "slope " << slope << endl;
  // a(y value) = from.y - slope * from.x;
  double a = from.second - slope * from.first;
  cout << "a parameter value : "  << a << endl;
  int supposed_y = (a + checked.first * slope);
  cout << "supposed_y : " << supposed_y << endl;
  cout << "stopped left_from_edge" << endl;
  if (slope > 0)
    return (supposed_y < checked.second);
  return (supposed_y > checked.second);
}

int right_from_edge(pair<int,int> from, pair<int,int> to, pair<int,int> checked)
{
  cout << "testing right_from_edge with the following parameters" << endl;
  cout << "from " <<  from.first << " " << from.second << " -> " << to.first <<
      to.second << "against this point : " << checked.first << " " <<
      checked.second << endl;
  double slope = 1.0 * (to.second - from.second) / (to.first - from.first);
  cout << "slope : " << slope << endl;
  double a = from.second - slope * from.first; // a(y value) = from.y - slope * from.x
  cout << "beginning point (a parameter): " << a << endl;
  int supposed_y = (a + checked.first * slope);
  cout << "supposed_y " << supposed_y << endl;
  cout << "finished working with right_from_edge" << endl;

  /** here we can also check if they are equal, it means the point is on the line*/
  if (slope > 0)
    return (supposed_y > checked.second);
  return (supposed_y < checked.second);
}

/*
 * move left in the standard heirarchy
**/

int left(int y)
{
  // do not forget to convert to 1-based indexing
  ++y;

  if (y % 2 == 1) // the output is really not well-defined for the bottom row
                  // in the binary tree
  {return y;}

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
**/

int right(int y)
{
  // convert y to 1-based indexing
  ++y;

  if (y % 2 == 1) // the output is really not well-defined for the bottom row
                  // in the binary tree
  {return y;}

  return y + (y / 2);
}

/**
 * returns the parent of x in standard search heirarchy
 */

int move(int x)
{
  // currently iterating thru p2-th level of the tree
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

  int p2_ = p2(l);

  while ((l - (pow(2, p2(l)) - 1) > r) || (l + (pow(2, p2(l)) - 1) < r))
    l = move(l);

  return l;
}

class Solution
{
 public:
  Solution(const vector<pair<double,double>>& points, const vector<vector<int>>& edges)
  {
    cout << "PSLG Point Location with Chain Decomposition Solution object has been invoked" << endl;
    cout << "Initial data..." << endl;
    cout << "--- point number as per input | coords (x) | coords(y) ---" << endl;
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

    /* cannot use unordered map with pair
     * pair is not hashed by means of pure standard library */

    map<pair<double,double>,int> p;
    for (int i = 0; i < sz; ++i)
    {
      p[points_[i]] = i;
    }

    /* we will keep both the sorted array and the index map to
     * convert between edge indices
     * as we may reorder them */

    for (int i = 0; i < sz; ++i)
    {
      // find the i-th smallest elements from the array
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

    // this is generally dangerous to do, but okay here
    auto& pref = points_;

    for (int i = 0; i < rows.size(); ++i)
    {
      sort(rows[i].begin(), rows[i].end(), [i, pref](int n, int m) {
          double atan2n = atan2(pref[n].first - pref[i].first, pref[n].second - pref[i].second),
              atan2m = atan2(pref[m].first - pref[i].first, pref[m].second - pref[i].second);
          return pref[n].first < pref[m].first;
        });
    }

    for (int i = 0; i < cols.size(); ++i)
    {
      sort(cols[i].begin(), cols[i].end(), [i, pref](int n, int m) {
          double atan2n = atan2(pref[i].first - pref[n].first, pref[i].second - pref[n].second),
              atan2m = atan2(pref[i].first - pref[m].first, pref[i].second - pref[m].second);
          return pref[n].first > pref[m].first;
        });
    }

    /**
     * TODO review this, it is obscure and not clear, most
     * probably a reader is not able to understant what is written below

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
     * inside, indices to the sorted structure are stored, filtered thru m[.] */

    // the following info is used for debug purposes
    // to show the internals of the algorithm

    cout << "Checking column lists" << endl;

    for (int i = 0; i < cols.size(); ++i)
    {
      cout << "Checking the columns of vertex #" << i << endl;
      for (int j = 0; j < cols[i].size(); ++j)
      {
        cout << cols[i][j] << " ";
      }
      cout << endl;
    }

    cout << "Checking row lists" << endl;

    for (int i = 0; i < rows.size(); ++i)
    {
      cout << "Checking the rows of vertex #" << i << endl;
      for (int j = 0; j < rows[i].size(); ++j)
      {
        cout << rows[i][j] << " ";
      }
      cout << endl;
    }

    cout << "Weight balancing..." << endl;

    for (int i = sz - 2; i > 0; --i)
    {
      int w_out = 0;
      for (int j = 0; j < rows[i].size(); ++j)
      {
        w_out += e2weight[make_pair(i,rows[i][j])];
      }

      cout << "w_out of vertex " << i << " is " << w_out << endl;

      if (w_out > cols[i].size())
      {
        cout << "debug statement; w_out > cols[i].size()" << endl;
        cout << "debug statement; cols[.].size() = " << cols[i].size() << endl;
        cout << "before processing the weight of edge i <- cols[.][0] = " << e2weight[make_pair(cols[i][0], i)] << endl;
        e2weight[make_pair(cols[i][0], i)] = w_out - cols[i].size() + 1;
        cout << "after processing the weight of edge i <- cols[.][0] = " << e2weight[make_pair(cols[i][0], i)] << endl;
      }
    }

    cout << "==============" << endl << endl;

    // second pass (as per the article) and construction of the chain data structure

    map<pair<int,int>,int> Imin, Imax, Rc, Lc;

    // TODO give these boys more meaningful names, but it is easier to
    // follow the original naming convention from the article with them now

    // L and R are for marking the
    // outer regions if we take into account current part of the figure
    // limited by 2 chains; initially these point to 0 - 0 is the name
    // of the outer region out figure lies in

    int A = 0, r = 1, L = 0, R = 0;

    // r is the index of the regions we are about to mark; we maintain 1 such
    // instance in the inner loop, it is enough

    // TODO document it! A means the distance between the bois, it is needed
    // at the very beginning; checkthe first stpe of the algorithm to see what
    // happens if we set it to 0! it needs to be set to 1 at the very beginning

    // udpte: A is 0 at the very beginning, it is the index os the leftmost edge,
    // so 0-based

    /*
      TODO FIXME HARDCODED!
      chain number is hardcoded to 4;
      if you have more/less chains, print it in the vector size parameter
    */

    vector<vector<pair<int,int>>> chains(4, vector<pair<int,int>>());

    int w_in = 0;
    for (int k = 0; k < rows[0].size(); ++k)
    {
      w_in += (e2weight[make_pair(0, rows[0][k])]);
    }
    int i = 0;
    cout << "Processing outcoming from " << i << "; weight is " << w_in << endl;

    while (i < sz - 1)
    {
      int w_out = 0;
      for (int k = 0; k < rows[i].size(); ++k)
      {
        w_out += (e2weight[make_pair(i, rows[i][k])]);
      }

      cout << "Processing outcoming from " << i << "; weight is " << w_out << endl;

      int a = w_in - w_out;

      cout << "a is (=w_in - w_out) " << a << endl;

      for (int j = 0; j < rows[i].size(); ++j)
      {
        pair<int,int> e = make_pair(i, rows[i][j]);
        Imin[e] = A;
        Imax[e] = A + a + e2weight[e] - 1;

        cout << "working with edge " << e.first << " -> " << e.second << endl;
        cout << "Imin[e] = " << Imin[e] << "; Imax[e] = " << Imax[e] << endl;

        // c must be decreased by 1, see the docs for lca
        int c = lca(Imin[e], Imax[e]) - 1;

        /**
           short reminder:
           the fact Imin(e) is the minimal chain index that contains this edge e;
                    Imax(e) is the maximal one;

           1 - 2 - 3 I
           1 - 2 - 4 J
           these are 2 chains
           it may be confusing tha the parent of I is J, but is is chosed to be
           this way deliberatey
           lca(l, r) = l here means that before we search for chain r,
           we search for chain l;
         */

        cout << "lca of Imin and Imax is " << c << endl;

        // assign e to chain c
        chains[c].push_back(e);

        // TODO not sure that coloring works - I have checked everything, maybe
        // forgot to delete this comment

        if (j == 0)
        {
          cout << "to the left of current edge is : " << L << endl;
          Lc[e] = L;
        } else
        {
          cout << "to the left of current edge if : " << r << endl;
          Lc[e] = r;
          ++r;
        }

        if (j == rows[i].size() - 1)
        {
          cout << "to the right of current edge is " << R << endl;
          Rc[e] = R;
        } else
        {
          cout << "to the right of current edge is " << r << endl;
          Rc[e] = r;
        }

        a = 0;
        A = Imax[e] + 1;
      }
      ++i; /* --r; IMPORTANT!
            * this must be a typo in the original article, it breaks things; */

      w_in = 0;
      for (int k = 0; k < cols[i].size(); ++k)
      {
        w_in += e2weight[make_pair(cols[i][k], i)];
      }

      cout << "Processing incoming from " << i << "; weight is " << w_in << endl;

      pair<int,int> d1 = make_pair(cols[i][0], i);
      R = Rc[d1];

      pair<int,int> d2 = make_pair(cols[i][cols[i].size() - 1], i);
      L = Lc[d2];

      cout << "new global left and right colors: L = " << L << " " << " R = " << R
           << endl;

      A = Imin[d2];
    }

    cout << "-------" << endl << endl;

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

    cout << "chain tree has been constructed; searching now..." << endl;

    // large TODO make this double-based, now the code works with integers only

    pair<int,int> checked_point(3, 2);

    // TODO fix magic numbers
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

    // large TODO move this out of the constructor - this code does not belong here...

    // think about the non-strict check...
    if (checked_point.second <= min_y_coord | checked_point.second >= max_y_coord)
      cout << "the given point lies outside of the planar straight line graph" <<
          endl;

    // TODO
    int j = 3; // this is hardcoded for now; but later will be assigned to
               // get_root_chain(max_chain_index); actually this
               // function needs only the the number of
               // chains, it then finds the lca of 2 boundary elements
               // which happens to be the root element

    // this value should be computed somewhere, i am not doing it now, but it needs
    // to be done somewhere above when we handle chains, or maybe in another place,
    // need to think about it
    int mi = 4; // this is the number of chains(1 + the index of the last one!)

    // TODO rename these; these two are responsible for binary search process;
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
        // convert to 0-based indexing
        cout << "moving to the right from " << j << " to " << right(j) - 1 << endl;
        j = right(j) - 1;
        continue;
      }

      if (ri <= j)
      {
        // convert to 0-based indexing
        cout << "moving to the left from " << j << " to " << left(j) - 1 << endl;
        j = left(j) - 1;
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
              (points_[e.first].second) << " and " << checked_point.second <<
              " >= " << (points_[e.second].second) << endl;
          break;
        }

        cout << "tried to check with edge "<< e.first << "->" << e.second
             << ", moving on to the next one" << endl;
      }

      // document what it means to be to the right side from the ende in the code
      // above, as it may be confusing to comprehend it now

      // now we got the edge, need to check if it is on the left side or the right one
      if (right_from_edge(points_[(chosen_edge.first)],
                          points_[(chosen_edge.second)], checked_point))
      {
        cout << "the tested point lies to the left side from the chosen edge"
             << " (in terms of directed vector), but to the right side from it"
             << " if we suppose it is in front of our eyes and disregard its"
             << " direction;" << endl;

        l = Imin[chosen_edge];
        cout << "new leftmost exclusive index is " << l << endl;
        cout << "region mark update " << region_mark << " -> "
             << Rc[chosen_edge] << endl;

        region_mark = Rc[chosen_edge];
      }
      else if (left_from_edge(points_[(chosen_edge.first)],
                              points_[(chosen_edge.second)], checked_point))
      {
        cout << "the tested point lies to the right side from the chosen edge"
             << " (in terms of directed vector), but to the left side from it"
             << " if we suppose it is in front of our eyes"
             << "and disregard its direction;" << endl;
        // this is in contrast to the original article, l and ri are swapped,
        // important: i think there is a typo in the original article

        ri = Imax[chosen_edge];
        cout << "new rightmost exclusive index is " << ri << endl;
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

  /*std::cout << "checking that left and right work properly " << std::endl;
  for (int i = 0; i < 4; ++i)
  {
    std::cout << PSLG_Point_Location::left(i) << " " <<
        PSLG_Point_Location::right(i) << std::endl;
  }
  */
  //this is the example from the article; needs to be modified as it it not regular!

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
