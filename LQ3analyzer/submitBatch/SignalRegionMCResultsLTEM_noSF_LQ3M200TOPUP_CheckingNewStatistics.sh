#!/bin/bash
#analysisClass.C -> ../LAC/analysisClass_DATA_MC_LTEM_CRtests_MuTau_OfflineSelectionSkimHistograms_V6_CheckLowestPtJet.C
#analysisClass.h -> ../LAC/analysisClass_LAC.h
#rootNtupleClass.h -> ../LAC/rootNtupleClass_DATA_MC.h  # DONT USE THIS!
#
#USE:  rootNtupleClass.h -> ../LAC/rootNtupleClass_LQ3.h

# LQ3s - preskimmed - OLD
#./scripts/launchAnalysis_batch.pl \
#-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_LQ3ONLY.txt \
#-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/SignalRegionMCResultsLTEM_noSF_LQ3M200TOPUP_CheckingNewStatistics/ \
#-j 1 -q 1nd 

# LQ3M200 - raw - OLD and TOPUP
./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_LQ3M200_TOPUP.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/SignalRegionMCResultsLTEM_noSF_LQ3M200TOPUP_CheckingNewStatistics/ \
-j 1 -q 1nd