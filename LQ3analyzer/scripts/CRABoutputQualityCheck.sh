#!/bin/bash
#
inputlist=$1
eosdir=$2
echo
echo " This is a script to check for duplicate and/or missing files that CRAB outputs to EOS directory"
echo "    stageout for 2012: /eos/cms/store/group/phys_exotica/leptonsPlusJets/RootNtuple/hsaka/"
#
if [ -z $1 ] || [ -z $2 ] || [ -z $3 ] || [ ! -z $6 ] || [ $5 -lt 1 ] || [ ! -e $inputlist ]; then
    echo
    echo " Missing arguments (1):"
    echo "           example:  ./CRABoutputQualityCheck.sh inputFile </eosdir/> mc/data d/nd <jobNo>"
    echo " exiting.."
    echo
    exit 1
fi
if [ $3 != "mc" ] && [ $3 != "data" ]; then
    echo
    echo " Missing arguments (2):"
    echo "           example:  ./CRABoutputQualityCheck.sh inputFile </eosdir/> mc/data d <jobNo>"
    echo " exiting.."
    echo
    exit 1
fi
if [ $4 != "d" ] && [ $4 != "nd" ]; then
    echo
    echo " Missing arguments (3):"
    echo "           example:  ./CRABoutputQualityCheck.sh inputFile </eosdir/> mc/data d <jobNo>"
    echo " exiting.."
    echo
    exit 1
fi
#
#
echo 
echo " Starting.. "
echo
#
if [ $3 == "mc" ]; then
    type="AODSIM"
fi
if [ $3 == "data" ]; then
    type="AOD"
fi
echo
#
for line in `cat $inputlist | sed  -e 's| Jan | 01 |g' -e 's| Feb | 02 |g' -e 's| Mar | 03 |g' -e 's| Apr | 04 |g' -e 's| May | 05 |g' -e 's| Jun | 06 |g' -e 's| Jul | 07 |g' -e 's| Aug | 08 |g' -e 's| Sep | 09 |g' -e 's| Oct | 10 |g' -e 's| Nov | 11 |g' -e 's| Dec | 12 |g' | sed 's|_| |g' | awk '{print $(NF-2)" "$(NF-1)"  "$6" "$7" "$8"    "$(NF)}' | sort -n -r | sed 's| |~|g'`; do
    ver2=`echo $line | sed 's|~| |g' | awk '{print $1}'`
    subver2=`echo $line | sed 's|~| |g' | awk '{print $2}'`
    tail2=`echo $line | sed 's|~| |g' | awk '{print $6}'`
    #
    if [ ! -z $ver1 ]; then
	if [ $ver2 -eq $ver1 ]; then
            filetodel=`cat $inputlist | awk '{print $9}' | grep "_"$ver2"_"$subver2"_"$tail2`
	    if [ $4 != "d" ]; then
		echo "eos rm "$eosdir"/"$filetodel
	    else
		cat $inputlist | awk '{print $9}' | grep $type"_"$ver2"_"
		echo " > > > "$filetodel
		echo
	    fi
        fi
    fi
    ver1=($ver2)
done 
echo
if [ $4 == "d" ]; then
    for fileno in {1..550}; do
	if [ $fileno -gt $5 ]; then
	    break
	fi
	missingfile=`cat $inputlist | grep $type"_"$fileno"_"`
	if [ -z "$missingfile" ]; then
	    echo "Missing file: "$fileno
	fi
    done
fi
echo
echo " -- Quality Check is complete! -- "
echo
#
exit 0