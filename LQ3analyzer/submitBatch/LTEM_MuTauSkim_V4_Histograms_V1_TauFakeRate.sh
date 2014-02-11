#!/bin/bash
#analysisClass.C -> ../LAC/analysisClass_DATA_MC_LTEM_FakeTau_OfflineSelectionSkimHistograms_MuTrigger.C
#analysisClass.h -> ../LAC/analysisClass_LAC.h
#rootNtupleClass.h -> ../LAC/rootNtupleClass_DATA_MC.h

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V4/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_NOLQ3.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V4_Histograms_V1_TauFakeRate/ \
-j 1 -q 2nd



#analysisClass.C -> ../LAC/analysisClass_DATA_MC_LTEM_FakeTau_OfflineSelectionSkimHistograms_MuTrigger.C   < EDIT FOR LQ3 specific gen-reco matching
#analysisClass.h -> ../LAC/analysisClass_LAC.h
#rootNtupleClass.h -> ../LAC/rootNtupleClass_LQ3.h

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V4/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_LQ3ONLY.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V4_Histograms_V1_TauFakeRate/ \
-j 1 -q 2nd