#!/bin/csh  
cd /afs/cern.ch/work/h/hsaka/private/LQ3/LimitCalc/DarinsCLsTool/CMSSW_6_1_1/src/LimitSetting_NTupleAnalyzerV2/SingleChannel  
eval `scramv1 runtime -csh`  
cd -  
cp /afs/cern.ch/work/h/hsaka/private/LQ3/LimitCalc/DarinsCLsTool/CMSSW_6_1_1/src/LimitSetting_NTupleAnalyzerV2/SingleChannel/CLSLimits/BetaOneLQ3ENDv8_CLs/confbetaone_LQ3ENDv8_CLs_LQ_M_800.cfg .   
combine -M HybridNew --frequentist --testStat LHC confbetaone_LQ3ENDv8_CLs_LQ_M_800.cfg --clsAcc=0 -s -1 -T 100 -i 100 --rMax 1000.0 --singlePoint 4.75909 --saveToys --saveHybridResult
cp *root /afs/cern.ch/work/h/hsaka/private/LQ3/LimitCalc/DarinsCLsTool/CMSSW_6_1_1/src/LimitSetting_NTupleAnalyzerV2/SingleChannel/CLSLimits/BetaOneLQ3ENDv8_CLs/LQ_M_800/  
  
rfcp *root /castor/cern.ch/user/h/hsaka/CLSLimits/BetaOneLQ3ENDv8_CLs/LQ_M_800/  
  
