#include "frameworks/apps.hpp"
#include <cstdlib>
#include <cstring>

// Scan configuration against Application Security compliance checks
apps_scan_result_t* apps_scan_config(const char *config_data) {
    if (!config_data) {
        return nullptr;
    }

    // Allocate scan result
    apps_scan_result_t *result = static_cast<apps_scan_result_t*>(calloc(1, sizeof(apps_scan_result_t)));
    if (!result) {
        return nullptr;
    }

    // Allocate results array (12 checks)
    result->results = static_cast<apps_check_result_t**>(calloc(12, sizeof(apps_check_result_t*)));
    if (!result->results) {
        free(result);
        return nullptr;
    }

    result->result_count = 0;
    result->passed_count = 0;
    result->failed_count = 0;

    // Check 1: Authentication Security
    int check1 = apps_check_authentication_security(config_data);
    result->results[result->result_count++] = create_apps_authentication_result(check1, nullptr);
    if (check1) result->passed_count++; else result->failed_count++;

    // Check 2: Authorization Controls
    int check2 = apps_check_authorization_controls(config_data);
    result->results[result->result_count++] = create_apps_authorization_result(check2, nullptr);
    if (check2) result->passed_count++; else result->failed_count++;

    // Check 3: Input Validation
    int check3 = apps_check_input_validation(config_data);
    result->results[result->result_count++] = create_apps_input_validation_result(check3, nullptr);
    if (check3) result->passed_count++; else result->failed_count++;

    // Check 4: Output Encoding
    int check4 = apps_check_output_encoding(config_data);
    result->results[result->result_count++] = create_apps_output_encoding_result(check4, nullptr);
    if (check4) result->passed_count++; else result->failed_count++;

    // Check 5: Session Management
    int check5 = apps_check_session_management(config_data);
    result->results[result->result_count++] = create_apps_session_result(check5, nullptr);
    if (check5) result->passed_count++; else result->failed_count++;

    // Check 6: Cryptography
    int check6 = apps_check_cryptography(config_data);
    result->results[result->result_count++] = create_apps_crypto_result(check6, nullptr);
    if (check6) result->passed_count++; else result->failed_count++;

    // Check 7: Error Handling
    int check7 = apps_check_error_handling(config_data);
    result->results[result->result_count++] = create_apps_error_result(check7, nullptr);
    if (check7) result->passed_count++; else result->failed_count++;

    // Check 8: Logging and Monitoring
    int check8 = apps_check_logging_monitoring(config_data);
    result->results[result->result_count++] = create_apps_logging_result(check8, nullptr);
    if (check8) result->passed_count++; else result->failed_count++;

    // Check 9: Data Protection
    int check9 = apps_check_data_protection(config_data);
    result->results[result->result_count++] = create_apps_data_protection_result(check9, nullptr);
    if (check9) result->passed_count++; else result->failed_count++;

    // Check 10: API Security
    int check10 = apps_check_api_security(config_data);
    result->results[result->result_count++] = create_apps_api_result(check10, nullptr);
    if (check10) result->passed_count++; else result->failed_count++;

    // Check 11: Dependency Management
    int check11 = apps_check_dependency_management(config_data);
    result->results[result->result_count++] = create_apps_dependency_result(check11, nullptr);
    if (check11) result->passed_count++; else result->failed_count++;

    // Check 12: Security Headers
    int check12 = apps_check_security_headers(config_data);
    result->results[result->result_count++] = create_apps_headers_result(check12, nullptr);
    if (check12) result->passed_count++; else result->failed_count++;

    return result;
}

// Free scan result
void free_apps_scan_result(apps_scan_result_t *result) {
    if (!result) return;

    if (result->results) {
        for (size_t i = 0; i < result->result_count; i++) {
            free_apps_check_result(result->results[i]);
        }
        free(result->results);
    }

    free(result);
}
