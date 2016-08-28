#!/bin/bash

input=
output=

function usage()
{
  cat << EOF
Usage: $0 [OPTIONS]

OPTIONS:
  -i [file]  Input file containing eigenvalues.
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

> spectra_plotter.gnu
echo "set terminal png enhanced size 1280, 200" >> spectra_plotter.gnu
echo "set output \"$output\"" >> spectra_plotter.gnu
echo "unset ytics" >> spectra_plotter.gnu
echo "stats \"$input\"" >> spectra_plotter.gnu
echo "set xrange [STATS_min-0.5:STATS_max+0.5]" >> spectra_plotter.gnu
echo "plot \"$input\" using 1:(0) notitle" >> spectra_plotter.gnu

gnuplot spectra_plotter.gnu

rm -f spectra_plotter.gnu
