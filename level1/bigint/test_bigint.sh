#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}🔍 Testing bigint...${NC}"
echo "================================="

# ---------- CREATE MAIN ----------
cat > main.cpp << 'EOF'
#include "bigint.hpp"
#include <iostream>

int main()
{
    const bigint a(42);
    bigint b(21), c, d(1337), e(d);

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = " << c << std::endl;
    std::cout << "d = " << d << std::endl;
    std::cout << "e = " << e << std::endl;

    std::cout << "a + b = " << a + b << std::endl;
    std::cout << "(c += a) = " << (c += a) << std::endl;

    std::cout << "b = " << b << std::endl;
    std::cout << "++b = " << ++b << std::endl;
    std::cout << "b++ = " << b++ << std::endl;

    std::cout << "(b << 10) + 42 = " << ((b << 10) + 42) << std::endl;
    std::cout << "(d <<= 4) = " << (d <<= 4) << std::endl;
    std::cout << "(d >>= 2) = " << (d >>= (const bigint)2) << std::endl;

    std::cout << "a =" << a << std::endl;
    std::cout << "d =" << d << std::endl;

    std::cout << "(d < a) = " << (d < a) << std::endl;
    std::cout << "(d <= a) = " << (d <= a) << std::endl;
    std::cout << "(d > a) = " << (d > a) << std::endl;
    std::cout << "(d >= a) = " << (d >= a) << std::endl;
    std::cout << "(d == a) = " << (d == a) << std::endl;
    std::cout << "(d != a) = " << (d != a) << std::endl;
}
EOF

# ---------- EXPECTED OUTPUT ----------
cat > expected.txt << 'EOF'
a = 42
b = 21
c = 0
d = 1337
e = 1337
a + b = 63
(c += a) = 42
b = 21
++b = 22
b++ = 22
(b << 10) + 42 = 230000000042
(d <<= 4) = 13370000
(d >>= 2) = 133700
a =42
d =133700
(d < a) = 0
(d <= a) = 0
(d > a) = 1
(d >= a) = 1
(d == a) = 0
(d != a) = 1
EOF

# ---------- CHECK FILES ----------
if [ ! -f "bigint.hpp" ] || [ ! -f "bigint.cpp" ]; then
    echo -e "${RED}❌ bigint.hpp or bigint.cpp not found!${NC}"
    echo "Make sure both files are in the same folder as this script."
    exit 1
fi

# ---------- COMPILE ----------
echo -e "${BLUE}📦 Compiling...${NC}"
g++ -Wall -Wextra -Werror -std=c++98 -o bigint main.cpp bigint.cpp 2> compile_err.txt

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Compilation failed!${NC}"
    cat compile_err.txt
    rm -f compile_err.txt main.cpp expected.txt
    exit 1
fi
echo -e "${GREEN}✅ Compilation successful!${NC}"
echo ""

# ---------- RUN ----------
./bigint > output.txt 2>&1

# ---------- COMPARE ----------
echo -e "${BLUE}📊 Comparing output...${NC}"
echo ""
if diff -q expected.txt output.txt > /dev/null; then
    echo -e "${GREEN}✅ Output matches!${NC}"
    output_ok=true
else
    echo -e "${RED}❌ Output does NOT match!${NC}"
    echo ""
    echo -e "${YELLOW}Expected:${NC}"
    cat expected.txt
    echo ""
    echo -e "${YELLOW}Your output:${NC}"
    cat output.txt
    echo ""
    echo -e "${YELLOW}Diff:${NC}"
    diff expected.txt output.txt
    output_ok=false
fi

echo ""

# ---------- VALGRIND ----------
echo -e "${BLUE}🔍 Running valgrind...${NC}"
valgrind_out=$(valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./bigint 2>&1)
val_exit=$?

has_leaks=false
has_errors=false

echo "$valgrind_out" | grep -q "definitely lost:" && \
    echo "$valgrind_out" | grep "definitely lost:" | grep -qv "0 bytes" && has_leaks=true
echo "$valgrind_out" | grep -q "indirectly lost:" && \
    echo "$valgrind_out" | grep "indirectly lost:" | grep -qv "0 bytes" && has_leaks=true
echo "$valgrind_out" | grep -q "ERROR SUMMARY" && \
    echo "$valgrind_out" | grep "ERROR SUMMARY" | grep -qv "0 errors" && has_errors=true

if [ "$has_leaks" = false ] && [ "$has_errors" = false ]; then
    echo -e "${GREEN}✅ No memory leaks!${NC}"
else
    echo -e "${RED}❌ Memory issues detected!${NC}"
    echo "$valgrind_out"
fi

echo ""
echo "================================="
echo -n "RESULT: "
if [ "$output_ok" = true ] && [ "$has_leaks" = false ] && [ "$has_errors" = false ]; then
    echo -e "${GREEN}✅ ALL TESTS PASSED!${NC}"
else
    echo -e "${RED}❌ SOME TESTS FAILED!${NC}"
fi
echo "================================="

# ---------- CLEANUP ----------
rm -f bigint  expected.txt output.txt compile_err.txt