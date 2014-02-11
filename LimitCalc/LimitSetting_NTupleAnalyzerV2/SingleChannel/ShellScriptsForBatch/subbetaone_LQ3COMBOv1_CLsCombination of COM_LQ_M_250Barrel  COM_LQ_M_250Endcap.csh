#!/bin/csh  
cd /afs/cern.ch/work/h/hsaka/private/LQ3/LimitCalc/DarinsCLsTool/CMSSW_6_1_1/src/LimitSetting_NTupleAnalyzerV2/SingleChannel  
eval `scramv1 runtime -csh`  
cd -  
cp /afs/cern.ch/work/h/hsaka/private/LQ3/LimitCalc/DarinsCLsTool/CMSSW_6_1_1/src/LimitSetting_NTupleAnalyzerV2/SingleChannel/CLSLimits/BetaOneLQ3COMBOv1_CLs/confbetaone_LQ3COMBOv1_CLs_Combination of COM_LQ_M_250Barrel  COM_LQ_M_250Endcap.cfg .   
SUBCOMMAND
cp *root /afs/cern.ch/work/h/hsaka/private/LQ3/LimitCalc/DarinsCLsTool/CMSSW_6_1_1/src/LimitSetting_NTupleAnalyzerV2/SingleChannel/CLSLimits/BetaOneLQ3COMBOv1_CLs/Combination of COM_LQ_M_250Barrel  COM_LQ_M_250Endcap/  
  
rfcp *root /castor/cern.ch/user/h/hsaka/CLSLimits/BetaOneLQ3COMBOv1_CLs/Combination of COM_LQ_M_250Barrel  COM_LQ_M_250Endcap/  
  
