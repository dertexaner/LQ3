#!/bin/bash
#analysisClass.C -> ../LAC/analysisClass_DATA_MC_LTEM_MuTauSkim.C
#analysisClass.h -> ../LAC/analysisClass_LAC.h
#rootNtupleClass.h -> ../LAC/rootNtupleClass_DATA_MC.h

#submitted
#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_QCD.txt \
#-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
#-j 50 -q 1nd

#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_WJets.txt \
#-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
#-j 50 -q 1nd

#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_SingleTop.txt \
#-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
#-j 25 -q 2nd

#submitted
#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_TTJets.txt \
#-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
#-j 75 -q 2nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_SingleMu_Run2012A.txt \
-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
-j 50 -q 1nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_SingleMu_Run2012B.txt \
-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
-j 50 -q 1nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_SingleMu_Run2012C.txt \
-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
-j 50 -q 2nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_SingleMu_Run2012D.txt \
-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
-j 50 -q 2nd

#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_ZJets.txt \
#-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
#-j 25 -q 2nd

#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_ZJets_byLeptonChannel.txt \
#-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
#-j 25 -q 2nd

##./scripts/launchAnalysis_batch.pl \
##-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_ZJets_tarball.txt \
##-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
##-n rootTupleTree/tree \
##-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
##-j 25 -q 2nd

#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_DiBoson.txt \
#-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
#-j 25 -q 1nd

#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_LQToTTau.txt \
#-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
#-j 1 -q 1nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_RareSS.txt \
-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
-j 100 -q 1nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_RareSS_TopUp.txt \
-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
-j 100 -q 1nd

#this is a fix for Higgs gen matching (It should only affect HtoTauTau sample)
./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_RareSS_HiggsGenMatchFix.txt \
-c config/cutFiles/cutFile_DATA_MC_JsonTrigOfflineSkim_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9/ \
-j 100 -q 1nd

