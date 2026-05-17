"""Problem A-1: benchmark dense vs. CSR sparse matrix-vector multiplication.

Usage: ``python problem_a1.py``  (run inside the project's devenv shell).

The script fixes the matrix dimension ``N`` and sweeps the non-zero density
``p``. For each ``p`` it times ``A @ x`` for both the dense ``np.ndarray``
and the ``scipy.sparse.csr_array`` returned by ``init_matrices``. The result
table is printed to stdout and a machine-readable copy is written to
``benchmark_results.json`` for use by the report.

The target application is assumed to multiply the same matrix by many
different vectors (Hint 4 of the problem statement), so the one-shot
construction cost inside ``init_matrices`` is excluded from the timed region.
"""
from __future__ import annotations

import json
import platform
import time
from dataclasses import dataclass, asdict
from pathlib import Path

import numpy as np
import scipy as sp

from problem_a import init_matrices


N = 4000
DENSITIES = [0.0, 0.001, 0.002, 0.005, 0.01, 0.02, 0.05, 0.1, 0.15, 0.2]
N_WARMUP = 3
N_ITERS = 50


@dataclass
class Row:
    p: float
    nnz: int
    dense_min_ms: float
    dense_med_ms: float
    sparse_min_ms: float
    sparse_med_ms: float
    dense_gflops: float
    sparse_gflops: float
    dense_bytes_per_s: float
    sparse_bytes_per_s: float


def _time_matvec(A, x: np.ndarray, n_warmup: int, n_iters: int) -> tuple[float, float]:
    """Return ``(min, median)`` wall-clock seconds for a single ``A @ x`` call."""
    if n_iters < 1:
        raise ValueError("n_iters must be >= 1")
    y = A @ x
    for _ in range(n_warmup):
        y = A @ x
    samples = np.empty(n_iters, dtype=np.float64)
    for i in range(n_iters):
        t0 = time.perf_counter()
        y = A @ x
        t1 = time.perf_counter()
        samples[i] = t1 - t0
    # Touch ``y`` so the runtime cannot dead-code-eliminate the multiplications.
    if not np.isfinite(y).all():
        raise RuntimeError("non-finite output from matvec")
    return float(samples.min()), float(np.median(samples))


def run_benchmark() -> list[Row]:
    rng = np.random.default_rng(42)
    x = rng.standard_normal(N).astype(np.float64)

    rows: list[Row] = []
    for p in DENSITIES:
        A_dense, A_sparse = init_matrices(N, p)
        assert A_dense.shape == (N, N)
        assert A_sparse.shape == (N, N)

        # Sanity check: dense and CSR must produce the same A @ x.
        assert np.allclose(A_dense @ x, A_sparse @ x), \
            f"dense/sparse result mismatch at p={p}"

        d_min, d_med = _time_matvec(A_dense, x, N_WARMUP, N_ITERS)
        s_min, s_med = _time_matvec(A_sparse, x, N_WARMUP, N_ITERS)

        nnz = int(A_sparse.nnz)
        # Useful FLOPs are 2*nnz (one multiply + one add per non-zero). For
        # dense the kernel processes every entry, so the *executed* FLOPs are
        # 2*N*N regardless of how many entries happen to be zero.
        dense_flops = 2.0 * N * N
        sparse_flops = 2.0 * nnz
        # Memory traffic for streaming SpMV (lower bound):
        #   dense  : N*N*8                (matrix) + 2*N*8         (x, y)
        #   sparse : nnz*(8+4) + (N+1)*4  (data + indices + indptr) + 2*N*8
        dense_bytes = N * N * 8 + 2 * N * 8
        sparse_bytes = nnz * (8 + 4) + (N + 1) * 4 + 2 * N * 8

        rows.append(
            Row(
                p=p,
                nnz=nnz,
                dense_min_ms=d_min * 1e3,
                dense_med_ms=d_med * 1e3,
                sparse_min_ms=s_min * 1e3,
                sparse_med_ms=s_med * 1e3,
                dense_gflops=dense_flops / d_min / 1e9,
                sparse_gflops=sparse_flops / s_min / 1e9 if nnz > 0 else 0.0,
                dense_bytes_per_s=dense_bytes / d_min,
                sparse_bytes_per_s=sparse_bytes / s_min,
            )
        )
        print(
            f"p={p:>6.4f}  nnz={nnz:>9d}  "
            f"dense {d_min*1e3:>7.2f} ms ({rows[-1].dense_gflops:>5.2f} GF/s)  "
            f"sparse {s_min*1e3:>7.2f} ms ({rows[-1].sparse_gflops:>5.2f} GF/s)  "
            f"speedup x{(d_min/s_min):>6.2f}",
            flush=True,
        )
    return rows


def env_info() -> dict:
    return {
        "python": platform.python_version(),
        "platform": platform.platform(),
        "processor": platform.processor(),
        "numpy": np.__version__,
        "scipy": sp.__version__,
        "N": N,
        "n_warmup": N_WARMUP,
        "n_iters": N_ITERS,
    }


def main() -> None:
    info = env_info()
    print("Environment:", json.dumps(info, indent=2))
    rows = run_benchmark()

    out = {
        "env": info,
        "rows": [asdict(r) for r in rows],
    }
    Path("benchmark_results.json").write_text(json.dumps(out, indent=2))
    print("\nSaved benchmark_results.json")


if __name__ == "__main__":
    main()
