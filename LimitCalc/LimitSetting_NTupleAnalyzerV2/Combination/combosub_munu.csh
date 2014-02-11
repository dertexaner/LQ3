#!/bin/csh
cd MYPWD
eval `scramv1 runtime -csh`
cd -
sleep 5
cp MYPWD/RunStatsCombo.py .
cp MYPWD/combineCards.py .
python RunStatsCombo.py --do_munu MYOPTIONS
sleep 5
cp ComboLog*txt MYDIR
