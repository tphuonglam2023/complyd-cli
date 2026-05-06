# Complyd Scanner — Technical Reference

> Deep-dive into the architecture, module internals, build system, and Docker packaging.

---

## Table of Contents

1. [Architecture Overview](#architecture-overview)
2. [Build System](#build-system)
3. [Module Internals](#module-internals)
   - [Scanner Core](#scanner-core)
   - [File Parsers](#file-parsers)
   - [HIPAA Module](#hipaa-module)
   - [ISO 27001 / ISO 27002 Modules](#iso-27001--iso-27002-modules)
   - [Application Security Module](#application-security-module)
   - [System Audit Module (new)](#system-audit-module-new)
4. [Check Engine](#check-engine)
5. [Docker Internals](#docker-internals)
6. [Data Flow](#data-flow)
7. [Adding a New Module](#adding-a-new-module)
8. [Configuration File Format](#configuration-file-format)

---

## Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                        CLI Entry Point                       │
│              src/main.cpp  /  src/main_*_test.cpp           │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│                     Scanner Core                             │
│                   src/scanner_core.cpp                       │
│  - Orchestrates file parsing + framework scanning            │
│  - Aggregates scores and prints overall summary              │
└──────┬──────────────────────────────────────┬───────────────┘
       │                                      │
       ▼                                      ▼
┌──────────────────┐              ┌───────────────────────────┐
│   File Parsers   │              │    Framework Scanners      │
│  src/parsers/    │              │  src/frameworks/           │
│  - JSON          │              │  ├── hipaa/                │
│  - Markdown      │              │  ├── iso27001/             │
│  - YAML          │              │  ├── iso27002/             │
│  - PDF           │              │  ├── apps/                 │
│  - Plain text    │              │  └── sysaudit/ (new)       │
└──────────────────┘              └───────────────────────────┘
```

**Language:** C++17 (pure standard library — zero external dependencies)

**Compiler:** g++ (Linux/Ubuntu) or clang++ (macOS)

**Standard:** ISO C++17 (`-std=c++17`)

---

## Build System

### Makefile targets

| Target | Description |
|---|---|
| `make all` | Build all 6 binaries |
| `make test` | Build + run all 5 test scanners |
| `make clean` | Remove `build/` and `bin/` directories |
| `make docker` | Build Docker image `complyd:latest` |
| `make docker-run` | Run System Audit scanner in Docker |
| `make docker-test` | Run all 5 scanners in Docker |
| `make docker-save` | Export image to `complyd-docker.tar.gz` |
| `make docker-multiarch` | Build for `linux/amd64` + `linux/arm64` |

### Compiler flags

```makefile
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include
```

- `-std=c++17` — enables structured bindings, `std::string_view`, `if constexpr`, etc.
- `-Wall -Wextra` — full warning set
- `-I./include` — adds `include/` to the header search path

### Object file layout

```
build/
├── main.o
├── main_hipaa_test.o
├── main_iso27001_test.o
├── main_iso27002_test.o
├── main_apps_test.o
├── main_sysaudit_test.o
├── scanner_core.o
├── parsers/
│   ├── file_parser_utils.o
│   ├── json_parser.o
│   ├── md_parser.o
│   └── pdf_parser.o
└── frameworks/
    ├── hipaa/
    │   ├── hipaa_checks.o
    │   ├── hipaa_loader.o
    │   └── hipaa_scanner.o
    ├── iso27001/
    ├── iso27002/
    ├── apps/
    └── sysaudit/
        ├── sysaudit_checks.o
        └── sysaudit_scanner.o
```

---

## Module Internals

### Scanner Core

**File:** `src/scanner_core.cpp`  
**Header:** `include/grc_scanner.hpp`

The core provides:
- `ScanResult` struct — holds `checkId`, `name`, `passed`, `severity`, `details`, `remediation`
- `ScanConfig` struct — holds the parsed key-value map from the config file
- `printResult()` — renders a single check result with colored box-drawing characters
- `printSummary()` — renders the compliance summary table with overall score

```cpp
struct ScanResult {
    std::string checkId;
    std::string name;
    bool        passed;
    std::string severity;   // CRITICAL / HIGH / MEDIUM / LOW / INFO
    std::string details;
    std::string remediation;
};

struct ScanConfig {
    std::map<std::string, std::string> values;
    bool hasKey(const std::string& key) const;
    std::string getValue(const std::string& key) const;
};
```

---

### File Parsers

**Directory:** `src/parsers/`  
**Header:** `include/parsers/file_parsers.hpp`

Each parser reads a file and populates a `ScanConfig` by extracting key-value pairs:

| Parser | File | Strategy |
|---|---|---|
| JSON | `json_parser.cpp` | Regex-based `"key": "value"` / `"key": true` extraction |
| Markdown | `md_parser.cpp` | Extracts from code blocks and `key: value` lines |
| YAML | `file_parser_utils.cpp` | Line-by-line `key: value` parsing |
| PDF | `pdf_parser.cpp` | Raw text extraction, then key-value scan |
| Plain text | `file_parser_utils.cpp` | Line-by-line `key: value` / `key = value` |

**Auto-detection** is done by file extension in `scanner_core.cpp`:

```cpp
if (ext == ".json")              return parseJSON(path, config);
if (ext == ".md" || ext == ".markdown") return parseMarkdown(path, config);
if (ext == ".yaml" || ext == ".yml")    return parseYAML(path, config);
if (ext == ".pdf")               return parsePDF(path, config);
return parsePlainText(path, config);  // fallback
```

---

### HIPAA Module

**Directory:** `src/frameworks/hipaa/`  
**Header:** `include/frameworks/hipaa.hpp`

| File | Responsibility |
|---|---|
| `hipaa_loader.cpp` | Loads built-in test configuration (hardcoded YAML string) |
| `hipaa_checks.cpp` | 8 check functions, one per HIPAA control |
| `hipaa_scanner.cpp` | Orchestrates checks, prints results, returns score |

**Check function signature:**
```cpp
ScanResult checkEncryptionAtRest(const ScanConfig& config);
ScanResult checkAuditControls(const ScanConfig& config);
// ... 6 more
```

Each check function looks for one or more keywords in `config.values` and returns a `ScanResult` with `passed = true/false`.

---

### ISO 27001 / ISO 27002 Modules

**Directories:** `src/frameworks/iso27001/`, `src/frameworks/iso27002/`

Same pattern as HIPAA. ISO 27001 has 11 checks (Annex A controls A.5–A.18). ISO 27002 extends with more granular controls.

---

### Application Security Module

**Directory:** `src/frameworks/apps/`  
**Header:** `include/frameworks/apps.hpp`

12 checks covering OWASP-aligned controls:

| Check ID | Control |
|---|---|
| APPS-AUTH-01 | Authentication Security |
| APPS-AUTHZ-01 | Authorization Controls |
| APPS-INPUT-01 | Input Validation |
| APPS-OUTPUT-01 | Output Encoding |
| APPS-SESSION-01 | Session Management |
| APPS-CRYPTO-01 | Cryptographic Controls |
| APPS-ERROR-01 | Secure Error Handling |
| APPS-LOG-01 | Security Logging & Monitoring |
| APPS-DATA-01 | Data Protection |
| APPS-API-01 | API Security |
| APPS-DEP-01 | Dependency Management |
| APPS-HEAD-01 | Security Headers |

---

### System Audit Module (new)

**Directory:** `src/frameworks/sysaudit/`  
**Header:** `include/frameworks/sysaudit.hpp`

Three sub-modules with 16 total checks:

#### Auto-Update Status (5 checks)

| Check ID | Keywords checked |
|---|---|
| SYSAUDIT-UPD-01 | `auto_update: enabled`, `automatic_updates: true`, `unattended_upgrades: enabled` |
| SYSAUDIT-UPD-02 | `security_updates_only: true`, `update_type: security` |
| SYSAUDIT-UPD-03 | `update_schedule`, `maintenance_window` |
| SYSAUDIT-UPD-04 | `reboot_after_update: true`, `auto_reboot` |
| SYSAUDIT-UPD-05 | `update_notifications: enabled`, `patch_alerts: true` |

#### SSH Key Audit (6 checks)

| Check ID | Keywords checked |
|---|---|
| SYSAUDIT-SSH-01 | `ssh_password_auth: disabled`, `password_authentication: false` |
| SYSAUDIT-SSH-02 | `ssh_root_login: disabled`, `permit_root_login: false` |
| SYSAUDIT-SSH-03 | `ssh_key_rotation: enabled`, `key_rotation_days` |
| SYSAUDIT-SSH-04 | `ssh_key_algorithm: ed25519`, `strong_ssh_keys: true` |
| SYSAUDIT-SSH-05 | `ssh_passphrase_required: true` |
| SYSAUDIT-SSH-06 | `authorized_keys_managed: true`, `ssh_ca: enabled` |

#### Unapproved Software (5 checks)

| Check ID | Keywords checked |
|---|---|
| SYSAUDIT-SW-01 | `software_whitelist: enabled`, `application_allowlist: true` |
| SYSAUDIT-SW-02 | `software_inventory: enabled`, `asset_inventory: maintained` |
| SYSAUDIT-SW-03 | `software_install_policy: restricted`, `admin_only_install: true` |
| SYSAUDIT-SW-04 | `software_scan: enabled`, `unapproved_software_scan: true` |
| SYSAUDIT-SW-05 | `software_removal_policy: defined`, `auto_remove_unapproved: true` |

**Scanner file:** `sysaudit_scanner.cpp`

```cpp
// Runs all 3 sub-modules and prints per-module + overall summary
void runSysauditScanner(const ScanConfig& config) {
    auto updateResults = runAutoUpdateChecks(config);
    auto sshResults    = runSSHKeyAuditChecks(config);
    auto swResults     = runUnapprovedSoftwareChecks(config);
    printSysauditSummary(updateResults, sshResults, swResults);
}
```

---

## Check Engine

Every check follows this pattern:

```cpp
ScanResult checkAutoUpdateEnabled(const ScanConfig& config) {
    ScanResult result;
    result.checkId = "SYSAUDIT-UPD-01";
    result.name    = "Auto-Update Enabled";
    result.severity = "HIGH";

    // Check for any matching keyword
    if (config.hasKey("auto_update") && config.getValue("auto_update") == "enabled" ||
        config.hasKey("automatic_updates") && config.getValue("automatic_updates") == "true" ||
        config.hasKey("unattended_upgrades") && config.getValue("unattended_upgrades") == "enabled") {
        result.passed  = true;
        result.details = "Automatic system updates are enabled";
    } else {
        result.passed      = false;
        result.details     = "Automatic system updates are NOT enabled";
        result.remediation = "Enable automatic updates (e.g. unattended-upgrades on Debian/Ubuntu...)";
    }
    return result;
}
```

**Scoring formula:**

```
score = (passed_checks / total_checks) * 100.0
```

Pass threshold: **≥ 70%** (configurable per module)

---

## Docker Internals

### Multi-stage Dockerfile

```
Stage 1: builder (ubuntu:22.04)
  ├── apt install g++ make
  ├── COPY source tree
  └── RUN make all  →  produces bin/complyd*

Stage 2: runtime (ubuntu:22.04)
  ├── apt install libstdc++6  (C++ runtime only, no compiler)
  ├── COPY --from=builder /build/bin/
  ├── COPY --from=builder /build/examples/
  ├── COPY --from=builder /build/tests/fixtures/
  └── CMD ["bin/complyd-sysaudit"]
```

**Why multi-stage?**
- Builder image: ~500 MB (includes g++, make, headers)
- Runtime image: ~80 MB (only binaries + libstdc++)
- The final image shipped to users is the small runtime image

### Image layers (runtime stage)

```
Layer 1: ubuntu:22.04 base
Layer 2: libstdc++6 install
Layer 3: WORKDIR /complyd
Layer 4: bin/ directory (compiled binaries)
Layer 5: examples/ directory
Layer 6: tests/fixtures/ directory
Layer 7: /scan directory (mount point)
```

### Volume mount for scanning local files

```bash
docker run --rm \
  -v /path/to/your/config.yaml:/scan/config.yaml \
  complyd bin/complyd /scan/config.yaml
```

The `/scan` directory inside the container is the designated mount point for user files.

### Multi-arch build (amd64 + arm64)

```bash
# Requires Docker Buildx (included in Docker Desktop)
docker buildx build \
  --platform linux/amd64,linux/arm64 \
  -t complyd:latest \
  --load \
  .
```

This produces a manifest list image that automatically selects the correct architecture on any machine.

---

## Data Flow

```
User runs: bin/complyd myconfig.yaml
                │
                ▼
        main.cpp: parse CLI args
                │
                ▼
        scanner_core.cpp: detect file type by extension
                │
                ▼
        parsers/yaml_parser.cpp: read file → ScanConfig{key→value map}
                │
                ▼
        ┌───────────────────────────────────────┐
        │  Run each framework scanner:           │
        │  hipaa_scanner.cpp    → []ScanResult  │
        │  sysaudit_scanner.cpp → []ScanResult  │
        │    ├── auto-update checks              │
        │    ├── ssh key audit checks            │
        │    └── unapproved software checks      │
        └───────────────────────────────────────┘
                │
                ▼
        scanner_core.cpp: aggregate scores
                │
                ▼
        Print OVERALL COMPLIANCE SUMMARY
        (HIPAA% + Auto-Update% + SSH% + Software%) / 4
```

---

## Adding a New Module

To add a new compliance framework (e.g. `PCI-DSS`):

### 1. Create the header

```cpp
// include/frameworks/pcidss.hpp
#pragma once
#include "grc_scanner.hpp"
#include <vector>

std::vector<ScanResult> runPCIDSSChecks(const ScanConfig& config);
```

### 2. Create the checks file

```cpp
// src/frameworks/pcidss/pcidss_checks.cpp
#include "frameworks/pcidss.hpp"

ScanResult checkFirewallConfig(const ScanConfig& config) {
    ScanResult r;
    r.checkId  = "PCIDSS-NET-01";
    r.name     = "Firewall Configuration";
    r.severity = "HIGH";
    r.passed   = config.hasKey("firewall") && config.getValue("firewall") == "enabled";
    r.details  = r.passed ? "Firewall is configured" : "Firewall is NOT configured";
    r.remediation = "Enable and configure a firewall...";
    return r;
}
```

### 3. Create the scanner file

```cpp
// src/frameworks/pcidss/pcidss_scanner.cpp
#include "frameworks/pcidss.hpp"

std::vector<ScanResult> runPCIDSSChecks(const ScanConfig& config) {
    return { checkFirewallConfig(config), /* more checks */ };
}
```

### 4. Add to Makefile

```makefile
CORE_SRCS += src/frameworks/pcidss/pcidss_checks.cpp \
             src/frameworks/pcidss/pcidss_scanner.cpp
```

### 5. Call from main.cpp

```cpp
#include "frameworks/pcidss.hpp"
// ...
auto pcidssResults = runPCIDSSChecks(config);
printResults(pcidssResults);
```

---

## Configuration File Format

The scanner accepts any file format. The key-value extraction is format-agnostic — it looks for patterns like:

```
key: value          # YAML / plain text
key = value         # INI / config files
"key": "value"      # JSON
"key": true         # JSON boolean
key: enabled        # custom
key: true/false     # boolean
```

### Full compliant YAML example

```yaml
# HIPAA
encryption: enabled
encrypt_at_rest: true
audit_log: enabled
mfa_enabled: true
tls: enabled
unique_user_id: true
backup_enabled: true
access_termination: automated
auto_logoff: enabled
session_timeout: 15

# Auto-Update
auto_update: enabled
automatic_updates: true
unattended_upgrades: enabled
security_updates_only: true
update_schedule: weekly
reboot_after_update: true
update_notifications: enabled

# SSH Key Audit
ssh_password_auth: disabled
password_authentication: false
ssh_root_login: disabled
permit_root_login: false
ssh_key_rotation: enabled
key_rotation_days: 90
ssh_key_algorithm: ed25519
ssh_passphrase_required: true
authorized_keys_managed: true
ssh_ca: enabled

# Unapproved Software
software_whitelist: enabled
application_allowlist: true
software_inventory: enabled
asset_inventory: maintained
software_install_policy: restricted
admin_only_install: true
software_scan: enabled
unapproved_software_scan: true
software_removal_policy: defined
auto_remove_unapproved: true
```

---

## Severity Levels

| Level | Color | Meaning |
|---|---|---|
| CRITICAL | Red | Must fix immediately — system is at high risk |
| HIGH | Orange | Fix as soon as possible |
| MEDIUM | Yellow | Fix within normal patch cycle |
| LOW | Blue | Best practice — fix when convenient |
| INFO | Green | Informational — check passed |

---

*Complyd Scanner — Technical Reference v1.0*  
*Copyright © 2026 Complyd. All rights reserved.*
