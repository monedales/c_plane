#!/bin/bash

BINARY="./c_plane"
PASS=0
FAIL=0

run_test() {
	local description="$1"
	local input="$2"
	local expected="$3"

	actual=$(echo "$input" | $BINARY 2>&1)
	if [ "$actual" = "$expected" ]; then
		echo "PASS  $description"
		PASS=$((PASS + 1))
	else
		echo "FAIL  $description"
		echo "      expected: $expected"
		echo "      got:      $actual"
		FAIL=$((FAIL + 1))
	fi
}

echo ""
echo "=== c_plane e2e tests ==="
echo ""
echo "-- enunciado examples --"

run_test "example 1 (N=3, max=3)" \
"3
1 5 7
9 13 12" "3"

run_test "example 2 (N=4, max=1)" \
"4
1 4 8 10
3 8 10 17" "1"

echo ""
echo "-- edge cases --"

run_test "N=1 single passenger" \
"1
5
10" "1"

run_test "N=1 min moments (1 and 2)" \
"1
1
2" "1"

run_test "all 3 exit and enter same moment (max=1)" \
"3
1 3 5
3 5 7" "1"

run_test "100 passengers all overlap (max=100)" \
"$(cat tests/input_100_all_overlap.txt)" "100"

run_test "100 passengers sequential (max=1)" \
"$(cat tests/input_100_sequential.txt)" "1"

echo ""
echo "-- validation errors --"

run_test "N=0 below minimum" \
"0
1
2" "Error: N must be an integer between 1 and 100"

run_test "N=101 above maximum" \
"101
1 5 7
9 13 12" "Error: N must be an integer between 1 and 100"

run_test "entry equals exit same passenger" \
"1
5
5" "Error: a passenger entry must occur before its exit"

run_test "entry after exit same passenger" \
"1
10
5" "Error: a passenger entry must occur before its exit"

echo ""
echo "-- input format errors (strtol coverage) --"

run_test "N is alphabetic" \
"abc
1 5 7
9 13 12" "Error: input contains invalid characters. Use plain numbers separated by spaces."

run_test "moment value is alphabetic" \
"3
1 abc 7
9 13 12" "Error: input contains invalid characters. Use plain numbers separated by spaces."

run_test "moment value with special character" \
"3
1 \$ 7
9 13 12" "Error: input contains invalid characters. Use plain numbers separated by spaces."

run_test "moment value overflow (strtol errno)" \
"3
1 99999999999999999999 7
9 13 12" "Error: numeric value is out of allowed range"

run_test "moment value above max (1001)" \
"3
1 1001 7
9 13 12" "Error: moment values must be integers between 1 and 1000"

run_test "moment value below min (0)" \
"3
1 0 7
9 13 12" "Error: moment values must be integers between 1 and 1000"

run_test "list smaller than N" \
"3
1 5
9 13 12" "Error: list size does not match the number of passengers"

run_test "list larger than N" \
"2
1 5 7
9 13 12" "Error: list size does not match the number of passengers"

echo ""
echo "=== $PASS passed, $FAIL failed ==="
echo ""

[ $FAIL -eq 0 ] && exit 0 || exit 1
