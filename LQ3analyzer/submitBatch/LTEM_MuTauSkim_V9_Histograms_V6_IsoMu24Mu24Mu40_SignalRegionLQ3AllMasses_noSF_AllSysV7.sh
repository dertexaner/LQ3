#!/bin/bash
# IF NEEDED FOR ACTUAL PROMPT FAKE PLOTS..  
# >>> FOR LQ3 specific gen-reco matching
#analysisClass.C -> ../LAC/analysisClass_DATA_MC_LTEM_CRtests_MuTau_OfflineSelectionSkimHistograms_V3new_PDFunc_noLJfilter.C
#analysisClass.h -> ../LAC/analysisClass_LAC.h
#rootNtupleClass.h -> ../LAC/rootNtupleClass_LQ3.h
#
#NEED TO REPLACE  include "LAC/ADDON5_LAC.C"  with  #include "LAC/ADDON5_LQ_LAC.C"
#
# skimmed sample
#-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_LQ3ONLY.txt \

# THIS IS for all LQ3 events, with NO FILTER! 
./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_LQToTTau.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9_Histograms_V6_IsoMu24Mu24Mu40_SignalRegionLQ3AllMasses_noSF_AllSysV7/ \
-j 1 -q 1nd 

