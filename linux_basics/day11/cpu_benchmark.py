
import statistics
import time

import numpy as np


def benchmark(fn, warmup=5, repeat=20):
    for _ in range(warmup):
        fn()

    times = []

    for _ in range(repeat):
        start = time.perf_counter()
        fn()
        elapsed = time.perf_counter() - start
        times.append(elapsed * 1000)

    return {
        "median_ms": statistics.median(times),
        "min_ms": min(times),
        "max_ms": max(times),
    }


def main():
    n = 100000

    a = np.random.rand(n).astype(np.float32)
    b = np.random.rand(n).astype(np.float32)

    def numpy_add():
        return a + b
    
    def np_add():
        return np.add(a, b)

    def python_add():
        return [float(x) + float(y) for x, y in zip(a, b)]

    print("NumPy:", benchmark(numpy_add))
    print("Python:", benchmark(python_add))
    print("np.add:", benchmark(np_add))


if __name__ == "__main__":
    main()
