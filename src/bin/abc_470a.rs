use std::io::{self, Read};


fn main() {
    let mut input = String::new();
    io::stdin().read_to_string(&mut input).unwrap();
    let mut iter = input.split_whitespace();
    let n: usize = iter.next().unwrap().parse().unwrap();
    for i in 1..n + 1 {
        if (i % 3) == 0 {
            println!("Fizz");
        } else {
            println!("{}", i);
        }
    }
}