use std::io::Read;

fn main() {
    let mut input = String::new();
    std::io::stdin().read_to_string(&mut input).unwrap();
    let mut iter = input.split_whitespace();
    let s = iter.next().unwrap();
    for c in s.chars() {
        if c == 'A' {
            print!("A");
        } else {
            print!(".");
        }
    }
    println!("");
}