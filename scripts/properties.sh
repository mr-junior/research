#!/bin/bash

N=
p=
delta=
step_count=
mu_values=
work_dir=

while getopts "N:p:m:c:d:w:" opt
do
	case "$opt" in
    N) N=$OPTARG;;
    p) p=$OPTARG;;
    m) mu_values=(`cat $OPTARG`);;
    c) step_count=$OPTARG;;
    d) delta=$OPTARG;;
		w) work_dir=$OPTARG;;
		?) usage; exit;;
	esac
done

usage()
{
  cat << EOF
Usage: $0 [OPTIONS]

OPTIONS:
  -w  Directory to work in

EOF
}

source ~/research/scripts/setup.sh

pushd .
cd $work_dir

for mu in ${mu_values[*]}
do
	pushd .
	cd "N${N}_p${p}_u${mu}_T/graphs"
	mkdir properties
	#these files will store global properties evolution
	touch properties/clustering_coefficient.txt
	touch properties/average_path_length.txt
	touch properties/diameter.txt
	#these files will store local properties evolution for each vertex
	mkdir properties/clustering_coefficients
	N_1=$((N-1))
	for i in `seq 0 ${N_1}`
	do
		touch properties/clustering_coefficients/${i}.txt
	done
	mkdir properties/closeness_centralities
	for i in `seq 0 ${N_1}`
	do
		touch properties/closeness_centralities/${i}.txt
	done
	mkdir properties/betweenness_centralities
	for i in `seq 0 ${N_1}`
	do
		touch properties/betweenness_centralities/${i}.txt
	done

	for i in `seq 0 ${delta} ${step_count}`
	do
		graph="final_graph__N${N}_p${p}_u${mu}_${i}.txt"
		echo $graph
		$TOOLS_DIR/graph_properties/bin/graph_properties.exe $graph
		props=`cat "${graph%.txt}_global_properties.txt"`
		echo $props | awk -v i="$i" '{print i,$1 >> "properties/clustering_coefficient.txt";}'
		echo $props | awk -v i="$i" '{print i,$2 >> "properties/average_path_length.txt";}'
		echo $props | awk -v i="$i" '{print i,$3 >> "properties/diameter.txt"}'

		while read line
		do
			echo $line | awk -v i="$i" '{print i,$2 >> "properties/clustering_coefficients/"$1".txt"}'
		done < "${graph%.txt}_clustering_coefficients.txt"

		while read line
		do
			echo $line | awk -v i="$i" '{print i,$2 >> "properties/closeness_centralities/"$1".txt"}'
		done < "${graph%.txt}_closeness_centralities.txt"

		while read line
		do
			echo $line | awk -v i="$i" '{print i,$2 >> "properties/betweenness_centralities/"$1".txt"}'
		done < "${graph%.txt}_betweenness_centralities.txt"
	done
	popd
done

popd
