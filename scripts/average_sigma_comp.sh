#!/bin/bash
N=
p=
step_count=
sigma_start=
mu_values=
results_directory=

while getopts "N:p:m:w:c:s:": opt
do
  case "$opt" in
    N) N=$OPTARG;;
    p) p=$OPTARG;;
    m) mu_values=(`cat $OPTARG`);;
    w) results_directory=$OPTARG;;
    c) step_count=$OPTARG;;
    s) sigma_start=$OPTARG;;
    ?) usage; exit;;
  esac
done

usage()
{
  cat << EOF
Usage: $0 [OPTIONS]

OPTIONS:
  -w  Directory to work in
  -c  Randomization step count
  -s  Start index for sigma calculation

EOF
}

source ~/research/scripts/setup.sh

function plot()
{
  file=$1
  title=$2
  > plotter.gnu
  echo "set terminal png enhanced size 1920, 1080" >> plotter.gnu
  echo "set output \"${file%.*}.png\"" >> plotter.gnu
  echo "stats \"$file\"" >> plotter.gnu
  echo "set xrange [STATS_min_x-0.04*(STATS_max_x-STATS_min_x):STATS_max_x+0.04*(STATS_max_x-STATS_min_x)]" >> plotter.gnu
  echo "set yrange [STATS_min_y-0.04*(STATS_max_y-STATS_min_y):STATS_max_y+0.04*(STATS_max_y-STATS_min_y)]" >> plotter.gnu
  echo "plot \"$file\" with lp title \"$title\"" >> plotter.gnu
  gnuplot plotter.gnu
  rm -f plotter.gnu
}

for u in ${mu_values[*]}
do
	echo "${TOOLS_DIR}/average_sigma/average_sigma_computer.exe $results_directory/N${N}_p${p}_u${u}_T/N${N}_p${p}_u${u}.txt $step_count $sigma_start"
	output=`${TOOLS_DIR}/average_sigma/average_sigma_computer.exe $results_directory/N${N}_p${p}_u${u}_T/N${N}_p${p}_u${u}.txt $step_count $sigma_start`
  echo "$u $output" | awk -F' ' '{print $1" "$2}' >> $results_directory/average.txt
  echo "$u $output" | awk -F' ' '{print $1" "$3}' >> $results_directory/sigma_average.txt
  echo "$u $output" | awk -F' ' '{print $1" "$4}' >> $results_directory/sigma.txt
done

plot average.txt "Average"
plot sigma_average.txt "Average on [$sigma_start, $step_count]"
plot sigma.txt "Standard deviation"

