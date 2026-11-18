# AIInfraCppLab · Day 6 —— 向量点积（dot_product）C++ 工程

第一周工程实践日产出：把前几天的点积程序整理成一个**能用 CMake 构建、能用 GDB 调试、能用 CTest 测试**的多文件 C++ 小工程。目的是从"会写 C++ 代码"过渡到"能参与 CUDA / 推理框架 / 分布式系统这类真实开源项目"——真实项目里没人手敲一长串 `g++`，构建、调试、测试都有标准化工具链。

## 1. 项目解决什么问题

实现两个等长 `std::vector<double>` 的**点积**：

```
dot(a, b) = a[0]*b[0] + a[1]*b[1] + ... + a[n-1]*b[n-1]
```

接口约定（写在 `include/dot_product.h`）：

- 输入：两个 `std::vector<double>`，**长度必须相等**；
- 输出：`double`，点积结果；
- 长度不等：抛 `std::invalid_argument`；
- 空向量：结果定义为 `0.0`（数学上空和为 0）。

选点积做练手项目，是因为它同时具备：**接口简单、便于测试**、**天然的循环热点**（后续接 CUDA 做 GPU kernel 测试时，它是最直接的起点）。

## 2. 目录结构

```
cpp-basics/day06/
├── CMakeLists.txt          # 构建描述：库 + 两个可执行 + CTest 注册
├── README.md               # 本文件
├── include/
│   └── dot_product.h       # 接口声明（唯一被外部 #include 的头）
├── src/
│   ├── dot_product.cpp     # 实现
│   └── main.cpp            # 演示入口
└── tests/
    └── test_dot.cpp        # 自动化测试（CTest 驱动）
```

关键设计：**接口与实现分离**。`dot_product.h` 只放声明（+ include guard + `#include <vector>`），任何 `.cpp` 只需 `#include` 这一个头就能认识 `dot_product`；具体算法藏在 `dot_product.cpp` 里。这样后续想把循环换成 CUDA kernel，只要改 `dot_product.cpp`（甚至只换 `add_library` 的源文件），`main.cpp` 和 `test_dot.cpp` 一行不用动。

## 3. 环境要求

| 工具 | 版本建议 | 用途 |
|---|---|---|
| C++ 编译器 | 支持 C++17（GCC ≥ 7 / Clang ≥ 5 / MSVC ≥ 2017） | 编译 |
| CMake | ≥ 3.16（`CMakeLists.txt` 声明的最低版本） | 构建 |
| GDB | 任意较新版本 | 调试 |
| CTest | 随 CMake 附带 | 测试驱动 |

Linux / macOS 直接可用；Windows 建议在 WSL 或 MSYS2 下操作，`gdb` 才有完整体验。

## 4. 如何构建

```bash
# 在 cpp-basics/day06/ 目录下
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```

两条命令分别对应 CMake 的两个阶段：

- **配置**：`cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug` 读取 `CMakeLists.txt`，在 `build/` 里生成 `Makefile`/`CMakeCache.txt`，**不编译任何 `.cpp`**；
- **构建**：`cmake --build build -j` 调用底层构建系统，并行执行 `-c`（编译成 `.o`）和链接（打包成 `libdot_product.a`、链接出 `app` / `test_dot`）。

> ⚠️ **一律用 Debug 构建**：测试代码里用到 `assert`，而 `assert` 在 `-DNDEBUG`（Release 默认打开）下会被**整个删除**，测试会静默"通过"而不做任何检查。要么坚持 Debug，要么把测试改成显式 `return 1` 的检查。
>
> ⚠️ **不要在同一个 `build/` 里切换 `CMAKE_BUILD_TYPE`**：`.o` 不会因编译选项变化而重编，产物会悄悄停留在旧模式。需要 Release 就单独建 `build-release/` 目录：
>
> ```bash
> cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
> cmake --build build-release -j
> ```

构建产物（都在 `build/` 里，**不要提交进 Git**）：

```
build/
├── libdot_product.a      # 静态库（add_library 产出，.o 的归档）
├── app                   # 演示程序（add_executable 产出）
├── test_dot              # 测试程序（add_executable 产出）
└── CMakeFiles/           # CMake 内部文件 + 中间 .o，不用手改
```

## 5. 如何运行程序

```bash
./build/app
# 输出示例：
# dot = 32
```

`src/main.cpp` 做的事：构造 `a = {1, 2, 3}`、`b = {4, 5, 6}`，调用 `dot_product(a, b)` 打印结果 `1*4 + 2*5 + 3*6 = 32`。

## 6. 如何运行测试

```bash
ctest --test-dir build --output-on-failure
```

正常结果：

```
    Start 1: DotProductTest
1/1 Test #1: DotProductTest .........   Passed    0.00 sec

100% tests passed, 0 tests failed out of 1
```

测试覆盖 4 个用例（`tests/test_dot.cpp`）：

| # | 用例 | 输入 | 期望 |
|---|---|---|---|
| 1 | 正常向量 | `{1,2,3}` · `{4,5,6}` | `32.0` |
| 2 | 空向量 | `{}` · `{}` | `0.0`（不抛异常） |
| 3 | 零向量 | `{0,0,0}` · `{1,2,3}` | `0.0`（并检查对称性） |
| 4 | 长度不匹配 | `{1,2}` · `{1,2,3}` | 抛 `std::invalid_argument` |

### 测试失败时怎么定位

`--output-on-failure` 会**只在失败时**把测试程序的 stdout/stderr 原样打出来，同时 CTest 的退出码非 0（CI 里靠它判红绿）。故意把 Test 1 的期望值改成 `33.0` 重跑，会看到：

```
1/1 Test #1: DotProductTest .......***Failed
FAIL [line 29]: approx_eq(dot_product(a, b), 33.0)
0% tests passed, 1 tests failed out of 1
```

`FAIL [line N]: <条件原文>` 直接给出失败的**源码行号和条件**，比"测试失败了"这种模糊信息好用得多。

排错常用选项：

| 命令 | 用途 |
|---|---|
| `ctest --output-on-failure` | 失败时打印程序输出（**日常首选**） |
| `ctest --rerun-failed --output-on-failure` | 只重跑上一次失败的用例（改完 bug 快速验证） |
| `ctest -V` | 打印**所有**测试的完整输出和执行命令行（确认工作目录、参数对不对） |
| `ctest -VV` | 再加 CTest 内部调试信息（一般不需要） |


## 7. 如何用 GDB 调试

```bash
gdb ./build/app
```

在 GDB 里依次输入：

```gdb
break main          # 在 main 入口设断点
run                 # 启动程序，停在 main
step                # 单步，遇到函数调用会进去
next                # 单步，不进入函数
break dot_product   # 直接打点到算法内部
continue            # 跑到下一个断点
print sum           # 查看累加结果
print a             # 查看向量内容
print a.size()
backtrace           # 看调用栈：main -> dot_product
quit
```

| 命令 | 用途 |
|---|---|
| `break <函数/行号>` | 设断点 |
| `run` | 启动（带参数：`run arg1 arg2`） |
| `step` | 逐行，进入函数 |
| `next` | 逐行，不进入函数 |
| `print <expr>` | 查看变量/表达式 |
| `backtrace`（`bt`） | 查看调用栈 |
| `continue` | 继续到下一个断点 |
| `quit` | 退出 |

**验收要点**：在 `dot_product()` 内循环里设断点，用 `print i` / `print sum` / `print a[i]` / `print b[i]` 观察每轮迭代，最后一次 `sum == 32`；再用 `backtrace` 说出栈帧：`dot_product` ← `main`。

> **前提**：Debug 构建（`-O0 -g`）。Release 下变量会被优化掉，`print sum` 可能报 `No symbol "sum" in current context`——不是 gdb 坏了，是编译器把变量塞寄存器/内联走了。

## 8. 项目用到的 C++17 知识

| 知识点 | 在哪里体现 |
|---|---|
| **头文件 / 声明与实现分离** | `include/dot_product.h` 声明，`src/dot_product.cpp` 实现 |
| **include guard** | `#ifndef DOT_PRODUCT_H / #define / #endif`，防重复展开 |
| **`std::vector<float>`** | 动态数组容器，`.size()` / `operator[]` / 范围 for |
| **`const` 引用参数** | `const std::vector<double>&`，不拷贝、不修改 |
| **基于范围的 for 循环** | `for (double x : a)`（或 `for (size_t i...)` 带下标版） |
| **异常处理** | `throw std::invalid_argument(...)` + 测试里 `try/catch` |
| **`size_t` 与无符号循环** | `for (size_t i = 0; i < a.size(); ++i)`，避免 `int` / `size_t` 混用告警 |
| **`<cmath>` 浮点比较** | 测试里用 `std::fabs(x - y) < eps`，**不用 `==`** |
| **`<cstdio>` / `<iostream>`** | `main.cpp` 打印结果；测试里用 `fprintf(stderr, ...)` 便于被 CTest 捕获 |
| **CMake Target 模型** | `add_library` / `add_executable` / `target_link_libraries` / `PUBLIC` 属性传播 |

（如果 `dot_product.cpp` 里用了 range-for 累加，把对应行补上；如果用的是 `std::inner_product`，也在表里注明——按你实际实现写。）

## 9. 后续如何扩展为 CUDA Kernel 测试工程

点积的**接口形状**对 GPU 化极其友好：输入是两个同长数组，输出一个标量，`O(n)` 次乘加——正是 CUDA 入门第一个 kernel 的题目。当前工程的目录划分（`include` / `src` / `tests`）和 CMake 的 **Target 模型**（库 + 可执行 + CTest）几乎可以原样复用，扩展路径如下。

### 9.1 目录扩展

```
day06/
├── include/
│   └── dot_product.h          # 保留 CPU 接口，作为"参考实现"
├── src/
│   ├── dot_product.cpp        # CPU 实现（作为 golden/reference）
│   └── cuda/
│       ├── dot_product_cuda.cu   # GPU kernel
│       └── dot_product_cuda.h    # GPU 接口（与 CPU 同签名，便于对比）
└── tests/
    └── test_dot.cpp           # 增加 CPU vs CUDA 一致性测试
```

### 9.2 CMake 扩展：用 `LANGUAGES CXX CUDA` 开启 CUDA

```cmake
project(AIInfraCppLab LANGUAGES CXX CUDA)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# CPU 参考实现保持独立库，便于单独测试
add_library(dot_product_cpu src/dot_product.cpp)
target_include_directories(dot_product_cpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# CUDA 实现作为另一个库，依赖 CPU 版（供对比测试）
add_library(dot_product_cuda src/cuda/dot_product_cuda.cu)
target_include_directories(dot_product_cuda PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(dot_product_cuda PUBLIC dot_product_cpu)  # 若测试里要比较

# 测试程序同时链两个实现
add_executable(test_dot tests/test_dot.cpp)
target_link_libraries(test_dot PRIVATE dot_product_cpu dot_product_cuda)
add_test(NAME DotProductTest COMMAND test_dot)
```

关键点：

- **CUDA 库和 CPU 库分开建**，命名带 `_cpu` / `_cuda` 后缀。这样在同一台机器上，没有 GPU 也能只构建 CPU 目标（`cmake --build build --target dot_product_cpu`），CI 友好。
- **`enable_language(CUDA)` 要确认 `nvcc` 可用**，否则配置阶段就会失败；可以用 `check_language(CUDA)` + `if(CMAKE_CUDA_COMPILER)` 条件式开启，无 GPU 环境自动跳过 CUDA 目标。
- **CTest 支持按标签分组**：`add_test` 后加 `set_tests_properties(... PROPERTIES LABELS "cpu")` / `"gpu"`，跑测试时 `ctest -L gpu` 只跑 GPU 用例，`ctest -LE gpu` 跑除 GPU 外的全部。

### 9.3 Kernel 侧要点

- **数值一致性测试**：GPU 的浮点累加顺序和 CPU 不同，结果不会逐位相等。测试里继续用**容差比较**（本项目 `approx_eq` 就是为此设计的，`eps` 视数据规模取 `1e-6 ~ 1e-9`），而不是 `==`。
- **错误检查**：kernel 启动后必须 `cudaGetLastError()` / `cudaDeviceSynchronize()`，这类检查可以作为测试用例的一部分（"kernel 执行无错误"）。
- **性能测试与正确性测试分开**：CTest 管正确性；性能（`torch.utils.benchmark` 或自写 `cudaEvent` 计时）单独用脚本，避免把跑几十毫秒的性能用例塞进必须秒过的 CTest。
- **GDB 的 GPU 版**：CPU 侧继续 `gdb ./build/app`；GPU kernel 调试用 `cuda-gdb`，命令习惯与 GDB 一致（`break` / `run` / `print` / `bt`），目标文件换成 CUDA 可执行即可。



## 一句话回顾

> 今天没学新语法，学的是**工程化**：`.h` 定契约、`.cpp` 给实现、CMake 把"编译+链接"从手敲命令升级成声明式依赖图（`add_library` / `add_executable` / `target_link_libraries` / `target_include_directories`），GDB 让你在运行时**看见**变量的值，CTest 用退出码把"对不对"变成可自动化、可 CI 的红绿信号。这套骨架**原样**就能长成 CUDA kernel 测试工程——这才是"能参与 AI Infra 开源项目"的起点。