# CP-for-career

## Rust

Enter the devenv shell first:

```sh
devenv shell
```

Create a new Rust solution file anywhere in the existing hierarchy:

```sh
rnew atcoder/ABC461/A.rs
```

Run it directly:

```sh
rr atcoder/ABC461/A.rs < input.txt
rr --release atcoder/ABC461/A.rs < input.txt
```

The root `Cargo.toml` exists for rust-analyzer and shared competitive-programming
dependencies. Individual solutions stay as single files such as
`atcoder/ABC461/A.rs`, so this does not conflict with the existing C++ files.
