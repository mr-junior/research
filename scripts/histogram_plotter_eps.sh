#!/bin/bash

input=
output=
width=0.5
x_label=
y_label=

function usage()
{
  cat << EOF
Usage: $0 [OPTIONS]

OPTIONS:
  -i [file]  Input file containing eigenvalues.
  -o [file]  Output image file.
  -w [num]   Bin width.
  -x [str]   X axis label.
  -y [str]   Y axis label.

EOF
}

while getopts "i:o:w:x:y:" opt
do
  case "$opt" in
    i) input=$OPTARG;;
    o) output=$OPTARG;;
    w) width=$OPTARG;;
    x) x_label=$OPTARG;;
    y) y_label=$OPTARG;;
    \:) usage; exit;;
    \?) usage; exit;;
  esac
done

if [ -z "$output" ]; then
  output=${input%.*}.eps
fi

> histogram_plotter.gnu
echo "set terminal eps size 10, 6 enhanced" >> histogram_plotter.gnu
echo "set output \"$output\"" >> histogram_plotter.gnu
#echo "unset ytics" >> histogram_plotter.gnu
echo "set xtics" >> histogram_plotter.gnu
echo "set mxtics" >> histogram_plotter.gnu
echo "set grid" >> histogram_plotter.gnu
echo "stats \"$input\" using 1" >> histogram_plotter.gnu
if [[ ! -z $x_label ]]; then
  echo "set xlabel \"$x_label\"" >> histogram_plotter.gnu
fi
if [[ ! -z $y_label ]]; then
  echo "set ylabel \"$y_label\"" >> histogram_plotter.gnu
fi
echo "set xrange [STATS_min-0.5:STATS_max+0.5]" >> histogram_plotter.gnu
echo "binwidth=$width" >> histogram_plotter.gnu
echo "bin(x, width)=width*floor(x/width)" >> histogram_plotter.gnu
echo "set style fill solid 0.5" >> histogram_plotter.gnu
echo "set boxwidth binwidth" >> histogram_plotter.gnu
echo "plot \"$input\" using (bin(\$1,binwidth)):(1.0) smooth freq with boxes notitle" >>histogram_plotter.gnu

gnuplot histogram_plotter.gnu

rm -f histogram_plotter.gnu
