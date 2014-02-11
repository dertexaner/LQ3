#!/bin/csh  
cd /afs/cern.ch/work/h/hsaka/private/LQ3/LimitCalc/DarinsCLsTool/CMSSW_6_1_1/src/LimitSetting_NTupleAnalyzerV2/SingleChannel  
eval `scramv1 runtime -csh`  
cd -  
cp /afs/cern.ch/work/h/hsaka/private/LQ3/LimitCalc/DarinsCLsTool/CMSSW_6_1_1/src/LimitSetting_NTupleAnalyzerV2/SingleChannel/CLSLimits/BetaOneLQ3ENDv9_CLs/confbetaone_LQ3ENDv9_CLs_LQ_M_300.cfg .   
SUBCOMMAND
cp *root /afs/cern.ch/work/h/hsaka/private/LQ3/LimitCalc/DarinsCLsTool/CMSSW_6_1_1/src/LimitSetting_NTupleAnalyzerV2/SingleChannel/CLSLimits/BetaOneLQ3ENDv9_CLs/LQ_M_300/  
  
rfcp *root /castor/cern.ch/user/h/hsaka/CLSLimits/BetaOneLQ3ENDv9_CLs/LQ_M_300/  
  
