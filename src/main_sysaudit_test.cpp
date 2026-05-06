#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "grc_scanner.hpp"
#include "frameworks/sysaudit.hpp"

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"

// ── Helpers ──────────────────────────────────────────────────────────────────

void print_line(char c, int length) {
    for (int i = 0; i < length; i++) printf("%c", c);
    printf("\n");
}

void print_box_header(const char *title) {
    printf("\n");
    print_line('=', 80);
    printf("%s%s%s%s\n", COLOR_BOLD, COLOR_CYAN, title, COLOR_RESET);
    print_line('=', 80);
}

void print_section_header(const char *title) {
    printf("\n");
    print_line('-', 80);
    printf("%s%s%s%s\n", COLOR_BOLD, COLOR_BLUE, title, COLOR_RESET);
    print_line('-', 80);
}

void print_check_result(const sysaudit_check_result_t *result) {
    const char *status_color = result->passed ? COLOR_GREEN : COLOR_RED;
    const char *status_text  = result->passed ? "PASS" : "FAIL";
    printf("\n┌─ [%s%s%s] %s%s - %s%s\n",
           status_color, status_text, COLOR_RESET,
           COLOR_BOLD, result->control_id, result->control_name, COLOR_RESET);
    printf("│  %sSeverity:%s %s\n", COLOR_BOLD, COLOR_RESET, result->severity);
    if (result->details)
        printf("│  %sDetails:%s %s\n", COLOR_BOLD, COLOR_RESET, result->details);
    if (!result->passed && result->remediation)
        printf("│  %s%sRemediation:%s %s\n",
               COLOR_YELLOW, COLOR_BOLD, COLOR_RESET, result->remediation);
    printf("└─────────────────────────────────────────────────────────────────\n");
}

void print_module_summary(const char *module_name,
                          const sysaudit_scan_result_t *scan) {
    double score = scan->result_count > 0
        ? (double)scan->passed_count / scan->result_count * 100.0
        : 0.0;
    printf("\n  %s%-30s%s  Passed: %s%zu%s / %zu   Score: %s%.1f%%%s\n",
           COLOR_BOLD, module_name, COLOR_RESET,
           COLOR_GREEN, scan->passed_count, COLOR_RESET, scan->result_count,
           score >= 80.0 ? COLOR_GREEN : COLOR_RED, score, COLOR_RESET);
}

// ── Test configuration strings ────────────────────────────────────────────────

char* create_auto_update_config() {
    return strdup(
        "# Auto-Update Configuration\n"
        "auto_update: enabled\n"
        "automatic_updates: true\n"
        "unattended_upgrades: enabled\n"
        "security_updates_only: true\n"
        "update_type: security\n"
        "update_schedule: weekly\n"
        "maintenance_window: sunday-02:00\n"
        "reboot_after_update: true\n"
        "auto_reboot: scheduled\n"
        "update_notifications: enabled\n"
        "patch_alerts: true\n"
    );
}

char* create_ssh_audit_config() {
    return strdup(
        "# SSH Key Audit Configuration\n"
        "ssh_password_auth: disabled\n"
        "password_authentication: false\n"
        "ssh_root_login: disabled\n"
        "permit_root_login: false\n"
        "ssh_key_rotation: enabled\n"
        "key_rotation_days: 90\n"
        "ssh_key_algorithm: ed25519\n"
        "strong_ssh_keys: true\n"
        "ssh_passphrase_required: true\n"
        "authorized_keys_managed: true\n"
        "ssh_ca: enabled\n"
    );
}

char* create_unapproved_software_config() {
    return strdup(
        "# Unapproved Software Controls\n"
        "software_whitelist: enabled\n"
        "application_allowlist: true\n"
        "software_inventory: enabled\n"
        "asset_inventory: maintained\n"
        "software_install_policy: restricted\n"
        "admin_only_install: true\n"
        "software_scan: enabled\n"
        "unapproved_software_scan: true\n"
        "software_removal_policy: defined\n"
        "auto_remove_unapproved: true\n"
    );
}

// ── Main ──────────────────────────────────────────────────────────────────────

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;

    // Banner
    printf("%s%s", COLOR_BOLD, COLOR_CYAN);
    printf("\n");
    printf("  ███████╗██╗   ██╗███████╗ █████╗ ██╗   ██╗██████╗ ██╗████████╗\n");
    printf("  ██╔════╝╚██╗ ██╔╝██╔════╝██╔══██╗██║   ██║██╔══██╗██║╚══██╔══╝\n");
    printf("  ███████╗ ╚████╔╝ ███████╗███████║██║   ██║██║  ██║██║   ██║   \n");
    printf("  ╚════██║  ╚██╔╝  ╚════██║██╔══██║██║   ██║██║  ██║██║   ██║   \n");
    printf("  ███████║   ██║   ███████║██║  ██║╚██████╔╝██████╔╝██║   ██║   \n");
    printf("  ╚══════╝   ╚═╝   ╚══════╝╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚═╝   ╚═╝  \n");
    printf("%s\n", COLOR_RESET);
    printf("%s        System Audit Scanner – 3 Security Modules%s\n", COLOR_BOLD, COLOR_RESET);
    printf("%s  Auto-Update Status | SSH Key Audit | Unapproved Software%s\n\n",
           COLOR_BOLD, COLOR_RESET);

    int overall_pass = 1;

    // ── MODULE 1: Auto-Update Status ─────────────────────────────────────────
    print_box_header("MODULE 1 – AUTO-UPDATE STATUS");

    char *upd_config = create_auto_update_config();
    if (!upd_config) { fprintf(stderr, "OOM\n"); return 1; }

    printf("%sTest Configuration:%s\n%s%s%s\n",
           COLOR_BOLD, COLOR_RESET, COLOR_YELLOW, upd_config, COLOR_RESET);

    print_section_header("Auto-Update Checks");
    sysaudit_scan_result_t *upd_result = sysaudit_scan_auto_update(upd_config);
    if (!upd_result) { fprintf(stderr, "Scan failed\n"); free(upd_config); return 1; }

    for (size_t i = 0; i < upd_result->result_count; i++)
        print_check_result(upd_result->results[i]);

    double upd_score = upd_result->result_count > 0
        ? (double)upd_result->passed_count / upd_result->result_count * 100.0 : 0.0;

    printf("\n  Total: %zu  |  Passed: %s%zu%s  |  Failed: %s%zu%s  |  Score: %s%.1f%%%s\n",
           upd_result->result_count,
           COLOR_GREEN, upd_result->passed_count, COLOR_RESET,
           COLOR_RED,   upd_result->failed_count, COLOR_RESET,
           upd_score >= 80.0 ? COLOR_GREEN : COLOR_RED, upd_score, COLOR_RESET);

    if (upd_score < 80.0) overall_pass = 0;

    free_sysaudit_scan_result(upd_result);
    free(upd_config);

    // ── MODULE 2: SSH Key Audit ───────────────────────────────────────────────
    print_box_header("MODULE 2 – SSH KEY AUDIT");

    char *ssh_config = create_ssh_audit_config();
    if (!ssh_config) { fprintf(stderr, "OOM\n"); return 1; }

    printf("%sTest Configuration:%s\n%s%s%s\n",
           COLOR_BOLD, COLOR_RESET, COLOR_YELLOW, ssh_config, COLOR_RESET);

    print_section_header("SSH Key Audit Checks");
    sysaudit_scan_result_t *ssh_result = sysaudit_scan_ssh_keys(ssh_config);
    if (!ssh_result) { fprintf(stderr, "Scan failed\n"); free(ssh_config); return 1; }

    for (size_t i = 0; i < ssh_result->result_count; i++)
        print_check_result(ssh_result->results[i]);

    double ssh_score = ssh_result->result_count > 0
        ? (double)ssh_result->passed_count / ssh_result->result_count * 100.0 : 0.0;

    printf("\n  Total: %zu  |  Passed: %s%zu%s  |  Failed: %s%zu%s  |  Score: %s%.1f%%%s\n",
           ssh_result->result_count,
           COLOR_GREEN, ssh_result->passed_count, COLOR_RESET,
           COLOR_RED,   ssh_result->failed_count, COLOR_RESET,
           ssh_score >= 80.0 ? COLOR_GREEN : COLOR_RED, ssh_score, COLOR_RESET);

    if (ssh_score < 80.0) overall_pass = 0;

    free_sysaudit_scan_result(ssh_result);
    free(ssh_config);

    // ── MODULE 3: Unapproved Software ────────────────────────────────────────
    print_box_header("MODULE 3 – UNAPPROVED SOFTWARE");

    char *sw_config = create_unapproved_software_config();
    if (!sw_config) { fprintf(stderr, "OOM\n"); return 1; }

    printf("%sTest Configuration:%s\n%s%s%s\n",
           COLOR_BOLD, COLOR_RESET, COLOR_YELLOW, sw_config, COLOR_RESET);

    print_section_header("Unapproved Software Checks");
    sysaudit_scan_result_t *sw_result = sysaudit_scan_unapproved_software(sw_config);
    if (!sw_result) { fprintf(stderr, "Scan failed\n"); free(sw_config); return 1; }

    for (size_t i = 0; i < sw_result->result_count; i++)
        print_check_result(sw_result->results[i]);

    double sw_score = sw_result->result_count > 0
        ? (double)sw_result->passed_count / sw_result->result_count * 100.0 : 0.0;

    printf("\n  Total: %zu  |  Passed: %s%zu%s  |  Failed: %s%zu%s  |  Score: %s%.1f%%%s\n",
           sw_result->result_count,
           COLOR_GREEN, sw_result->passed_count, COLOR_RESET,
           COLOR_RED,   sw_result->failed_count, COLOR_RESET,
           sw_score >= 80.0 ? COLOR_GREEN : COLOR_RED, sw_score, COLOR_RESET);

    if (sw_score < 80.0) overall_pass = 0;

    free_sysaudit_scan_result(sw_result);
    free(sw_config);

    // ── Overall Summary ───────────────────────────────────────────────────────
    print_box_header("SYSTEM AUDIT OVERALL SUMMARY");
    printf("\n");
    printf("  %-35s %s%.1f%%%s\n", "Auto-Update Status Score:",
           upd_score >= 80.0 ? COLOR_GREEN : COLOR_RED, upd_score, COLOR_RESET);
    printf("  %-35s %s%.1f%%%s\n", "SSH Key Audit Score:",
           ssh_score >= 80.0 ? COLOR_GREEN : COLOR_RED, ssh_score, COLOR_RESET);
    printf("  %-35s %s%.1f%%%s\n", "Unapproved Software Score:",
           sw_score >= 80.0 ? COLOR_GREEN : COLOR_RED, sw_score, COLOR_RESET);
    printf("\n");

    if (overall_pass)
        printf("  %s✓ PASSED – All system audit modules meet the security threshold%s\n",
               COLOR_GREEN, COLOR_RESET);
    else {
        printf("  %s✗ FAILED – One or more modules did not meet the security threshold%s\n",
               COLOR_RED, COLOR_RESET);
        printf("  %sPlease review and remediate the failed checks above%s\n",
               COLOR_YELLOW, COLOR_RESET);
    }

    print_line('=', 80);
    printf("\n");

    return overall_pass ? 0 : 1;
}
