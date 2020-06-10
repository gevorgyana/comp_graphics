use std::cmp::Ordering;
use std::ops::{Add, Sub, Mul};
use std::collections::BTreeSet;

#[derive(Debug, Copy, Clone)]
pub struct DefinitelyANumber(f64);

impl DefinitelyANumber {
    // TODO: Make this a real error
    pub fn new(v: f64) -> Result<Self, ()> {
        if v.is_nan() {
            Err(())
        } else {
            Ok(DefinitelyANumber(v))
        }
    }

    pub fn to_f64(&self) -> f64 {
        self.0
    }
}

impl Add for DefinitelyANumber {
    type Output = DefinitelyANumber;
    fn add(self, rhs: DefinitelyANumber) -> DefinitelyANumber {
        DefinitelyANumber(self.0 + rhs.0)
    }
}

impl Sub for DefinitelyANumber {
    type Output = DefinitelyANumber;
    fn sub(self, rhs: DefinitelyANumber) -> DefinitelyANumber {
        DefinitelyANumber(self.0 - rhs.0)
    }
}

impl Mul for DefinitelyANumber {
    type Output = DefinitelyANumber;
    fn mul(self, rhs: DefinitelyANumber) -> DefinitelyANumber {
        DefinitelyANumber(self.0 * rhs.0)
    }
}

impl PartialEq for DefinitelyANumber {
    fn eq(&self, other: &DefinitelyANumber) -> bool {
        self.0 == other.0
    }
}

impl PartialEq<f64> for DefinitelyANumber {
    fn eq(&self, other: &f64) -> bool {
        if other.is_nan() {
            return false;
        }
        self.0 == *other
    }
}

impl Eq for DefinitelyANumber {}

impl PartialOrd for DefinitelyANumber {
    fn partial_cmp(&self, other: &DefinitelyANumber) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for DefinitelyANumber {
    fn cmp(&self, other: &DefinitelyANumber) -> Ordering {
        self.0.partial_cmp(&other.0).expect("A number that can't be NaN was NaN")
    }
}

#[derive(Debug, Copy, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub struct Point {
    x: DefinitelyANumber,
    y: DefinitelyANumber,
}

impl Point {
    fn new(x: f64, y: f64) -> Point {
        Point {
            x: DefinitelyANumber::new(x).expect("X coordinate cannot be NaN!"),
            y: DefinitelyANumber::new(y).expect("Y coordinate cannot be NaN!"),
        }
    }

    // Euclidean distance
    fn distance(&self, other: &Point) -> f64 {
        ((self.x - other.x).to_f64().powi(2) + (self.y - other.y).to_f64().powi(2)).sqrt()
    }

    // Draw a horizontal line through this point, connect this point with the other, and measure the angle between these two lines.
    fn angle(&self, other: &Point) -> f64 {
        if self == other {
            0.0
        } else {
            (other.y - self.y).to_f64().atan2((other.x - self.x).to_f64())
        }
    }
}

impl Add for Point {
    type Output = Point;
    fn add(self, rhs: Point) -> Point {
        Point {
            x: self.x + rhs.x,
            y: self.y + rhs.y,
        }
    }
}
impl Sub for Point {
    type Output = Point;
    fn sub(self, rhs: Point) -> Point {
        Point {
            x: self.x - rhs.x,
            y: self.y - rhs.y,
        }
    }
}
// dot product
impl Mul for Point {
    type Output = f64;
    fn mul(self, rhs: Point) -> f64 {
        (self.x * rhs.x + self.y * rhs.y).to_f64()
    }
}

#[derive(PartialEq, Eq, Debug)]
pub struct Triangle {
    p0: Point,
    p1: Point,
    p2: Point,
}

impl Triangle {
    pub fn new(p0: Point, p1: Point, p2: Point) -> Triangle {
        // Sort by x-coordinate to make sure the first point is the leftmost and lowest.
        let mut v = [p0, p1, p2];
        v.sort();
        Triangle {
            p0: v[0],
            p1: v[1],
            p2: v[2],
        }
    }

    pub fn range_x(&self) -> (f64, f64) {
        (self.p0.x.to_f64(), self.p2.x.to_f64())
    }

    pub fn range_y(&self) -> (f64, f64) {
        let mut v = [self.p0, self.p1, self.p2];
        v.sort_by_key(|v| v.y);
        (v[0].y.to_f64(), v[2].y.to_f64())
    }

    // Barycentric Technique, check whether point is in triangle, see http://blackpawn.com/texts/pointinpoly/
    pub fn contains(&self, p: Point) -> bool {
        let v0 = self.p2 - self.p0;
        let v1 = self.p1 - self.p0;
        let v2 = p - self.p0;
        let dot00 = v0 * v0;
        let dot01 = v0 * v1;
        let dot02 = v0 * v2;
        let dot11 = v1 * v1;
        let dot12 = v1 * v2;
        let inv_denom = (dot00 * dot11 - dot01 * dot01).recip();
        let u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
        let v = (dot00 * dot12 - dot01 * dot02) * inv_denom;

        (u > 0.0) && (v > 0.0) && (u + v < 1.0)
    }
}

#[macro_export]
macro_rules! btreeset {
    ($($x: expr),*) => {{
         let mut set = ::std::collections::BTreeSet::new();
         $( set.insert($x); )*
         set
    }}
}

pub fn convex_hull_naive(points: &BTreeSet<Point>) -> BTreeSet<Point> {
    // you must have at least 3 points, otherwise there is no hull
    assert!(points.len() >= 3);
    // Remove just one point from the set
    let minus_one = |p: &Point| {
        let mut subset = points.clone();
        subset.remove(p);
        subset
    };
    // set of points that are marked as internal
    let mut p_internal_set = BTreeSet::new();
    // check permutations of 4 points, check if the fourth point is contained in the triangle
    for p_i in points {
        let minus_i = minus_one(&p_i);
        for p_j in minus_i {
            let minus_j = minus_one(&p_j);
            for p_k in minus_j {
                let minus_k = minus_one(&p_k);
                for p_m in minus_k {
                    if Triangle::new(*p_i, p_j, p_k).contains(p_m) {
                        p_internal_set.insert(p_m);
                    }
                }
            }
        }
    }
    // set of points that are not internal
    let mut hull: Vec<_> = points.difference(&p_internal_set).cloned().collect();
    // sort by coordinates so that the first point is the leftmost
    hull.sort();
    let head = hull[0];

    // sort by the angle with the first point
    // when that is equal, sort by distance to head
    hull.sort_by(|a, b| {
        let angle_a = head.angle(&a);
        let angle_b = head.angle(&b);
        angle_a.partial_cmp(&angle_b).unwrap()
    });
    hull.into_iter().collect()
}

#[cfg(test)]
mod test {
    use std::collections::BTreeSet;

    use super::*;

    #[test]
    pub fn test_point() {
        use std::f64::consts::PI;
        let p1 = Point::new(0.0, 0.0);
        let p2 = Point::new(0.0, 1.0);
        assert_eq!(p1.angle(&p2), PI / 2.0);
        assert_eq!(p1.distance(&p2), 1.0);
        let p1 = Point::new(0.0, 0.0);
        let p2 = Point::new(1.0, 1.0);
        assert_eq!(p1.angle(&p2), PI / 4.0);
        assert_eq!(p1.distance(&p2), 2.0f64.sqrt());
        let p1 = Point::new(0.0, 0.0);
        let p2 = Point::new(1.0, -1.0);
        assert_eq!(p1.angle(&p2), -PI / 4.0);
        assert_eq!(p1.distance(&p2), 2.0f64.sqrt());
    }

    #[test]
    pub fn test_convex_hull_naive() {
        let points: BTreeSet<_> = (0..4)
            .into_iter()
            .flat_map(move |i| {
                let i = i as f64;
                (0..4).into_iter().map(move |j| {
                    let j = j as f64;
                    Point::new(i, j)
                })
            })
            .collect();
        assert_eq!((&points).len(), 16);
        let hull = convex_hull_naive(&points);
        let hull_should_be = btreeset!(Point::new(0.0, 0.0),
                                       Point::new(1.0, 0.0),
                                       Point::new(2.0, 0.0),
                                       Point::new(3.0, 0.0),
                                       Point::new(3.0, 1.0),
                                       Point::new(3.0, 2.0),
                                       Point::new(3.0, 3.0),
                                       Point::new(2.0, 3.0),
                                       Point::new(1.0, 3.0),
                                       Point::new(0.0, 3.0),
                                       Point::new(0.0, 2.0),
                                       Point::new(0.0, 1.0));
        assert_eq!(hull, hull_should_be);
    }

    #[test]
    pub fn test_triangle() {
        let p2 = Point::new(0.0, 0.0);
        let p1 = Point::new(0.0, 1.0);
        let p0 = Point::new(1.0, 1.0);
        let t = Triangle::new(p0, p1, p2);
        assert_eq!(t.range_x(), (0.0, 1.0));
        assert_eq!(t.range_y(), (0.0, 1.0));
        assert_eq!(t.p0, p2);
        assert_eq!(t.p1, p1);
        assert_eq!(t.p2, p0);
        // triangle should not contain its vertices
        assert!(!t.contains(p0));
        assert!(!t.contains(p1));
        assert!(!t.contains(p2));
        // triangle should contain points on any side
        assert!(!t.contains(Point::new(0.5, 0.5)));
        assert!(!t.contains(Point::new(0.3, 0.3)));
        assert!(!t.contains(Point::new(0.2, 0.2)));
        assert!(!t.contains(Point::new(0.1, 0.1)));
        assert!(!t.contains(Point::new(0.0, 0.1)));
        assert!(!t.contains(Point::new(0.0, 0.2)));
        assert!(!t.contains(Point::new(0.1, 1.0)));
        assert!(!t.contains(Point::new(0.2, 1.0)));
        assert!(!t.contains(Point::new(0.2, 1.1)));
        // strictly inside the triangle
        assert!(t.contains(Point::new(0.5, 0.51)));
        assert!(t.contains(Point::new(0.5, 0.52)));
        assert!(t.contains(Point::new(0.5, 0.53)));
        let p2 = Point::new(0.0, 0.0);
        let p1 = Point::new(0.5, 1.0);
        let p0 = Point::new(1.0, 0.5);
        let t = Triangle::new(p0, p1, p2);
        assert_eq!(t.range_x(), (0.0, 1.0));
        assert_eq!(t.range_y(), (0.0, 1.0));
        assert_eq!(t.p0, p2);
        assert_eq!(t.p1, p1);
        assert_eq!(t.p2, p0);
    }
}
