#!/bin/bash
# Usage:
#   ./run_all_tests.sh              # Run all framework tests
#   ./run_all_tests.sh hipaa        # Run HIPAA test only
#   ./run_all_tests.sh iso27001     # Run ISO 27001 test only
#   ./run_all_tests.sh iso27002     # Run ISO 27002 test only
#   ./run_all_tests.sh apps         # Run APPs test only

FRAMEWORK="${1:-all}"

run_hipaa() {
    echo "1️⃣  HIPAA Compliance Test"
    echo "------------------------------------------------------------------------"
    ./complyd-scan-hipaa 2>&1 | tail -20
    echo ""
}

run_iso27001() {
    echo "2️⃣  ISO 27001 ISMS Test"
    echo "------------------------------------------------------------------------"
    ./complyd-scan-iso27001 2>&1 | tail -20
    echo ""
}

run_iso27002() {
    echo "3️⃣  ISO 27002 Security Controls Test"
    echo "------------------------------------------------------------------------"
    ./complyd-scan-iso27002 2>&1 | tail -20
    echo ""
}

run_apps() {
    echo "4️⃣  Application Security (APPs) Test"
    echo "------------------------------------------------------------------------"
    ./complyd-scan-apps 2>&1 | tail -20
    echo ""
}

case "$FRAMEWORK" in
    hipaa)
        echo "========================================================================"
        echo "                    HIPAA Compliance Test"
        echo "========================================================================"
        echo ""
        run_hipaa
        ;;
    iso27001)
        echo "========================================================================"
        echo "                    ISO 27001 ISMS Test"
        echo "========================================================================"
        echo ""
        run_iso27001
        ;;
    iso27002)
        echo "========================================================================"
        echo "                ISO 27002 Security Controls Test"
        echo "========================================================================"
        echo ""
        run_iso27002
        ;;
    apps)
        echo "========================================================================"
        echo "               Application Security (APPs) Test"
        echo "========================================================================"
        echo ""
        run_apps
        ;;
    all)
        echo "========================================================================"
        echo "           Running All Compliance Framework Tests"
        echo "========================================================================"
        echo ""
        run_hipaa
        run_iso27001
        run_iso27002
        run_apps
        echo "========================================================================"
        echo "                    All Tests Completed!"
        echo "========================================================================"
        ;;
    *)
        echo "Unknown framework: $FRAMEWORK"
        echo ""
        echo "Usage: $0 [hipaa|iso27001|iso27002|apps|all]"
        echo ""
        echo "  hipaa     - Run HIPAA compliance test"
        echo "  iso27001  - Run ISO 27001 ISMS test"
        echo "  iso27002  - Run ISO 27002 Security Controls test"
        echo "  apps      - Run Application Security test"
        echo "  all       - Run all tests (default)"
        exit 1
        ;;
esac
