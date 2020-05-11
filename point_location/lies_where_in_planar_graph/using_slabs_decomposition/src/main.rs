use slice_group_by::GroupBy;
use geo::Line;

/**
 ** this structure contains the slabs
 ** and I define searching methods on it
 ** later in this code
 */

#[allow(unused)]
struct SearchStructure <'a> {
    // slabs are slices of lines
    slabs : &'a [Line<i32>],
    // levels are slices of tuples
    levels : &'a [(i32, i32)],
}

/**
 ** - constructs a full polygon from
 ** the given points
 ** - works with integers
 ** - immutable as this routine does not
 ** change the slices
 */

#[allow(unused)]
impl SearchStructure <'_> {
    fn accept_next_level<'a> (&self, level: &'a[(i32, i32)]) {
        // for earch point; for each line going from it upwards (for each
        // element that has not yet been covered (it is not in the
        // HashSet)), draw a line to it and see at which level the upper
        // point is located (say l2);
        // for every level that has been crossed, (l1 < l <= l2)
        // having the equation x(y) for this segment by 2 points,
        // add this segment (ly, x(ly)) to the array that is
        // located at each level
        // and then sort the array, there is no need for a tree;

        // 2 options;
        // - get the tree; at each level, add items to the tree (current
        // tree), and remove all that end at this level; then collect
        // the slabs by iterating over the tree; the obtained
        // sequence is guaranteed to be what u need;

        // - second option is doing like this; for each item that u
        // process at some level, shoot all the rays coming up from
        // this point and then for each level that is crossed,
        // add it to the array; in fact, you would be spending a lot
        // of time ( which would be proportional to n^2 ) by doing do
        // just because of the fact that you would need to manually
        // process every slab (that would be wasteful)

        // first option is better;

        // also need to get the tree that would sort these segments;
        // a tree that would sort implicityly the segments by
        // insert/remove operations; then u would iterate the tree
        // and collect the segments in the right order;

        // the slabs should be allocated in advance;
    }
}

/**
 ** mutable as this routine needs to
 ** sort the slices
 */

#[allow(unused)]
fn preprocess(points : &mut [(i32, i32)]) {
    // allocate search structure
    let search_structure = SearchStructure { slabs : &[], levels : &[]};

    // sort by the y-coordinate
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
