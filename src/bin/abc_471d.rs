use std::{cmp::min, collections::BinaryHeap, io::Read};

fn main() {
    let mut input = String::new();
    std::io::stdin().read_to_string(&mut input).unwrap();
    let mut iter = input.split_whitespace();
    let mut read_to_number = || -> i32 {
        iter.next().unwrap().parse().unwrap()
    };
    let (q, v) = (read_to_number(), read_to_number());

    let mut batteries = BinaryHeap::new();
    for _ in 0..q {
        let ty = read_to_number();
        if ty == 1 {
            let (t, w) = (read_to_number(), read_to_number());
            batteries.push(w - t);
        } else {
            let t = read_to_number();
            match batteries.pop() {
                Some(x) => {
                    let res = min(x + t, v);
                    println!("{res}");
                },
                None => {
                    println!("-1");
                }
            }
        }
    }
}