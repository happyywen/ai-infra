import torch
import gpu_kernel_lab.triton.vector_add.vector_add as vector_add

sizes = [1, 127, 1024, 1025, 100003, 1000003]

for n in sizes:
    x = torch.randn(n, device="cuda")
    y = torch.randn(n, device="cuda")

    actual = vector_add.vector_add(x, y)
    expected = x + y

    torch.testing.assert_close(actual, expected)

    print(f"N={n}: PASS")