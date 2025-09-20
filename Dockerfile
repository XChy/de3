#use ubuntu:24.04  to  ensure gcc 13
FROM ubuntu:24.04

ENV http_proxy http://114.212.82.225:4411
ENV https_proxy http://114.212.82.225:4411
RUN echo 'Acquire::http::Proxy "http://114.212.82.225:4411";' | tee -a /etc/apt/apt.conf

# 设置非交互式环境
ENV DEBIAN_FRONTEND=noninteractive


RUN sed -i 's|http://archive.ubuntu.com/ubuntu/|http://mirrors.tuna.tsinghua.edu.cn/ubuntu/|g' /etc/apt/sources.list && \
    sed -i 's|http://security.ubuntu.com/ubuntu/|http://mirrors.tuna.tsinghua.edu.cn/ubuntu/|g' /etc/apt/sources.list


# 安装基础依赖 + wget/curl
RUN apt-get update && apt-get install -y \
    --fix-missing git wget curl gnupg software-properties-common \
    build-essential cmake ninja-build \
    python3 python3-pip python3-venv \
    lld zlib1g-dev libzstd-dev ccache \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install --break-system-packages toml

# 下载并构建 LLVM/Clang (固定 commit: 37eb9c9632f...)
WORKDIR /opt
RUN git clone https://github.com/llvm/llvm-project.git llvm-project && \
    cd llvm-project && \
    git checkout 37eb9c9632fb5e82827d1a0559f2279e9a9f1969 && \
    cmake -GNinja -B build -S llvm \
      -DLLVM_ENABLE_PROJECTS="clang" \
      -DLLVM_TARGETS_TO_BUILD="all" \
      -DCMAKE_BUILD_TYPE=Release \
      -DLLVM_ENABLE_ASSERTIONS=ON \
      -DLLVM_CCACHE_BUILD=ON \
      -DLLVM_USE_LINKER=lld && \
    cmake --build build --target install


# 设置环境变量
ENV PATH="/usr/local/bin:$PATH"
ENV LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"

# 拷贝 de3 源码到容器
WORKDIR /opt
COPY . /opt/de3

# 编译 de3
WORKDIR /opt/de3
RUN cmake -DLLVM_DIR=/usr/lib/llvm-19/cmake . && make -j$(nproc)

# 设置 PATH（确保 opt/clang 可以直接用）
ENV PATH="/usr/local/bin:$PATH"

# 默认启动命令：进入 de3 目录
WORKDIR /opt/de3
CMD ["/bin/bash"]

