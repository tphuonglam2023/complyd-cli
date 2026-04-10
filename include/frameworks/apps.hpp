#ifndef APPS_HPP
#define APPS_HPP

#include <cstddef>

// APPs (Application Security) Control structure
struct apps_control_t {
    char *id;
    char *name;
    char *description;
    char *category;
};

// APPs Framework structure
struct apps_framework_t {
    apps_control_t *controls;
    size_t control_count;
    size_t control_capacity;
};

// Check result structure
struct apps_check_result_t {
    bool passed;
    char *control_id;
    char *control_name;
    const char *severity;  // "CRITICAL", "HIGH", "MEDIUM", "LOW", "INFO"
    char *details;
    char *remediation;
};

// Scan result structure
struct apps_scan_result_t {
    apps_check_result_t **results;
    size_t result_count;
    size_t passed_count;
    size_t failed_count;
};

// Framework loader functions
apps_framework_t* apps_load_framework(const char *yaml_file);
void apps_free_framework(apps_framework_t *framework);

// Check functions - Application Security Best Practices
int apps_check_authentication_security(const char *config_data);
int apps_check_authorization_controls(const char *config_data);
int apps_check_input_validation(const char *config_data);
int apps_check_output_encoding(const char *config_data);
int apps_check_session_management(const char *config_data);
int apps_check_cryptography(const char *config_data);
int apps_check_error_handling(const char *config_data);
int apps_check_logging_monitoring(const char *config_data);
int apps_check_data_protection(const char *config_data);
int apps_check_api_security(const char *config_data);
int apps_check_dependency_management(const char *config_data);
int apps_check_security_headers(const char *config_data);

// Result creator functions
apps_check_result_t* create_apps_authentication_result(int passed, const char *details);
apps_check_result_t* create_apps_authorization_result(int passed, const char *details);
apps_check_result_t* create_apps_input_validation_result(int passed, const char *details);
apps_check_result_t* create_apps_output_encoding_result(int passed, const char *details);
apps_check_result_t* create_apps_session_result(int passed, const char *details);
apps_check_result_t* create_apps_crypto_result(int passed, const char *details);
apps_check_result_t* create_apps_error_result(int passed, const char *details);
apps_check_result_t* create_apps_logging_result(int passed, const char *details);
apps_check_result_t* create_apps_data_protection_result(int passed, const char *details);
apps_check_result_t* create_apps_api_result(int passed, const char *details);
apps_check_result_t* create_apps_dependency_result(int passed, const char *details);
apps_check_result_t* create_apps_headers_result(int passed, const char *details);

// Scanner function
apps_scan_result_t* apps_scan_config(const char *config_data);

// Cleanup functions
void free_apps_check_result(apps_check_result_t *result);
void free_apps_scan_result(apps_scan_result_t *result);

#endif // APPS_HPP
