#set page(paper: "a4", margin: (x: 16mm, y: 16mm), numbering: "1")
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
#show raw.where(block: true): set text(size: 8.5pt)
#show raw.where(block: true): set par(justify: false, leading: 0.45em)

#align(center)[
  #text(size: 18pt, weight: "bold")[Thematic Questions]
]

#v(1.0em)

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

#pagebreak()

= Problem B --- An Accelerator for LLM Inference: Hardware and Software

== 1. The shape of LLM inference

A large language model is a stack of transformer blocks. To produce
text from a prompt, the chip runs two phases that look completely
different to the silicon underneath. *Prefill* sends the whole prompt
through every layer once; the layer is dominated by a few large matrix
multiplies of shape roughly $(L times d) times (d times d)$, where $L$
is the prompt length and $d$ is the hidden width. With $L$ in the
hundreds, every byte of weight read from memory feeds hundreds of
multiply-accumulate operations, so prefill is *compute-bound*: peak
FLOPs decide its speed. *Decode* then generates tokens one at a time;
each new token re-uses the same weights with $L = 1$, every layer
collapses to a matrix #sym.times vector, and the chip does only a
handful of operations per byte read. Decode is therefore
*memory-bandwidth-bound*: GB/s decide its speed, and FLOPs are mostly
idle. A 70-billion-parameter model in 8-bit weights occupies 70 GB;
generating *one* token requires reading all of it at least once,
which on a 3 TB/s HBM stack costs 23 ms before any arithmetic. The
bandwidth-bound regime is the one users feel as latency, so we treat
it as the primary design constraint.

== 2. Architectural characteristics

=== 2.1 A memory system designed around the decode roofline

The dominant cost during decode is moving bytes, not computing on
them, so the memory subsystem is the most important block on the
chip. Two layers matter.

*Off-chip --- HBM.* HBM (High-Bandwidth Memory) is DRAM stacked
vertically right next to the compute die and connected through a wide
silicon interposer; relative to the DDR DIMMs in a normal server, it
delivers 5--10#sym.times the bandwidth at far lower energy per byte,
because the wires are short and very numerous. A modern accelerator
ships with 4--8 stacks giving a total of 3--5 TB/s and 80--200 GB of
capacity. *Both numbers matter:* if the model plus its KV-cache (the
keys and values produced for every previous token, kept so that
attention does not recompute them) does not fit on-package, weights
spill to host memory over PCIe and effective bandwidth drops by an
order of magnitude. Capacity, not just bandwidth, is therefore a
first-class design parameter.

*On-chip --- SRAM and operator fusion.* HBM bytes are precious, so
the chip should re-read as little as possible. The lever is on-chip
*SRAM* --- a small but very fast memory built into the same silicon as
the compute units, with nanosecond latency and bandwidth orders of
magnitude above HBM. With enough SRAM (tens of MiB per compute
cluster) the compiler can *tile* big tensors into blocks that fit on
chip and *fuse* several operators into a single pass over those
blocks. The canonical case is attention. The naive implementation
forms an $L times L$ score matrix, writes it to HBM, reads it back,
softmaxes it, multiplies by $V$, and writes again --- four trips
across the bandwidth wall whose total size scales as $L^2$. If a
tile of queries, keys and values fits in SRAM, the score block, the
softmax and the value-product all happen on chip, and only the final
output tile is written back. This is FlashAttention, and it is the
single largest software-visible reason to spend transistors on SRAM:
on long contexts it cuts attention bandwidth by more than $10
times$ and turns attention from a memory-bound bottleneck into a
compute-bound block. The same tiling logic fuses the GEMM epilogue
(bias, RMSNorm, RoPE rotation, KV-cache append) into the preceding
matrix multiply, so the activation tensor is never spilled to HBM
between layers. SRAM size therefore directly sets how aggressively
the compiler can fuse, and how close decode runs to the HBM roof.

=== 2.2 Matrix engines tuned for low precision

Inside each compute tile, the bulk of the work is dense matrix
multiplication. The standard structure is a *systolic array*: a 2-D
grid of multiply-accumulate cells through which operands flow in
lock-step, so each value read from SRAM feeds many multiplications
before being discarded. The array's economic value is set almost
entirely by its supported numeric formats. Inference, unlike
training, tolerates very low precision: there is no backward pass to
amplify rounding error, and post-training calibration can absorb most
of the remaining loss. In practice, *FP8* (8-bit float, with separate
E4M3 and E5M2 variants chosen per-tensor for activations vs.
gradients-of-attention) is now the default for both weights and
activations, doubling throughput over FP16 with sub-percent accuracy
loss, and *INT4 weight-only* quantisation (weights stored in 4 bits,
activations kept in FP16/FP8) halves the weight-bandwidth bill on
decode --- where weight bandwidth is *the* bottleneck --- at the cost
of a small dequantise step inside the GEMM epilogue. The matrix engine
must therefore expose FP16, BF16, FP8 and INT4 as first-class input
types with *FP32 accumulation* (a wide running sum keeps the result
accurate even when the inputs are narrow). Treating low precision as
a fallback rather than a primary path is the most common way for a
chip to look fast on paper and slow on real workloads.

=== 2.3 Chip-to-chip interconnect

A 70-B model in 8-bit fits on one chip; a 400-B Mixture-of-Experts
model does not, and even a 70-B model is usually sharded across
several chips to push decode latency below 50 ms/token. The dominant
sharding pattern is *tensor parallelism*: every weight matrix is split
column-wise across $k$ chips, and an `all-reduce` collective sums the
partial activations after each block. During decode the activation
being reduced is tiny (one row of width $d$, a few KB), so what kills
performance is *latency per collective*, not bandwidth. The
interconnect must therefore offer (i) sub-microsecond latency between
chips in the same node, (ii) of order 1 TB/s of bisection bandwidth
to keep prefill all-reduces from becoming the bottleneck, and (iii)
hardware support for *overlap*: launching the all-reduce of layer
$ell$ while the matrix engines start layer $ell + 1$, so
communication and compute share the wall clock instead of stacking.
Without it, decode latency stops scaling past two or three chips.

== 3. Software stack

=== 3.1 A tensor compiler with attention-aware fusion

The frontend (PyTorch, JAX) hands over a static computation graph,
which a compiler lowers through an *intermediate representation* (a
typed, hardware-neutral description of the graph, similar in spirit to
LLVM IR but with tensors as the primitive type) such as MLIR, OpenXLA
or TVM. The IR's job on this hardware is concrete: choose tile shapes
that match the systolic array and fit the SRAM budget, fuse every
element-wise op into the GEMM that produces its input, lower
quantised types to the right hardware instruction, and emit the
asynchronous DMAs that prefetch the next tile while the current one
computes. The frontier feature is *attention-aware fusion*: instead of
treating attention as a sequence of generic ops, the compiler
recognises the pattern and emits a single kernel templated over head
size, sequence length and KV-cache layout (paged or contiguous). The
hand-written kernel library still exists --- FlashAttention,
cuBLAS-style GEMMs, MoE routing --- but as a *fallback and a
correctness oracle*; on a well-engineered compiler stack the
auto-generated path closes most of the gap.

=== 3.2 A serving runtime built around the KV-cache

A trained model becomes a useful product only when many users share
one accelerator. The runtime is what makes that sharing efficient,
and it is structured entirely around the KV-cache, because the
KV-cache is the largest and most awkward tensor in the system: it
grows linearly with context length, varies per request, and must
persist across decode steps. Two ideas dominate.

*Paged KV-cache.* Allocating one contiguous slab per request leads to
internal fragmentation comparable to early operating systems' contiguous
allocation: a request that *might* reach 32 K tokens reserves 32 K
tokens' worth of GPU memory from the start, even if it stops at 200.
The fix, popularised by vLLM, is to allocate the KV-cache in
fixed-size *blocks* (say 16 tokens each) and maintain per-request page
tables that map logical token positions to physical blocks. Two
requests that share a prompt prefix (a system message, a few-shot
template) can then share the prefix blocks by reference, paying the
prefill cost only once. The hardware needs nothing more than fast
gather/scatter with indirect addressing, but the software win is
several-fold: realised throughput typically rises $2"--"4 times$ at
fixed latency.

*Continuous batching.* A naive scheduler decodes a batch to
completion before admitting new requests; short replies then wait for
the longest one in the batch, and the chip is half-idle for most of
the window. Continuous batching merges new requests into the running
decode batch *every step*, so a request that finishes in 50 tokens
frees its slot immediately for an arriving prefill, and a long
request keeps decoding without interruption. The runtime must
therefore co-schedule prefill and decode in the same step (a *chunked
prefill* breaks long prompts into pieces that fit alongside ongoing
decodes), respect per-request priorities, and hand the matrix engine
a different shape every iteration. *Speculative decoding* sits on top:
a small draft model proposes several tokens, the main model verifies
them in one prefill-shaped pass, and accepted tokens skip a full
decode step --- a 2--3#sym.times speed-up at no quality loss.

=== 3.3 Quantisation toolchain

The FP8 and INT4 paths in Section 2.2 are only useful if a trained
BF16 checkpoint can be lowered into them without retraining. An
offline *quantisation toolchain* (GPTQ, AWQ, SmoothQuant) uses a
small calibration set --- a few hundred prompts --- to choose
per-channel scales that preserve activation shapes, and an
outlier-handling step that keeps a fraction of weight columns in
higher precision when their dynamic range demands it. Small in code
volume, it is the bridge that makes the chip's headline FP8/INT4
throughput reachable on real models.

#pagebreak()

= Appendix 1 --- Benchmark programs for Problem A

The two scripts below are the source code referenced in Sections A-1
and A-2.

== A.1  `problem_a1.py`

#raw(read("P_A/problem_a1.py"), lang: "python", block: true)

#pagebreak()

== A.2  `problem_a2.py`

#raw(read("P_A/problem_a2.py"), lang: "python", block: true)

#pagebreak()

= Appendix 2 --- Generative AI Usage Disclosure

#raw(read("P_B/LLM-Usage.md"), lang: "markdown", block: true)
