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

1. 下载
    ```bash
    cd ~/libraries/
    wget http://www.mpich.org/static/downloads/3.4.2/mpich-3.4.2.tar.gz
    ```
2. 安装说明 `https://www.mpich.org/documentation/guides/`
   ```bash
   tar xfz mpich-3.4.2.tar.gz
   mkdir mpich-install
   
   # configure
   mkdir /tmp/build/mpich-3.4.2 && cd /tmp/build/mpich-3.4.2
   ~/libraries/mpich-3.4.2/configure -prefix=/home/<yourname>/libraries/mpich-install 2>&1 | tee c.txt
   # 其中:
   # prefix选项要求使用绝对路径, 因此不能用~/
   # 2>&1 的意思是 2 的输出管道等同于 1  其中 2 是标准错误, 1 是标准输出
   # 或者用 |&
   # tee是用来把输出写在文件里(便于事后如果需要的话查找), 同时也依然在屏幕上显示
   # 上述命令如果报错, 按提示需要加上:
   --with-device=ch4:ofi
   
   # build (需要好久, 大概半小时?)
   make 2>&1 | tee m.txt

   # install commands
   make install 2>&1 | tee mi.txt

   # 环境变量
   sudo vim ~/.bashrc

   export MPI_ROOT=/home/yuzh/libraries/mpich-install
   export PATH=$MPI_ROOT/bin:$PATH
   export LD_LIBRARY_PATH=$MPI_ROOT/lib:$LD_LIBRARY_PATH
   export MANPATH=$MPI_ROOT/man:$MANPATH

   ```

## linux系统最佳实践小记录(先放到这吧)
1. 用户目录下建立`~/libraries/`库用来放直接下载到的各种库, 如`~/libraries/mpich-3.4.2/`, 安装路径也放到这里面`~/libraries/mpich-install/`
2. 不要在上面的库文件夹里直接configure, 而是到`/tmp/`里建一个文件夹用来configure以及build
3. boost其实也应该这样装就好了, 但是把它用sudo装进`/usr/local/`了sigh...


## makefile

修改makefile中关于llvm-config和mpi.h的路径即可
