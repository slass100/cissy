#!/bin/sh

ANYFAIL=0;

for file in test_in_out/test_*_input.txt
do
    TEST=`echo ${file} | sed 's/.*\/\(.*\)_input.txt/\1/'`
    echo -n "Running ${TEST}:"
    cat test_in_out/${TEST}_input.txt \
	| ../src/cissy `cat test_in_out/${TEST}_cmdargs.txt` \
	> test_in_out/${TEST}_gen.txt
    diff test_in_out/${TEST}_gen.txt test_in_out/${TEST}_output.txt > /dev/null
    if [ $? -ne 0 ]; then
	/bin/echo -e "\tFail";
	ANYFAIL=1;
    else
	/bin/echo -e "\tPass";
    fi
done

exit ${ANYFAIL}
