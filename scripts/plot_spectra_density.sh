#!/bin/bash

input=
output=

function usage()
{
  cat << EOF
Usage: $0 [OPTIONS]

OPTIONS:
  -i [file]  Input file containing eigenvalues density.

EOF
}

while getopts "i:o:" opt
do
  case "$opt" in
    i) input=$OPTARG;;
    \:) usage; exit;;
    \?) usage; exit;;
  esac
done

function plot_spectra_density()
{
  input=$1
  > spectra_density_plotter.gnu
  echo "set terminal png enhanced size 1280, 300" >> spectra_density_plotter.gnu
  echo "set output \"${input%.*}.png\"" >> spectra_density_plotter.gnu
  echo "unset ytics" >> spectra_density_plotter.gnu
  echo "stats \"$input\" using 1" >> spectra_density_plotter.gnu
  echo "set xrange [STATS_min-0.5:STATS_max+0.5]" >> spectra_density_plotter.gnu
  echo "set cbtics scale 0" >> spectra_density_plotter.gnu
  echo "set palette defined (0 \"#000090\",\\" >> spectra_density_plotter.gnu
  echo "                     1 \"#000fff\",\\" >> spectra_density_plotter.gnu
  echo "                     2 \"#0090ff\",\\" >> spectra_density_plotter.gnu
  echo "                     3 \"#0fffee\",\\" >> spectra_density_plotter.gnu
  echo "                     4 \"#90ff70\",\\" >> spectra_density_plotter.gnu
  echo "                     5 \"#ffee00\",\\" >> spectra_density_plotter.gnu
  echo "                     6 \"#ff7000\",\\" >> spectra_density_plotter.gnu
  echo "                     7 \"#ee0000\",\\" >> spectra_density_plotter.gnu
  echo "                     8 \"#7f0000\")" >> spectra_density_plotter.gnu
  echo "set view map" >> spectra_density_plotter.gnu
  echo "splot \"$input\" using 1:(0):2 with points palette notitle" >> spectra_density_plotter.gnu
  gnuplot spectra_density_plotter.gnu
  rm -f spectra_density_plotter.gnu
}

plot_spectra_density $input
