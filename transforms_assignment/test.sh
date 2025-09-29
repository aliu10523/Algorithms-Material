#!/usr/bin/env bash
g++ -std=c++17 -O2 main.cpp -o main || exit 1

for inFile in io/*.in.*; do
    outFile=${inFile/.in./.out.}
    if diff -q <(./main < "$inFile") "$outFile" >/dev/null 2>&1; then
        echo "$inFile OK"
    else
        echo "$inFile WRONG"
        diff -u <(./main < "$inFile") "$outFile" | head -n 20
    fi
done
