use std::collections::{BTreeSet, HashSet};
use geo::{Line, Coordinate};
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

/**
 * this structure contains the slabs
 * and I define searching methods on it
 * later in this code
 */

#[allow(unused)]
pub struct SearchStructure {
    pub data : Option<Vec<CompLine>>,
    pub slabs : Option<Vec<CompLine>>,
    pub tree : Option<BTreeSet<CompLine>>,
}

impl Default for SearchStructure {
    fn default() -> Self {
        Self {
            data : None,
            slabs : None,
            tree : None
        }
    }
}
