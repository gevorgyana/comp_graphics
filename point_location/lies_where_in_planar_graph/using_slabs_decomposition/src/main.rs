use geo::{Coordinate};
use std::collections::BTreeSet;

mod library_types;
use crate::library_types as types;

/**
 * this structure contains the slabs
 * and I define searching methods on it
 * later in this code
 */

#[allow(unused)]
pub struct SearchStructure {
    pub data : Option<Vec<library_types::CompLine>>,
    pub slabs : Option<Vec<library_types::CompLine>>,
    pub tree : Option<BTreeSet<library_types::CompLine>>,
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

impl SearchStructure {

    /**
     * For each line going from it upwards that has not yet
     * been covered, draw a line to it. The upper point of
     * each line is located at, say, l2, and the lower point is
     * at l1. For every level that has been crossed, (l1 < l <= l2)
     * segment (l, x(l)) is used for locating the test point,
     * where l is the level's y-value, and x(l) is given by the
     * equation of the line that goes through the two points of the
     * selected line.
     *
     * ```
     *  -----b-----| level 3
     *       |     |
     *  ---a-------| level 2
     *      \|     |
     *  -----c-----| level 1
     *
     * [level 1] -> add line(c, a), line(c, b)
     * [level 2] -> remove line(c, a)
     * [level 3] -> remove line(c, b)
     *
     *  b------ level 3
     *  |
     * 'b c     level 2
     *  |/
     *  a------ level 1
     * ```
     * In this picture, line ab should be added when we process level 1,
     * and removed when we process level 3 (note: not on level 2, though
     * at level 2 line a'b still exists, where 'b is the intersection of
     * level 2 and ab.
     *
     * @param{level} contains the indices of all the lines in @field{data}
     * that start from the y-level that is being currently processed.
     * One of the ideas is to accept the line from input and not shorten
     * it when we are processing multiple levels.
     */

    fn accept_next_level (&mut self, level: &mut[usize]) {
        for _index in level.iter() {
            let _line : &types::CompLine = &self.data.as_ref().unwrap()[level[0]];
            self.tree.as_mut().unwrap().insert(*_line);
        }
    }
}

#[allow(unused)]
fn preprocess(points : &mut Vec<Coordinate<f64>>) {
    let mut search_structure = SearchStructure{..Default::default()};
    //search_structure.accept_next_level(&mut[0]);
}

fn main() {
    let mut vec = vec![Coordinate {x : 1.0, y : 2.0}];
    preprocess(&mut vec);
}
