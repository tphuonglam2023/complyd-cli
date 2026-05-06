#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "grc_scanner.hpp"
#include "frameworks/hipaa.hpp"
#include "frameworks/sysaudit.hpp"
#include "parsers/file_parsers.hpp"

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
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

void print_check_result(const check_result_t *result) {
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

void print_usage(const char *program_name) {
    printf("Usage: %s <config-file>\n\n", program_name);
    printf("Supported file formats:\n");
    printf("  - Markdown (.md, .markdown)\n");
    printf("  - JSON (.json)\n");
    printf("  - PDF (.pdf)\n");
    printf("  - Text/YAML (.txt, .yaml, .yml, .conf, .config)\n\n");
    printf("Example:\n");
    printf("  %s config.json\n", program_name);
    printf("  %s security-policy.md\n", program_name);
    printf("  %s compliance-doc.pdf\n", program_name);
}

void print_sysaudit_check_result(const sysaudit_check_result_t *result) {
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

void print_banner() {
    printf("%s%s", COLOR_BOLD, COLOR_CYAN);
    printf("\n");
    printf("   ██████╗ ██████╗ ███╗   ███╗██████╗ ██╗  ██╗   ██╗██████╗ \n");
    printf("  ██╔════╝██╔═══██╗████╗ ████║██╔══██╗██║  ╚██╗ ██╔╝██╔══██╗\n");
    printf("  ██║     ██║   ██║██╔████╔██║██████╔╝██║   ╚████╔╝ ██║  ██║\n");
    printf("  ██║     ██║   ██║██║╚██╔╝██║██╔═══╝ ██║    ╚██╔╝  ██║  ██║\n");
    printf("  ╚██████╗╚██████╔╝██║ ╚═╝ ██║██║     ███████╗██║   ██████╔╝\n");
    printf("   ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝   ╚═════╝ \n");
    printf("%s\n", COLOR_RESET);
    printf("%s                    Complyd Scanner v1.0%s\n\n", COLOR_BOLD, COLOR_RESET);
}

int main(int argc, char *argv[]) {
    print_banner();

    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    file_type_t file_type = detect_file_type(filename);
    const char *file_type_str = "Unknown";

    switch (file_type) {
        case FILE_TYPE_MD:   file_type_str = "Markdown"; break;
        case FILE_TYPE_JSON: file_type_str = "JSON"; break;
        case FILE_TYPE_PDF:  file_type_str = "PDF"; break;
        case FILE_TYPE_YAML: file_type_str = "YAML"; break;
        case FILE_TYPE_TEXT: file_type_str = "Text"; break;
        default:             file_type_str = "Unknown"; break;
    }

    printf("%sScanning file:%s %s\n", COLOR_BOLD, COLOR_RESET, filename);
    printf("%sFile type:%s %s\n", COLOR_BOLD, COLOR_RESET, file_type_str);

    print_box_header("PARSING CONFIGURATION FILE");

    parse_result_t *parse_result = parse_file(filename);

    if (!parse_result || !parse_result->success) {
        fprintf(stderr, "%sError parsing file:%s %s\n",
                COLOR_RED, COLOR_RESET,
                parse_result && parse_result->error_message ? parse_result->error_message : "Unknown error");
        if (parse_result) free_parse_result(parse_result);
        return 1;
    }

    printf("%s✓ Successfully parsed file (%zu bytes)%s\n",
           COLOR_GREEN, parse_result->content_length, COLOR_RESET);

    printf("\n%sParsed Configuration (preview):%s\n", COLOR_BOLD, COLOR_RESET);
    print_line('-', 80);

    size_t preview_len = parse_result->content_length < 500 ? parse_result->content_length : 500;
    printf("%s%.*s%s", COLOR_YELLOW, (int)preview_len, parse_result->content, COLOR_RESET);
    if (parse_result->content_length > 500) printf("\n... (truncated)");
    printf("\n");
    print_line('-', 80);

    // ── HIPAA ────────────────────────────────────────────────────────────────
    print_box_header("RUNNING HIPAA COMPLIANCE CHECKS");

    scan_result_t *scan_result = hipaa_scan_config(parse_result->content);

    if (!scan_result) {
        fprintf(stderr, "%sError: HIPAA scan failed%s\n", COLOR_RED, COLOR_RESET);
        free_parse_result(parse_result);
        return 1;
    }

    print_box_header("HIPAA SCAN RESULTS");

    for (size_t i = 0; i < scan_result->result_count; i++) {
        print_check_result(scan_result->results[i]);
    }

    double hipaa_score = scan_result->result_count > 0
        ? (double)scan_result->passed_count / scan_result->result_count * 100.0
        : 0.0;

    printf("\n  %sHIPAA Score:%s %s%.1f%%%s  (%zu passed / %zu total)\n",
           COLOR_BOLD, COLOR_RESET,
           hipaa_score >= 80.0 ? COLOR_GREEN : COLOR_RED, hipaa_score, COLOR_RESET,
           scan_result->passed_count, scan_result->result_count);

    free_scan_result(scan_result);

    // ── AUTO-UPDATE STATUS ───────────────────────────────────────────────────
    print_box_header("RUNNING AUTO-UPDATE STATUS CHECKS");

    sysaudit_scan_result_t *upd_result = sysaudit_scan_auto_update(parse_result->content);
    if (!upd_result) {
        fprintf(stderr, "%sError: Auto-update scan failed%s\n", COLOR_RED, COLOR_RESET);
        free_parse_result(parse_result);
        return 1;
    }

    for (size_t i = 0; i < upd_result->result_count; i++)
        print_sysaudit_check_result(upd_result->results[i]);

    double upd_score = upd_result->result_count > 0
        ? (double)upd_result->passed_count / upd_result->result_count * 100.0 : 0.0;

    printf("\n  %sAuto-Update Score:%s %s%.1f%%%s  (%zu passed / %zu total)\n",
           COLOR_BOLD, COLOR_RESET,
           upd_score >= 80.0 ? COLOR_GREEN : COLOR_RED, upd_score, COLOR_RESET,
           upd_result->passed_count, upd_result->result_count);

    free_sysaudit_scan_result(upd_result);

    // ── SSH KEY AUDIT ────────────────────────────────────────────────────────
    print_box_header("RUNNING SSH KEY AUDIT CHECKS");

    sysaudit_scan_result_t *ssh_result = sysaudit_scan_ssh_keys(parse_result->content);
    if (!ssh_result) {
        fprintf(stderr, "%sError: SSH key audit scan failed%s\n", COLOR_RED, COLOR_RESET);
        free_parse_result(parse_result);
        return 1;
    }

    for (size_t i = 0; i < ssh_result->result_count; i++)
        print_sysaudit_check_result(ssh_result->results[i]);

    double ssh_score = ssh_result->result_count > 0
        ? (double)ssh_result->passed_count / ssh_result->result_count * 100.0 : 0.0;

    printf("\n  %sSSH Key Audit Score:%s %s%.1f%%%s  (%zu passed / %zu total)\n",
           COLOR_BOLD, COLOR_RESET,
           ssh_score >= 80.0 ? COLOR_GREEN : COLOR_RED, ssh_score, COLOR_RESET,
           ssh_result->passed_count, ssh_result->result_count);

    free_sysaudit_scan_result(ssh_result);

    // ── UNAPPROVED SOFTWARE ──────────────────────────────────────────────────
    print_box_header("RUNNING UNAPPROVED SOFTWARE CHECKS");

    sysaudit_scan_result_t *sw_result = sysaudit_scan_unapproved_software(parse_result->content);
    if (!sw_result) {
        fprintf(stderr, "%sError: Unapproved software scan failed%s\n", COLOR_RED, COLOR_RESET);
        free_parse_result(parse_result);
        return 1;
    }

    for (size_t i = 0; i < sw_result->result_count; i++)
        print_sysaudit_check_result(sw_result->results[i]);

    double sw_score = sw_result->result_count > 0
        ? (double)sw_result->passed_count / sw_result->result_count * 100.0 : 0.0;

    printf("\n  %sUnapproved Software Score:%s %s%.1f%%%s  (%zu passed / %zu total)\n",
           COLOR_BOLD, COLOR_RESET,
           sw_score >= 80.0 ? COLOR_GREEN : COLOR_RED, sw_score, COLOR_RESET,
           sw_result->passed_count, sw_result->result_count);

    free_sysaudit_scan_result(sw_result);

    // ── OVERALL SUMMARY ──────────────────────────────────────────────────────
    print_box_header("OVERALL COMPLIANCE SUMMARY");

    double overall_score = (hipaa_score + upd_score + ssh_score + sw_score) / 4.0;

    printf("\n");
    printf("  File:                      %s%s%s\n", COLOR_BOLD, filename, COLOR_RESET);
    printf("  File Type:                 %s%s%s\n", COLOR_BOLD, file_type_str, COLOR_RESET);
    printf("\n");
    printf("  %-30s %s%.1f%%%s\n", "HIPAA Compliance:",
           hipaa_score >= 80.0 ? COLOR_GREEN : COLOR_RED, hipaa_score, COLOR_RESET);
    printf("  %-30s %s%.1f%%%s\n", "Auto-Update Status:",
           upd_score >= 80.0 ? COLOR_GREEN : COLOR_RED, upd_score, COLOR_RESET);
    printf("  %-30s %s%.1f%%%s\n", "SSH Key Audit:",
           ssh_score >= 80.0 ? COLOR_GREEN : COLOR_RED, ssh_score, COLOR_RESET);
    printf("  %-30s %s%.1f%%%s\n", "Unapproved Software:",
           sw_score >= 80.0 ? COLOR_GREEN : COLOR_RED, sw_score, COLOR_RESET);
    printf("\n");
    printf("  %-30s %s%.1f%%%s\n", "Overall Score:",
           overall_score >= 80.0 ? COLOR_GREEN : COLOR_RED, overall_score, COLOR_RESET);
    printf("\n");

    if (overall_score >= 80.0) {
        printf("  %s✓ PASSED - Configuration meets all compliance requirements%s\n",
               COLOR_GREEN, COLOR_RESET);
    } else {
        printf("  %s✗ FAILED - Configuration does not meet all compliance requirements%s\n",
               COLOR_RED, COLOR_RESET);
        printf("  %sPlease review and remediate the failed checks above%s\n",
               COLOR_YELLOW, COLOR_RESET);
    }

    print_line('=', 80);
    printf("\n");

    free_parse_result(parse_result);

    return overall_score >= 80.0 ? 0 : 1;
}
