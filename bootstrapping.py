import numpy as np
from numpy.typing import NDArray
from numba import njit
from time import perf_counter, time


def f(x: NDArray, p: NDArray) -> NDArray:
    return p[0] + p[1]*x + np.random.randn(*x.shape)*0.1*x


def model(x: NDArray, p: NDArray) -> NDArray:
    p[0] + p[1]*x


def fit(x: NDArray, y: NDArray) -> NDArray:
    X = np.stack((np.ones_like(x), x), axis=1)
    return np.linalg.lstsq(X, y)[0]


def bootstrap(
    x: NDArray,
    y: NDArray,
    num_iters: int = 1000,
) -> NDArray:
    p_boot = np.zeros((num_iters, 2))

    for i in range(num_iters):
        idx_boot = np.random.randint(low=0, high=x.shape[0], size=x.shape[0])
        x_boot = x[idx_boot]
        y_boot = y[idx_boot]

        p = fit(x_boot, y_boot)

        p_boot[i, :] = p
    return p_boot


if __name__ == "__main__":
    x = np.arange(0, 10, 0.01)
    p_true = np.array([0, 1])
    y = f(x, p_true)

    start = perf_counter()
    _ = bootstrap(x, y, num_iters=100_000)
    stop = perf_counter()
    print(f"Python: {(stop-start)*1000:.2f} ms")

    bootstrap = njit(bootstrap)
    fit = njit(fit)

    # warm-up
    p_boot = bootstrap(x, y, num_iters=10)

    start = perf_counter()
    p_boot = bootstrap(x, y, num_iters=100_000)
    stop = perf_counter()
    print(f"numba:  {(stop-start)*1000:.2f} ms")

    print("mean:", np.mean(p_boot, axis=0))
    print("std:", np.std(p_boot, axis=0))

    print(np.percentile(p_boot[:,0], 2.5))
    print(np.percentile(p_boot[:,0], 97.5))

    print(np.percentile(p_boot[:,1], 2.5))
    print(np.percentile(p_boot[:,1], 97.5))