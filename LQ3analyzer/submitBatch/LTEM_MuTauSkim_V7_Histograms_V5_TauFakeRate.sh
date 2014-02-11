#!/bin/bash
#analysisClass.C -> ../LAC/analysisClass_DATA_MC_LTEM_FakeTau_OfflineSelectionSkimHistograms_MuTrigger.C
#analysisClass.h -> ../LAC/analysisClass_LAC.h
#rootNtupleClass.h -> ../LAC/rootNtupleClass_DATA_MC.h

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V6/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_NOLQ3.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V7_Histograms_V5_TauFakeRate/ \
-j 1 -q 2nd



# this edit is ignored!!!!!!!!! > > >
#analysisClass.C -> ../LAC/analysisClass_DATA_MC_LTEM_FakeTau_OfflineSelectionSkimHistograms_MuTrigger.C   < EDIT FOR LQ3 specific gen-reco matching
#analysisClass.h -> ../LAC/analysisClass_LAC.h
#rootNtupleClass.h -> ../LAC/rootNtupleClass_LQ3.h
# < < <

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V6/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_LQ3ONLY.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V7_Histograms_V5_TauFakeRate/ \
-j 1 -q 2nd