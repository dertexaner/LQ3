#!/bin/bash
#analysisClass.C -> ../LAC/analysisClass_DATA_MC_LTEM_Preselection_MuTau_OfflineSelectionSkimHistograms.C
#analysisClass.h -> ../LAC/analysisClass_LAC.h
#rootNtupleClass.h -> ../LAC/rootNtupleClass_DATA_MC.h

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_NODATA_NOLQ3.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9_Histograms_V3_MuIso24_Preselection/ \
-j 1 -q 1nd

sleep 5

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_RAREONLY.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9_Histograms_V3_MuIso24_Preselection/ \
-j 1 -q 1nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_RAREONLY_TOPUP.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9_Histograms_V3_MuIso24_Preselection/ \
-j 1 -q 1nd

#

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_DATAONLY.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9_Histograms_V3_MuIso24_Preselection/ \
-j 1 -q 1nd

sleep 5
# IF NEEDED FOR ACTUAL PROMPT FAKE PLOTS..
# >>> FOR LQ3 specific gen-reco matching
#analysisClass.C -> ../LAC/analysisClass_DATA_MC_LTEM_Preselection_MuTau_OfflineSelectionSkimHistograms.C
#analysisClass.h -> ../LAC/analysisClass_LAC.h
#rootNtupleClass.h -> ../LAC/rootNtupleClass_LQ3.h

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_LQ3ONLY.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9_Histograms_V3_MuIso24_Preselection/ \
-j 1 -q 1nd 
