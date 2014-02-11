#!/bin/bash

# analysisClass.C -> ../LAC/analysisClass_MC_LTEM_PromptFakeMuTauStudies.C

#disabled for now, since im looking at prompt muons at the moment
#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_QCD.txt \
#-c config/cutFiles/cutFile_MC_ReducedSkim_PromptFakeMuTauStudies.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptFakeMuTau_MCStudies_V5/ \
#-j 50 -q 1nd

#disabled to save time, tarball sample is already used!
#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_WJets.txt \
#-c config/cutFiles/cutFile_MC_ReducedSkim_PromptFakeMuTauStudies.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptFakeMuTau_MCStudies_V5/ \
#-j 50 -q 1nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_TTJets.txt \
-c config/cutFiles/cutFile_MC_ReducedSkim_PromptFakeMuTauStudies.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptFakeMuTau_MCStudies_V5/ \
-j 50 -q 1nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_ZJets_tarball.txt \
-c config/cutFiles/cutFile_MC_ReducedSkim_PromptFakeMuTauStudies.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptFakeMuTau_MCStudies_V5/ \
-j 100 -q 1nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_LQToTTau.txt \
-c config/cutFiles/cutFile_MC_ReducedSkim_PromptFakeMuTauStudies.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptFakeMuTau_MCStudies_V5/ \
-j 1 -q 1nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_WJets_tarball.txt \
-c config/cutFiles/cutFile_MC_ReducedSkim_PromptFakeMuTauStudies.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptFakeMuTau_MCStudies_V5/ \
-j 75 -q 1nd