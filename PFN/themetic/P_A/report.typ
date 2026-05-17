#set page(paper: "a4", margin: (x: 16mm, y: 16mm))
#set text(font: "New Computer Modern", size: 11pt)
#set par(justify: true, leading: 0.55em, spacing: 0.65em)
#show heading.where(level: 1): set text(size: 14pt, weight: "bold")
#show heading.where(level: 2): set text(size: 12pt, weight: "bold")
#show heading.where(level: 3): set text(size: 11pt, weight: "bold")
#show heading.where(level: 1): it => block(above: 1.0em, below: 0.55em, it)
#show heading.where(level: 2): it => block(above: 0.9em, below: 0.5em, it)
#show heading.where(level: 3): it => block(above: 0.7em, below: 0.4em, it)
#set table(stroke: 0.5pt, inset: 3.5pt)
#show table.cell.where(y: 0): strong

= Problem A --- Dense vs. Sparse Matrix-Vector Multiplication

== Setup

Computation environment, fixed throughout: Intel Core Ultra 5 225H
(14 cores), 32 GiB DDR5, Linux 7.0.1, Python 3.13.12, NumPy 2.4.4,
SciPy 1.17.1, scipy-openblas 0.3.31 with all cores enabled. The matrix
dimension is fixed at *N = 4000*; `A` is the output of
`init_matrices(N, p)` and `x` is a fixed `float64` vector of length
$N$. Each timing is the minimum of 50 timed `A @ x` runs after 3
warm-up runs. The Python construction loop inside `init_matrices` runs
only once and is excluded from the timing, consistent with the stated
assumption that the target application multiplies the same matrix.

== A-1 --- Benchmark and discussion

The benchmark driver `problem_a1.py` sweeps the non-zero density $p$ and
times `A_dense @ x` against `A_sparse @ x` (CSR). Selected rows of
the resulting `benchmark_results.json` are reproduced below.

#align(center)[
  #table(
    columns: (auto, auto, auto, auto, auto),
    align: (right, right, right, right, right),
    table.header([$p$], [nnz], [dense (ms)], [sparse (ms)], [speed-up]),
    [0.001], [16 051],    [1.60], [0.014], [#sym.times 118],
    [0.010], [160 225],   [1.73], [0.131], [#sym.times 13],
    [0.050], [802 034],   [1.73], [0.756], [#sym.times 2.3],
    [0.100], [1 601 646], [1.58], [1.21],  [#sym.times 1.3],
    [0.150], [2 402 221], [1.58], [1.80],  [#sym.times 0.88],
    [0.200], [3 202 938], [1.64], [2.28],  [#sym.times 0.72],
  )
]

Dense throughput reaches $tilde 20$ GFLOP/s; CSR throughput reaches
$tilde 2.7$ GFLOP/s for $p >= 0.05$.

*Throughput analysis.* Both kernels are bounded by memory bandwidth
rather than by floating-point throughput. The dense call dispatches
to OpenBLAS `dgemv`, which streams every entry of `A`: the matrix
occupies $N^2 dot 8 "B" = 128$ MiB, so reading it in 1.6 ms
corresponds to approximately *80 GB/s* --- close to the practical
DDR5 ceiling of this machine (#sym.tilde 90 GB/s). The CSR
(Compressed Sparse Row) format stores only the non-zero entries (one
8-byte value plus one 4-byte column index per non-zero, plus a small
row-pointer array); the SciPy implementation is in C but
single-threaded. At $p >= 0.05$ it sustains approximately 16 GB/s,
the bandwidth a single core can extract from one DDR5 channel. Both
kernels therefore operate at their respective hardware limits, and
the dense-vs-CSR gap is primarily a consequence of the dense kernel
exploiting all 14 cores while CSR uses one.

*Effect of density on performance.*

+ Dense execution time is essentially independent of $p$
  ($approx 1.6$ ms throughout): multiplications by zero entries
  consume the same memory bandwidth and floating-point cycles as
  non-zero entries, so zeros are not free under the dense format.
+ CSR execution time grows linearly with the number of non-zeros;
  doubling $p$ doubles the time (1.21 ms at $p=0.1$
  #sym.arrow.r 2.28 ms at $p=0.2$).
+ The crossover lies at approximately $bold(p approx 0.13)$
  (~15% density). Beyond this point the 4-byte column index per
  non-zero plus the absence of multi-threading in the CSR kernel
  outweigh the saving from skipping zero entries.
+ The crossover is sensitive to thread count. Setting
  `OPENBLAS_NUM_THREADS=1` raises the dense time to ~3.9 ms and pushes
  the crossover beyond $p = 0.2$, so the appropriate format also
  depends on the BLAS thread configuration.

Memory footprint similarly favours CSR: $approx 12 p N^2$ bytes versus
the dense $8 N^2$, so CSR is the smaller representation up to roughly
$p approx 2/3$.

== A-2 --- PageRank as an SpMV-bound application

*Definition.* PageRank assigns to each page $i$ of a directed web
graph a numerical importance $x[i]$, defined as the stationary
distribution of a *random surfer* who at each step either follows a
uniformly random outgoing link (with probability $alpha$) or teleports
to a uniformly random page (with probability $1 - alpha$). The
teleport term, parameterised by the *damping factor* $alpha = 0.85$,
makes the underlying Markov chain irreducible and aperiodic --- so
the stationary distribution is unique --- and bounds the spectral
gap, which controls the convergence rate of the iterative solver.
With $P$ the row-stochastic transition matrix of the link graph, the
rank vector satisfies the fixed-point equation
$ x = alpha dot P^T x + (1 - alpha) / N dot bold(1). $

*Reduction to repeated SpMV.* The standard solver is the *power
iteration*: starting from $x^((0)) = bold(1) \/ N$, the right-hand
side above is applied repeatedly. Each iteration is one CSR
multiplication by the fixed matrix $P^T$ plus an $O(N)$ vector
update; the residual shrinks by a factor of $alpha$ per iteration, so
50--100 iterations reach a tolerance of $10^(-8)$ to $10^(-10)$.
Real web graphs have only ~10--30 outgoing links per page regardless
of $N$, so at $N = 10^8$ the matrix has $"nnz" approx 3 dot 10^9$
(~36 GB in CSR) while the dense representation would require $10^16$
entries and is not a viable storage option --- the operating point is
the very-low-density regime of Section A-1, where CSR is already
100--400 times faster than dense.

*Measured cost.* The script `problem_a2.py` runs the power iteration
above using the SpMV kernel of Section A-1, with `P` taken to be the
row-normalised output of `init_matrices`, $N = 4000$, $alpha = 0.85$,
tolerance $10^(-10)$, and at most 200 iterations.

#align(center)[
  #table(
    columns: (auto, auto, auto, auto, auto),
    align: (right, right, right, right, right),
    table.header([$p$], [iters], [total SpMV (ms)], [per iter (#sym.mu s)], [SpMV / total]),
    [$1 dot 10^(-3)$], [93], [1.33],  [14],   [63%],
    [$5 dot 10^(-3)$], [16], [0.79],  [50],   [84%],
    [$1 dot 10^(-2)$], [13], [1.77],  [137],  [88%],
    [$1 dot 10^(-1)$],  [8], [14.36], [1795], [98%],
  )
]

The SpMV share rises from ~63% to ~98% as the matrix becomes denser;
the vector update and renormalisation are negligible whenever the
matrix is non-trivial, so the throughput measured in Section A-1
directly predicts the application throughput. Extrapolating to
$N = 10^7$ with average out-degree 20 ($"nnz" = 2 dot 10^8$), the CSR
kernel of Section A-1 yields ~1.5 s per SpMV; 80 power iterations
therefore cost roughly 2 minutes of wall time --- practical, but with
all non-SpMV work invisible.

=== SpMV specialisations for the PageRank matrix

+ *Binary-matrix SpMV (factor out the row-stochastic scaling).* Every
  non-zero of $P$ has the value $1\/d_"out"(i)$, which depends only on
  the row, so $P^T = A^T D$ where $A in {0,1}^(N times N)$ is the
  binary link adjacency and $D = "diag"(1\/d_"out")$. The PageRank
  step becomes $P^T x = A^T (D x) = A^T z$ with $z = D x$ pre-computed
  in $O(N)$. The SpMV then reads only column indices, no values, so
  bytes per non-zero drop from 12 to 4 --- a $3 times$ reduction in
  memory traffic on this memory-bound kernel. The decomposition is
  unavailable to a generic SpMV because arbitrary matrix entries are
  not products of a row-only factor and a binary indicator.

+ *Reorder columns by host, then delta-encode indices.* Real web
  graphs have ~80% of links staying within the same host. After
  permuting page IDs so that pages on the same host are contiguous,
  the column indices within each row of $P^T$ are tightly clustered
  --- typically within a span of a few thousand --- so CSR can store
  column *deltas* in 16 bits instead of absolute 32-bit indices,
  shrinking index bytes by $2"--"4 times$. Combined with item 1 the
  SpMV reads ~2 B per non-zero. The reordering is paid once and
  reused; the speed-up depends on a property of web graphs, not of
  sparse matrices in general.

+ *Hub-row chunking for power-law in-degree.* In-degree on a web
  graph follows a power law: a few pages have $10^5$+ in-links while
  the median has under 10. A row-partitioned parallel CSR SpMV gives
  catastrophic thread imbalance --- the thread owning a hub row works
  thousands of times longer than its neighbours. A PageRank-tuned
  kernel splits any row longer than a threshold (chosen at the
  inflection point of the measured in-degree distribution) into
  chunks that several threads sum into the same $y[j]$ via partial
  accumulators. Uniform-padding formats (ELLPACK / Sliced-ELLPACK)
  cannot do this without wasting enormous storage on the long tail.

+ *Fused SpMV + dangling correction + AXPY.* The full PageRank step
  is $x arrow.l alpha (P^T x) + (alpha s + 1 - alpha) \/ N dot bold(1)$
  with $s = sum_(i in D) x[i]$ over dangling pages $D$. A textbook
  pipeline performs this in three separate passes over $y$ (SpMV,
  scale, add bias). A specialised kernel emits each $y[j]$ in a single
  streaming pass over CSR --- row sum, scale by $alpha$, add the
  pre-computed scalar bias --- and updates $s$ for the next iteration
  on the same pass. This roughly halves the per-iteration $y$-traffic
  on the memory-bound kernel.
