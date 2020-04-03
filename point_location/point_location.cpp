#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>

namespace computer_graphics_algorithms {
using std::vector;
using std::pair;
using std::cout;

/**
 * A solution for the single-shot variant of the problems of answering if a point is inside of a polygon; this solution does not involve differentiating between convex and non-convex polygons; the usage is as follows - construct an object that will rememeber the information about the polygon - then invoke the pointInPolygon method with a particular point and see what happens; this algorithms uses even-odd rule, for coverage of this topic visit wikipeia;
 */

class PointInPlaneSolution
{
 public:
  PointInPlaneSolution(int poly_corners, vector<float> poly_x_coords, vector<float> poly_y_coords) :
      poly_corners_(poly_corners), poly_x_coords_(poly_x_coords), poly_y_coords_(poly_y_coords)
  {
    cout << "The polygon is being constructed with the following data" << std::endl;
    for (int i = 0; i < poly_x_coords_.size(); ++i)
    {
      std::cout << poly_x_coords_[i] << ' ' << poly_y_coords_[i] << std::endl;
    }
  }

  void PointInPolygon(float x, float y)
  {
    cout << "Point (" << x << ", " << y << ") is being tested" << std::endl;
    int i, j = poly_corners_ - 1;
    bool oddNodes = false;
    for (i = 0; i < poly_corners_; ++i) {
      if ((poly_y_coords_[i] < y && poly_y_coords_[j] >= y)
          || (poly_y_coords_[j] < y && poly_y_coords_[i] >= y)) // if we have a promising point - means that if we have something that lies in between the y coords of the 2 points - jth and ith ones , otherwise we simply cannot intersect ANY edge going from i and j, because intersection means lying between the points in BOTH direction - Ox and Oy;
      {
        if (poly_x_coords_[i] + (y - poly_y_coords_[i]) / (poly_y_coords_[j] - poly_y_coords_[i]) *
            (poly_x_coords_[j] - poly_x_coords_[i]) < x)

          // todo xplain this, done in paper
        {
          oddNodes = !oddNodes;
        }
      }
      j = i;
    }

    // time elapsed
    cout << "The point lies inside the region? " << (oddNodes? "Yes" : "No") << std::endl;;
  }

 private:
  int poly_corners_;
  vector<float> poly_x_coords_;
  vector<float> poly_y_coords_;
};

vector<pair<vector<float>,vector<float>>> PolygonDataPresetComplete()
{
  return {
    {{1, 3, 1, 3}, {1, 3, 3, 1}},
    {{},{}}
  };
}

pair<vector<float>,vector<float>> PolygonDataPresetConcrete(unsigned int test_code)
{
  return PolygonDataPresetComplete()[test_code];
}

vector<pair<float, float>> PointDataPresetComplete()
{
  return {
    {1.4, 1.3}
  };
}

pair<float, float> PointDataPresetConcrete(unsigned int test_code)
{
  return PointDataPresetComplete()[test_code];
}

void GenerateTestcase(unsigned int polygon_preset_testcode,
                      unsigned int point_preset_testcode)
{
  auto polygons = PolygonDataPresetConcrete(0);
  auto points = PointDataPresetConcrete(0);
  PointInPlaneSolution p(polygons.first.size(), polygons.first, polygons.second);
  p.PointInPolygon(points.first, points.second);
}

void CompleteCheck()
{
  cout << "===complete check test started;===" << std::endl;
  for (int i = 0; i < 1; ++i)
  {
    // try every possible testcase
    for (int j = 0; j < 1; ++j)
    {
      GenerateTestcase(i, j);
    }
  }
  cout << "===complete test finished;===" << std::endl;
}

} // namespace computer_graphics_algorithms

int main()
{
  using namespace computer_graphics_algorithms;
  CompleteCheck();
  return 0;
}
