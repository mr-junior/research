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

if [ ! -e $input ]; then
  echo "Invalid input file."
  exit
fi

if [ -z "$output" ]; then
  output=spectra.png
fi

> spectra_plotter.gnu
echo "set terminal png enhanced size 1200, 600" >> spectra_plotter.gnu
echo "set output \"$output\"" >> spectra_plotter.gnu
echo "unset ytics" >> spectra_plotter.gnu
echo "set key outside title \"Step\"" >> spectra_plotter.gnu
echo "set key box" >> spectra_plotter.gnu

command_line="plot "
current_line=0

while read line           
do
  num=`echo $line | awk -F_ '{print $(NF-1)}'`
  command_line="$command_line \"$line\" using 1:($current_line) title \"$num\", "
  ((current_line++))
done < $input

echo "set yrange [-0.5:$current_line-0.5]" >> spectra_plotter.gnu
echo "$command_line" >> spectra_plotter.gnu

gnuplot spectra_plotter.gnu

rm -f spectra_plotter.gnu
