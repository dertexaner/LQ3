#!/bin/bash
#analysisClass.C -> ../LAC/analysisClass_DATA_MC_LTEM_PromptMu_OfflineSelectionSkimHistograms_MuTrigger_V2.C
#analysisClass.h -> ../LAC/analysisClass_LAC.h
#rootNtupleClass.h -> ../LAC/rootNtupleClass_DATA_MC.h

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuMuSkim_V1/inputFile_batch_analysisClass_DATA_MC_LTEM_MuMuSkim_NOLQ3_NOZJETS.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuMuSkim_V1_Histograms_V1_dR0p4toInf_Endcap_MuPromptRate/ \
-j 1 -q 2nd


./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_ZJets.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuMuSkim_V1_Histograms_V1_dR0p4toInf_Endcap_MuPromptRate/ \
-j 100 -q 1nd


./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_ZJets_byLeptonChannel.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuMuSkim_V1_Histograms_V1_dR0p4toInf_Endcap_MuPromptRate/ \
-j 100 -q 2nd


# this edit is ignored!!!!!!!!! > > >
#analysisClass.C -> ../LAC/analysisClass_DATA_MC_LTEM_FakeTau_OfflineSelectionSkimHistograms_MuTrigger.C   < EDIT FOR LQ3 specific gen-reco matching
#analysisClass.h -> ../LAC/analysisClass_LAC.h
#rootNtupleClass.h -> ../LAC/rootNtupleClass_LQ3.h
# < < <

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuMuSkim_V1/inputFile_batch_analysisClass_DATA_MC_LTEM_MuMuSkim_LQ3ONLY.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuMuSkim_V1_Histograms_V1_dR0p4toInf_Endcap_MuPromptRate/ \
-j 1 -q 2nd

#ADDED 31-AUG-2013
./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_RareSS.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuMuSkim_V1_Histograms_V1_dR0p4toInf_Endcap_MuPromptRate/ \
-j 50 -q 2nd