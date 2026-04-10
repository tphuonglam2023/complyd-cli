#include "frameworks/apps.hpp"
#include <cstring>
#include <cstdlib>

// ==================== CHECK 1: Authentication Security ====================
int apps_check_authentication_security(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "strong_authentication: enabled") ||
        strstr(config_data, "password_policy: enforced") ||
        strstr(config_data, "mfa_enabled: true") ||
        strstr(config_data, "auth_mechanism: secure")) {
        return 1;
    }
    return 0;
}

apps_check_result_t* create_apps_authentication_result(int passed, const char* details) {
    apps_check_result_t* result = static_cast<apps_check_result_t*>(calloc(1, sizeof(apps_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("APPS-AUTH-01");
    result->control_name = strdup("Authentication Security");
    result->severity = passed ? "INFO" : "CRITICAL";
    result->details = details ? strdup(details) : strdup(passed ?
        "Strong authentication is implemented" :
        "Strong authentication is NOT implemented");
    result->remediation = passed ? nullptr :
        strdup("Implement strong authentication mechanisms with MFA and secure password policies");
    return result;
}

// ==================== CHECK 2: Authorization Controls ====================
int apps_check_authorization_controls(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "rbac: enabled") ||
        strstr(config_data, "authorization: enforced") ||
        strstr(config_data, "access_control: implemented") ||
        strstr(config_data, "least_privilege: true")) {
        return 1;
    }
    return 0;
}

apps_check_result_t* create_apps_authorization_result(int passed, const char* details) {
    apps_check_result_t* result = static_cast<apps_check_result_t*>(calloc(1, sizeof(apps_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("APPS-AUTHZ-01");
    result->control_name = strdup("Authorization Controls");
    result->severity = passed ? "INFO" : "CRITICAL";
    result->details = details ? strdup(details) : strdup(passed ?
        "Authorization controls are enforced" :
        "Authorization controls are NOT enforced");
    result->remediation = passed ? nullptr :
        strdup("Implement role-based access control and principle of least privilege");
    return result;
}

// ==================== CHECK 3: Input Validation ====================
int apps_check_input_validation(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "input_validation: enabled") ||
        strstr(config_data, "sanitization: enforced") ||
        strstr(config_data, "whitelist_validation: true") ||
        strstr(config_data, "input_filtering: enabled")) {
        return 1;
    }
    return 0;
}

apps_check_result_t* create_apps_input_validation_result(int passed, const char* details) {
    apps_check_result_t* result = static_cast<apps_check_result_t*>(calloc(1, sizeof(apps_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("APPS-INPUT-01");
    result->control_name = strdup("Input Validation");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Input validation is implemented" :
        "Input validation is NOT implemented");
    result->remediation = passed ? nullptr :
        strdup("Implement comprehensive input validation and sanitization");
    return result;
}

// ==================== CHECK 4: Output Encoding ====================
int apps_check_output_encoding(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "output_encoding: enabled") ||
        strstr(config_data, "xss_protection: true") ||
        strstr(config_data, "context_aware_encoding: true") ||
        strstr(config_data, "safe_rendering: enabled")) {
        return 1;
    }
    return 0;
}

apps_check_result_t* create_apps_output_encoding_result(int passed, const char* details) {
    apps_check_result_t* result = static_cast<apps_check_result_t*>(calloc(1, sizeof(apps_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("APPS-OUTPUT-01");
    result->control_name = strdup("Output Encoding");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Output encoding is implemented" :
        "Output encoding is NOT implemented");
    result->remediation = passed ? nullptr :
        strdup("Implement context-aware output encoding to prevent XSS attacks");
    return result;
}

// ==================== CHECK 5: Session Management ====================
int apps_check_session_management(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "secure_sessions: enabled") ||
        strstr(config_data, "session_timeout: configured") ||
        strstr(config_data, "httponly_cookies: true") ||
        strstr(config_data, "secure_cookies: true")) {
        return 1;
    }
    return 0;
}

apps_check_result_t* create_apps_session_result(int passed, const char* details) {
    apps_check_result_t* result = static_cast<apps_check_result_t*>(calloc(1, sizeof(apps_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("APPS-SESSION-01");
    result->control_name = strdup("Session Management");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Secure session management is implemented" :
        "Secure session management is NOT implemented");
    result->remediation = passed ? nullptr :
        strdup("Implement secure session management with HTTPOnly and Secure flags");
    return result;
}

// ==================== CHECK 6: Cryptography ====================
int apps_check_cryptography(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "strong_crypto: enabled") ||
        strstr(config_data, "encryption_at_rest: true") ||
        strstr(config_data, "tls: enforced") ||
        strstr(config_data, "key_management: secure")) {
        return 1;
    }
    return 0;
}

apps_check_result_t* create_apps_crypto_result(int passed, const char* details) {
    apps_check_result_t* result = static_cast<apps_check_result_t*>(calloc(1, sizeof(apps_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("APPS-CRYPTO-01");
    result->control_name = strdup("Cryptographic Controls");
    result->severity = passed ? "INFO" : "CRITICAL";
    result->details = details ? strdup(details) : strdup(passed ?
        "Strong cryptography is implemented" :
        "Strong cryptography is NOT implemented");
    result->remediation = passed ? nullptr :
        strdup("Use strong cryptographic algorithms and secure key management");
    return result;
}

// ==================== CHECK 7: Error Handling ====================
int apps_check_error_handling(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "secure_error_handling: enabled") ||
        strstr(config_data, "error_messages: sanitized") ||
        strstr(config_data, "stack_traces: disabled") ||
        strstr(config_data, "generic_errors: true")) {
        return 1;
    }
    return 0;
}

apps_check_result_t* create_apps_error_result(int passed, const char* details) {
    apps_check_result_t* result = static_cast<apps_check_result_t*>(calloc(1, sizeof(apps_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("APPS-ERROR-01");
    result->control_name = strdup("Secure Error Handling");
    result->severity = passed ? "INFO" : "MEDIUM";
    result->details = details ? strdup(details) : strdup(passed ?
        "Secure error handling is implemented" :
        "Secure error handling is NOT implemented");
    result->remediation = passed ? nullptr :
        strdup("Implement secure error handling without exposing sensitive information");
    return result;
}

// ==================== CHECK 8: Logging and Monitoring ====================
int apps_check_logging_monitoring(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "security_logging: enabled") ||
        strstr(config_data, "audit_trail: maintained") ||
        strstr(config_data, "monitoring: active") ||
        strstr(config_data, "log_analysis: enabled")) {
        return 1;
    }
    return 0;
}

apps_check_result_t* create_apps_logging_result(int passed, const char* details) {
    apps_check_result_t* result = static_cast<apps_check_result_t*>(calloc(1, sizeof(apps_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("APPS-LOG-01");
    result->control_name = strdup("Security Logging and Monitoring");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Security logging and monitoring are implemented" :
        "Security logging and monitoring are NOT implemented");
    result->remediation = passed ? nullptr :
        strdup("Implement comprehensive security logging and monitoring");
    return result;
}

// ==================== CHECK 9: Data Protection ====================
int apps_check_data_protection(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "data_classification: defined") ||
        strstr(config_data, "sensitive_data_protection: enabled") ||
        strstr(config_data, "data_masking: implemented") ||
        strstr(config_data, "pii_protection: enforced")) {
        return 1;
    }
    return 0;
}

apps_check_result_t* create_apps_data_protection_result(int passed, const char* details) {
    apps_check_result_t* result = static_cast<apps_check_result_t*>(calloc(1, sizeof(apps_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("APPS-DATA-01");
    result->control_name = strdup("Data Protection");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Data protection controls are implemented" :
        "Data protection controls are NOT implemented");
    result->remediation = passed ? nullptr :
        strdup("Implement data classification and protection mechanisms");
    return result;
}

// ==================== CHECK 10: API Security ====================
int apps_check_api_security(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "api_authentication: enforced") ||
        strstr(config_data, "rate_limiting: enabled") ||
        strstr(config_data, "api_gateway: deployed") ||
        strstr(config_data, "api_security: enabled")) {
        return 1;
    }
    return 0;
}

apps_check_result_t* create_apps_api_result(int passed, const char* details) {
    apps_check_result_t* result = static_cast<apps_check_result_t*>(calloc(1, sizeof(apps_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("APPS-API-01");
    result->control_name = strdup("API Security");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "API security controls are implemented" :
        "API security controls are NOT implemented");
    result->remediation = passed ? nullptr :
        strdup("Implement API authentication, rate limiting, and security controls");
    return result;
}

// ==================== CHECK 11: Dependency Management ====================
int apps_check_dependency_management(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "dependency_scanning: enabled") ||
        strstr(config_data, "sca: implemented") ||
        strstr(config_data, "vulnerability_checks: automated") ||
        strstr(config_data, "package_management: secure")) {
        return 1;
    }
    return 0;
}

apps_check_result_t* create_apps_dependency_result(int passed, const char* details) {
    apps_check_result_t* result = static_cast<apps_check_result_t*>(calloc(1, sizeof(apps_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("APPS-DEP-01");
    result->control_name = strdup("Dependency Management");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Secure dependency management is implemented" :
        "Secure dependency management is NOT implemented");
    result->remediation = passed ? nullptr :
        strdup("Implement dependency scanning and vulnerability management");
    return result;
}

// ==================== CHECK 12: Security Headers ====================
int apps_check_security_headers(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "security_headers: enabled") ||
        strstr(config_data, "hsts: enabled") ||
        strstr(config_data, "csp: configured") ||
        strstr(config_data, "x_frame_options: set")) {
        return 1;
    }
    return 0;
}

apps_check_result_t* create_apps_headers_result(int passed, const char* details) {
    apps_check_result_t* result = static_cast<apps_check_result_t*>(calloc(1, sizeof(apps_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("APPS-HEAD-01");
    result->control_name = strdup("Security Headers");
    result->severity = passed ? "INFO" : "MEDIUM";
    result->details = details ? strdup(details) : strdup(passed ?
        "Security headers are configured" :
        "Security headers are NOT configured");
    result->remediation = passed ? nullptr :
        strdup("Configure security headers (HSTS, CSP, X-Frame-Options, etc.)");
    return result;
}

// ==================== Cleanup Function ====================
void free_apps_check_result(apps_check_result_t* result) {
    if (!result) return;

    if (result->control_id) free(result->control_id);
    if (result->control_name) free(result->control_name);
    if (result->details) free(result->details);
    if (result->remediation) free(result->remediation);

    free(result);
}
