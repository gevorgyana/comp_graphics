#include <iostream>
#include <cmath>

using std::cout;
using std::endl;

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

/**
 * returns the parent of x in standard search heirarchy
 */
int move(int x)
{
  int standard_x = p2(x);
  int next_x = standard_x;
  int to_the_right = true;
  while (next_x != x)
  {
    to_the_right = false;
    next_x += standard_x * 2;
  }
}

int main()
{
  cout << endl;
  cout << "application is running" << endl;
  cout << p2(2);
  return 0;
}
