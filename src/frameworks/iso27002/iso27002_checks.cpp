#include "frameworks/iso27002.hpp"
#include <cstring>
#include <cstdlib>

// ==================== CHECK 1: Asset Management (5.9) ====================
int iso27002_check_asset_management(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "asset_inventory: maintained") ||
        strstr(config_data, "asset_management: enabled") ||
        strstr(config_data, "asset_tracking: true") ||
        strstr(config_data, "cmdb: enabled")) {
        return 1;
    }
    return 0;
}

iso27002_check_result_t* create_iso27002_asset_result(int passed, const char* details) {
    iso27002_check_result_t* result = static_cast<iso27002_check_result_t*>(calloc(1, sizeof(iso27002_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("5.9");
    result->control_name = strdup("Inventory of Information and Assets");
    result->severity = passed ? "INFO" : "MEDIUM";
    result->details = details ? strdup(details) : strdup(passed ?
        "Asset inventory is maintained" :
        "Asset inventory is NOT maintained");
    result->remediation = passed ? nullptr :
        strdup("Implement and maintain an inventory of information and associated assets");
    return result;
}

// ==================== CHECK 2: Human Resources Security (6.1) ====================
int iso27002_check_human_resources_security(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "background_checks: enabled") ||
        strstr(config_data, "hr_security: enforced") ||
        strstr(config_data, "employee_screening: true") ||
        strstr(config_data, "security_roles: defined")) {
        return 1;
    }
    return 0;
}

iso27002_check_result_t* create_iso27002_hr_security_result(int passed, const char* details) {
    iso27002_check_result_t* result = static_cast<iso27002_check_result_t*>(calloc(1, sizeof(iso27002_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("6.1");
    result->control_name = strdup("Screening");
    result->severity = passed ? "INFO" : "MEDIUM";
    result->details = details ? strdup(details) : strdup(passed ?
        "HR security screening is enforced" :
        "HR security screening is NOT enforced");
    result->remediation = passed ? nullptr :
        strdup("Implement background verification checks for all candidates");
    return result;
}

// ==================== CHECK 3: Security Awareness (6.3) ====================
int iso27002_check_security_awareness(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "security_training: required") ||
        strstr(config_data, "awareness_program: active") ||
        strstr(config_data, "training_completed: true") ||
        strstr(config_data, "security_education: enabled")) {
        return 1;
    }
    return 0;
}

iso27002_check_result_t* create_iso27002_awareness_result(int passed, const char* details) {
    iso27002_check_result_t* result = static_cast<iso27002_check_result_t*>(calloc(1, sizeof(iso27002_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("6.3");
    result->control_name = strdup("Information Security Awareness and Training");
    result->severity = passed ? "INFO" : "MEDIUM";
    result->details = details ? strdup(details) : strdup(passed ?
        "Security awareness training is provided" :
        "Security awareness training is NOT provided");
    result->remediation = passed ? nullptr :
        strdup("Provide information security awareness education and training");
    return result;
}

// ==================== CHECK 4: Vulnerability Management (8.8) ====================
int iso27002_check_vulnerability_management(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "vulnerability_scanning: enabled") ||
        strstr(config_data, "patch_management: automated") ||
        strstr(config_data, "security_scanning: true") ||
        strstr(config_data, "vuln_assessment: scheduled")) {
        return 1;
    }
    return 0;
}

iso27002_check_result_t* create_iso27002_vulnerability_result(int passed, const char* details) {
    iso27002_check_result_t* result = static_cast<iso27002_check_result_t*>(calloc(1, sizeof(iso27002_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("8.8");
    result->control_name = strdup("Management of Technical Vulnerabilities");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Vulnerability management is implemented" :
        "Vulnerability management is NOT implemented");
    result->remediation = passed ? nullptr :
        strdup("Implement vulnerability scanning and patch management processes");
    return result;
}

// ==================== CHECK 5: Malware Protection (8.7) ====================
int iso27002_check_malware_protection(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "antivirus: enabled") ||
        strstr(config_data, "malware_protection: active") ||
        strstr(config_data, "edr: deployed") ||
        strstr(config_data, "endpoint_protection: true")) {
        return 1;
    }
    return 0;
}

iso27002_check_result_t* create_iso27002_malware_result(int passed, const char* details) {
    iso27002_check_result_t* result = static_cast<iso27002_check_result_t*>(calloc(1, sizeof(iso27002_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("8.7");
    result->control_name = strdup("Protection Against Malware");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Malware protection is active" :
        "Malware protection is NOT active");
    result->remediation = passed ? nullptr :
        strdup("Deploy and maintain malware detection and prevention controls");
    return result;
}

// ==================== CHECK 6: Logging and Monitoring (8.15) ====================
int iso27002_check_logging_monitoring(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "logging: enabled") ||
        strstr(config_data, "siem: deployed") ||
        strstr(config_data, "log_monitoring: active") ||
        strstr(config_data, "audit_log: enabled")) {
        return 1;
    }
    return 0;
}

iso27002_check_result_t* create_iso27002_logging_result(int passed, const char* details) {
    iso27002_check_result_t* result = static_cast<iso27002_check_result_t*>(calloc(1, sizeof(iso27002_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("8.15");
    result->control_name = strdup("Logging");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Logging and monitoring are enabled" :
        "Logging and monitoring are NOT enabled");
    result->remediation = passed ? nullptr :
        strdup("Implement comprehensive logging and monitoring of security events");
    return result;
}

// ==================== CHECK 7: Secure Coding (8.28) ====================
int iso27002_check_secure_coding(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "secure_coding: enforced") ||
        strstr(config_data, "code_review: required") ||
        strstr(config_data, "sast: enabled") ||
        strstr(config_data, "security_testing: automated")) {
        return 1;
    }
    return 0;
}

iso27002_check_result_t* create_iso27002_coding_result(int passed, const char* details) {
    iso27002_check_result_t* result = static_cast<iso27002_check_result_t*>(calloc(1, sizeof(iso27002_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("8.28");
    result->control_name = strdup("Secure Coding");
    result->severity = passed ? "INFO" : "MEDIUM";
    result->details = details ? strdup(details) : strdup(passed ?
        "Secure coding practices are enforced" :
        "Secure coding practices are NOT enforced");
    result->remediation = passed ? nullptr :
        strdup("Apply secure coding principles throughout software development lifecycle");
    return result;
}

// ==================== CHECK 8: Change Control (8.32) ====================
int iso27002_check_change_control(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "change_control: enforced") ||
        strstr(config_data, "change_management: enabled") ||
        strstr(config_data, "approval_workflow: required") ||
        strstr(config_data, "change_tracking: enabled")) {
        return 1;
    }
    return 0;
}

iso27002_check_result_t* create_iso27002_change_result(int passed, const char* details) {
    iso27002_check_result_t* result = static_cast<iso27002_check_result_t*>(calloc(1, sizeof(iso27002_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("8.32");
    result->control_name = strdup("Change Management");
    result->severity = passed ? "INFO" : "MEDIUM";
    result->details = details ? strdup(details) : strdup(passed ?
        "Change control is enforced" :
        "Change control is NOT enforced");
    result->remediation = passed ? nullptr :
        strdup("Implement formal change management procedures for all systems");
    return result;
}

// ==================== CHECK 9: Capacity Planning (8.6) ====================
int iso27002_check_capacity_planning(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "capacity_planning: enabled") ||
        strstr(config_data, "resource_monitoring: active") ||
        strstr(config_data, "scalability: planned") ||
        strstr(config_data, "performance_monitoring: true")) {
        return 1;
    }
    return 0;
}

iso27002_check_result_t* create_iso27002_capacity_result(int passed, const char* details) {
    iso27002_check_result_t* result = static_cast<iso27002_check_result_t*>(calloc(1, sizeof(iso27002_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("8.6");
    result->control_name = strdup("Capacity Management");
    result->severity = passed ? "INFO" : "LOW";
    result->details = details ? strdup(details) : strdup(passed ?
        "Capacity planning is implemented" :
        "Capacity planning is NOT implemented");
    result->remediation = passed ? nullptr :
        strdup("Monitor and project future capacity requirements");
    return result;
}

// ==================== CHECK 10: Cloud Security (5.23) ====================
int iso27002_check_cloud_security(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "cloud_security: enabled") ||
        strstr(config_data, "cspm: deployed") ||
        strstr(config_data, "cloud_controls: enforced") ||
        strstr(config_data, "cloud_compliance: monitored")) {
        return 1;
    }
    return 0;
}

iso27002_check_result_t* create_iso27002_cloud_result(int passed, const char* details) {
    iso27002_check_result_t* result = static_cast<iso27002_check_result_t*>(calloc(1, sizeof(iso27002_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("5.23");
    result->control_name = strdup("Information Security for Cloud Services");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Cloud security controls are implemented" :
        "Cloud security controls are NOT implemented");
    result->remediation = passed ? nullptr :
        strdup("Implement cloud-specific security controls and monitoring");
    return result;
}

// ==================== Cleanup Function ====================
void free_iso27002_check_result(iso27002_check_result_t* result) {
    if (!result) return;

    if (result->control_id) free(result->control_id);
    if (result->control_name) free(result->control_name);
    if (result->details) free(result->details);
    if (result->remediation) free(result->remediation);

    free(result);
}
