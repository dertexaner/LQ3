#!/bin/bash
#analysisClass.C -> ../LAC/analysisClass_DATA_MC_LTEM_CRtests_MuTau_OfflineSelectionSkimHistograms_V3new.C
#analysisClass.h -> ../LAC/analysisClass_LAC.h
#rootNtupleClass.h -> ../LAC/rootNtupleClass_DATA_MC.h
./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_NODATA_NOLQ3.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9_Histograms_V6_IsoMu24Mu24Mu40_SignalRegionLQ3AllMasses_noSF_AllSysV5bugfix_BARREL/ \
-j 1 -q 1nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_RAREONLY.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9_Histograms_V6_IsoMu24Mu24Mu40_SignalRegionLQ3AllMasses_noSF_AllSysV5bugfix_BARREL/ \
-j 1 -q 1nd

#include "LAC/ADDON5_H_LAC.C"
#rootNtupleClass.h -> ../LAC/rootNtupleClass_H.h
./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_HIGGSONLY.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9_Histograms_V6_IsoMu24Mu24Mu40_SignalRegionLQ3AllMasses_noSF_AllSysV5bugfix_BARREL/ \
-j 1 -q 1nd


#include "LAC/ADDON5_LQ_LAC.C"
#rootNtupleClass.h -> ../LAC/rootNtupleClass_LQ.h
./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_LQ3ONLY.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9_Histograms_V6_IsoMu24Mu24Mu40_SignalRegionLQ3AllMasses_noSF_AllSysV5bugfix_BARREL/ \
-j 1 -q 1nd 

