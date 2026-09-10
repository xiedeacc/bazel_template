#!/bin/bash

# format.sh - Format all source code files in the project
# Uses clang-format for C/C++ files and buildifier for Bazel files

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Get the project root directory (where this script is located)
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_ROOT"

echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}  Code Formatting Tool${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
echo ""

# Check for required tools
check_tool() {
    if ! command -v "$1" &> /dev/null; then
        echo -e "${RED}Error: $1 is not installed or not in PATH${NC}"
        echo -e "${YELLOW}Please install $1 to continue${NC}"
        exit 1
    fi
}

# Visual Studio ships clang-format but does not put it on PATH, so a Windows
# machine with the C++ Clang tools installed would still fail check_tool.
if ! command -v clang-format &> /dev/null; then
    for d in \
        "/c/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/Llvm/x64/bin" \
        "/c/Program Files/Microsoft Visual Studio/2022/BuildTools/VC/Tools/Llvm/x64/bin" \
        "/c/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/Llvm/x64/bin" \
        "/c/Program Files/LLVM/bin"; do
        if [ -x "$d/clang-format.exe" ]; then
            PATH="$d:$PATH"
            break
        fi
    done
fi

echo -e "${BLUE}Checking required tools...${NC}"
check_tool clang-format
check_tool buildifier
echo -e "${GREEN}✓ All required tools found${NC}"
echo ""

# Format C/C++ source files with clang-format
echo -e "${BLUE}Formatting C/C++ source files...${NC}"
CPP_FILES=$(find src -type f \( -name "*.cc" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) 2>/dev/null | wc -l)

if [ "$CPP_FILES" -gt 0 ]; then
    echo -e "${YELLOW}Found $CPP_FILES C/C++ files to format${NC}"
    
    # Format all C/C++ files in src directory
    find src -type f \( -name "*.cc" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) \
        -exec echo "  Formatting: {}" \; \
        -exec clang-format -i --style=file {} \;
    
    echo -e "${GREEN}✓ C/C++ files formatted${NC}"
else
    echo -e "${YELLOW}No C/C++ files found${NC}"
fi
echo ""

# Format Bazel BUILD and .bzl files with buildifier
echo -e "${BLUE}Formatting Bazel files...${NC}"
BAZEL_FILES=$(find . -type f \( -name "BUILD" -o -name "BUILD.bazel" -o -name "*.bzl" -o -name "WORKSPACE" -o -name "WORKSPACE.bazel" \) 2>/dev/null | wc -l)

if [ "$BAZEL_FILES" -gt 0 ]; then
    echo -e "${YELLOW}Found $BAZEL_FILES Bazel files to format${NC}"
    
    # Format BUILD files
    find . -type f \( -name "BUILD" -o -name "BUILD.bazel" \) \
        -exec echo "  Formatting: {}" \; \
        -exec buildifier {} \;
    
    # Format .bzl files
    find . -type f -name "*.bzl" \
        -exec echo "  Formatting: {}" \; \
        -exec buildifier {} \;
    
    # Format WORKSPACE files
    find . -type f \( -name "WORKSPACE" -o -name "WORKSPACE.bazel" \) \
        -exec echo "  Formatting: {}" \; \
        -exec buildifier {} \;
    
    echo -e "${GREEN}✓ Bazel files formatted${NC}"
else
    echo -e "${YELLOW}No Bazel files found${NC}"
fi
echo ""

# Summary
echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
echo -e "${GREEN}✓ Formatting complete!${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
echo ""
echo -e "Formatted files:"
echo -e "  ${GREEN}•${NC} C/C++ files: ${CPP_FILES}"
echo -e "  ${GREEN}•${NC} Bazel files: ${BAZEL_FILES}"
echo ""
echo -e "${YELLOW}Note: Changes have been applied in-place${NC}"
echo -e "${YELLOW}Review with: git diff${NC}"
echo ""

