use std::io::Read;

fn main() {
    let mut input = String::new();
    std::io::stdin().read_to_string(&mut input).unwrap();
    let mut iter = input.split_whitespace();
    let mut get_num = || -> usize {
        iter.next().unwrap().parse().unwrap()
    };
    let a = get_num();
    let b = get_num();

    if a + b == 9 || a - b == 9 || a * b == 9 || (a / b == 9 && a % b == 0) {
        println!("Nine");
    } else {
        println!("Nein");
    }
}