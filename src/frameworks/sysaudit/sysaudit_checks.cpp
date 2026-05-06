#include "frameworks/sysaudit.hpp"
#include <cstring>
#include <cstdlib>

// ============================================================================
//  MODULE 1 – AUTO-UPDATE STATUS
// ============================================================================

// CHECK 1.1: Auto-update is enabled
int sysaudit_check_auto_update_enabled(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "auto_update: enabled") ||
        strstr(config_data, "automatic_updates: true") ||
        strstr(config_data, "unattended_upgrades: enabled") ||
        strstr(config_data, "auto_update_enabled: true") ||
        strstr(config_data, "windows_update: automatic")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_auto_update_enabled_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-UPD-01");
    r->control_name = strdup("Auto-Update Enabled");
    r->severity     = passed ? "INFO" : "HIGH";
    r->details      = details ? strdup(details) : strdup(passed ?
        "Automatic system updates are enabled" :
        "Automatic system updates are NOT enabled");
    r->remediation  = passed ? nullptr :
        strdup("Enable automatic updates (e.g. unattended-upgrades on Debian/Ubuntu, "
               "yum-cron on RHEL, or Windows Update set to automatic)");
    return r;
}

// CHECK 1.2: Security-only updates are configured
int sysaudit_check_auto_update_security_only(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "security_updates_only: true") ||
        strstr(config_data, "update_type: security") ||
        strstr(config_data, "security_patches: auto") ||
        strstr(config_data, "critical_updates: automatic")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_auto_update_security_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-UPD-02");
    r->control_name = strdup("Security-Only Update Policy");
    r->severity     = passed ? "INFO" : "HIGH";
    r->details      = details ? strdup(details) : strdup(passed ?
        "Security-only update policy is configured" :
        "Security-only update policy is NOT configured");
    r->remediation  = passed ? nullptr :
        strdup("Configure the update manager to automatically apply security patches "
               "and critical fixes without requiring manual intervention");
    return r;
}

// CHECK 1.3: Update schedule is defined
int sysaudit_check_auto_update_schedule(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "update_schedule:") ||
        strstr(config_data, "patch_schedule:") ||
        strstr(config_data, "maintenance_window:") ||
        strstr(config_data, "update_cron:")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_auto_update_schedule_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-UPD-03");
    r->control_name = strdup("Update Schedule Defined");
    r->severity     = passed ? "INFO" : "MEDIUM";
    r->details      = details ? strdup(details) : strdup(passed ?
        "A patching/maintenance schedule is defined" :
        "No patching/maintenance schedule is defined");
    r->remediation  = passed ? nullptr :
        strdup("Define a regular patching schedule (e.g. weekly maintenance window) "
               "and document it in the system configuration");
    return r;
}

// CHECK 1.4: Reboot policy after updates is defined
int sysaudit_check_auto_update_reboot_policy(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "reboot_after_update: true") ||
        strstr(config_data, "auto_reboot: scheduled") ||
        strstr(config_data, "reboot_policy: automatic") ||
        strstr(config_data, "post_update_reboot: enabled")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_auto_update_reboot_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-UPD-04");
    r->control_name = strdup("Post-Update Reboot Policy");
    r->severity     = passed ? "INFO" : "MEDIUM";
    r->details      = details ? strdup(details) : strdup(passed ?
        "Post-update reboot policy is configured" :
        "Post-update reboot policy is NOT configured");
    r->remediation  = passed ? nullptr :
        strdup("Configure an automatic or scheduled reboot policy so kernel and "
               "library updates take effect promptly");
    return r;
}

// CHECK 1.5: Update notifications are enabled
int sysaudit_check_auto_update_notifications(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "update_notifications: enabled") ||
        strstr(config_data, "patch_alerts: true") ||
        strstr(config_data, "update_email_alerts: true") ||
        strstr(config_data, "notify_on_update: true")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_auto_update_notify_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-UPD-05");
    r->control_name = strdup("Update Notifications Enabled");
    r->severity     = passed ? "INFO" : "LOW";
    r->details      = details ? strdup(details) : strdup(passed ?
        "Update/patch notifications are enabled" :
        "Update/patch notifications are NOT enabled");
    r->remediation  = passed ? nullptr :
        strdup("Enable email or alerting notifications for applied patches so "
               "administrators are aware of system changes");
    return r;
}

// ============================================================================
//  MODULE 2 – SSH KEY AUDIT
// ============================================================================

// CHECK 2.1: Password authentication disabled (key-only)
int sysaudit_check_ssh_key_auth_only(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "ssh_password_auth: disabled") ||
        strstr(config_data, "password_authentication: false") ||
        strstr(config_data, "pubkey_auth_only: true") ||
        strstr(config_data, "ssh_key_only: true")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_ssh_key_auth_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-SSH-01");
    r->control_name = strdup("SSH Key-Only Authentication");
    r->severity     = passed ? "INFO" : "CRITICAL";
    r->details      = details ? strdup(details) : strdup(passed ?
        "SSH password authentication is disabled; key-based auth only" :
        "SSH password authentication is NOT disabled");
    r->remediation  = passed ? nullptr :
        strdup("Set 'PasswordAuthentication no' in /etc/ssh/sshd_config and "
               "restart the SSH daemon to enforce key-only login");
    return r;
}

// CHECK 2.2: Root login over SSH is disabled
int sysaudit_check_ssh_root_login_disabled(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "ssh_root_login: disabled") ||
        strstr(config_data, "permit_root_login: false") ||
        strstr(config_data, "root_login_disabled: true") ||
        strstr(config_data, "ssh_deny_root: true")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_ssh_root_login_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-SSH-02");
    r->control_name = strdup("SSH Root Login Disabled");
    r->severity     = passed ? "INFO" : "CRITICAL";
    r->details      = details ? strdup(details) : strdup(passed ?
        "Direct root login via SSH is disabled" :
        "Direct root login via SSH is NOT disabled");
    r->remediation  = passed ? nullptr :
        strdup("Set 'PermitRootLogin no' in /etc/ssh/sshd_config. "
               "Use sudo for privileged operations instead");
    return r;
}

// CHECK 2.3: SSH key rotation policy is defined
int sysaudit_check_ssh_key_rotation(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "ssh_key_rotation: enabled") ||
        strstr(config_data, "key_rotation_days:") ||
        strstr(config_data, "ssh_key_expiry:") ||
        strstr(config_data, "rotate_ssh_keys: true")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_ssh_key_rotation_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-SSH-03");
    r->control_name = strdup("SSH Key Rotation Policy");
    r->severity     = passed ? "INFO" : "HIGH";
    r->details      = details ? strdup(details) : strdup(passed ?
        "SSH key rotation policy is defined" :
        "SSH key rotation policy is NOT defined");
    r->remediation  = passed ? nullptr :
        strdup("Establish a key rotation policy (e.g. every 90 days) and automate "
               "revocation of expired keys from authorized_keys files");
    return r;
}

// CHECK 2.4: Strong key algorithms are enforced
int sysaudit_check_ssh_key_algorithm(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "ssh_key_algorithm: ed25519") ||
        strstr(config_data, "ssh_key_algorithm: rsa4096") ||
        strstr(config_data, "ssh_key_type: ed25519") ||
        strstr(config_data, "ssh_key_bits: 4096") ||
        strstr(config_data, "strong_ssh_keys: true")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_ssh_key_algorithm_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-SSH-04");
    r->control_name = strdup("Strong SSH Key Algorithm");
    r->severity     = passed ? "INFO" : "HIGH";
    r->details      = details ? strdup(details) : strdup(passed ?
        "Strong SSH key algorithms (Ed25519 / RSA-4096) are enforced" :
        "Strong SSH key algorithm policy is NOT enforced");
    r->remediation  = passed ? nullptr :
        strdup("Require Ed25519 or RSA-4096 keys. Add 'PubkeyAcceptedKeyTypes "
               "ssh-ed25519,rsa-sha2-512' to sshd_config and regenerate weak keys");
    return r;
}

// CHECK 2.5: SSH key passphrase is required
int sysaudit_check_ssh_passphrase_required(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "ssh_passphrase_required: true") ||
        strstr(config_data, "require_key_passphrase: true") ||
        strstr(config_data, "ssh_key_passphrase: enforced")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_ssh_passphrase_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-SSH-05");
    r->control_name = strdup("SSH Key Passphrase Required");
    r->severity     = passed ? "INFO" : "MEDIUM";
    r->details      = details ? strdup(details) : strdup(passed ?
        "SSH private key passphrase requirement is enforced" :
        "SSH private key passphrase requirement is NOT enforced");
    r->remediation  = passed ? nullptr :
        strdup("Enforce passphrase protection on all SSH private keys via policy "
               "and use an SSH agent (ssh-agent) to avoid repeated prompts");
    return r;
}

// CHECK 2.6: Authorized keys are centrally managed
int sysaudit_check_ssh_authorized_keys_managed(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "authorized_keys_managed: true") ||
        strstr(config_data, "ssh_key_management: centralized") ||
        strstr(config_data, "ssh_ca: enabled") ||
        strstr(config_data, "ssh_certificates: enabled")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_ssh_authorized_keys_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-SSH-06");
    r->control_name = strdup("Centralized SSH Authorized Keys Management");
    r->severity     = passed ? "INFO" : "MEDIUM";
    r->details      = details ? strdup(details) : strdup(passed ?
        "SSH authorized keys are centrally managed or use SSH CA certificates" :
        "SSH authorized keys are NOT centrally managed");
    r->remediation  = passed ? nullptr :
        strdup("Use a centralized SSH key management solution or SSH Certificate "
               "Authority (CA) to control and audit all authorized keys");
    return r;
}

// ============================================================================
//  MODULE 3 – UNAPPROVED SOFTWARE
// ============================================================================

// CHECK 3.1: Software whitelist / allowlist is defined
int sysaudit_check_software_whitelist(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "software_whitelist: enabled") ||
        strstr(config_data, "application_allowlist: true") ||
        strstr(config_data, "approved_software_list: defined") ||
        strstr(config_data, "software_allowlist: enforced")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_software_whitelist_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-SW-01");
    r->control_name = strdup("Software Whitelist / Allowlist");
    r->severity     = passed ? "INFO" : "HIGH";
    r->details      = details ? strdup(details) : strdup(passed ?
        "An approved software allowlist is defined and enforced" :
        "No approved software allowlist is defined");
    r->remediation  = passed ? nullptr :
        strdup("Define and enforce an application allowlist using tools such as "
               "AppArmor, SELinux, Windows Defender Application Control, or similar");
    return r;
}

// CHECK 3.2: Software inventory is maintained
int sysaudit_check_software_inventory(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "software_inventory: enabled") ||
        strstr(config_data, "asset_inventory: maintained") ||
        strstr(config_data, "software_catalog: active") ||
        strstr(config_data, "installed_software_tracking: true")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_software_inventory_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-SW-02");
    r->control_name = strdup("Software Inventory Maintained");
    r->severity     = passed ? "INFO" : "HIGH";
    r->details      = details ? strdup(details) : strdup(passed ?
        "A software inventory / asset catalog is actively maintained" :
        "No software inventory is maintained");
    r->remediation  = passed ? nullptr :
        strdup("Implement automated software inventory tracking (e.g. CMDB, "
               "osquery, or SCCM) to maintain an up-to-date list of installed software");
    return r;
}

// CHECK 3.3: Software installation policy restricts non-admin installs
int sysaudit_check_software_install_policy(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "software_install_policy: restricted") ||
        strstr(config_data, "admin_only_install: true") ||
        strstr(config_data, "restrict_software_install: true") ||
        strstr(config_data, "user_install_blocked: true")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_software_install_policy_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-SW-03");
    r->control_name = strdup("Software Installation Policy");
    r->severity     = passed ? "INFO" : "HIGH";
    r->details      = details ? strdup(details) : strdup(passed ?
        "Software installation is restricted to authorized administrators" :
        "Software installation is NOT restricted to authorized administrators");
    r->remediation  = passed ? nullptr :
        strdup("Restrict software installation to privileged accounts only. "
               "Use group policy, sudoers rules, or MDM profiles to prevent "
               "unauthorized installations by standard users");
    return r;
}

// CHECK 3.4: Automated software scanning is enabled
int sysaudit_check_software_scan_enabled(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "software_scan: enabled") ||
        strstr(config_data, "unapproved_software_scan: true") ||
        strstr(config_data, "endpoint_detection: active") ||
        strstr(config_data, "software_compliance_scan: enabled")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_software_scan_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-SW-04");
    r->control_name = strdup("Automated Software Compliance Scanning");
    r->severity     = passed ? "INFO" : "MEDIUM";
    r->details      = details ? strdup(details) : strdup(passed ?
        "Automated scanning for unapproved software is enabled" :
        "Automated scanning for unapproved software is NOT enabled");
    r->remediation  = passed ? nullptr :
        strdup("Enable periodic automated scans (e.g. via Nessus, Qualys, or "
               "osquery) to detect software that is not on the approved list");
    return r;
}

// CHECK 3.5: Unapproved software removal policy is defined
int sysaudit_check_software_removal_policy(const char* config_data) {
    if (!config_data) return 0;
    if (strstr(config_data, "software_removal_policy: defined") ||
        strstr(config_data, "auto_remove_unapproved: true") ||
        strstr(config_data, "unapproved_software_action: remove") ||
        strstr(config_data, "software_quarantine: enabled")) {
        return 1;
    }
    return 0;
}

sysaudit_check_result_t* create_sysaudit_software_removal_result(int passed, const char* details) {
    sysaudit_check_result_t* r = static_cast<sysaudit_check_result_t*>(calloc(1, sizeof(sysaudit_check_result_t)));
    if (!r) return nullptr;
    r->passed       = passed;
    r->control_id   = strdup("SYSAUDIT-SW-05");
    r->control_name = strdup("Unapproved Software Removal Policy");
    r->severity     = passed ? "INFO" : "MEDIUM";
    r->details      = details ? strdup(details) : strdup(passed ?
        "A policy for removing or quarantining unapproved software is defined" :
        "No unapproved software removal/quarantine policy is defined");
    r->remediation  = passed ? nullptr :
        strdup("Define and automate a remediation workflow that removes or quarantines "
               "software detected outside the approved list within an SLA timeframe");
    return r;
}

// ============================================================================
//  CLEANUP
// ============================================================================
void free_sysaudit_check_result(sysaudit_check_result_t* result) {
    if (!result) return;
    if (result->control_id)   free(result->control_id);
    if (result->control_name) free(result->control_name);
    if (result->details)      free(result->details);
    if (result->remediation)  free(result->remediation);
    free(result);
}
