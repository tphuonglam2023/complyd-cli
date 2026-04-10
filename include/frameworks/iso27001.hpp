#ifndef ISO27001_HPP
#define ISO27001_HPP

#include <cstddef>

// ISO 27001 Control structure
struct iso27001_control_t {
    char *id;
    char *name;
    char *description;
    char *category;
};

// ISO 27001 Framework structure
struct iso27001_framework_t {
    iso27001_control_t *controls;
    size_t control_count;
    size_t control_capacity;
};

// Check result structure
struct iso27001_check_result_t {
    bool passed;
    char *control_id;
    char *control_name;
    const char *severity;  // "CRITICAL", "HIGH", "MEDIUM", "LOW", "INFO"
    char *details;
    char *remediation;
};

// Scan result structure
struct iso27001_scan_result_t {
    iso27001_check_result_t **results;
    size_t result_count;
    size_t passed_count;
    size_t failed_count;
};

// Framework loader functions
iso27001_framework_t* iso27001_load_framework(const char *yaml_file);
void iso27001_free_framework(iso27001_framework_t *framework);

// Check functions - Annex A Controls
int iso27001_check_information_security_policies(const char *config_data);
int iso27001_check_access_control_policy(const char *config_data);
int iso27001_check_cryptographic_controls(const char *config_data);
int iso27001_check_physical_security(const char *config_data);
int iso27001_check_operations_security(const char *config_data);
int iso27001_check_network_security(const char *config_data);
int iso27001_check_system_acquisition(const char *config_data);
int iso27001_check_supplier_relationships(const char *config_data);
int iso27001_check_incident_management(const char *config_data);
int iso27001_check_business_continuity(const char *config_data);
int iso27001_check_compliance(const char *config_data);

// Result creator functions
iso27001_check_result_t* create_iso27001_policies_result(int passed, const char *details);
iso27001_check_result_t* create_iso27001_access_control_result(int passed, const char *details);
iso27001_check_result_t* create_iso27001_crypto_result(int passed, const char *details);
iso27001_check_result_t* create_iso27001_physical_result(int passed, const char *details);
iso27001_check_result_t* create_iso27001_operations_result(int passed, const char *details);
iso27001_check_result_t* create_iso27001_network_result(int passed, const char *details);
iso27001_check_result_t* create_iso27001_acquisition_result(int passed, const char *details);
iso27001_check_result_t* create_iso27001_supplier_result(int passed, const char *details);
iso27001_check_result_t* create_iso27001_incident_result(int passed, const char *details);
iso27001_check_result_t* create_iso27001_continuity_result(int passed, const char *details);
iso27001_check_result_t* create_iso27001_compliance_result(int passed, const char *details);

// Scanner function
iso27001_scan_result_t* iso27001_scan_config(const char *config_data);

// Cleanup functions
void free_iso27001_check_result(iso27001_check_result_t *result);
void free_iso27001_scan_result(iso27001_scan_result_t *result);

#endif // ISO27001_HPP
