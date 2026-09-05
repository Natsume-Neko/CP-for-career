use std::{collections::HashMap, io::{self, Read}};

fn main() {
    let mut input = String::new();
    io::stdin().read_to_string(&mut input).unwrap();
    let mut iter = input.split_whitespace();

    let _: usize = iter.next().unwrap().parse().unwrap();
    let q: usize = iter.next().unwrap().parse().unwrap();
    let mut values = HashMap::new();
    let mut lst_res = 0;
    for _ in 0..q {
        let ty: usize = iter.next().unwrap().parse().unwrap();
        if ty == 1 {
            let x: usize = iter.next().unwrap().parse().unwrap();
            let value = values.entry(x).or_insert(0);
            let res = lst_res ^ *value ^ (*value + 1);
            *value += 1;
            println!("{}", res);
            lst_res = res;
        } else {
            values.retain(|_, value| {
                *value -= 1;
                *value != 0
            });
            let res = values.values().fold(0, |acc, &value| acc ^ value);
            println!("{}", res);
            lst_res = res;
        }
    }
}