#!/bin/bash 
#
if [ -z $1 ] || [ -z $2 ] || [ -z $3 ] || [ ! -z $4 ]; then
    echo
    echo "EOS directory is: /eos/cms/store/group/phys_exotica/leptonsPlusJets/RootNtuple/"
    echo
    echo "Usage: "  
    echo "./MakeEOSinputFile.sh  <username> <SampleSpecificDirName> <SampleName>"
    echo
    echo "Example: "
    echo "./MakeEOSinputFile.sh hsaka RootNtuple-V00-03-11-Summer12MC_TTJets_FullLeptMGDecays_20130215_194721 TTJets_FullLeptMGDecays"
    echo
    echo "Exiting.."
    echo
    exit 1
fi
#
echo
eoshome='/eos/cms/store/group/phys_exotica/leptonsPlusJets/RootNtuple/'
eossource=$eoshome$1"/"$2"/"
echo $eossource
echo
echo -n "Number of files: "
/afs/cern.ch/project/eos/installation/0.2.5/bin/eos.select ls $eossource | grep root | grep $3 | wc -l
echo
for file in `/afs/cern.ch/project/eos/installation/0.2.5/bin/eos.select ls $eossource | grep root | grep $3`; do
    echo "root://eoscms/"$eossource"/"$file
done > "inputList_RootNtuple-V00-03-11_"$3.txt
#
#mv $2.txt $2_$3.txt
#
exit 0
