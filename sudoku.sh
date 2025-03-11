#!/bin/bash

ITER=50

run() {
	option=$1
	output_file="results_for_option_${option}.txt"
	> "$output_file"
	total_time=0

	echo "Running $ITER trials for Option $option..." | tee -a "$output_file"

	for ((i=1; i <=ITER; i++)); do
		echo "Run $i:" >> "$output_file"
		result=$(./main $option | tail -n 1)
		echo "$result" >> "$output_file"

		run_time=$(echo "$result" | grep -oP '\(\K[0-9]+\.[0-9]+(?= seconds\))')

		if [ -z "$run_time" ]; then
			run_time=0
		fi

		total_time=$(echo "$total_time + $run_time" | bc)
		echo "Run $i time: $run_time seconds" >> "$output_file"
	done
	average=$(echo "scale=4; $tot_time / $ITER" | bc)
	echo "Average time for Option $option: $average seconds" | tee -a "$output_file"
	echo ""
}

run 1
run 2
run 3
