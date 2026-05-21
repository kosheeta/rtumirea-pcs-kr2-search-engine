FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    pkg-config \
    qtbase5-dev \
    qtchooser \
    qt5-qmake \
    qtbase5-dev-tools \
    libqt5widgets5 \
    libqt5gui5 \
    libqt5core5a \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release

RUN cmake --build build

# Tests
RUN cd build && xvfb-run --auto-servernum ctest --output-on-failure

FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    libqt5widgets5 \
    libqt5gui5 \
    libqt5core5a \
    libx11-6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/build /app/build

# Qt/X11
ENV DISPLAY=:0

CMD ["./build/rtumirea_pcs_kr2_search_engine"]
