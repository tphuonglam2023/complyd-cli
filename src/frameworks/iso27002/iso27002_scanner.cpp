#include "frameworks/iso27002.hpp"
#include <cstdlib>
#include <cstring>

// Scan configuration against ISO 27002 compliance checks
iso27002_scan_result_t* iso27002_scan_config(const char *config_data) {
    if (!config_data) {
        return nullptr;
    }

    // Allocate scan result
    iso27002_scan_result_t *result = static_cast<iso27002_scan_result_t*>(calloc(1, sizeof(iso27002_scan_result_t)));
    if (!result) {
        return nullptr;
    }

    // Allocate results array (10 checks)
    result->results = static_cast<iso27002_check_result_t**>(calloc(10, sizeof(iso27002_check_result_t*)));
    if (!result->results) {
        free(result);
        return nullptr;
    }

    result->result_count = 0;
    result->passed_count = 0;
    result->failed_count = 0;

    // Check 1: Asset Management
    int check1 = iso27002_check_asset_management(config_data);
    result->results[result->result_count++] = create_iso27002_asset_result(check1, nullptr);
    if (check1) result->passed_count++; else result->failed_count++;

    // Check 2: Human Resources Security
    int check2 = iso27002_check_human_resources_security(config_data);
    result->results[result->result_count++] = create_iso27002_hr_security_result(check2, nullptr);
    if (check2) result->passed_count++; else result->failed_count++;

    // Check 3: Security Awareness
    int check3 = iso27002_check_security_awareness(config_data);
    result->results[result->result_count++] = create_iso27002_awareness_result(check3, nullptr);
    if (check3) result->passed_count++; else result->failed_count++;

    // Check 4: Vulnerability Management
    int check4 = iso27002_check_vulnerability_management(config_data);
    result->results[result->result_count++] = create_iso27002_vulnerability_result(check4, nullptr);
    if (check4) result->passed_count++; else result->failed_count++;

    // Check 5: Malware Protection
    int check5 = iso27002_check_malware_protection(config_data);
    result->results[result->result_count++] = create_iso27002_malware_result(check5, nullptr);
    if (check5) result->passed_count++; else result->failed_count++;

    // Check 6: Logging and Monitoring
    int check6 = iso27002_check_logging_monitoring(config_data);
    result->results[result->result_count++] = create_iso27002_logging_result(check6, nullptr);
    if (check6) result->passed_count++; else result->failed_count++;

    // Check 7: Secure Coding
    int check7 = iso27002_check_secure_coding(config_data);
    result->results[result->result_count++] = create_iso27002_coding_result(check7, nullptr);
    if (check7) result->passed_count++; else result->failed_count++;

    // Check 8: Change Control
    int check8 = iso27002_check_change_control(config_data);
    result->results[result->result_count++] = create_iso27002_change_result(check8, nullptr);
    if (check8) result->passed_count++; else result->failed_count++;

    // Check 9: Capacity Planning
    int check9 = iso27002_check_capacity_planning(config_data);
    result->results[result->result_count++] = create_iso27002_capacity_result(check9, nullptr);
    if (check9) result->passed_count++; else result->failed_count++;

    // Check 10: Cloud Security
    int check10 = iso27002_check_cloud_security(config_data);
    result->results[result->result_count++] = create_iso27002_cloud_result(check10, nullptr);
    if (check10) result->passed_count++; else result->failed_count++;

    return result;
}

// Free scan result
void free_iso27002_scan_result(iso27002_scan_result_t *result) {
    if (!result) return;

    if (result->results) {
        for (size_t i = 0; i < result->result_count; i++) {
            free_iso27002_check_result(result->results[i]);
        }
        free(result->results);
    }

    free(result);
}
