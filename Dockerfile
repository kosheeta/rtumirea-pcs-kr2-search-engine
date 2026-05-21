FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    qt6-base-dev \
    qt6-base-dev-tools \
    libgl1-mesa-dev \
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

RUN apt-get update && apt-get install -y software-properties-common \
    && add-apt-repository universe \
    && apt-get update && apt-get install -y \
        libqt6core6 \
        libqt6gui6 \
        libqt6widgets6 \
        libxkbcommon-x11-0 \
        libfontconfig1 \
        libglib2.0-0 \
        libgl1 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/build /app/build
COPY ./data /app/build/data

# Qt/X11
ENV DISPLAY=:0

CMD ["./build/rtumirea_pcs_kr2_search_engine"]
