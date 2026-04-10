#include "frameworks/iso27001.hpp"
#include <cstdlib>
#include <cstring>

// Scan configuration against ISO 27001 compliance checks
iso27001_scan_result_t* iso27001_scan_config(const char *config_data) {
    if (!config_data) {
        return nullptr;
    }

    // Allocate scan result
    iso27001_scan_result_t *result = static_cast<iso27001_scan_result_t*>(calloc(1, sizeof(iso27001_scan_result_t)));
    if (!result) {
        return nullptr;
    }

    // Allocate results array (11 checks)
    result->results = static_cast<iso27001_check_result_t**>(calloc(11, sizeof(iso27001_check_result_t*)));
    if (!result->results) {
        free(result);
        return nullptr;
    }

    result->result_count = 0;
    result->passed_count = 0;
    result->failed_count = 0;

    // Check 1: Information Security Policies
    int check1 = iso27001_check_information_security_policies(config_data);
    result->results[result->result_count++] = create_iso27001_policies_result(check1, nullptr);
    if (check1) result->passed_count++; else result->failed_count++;

    // Check 2: Access Control Policy
    int check2 = iso27001_check_access_control_policy(config_data);
    result->results[result->result_count++] = create_iso27001_access_control_result(check2, nullptr);
    if (check2) result->passed_count++; else result->failed_count++;

    // Check 3: Cryptographic Controls
    int check3 = iso27001_check_cryptographic_controls(config_data);
    result->results[result->result_count++] = create_iso27001_crypto_result(check3, nullptr);
    if (check3) result->passed_count++; else result->failed_count++;

    // Check 4: Physical Security
    int check4 = iso27001_check_physical_security(config_data);
    result->results[result->result_count++] = create_iso27001_physical_result(check4, nullptr);
    if (check4) result->passed_count++; else result->failed_count++;

    // Check 5: Operations Security
    int check5 = iso27001_check_operations_security(config_data);
    result->results[result->result_count++] = create_iso27001_operations_result(check5, nullptr);
    if (check5) result->passed_count++; else result->failed_count++;

    // Check 6: Network Security
    int check6 = iso27001_check_network_security(config_data);
    result->results[result->result_count++] = create_iso27001_network_result(check6, nullptr);
    if (check6) result->passed_count++; else result->failed_count++;

    // Check 7: System Acquisition
    int check7 = iso27001_check_system_acquisition(config_data);
    result->results[result->result_count++] = create_iso27001_acquisition_result(check7, nullptr);
    if (check7) result->passed_count++; else result->failed_count++;

    // Check 8: Supplier Relationships
    int check8 = iso27001_check_supplier_relationships(config_data);
    result->results[result->result_count++] = create_iso27001_supplier_result(check8, nullptr);
    if (check8) result->passed_count++; else result->failed_count++;

    // Check 9: Incident Management
    int check9 = iso27001_check_incident_management(config_data);
    result->results[result->result_count++] = create_iso27001_incident_result(check9, nullptr);
    if (check9) result->passed_count++; else result->failed_count++;

    // Check 10: Business Continuity
    int check10 = iso27001_check_business_continuity(config_data);
    result->results[result->result_count++] = create_iso27001_continuity_result(check10, nullptr);
    if (check10) result->passed_count++; else result->failed_count++;

    // Check 11: Compliance
    int check11 = iso27001_check_compliance(config_data);
    result->results[result->result_count++] = create_iso27001_compliance_result(check11, nullptr);
    if (check11) result->passed_count++; else result->failed_count++;

    return result;
}

// Free scan result
void free_iso27001_scan_result(iso27001_scan_result_t *result) {
    if (!result) return;

    if (result->results) {
        for (size_t i = 0; i < result->result_count; i++) {
            free_iso27001_check_result(result->results[i]);
        }
        free(result->results);
    }

    free(result);
}
