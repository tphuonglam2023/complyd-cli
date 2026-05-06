# ============================================================================
#  Complyd Scanner – Multi-stage Dockerfile
#  Supports: linux/amd64 (Intel/AMD) and linux/arm64 (Apple Silicon M1/M2/M3)
#
#  Build:  docker build -t complyd .
#  Run:    docker run --rm complyd
# ============================================================================

# ── Stage 1: Builder ─────────────────────────────────────────────────────────
FROM ubuntu:22.04 AS builder

# Avoid interactive prompts during apt installs
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build

# Copy source tree
COPY . .

# Build all binaries
RUN make all

# ── Stage 2: Runtime ─────────────────────────────────────────────────────────
FROM ubuntu:22.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

# Install only the C++ runtime library (no compiler)
RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /complyd

# Copy compiled binaries from builder stage
COPY --from=builder /build/bin/ ./bin/

# Copy example configs and test fixtures for scanning
COPY --from=builder /build/examples/ ./examples/
COPY --from=builder /build/tests/fixtures/ ./tests/fixtures/

# Create a /scan directory for mounting user config files
RUN mkdir -p /scan

# Default command: run the System Audit scanner (all 3 new modules)
CMD ["bin/complyd-sysaudit"]
