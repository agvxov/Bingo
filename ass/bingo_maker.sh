#!/bin/bash

declare -a WORDS

WORDS=("test" "macska" "kutya" "pingvin" "alma")

for j in ${WORDS[*]}; do
	OFILE="debug/${j}.bingo"
	rm $OFILE
	for i in $(seq 1 1 5); do
		for h in $(seq 1 1 5); do
			printf "$j $(expr '(' '(' $i - 1 ')' '*' 5 ')' + $h );" >> $OFILE
		done
		echo "" >> $OFILE
	done
	sed -i "s/;$//" $OFILE
done
