#!/bin/bash
# Initialize a new Advent of Code day from the template
# Usage: ./init_day.sh <day_number>
# Example: ./init_day.sh 3

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEMPLATE_DIR="$SCRIPT_DIR/template"

if [ -z "$1" ]; then
    echo "Usage: $0 <day_number>"
    echo "Example: $0 3"
    exit 1
fi

DAY=$1
DAY_DIR="$SCRIPT_DIR/day$DAY"

if [ -d "$DAY_DIR" ]; then
    echo "Error: $DAY_DIR already exists!"
    exit 1
fi

echo "Creating day$DAY from template..."

# Create directory structure
mkdir -p "$DAY_DIR/input"

# Copy and transform template files (replace DayX -> Day$DAY)
sed "s/DayX/Day$DAY/g; s/Day X/Day $DAY/g" "$TEMPLATE_DIR/solution.h" > "$DAY_DIR/solution.h"
sed "s/DayX/Day$DAY/g" "$TEMPLATE_DIR/solution.cpp" > "$DAY_DIR/solution.cpp"
sed "s/DayX/Day$DAY/g" "$TEMPLATE_DIR/main.cpp" > "$DAY_DIR/main.cpp"
sed "s/DayX/Day$DAY/g" "$TEMPLATE_DIR/test.cpp" > "$DAY_DIR/test.cpp"
cp "$TEMPLATE_DIR/Makefile" "$DAY_DIR/"

# Create placeholder input file
touch "$DAY_DIR/input/input.txt"

# Create example test files
cat > "$DAY_DIR/input/part1-test1.txt" << 'EOF'
EXPECTED
0
DATA

EOF

cat > "$DAY_DIR/input/part2-test1.txt" << 'EOF'
EXPECTED
0
DATA

EOF

echo "Created day$DAY/"
echo ""
echo "Structure:"
echo "  day$DAY/"
echo "  ├── solution.h     - Day$DAY, Day${DAY}Part1, Day${DAY}Part2 classes"
echo "  ├── solution.cpp   - Implement solve() and helpers here"
echo "  ├── main.cpp       - CLI runner (no changes needed)"
echo "  ├── test.cpp       - Test runner (no changes needed)"
echo "  ├── Makefile"
echo "  └── input/"
echo "      ├── input.txt"
echo "      ├── part1-test1.txt"
echo "      └── part2-test1.txt"
echo ""
echo "Shared code in common/:"
echo "  - aoc::Solver<T>        - Base class (override solve(), getInputStream())"
echo "  - aoc::TestDecorator<T> - Wraps solver for testing"
echo ""
echo "Quick start:"
echo "  cd day$DAY"
echo "  make runtest   # Run tests"
echo "  make run1      # Run Part 1"
echo "  make run2      # Run Part 2"
