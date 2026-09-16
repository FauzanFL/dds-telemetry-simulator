# ==========================================
# STAGE 1: Build Environment
# ==========================================
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Install paket-paket dependen yang dibutuhkan
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Build & Install Eclipse CycloneDDS (C Core Library)
WORKDIR /opt
RUN git clone https://github.com/eclipse-cyclonedds/cyclonedds.git && \
    cd cyclonedds && \
    cmake -B build -DCMAKE_INSTALL_PREFIX=/usr/local && \
    cmake --build build --target install -j$(nproc)

# Build & Install Eclipse CycloneDDS-CXX (C++ Binding)
WORKDIR /opt
RUN git clone https://github.com/eclipse-cyclonedds/cyclonedds-cxx.git && \
    cd cyclonedds-cxx && \
    cmake -B build -DCMAKE_INSTALL_PREFIX=/usr/local && \
    cmake --build build --target install -j$(nproc)

# Copy source code proyek ke dalam kontainer
WORKDIR /app
COPY . .

# Build Aplikasi DdsSimulator
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build -j$(nproc)


# ==========================================
# STAGE 2: Runtime Environment
# ==========================================
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Copy shared libraries dari stage builder
COPY --from=builder /usr/local/lib /usr/local/lib
COPY --from=builder /usr/local/bin /usr/local/bin

# Copy hasil executable aplikasi
WORKDIR /app
COPY --from=builder /app/build/dds_telemetry_simulator .

# Update dynamic linker cache & set environment path
ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

# Entrypoint aplikasi
ENTRYPOINT ["./dds_telemetry_simulator"]
