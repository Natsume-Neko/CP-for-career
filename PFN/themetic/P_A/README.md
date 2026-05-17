# Problem A — Dense vs. Sparse Matrix-Vector Multiplication

The full write-up is [`report.typ`](report.typ) (compiled to
[`report.pdf`](report.pdf)). A plain-Markdown copy is in
[`report.md`](report.md).

## How to reproduce

```sh
# Inside the devenv shell (uv-managed venv).
python problem_a1.py    # A-1 benchmark; writes benchmark_results.json
python problem_a2.py    # A-2 PageRank power-iteration benchmark
typst compile report.typ  # rebuild report.pdf (optional)
```

## Files

- [`problem_a1.py`](problem_a1.py) — A-1 benchmark driver (dense vs. CSR SpMV).
- [`problem_a2.py`](problem_a2.py) — A-2 application benchmark (PageRank power iteration).
- [`benchmark_results.json`](benchmark_results.json) — raw A-1 timings + environment info.
- [`report.typ`](report.typ) / [`report.pdf`](report.pdf) — Typst source and compiled report.
- [`report.md`](report.md) — Markdown copy of the report.
- [`problem_a.py`](problem_a.py) — provided, unmodified.
