use slice_group_by::GroupBy;

/// constructs a full polygon from
/// the given points

/// works with integers

/// immutable as this routine does
/// change the slices

fn register_level(level: &[(i32, i32)]) {
    println!("building a new level from these {:?}", level);
}

/// mutable as this routine needs to
/// sort the slices

fn preprocess(points : &mut [(i32, i32)]) {
    // sort by the y-coordinate
    points.sort_by_key(|k| k.1);
    // form groups where the y-value is the same
    let mut iter = points.linear_group_by(|a, b| a.1 == b.1);
    for group in iter { register_level(group); }
}

fn main() {
    let mut vec = vec![(1, 2), (1, 2), (2, 3)];
    preprocess(&mut vec);
}
