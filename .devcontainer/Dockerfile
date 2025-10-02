FROM gcc:11-bullseye

RUN apt-get update && apt-get install -y \
    cmake \
    make \
    gdb \
    valgrind \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

CMD ["bash"]