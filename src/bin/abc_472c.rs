use std::io::Read;

fn main() {
    let mut input = String::new();
    std::io::stdin().read_to_string(&mut input).unwrap();
    let mut iter = input.split_whitespace();
    let mut read_to_i64 = || -> i64 {
        iter.next().unwrap().parse().unwrap()
    };
    let (n, m, k) = (read_to_i64() as usize, read_to_i64() as usize, read_to_i64());
    let snacks = (0..n).map(|_| read_to_i64()).collect::<Vec<_>>();
    let mut is_eaten = Vec::with_capacity(n);
    let mut sum = 0;
    for snack in snacks.iter().take(m) {
        if sum + snack <= k {
            sum += snack;
            is_eaten.push(true);
            println!("Yes");
        } else {
            is_eaten.push(false);
            println!("No");
        }
    }
    let mut l = 0;
    for snack in snacks.iter().skip(m) {
        if is_eaten[l] {
            sum -= snacks[l];
        }
        l += 1;
        if sum + snack <= k {
            sum += snack;
            is_eaten.push(true);
            println!("Yes");
        } else {
            is_eaten.push(false);
            println!("No");
        }
    }
}