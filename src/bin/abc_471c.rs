use std::io::Read;

fn main() {
    let mut input = String::new();
    std::io::stdin().read_to_string(&mut input).unwrap();
    let mut iter = input.split_whitespace();
    let n: usize = iter.next().unwrap().parse().unwrap();
    let mut cookies: Vec<i64> = (0..n).map(|_| iter.next().unwrap().parse().unwrap()).collect();
    cookies.sort();
    
    let mut pos = 0;
    let mut l = 0;
    while l < cookies.len() && cookies[l] < 0 {
        l += 1;
    }
    let mut r = l;
    let mut count = 0;
    while l > 0 || r < cookies.len() {
        if l == 0 {
            count += cookies[r] - pos;
            pos = cookies[r];
            r += 1;
        } else if r == cookies.len() {
            l -= 1;
            count += pos - cookies[l];
            pos = cookies[l];
        } else {
            if cookies[r] - pos >= pos - cookies[l - 1] {
                l -= 1;
                count += pos - cookies[l];
                pos = cookies[l];
            } else {
                count += cookies[r] - pos;
                pos = cookies[r];
                r += 1;
            }
        }
    }
    println!("{count}");
}