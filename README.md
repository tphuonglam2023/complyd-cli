# Complyd Scanner

**Automated Compliance Scanning Tool by Complyd**

Complyd Scanner is a powerful command-line tool designed to automate compliance verification for HIPAA, ISO 27001, ISO 27002, Application Security, and System Audit frameworks. Built to streamline security audits and compliance checks for modern infrastructures.

## About Complyd

Complyd is a compliance automation company dedicated to simplifying security and regulatory compliance for organizations of all sizes. Our tools help teams achieve and maintain compliance efficiently.

---

## Features

- ✅ **HIPAA Compliance Checks** – 8 comprehensive security controls
- 🔐 **ISO 27001 / ISO 27002** – Information security management controls
- 📱 **Application Security (APPs)** – 12 secure development best-practice checks
- 🔄 **Auto-Update Status** – 5 system patching & update policy checks *(new)*
- 🔑 **SSH Key Audit** – 6 SSH hardening & key management checks *(new)*
- 🚫 **Unapproved Software** – 5 software allowlist & inventory checks *(new)*
- 📄 **Multiple File Format Support** – JSON, Markdown, YAML, PDF, and text files
- 📊 **Compliance Scoring** – Clear pass/fail criteria with remediation guidance
- 🚀 **Easy Integration** – Simple CLI interface for CI/CD pipelines

---

## Supported Compliance Frameworks

| Framework | Binary | Checks |
|---|---|---|
| HIPAA | `bin/complyd-hipaa` | 8 |
| ISO 27001 | `bin/complyd-iso27001` | varies |
| ISO 27002 | `bin/complyd-iso27002` | varies |
| Application Security (APPs) | `bin/complyd-apps` | 12 |
| **Auto-Update Status** *(new)* | `bin/complyd-sysaudit` | 5 |
| **SSH Key Audit** *(new)* | `bin/complyd-sysaudit` | 6 |
| **Unapproved Software** *(new)* | `bin/complyd-sysaudit` | 5 |
| **All-in-one (HIPAA + 3 new)** | `bin/complyd` | 24 |

---

## Requirements

- **g++** (C++17 or later)
- **make**

```bash
# Ubuntu / Debian
sudo apt install g++ make

# RHEL / CentOS / Fedora
sudo dnf install gcc-c++ make
```

---

## Quick Start

### 1. Clone the repository

```bash
git clone https://github.com/tphuonglam2023/complyd-cli.git
cd complyd-cli
```

### 2. Build all binaries

```bash
make all
```

This produces the following binaries inside `bin/`:

| Binary | Description |
|---|---|
| `bin/complyd` | Main scanner – HIPAA + Auto-Update + SSH Keys + Unapproved Software |
| `bin/complyd-hipaa` | HIPAA-only scanner |
| `bin/complyd-iso27001` | ISO 27001 scanner |
| `bin/complyd-iso27002` | ISO 27002 scanner |
| `bin/complyd-apps` | Application Security scanner |
| `bin/complyd-sysaudit` | System Audit scanner (all 3 new modules) |

### 3. Run the System Audit scanner (new modules)

```bash
bin/complyd-sysaudit
```

No arguments needed – it uses built-in test configurations and shows results for all three new modules.

### 4. Run the main scanner against a config file

```bash
bin/complyd <config-file>
```

Examples:

```bash
bin/complyd examples/markdown/security-config.md
bin/complyd tests/fixtures/compliant/config-full-compliant.yaml
bin/complyd tests/fixtures/compliant/config-full-compliant.md
```

The main scanner runs **4 sections** in sequence and produces an overall score:
1. HIPAA Compliance
2. Auto-Update Status *(new)*
3. SSH Key Audit *(new)*
4. Unapproved Software *(new)*

### 5. Run all tests at once

```bash
make test
```

Runs all 5 test binaries in sequence: HIPAA → ISO 27001 → ISO 27002 → Apps Security → System Audit.

### 6. Clean and rebuild

```bash
make clean && make all
```

---

## Running Individual Scanners

```bash
# HIPAA only
bin/complyd-hipaa

# ISO 27001 only
bin/complyd-iso27001

# ISO 27002 only
bin/complyd-iso27002

# Application Security only
bin/complyd-apps

# System Audit (Auto-Update + SSH Keys + Unapproved Software)
bin/complyd-sysaudit

# All-in-one with a real config file
bin/complyd <your-config-file>
```

---

## HIPAA Compliance Checks (8 checks)

| Control ID | Name | Severity |
|---|---|---|
| 164.312(a)(2)(iv) | Encryption at Rest | HIGH |
| 164.312(b) | Audit Controls | HIGH |
| 164.312(d) | Multi-Factor Authentication | CRITICAL |
| 164.312(e)(2)(ii) | Encryption in Transit | HIGH |
| 164.312(a)(2)(i) | Unique User Identification | MEDIUM |
| 164.308(a)(7)(ii)(A) | Data Backup | HIGH |
| 164.308(a)(3)(ii)(C) | Access Termination | MEDIUM |
| 164.312(a)(2)(iii) | Automatic Logoff | LOW |

---

## Auto-Update Status Checks (5 checks) — *new*

| Control ID | Name | Severity |
|---|---|---|
| SYSAUDIT-UPD-01 | Auto-Update Enabled | HIGH |
| SYSAUDIT-UPD-02 | Security-Only Update Policy | HIGH |
| SYSAUDIT-UPD-03 | Update Schedule Defined | MEDIUM |
| SYSAUDIT-UPD-04 | Post-Update Reboot Policy | MEDIUM |
| SYSAUDIT-UPD-05 | Update Notifications Enabled | LOW |

**Config keywords that trigger PASS:**

```yaml
auto_update: enabled
automatic_updates: true
unattended_upgrades: enabled
security_updates_only: true
update_type: security
update_schedule: weekly
maintenance_window: sunday-02:00
reboot_after_update: true
auto_reboot: scheduled
update_notifications: enabled
patch_alerts: true
```

---

## SSH Key Audit Checks (6 checks) — *new*

| Control ID | Name | Severity |
|---|---|---|
| SYSAUDIT-SSH-01 | SSH Key-Only Authentication | CRITICAL |
| SYSAUDIT-SSH-02 | SSH Root Login Disabled | CRITICAL |
| SYSAUDIT-SSH-03 | SSH Key Rotation Policy | HIGH |
| SYSAUDIT-SSH-04 | Strong SSH Key Algorithm | HIGH |
| SYSAUDIT-SSH-05 | SSH Key Passphrase Required | MEDIUM |
| SYSAUDIT-SSH-06 | Centralized SSH Authorized Keys Management | MEDIUM |

**Config keywords that trigger PASS:**

```yaml
ssh_password_auth: disabled
password_authentication: false
ssh_root_login: disabled
permit_root_login: false
ssh_key_rotation: enabled
key_rotation_days: 90
ssh_key_algorithm: ed25519
strong_ssh_keys: true
ssh_passphrase_required: true
authorized_keys_managed: true
ssh_ca: enabled
```

---

## Unapproved Software Checks (5 checks) — *new*

| Control ID | Name | Severity |
|---|---|---|
| SYSAUDIT-SW-01 | Software Whitelist / Allowlist | HIGH |
| SYSAUDIT-SW-02 | Software Inventory Maintained | HIGH |
| SYSAUDIT-SW-03 | Software Installation Policy | HIGH |
| SYSAUDIT-SW-04 | Automated Software Compliance Scanning | MEDIUM |
| SYSAUDIT-SW-05 | Unapproved Software Removal Policy | MEDIUM |

**Config keywords that trigger PASS:**

```yaml
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

## Supported File Formats

| Extension | Format |
|---|---|
| `.json` | JSON |
| `.md`, `.markdown` | Markdown |
| `.yaml`, `.yml` | YAML |
| `.pdf` | PDF |
| `.txt`, `.conf`, `.config` | Plain text |

---

## Example Output

### System Audit Scanner (`bin/complyd-sysaudit`)

```
  ███████╗██╗   ██╗███████╗ █████╗ ██╗   ██╗██████╗ ██╗████████╗
  ...
        System Audit Scanner – 3 Security Modules
  Auto-Update Status | SSH Key Audit | Unapproved Software

================================================================================
MODULE 1 – AUTO-UPDATE STATUS
================================================================================
┌─ [PASS] SYSAUDIT-UPD-01 - Auto-Update Enabled
│  Severity: INFO
│  Details: Automatic system updates are enabled
└─────────────────────────────────────────────────────────────────
...
  Total: 5  |  Passed: 5  |  Failed: 0  |  Score: 100.0%

================================================================================
SYSTEM AUDIT OVERALL SUMMARY
================================================================================
  Auto-Update Status Score:           100.0%
  SSH Key Audit Score:                100.0%
  Unapproved Software Score:          100.0%

  ✓ PASSED – All system audit modules meet the security threshold
```

### Main Scanner (`bin/complyd <file>`)

```
Scanning file: config.yaml
File type: YAML
✓ Successfully parsed file (512 bytes)

================================================================================
RUNNING HIPAA COMPLIANCE CHECKS
...
  HIPAA Score: 100.0%  (8 passed / 8 total)

================================================================================
RUNNING AUTO-UPDATE STATUS CHECKS
...
  Auto-Update Score: 100.0%  (5 passed / 5 total)

================================================================================
RUNNING SSH KEY AUDIT CHECKS
...
  SSH Key Audit Score: 100.0%  (6 passed / 6 total)

================================================================================
RUNNING UNAPPROVED SOFTWARE CHECKS
...
  Unapproved Software Score: 100.0%  (5 passed / 5 total)

================================================================================
OVERALL COMPLIANCE SUMMARY
================================================================================
  HIPAA Compliance:              100.0%
  Auto-Update Status:            100.0%
  SSH Key Audit:                 100.0%
  Unapproved Software:           100.0%

  Overall Score:                 100.0%

  ✓ PASSED - Configuration meets all compliance requirements
```

---

## Project Structure

```
complyd-cli/
├── src/
│   ├── main.cpp                        # Main scanner (all modules)
│   ├── main_hipaa_test.cpp             # HIPAA standalone test
│   ├── main_iso27001_test.cpp          # ISO 27001 standalone test
│   ├── main_iso27002_test.cpp          # ISO 27002 standalone test
│   ├── main_apps_test.cpp              # Apps Security standalone test
│   ├── main_sysaudit_test.cpp          # System Audit standalone test (new)
│   ├── scanner_core.cpp
│   ├── frameworks/
│   │   ├── hipaa/
│   │   │   ├── hipaa_checks.cpp
│   │   │   ├── hipaa_loader.cpp
│   │   │   └── hipaa_scanner.cpp
│   │   ├── iso27001/
│   │   ├── iso27002/
│   │   ├── apps/
│   │   └── sysaudit/                   # NEW
│   │       ├── sysaudit_checks.cpp     # 16 check functions
│   │       └── sysaudit_scanner.cpp    # Scanner orchestration
│   └── parsers/
├── include/
│   ├── grc_scanner.hpp
│   ├── frameworks/
│   │   ├── hipaa.hpp
│   │   ├── iso27001.hpp
│   │   ├── iso27002.hpp
│   │   ├── apps.hpp
│   │   └── sysaudit.hpp                # NEW
│   └── parsers/
├── bin/                                # Compiled binaries (after make)
│   ├── complyd
│   ├── complyd-hipaa
│   ├── complyd-iso27001
│   ├── complyd-iso27002
│   ├── complyd-apps
│   └── complyd-sysaudit                # NEW
├── tests/
│   └── fixtures/
├── examples/
└── Makefile
```

---

## Docker — Run on Any Platform

Docker is the recommended way to run Complyd Scanner on **any OS** (Ubuntu, macOS, Windows) without installing a compiler.

### Prerequisites

| Platform | Install |
|---|---|
| Ubuntu / Debian | `sudo apt install docker.io docker-compose-plugin` |
| macOS (Intel & Apple Silicon) | [Docker Desktop for Mac](https://www.docker.com/products/docker-desktop/) |
| Windows | [Docker Desktop for Windows](https://www.docker.com/products/docker-desktop/) |

---

### Step 1 — Build the image

```bash
# Clone the repo (if you haven't already)
git clone https://github.com/tphuonglam2023/complyd-cli.git
cd complyd-cli

# Build the Docker image (compiles everything inside the container)
make docker
# or directly:
docker build -t complyd .
```

> **Apple Silicon (M1/M2/M3)?** Use the multi-arch build to get a native ARM64 image:
> ```bash
> make docker-multiarch
> ```

---

### Step 2 — Run the scanners

```bash
# System Audit (Auto-Update + SSH Key Audit + Unapproved Software)
docker run --rm complyd

# HIPAA only
docker run --rm complyd bin/complyd-hipaa

# ISO 27001 only
docker run --rm complyd bin/complyd-iso27001

# ISO 27002 only
docker run --rm complyd bin/complyd-iso27002

# Application Security only
docker run --rm complyd bin/complyd-apps

# Run all scanners in sequence
make docker-test
```

---

### Step 3 — Scan your own config file

Mount a local file into the container using `-v`:

```bash
# Scan a local YAML file
docker run --rm \
  -v $(pwd)/myconfig.yaml:/scan/myconfig.yaml \
  complyd bin/complyd /scan/myconfig.yaml

# Scan a local Markdown file
docker run --rm \
  -v $(pwd)/security-policy.md:/scan/security-policy.md \
  complyd bin/complyd /scan/security-policy.md
```

---

### Using Docker Compose (easier)

```bash
# Run System Audit scanner
docker compose run sysaudit

# Run HIPAA scanner
docker compose run hipaa

# Scan a local config file (path inside container is /scan/<filename>)
docker compose run scanner /scan/examples/markdown/security-config.md
```

---

### Transfer the image offline (no internet needed)

```bash
# On the source machine — save image to a file
make docker-save
# produces: complyd-docker.tar.gz

# Copy the file to the target machine (scp, USB, etc.)
scp complyd-docker.tar.gz user@target-machine:~/

# On the target machine — load and run
docker load < complyd-docker.tar.gz
docker run --rm complyd
```

---

### Docker compatibility matrix

| Platform | Architecture | Works? |
|---|---|---|
| Ubuntu 20.04 / 22.04 / 24.04 | x86_64 (amd64) | ✅ |
| macOS Intel | x86_64 (amd64) | ✅ |
| macOS Apple Silicon M1/M2/M3 | arm64 | ✅ (use `make docker-multiarch`) |
| Windows 10/11 (Docker Desktop) | x86_64 (amd64) | ✅ |
| Raspberry Pi / ARM servers | arm64 | ✅ (use `make docker-multiarch`) |

---

## CI/CD Integration

### GitHub Actions

```yaml
- name: Run Complyd Scanner
  run: |
    make all
    bin/complyd config.yaml
    bin/complyd-sysaudit
```

### GitLab CI

```yaml
compliance_check:
  script:
    - make all
    - bin/complyd config.yaml
    - bin/complyd-sysaudit
```

---

## Contributing

We welcome contributions! Please see our [Contributing Guidelines](CONTRIBUTING.md) for details.

---

## License

Copyright © 2026 Complyd. All rights reserved.

## Support

For questions, issues, or feature requests:
- 📧 Email: support@complyd.com
- 🐛 Issues: [GitHub Issues](https://github.com/tphuonglam2023/complyd-cli/issues)
- 📖 Documentation: [docs.complyd.com](https://docs.complyd.com)

---

**Complyd** - Simplifying Compliance.
