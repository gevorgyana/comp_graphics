use geo::{Line};
use std::cmp::Ordering;
use std::rc::Rc;

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

pub enum EventType {
    Begins(Rc<L>),
    Ends(Rc<L>),
}

impl Clone for EventType {
    fn clone(&self) -> EventType {
        match self {
            EventType::Begins(rc) => EventType::Begins(rc.clone()),
            EventType::Ends(rc) => EventType::Ends(rc.clone()),
        }
    }
}

impl PartialEq for EventType {
    fn eq(&self, other : &Self) -> bool {
        self.cmp(other) == Ordering::Equal
    }
}

impl Eq for EventType {}

impl PartialOrd for EventType {
    fn partial_cmp(&self, other : &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for EventType {
    fn cmp(&self, other : &Self) -> Ordering {

        match self {
            EventType::Begins (lhs) => {
                match other {
                    EventType::Begins(rhs) => {
                        if lhs.line.start.y == rhs.line.start.y {
                            Ordering::Equal
                        } else if lhs.line.start.y < rhs.line.start.y {
                            Ordering::Less
                        } else {
                            Ordering::Greater
                        }
                    }
                    EventType::Ends(rhs) => {
                        if lhs.line.start.y == rhs.line.end.y {
                            Ordering::Equal
                        } else if lhs.line.start.y < rhs.line.end.y {
                            Ordering::Less
                        } else {
                            Ordering::Greater
                        }
                    }
                }
            },
            EventType::Ends(lhs) => {
                match other {
                    EventType::Begins(rhs) => {
                        if lhs.line.end.y == rhs.line.start.y {
                            Ordering::Equal
                        } else if lhs.line.end.y < rhs.line.start.y {
                            Ordering::Less
                        } else {
                            Ordering::Greater
                        }
                    }
                    EventType::Ends(rhs) => {
                        if lhs.line.end.y == rhs.line.end.y {
                            Ordering::Equal
                        } else if lhs.line.end.y < rhs.line.end.y {
                            Ordering::Less
                        } else {
                            Ordering::Greater
                        }
                    }
                }
            },
        }
    }
}
