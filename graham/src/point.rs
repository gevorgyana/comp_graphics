use std::ops::{Add, Sub, Mul};
use std::cmp::Ordering;

mod definite_num;
use point::definite_num::DefinitelyANumber;


#[derive(Debug, Copy, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub struct Point {
    pub x: DefinitelyANumber,
    pub y: DefinitelyANumber,
}

impl Point {
    pub fn new(x: f64, y: f64) -> Point {
        Point {
            x: DefinitelyANumber::new(x).expect("X coordinate cannot be NaN!"),
            y: DefinitelyANumber::new(y).expect("Y coordinate cannot be NaN!"),
        }
    }

    // Euclidean distance
    pub fn distance(&self, other: &Point) -> f64 {
        ((self.x - other.x).to_f64().powi(2) + (self.y - other.y).to_f64().powi(2)).sqrt()
    }

    // Draw a horizontal line through this point, connect this point with the other, and measure the angle between these two lines.
    pub fn angle(&self, other: &Point) -> f64 {
        if self == other {
            0.0
        } else {
            (other.y - self.y).to_f64().atan2((other.x - self.x).to_f64())
        }
    }

    pub fn magnitude(&self) -> f64 {
        (self.x.to_f64().powi(2) + self.y.to_f64().powi(2)).sqrt()
    }

    pub fn sin_cos(&self) -> (f64, f64) {
        let mag = self.magnitude();
        (self.y.to_f64() / mag, self.x.to_f64() / mag)
    }

    pub fn rotate(&self, theta: f64) -> Point {
        let x = self.x.to_f64();
        let y = self.y.to_f64();
        let cosine = theta.cos();
        let sine = theta.sin();
        let x_cos_theta = x * cosine;
        let x_sin_theta = x * sine;
        let y_cos_theta = y * cosine;
        let y_sin_theta = y * sine;
        let x1 = x_cos_theta - y_sin_theta;
        let y1 = x_sin_theta + y_cos_theta;
        Point::new(x1, y1)
    }

    pub fn direction(&self, p1: &Point, p2: &Point) -> Direction {
        let v1 = *p1 - *self;
        let v2 = *p2 - *self;
        let x1 = v1.x.to_f64();
        let x2 = v2.x.to_f64();
        let y1 = v1.y.to_f64();
        let y2 = v2.y.to_f64();
        let det = x1 * y2 - y1 * x2;
        if det < 0.0 {
            Direction::Right
        } else if det > 0.0 {
            Direction::Left
        } else {
            Direction::Ahead
        }
    }
}

#[derive(Debug, PartialEq)]
pub enum Direction {
    Left,
    Right,
    Ahead,
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

// sort by angle to head
pub fn sort_points(points: &mut Vec<Point>) {
    // sort by coordinates so that the first point is the leftmost
    points.sort();
    let head = points[0];
    // sort by the angle with the first point
    // when that is equal, sort by x
    // when that is equal, sort by y
    points.sort_by(|a, b| {
        // head always comes first.
        if a == &head {
            return Ordering::Less;
        }
        if b == &head {
            return Ordering::Greater
        }
        let angle_a = head.angle(&a);
        let angle_b = head.angle(&b);
        let angle_cmp = angle_a.partial_cmp(&angle_b).unwrap();
        if angle_cmp == Ordering::Equal {
            a.cmp(&b)
        } else {
            angle_cmp
        }
    });
}


#[cfg(test)]
mod test {
    use point::Point;
    use super::Direction;
    #[test]
    fn test_point() {
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
    fn test_direction() {
        let p1 = Point::new(1.0, 1.0);
        let p2 = Point::new(2.0, 2.0);
        let p3 = Point::new(3.0, 3.0);
        assert_eq!(p1.direction(&p2, &p3), Direction::Ahead);
        let p1 = Point::new(1.0, 1.0);
        let p2 = Point::new(2.0, 2.0);
        let p3 = Point::new(3.0, 2.5);
        assert_eq!(p1.direction(&p2, &p3), Direction::Right);
        let p1 = Point::new(1.0, 1.0);
        let p2 = Point::new(2.0, 2.0);
        let p3 = Point::new(3.0, 3.5);
        assert_eq!(p1.direction(&p2, &p3), Direction::Left);
        let p1 = Point::new(1.0, -1.0);
        let p2 = Point::new(2.0, -2.0);
        let p3 = Point::new(3.0, -3.0);
        assert_eq!(p1.direction(&p2, &p3), Direction::Ahead);
        let p1 = Point::new(1.0, -1.0);
        let p2 = Point::new(2.0, -2.0);
        let p3 = Point::new(3.0, -2.5);
        assert_eq!(p1.direction(&p2, &p3), Direction::Left);
        let p1 = Point::new(1.0, -1.0);
        let p2 = Point::new(2.0, -2.0);
        let p3 = Point::new(3.0, -3.5);
        assert_eq!(p1.direction(&p2, &p3), Direction::Right);
        let p3 = Point::new(1.0, -1.0);
        let p2 = Point::new(2.0, -2.0);
        let p1 = Point::new(3.0, -3.5);
        assert_eq!(p1.direction(&p2, &p3), Direction::Left);
        let p3 = Point::new(1.0, -1.0);
        let p2 = Point::new(2.0, -2.0);
        let p1 = Point::new(3.0, -2.5);
        assert_eq!(p1.direction(&p2, &p3), Direction::Right);
    }

}
