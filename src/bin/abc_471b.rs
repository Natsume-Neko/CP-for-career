use std::{collections::HashMap, io::Read};

fn main() {
    let mut input = String::new();
    std::io::stdin().read_to_string(&mut input).unwrap();
    let mut iter = input.split_whitespace();
    let n: usize = iter.next().unwrap().parse().unwrap();
    
    let mut count = HashMap::new();
    for _ in 0..n {
        let answer = iter.next().unwrap().to_lowercase();
        *count.entry(answer).or_insert(0) += 1;
    }
    let ans = count.values().max().unwrap();
    println!("{ans}");
}