use slice_group_by::GroupBy;
use std::collections::BTreeSet;
use std::collections::HashSet;
use geo::{Line, Coordinate};
// mod library_types; // need this to define that
// the contents of the file types.rs is
// a module.

mod library_types;
//use library_types;

/**
 * this structure contains the slabs
 * and I define searching methods on it
 * later in this code
 */

#[allow(unused)]
struct SearchStructure <'a> {
    // slabs are slices of lines
    slabs : &'a [Line<i32>],
    // levels are slices of tuples
    levels : &'a [(i32, i32)],
    // stores the points that are yet to
    // be visited;
    unvisited : Option<HashSet<(i32, i32)>>,
    // this thing is required by the algorithm
    intermediate : Option<BTreeSet<library_types::CompLine>>,
    visited : Option<HashSet<(i32, i32)>>,
}

impl<'a> Default for SearchStructure <'a> {
    fn default() -> Self {
        Self {
            slabs : &[],
            levels : &[],
            unvisited : None,
            visited : None,
            intermediate : None
        }
    }
}

/**
 * - constructs a full polygon from
 * the given points
 * - works with integers
 * - immutable as this routine does not
 * change the slices
 */

#[allow(unused)]
impl<'a> SearchStructure <'a> {
    /**
     * for earch point; for each line going from it upwards (for each
     * element that has not yet been covered (it is not in the
     * HashSet)), draw a line to it and see at which level the upper
     * point is located (say l2);
     * for every level that has been crossed, (l1 < l <= l2)
     * having the equation x(y) for this segment by 2 points,
     * add this segment (ly, x(ly)) to the array that is
     * located at each level
     * and then sort the array, there is no need for a tree;
     *
     * 2 options;
     * - get the tree; at each level, add items to the tree (current
     * tree), and remove all that end at this level; then collect
     * the slabs by iterating over the tree; the obtained
     * sequence is guaranteed to be what u need;
     *
     * - second option is doing like this; for each item that u
     * process at some level, shoot all the rays coming up from
     * this point and then for each level that is crossed,
     * add it to the array; in fact, you would be spending a lot
     * of time ( which would be proportional to n^2 ) by doing do
     * just because of the fact that you would need to manually
     * process every slab (that would be wasteful)
     *
     * first option is better;
     *
     * also need to get the tree that would sort these segments;
     * a tree that would sort implicityly the segments by
     * insert/remove operations; then u would iterate the tree
     * and collect the segments in the right order;
     *
     * here is the problem; if unwrap sees None, it
     * panicks; of course it will. handle that? yes;
     *
     * the slabs should be allocated in advance;
     * ```
     *       b
     *  -----+-----| level 3
     *     a |     |
     *  ---+-------| level 2
     *      \|     |
     *  -----+-----| level 1
     *       c     |
     *
     * [level 1] -> add line(c, a), line(c, b)
     * [level 2] -> remove line(c, a)
     * [level 3] -> remove line(c, b)
     * ```
     */

    fn accept_next_level (&mut self, level: &[(i32, i32)]) {
        let level_y : i32  = level[0].0;

        for point in level.iter() {
            // maintain the visited/unvisited sets
            self.unvisited.as_mut().unwrap().remove(point);
            self.visited.as_mut().unwrap().insert(*point);

            // maintain the lines above the current level
            for above_point in self.unvisited.as_ref().unwrap() {
                let new_verical_delimiter = Line::<f64> {
                    start :
                    Coordinate::<f64> { x : point.0.into(),
                                        y : point.1.into()},
                    end :
                    Coordinate::<f64> { x : above_point.0.into(),
                                        y : above_point.1.into() }
                };

                let converted_line = library_types::CompLine { val : new_verical_delimiter };

                self.intermediate.as_mut().unwrap().
                    insert(converted_line);
            }

            for below_point in self.visited.as_ref().unwrap() {
                let new_verical_delimiter = Line::<f64> {
                    start :
                    Coordinate::<f64> { x : point.0.into(),
                                        y : point.1.into()},
                    end :
                    Coordinate::<f64> { x : below_point.0.into(),
                                        y : below_point.1.into() }
                };
                let converted_line = library_types::CompLine { val : new_verical_delimiter };
                self.intermediate.as_mut().unwrap().
                    remove(&converted_line);
            }
        }

        // take snapshot of the data; add the array of data to the
        // last slice; need to make the slice 2 dimensional or use
        // vectors;
        /*for i in self.intermediate.unwrap().iter() {
            slabs.
        }*/
    }
}

/**
 * mutable as this routine needs to
 * sort the slices
 */

#[allow(unused)]
fn preprocess(points : &mut [(i32, i32)]) {
    // allocate search structure
    let mut search_structure = SearchStructure{..Default::default()};
    // todo fill the unvisited keys
    // and leave the visited keys in their default state (empty);
    // insert the option with unvisited keys (hash map) here.
    // if u rely on defualts here, the functoin will panic.

    // sort by y-coordinate
    points.sort_by_key(|k| k.1);
    // form groups where the y-value is the same
    let mut iter = points.linear_group_by(|a, b| a.1 == b.1);
    // register points level by level
    for group in iter {
        search_structure.accept_next_level(group);
    }
}

fn main() {
    let mut vec = vec![(1, 2), (1, 2), (2, 3)];
    preprocess(&mut vec);
}
