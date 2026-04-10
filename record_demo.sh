#!/bin/bash
# Demo recording script for Complyd Scanner
# Records: build phase + run_all_tests.sh + individual framework tests

set -e
cd /home/lamtp82/Documents/grc-scan

echo "========================================================================"
echo "  Complyd Scanner - GRC Compliance Tool Demo"
echo "  Recording: Build + Tests"
echo "========================================================================"
echo ""
sleep 1

# ── Part 1: Show project structure ──────────────────────────────────────────
echo "$ ls -lh"
ls -lh
sleep 1

echo ""
echo "$ ls src/frameworks/"
ls src/frameworks/
sleep 1

# ── Part 2: Clean build ──────────────────────────────────────────────────────
echo ""
echo "========================================================================"
echo "  PHASE 1: Building from source"
echo "========================================================================"
echo ""
sleep 1

echo "$ make clean"
make clean
sleep 1

echo ""
echo "$ make"
make 2>&1
sleep 1

echo ""
echo "$ ls -lh complyd-scan*"
ls -lh complyd-scan*
sleep 2

# ── Part 3: Run all tests ────────────────────────────────────────────────────
echo ""
echo "========================================================================"
echo "  PHASE 2: Running All Framework Tests"
echo "========================================================================"
echo ""
sleep 1

echo "$ ./run_all_tests.sh"
./run_all_tests.sh
sleep 2

# ── Part 4: Individual framework tests ──────────────────────────────────────
echo ""
echo "========================================================================"
echo "  PHASE 3: Individual Framework Tests"
echo "========================================================================"
echo ""
sleep 1

echo "$ ./run_all_tests.sh hipaa"
./run_all_tests.sh hipaa
sleep 1

echo "$ ./run_all_tests.sh iso27001"
./run_all_tests.sh iso27001
sleep 1

echo "$ ./run_all_tests.sh iso27002"
./run_all_tests.sh iso27002
sleep 1

echo "$ ./run_all_tests.sh apps"
./run_all_tests.sh apps
sleep 2

# ── Summary ──────────────────────────────────────────────────────────────────
echo ""
echo "========================================================================"
echo "  DEMO COMPLETE"
echo "  All compliance frameworks tested successfully!"
echo "  HIPAA | ISO 27001 | ISO 27002 | APPs - All PASSED ✓"
echo "========================================================================"
echo ""
sleep 2
