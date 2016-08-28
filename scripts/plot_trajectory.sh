#!/bin/bash

input=
output=

function usage()
{
  cat << EOF
Usage: $0 [OPTIONS]

OPTIONS:
  -i [file]  Trajectory file.
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
  output=${input%.*}.png
fi

first_line=`head -n1 $input`
N=`echo $first_line | cut -d ' ' -f1`
p=`echo $first_line | cut -d ' ' -f2`
mu=`echo $first_line | cut -d ' ' -f3`

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
echo "stats \"$input\" using 1" >> $plotter
echo "set xrange [STATS_min-(STATS_max-STATS_min)/20:STATS_max+(STATS_max-STATS_min)/20]" >> $plotter
echo "set title \"N=${N}, p=${p}\" font \"Tahoma,16\"" >> $plotter
echo "plot \"$input\" every ::1 with lines lw 2 title \"{/Symbol m} = ${mu}\"" >> $plotter

gnuplot $plotter

rm -rf $plotter
