#!/bin/bash

cluster_1="cluster_1.txt"
cluster_2="cluster_2.txt"
mus="mus.txt"

while getopts "f:s:": opt
do
  case "$opt" in
    f) cluster_1=$OPTARG;;
    s) cluster_2=$OPTARG;;
    ?) usage; exit;;
  esac
done

usage()
{
  cat << EOF
Usage: $0 [OPTIONS]

OPTIONS:
  -f  First cluster
  -s  Second cluster

EOF
}

output="output_between_${cluster_1%.*}_${cluster_2%.*}.txt"

OLDIFS=$IFS
IFS=$'\n'
mu_list=($(cat ${mus}))
IFS=$OLDIFS

: > ${output}

for mu_index in ${!mu_list[*]}
do
	mu=${mu_list[$mu_index]}
	graph_file="graph_${mu}.txt"
	echo "Processing ${graph_file}..."
	echo ${mu} `~/research/tools/cluster_links/links_between/bin/links_between_clusters.exe ${graph_file} ${cluster_1} ${cluster_2}` >> ${output}
done
