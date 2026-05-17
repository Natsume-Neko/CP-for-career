# Problem A — Dense vs. Sparse Matrix-Vector Multiplication

**Environment (fixed for the whole report).** Intel Core Ultra 5 225H
(14 cores), 32 GiB DDR5, Linux 7.0.1, Python 3.13.12, NumPy 2.4.4,
SciPy 1.17.1, scipy-openblas 0.3.31 using all cores. Matrix size
**N = 4000**; `A` from `init_matrices(N, p)`, `x` a fixed `float64`
vector. Each timing is the **min** of 50 runs after 3 warm-ups. The
slow Python construction loop in `init_matrices` runs only once and is
**not** included — the assumed application reuses the same `A`.

## A-1 — Benchmark and discussion

`problem_a1.py` sweeps the non-zero density `p` and times `A_dense @ x` against
`A_sparse @ x` (CSR). Selected rows from `benchmark_results.json`:

| p | nnz | dense (ms) | sparse (ms) | speed-up |
|---:|---:|---:|---:|---:|
| 0.001 |    16 051 | 1.60 | 0.014 | ×118 |
| 0.010 |   160 225 | 1.73 | 0.131 |  ×13 |
| 0.050 |   802 034 | 1.73 | 0.756 |  ×2.3 |
| 0.100 | 1 601 646 | 1.58 | 1.21  |  ×1.3 |
| 0.150 | 2 402 221 | 1.58 | 1.80  |  ×0.88 |
| 0.200 | 3 202 938 | 1.64 | 2.28  |  ×0.72 |

Dense reaches ~20 GFLOP/s; sparse ~2.7 GFLOP/s at p ≥ 0.05.

**Are these numbers reasonable?** Both kernels are limited by how fast
memory feeds the CPU, not by raw arithmetic. Dense `A @ x` calls
OpenBLAS, which goes through every entry of `A`. The matrix is
`N²·8 B = 128 MiB`; reading 128 MiB in 1.6 ms equals about **80 GB/s**,
close to this machine's DDR5 ceiling (~90 GB/s). So dense is already as
fast as the hardware allows. CSR (Compressed Sparse Row) stores only
the non-zeros — one 8-byte value plus one 4-byte column index per
non-zero, plus a small row-pointer array. SciPy's CSR multiplication is
written in C but uses **only one thread**. At p ≥ 0.05 it delivers
≈ 16 GB/s of memory traffic — what one core can pull from one memory
channel. So sparse is also at its hardware limit; the gap to dense is
mostly because sparse is single-threaded.

**How density `p` changes the picture.**

1. *Dense time barely depends on `p`* (≈ 1.6 ms). Multiplying by zero
   still costs the same memory read and floating-point op, so zeros
   are not free.
2. *Sparse time grows linearly with `nnz`*: doubling `p` doubles the
   time (1.21 ms at p=0.1 → 2.28 ms at p=0.2).
3. *Crossover.* Sparse beats dense up to **p ≈ 0.13** (~15 % full).
   Above that, the extra 4 bytes per non-zero for the column index plus
   the lack of multi-threading make sparse slower than dense.
4. *Threading note.* With `OPENBLAS_NUM_THREADS=1`, dense slows to
   ~3.9 ms and sparse stays faster than dense at every density tested,
   so the right format depends on the BLAS thread count too.

**Practical guidance.** Prefer CSR when `p ≲ 0.1`. Memory footprint
also favours sparse: dense uses `8 N²` bytes regardless of `p`, while
CSR uses ≈ `12·p·N²` bytes — CSR is the smaller representation up to
roughly p ≈ 2/3.

## A-2 — PageRank as an SpMV-bound application

**Application.** PageRank ranks pages of a directed graph (e.g. the
web). With `P` the row-stochastic transition matrix (rows sum to 1,
entry `P[i,j]` = probability of jumping i → j), the rank vector `x`
solves

  x ← α · Pᵀ x + (1 − α) / N · 𝟙,    α = 0.85.

Each step is one CSR multiplication with a fixed sparse matrix plus
cheap O(N) vector updates, so the runtime is essentially
`(iterations) × (one SpMV)`. Real web graphs have N up to 10⁸ with only
~10–30 outgoing links per node, i.e. p ≈ 10⁻⁷; dense storage would need
10¹⁶ entries — physically impossible — so sparse is the **only** option.

**Measured cost** (`problem_a2.py`, N = 4000, A-1's SpMV kernel, row-
normalised `init_matrices` output, α = 0.85, tol 10⁻¹⁰, ≤ 200 iters):

| p | iters | total SpMV (ms) | per iter (µs) | SpMV / total |
|---:|---:|---:|---:|---:|
| 1·10⁻³ | 93 |  1.33 |   14 | 63 % |
| 5·10⁻³ | 16 |  0.79 |   50 | 84 % |
| 1·10⁻² | 13 |  1.77 |  137 | 88 % |
| 1·10⁻¹ |  8 | 14.36 | 1795 | 98 % |

The SpMV share rises from ~63 % to ~98 % of total wall-clock time as
the matrix grows; the vector update and renormalisation are negligible
once the matrix is non-trivial. **A-1's SpMV throughput directly
predicts the application throughput.**

**Estimate at realistic scale.** For a medium web graph N = 10⁷ with
average degree 20 (nnz = 2·10⁸), A-1's CSR throughput of
~1.4·10⁸ non-zeros/s gives ≈ 1.5 s per SpMV, so 50 power iterations
cost ≈ 75 s — usable, but SpMV is the bottleneck.

### Optimisations that target this CPU + this workload

1. **Multi-thread SpMV.** SciPy's CSR kernel uses one thread; A-1 shows
   the CPU sustains ~80 GB/s using all 14 cores. Splitting rows of `Pᵀ`
   across threads is lock-free (each thread writes its own slice of
   `y`). With Numba, `mkl-sparse`, or hand-written OpenMP, expect ~8–10×
   speed-up before DRAM bandwidth caps the gain.
2. **Reorder vertices for cache locality.** SpMV reads `x` at scattered
   positions. Relabelling nodes so linked nodes have nearby IDs makes
   those reads nearly sequential and raises cache hit rate.
   `scipy.sparse.csgraph.reverse_cuthill_mckee` does this; METIS / gorder
   are stronger. The relabelling is paid once and reused over all
   iterations — exactly the "fixed matrix" assumption.
3. **Use a vector-friendly sparse format.** CSR's irregular row lengths
   make SIMD (AVX2/AVX-512) hard. ELLPACK / Sliced-ELLPACK pad each row
   to the same length, so a single AVX register processes 4–8 non-zeros
   at once — typically 1.5–2× of CSR throughput when row lengths are
   similar.
4. **Cut the iteration count.** Gauss–Seidel PageRank updates `x` in
   place row-by-row, so later rows in the same sweep already see the
   newer values; this typically halves the number of iterations.
   Chebyshev / Aitken acceleration gives a similar improvement.
5. **Mixed precision.** Power iteration tolerates `float32` for most
   steps. Halving the bytes per non-zero roughly doubles SpMV
   throughput on this memory-bound kernel; one final `float64` step
   recovers the full accuracy.

The first two — multi-threading and reordering — are highest-impact on
this exact machine because the kernel is already memory-bound: they
turn idle DRAM bandwidth and idle cores into useful work without
changing the algorithm.
