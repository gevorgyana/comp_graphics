use geo::{Coordinate};
use std::collections::BTreeSet;

mod library_types;
use crate::library_types as types;

/**
 * This structure contains the slabs after last level is
 * passed to accept_next_level().
 */

#[allow(unused)]
pub struct SearchStructure <'a> {
    pub data : Vec<library_types::CompLine>,
    // slabs are references to data, this way, no memory
    // duplication happens. note: referecnes do not need
    // to be mutable, this was a mistake previously.
    pub slabs : Vec<Vec<&'a library_types::CompLine>>,
    pub tree : BTreeSet<library_types::CompLine>,
}

impl<'a> Default for SearchStructure<'a> {
    fn default() -> Self {
        Self {
            data : Default::default(),
            slabs : Default::default(),
            tree : Default::default()
        }
    }
}

impl<'a> SearchStructure<'a> {

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
     *
     * When after processing level_start, slabs is empty, that
     * means that we found the upper layer and should stop at
     * this step.
     *
     * Each line is processed in this way; find if we currently have
     * any lines that are not closed by calculating the amount of lines
     * in the tree after we add what we have in line_up and remove what
     * we have in line_down. When finished, if the tree is empty, that
     * means the current line that is being processed belongs to the
     * topmost slab in the resulting data structure. If there are any
     * non-closed lines, then there will be more slabs in next queries.
     * If there will be more lines in the next queries, add a slab to
     * the vector.
     *
     */

    fn accept_next_level (&'a mut self, line_up: &[usize],
                          line_down : &[usize]) {

        // select each line in the line_up
        for index in line_up.iter() {
            let line : &types::CompLine =
                &self.data[line_up[*index]];
            // add it to the tree
            self.tree.insert(*line);
        }

        // select each line in line_down
        for index in line_down.iter() {
            let line : &types::CompLine =
                &self.data[line_down[*index]];
            // remove it from the tree
            self.tree.remove(line);
        }

        /*
         * If the tree is not empty, we are dealing with the topmost
         * level, which closes the last (highest) slab. This means
         * that we have to add one slab to the vector, which will
         * be closed somewhere at the next levels.
         */

        if !self.tree.is_empty() {
            self.slabs.push(Vec::default());
        }

        for _i in self.tree.iter() {
            self.slabs.last_mut().unwrap().push(&_i);
        }
    }
}

#[allow(unused)]
fn preprocess(points : &mut Vec<Coordinate<f64>>) {
    let mut search_structure : SearchStructure = Default::default();
    search_structure.accept_next_level(&[0], &[0]);
}

fn main() {
    let mut vec = vec![Coordinate {x : 1.0, y : 2.0}];
    preprocess(&mut vec);
}
