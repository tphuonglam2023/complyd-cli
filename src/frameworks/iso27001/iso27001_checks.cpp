#include "frameworks/iso27001.hpp"
#include <cstring>
#include <cstdlib>

// ==================== CHECK 1: Information Security Policies (A.5.1) ====================
int iso27001_check_information_security_policies(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "security_policy: defined") ||
        strstr(config_data, "information_security_policy: true") ||
        strstr(config_data, "policy_documented: true") ||
        strstr(config_data, "security_policies: enabled")) {
        return 1;
    }
    return 0;
}

iso27001_check_result_t* create_iso27001_policies_result(int passed, const char* details) {
    iso27001_check_result_t* result = static_cast<iso27001_check_result_t*>(calloc(1, sizeof(iso27001_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("A.5.1");
    result->control_name = strdup("Information Security Policies");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Information security policies are defined and documented" :
        "Information security policies are NOT defined");
    result->remediation = passed ? nullptr :
        strdup("Define, document, and approve information security policies");
    return result;
}

// ==================== CHECK 2: Access Control Policy (A.9.1) ====================
int iso27001_check_access_control_policy(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "access_control: enforced") ||
        strstr(config_data, "rbac_enabled: true") ||
        strstr(config_data, "least_privilege: true") ||
        strstr(config_data, "access_policy: defined")) {
        return 1;
    }
    return 0;
}

iso27001_check_result_t* create_iso27001_access_control_result(int passed, const char* details) {
    iso27001_check_result_t* result = static_cast<iso27001_check_result_t*>(calloc(1, sizeof(iso27001_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("A.9.1");
    result->control_name = strdup("Access Control Policy");
    result->severity = passed ? "INFO" : "CRITICAL";
    result->details = details ? strdup(details) : strdup(passed ?
        "Access control policy is enforced" :
        "Access control policy is NOT enforced");
    result->remediation = passed ? nullptr :
        strdup("Implement access control policies based on business and security requirements");
    return result;
}

// ==================== CHECK 3: Cryptographic Controls (A.10.1) ====================
int iso27001_check_cryptographic_controls(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "encryption: enabled") ||
        strstr(config_data, "crypto_policy: defined") ||
        strstr(config_data, "key_management: enabled") ||
        strstr(config_data, "tls: enabled")) {
        return 1;
    }
    return 0;
}

iso27001_check_result_t* create_iso27001_crypto_result(int passed, const char* details) {
    iso27001_check_result_t* result = static_cast<iso27001_check_result_t*>(calloc(1, sizeof(iso27001_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("A.10.1");
    result->control_name = strdup("Cryptographic Controls");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Cryptographic controls are implemented" :
        "Cryptographic controls are NOT implemented");
    result->remediation = passed ? nullptr :
        strdup("Implement cryptographic policy and key management procedures");
    return result;
}

// ==================== CHECK 4: Physical Security (A.11.1) ====================
int iso27001_check_physical_security(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "physical_security: enabled") ||
        strstr(config_data, "secure_areas: defined") ||
        strstr(config_data, "datacenter_security: true") ||
        strstr(config_data, "physical_access_control: true")) {
        return 1;
    }
    return 0;
}

iso27001_check_result_t* create_iso27001_physical_result(int passed, const char* details) {
    iso27001_check_result_t* result = static_cast<iso27001_check_result_t*>(calloc(1, sizeof(iso27001_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("A.11.1");
    result->control_name = strdup("Physical and Environmental Security");
    result->severity = passed ? "INFO" : "MEDIUM";
    result->details = details ? strdup(details) : strdup(passed ?
        "Physical security controls are in place" :
        "Physical security controls are NOT configured");
    result->remediation = passed ? nullptr :
        strdup("Define secure areas and implement physical entry controls");
    return result;
}

// ==================== CHECK 5: Operations Security (A.12.1) ====================
int iso27001_check_operations_security(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "operations_procedures: documented") ||
        strstr(config_data, "change_management: enabled") ||
        strstr(config_data, "capacity_management: true") ||
        strstr(config_data, "monitoring: enabled")) {
        return 1;
    }
    return 0;
}

iso27001_check_result_t* create_iso27001_operations_result(int passed, const char* details) {
    iso27001_check_result_t* result = static_cast<iso27001_check_result_t*>(calloc(1, sizeof(iso27001_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("A.12.1");
    result->control_name = strdup("Operations Security");
    result->severity = passed ? "INFO" : "MEDIUM";
    result->details = details ? strdup(details) : strdup(passed ?
        "Operations security procedures are documented" :
        "Operations security procedures are NOT documented");
    result->remediation = passed ? nullptr :
        strdup("Document and implement operational procedures and responsibilities");
    return result;
}

// ==================== CHECK 6: Network Security (A.13.1) ====================
int iso27001_check_network_security(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "network_security: enabled") ||
        strstr(config_data, "firewall: enabled") ||
        strstr(config_data, "network_segmentation: true") ||
        strstr(config_data, "ids_ips: enabled")) {
        return 1;
    }
    return 0;
}

iso27001_check_result_t* create_iso27001_network_result(int passed, const char* details) {
    iso27001_check_result_t* result = static_cast<iso27001_check_result_t*>(calloc(1, sizeof(iso27001_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("A.13.1");
    result->control_name = strdup("Network Security Management");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Network security controls are implemented" :
        "Network security controls are NOT implemented");
    result->remediation = passed ? nullptr :
        strdup("Implement network controls, segregation, and monitoring");
    return result;
}

// ==================== CHECK 7: System Acquisition (A.14.1) ====================
int iso27001_check_system_acquisition(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "security_requirements: defined") ||
        strstr(config_data, "sdlc_security: integrated") ||
        strstr(config_data, "secure_development: true") ||
        strstr(config_data, "development_policy: enforced")) {
        return 1;
    }
    return 0;
}

iso27001_check_result_t* create_iso27001_acquisition_result(int passed, const char* details) {
    iso27001_check_result_t* result = static_cast<iso27001_check_result_t*>(calloc(1, sizeof(iso27001_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("A.14.1");
    result->control_name = strdup("Security in Development and Support");
    result->severity = passed ? "INFO" : "MEDIUM";
    result->details = details ? strdup(details) : strdup(passed ?
        "Security requirements for systems are defined" :
        "Security requirements for systems are NOT defined");
    result->remediation = passed ? nullptr :
        strdup("Define and implement security requirements in system development lifecycle");
    return result;
}

// ==================== CHECK 8: Supplier Relationships (A.15.1) ====================
int iso27001_check_supplier_relationships(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "supplier_security: managed") ||
        strstr(config_data, "third_party_risk: assessed") ||
        strstr(config_data, "vendor_management: enabled") ||
        strstr(config_data, "supplier_agreements: defined")) {
        return 1;
    }
    return 0;
}

iso27001_check_result_t* create_iso27001_supplier_result(int passed, const char* details) {
    iso27001_check_result_t* result = static_cast<iso27001_check_result_t*>(calloc(1, sizeof(iso27001_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("A.15.1");
    result->control_name = strdup("Supplier Relationships");
    result->severity = passed ? "INFO" : "MEDIUM";
    result->details = details ? strdup(details) : strdup(passed ?
        "Supplier security requirements are managed" :
        "Supplier security requirements are NOT managed");
    result->remediation = passed ? nullptr :
        strdup("Define security requirements for supplier relationships and service delivery");
    return result;
}

// ==================== CHECK 9: Incident Management (A.16.1) ====================
int iso27001_check_incident_management(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "incident_response: enabled") ||
        strstr(config_data, "incident_management: defined") ||
        strstr(config_data, "security_events: monitored") ||
        strstr(config_data, "incident_procedures: documented")) {
        return 1;
    }
    return 0;
}

iso27001_check_result_t* create_iso27001_incident_result(int passed, const char* details) {
    iso27001_check_result_t* result = static_cast<iso27001_check_result_t*>(calloc(1, sizeof(iso27001_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("A.16.1");
    result->control_name = strdup("Incident Management");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Incident management procedures are defined" :
        "Incident management procedures are NOT defined");
    result->remediation = passed ? nullptr :
        strdup("Establish incident management procedures and responsibilities");
    return result;
}

// ==================== CHECK 10: Business Continuity (A.17.1) ====================
int iso27001_check_business_continuity(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "business_continuity: planned") ||
        strstr(config_data, "disaster_recovery: enabled") ||
        strstr(config_data, "continuity_plan: documented") ||
        strstr(config_data, "backup: enabled")) {
        return 1;
    }
    return 0;
}

iso27001_check_result_t* create_iso27001_continuity_result(int passed, const char* details) {
    iso27001_check_result_t* result = static_cast<iso27001_check_result_t*>(calloc(1, sizeof(iso27001_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("A.17.1");
    result->control_name = strdup("Business Continuity");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Business continuity plans are in place" :
        "Business continuity plans are NOT in place");
    result->remediation = passed ? nullptr :
        strdup("Develop and maintain business continuity and disaster recovery plans");
    return result;
}

// ==================== CHECK 11: Compliance (A.18.1) ====================
int iso27001_check_compliance(const char* config_data) {
    if (!config_data) return 0;

    if (strstr(config_data, "compliance: monitored") ||
        strstr(config_data, "legal_requirements: tracked") ||
        strstr(config_data, "regulatory_compliance: true") ||
        strstr(config_data, "compliance_review: enabled")) {
        return 1;
    }
    return 0;
}

iso27001_check_result_t* create_iso27001_compliance_result(int passed, const char* details) {
    iso27001_check_result_t* result = static_cast<iso27001_check_result_t*>(calloc(1, sizeof(iso27001_check_result_t)));
    if (!result) return nullptr;

    result->passed = passed;
    result->control_id = strdup("A.18.1");
    result->control_name = strdup("Compliance");
    result->severity = passed ? "INFO" : "HIGH";
    result->details = details ? strdup(details) : strdup(passed ?
        "Compliance with legal and contractual requirements is monitored" :
        "Compliance with legal and contractual requirements is NOT monitored");
    result->remediation = passed ? nullptr :
        strdup("Identify and document all applicable legal and regulatory requirements");
    return result;
}

// ==================== Cleanup Function ====================
void free_iso27001_check_result(iso27001_check_result_t* result) {
    if (!result) return;

    if (result->control_id) free(result->control_id);
    if (result->control_name) free(result->control_name);
    if (result->details) free(result->details);
    if (result->remediation) free(result->remediation);

    free(result);
}
