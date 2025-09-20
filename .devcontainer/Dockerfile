FROM gcc:11-bullseye

# Устанавливаем необходимые зависимости
RUN apt-get update && apt-get install -y \
    cmake \
    make \
    python3 \
    python3-dev \
    python3-pip \
    gdb \
    valgrind \
    git \
    && rm -rf /var/lib/apt/lists/*

# Устанавливаем Python пакеты
RUN pip3 install numpy

# Устанавливаем рабочую директорию
WORKDIR /workspace

# Команда по умолчанию
CMD ["bash"]