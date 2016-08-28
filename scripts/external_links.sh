#!/bin/bash

cluster="cluster_1.txt"
mus="mus.txt"

while getopts "c:": opt
do
  case "$opt" in
    c) cluster=$OPTARG;;
    ?) usage; exit;;
  esac
done

usage()
{
  cat << EOF
Usage: $0 [OPTIONS]

OPTIONS:
  -c  Cluster

EOF
}

output="output_external_${cluster%.*}.txt"

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
	echo ${mu} `~/research/tools/cluster_links/external_links/bin/cluster_external_links.exe ${graph_file} ${cluster}` >> ${output}
done
