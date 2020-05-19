use geo::{Line};
use std::cmp::Ordering;

pub struct L {
    pub line : Line<f64>
}

impl PartialEq for L {
    fn eq(&self, other : &Self) -> bool {
        self.line.start.x == other.line.start.x &&
            self.line.start.y == other.line.start.y &&
            self.line.end.x == other.line.end.x &&
            self.line.end.y == other.line.end.y
    }
}

impl Eq for L {}

impl PartialOrd for L {
    fn partial_cmp(&self, other : &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

/**
 * Sort the lines (think of them as directed upwards). Sort them
 * by the position on the `start` values on Y-axis, and then by their
 * `start` coordinate's X-value, and then by their angle
 * ```dY / dX * (-1, if angle > 90)```
 */

impl Ord for L {
    fn cmp(&self, other : &Self) -> Ordering {

        if self.eq(other) == true { Ordering::Equal }

        else if
            self.line.start.y < other.line.start.y ||
            self.line.start.x < other.line.start.x ||
            self.line.start.x == other.line.start.x &&
            self.line.slope() < other.line.slope()

        { Ordering::Less }

        else { Ordering::Greater }
    }
}
