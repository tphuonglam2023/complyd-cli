#ifndef SYSAUDIT_HPP
#define SYSAUDIT_HPP

#include <cstddef>

// System Audit Control structure
struct sysaudit_control_t {
    char *id;
    char *name;
    char *description;
    char *category;
};

// System Audit Framework structure
struct sysaudit_framework_t {
    sysaudit_control_t *controls;
    size_t control_count;
    size_t control_capacity;
};

// Check result structure
struct sysaudit_check_result_t {
    bool passed;
    char *control_id;
    char *control_name;
    const char *severity;  // "CRITICAL", "HIGH", "MEDIUM", "LOW", "INFO"
    char *details;
    char *remediation;
};

// Scan result structure
struct sysaudit_scan_result_t {
    sysaudit_check_result_t **results;
    size_t result_count;
    size_t passed_count;
    size_t failed_count;
};

// ── Module 1: Auto-update Status ──────────────────────────────────────────────
int sysaudit_check_auto_update_enabled(const char *config_data);
int sysaudit_check_auto_update_security_only(const char *config_data);
int sysaudit_check_auto_update_schedule(const char *config_data);
int sysaudit_check_auto_update_reboot_policy(const char *config_data);
int sysaudit_check_auto_update_notifications(const char *config_data);

sysaudit_check_result_t* create_sysaudit_auto_update_enabled_result(int passed, const char *details);
sysaudit_check_result_t* create_sysaudit_auto_update_security_result(int passed, const char *details);
sysaudit_check_result_t* create_sysaudit_auto_update_schedule_result(int passed, const char *details);
sysaudit_check_result_t* create_sysaudit_auto_update_reboot_result(int passed, const char *details);
sysaudit_check_result_t* create_sysaudit_auto_update_notify_result(int passed, const char *details);

// ── Module 2: SSH Key Audit ───────────────────────────────────────────────────
int sysaudit_check_ssh_key_auth_only(const char *config_data);
int sysaudit_check_ssh_root_login_disabled(const char *config_data);
int sysaudit_check_ssh_key_rotation(const char *config_data);
int sysaudit_check_ssh_key_algorithm(const char *config_data);
int sysaudit_check_ssh_passphrase_required(const char *config_data);
int sysaudit_check_ssh_authorized_keys_managed(const char *config_data);

sysaudit_check_result_t* create_sysaudit_ssh_key_auth_result(int passed, const char *details);
sysaudit_check_result_t* create_sysaudit_ssh_root_login_result(int passed, const char *details);
sysaudit_check_result_t* create_sysaudit_ssh_key_rotation_result(int passed, const char *details);
sysaudit_check_result_t* create_sysaudit_ssh_key_algorithm_result(int passed, const char *details);
sysaudit_check_result_t* create_sysaudit_ssh_passphrase_result(int passed, const char *details);
sysaudit_check_result_t* create_sysaudit_ssh_authorized_keys_result(int passed, const char *details);

// ── Module 3: Unapproved Software ────────────────────────────────────────────
int sysaudit_check_software_whitelist(const char *config_data);
int sysaudit_check_software_inventory(const char *config_data);
int sysaudit_check_software_install_policy(const char *config_data);
int sysaudit_check_software_scan_enabled(const char *config_data);
int sysaudit_check_software_removal_policy(const char *config_data);

sysaudit_check_result_t* create_sysaudit_software_whitelist_result(int passed, const char *details);
sysaudit_check_result_t* create_sysaudit_software_inventory_result(int passed, const char *details);
sysaudit_check_result_t* create_sysaudit_software_install_policy_result(int passed, const char *details);
sysaudit_check_result_t* create_sysaudit_software_scan_result(int passed, const char *details);
sysaudit_check_result_t* create_sysaudit_software_removal_result(int passed, const char *details);

// Scanner functions (one per module, one combined)
sysaudit_scan_result_t* sysaudit_scan_auto_update(const char *config_data);
sysaudit_scan_result_t* sysaudit_scan_ssh_keys(const char *config_data);
sysaudit_scan_result_t* sysaudit_scan_unapproved_software(const char *config_data);
sysaudit_scan_result_t* sysaudit_scan_all(const char *config_data);

// Cleanup functions
void free_sysaudit_check_result(sysaudit_check_result_t *result);
void free_sysaudit_scan_result(sysaudit_scan_result_t *result);

#endif // SYSAUDIT_HPP
