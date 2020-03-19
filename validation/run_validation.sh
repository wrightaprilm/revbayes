#!/bin/bash

mkdir -p logs
scripts=$(ls scripts/*.Rev)
toterror=0
totfail=0
totsucc=0
for s in $scripts
do
	sh=${s##*/}

	if [[ ! -f logs/$sh.log ]]
	then
		mpirun -np 4 ../projects/cmake/rb-mpi $s > logs/$sh.log
	fi

	if [ $? -ne 0 ]
	then
		#echo $?
		echo "Test failed : "$sh" errored during run"
		toterror=$((toterror + 1))
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

	totfail=$((totfail + nfail))
	totsucc=$((totsucc + nsuccess))
done

echo "Total number of errors : "$toterror
echo "Total number of failures : "$totfail
echo "Total number of successes : "$totsucc