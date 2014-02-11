#!/bin/bash
#analysisClass.C -> ../LAC/analysisClass_DATA_MC_LTEM_CRtests_MuTau_OfflineSelectionSkimHistograms_V6_CheckLowestPtJet.C
#analysisClass.h -> ../LAC/analysisClass_LAC.h
#rootNtupleClass.h -> ../LAC/rootNtupleClass_DATA_MC.h
# USING LAC/ADDON1_LAC_Alt.C, this is included in LAC/ADDON2_LAC.C !!! and has the fixed tauFR and tauFRE

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_NODATA_NOLQ3.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9_Histograms_V15_IsoMu24Mu24Mu40_SignalRegionMCResultsLTEM_noSF_AllMasses_WithSTCorrForTaus/ \
-j 1 -q 2nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_RAREONLY.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9_Histograms_V15_IsoMu24Mu24Mu40_SignalRegionMCResultsLTEM_noSF_AllMasses_WithSTCorrForTaus/ \
-j 1 -q 2nd

./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_DATAONLY.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9_Histograms_V15_IsoMu24Mu24Mu40_SignalRegionMCResultsLTEM_noSF_AllMasses_WithSTCorrForTaus/ \
-j 1 -q 2nd


#USE: rootNtupleClass_H.h and
#rootNtupleClass.h -> ../LAC/rootNtupleClass_H.h
./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_HIGGSONLY.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9_Histograms_V15_IsoMu24Mu24Mu40_SignalRegionMCResultsLTEM_noSF_AllMasses_WithSTCorrForTaus/ \
-j 1 -q 2nd


#USE: rootNtupleClass_LQ.h and
#rootNtupleClass.h -> ../LAC/rootNtupleClass_LQ3.h
./scripts/launchAnalysis_batch.pl \
-i config/inputFile_analysisClass_DATA_MC_LTEM_MuTauSkim_V9/inputFile_batch_analysisClass_DATA_MC_LTEM_MuTauSkim_LQ3ONLY.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_Dilepton.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_MuTauSkim_V9_Histograms_V15_IsoMu24Mu24Mu40_SignalRegionMCResultsLTEM_noSF_AllMasses_WithSTCorrForTaus/ \
-j 1 -q 1nd 

