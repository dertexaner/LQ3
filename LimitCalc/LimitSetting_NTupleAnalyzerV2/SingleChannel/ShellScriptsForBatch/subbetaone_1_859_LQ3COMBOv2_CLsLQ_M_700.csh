#!/bin/csh  
cd /afs/cern.ch/work/h/hsaka/private/LQ3/LimitCalc/DarinsCLsTool/CMSSW_6_1_1/src/LimitSetting_NTupleAnalyzerV2/SingleChannel  
eval `scramv1 runtime -csh`  
cd -  
cp /afs/cern.ch/work/h/hsaka/private/LQ3/LimitCalc/DarinsCLsTool/CMSSW_6_1_1/src/LimitSetting_NTupleAnalyzerV2/SingleChannel/CLSLimits/BetaOneLQ3COMBOv2_CLs/confbetaone_LQ3COMBOv2_CLs_LQ_M_700.cfg .   
combine -M HybridNew --frequentist --testStat LHC confbetaone_LQ3COMBOv2_CLs_LQ_M_700.cfg --clsAcc=0 -s -1 -T 70 -i 70 --singlePoint 1.859 --saveToys --saveHybridResult
cp *root /afs/cern.ch/work/h/hsaka/private/LQ3/LimitCalc/DarinsCLsTool/CMSSW_6_1_1/src/LimitSetting_NTupleAnalyzerV2/SingleChannel/CLSLimits/BetaOneLQ3COMBOv2_CLs/LQ_M_700/  
  
rfcp *root /castor/cern.ch/user/h/hsaka/CLSLimits/BetaOneLQ3COMBOv2_CLs/LQ_M_700/  
  
