import triton
import torch
import vector_add

sizes = [1024, 65536, 1048576, 4194304]

for n in sizes:
    x = torch.randn(n, device="cuda")
    y = torch.randn(n, device="cuda")

    torch_ms = triton.testing.do_bench(
        lambda: x + y
    )

    triton_ms = triton.testing.do_bench(
        lambda: vector_add.vector_add(x, y)
    )

    print(f"PyTorch: {torch_ms:.4f} ms")
    print(f"Triton:  {triton_ms:.4f} ms")