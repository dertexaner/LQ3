#!/bin/bash
#
if [ -z $1 ] || [ -z $2 ] || [ ! -z $3 ]; then
    echo
    echo "EOS directory is: /eos/cms/store/user/hsaka/2013Skims/"
    echo
    echo "Usage: "  
    echo "./inputFileGenerator.sh  <EOSdir> config/<outputFolder>"
    echo
    echo "Example: "
    echo "./scripts/inputFileGenerator.sh /eos/cms/store/user/hsaka/2013Skims/2013_MuTauSkim_V1/ analysisClass_DATA_MC_LTEM_MuTauSkim"
    echo
    echo "Exiting.."
    echo
    exit 1
fi
#
if [ -d config/inputFile_$2 ]; then
    echo 
    echo "DIRECTORY ERROR"
    echo config/inputFile_$2" exists."
    echo
    echo "Exiting.."
    exit 1
fi
if [ ! -d config/ ]; then
    echo 
    echo "WRONG WORKING DIRECTORY"
    echo "Go to: rootNtupleAnalyzerV2013/"
    echo
    echo "Exiting.."
    exit 1
fi
#
mkdir config/inputFile_$2
#
for file in `/afs/cern.ch/project/eos/installation/0.2.5/bin/eos.select ls $1 | grep root`; do
    echo $file
    filename=`echo $file | sed 's|.root||g' | sed 's|'$2'||g' | sed 's|___||g'`
    echo root://eoscms/$1/$file > config/inputFile_$2/$filename.txt
done
#
for file in `ls config/inputFile_$2/*.txt`; do
    echo $file | grep -v batch
done >  config/inputFile_$2/inputFile_batch_$2.txt
#
exit 0
