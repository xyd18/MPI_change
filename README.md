# MPI_change

## Environment
### Setup Guide
参考 [Getting Started: Building and Running Clang](https://clang.llvm.org/get_started.html)
clang version 14.0.0 (https://github.com/llvm/llvm-project.git 063c2f89aa7f5b0b61a63d639d8124035f26935c)
LLVM version 14.0.0git


## llvm

下面折叠的都不用了, 直接利用apt安装即可
```bash
sudo apt install clang lld llvm
# 还可以安装ninja
sudo apt install ninja-build

```

<details>
  <summary>从源码构建llvm(弃用)</summary>
    参考
    1. [Getting Started with the LLVM System](https://llvm.org/docs/GettingStarted.html#getting-the-source-code-and-building-llvm)
    2. [构建clang](https://mirrors.gitcode.host/FuchsiaOS/FuchsiaOS-docs-zh_CN/development/build/toolchain.html)
    步骤
    1. 浅克隆并使用release/13分支
        `git clone --depth 1 -b release/13.x https://github.com/llvm/llvm-project.git`
    2. 使用`ninja`      
        `sudo apt install ninja-build`
    3. 或许先安装build好的clang和lld可以帮助build
        `sudo apt install clang lld`
    4. build 先构建一次, 或许之后要加上enable_lld再构建一次
        ```bash
        cd llvm-project
        mkdir build && cd build
        cmake -G Ninja -DLLVM_ENABLE_PROJECTS="llvm"  -DLLVM_TARGETS_TO_BUILD="X86, ARM"  -DLLVM_ENABLE_LLD=ON  ../llvm
        cmake --build .
        ```
</details>


## mpich

`sudo apt install mpich`

## makefile

修改makefile中关于llvm-config和mpi.h的路径即可
