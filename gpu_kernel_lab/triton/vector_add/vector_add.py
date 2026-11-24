import triton
import triton.language as tl
import torch

DEVICE = triton.runtime.driver.active.get_active_torch_device()

@triton.jit
def vector_add_kernel(
    x_ptr,
    y_ptr,
    out_ptr,
    n_elements: tl.constexpr,
    BLOCK_SIZE: tl.constexpr
):
    pid = tl.program_id(axis=0)

    # 当前 program 负责的数据块的起始位置
    start = pid * BLOCK_SIZE
    # 把起始位置加上这个序号向量，就得到了当前 Program 需要访问的一组精确的显存地址，eg: [2048, 2049, …, 3071]
    offsets = start + tl.arange(0, BLOCK_SIZE)

    # 掩码防止越界访问
    masks = offsets < n_elements

    # 加载当前 program 需要的数据
    x = tl.load(x_ptr + offsets, mask=masks)
    y = tl.load(y_ptr + offsets, mask=masks)

    out = x + y
    
    # 将当前 program 的结果放入输出结果的对应位置上
    tl.store(out_ptr + offsets, out, mask=masks)

def vector_add(x: torch.Tensor, y: torch.Tensor):
    # 分配存放结果的空间
    out = torch.empty_like(x)
    assert x.device == DEVICE and y.device == DEVICE and out.device == DEVICE

    n_elements = out.numel()

    # 启动多少个 program
    # meta 为元数据，一个字典（dict），里面装着启动时传入的所有编译期常量参数。
    # 凡是 Kernel 签名里标了 tl.constexpr 的参数（比如这里的 BLOCK_SIZE），都会被收集进 meta
    grid = lambda meta: (triton.cdiv(n_elements, meta["BLOCK_SIZE"]),)
    # 启动 kernel
    vector_add_kernel[grid](x, y, out, n_elements=n_elements, BLOCK_SIZE=1024)

    return out

# torch.manual_seed(0)
# size = 984520
# x = torch.rand(size, device=DEVICE)
# y = torch.rand(size, device=DEVICE)
# output_torch = x + y
# output_triton = vector_add(x, y)
# print(output_torch)
# print(output_triton)
# print(f'The maximum difference between torch and triton is '
#       f'{torch.max(torch.abs(output_torch - output_triton))}')

# # bench
# ms = triton.testing.do_bench(lambda: vector_add(x, y), return_mode="mean")
# print(f"Triton latency: {ms:.4f} ms")