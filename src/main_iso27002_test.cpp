#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "grc_scanner.hpp"
#include "frameworks/iso27002.hpp"

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"

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

void print_check_result(const iso27002_check_result_t *result) {
    const char *status_color = result->passed ? COLOR_GREEN : COLOR_RED;
    const char *status_text = result->passed ? "PASS" : "FAIL";
    printf("\n┌─ [%s%s%s] %s%s - %s%s\n",
           status_color, status_text, COLOR_RESET,
           COLOR_BOLD, result->control_id, result->control_name, COLOR_RESET);
    printf("│  %sSeverity:%s %s\n", COLOR_BOLD, COLOR_RESET, result->severity);
    if (result->details)
        printf("│  %sDetails:%s %s\n", COLOR_BOLD, COLOR_RESET, result->details);
    if (!result->passed && result->remediation)
        printf("│  %s%sRemediation:%s %s\n", COLOR_YELLOW, COLOR_BOLD, COLOR_RESET, result->remediation);
    printf("└─────────────────────────────────────────────────────────────────\n");
}

char* create_test_config_string() {
    const char *config_template =
        "# ISO 27002 Security Controls Configuration\n"
        "asset_inventory: maintained\n"
        "asset_management: enabled\n"
        "asset_tracking: true\n"
        "cmdb: enabled\n"
        "background_checks: enabled\n"
        "hr_security: enforced\n"
        "employee_screening: true\n"
        "security_training: required\n"
        "awareness_program: active\n"
        "training_completed: true\n"
        "vulnerability_scanning: enabled\n"
        "patch_management: automated\n"
        "security_scanning: true\n"
        "antivirus: enabled\n"
        "malware_protection: active\n"
        "edr: deployed\n"
        "endpoint_protection: true\n"
        "logging: enabled\n"
        "siem: deployed\n"
        "log_monitoring: active\n"
        "audit_log: enabled\n"
        "secure_coding: enforced\n"
        "code_review: required\n"
        "sast: enabled\n"
        "change_control: enforced\n"
        "change_management: enabled\n"
        "approval_workflow: required\n"
        "capacity_planning: enabled\n"
        "resource_monitoring: active\n"
        "performance_monitoring: true\n"
        "cloud_security: enabled\n"
        "cspm: deployed\n"
        "cloud_controls: enforced\n";
    return strdup(config_template);
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    printf("%s%s", COLOR_BOLD, COLOR_CYAN);
    printf("\n");
    printf("  ██╗███████╗ ██████╗     ██████╗ ███████╗ ██████╗  ██████╗ ██████╗ \n");
    printf("  ██║██╔════╝██╔═══██╗    ╚════██╗╚════██║██╔═████╗██╔═████╗╚════██╗\n");
    printf("  ██║███████╗██║   ██║     █████╔╝    ██╔╝██║██╔██║██║██╔██║ █████╔╝\n");
    printf("  ██║╚════██║██║   ██║    ██╔═══╝    ██╔╝ ████╔╝██║████╔╝██║██╔═══╝ \n");
    printf("  ██║███████║╚██████╔╝    ███████╗   ██║  ╚██████╔╝╚██████╔╝███████╗\n");
    printf("  ╚═╝╚══════╝ ╚═════╝     ╚══════╝   ╚═╝   ╚═════╝  ╚═════╝ ╚══════╝\n");
    printf("%s\n", COLOR_RESET);
    printf("%s            ISO 27002 Security Controls Scanner\n", COLOR_BOLD);
    printf("                 Code of Practice for Security%s\n\n", COLOR_RESET);

    char *config_data = create_test_config_string();
    if (!config_data) {
        fprintf(stderr, "%sError: Failed to create test configuration%s\n", COLOR_RED, COLOR_RESET);
        return 1;
    }

    printf("%sTest Configuration:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("%s%s%s\n", COLOR_YELLOW, config_data, COLOR_RESET);

    print_box_header("RUNNING ISO 27002 COMPLIANCE CHECKS");

    iso27002_scan_result_t *scan_result = iso27002_scan_config(config_data);
    if (!scan_result) {
        fprintf(stderr, "%sError: Scan failed%s\n", COLOR_RED, COLOR_RESET);
        free(config_data);
        return 1;
    }

    print_box_header("SCAN RESULTS");
    for (size_t i = 0; i < scan_result->result_count; i++) {
        print_check_result(scan_result->results[i]);
    }

    print_box_header("ISO 27002 COMPLIANCE SUMMARY");

    double compliance_score = scan_result->result_count > 0
        ? (double)scan_result->passed_count / scan_result->result_count * 100.0
        : 0.0;

    printf("\n");
    printf("  Total Checks:    %s%zu%s\n", COLOR_BOLD, scan_result->result_count, COLOR_RESET);
    printf("  %sPassed:%s          %s%zu%s\n", COLOR_GREEN, COLOR_RESET, COLOR_BOLD, scan_result->passed_count, COLOR_RESET);
    printf("  %sFailed:%s          %s%zu%s\n", COLOR_RED, COLOR_RESET, COLOR_BOLD, scan_result->failed_count, COLOR_RESET);
    printf("  Compliance Score: %s%.1f%%%s\n",
           compliance_score >= 80.0 ? COLOR_GREEN : COLOR_RED, compliance_score, COLOR_RESET);
    printf("\n");

    if (compliance_score >= 80.0)
        printf("  %s✓ PASSED - Configuration meets ISO 27002 compliance requirements%s\n", COLOR_GREEN, COLOR_RESET);
    else {
        printf("  %s✗ FAILED - Configuration does not meet ISO 27002 compliance requirements%s\n", COLOR_RED, COLOR_RESET);
        printf("  %sPlease review and remediate the failed checks above%s\n", COLOR_YELLOW, COLOR_RESET);
    }

    print_line('=', 80);
    printf("\n");

    free_iso27002_scan_result(scan_result);
    free(config_data);

    return compliance_score >= 80.0 ? 0 : 1;
}
