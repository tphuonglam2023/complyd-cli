# New Compliance Frameworks

This document describes the three new compliance frameworks that have been added to the GRC Scanner:
- **ISO 27001** - Information Security Management System (ISMS)
- **ISO 27002** - Code of Practice for Information Security Controls
- **APPs** - Application Security Best Practices

## Overview

All frameworks follow the same architecture pattern as HIPAA, providing:
- Header files with type definitions and function declarations
- Check functions that validate configuration against specific controls
- Scanner functions that run all checks and aggregate results
- Remediation guidance for failed checks

---

## ISO 27001 Framework

**Location:**
- Header: `include/frameworks/iso27001.h`
- Implementation: `src/frameworks/iso27001/`

**Total Controls: 11**

### Controls Implemented

| Control ID | Control Name | Severity | Description |
|------------|--------------|----------|-------------|
| A.5.1 | Information Security Policies | HIGH | Policies for information security management |
| A.9.1 | Access Control Policy | CRITICAL | Access control based on business requirements |
| A.10.1 | Cryptographic Controls | HIGH | Policy on cryptography and key management |
| A.11.1 | Physical and Environmental Security | MEDIUM | Physical security perimeters and controls |
| A.12.1 | Operations Security | MEDIUM | Operational procedures and responsibilities |
| A.13.1 | Network Security Management | HIGH | Network controls and segregation |
| A.14.1 | Security in Development | MEDIUM | Security requirements in SDLC |
| A.15.1 | Supplier Relationships | MEDIUM | Security in supplier relationships |
| A.16.1 | Incident Management | HIGH | Management of information security incidents |
| A.17.1 | Business Continuity | HIGH | Business continuity planning |
| A.18.1 | Compliance | HIGH | Compliance with legal requirements |

### Configuration Keys

The scanner checks for the following configuration patterns:
- `security_policy: defined`
- `access_control: enforced`
- `encryption: enabled`
- `physical_security: enabled`
- `operations_procedures: documented`
- `network_security: enabled`
- `security_requirements: defined`
- `supplier_security: managed`
- `incident_response: enabled`
- `business_continuity: planned`
- `compliance: monitored`

---

## ISO 27002 Framework

**Location:**
- Header: `include/frameworks/iso27002.h`
- Implementation: `src/frameworks/iso27002/`

**Total Controls: 10**

### Controls Implemented

| Control ID | Control Name | Severity | Description |
|------------|--------------|----------|-------------|
| 5.9 | Inventory of Information and Assets | MEDIUM | Asset management and inventory |
| 6.1 | Screening | MEDIUM | Background verification checks |
| 6.3 | Information Security Awareness | MEDIUM | Security awareness and training |
| 8.8 | Management of Technical Vulnerabilities | HIGH | Vulnerability management |
| 8.7 | Protection Against Malware | HIGH | Malware detection and prevention |
| 8.15 | Logging | HIGH | Event logging and monitoring |
| 8.28 | Secure Coding | MEDIUM | Secure coding practices |
| 8.32 | Change Management | MEDIUM | Change control procedures |
| 8.6 | Capacity Management | LOW | Resource capacity planning |
| 5.23 | Information Security for Cloud Services | HIGH | Cloud security controls |

### Configuration Keys

The scanner checks for the following configuration patterns:
- `asset_inventory: maintained`
- `background_checks: enabled`
- `security_training: required`
- `vulnerability_scanning: enabled`
- `antivirus: enabled`
- `logging: enabled`
- `secure_coding: enforced`
- `change_control: enforced`
- `capacity_planning: enabled`
- `cloud_security: enabled`

---

## APPs (Application Security) Framework

**Location:**
- Header: `include/frameworks/apps.h`
- Implementation: `src/frameworks/apps/`

**Total Controls: 12**

### Controls Implemented

| Control ID | Control Name | Severity | Description |
|------------|--------------|----------|-------------|
| APPS-AUTH-01 | Authentication Security | CRITICAL | Strong authentication mechanisms |
| APPS-AUTHZ-01 | Authorization Controls | CRITICAL | Role-based access control |
| APPS-INPUT-01 | Input Validation | HIGH | Input validation and sanitization |
| APPS-OUTPUT-01 | Output Encoding | HIGH | Context-aware output encoding |
| APPS-SESSION-01 | Session Management | HIGH | Secure session management |
| APPS-CRYPTO-01 | Cryptographic Controls | CRITICAL | Strong cryptography |
| APPS-ERROR-01 | Secure Error Handling | MEDIUM | Secure error handling |
| APPS-LOG-01 | Security Logging and Monitoring | HIGH | Security logging |
| APPS-DATA-01 | Data Protection | HIGH | Data classification and protection |
| APPS-API-01 | API Security | HIGH | API authentication and security |
| APPS-DEP-01 | Dependency Management | HIGH | Dependency scanning |
| APPS-HEAD-01 | Security Headers | MEDIUM | HTTP security headers |

### Configuration Keys

The scanner checks for the following configuration patterns:
- `strong_authentication: enabled`
- `rbac: enabled`
- `input_validation: enabled`
- `output_encoding: enabled`
- `secure_sessions: enabled`
- `strong_crypto: enabled`
- `secure_error_handling: enabled`
- `security_logging: enabled`
- `data_classification: defined`
- `api_authentication: enforced`
- `dependency_scanning: enabled`
- `security_headers: enabled`

---

## Usage

### Building with New Frameworks

The Makefile has been updated to automatically include all frameworks:

```bash
make clean
make all
```

### Using the Frameworks in Code

```c
#include "frameworks/iso27001.h"
#include "frameworks/iso27002.h"
#include "frameworks/apps.h"

// Example: ISO 27001 scan
iso27001_scan_result_t* results = iso27001_scan_config(config_data);
printf("ISO 27001: %zu passed, %zu failed\n", 
       results->passed_count, results->failed_count);
free_iso27001_scan_result(results);

// Example: ISO 27002 scan
iso27002_scan_result_t* results2 = iso27002_scan_config(config_data);
printf("ISO 27002: %zu passed, %zu failed\n", 
       results2->passed_count, results2->failed_count);
free_iso27002_scan_result(results2);

// Example: APPs scan
apps_scan_result_t* results3 = apps_scan_config(config_data);
printf("Application Security: %zu passed, %zu failed\n", 
       results3->passed_count, results3->failed_count);
free_apps_scan_result(results3);
```

---

## Framework Summary

| Framework | Controls | Focus Area | Compliance Level |
|-----------|----------|------------|------------------|
| **HIPAA** | 8 | Healthcare data protection | Regulatory |
| **ISO 27001** | 11 | Information security management | International Standard |
| **ISO 27002** | 10 | Security control implementation | Best Practice Guide |
| **APPs** | 12 | Application security | Security Best Practices |
| **Total** | **41** | **Comprehensive GRC Coverage** | - |

---

## Architecture

All frameworks follow the same consistent architecture:

### File Structure
```
include/frameworks/
├── [framework].h           # Public API and types
src/frameworks/[framework]/
├── [framework]_checks.c    # Individual control checks
└── [framework]_scanner.c   # Orchestrates all checks
```

### Key Components

1. **Check Functions**: Individual validation functions for each control
2. **Result Creators**: Generate structured results with details and remediation
3. **Scanner Function**: Runs all checks and aggregates results
4. **Cleanup Functions**: Memory management for results

---

## Next Steps

To extend the framework:

1. Add new check functions in `*_checks.c`
2. Add corresponding result creators
3. Update the scanner in `*_scanner.c` to include new checks
4. Update header file with new function declarations
5. Rebuild with `make clean && make all`

---

## Testing

Test your configuration files:

```bash
# Test with JSON configuration
./complyd-scan examples/json/test-config.json

# Test with Markdown configuration
./complyd-scan examples/markdown/security-config.md

# Run HIPAA test suite
./complyd-scan-hipaa
```

---

## References

- **ISO/IEC 27001:2022** - Information security management systems - Requirements
- **ISO/IEC 27002:2022** - Information security controls
- **OWASP Top 10** - Web Application Security Risks
- **NIST Cybersecurity Framework** - Framework for Improving Critical Infrastructure Cybersecurity

---

*Document generated: January 19, 2026*
*GRC Scanner Version: 2.0 with Multi-Framework Support*
