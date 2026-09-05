use std::{cmp::min, io::Read};

fn main() {
    let mut input = String::new();
    std::io::stdin().read_to_string(&mut input).unwrap();
    let mut iter = input.split_whitespace();
    let mut read_to_number = || -> i32 {
        iter.next().unwrap().parse().unwrap()
    };
    let n = read_to_number();
    let sticks: Vec<_> = (0..n).map(|_| read_to_number()).collect();
    let mut post_sum: i32 = sticks.iter().sum();
    let mut pre_sum = 0;
    let mut ans = 0x7FFFFFFF;
    for l in sticks.iter() {
        pre_sum += l;
        post_sum -= l;
        ans = min(ans, (post_sum - pre_sum).abs());
    }
    println!("{ans}");
}