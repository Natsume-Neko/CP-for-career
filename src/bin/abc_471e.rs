use std::io::Read;

const MOD: i64 = 998244353;

fn pow(mut a: i64, mut b: i64, m: i64) -> i64 {
    let mut res = 1;
    while b > 0 {
        if b & 1 == 1 {
            res = res * a % m;
        }
        a = a * a % m;
        b >>= 1;
    }
    res
}

fn inverse(a: i64, p: i64) -> i64 {
    pow(a, p - 2, p)
}

fn combination(a: i64, b: i64, p: i64) -> i64 {
    let (mut up, mut down) = (1, 1);
    for i in a - b + 1..a + 1 {
        up = up * i % p;
    }
    for i in 1..b + 1 {
        down = down * i % p;
    }
    up * inverse(down, p) % p
}

fn main() {
    let mut input = String::new();
    std::io::stdin().read_to_string(&mut input).unwrap();
    let mut iter = input.split_whitespace();
    let mut read_to_number = || -> i64 {
        iter.next().unwrap().parse().unwrap()
    };
    let (n, k) = (read_to_number(), read_to_number());
    let numbers: Vec<_> = (0..n).map(|_| read_to_number()).collect();

    if k == 1 {
        let result = numbers.iter().fold(0, |acc, elem| (acc + elem * elem % MOD) % MOD);
        println!("{result}");
        return;
    }

    let mut result = 0;

    let c1 = combination(n - 1, k - 1, MOD);
    let c2 = combination(n - 2, k - 2, MOD);
    let sum = numbers.iter().fold(0, |acc, elem| (acc + elem) % MOD);
    result += numbers.iter().fold(0, |acc, elem| (acc + c2 * elem % MOD * ((sum - elem + MOD) % MOD) % MOD) % MOD);
    result %= MOD;
    result += numbers.iter().fold(0, |acc, elem| (acc + elem * elem % MOD * c1 % MOD) % MOD);
    result %= MOD;

    println!("{result}");
}