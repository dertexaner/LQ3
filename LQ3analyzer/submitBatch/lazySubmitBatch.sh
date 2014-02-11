#!/bin/bash
echo
#./scripts/launchAnalysis_batch.pl -i config/inputFile_analysisClass_DATA_MC_LTEM_FakeMuorTauSkim_JetTrigger/inputFile_batch_analysisClass_DATA_MC_LTEM_FakeMuorTauSkim_JetTrigger.txt -c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt -n rootTupleTree/tree -o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013RootNtuple-V00-03-11_LTEM_FakeMuorTauSkim_JetTrigger_FakeMuHistogramsV33/ -j 1 -q 2nd
#sleep 10
#./scripts/launchAnalysis_batch.pl -i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_QCD.txt -c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt -n rootTupleTree/tree -o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013RootNtuple-V00-03-11_LTEM_FakeMuorTauSkim_JetTrigger_FakeMuHistogramsV33/ -j 50 -q 1nd
#sleep 10
#./scripts/launchAnalysis_batch.pl -i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_LQToTTau.txt -c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt -n rootTupleTree/tree -o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013RootNtuple-V00-03-11_LTEM_FakeMuorTauSkim_JetTrigger_FakeMuHistogramsV33/ -j 1 -q 2nd
#sleep 10
#./scripts/launchAnalysis_batch.pl -i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_ZJets_noLepton.txt -c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt -n rootTupleTree/tree -o  /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013RootNtuple-V00-03-11_LTEM_FakeMuorTauSkim_JetTrigger_FakeMuHistogramsV33/ -j 50 -q 2nd
#sleep 10
#./scripts/launchAnalysis_batch.pl -i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_WJets_noLepton.txt -c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_ReducedSkim_FakeStudies_JetTrigger.txt -n rootTupleTree/tree -o  /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013RootNtuple-V00-03-11_LTEM_FakeMuorTauSkim_JetTrigger_FakeMuHistogramsV33/ -j 50 -q 2nd
#sleep 10
#exit 0


#./scripts/launchAnalysis_batch.pl -i config/inputFile_analysisClass_DATA_MC_LTEM_PromptMuSkim_MuTrigger/inputFile_batch_analysisClass_DATA_MC_LTEM_PromptMuSkim_MuTrigger.txt -c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_PromptStudies_MuTrigger.txt -n rootTupleTree/tree -o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV1/ -j 1 -q 1nd
#sleep 5
#./scripts/launchAnalysis_batch.pl -i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_SingleMu_Run2012A.txt -cconfig/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_PromptStudies_MuTrigger.txt -n rootTupleTree/tree -o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV1/ -j 50 -q 1nd
#sleep 5
#./scripts/launchAnalysis_batch.pl -i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_SingleMu_Run2012B.txt -c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_PromptStudies_MuTrigger.txt -n rootTupleTree/tree -o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV1/ -j 50 -q 1nd
sleep 5
./scripts/launchAnalysis_batch.pl -i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_SingleMu_Run2012C.txt -c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_PromptStudies_MuTrigger.txt -n rootTupleTree/tree -o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV1/ -j 50 -q 1nd
sleep 5
./scripts/launchAnalysis_batch.pl -i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_SingleMu_Run2012D.txt -c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_PromptStudies_MuTrigger.txt -n rootTupleTree/tree -o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV1/ -j 50 -q 1nd
sleep 5
./scripts/launchAnalysis_batch.pl -i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_ZJets.txt -c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_PromptStudies_MuTrigger.txt -n rootTupleTree/tree -o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV1/ -j 100 -q 2nd
sleep 5
./scripts/launchAnalysis_batch.pl -i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_DiBoson.txt -c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_PromptStudies_MuTrigger.txt -n rootTupleTree/tree -o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV1/ -j 50 -q 1nd
sleep 5
./scripts/launchAnalysis_batch.pl -i config/inputFolder/inputList_batch_RootNtuple-V00-03-11_LQToTTau.txt -c config/cutFiles/cutFile_DATA_MC_OfflineSelectionSkimHistograms_PromptStudies_MuTrigger.txt -n rootTupleTree/tree -o /afs/cern.ch/work/h/hsaka/private/LQ3/BatchJobs2013/RootNtuple-V00-03-11_LTEM_PromptMuSkim_MuTrigger_HistogramsV1/ -j 1 -q 1nd
sleep 5
exit 0