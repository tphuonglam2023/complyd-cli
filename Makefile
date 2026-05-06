CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include
LDFLAGS  =

BUILD_DIR = build
BIN_DIR   = bin

# Source files
CORE_SRCS = src/scanner_core.cpp \
            src/parsers/file_parser_utils.cpp \
            src/parsers/json_parser.cpp \
            src/parsers/md_parser.cpp \
            src/parsers/pdf_parser.cpp \
            src/frameworks/hipaa/hipaa_loader.cpp \
            src/frameworks/hipaa/hipaa_checks.cpp \
            src/frameworks/hipaa/hipaa_scanner.cpp \
            src/frameworks/iso27001/iso27001_checks.cpp \
            src/frameworks/iso27001/iso27001_scanner.cpp \
            src/frameworks/iso27002/iso27002_checks.cpp \
            src/frameworks/iso27002/iso27002_scanner.cpp \
            src/frameworks/apps/apps_checks.cpp \
            src/frameworks/apps/apps_scanner.cpp \
            src/frameworks/sysaudit/sysaudit_checks.cpp \
            src/frameworks/sysaudit/sysaudit_scanner.cpp

CORE_OBJS = $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(CORE_SRCS))

# Targets
MAIN_BIN          = $(BIN_DIR)/complyd
HIPAA_TEST_BIN    = $(BIN_DIR)/complyd-hipaa
ISO27001_TEST_BIN = $(BIN_DIR)/complyd-iso27001
ISO27002_TEST_BIN = $(BIN_DIR)/complyd-iso27002
APPS_TEST_BIN     = $(BIN_DIR)/complyd-apps
SYSAUDIT_TEST_BIN = $(BIN_DIR)/complyd-sysaudit

ALL_BINS = $(MAIN_BIN) $(HIPAA_TEST_BIN) $(ISO27001_TEST_BIN) $(ISO27002_TEST_BIN) $(APPS_TEST_BIN) $(SYSAUDIT_TEST_BIN)

DOCKER_IMAGE = complyd
DOCKER_TAG   = latest

.PHONY: all clean dirs test docker docker-run docker-save docker-push

all: dirs $(ALL_BINS)

dirs:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)/frameworks/hipaa
	@mkdir -p $(BUILD_DIR)/frameworks/iso27001
	@mkdir -p $(BUILD_DIR)/frameworks/iso27002
	@mkdir -p $(BUILD_DIR)/frameworks/apps
	@mkdir -p $(BUILD_DIR)/frameworks/sysaudit
	@mkdir -p $(BUILD_DIR)/parsers

# Compile core object files
$(BUILD_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Main binary (HIPAA scanner with file parsing)
$(MAIN_BIN): $(CORE_OBJS) $(BUILD_DIR)/main.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# HIPAA test binary
$(HIPAA_TEST_BIN): $(CORE_OBJS) $(BUILD_DIR)/main_hipaa_test.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/main_hipaa_test.o: src/main_hipaa_test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ISO 27001 test binary
$(ISO27001_TEST_BIN): $(CORE_OBJS) $(BUILD_DIR)/main_iso27001_test.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/main_iso27001_test.o: src/main_iso27001_test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ISO 27002 test binary
$(ISO27002_TEST_BIN): $(CORE_OBJS) $(BUILD_DIR)/main_iso27002_test.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/main_iso27002_test.o: src/main_iso27002_test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Apps test binary
$(APPS_TEST_BIN): $(CORE_OBJS) $(BUILD_DIR)/main_apps_test.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/main_apps_test.o: src/main_apps_test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# System Audit test binary (Auto-Update, SSH Key Audit, Unapproved Software)
$(SYSAUDIT_TEST_BIN): $(CORE_OBJS) $(BUILD_DIR)/main_sysaudit_test.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/main_sysaudit_test.o: src/main_sysaudit_test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run all tests
test: all
	@echo "Running HIPAA test..."
	@$(HIPAA_TEST_BIN)
	@echo ""
	@echo "Running ISO 27001 test..."
	@$(ISO27001_TEST_BIN)
	@echo ""
	@echo "Running ISO 27002 test..."
	@$(ISO27002_TEST_BIN)
	@echo ""
	@echo "Running Apps Security test..."
	@$(APPS_TEST_BIN)
	@echo ""
	@echo "Running System Audit test (Auto-Update / SSH Keys / Unapproved Software)..."
	@$(SYSAUDIT_TEST_BIN)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# ── Docker targets ────────────────────────────────────────────────────────────

# Build the Docker image (multi-stage, works on amd64 and arm64)
# Uses 'sg docker' so it works even if the user hasn't re-logged in after being added to the docker group
docker:
	sg docker -c "docker build -t $(DOCKER_IMAGE):$(DOCKER_TAG) ."

# Run the System Audit scanner inside Docker
docker-run:
	sg docker -c "docker run --rm $(DOCKER_IMAGE):$(DOCKER_TAG)"

# Run all scanners inside Docker
docker-test:
	sg docker -c "docker run --rm $(DOCKER_IMAGE):$(DOCKER_TAG) bin/complyd-hipaa"
	sg docker -c "docker run --rm $(DOCKER_IMAGE):$(DOCKER_TAG) bin/complyd-iso27001"
	sg docker -c "docker run --rm $(DOCKER_IMAGE):$(DOCKER_TAG) bin/complyd-iso27002"
	sg docker -c "docker run --rm $(DOCKER_IMAGE):$(DOCKER_TAG) bin/complyd-apps"
	sg docker -c "docker run --rm $(DOCKER_IMAGE):$(DOCKER_TAG) bin/complyd-sysaudit"

# Export the image to a .tar.gz file for offline transfer
docker-save:
	sg docker -c "docker save $(DOCKER_IMAGE):$(DOCKER_TAG) | gzip > $(DOCKER_IMAGE)-docker.tar.gz"
	@echo "Saved to $(DOCKER_IMAGE)-docker.tar.gz"
	@echo "Transfer it and load with: docker load < $(DOCKER_IMAGE)-docker.tar.gz"

# Build a multi-arch image for both Intel/AMD and Apple Silicon
docker-multiarch:
	sg docker -c "docker buildx build \
		--platform linux/amd64,linux/arm64 \
		-t $(DOCKER_IMAGE):$(DOCKER_TAG) \
		--load \
		."
