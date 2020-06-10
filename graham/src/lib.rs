mod point;
use point::{Point, sort_points, Direction};

// see http://i.imgur.com/C2zng5r.png
fn graham_scan(points: &mut Vec<Point>) -> Vec<Point> {
    let mut hull: Vec<Point> = Vec::new();
    sort_points(points);
    hull.push(points[0]);
    hull.push(points[1]);
    for i in 2..points.len() {
        loop {
            println!("{:?}", &hull);
            let m1 = hull.len() - 1;
            let m0 = m1 - 1;
            let direction = hull[m0].direction(&hull[m1], &points[i]);
            match direction {
                Direction::Left => {
                    hull.push(points[i]);
                    break;
                },
                Direction::Ahead =>{
                    hull.pop();
                    hull.push(points[i]);
                    break;
                },
                _ => {
                    hull.pop();
                    ()
                }
            }
        }
    }
    return hull;
}

#[cfg(test)]
mod test {
    use super::graham_scan;
    use point::Point;
    #[test]
    fn test_graham_scan() {
        let mut points: Vec<Point> = Vec::new();
        // These points form a triangle, so only the 3 vertices should be in the convex hull.
        for i in 1..10 {
            points.push(Point::new(i as f64, i as f64));
            points.push(Point::new(i as f64, (-i) as f64));
            points.push(Point::new(i as f64, 0.0));
        }
        points.push(Point::new(0.0, 0.0));
        let hull = graham_scan(&mut points);
        let hull_should_be = vec![
            Point::new(0.0, 0.0),
            Point::new(9.0, -9.0),
            Point::new(9.0, 9.0),
        ];
        assert_eq!(hull, hull_should_be);
    }

}
