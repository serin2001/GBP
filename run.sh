#!/bin/bash

OUTPUT_FILE="output.txt"

# Clear previous output
> "$OUTPUT_FILE"

./CBAG P81.csv 9 >> "$OUTPUT_FILE" 2>&1
./CBAG netscience.csv 6 >> "$OUTPUT_FILE" 2>&1
./CBAG polblogs.csv 5 >> "$OUTPUT_FILE" 2>&1
./CBAG reed98.csv 4 >> "$OUTPUT_FILE" 2>&1
./CBAG mahindas.csv 5 >> "$OUTPUT_FILE" 2>&1
./CBAG cite.csv 41 >> "$OUTPUT_FILE" 2>&1
./CBAG chameleon.csv 6 >> "$OUTPUT_FILE" 2>&1
./CBAG tvshow.csv 9 >> "$OUTPUT_FILE" 2>&1
./CBAG ego-facebook.csv 4 >> "$OUTPUT_FILE" 2>&1
./CBAG squirrel.csv 6 >> "$OUTPUT_FILE" 2>&1
./CBAG politician.csv 7 >> "$OUTPUT_FILE" 2>&1
./CBAG government.csv 6 >> "$OUTPUT_FILE" 2>&1
./CBAG crocodile.csv 6 >> "$OUTPUT_FILE" 2>&1
./CBAG HR.csv 7 >> "$OUTPUT_FILE" 2>&1
./CBAG BAexample00.csv 4 >> "$OUTPUT_FILE" 2>&1
./CBAG BAexample01.csv 4 >> "$OUTPUT_FILE" 2>&1
./CBAG BAexample02.csv 4 >> "$OUTPUT_FILE" 2>&1
./CBAG BAexample03.csv 4 >> "$OUTPUT_FILE" 2>&1
./CBAG BAexample04.csv 5 >> "$OUTPUT_FILE" 2>&1
./CBAG BAexample05.csv 4 >> "$OUTPUT_FILE" 2>&1
./CBAG BAexample06.csv 5 >> "$OUTPUT_FILE" 2>&1
./CBAG BAexample07.csv 4 >> "$OUTPUT_FILE" 2>&1
./CBAG BAexample08.csv 4 >> "$OUTPUT_FILE" 2>&1
./CBAG BAexample09.csv 4 >> "$OUTPUT_FILE" 2>&1
./CBAG ERexample00.csv 5 >> "$OUTPUT_FILE" 2>&1
./CBAG ERexample01.csv 5 >> "$OUTPUT_FILE" 2>&1
./CBAG ERexample02.csv 5 >> "$OUTPUT_FILE" 2>&1
./CBAG ERexample03.csv 5 >> "$OUTPUT_FILE" 2>&1
./CBAG ERexample04.csv 5 >> "$OUTPUT_FILE" 2>&1
./CBAG ERexample05.csv 5 >> "$OUTPUT_FILE" 2>&1
./CBAG ERexample06.csv 5 >> "$OUTPUT_FILE" 2>&1
./CBAG ERexample07.csv 5 >> "$OUTPUT_FILE" 2>&1
./CBAG ERexample08.csv 5 >> "$OUTPUT_FILE" 2>&1
./CBAG ERexample09.csv 5 >> "$OUTPUT_FILE" 2>&1
