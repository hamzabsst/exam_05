#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

SUBJECT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd) || exit 1
USER_DIR="$SUBJECT_DIR/.."
BUILD_DIR=$(mktemp -d "${TMPDIR:-/tmp}/polyset-test.XXXXXX") || exit 1
trap 'rm -rf -- "$BUILD_DIR"' EXIT

SOURCES=(
    "$SUBJECT_DIR/array_bag.cpp"
    "$SUBJECT_DIR/tree_bag.cpp"
    "$USER_DIR/searchable_array_bag.cpp"
    "$USER_DIR/searchable_tree_bag.cpp"
    "$USER_DIR/set.cpp"
)

compile_program() {
    g++ -Wall -Wextra -Werror -std=c++98 -g \
        -I"$USER_DIR" -I"$SUBJECT_DIR" "$1" "${SOURCES[@]}" -o "$2"
}

echo -e "${BLUE}📦 Compiling polyset and comparison tests...${NC}"
if ! compile_program "$SUBJECT_DIR/main.cpp" "$BUILD_DIR/polyset" || \
   ! compile_program "$SUBJECT_DIR/tests/compare_has.cpp" "$BUILD_DIR/polyset_tests"; then
    echo -e "${RED}❌ Compilation failed!${NC}"
    exit 1
fi
echo -e "${GREEN}✅ Compilation successful!${NC}"

# main.cpp needs at least one number. Accept custom inputs or use a sample.
if [ "$#" -eq 0 ]; then
    set -- 3 1 2 3
fi

echo -e "${BLUE}🚀 Running polyset with: $*${NC}"
"$BUILD_DIR/polyset" "$@"
run_status=$?
if [ "$run_status" -ne 0 ]; then
    echo -e "${RED}❌ Program exited with status $run_status.${NC}"
    exit "$run_status"
fi

echo -e "${BLUE}🚀 Comparing tree/array has() with expected membership...${NC}"
"$BUILD_DIR/polyset_tests"
test_status=$?
if [ "$test_status" -ne 0 ]; then
    echo -e "${RED}❌ Comparison tests failed (status $test_status).${NC}"
    exit "$test_status"
fi

if ! command -v valgrind > /dev/null 2>&1; then
    echo -e "${YELLOW}Valgrind is not installed; memory checks skipped.${NC}"
    exit 0
fi

echo -e "${BLUE}🚀 Checking memory with valgrind...${NC}"
check_memory() {
    local program="$1"
    shift
    valgrind --leak-check=full --show-leak-kinds=all \
        --errors-for-leak-kinds=all --track-origins=yes -s \
        --error-exitcode=1 "$BUILD_DIR/$program" "$@" \
        > "$BUILD_DIR/$program.valgrind.txt" 2>&1
    local status=$?
    cat "$BUILD_DIR/$program.valgrind.txt"
    return "$status"
}
valgrind_status=0
check_memory polyset "$@" || valgrind_status=1
check_memory polyset_tests || valgrind_status=1

if [ "$valgrind_status" -eq 0 ]; then
    echo -e "${GREEN}✅ Sample and comparison tests passed; no memory errors reported.${NC}"
else
    echo -e "${RED}❌ Valgrind reported a problem (status $valgrind_status).${NC}"
fi
exit "$valgrind_status"
