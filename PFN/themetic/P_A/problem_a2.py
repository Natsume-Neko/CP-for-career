"""Problem A-2: PageRank power iteration as an SpMV-bound application.

Usage: ``python problem_a2.py``  (run inside the project's devenv shell).

PageRank is the stationary distribution of a random surfer on a directed
graph: at each step the surfer follows a uniformly random outgoing link
from the current page (with probability ``alpha``) or teleports to a
uniformly random page (with probability ``1 - alpha``). With ``P`` the
row-stochastic transition matrix of the link graph, the rank vector ``x``
is the unique solution of the fixed-point equation

    x = alpha * P^T @ x + (1 - alpha) / N * 1.

The standard solver is power iteration: starting from ``x = 1 / N``, the
right-hand side is applied repeatedly until convergence. Each step is one
CSR multiplication with the *fixed* sparse matrix ``P^T`` plus an O(N)
vector update, so the script exercises the same SpMV implementation as
problem_a1.py and reports the share of total wall-time spent inside the
SpMV. The random matrix returned by ``init_matrices`` is row-normalised so
that ``P`` is a valid row-stochastic transition matrix.
"""
from __future__ import annotations

import time
from dataclasses import dataclass

import numpy as np
import scipy as sp

from problem_a import init_matrices


N = 4000
DENSITIES = [0.0001, 0.001, 0.005, 0.01, 0.05, 0.1]
ALPHA = 0.85
TOL = 1e-10
MAX_ITER = 200


@dataclass
class PageRankResult:
    n_iters: int
    final_residual: float
    total_time_s: float
    spmv_time_s: float


def row_stochastic_csr(A: sp.sparse.csr_array) -> sp.sparse.csr_array:
    """Return a copy of ``A`` with each non-zero row scaled to sum to 1.

    Rows whose entries are all zero (dangling nodes) are left as zero rows.
    """
    A = A.copy()
    row_sums = np.asarray(A.sum(axis=1)).ravel()
    inv = np.zeros_like(row_sums)
    mask = row_sums > 0
    inv[mask] = 1.0 / row_sums[mask]
    return (sp.sparse.diags_array(inv) @ A).tocsr()


def pagerank(P_T: sp.sparse.csr_array, alpha: float = ALPHA,
             tol: float = TOL, max_iter: int = MAX_ITER) -> PageRankResult:
    """Run PageRank power iteration to convergence (or until ``max_iter``).

    ``P_T`` is the transpose of the row-stochastic transition matrix; we
    take it as input so the per-step kernel is exactly one CSR SpMV.
    Returns timing statistics together with the final L1 residual.
    """
    N_ = P_T.shape[0]
    x = np.full(N_, 1.0 / N_, dtype=np.float64)
    bias = (1.0 - alpha) / N_

    spmv_time = 0.0
    t_start = time.perf_counter()
    residual = float("inf")
    k = 0
    for k in range(1, max_iter + 1):
        t0 = time.perf_counter()
        y = P_T @ x
        spmv_time += time.perf_counter() - t0

        x_new = alpha * y + bias
        # Renormalise to absorb the rank mass that leaks through dangling rows.
        x_new /= x_new.sum()

        residual = float(np.abs(x_new - x).sum())
        x = x_new
        if residual < tol:
            break
    total_time = time.perf_counter() - t_start
    return PageRankResult(n_iters=k, final_residual=residual,
                          total_time_s=total_time, spmv_time_s=spmv_time)


def main() -> None:
    print(f"PageRank power iteration  N={N}  alpha={ALPHA}  tol={TOL}  "
          f"max_iter={MAX_ITER}")
    header = (f"{'p':>8}  {'nnz':>10}  {'iters':>6}  "
              f"{'spmv (ms)':>10}  {'spmv/iter (us)':>15}  "
              f"{'spmv/total':>10}  {'residual':>12}")
    print(header)
    for p in DENSITIES:
        _, A_sparse = init_matrices(N, p)
        P = row_stochastic_csr(A_sparse)
        P_T = P.T.tocsr()
        res = pagerank(P_T)
        spmv_per_iter_us = res.spmv_time_s / res.n_iters * 1e6
        print(f"{p:>8.4f}  {A_sparse.nnz:>10d}  {res.n_iters:>6d}  "
              f"{res.spmv_time_s*1e3:>10.2f}  {spmv_per_iter_us:>15.1f}  "
              f"{res.spmv_time_s/res.total_time_s:>9.1%}  "
              f"{res.final_residual:>12.2e}")


if __name__ == "__main__":
    main()
