#!/bin/bash
#analysisClass.C -> ../LAC/analysisClass_DATA_MC_LTEM_MuTau_OfflineSelectionSkimHistograms.C
#analysisClass.h -> ../LAC/analysisClass_LAC.h
#rootNtupleClass.h -> ../LAC/rootNtupleClass_DATA_MC.h

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V1_Histograms_V1_IsoMu24/ \
-j 1 -q 2nd


