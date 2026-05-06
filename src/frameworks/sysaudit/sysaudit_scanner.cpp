#include "frameworks/sysaudit.hpp"
#include <cstdlib>
#include <cstring>

// ── Helper: allocate a fresh scan result with capacity for `cap` checks ──────
static sysaudit_scan_result_t* alloc_scan_result(size_t cap) {
    sysaudit_scan_result_t* r =
        static_cast<sysaudit_scan_result_t*>(calloc(1, sizeof(sysaudit_scan_result_t)));
    if (!r) return nullptr;
    r->results = static_cast<sysaudit_check_result_t**>(
        calloc(cap, sizeof(sysaudit_check_result_t*)));
    if (!r->results) { free(r); return nullptr; }
    return r;
}

#define ADD_CHECK(scan, fn_check, fn_create) \
    do { \
        int _p = (fn_check)(config_data); \
        (scan)->results[(scan)->result_count++] = (fn_create)(_p, nullptr); \
        if (_p) (scan)->passed_count++; else (scan)->failed_count++; \
    } while (0)

// ============================================================================
//  MODULE 1 – AUTO-UPDATE STATUS  (5 checks)
// ============================================================================
sysaudit_scan_result_t* sysaudit_scan_auto_update(const char* config_data) {
    if (!config_data) return nullptr;

    sysaudit_scan_result_t* result = alloc_scan_result(5);
    if (!result) return nullptr;

    ADD_CHECK(result, sysaudit_check_auto_update_enabled,       create_sysaudit_auto_update_enabled_result);
    ADD_CHECK(result, sysaudit_check_auto_update_security_only, create_sysaudit_auto_update_security_result);
    ADD_CHECK(result, sysaudit_check_auto_update_schedule,      create_sysaudit_auto_update_schedule_result);
    ADD_CHECK(result, sysaudit_check_auto_update_reboot_policy, create_sysaudit_auto_update_reboot_result);
    ADD_CHECK(result, sysaudit_check_auto_update_notifications, create_sysaudit_auto_update_notify_result);

    return result;
}

// ============================================================================
//  MODULE 2 – SSH KEY AUDIT  (6 checks)
// ============================================================================
sysaudit_scan_result_t* sysaudit_scan_ssh_keys(const char* config_data) {
    if (!config_data) return nullptr;

    sysaudit_scan_result_t* result = alloc_scan_result(6);
    if (!result) return nullptr;

    ADD_CHECK(result, sysaudit_check_ssh_key_auth_only,           create_sysaudit_ssh_key_auth_result);
    ADD_CHECK(result, sysaudit_check_ssh_root_login_disabled,     create_sysaudit_ssh_root_login_result);
    ADD_CHECK(result, sysaudit_check_ssh_key_rotation,            create_sysaudit_ssh_key_rotation_result);
    ADD_CHECK(result, sysaudit_check_ssh_key_algorithm,           create_sysaudit_ssh_key_algorithm_result);
    ADD_CHECK(result, sysaudit_check_ssh_passphrase_required,     create_sysaudit_ssh_passphrase_result);
    ADD_CHECK(result, sysaudit_check_ssh_authorized_keys_managed, create_sysaudit_ssh_authorized_keys_result);

    return result;
}

// ============================================================================
//  MODULE 3 – UNAPPROVED SOFTWARE  (5 checks)
// ============================================================================
sysaudit_scan_result_t* sysaudit_scan_unapproved_software(const char* config_data) {
    if (!config_data) return nullptr;

    sysaudit_scan_result_t* result = alloc_scan_result(5);
    if (!result) return nullptr;

    ADD_CHECK(result, sysaudit_check_software_whitelist,       create_sysaudit_software_whitelist_result);
    ADD_CHECK(result, sysaudit_check_software_inventory,       create_sysaudit_software_inventory_result);
    ADD_CHECK(result, sysaudit_check_software_install_policy,  create_sysaudit_software_install_policy_result);
    ADD_CHECK(result, sysaudit_check_software_scan_enabled,    create_sysaudit_software_scan_result);
    ADD_CHECK(result, sysaudit_check_software_removal_policy,  create_sysaudit_software_removal_result);

    return result;
}

// ============================================================================
//  COMBINED SCAN – all 16 checks in one pass
// ============================================================================
sysaudit_scan_result_t* sysaudit_scan_all(const char* config_data) {
    if (!config_data) return nullptr;

    sysaudit_scan_result_t* result = alloc_scan_result(16);
    if (!result) return nullptr;

    // --- Auto-update (5) ---
    ADD_CHECK(result, sysaudit_check_auto_update_enabled,       create_sysaudit_auto_update_enabled_result);
    ADD_CHECK(result, sysaudit_check_auto_update_security_only, create_sysaudit_auto_update_security_result);
    ADD_CHECK(result, sysaudit_check_auto_update_schedule,      create_sysaudit_auto_update_schedule_result);
    ADD_CHECK(result, sysaudit_check_auto_update_reboot_policy, create_sysaudit_auto_update_reboot_result);
    ADD_CHECK(result, sysaudit_check_auto_update_notifications, create_sysaudit_auto_update_notify_result);

    // --- SSH Key Audit (6) ---
    ADD_CHECK(result, sysaudit_check_ssh_key_auth_only,           create_sysaudit_ssh_key_auth_result);
    ADD_CHECK(result, sysaudit_check_ssh_root_login_disabled,     create_sysaudit_ssh_root_login_result);
    ADD_CHECK(result, sysaudit_check_ssh_key_rotation,            create_sysaudit_ssh_key_rotation_result);
    ADD_CHECK(result, sysaudit_check_ssh_key_algorithm,           create_sysaudit_ssh_key_algorithm_result);
    ADD_CHECK(result, sysaudit_check_ssh_passphrase_required,     create_sysaudit_ssh_passphrase_result);
    ADD_CHECK(result, sysaudit_check_ssh_authorized_keys_managed, create_sysaudit_ssh_authorized_keys_result);

    // --- Unapproved Software (5) ---
    ADD_CHECK(result, sysaudit_check_software_whitelist,       create_sysaudit_software_whitelist_result);
    ADD_CHECK(result, sysaudit_check_software_inventory,       create_sysaudit_software_inventory_result);
    ADD_CHECK(result, sysaudit_check_software_install_policy,  create_sysaudit_software_install_policy_result);
    ADD_CHECK(result, sysaudit_check_software_scan_enabled,    create_sysaudit_software_scan_result);
    ADD_CHECK(result, sysaudit_check_software_removal_policy,  create_sysaudit_software_removal_result);

    return result;
}

// ============================================================================
//  CLEANUP
// ============================================================================
void free_sysaudit_scan_result(sysaudit_scan_result_t* result) {
    if (!result) return;
    if (result->results) {
        for (size_t i = 0; i < result->result_count; i++)
            free_sysaudit_check_result(result->results[i]);
        free(result->results);
    }
    free(result);
}
