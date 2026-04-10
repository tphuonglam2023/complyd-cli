#ifndef ISO27002_HPP
#define ISO27002_HPP

#include <cstddef>

// ISO 27002 Control structure (implementation guidance for ISO 27001)
struct iso27002_control_t {
    char *id;
    char *name;
    char *description;
    char *category;
};

// ISO 27002 Framework structure
struct iso27002_framework_t {
    iso27002_control_t *controls;
    size_t control_count;
    size_t control_capacity;
};

// Check result structure
struct iso27002_check_result_t {
    bool passed;
    char *control_id;
    char *control_name;
    const char *severity;  // "CRITICAL", "HIGH", "MEDIUM", "LOW", "INFO"
    char *details;
    char *remediation;
};

// Scan result structure
struct iso27002_scan_result_t {
    iso27002_check_result_t **results;
    size_t result_count;
    size_t passed_count;
    size_t failed_count;
};

// Framework loader functions
iso27002_framework_t* iso27002_load_framework(const char *yaml_file);
void iso27002_free_framework(iso27002_framework_t *framework);

// Check functions - Enhanced controls based on ISO 27002:2022
int iso27002_check_asset_management(const char *config_data);
int iso27002_check_human_resources_security(const char *config_data);
int iso27002_check_security_awareness(const char *config_data);
int iso27002_check_vulnerability_management(const char *config_data);
int iso27002_check_malware_protection(const char *config_data);
int iso27002_check_logging_monitoring(const char *config_data);
int iso27002_check_secure_coding(const char *config_data);
int iso27002_check_change_control(const char *config_data);
int iso27002_check_capacity_planning(const char *config_data);
int iso27002_check_cloud_security(const char *config_data);

// Result creator functions
iso27002_check_result_t* create_iso27002_asset_result(int passed, const char *details);
iso27002_check_result_t* create_iso27002_hr_security_result(int passed, const char *details);
iso27002_check_result_t* create_iso27002_awareness_result(int passed, const char *details);
iso27002_check_result_t* create_iso27002_vulnerability_result(int passed, const char *details);
iso27002_check_result_t* create_iso27002_malware_result(int passed, const char *details);
iso27002_check_result_t* create_iso27002_logging_result(int passed, const char *details);
iso27002_check_result_t* create_iso27002_coding_result(int passed, const char *details);
iso27002_check_result_t* create_iso27002_change_result(int passed, const char *details);
iso27002_check_result_t* create_iso27002_capacity_result(int passed, const char *details);
iso27002_check_result_t* create_iso27002_cloud_result(int passed, const char *details);

// Scanner function
iso27002_scan_result_t* iso27002_scan_config(const char *config_data);

// Cleanup functions
void free_iso27002_check_result(iso27002_check_result_t *result);
void free_iso27002_scan_result(iso27002_scan_result_t *result);

#endif // ISO27002_HPP
