use geo::{Line};
use std::cmp::Ordering::{self, Less, Greater, Equal};

pub struct L {
    pub line : Line<f64>
}

impl PartialEq for L {
    fn eq(&self, other : &Self) -> bool {
        self.line.start.x == other.line.start.x
    }
}

impl Eq for L {}

impl PartialOrd for L {
    fn partial_cmp(&self, other : &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

/// this is the logic of comparing lines

impl Ord for L {
    fn cmp(&self, other : &Self) -> Ordering {
        if self.line.start.x == other.line.start.x { Ordering::Equal }
        else if self.line.start.x < other.line.start.x { Ordering::Less }
        else { Ordering::Greater }
    }
}
