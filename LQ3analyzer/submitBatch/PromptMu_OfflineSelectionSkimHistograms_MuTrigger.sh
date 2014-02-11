#!/bin/bash

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_PromptMuSkim_MuTrigger/inputFile_batch_analysisClass_DATA_MC_LTEM_PromptMuSkim_MuTrigger.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_PromptStudies_MuTrigger.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV2/ \
-j 1 -q 1nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_SingleMu_Run2012A.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_PromptStudies_MuTrigger.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV2/ \
-j 50 -q 1nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_SingleMu_Run2012B.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_PromptStudies_MuTrigger.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV2/ \
-j 50 -q 1nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_SingleMu_Run2012C.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_PromptStudies_MuTrigger.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV2/ \
-j 50 -q 1nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_SingleMu_Run2012D.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_PromptStudies_MuTrigger.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV2/ \
-j 50 -q 1nd

#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_ZJets.txt 

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_ZJets_tarball.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_PromptStudies_MuTrigger.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV2/ \
-j 100 -q 2nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_DiBoson.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_PromptStudies_MuTrigger.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV2/ \
-j 50 -q 1nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_LQToTTau.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_PromptStudies_MuTrigger.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV2/ \
-j 1 -q 1nd

