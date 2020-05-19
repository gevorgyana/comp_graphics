use geo::{Coordinate};
use std::collections::BTreeSet;
use std::rc::Rc;

mod ltypes;
use crate::ltypes as types;

/**
 * This structure contains the slabs after last level is
 * passed to accept_next_level().
 */

pub struct SearchStructure {
    // data lives somewhere in heap
    pub data : Vec<Rc<types::L>>,
    pub slabs : Vec<Vec<Rc<types::L>>>,
    pub tree : BTreeSet<Rc<types::L>>,
}

impl Default for SearchStructure {
    fn default() -> Self {
        Self {
            data : Default::default(),
            slabs : Default::default(),
            tree : Default::default()
        }
    }
}

impl SearchStructure {

    /**
     * For each line going upwards that has not yet
     * been covered, draw a line to it. The upper point of
     * each line is located at, say, y-level l2, and the lower point is
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
     * __level__ contains the indices of all the lines in __data__
     * that start from the y-level that is being currently processed.
     * One of the ideas is to accept the line from input and not shorten
     * it when we are processing multiple levels.
     *
     * If, after processing line_up, __slabs__ is empty, we found the
     * upper layer and should stop at this step.
     *
     * Each line is processed in this way; find if we currently have
     * any lines that are not closed, by calculating the amount of lines
     * in the tree after we add what we have in line_up and remove what
     * we have in line_down. When finished, if the tree is empty, that
     * means the line that is being processed belongs to the
     * topmost slab in the resulting data structure. If there are any
     * non-closed lines, then there will be more slabs in next queries.
     * If there are more lines in the next queries, add a slab to
     * the vector.
     */

    fn accept_next_level (&mut self,
                          up: &[Rc<types::L>],
                          down: &[Rc<types::L>]) {

        for index in up.iter() {
            self.tree.insert(index.clone());
        }

        for index in down.iter() {
            self.tree.remove(index);
        }

        /*
         * If the tree is not empty, we are not dealing with the topmost
         * level, which closes the last (highest) slab. This means
         * that we have to add one slab to the vector, which will
         * be closed somewhere at the next levels.
         */

        if !self.tree.is_empty() {
            self.slabs.push(Vec::default());
        }

        // can I rewrite this to be more functional?
        let current_slab = self.slabs.len() - 1;
        for _i in self.tree.iter() {
            &mut self.slabs[current_slab].push(_i.clone());
        }
    }
}

fn main() {
    let mut _search_structure : SearchStructure = Default::default();
}

#[cfg(test)]
mod tests {
    use super::*;
    use geo::Line;


    #[test]
    fn test_general() {
        /*  ___+_
         * |  /-----line that lies inside the slab
         * |_+___   between two plus signs
         */
        let mut search_structure : SearchStructure = Default::default();
        search_structure.data = vec![
            Rc::new (
            types::L { line :
                              Line::<f64> {
                                  start :
                                  Coordinate::<f64> {
                                      x : 0.0,
                                      y : 0.0
                                  },
                                  end :
                                  Coordinate::<f64> {
                                      x : 1.0,
                                      y : 1.0
                                  }
                              }},
            ),
            Rc::new(
            types::L { line :
                              Line::<f64> {
                                  start :
                                  Coordinate::<f64> {
                                      x : 3.0,
                                      y : 3.0
                                  },
                                  end :
                                  Coordinate::<f64> {
                                      x : 4.0,
                                      y : 4.0
                                  }
                              }}
            ),
        ];

        // we can actually insert and remove lines into
        // the internal representation
        assert_eq!(2, search_structure.data.len());
        search_structure.accept_next_level(
            &[search_structure.data[0].clone()], &[]);
        assert_eq!(1, search_structure.tree.len());
        search_structure.accept_next_level(
            &[], &[search_structure.data[0].clone()]);
        assert_eq!(0, search_structure.tree.len());

        // and the slabs are correct
        assert_eq!(1, search_structure.slabs.len());
        assert_eq!(0., search_structure.slabs[0][0].line.start.x);
        assert_eq!(0., search_structure.slabs[0][0].line.start.y);
    }

    // todo test that types work ; this should be done in the other
    // module; here, test if the next_level_works(); also, in the other
    // module, test if the types can be properly stored.
}
