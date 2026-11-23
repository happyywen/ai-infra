import os
import time

# VmSize 代表“地盘”：只要程序向操作系统开口要了 100MB 的虚拟地址空间，无论你有没有真的去用，VmSize 都会立刻增加。
# VmRSS 代表“实体”：只有真正往这些虚拟地址里写入数据时，操作系统才会去物理内存里切一块真的内存（物理页）出来，VmRSS 才会增加。


print("PID:", os.getpid(), flush=True)

data = bytearray(100 * 1024 * 1024)

input("Allocated 100 MiB. Press Enter to touch pages...")

for i in range(0, len(data), 4096):
    data[i] = 1

input("Pages touched. Press Enter to exit...")