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


for graph in ;
do
  ~/research/tools/louvain/bin/convert -i initial/${graph} -o clusters/${graph%.*}.bin
done

~/research/tools/louvain/bin/community louvain_binary/d31_11.bin -l -1 > louvain_binary/d31_11_hierarchy.txt

~/research/tools/louvain/bin/hierarchy louvain_binary/d31_11_hierarchy.txt -l 1 > louvain_binary/d31_11_clusters.txt

for i in {11..60}; do ~/research/tools/louvain/bin/community louvain_binary/d63_${i}.bin -l -1 > louvain_binary/d63_${i}_hierarchy.txt; ~/research/tools/louvain/bin/hierarchy louvain_binary/d63_${i}_hierarchy.txt -l 1 > louvain_binary/d63_${i}_clusters.txt; sort -k 2 -n louvain_binary/d63_${i}_clusters.txt > _tmp_; mv _tmp_ louvain_binary/d63_${i}_clusters.txt; done

sort -k2 -n louvain_binary/d31_11_clusters.txt > tmp
mv tmp  louvain_binary/d31_11_clusters.txt

for i in {11..60}; do mkdir ${i}; cd ${i}; ~/research/tools/cluster_eigenvalues/bin/cluster_eigens.exe ../../initial/d31_${i}.txt ../../louvain_binary/d31_${i}_clusters.txt; cd -; done

for i in {11..60}; do for j in {1..8}; do cat ${i}/${j}_eigenvalues.txt >> all_eigenvalues.txt; done; done; sort -g all_eigenvalues.txt > all_eigenvalues_sorted.txt

    ~/research/tools/density/bin/density.exe all_eigenvalues_sorted.txt 0.5 all_eigenvalues_density_bin_0.5.txt

