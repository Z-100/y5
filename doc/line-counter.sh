#!/usr/bin/env bash

EXCLUDES=(
  -not -path "./lib/*"
  -not -path "./build/*"
  -not -path "./.idea/*"
)

count_lines_for_filetype() {
  local pattern="$1"

  find . -type f -name "$pattern" \
      "${EXCLUDES[@]}" \
      -exec cat {} + 2>/dev/null | wc -l
}

c_count=$(count_lines_for_filetype "*.c")
cpp_count=$(count_lines_for_filetype "*.cpp")
h_count=$(count_lines_for_filetype "*.h")
glsl_count=$(count_lines_for_filetype "*.glsl")

echo "C:       $c_count"
echo "C++:     $cpp_count"
echo "Headers: $h_count"
echo "GLSL:    $glsl_count"

total=$((c_count + cpp_count + h_count + glsl_count))

echo ""
echo "TOTAL: $total"
