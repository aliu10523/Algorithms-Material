#!/usr/bin/env bash
g++ -std=c++17 -O2 palindrome.cpp -o palindrome || exit 1

for inFile in io2/*.in.*; do
    outFile=${inFile/.in./.out.}
    ./palindrome < "$inFile" > tmp
    echo Testing "$inFile" 
    diff tmp "$outFile"
    echo $outFile
done
rm tmp
