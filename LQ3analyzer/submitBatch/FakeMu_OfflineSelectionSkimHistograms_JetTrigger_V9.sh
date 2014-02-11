#!/bin/bash
#analysisClass.C -> ../LAC/analysisClass_DATA_MC_LTEM_FakeMu_OfflineSelectionSkimHistograms_JetTrigger.C

./scripts/launchAnalysis_batch.pl \
-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_QCD.txt \
-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt \
-n rootTupleTree/tree \
-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_FakeMuSkim_JetTrigger_HistogramsV9/ \
-j 50 -q 1nd

##./scripts/launchAnalysis_batch.pl \
##-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_WJets.txt \
##-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt \
##-n rootTupleTree/tree \
##-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_FakeMuSkim_JetTrigger_HistogramsV9/ \
##-j 50 -q 1nd

#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_WJets_tarball.txt \
#-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_FakeMuSkim_JetTrigger_HistogramsV9/ \
#-j 50 -q 1nd

#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_SingleTop.txt \
#-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_FakeMuSkim_JetTrigger_HistogramsV9/ \
#-j 25 -q 2nd

#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_TTJets.txt \
#-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_FakeMuSkim_JetTrigger_HistogramsV9/ \
#-j 50 -q 1nd

#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_JetHT_Run2012C.txt \
#-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_FakeMuSkim_JetTrigger_HistogramsV9/ \
#-j 100 -q 1nd

#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_JetHT_Run2012D.txt \
#-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_FakeMuSkim_JetTrigger_HistogramsV9/ \
#-j 100 -q 1nd

##./scripts/launchAnalysis_batch.pl \
##-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_ZJets.txt \
##-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt \
##-n rootTupleTree/tree \
##-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_FakeMuSkim_JetTrigger_HistogramsV9/ \
##-j 100 -q 2nd

#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_ZJets_tarball.txt \
#-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_FakeMuSkim_JetTrigger_HistogramsV9/ \
#-j 100 -q 2nd

#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_DiBoson.txt \
#-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_FakeMuSkim_JetTrigger_HistogramsV9/ \
#-j 50 -q 2nd

#./scripts/launchAnalysis_batch.pl \
#-i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_LQToTTau.txt \
#-c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt \
#-n rootTupleTree/tree \
#-o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_FakeMuSkim_JetTrigger_HistogramsV9/ \
#-j 1 -q 2nd

