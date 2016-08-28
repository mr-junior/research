#/bin/bash

N=
p=
mu_values=
results_directory=

while getopts "N:p:m:w:": opt
do
  case "$opt" in
    N) N=$OPTARG;;
    p) p=$OPTARG;;
    m) mu_values=(`cat $OPTARG`);;
    w) results_directory=$OPTARG;;
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
}

source ~/research/scripts/setup.sh

pushd .
cd ${results_directory}

for u in ${mu_values[*]}
do
	count=0
	for i in `ls N${N}_p${p}_u${u}_T/graphs/ | grep final`
	do
		echo N${N}_p${p}_u${u}_T/graphs/$i
		${TOOLS_DIR}/graph_converter/bin/converter.exe b N${N}_p${p}_u${u}_T/graphs/$i N${N}_p${p}_u${u}_T/graphs/${i}_converted.txt&
		((count++))
		if (($count > 50)); then
			wait
			count=0
		fi
	done
	wait
	mkdir -p N${N}_p${p}_u${u}_T/graphs/converted
	mv N${N}_p${p}_u${u}_T/graphs/*_converted.txt N${N}_p${p}_u${u}_T/graphs/converted/
done

popd
