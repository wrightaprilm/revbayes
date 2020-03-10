#!/bin/bash

mkdir logs
scripts=$(ls scripts/*.Rev)
for s in $scripts
do
	sh=${s##*/}
	../projects/cmake/rb $s > logs/$sh.log
	if [ $? -ne 0 ]
	then
		#echo $?
		echo "Test failed : "$sh" errored during run"
		continue
	fi
	
	cov=$(grep "Coverage frequencies should be between" logs/$sh.log)
	low=$(echo $cov | cut -d " " -f 6)
	high=$(echo $cov | cut -d " " -f 8)

	nfail=0
	nsuccess=0
	while IFS= read line; do

		if [[ $line =~ ^\ *$ ]]; then
			continue
		fi

    	if echo $line | grep -q === ; then
    		break
    	fi

    	prob=$(echo $line | sed 's/  */:/g' | cut -d : -f 2)

    	if [ $(echo $low">"$prob | bc) -eq 1 ] #|| [ $(echo $prob">"$high | bc) -eq 1 ]
    	then
    		nfail=$((nfail + 1))
    	else
    		nsuccess=$((nsuccess + 1))
    	fi
	done < <( tail -r logs/$sh.log )

	if [ "$nfail" -gt "0" ]
	then
		echo "Test failed : "$sh" with "$nfail" failed and "$nsuccess" passed"
	else
		echo "Test passed : "$sh" with "$nsuccess" passed"
	fi
done