#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "grc_scanner.hpp"
#include "frameworks/apps.hpp"

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

void print_check_result(const apps_check_result_t *result) {
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
        "# Application Security Configuration\n"
        "strong_authentication: enabled\n"
        "password_policy: enforced\n"
        "mfa_enabled: true\n"
        "auth_mechanism: secure\n"
        "rbac: enabled\n"
        "authorization: enforced\n"
        "access_control: implemented\n"
        "least_privilege: true\n"
        "input_validation: enabled\n"
        "sanitization: enforced\n"
        "whitelist_validation: true\n"
        "input_filtering: enabled\n"
        "output_encoding: enabled\n"
        "xss_protection: true\n"
        "context_aware_encoding: true\n"
        "safe_rendering: enabled\n"
        "secure_sessions: enabled\n"
        "session_timeout: configured\n"
        "httponly_cookies: true\n"
        "secure_cookies: true\n"
        "strong_crypto: enabled\n"
        "encryption_at_rest: true\n"
        "tls: enforced\n"
        "key_management: secure\n"
        "secure_error_handling: enabled\n"
        "error_messages: sanitized\n"
        "stack_traces: disabled\n"
        "generic_errors: true\n"
        "security_logging: enabled\n"
        "audit_trail: maintained\n"
        "monitoring: active\n"
        "log_analysis: enabled\n"
        "data_classification: defined\n"
        "sensitive_data_protection: enabled\n"
        "data_masking: implemented\n"
        "pii_protection: enforced\n"
        "api_authentication: enforced\n"
        "rate_limiting: enabled\n"
        "api_gateway: deployed\n"
        "api_security: enabled\n"
        "dependency_scanning: enabled\n"
        "sca: implemented\n"
        "vulnerability_checks: automated\n"
        "package_management: secure\n"
        "security_headers: enabled\n"
        "hsts: enabled\n"
        "csp: configured\n"
        "x_frame_options: set\n";
    return strdup(config_template);
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    printf("%s%s", COLOR_BOLD, COLOR_CYAN);
    printf("\n");
    printf("   █████╗ ██████╗ ██████╗ ███████╗    ███████╗███████╗ ██████╗\n");
    printf("  ██╔══██╗██╔══██╗██╔══██╗██╔════╝    ██╔════╝██╔════╝██╔════╝\n");
    printf("  ███████║██████╔╝██████╔╝███████╗    ███████╗█████╗  ██║     \n");
    printf("  ██╔══██║██╔═══╝ ██╔═══╝ ╚════██║    ╚════██║██╔══╝  ██║     \n");
    printf("  ██║  ██║██║     ██║     ███████║    ███████║███████╗╚██████╗\n");
    printf("  ╚═╝  ╚═╝╚═╝     ╚═╝     ╚══════╝    ╚══════╝╚══════╝ ╚═════╝\n");
    printf("%s\n", COLOR_RESET);
    printf("%s          Application Security Best Practices Scanner\n", COLOR_BOLD);
    printf("              Secure Development & Deployment Checks%s\n\n", COLOR_RESET);

    char *config_data = create_test_config_string();
    if (!config_data) {
        fprintf(stderr, "%sError: Failed to create test configuration%s\n", COLOR_RED, COLOR_RESET);
        return 1;
    }

    printf("%sTest Configuration:%s\n", COLOR_BOLD, COLOR_RESET);
    printf("%s%s%s\n", COLOR_YELLOW, config_data, COLOR_RESET);

    print_box_header("RUNNING APPLICATION SECURITY CHECKS");

    apps_scan_result_t *scan_result = apps_scan_config(config_data);
    if (!scan_result) {
        fprintf(stderr, "%sError: Scan failed%s\n", COLOR_RED, COLOR_RESET);
        free(config_data);
        return 1;
    }

    print_box_header("SCAN RESULTS");
    for (size_t i = 0; i < scan_result->result_count; i++) {
        print_check_result(scan_result->results[i]);
    }

    print_box_header("APPLICATION SECURITY SUMMARY");

    double compliance_score = scan_result->result_count > 0
        ? (double)scan_result->passed_count / scan_result->result_count * 100.0
        : 0.0;

    printf("\n");
    printf("  Total Checks:    %s%zu%s\n", COLOR_BOLD, scan_result->result_count, COLOR_RESET);
    printf("  %sPassed:%s          %s%zu%s\n", COLOR_GREEN, COLOR_RESET, COLOR_BOLD, scan_result->passed_count, COLOR_RESET);
    printf("  %sFailed:%s          %s%zu%s\n", COLOR_RED, COLOR_RESET, COLOR_BOLD, scan_result->failed_count, COLOR_RESET);
    printf("  Security Score:   %s%.1f%%%s\n",
           compliance_score >= 80.0 ? COLOR_GREEN : COLOR_RED, compliance_score, COLOR_RESET);
    printf("\n");

    if (compliance_score >= 80.0)
        printf("  %s✓ PASSED - Application meets security best practices%s\n", COLOR_GREEN, COLOR_RESET);
    else {
        printf("  %s✗ FAILED - Application does not meet security best practices%s\n", COLOR_RED, COLOR_RESET);
        printf("  %sPlease review and remediate the failed checks above%s\n", COLOR_YELLOW, COLOR_RESET);
    }

    print_line('=', 80);
    printf("\n");

    free_apps_scan_result(scan_result);
    free(config_data);

    return compliance_score >= 80.0 ? 0 : 1;
}
