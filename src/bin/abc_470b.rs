use std::{collections::HashMap, io::{self, Read}};

fn main() {
    let mut input = String::new();
    io::stdin().read_to_string(&mut input).unwrap();
    let mut iter = input.split_whitespace();
    let n: usize = iter.next().unwrap().parse().unwrap();
    
    let mut count = HashMap::new();
    for _ in 0..n {
        let num: usize = iter.next().unwrap().parse().unwrap();
        *count.entry(num).or_insert(0) += 1;
    }
    
    let ans = n - count.values().max().unwrap();
    println!("{}", ans);
}