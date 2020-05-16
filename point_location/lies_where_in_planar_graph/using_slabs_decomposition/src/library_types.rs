use geo::{Line};
use std::cmp::Ordering::{self, Less, Greater};

pub struct CompLine {
    pub val : Line<f64>,
}

impl Ord for CompLine {
    fn cmp(&self, other : &Self) -> Ordering {
        if self.val.start.x < other.val.start.x {Less}
        else {Greater}
    }
}

impl PartialOrd for CompLine {
    fn partial_cmp(&self, other : &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Eq for CompLine {}

impl PartialEq for CompLine {
    fn eq(&self, other : &Self) -> bool {
        self.val.start.x == other.val.start.x
    }
}
