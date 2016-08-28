#!/bin/bash

input=
output=

function usage()
{
  cat << EOF
Usage: $0 [OPTIONS]

OPTIONS:
  -i [files]  Trajectory file list.
  -o [file]  Output image file.

EOF
}

while getopts "i:o:" opt
do
  case "$opt" in
    i) input=$OPTARG;;
    o) output=$OPTARG;;
    \:) usage; exit;;
    \?) usage; exit;;
  esac
done

if [ -z "$output" ]; then
  output=combined_trajectories.png
fi

first_file=`echo $input | cut -d ' ' -f1`
first_line=`head -n1 $first_file`
N=`echo $first_line | cut -d ' ' -f1`
p=`echo $first_line | cut -d ' ' -f2`
mu=`echo $first_line | cut -d ' ' -f3`
input=`echo $input | sed -e "s:^\<$first_file\>::"`

plotter=`mktemp --suffix ".gnu"`

> $plotter

echo 'set term pngcairo size 1920,1080 truecolor fon "Tahoma" enhanced' >> $plotter
echo "set output \"$output\"" >> $plotter
echo 'set grid x' >> $plotter
echo 'set xtics' >> $plotter
echo 'set mxtics' >> $plotter
echo 'set grid y' >> $plotter
echo 'set ytics' >> $plotter
echo 'set mytics' >> $plotter
echo 'set key outside right spacing 1.5 enhanced' >> $plotter
echo "stats \"$first_file\" using 1" >> $plotter
echo "set xrange [STATS_min-(STATS_max-STATS_min)/20:STATS_max+(STATS_max-STATS_min)/20]" >> $plotter
echo "set title \"N=${N}, p=${p}\" font \"Tahoma,16\"" >> $plotter
echo "plot \"$first_file\" every ::1 with lines lw 2 title \"{/Symbol m} = ${mu}\"\\" >> $plotter
while [ ! 0 -eq ${#input} ]
do
  first_file=`echo $input | cut -d ' ' -f1`
  first_line=`head -n1 $first_file`
  mu=`echo $first_line | cut -d ' ' -f3`
  echo ", \"$first_file\" every ::1 with lines lw 2 title \"{/Symbol m} = ${mu}\"\\" >> $plotter
  input=`echo $input | sed -e "s:^\<$first_file\>::"`
done

gnuplot $plotter

rm -rf $plotter
