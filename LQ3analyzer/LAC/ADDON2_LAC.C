//////////////////////////////////////////////////////////
// Trigger Related Functions
// 
// H.S  - Feb 2012
//////////////////////////////////////////////////////////
#define analysisClass_cxx
#include "analysisClass.h"
#include <TH2.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TVector3.h>
#include <iostream>
//#include "ADDON1_LAC.C"
#include "ADDON1_LAC_Alt.C"
using namespace std;

// // //
int analysisClass::HLT_MuPT_eta2p1_passTrigger(){
  //
  bool   isTrigFound_=false;
  bool   trigDec_=false;
  bool   goodTrigOverlap_=false;
  int    goodTrigNumber=5;
  int    SingleMuTrigDec=0;
  //
  //string SingleMuTriggers[3]={"HLT_Mu30_eta2p1_v3","HLT_Mu30_eta2p1_v4","HLT_Mu30_eta2p1_v5"};//DATA & MC 
  string SingleMuTriggers[5]={"HLT_Mu24_eta2p1_v1","HLT_Mu24_eta2p1_v2","HLT_Mu24_eta2p1_v3","HLT_Mu24_eta2p1_v4","HLT_Mu24_eta2p1_v5"};
  //			      "HLT_Mu30_eta2p1_v1","HLT_Mu30_eta2p1_v2","HLT_Mu30_eta2p1_v3","HLT_Mu30_eta2p1_v4","HLT_Mu30_eta2p1_v5",
  //			      "HLT_Mu40_eta2p1_v9","HLT_Mu40_eta2p1_v10","HLT_Mu40_eta2p1_v11"}
  //
  for(unsigned int itrig=0; itrig<HLTInsideDatasetTriggerNames->size(); itrig++){
    TString trigname=HLTInsideDatasetTriggerNames->at(itrig);
    for(int igoodtrig=0; igoodtrig<goodTrigNumber; igoodtrig++){
      if( trigname==SingleMuTriggers[igoodtrig] ){
        if(isTrigFound_){cout<<" WARNING!!!  Good Triggers Overlap"<<endl; goodTrigOverlap_=true;}
        isTrigFound_=true;
	SingleMuTrigDec=(igoodtrig+1);
	vector<bool>  * TriggerDecisions;
        TriggerDecisions=HLTInsideDatasetTriggerDecisions;
        trigDec_=(*TriggerDecisions)[itrig];
	// -------------------- debug -------------------- 
	//cout<<"SingleMu: "<<trigname<<" : "<<trigDec_<<" "<<endl;//debug
	// -------------------- debug -------------------- 	
      }
    }
  }//loop over all triggers per event
  //
  if(!isTrigFound_){cout<<" WARNING!!!  Trigger Not Found"<<endl; }
  //
  if(    !isTrigFound_ ){ return -2;}
  if( goodTrigOverlap_ ){ return -1;}
  if(        !trigDec_ ){ return  0;}
  if(         trigDec_ ){ return  SingleMuTrigDec;}// returns 1,2,3
  //
}
// // //
int analysisClass::SingleMu40_passTrigger(){
  //
  bool   isTrigFound_=false;
  bool   trigDec_=false;
  bool   goodTrigOverlap_=false;
  int    goodTrigNumber=3;
  int    SingleMuTrigDec=0;
  //
  //string SingleMuTriggers[4]={"HLT_Mu40_v11","HLT_Mu40_v12","HLT_Mu40_v13","HLT_Mu40_v14"};//DATA & MC 
  string SingleMuTriggers[3]={"HLT_Mu40_eta2p1_v9","HLT_Mu40_eta2p1_v10","HLT_Mu40_eta2p1_v11"};//DATA & MC 
  //
  for(unsigned int itrig=0; itrig<HLTInsideDatasetTriggerNames->size(); itrig++){
    TString trigname=HLTInsideDatasetTriggerNames->at(itrig);
    for(int igoodtrig=0; igoodtrig<goodTrigNumber; igoodtrig++){
      if( trigname==SingleMuTriggers[igoodtrig] ){
        if(isTrigFound_){cout<<" WARNING!!!  Good Triggers Overlap"<<endl; goodTrigOverlap_=true;}
        isTrigFound_=true;
	SingleMuTrigDec=(igoodtrig+1);
	vector<bool>  * TriggerDecisions;
        TriggerDecisions=HLTInsideDatasetTriggerDecisions;
        trigDec_=(*TriggerDecisions)[itrig];
	// -------------------- debug -------------------- 
	//cout<<"SingleMu: "<<trigname<<" : "<<trigDec_<<" "<<endl;//debug
	// -------------------- debug -------------------- 	
      }
    }
  }//loop over all triggers per event
  //
  if(!isTrigFound_){cout<<" WARNING!!!  Trigger Not Found"<<endl; }
  //
  if(    !isTrigFound_ ){ return -2;}
  if( goodTrigOverlap_ ){ return -1;}
  if(        !trigDec_ ){ return  0;}
  if(         trigDec_ ){ return  SingleMuTrigDec;}// returns 1,2,3,4
  //
}
// // //
int analysisClass::Jet_passTrigger(){
  //
  bool   isTrigFound_=false;
  bool   trigDec_=false;
  bool   goodTrigOverlap_=false;
  int    goodTrigNumber=9;
  int    TrigDec=0;
  //
  //string Triggers[6]={"HLT_QuadJet80_v1","HLT_QuadJet80_v2","HLT_QuadJet80_v3","HLT_QuadJet80_v4","HLT_QuadJet80_v5","HLT_QuadJet80_v6"};
  //string Triggers[9]={"HLT_HT350_v1","HLT_HT350_v2","HLT_HT350_v3","HLT_HT350_v4","HLT_HT350_v5","HLT_HT350_v6","HLT_HT350_v7","HLT_HT350_v8","HLT_HT350_v9"};
  //string Triggers[9]={"HLT_PFJet320_v1","HLT_PFJet320_v2","HLT_PFJet320_v3","HLT_PFJet320_v4","HLT_PFJet320_v5",
  //  		       "HLT_PFJet320_v6","HLT_PFJet320_v7","HLT_PFJet320_v8","HLT_PFJet320_v9"};
  //string Triggers[9]={"HLT_PFJet200_v1","HLT_PFJet200_v2","HLT_PFJet200_v3","HLT_PFJet200_v4","HLT_PFJet200_v5",
  //                    "HLT_PFJet200_v6","HLT_PFJet200_v7","HLT_PFJet200_v8","HLT_PFJet200_v9"};
  //string Triggers[9]={"HLT_PFJet140_v1","HLT_PFJet140_v2","HLT_PFJet140_v3","HLT_PFJet140_v4","HLT_PFJet140_v5",
  //    		      "HLT_PFJet140_v6","HLT_PFJet140_v7","HLT_PFJet140_v8","HLT_PFJet140_v9"};
  //  string Triggers[9]={"HLT_PFJet80_v1","HLT_PFJet80_v2","HLT_PFJet80_v3","HLT_PFJet80_v4","HLT_PFJet80_v5",
  //      		      "HLT_PFJet80_v6","HLT_PFJet80_v7","HLT_PFJet80_v8","HLT_PFJet80_v9"};
  string Triggers[9]={"HLT_PFJet40_v1","HLT_PFJet40_v2","HLT_PFJet40_v3","HLT_PFJet40_v4","HLT_PFJet40_v5",
      		      "HLT_PFJet40_v6","HLT_PFJet40_v7","HLT_PFJet40_v8","HLT_PFJet40_v9"};
  //
  //previously used
  //string Triggers[10]={"HLT_PFNoPUHT650_v1","HLT_PFNoPUHT650_v2","HLT_PFNoPUHT650_v3","HLT_PFNoPUHT650_v4","HLT_PFNoPUHT650_v5",
  //  		       "HLT_PFNoPUHT650_v6","HLT_PFNoPUHT650_v7","HLT_PFNoPUHT650_v8","HLT_PFNoPUHT650_v9",
  //  		       "HLT_HT650_v3"};
  //
  //string Triggers[10]={"HLT_HT250_v1","HLT_HT250_v2","HLT_HT250_v3","HLT_HT250_v4","HLT_HT250_v5",
  //  		      "HLT_HT250_v6","HLT_HT250_v7","HLT_HT250_v8","HLT_HT250_v9","HLT_HT250_v10"};
  //
  //string Triggers[9]={"HLT_HT750_v1","HLT_HT750_v2","HLT_HT750_v3","HLT_HT750_v4","HLT_HT750_v5",
  //  		      "HLT_HT750_v6","HLT_HT750_v7","HLT_HT750_v8","HLT_HT750_v9"};
  //
  //
  for(unsigned int itrig=0; itrig<HLTInsideDatasetTriggerNames->size(); itrig++){
    TString trigname=HLTInsideDatasetTriggerNames->at(itrig);
    for(int igoodtrig=0; igoodtrig<goodTrigNumber; igoodtrig++){
      if( trigname==Triggers[igoodtrig] ){
        if(isTrigFound_){cout<<" WARNING!!!  Good Triggers Overlap"<<endl; goodTrigOverlap_=true;}
        isTrigFound_=true;
	TrigDec=(igoodtrig+1);
	vector<bool>  * TriggerDecisions;
        TriggerDecisions=HLTInsideDatasetTriggerDecisions;
        trigDec_=(*TriggerDecisions)[itrig];
	// -------------------- debug -------------------- 
	//cout<<": "<<trigname<<" : "<<trigDec_<<" "<<endl;//debug
	// -------------------- debug -------------------- 	
      }
    }
  }//loop over all triggers per event
  //
  if(!isTrigFound_){cout<<" WARNING!!!  Trigger Not Found"<<endl; }
  //
  if(    !isTrigFound_ ){ return -2;}
  if( goodTrigOverlap_ ){ return -1;}
  if(        !trigDec_ ){ return  0;}
  if(         trigDec_ ){ return  TrigDec;}// returns 1,2,3,4,5,6
  //
}
// // //
int analysisClass::SingleMu_passTrigger(){
  //
  bool   isTrigFound_=false;
  bool   trigDec_=false;
  bool   goodTrigOverlap_=false;
  int    goodTrigNumber=5;
  int    SingleMuTrigDec=0;
  //
  //plot HLT_IsoMu24_eta2p1_v11 190645-190738 3.80 16.30 2.66 40.75 38.69 38.69 225,147 1.00
  //plot HLT_IsoMu24_eta2p1_v12 190782-193621 8.80 35.05 6.08 853.35 832.58 832.58 4,966,774 1.00
  //plot HLT_IsoMu24_eta2p1_v13 193834-196531 21.19 46.20 6.59 4,481.55 4,397.25 4,397.25 30,118,472 1.00
  //plot HLT_IsoMu24_eta2p1_v14 198049-199608 20.42 43.03 6.21 1,774.99 1,751.72 1,751.72 12,074,768 1.00
  //plot HLT_IsoMu24_eta2p1_v15 199698-208686 22.14 49.98 7.53 12,686.80 12,461.42 12,461.42 85,231,292 1.00
  string SingleMuTriggers[5]={"HLT_IsoMu24_eta2p1_v11","HLT_IsoMu24_eta2p1_v12","HLT_IsoMu24_eta2p1_v13","HLT_IsoMu24_eta2p1_v14","HLT_IsoMu24_eta2p1_v15"};
  //
  for(unsigned int itrig=0; itrig<HLTInsideDatasetTriggerNames->size(); itrig++){
    TString trigname=HLTInsideDatasetTriggerNames->at(itrig);
    for(int igoodtrig=0; igoodtrig<goodTrigNumber; igoodtrig++){
      if( trigname==SingleMuTriggers[igoodtrig] ){
        if(isTrigFound_){cout<<" WARNING!!!  Good Triggers Overlap"<<endl; goodTrigOverlap_=true;}
        isTrigFound_=true;
	SingleMuTrigDec=(igoodtrig+1);
	vector<bool>  * TriggerDecisions;
        TriggerDecisions=HLTInsideDatasetTriggerDecisions;
        trigDec_=(*TriggerDecisions)[itrig];
	// -------------------- debug -------------------- 
	//cout<<"SingleMu: "<<trigname<<" : "<<trigDec_<<" "<<endl;//debug
	// -------------------- debug -------------------- 	
      }
    }
  }//loop over all triggers per event
  //
  if(!isTrigFound_){cout<<" WARNING!!!  Trigger Not Found"<<endl; }
  //
  if(    !isTrigFound_ ){ return -2;}
  if( goodTrigOverlap_ ){ return -1;}
  if(        !trigDec_ ){ return  0;}
  if(         trigDec_ ){ return  SingleMuTrigDec;}// returns 1,2,3,4,5
  //
}
// // // 
int analysisClass::DoubleMu_passTrigger(){
  //
  bool   isTrigFound_=false;
  bool   trigDec_=false;
  bool   goodTrigOverlap_=false;
  int    goodTrigNumber=6;
  int    MuTrigDec=0;
  //
  //plot HLT_Mu17_Mu8_v16 190645-193621 2.60 8.88 6.08 894.05 871.28 871.28 1,224,742 1.00
  //plot HLT_Mu17_Mu8_v17 193834-196027 8.03 23.68 6.59 3,524.03 3,455.78 3,455.78 9,356,544 1.00
  //plot HLT_Mu17_Mu8_v18 196046-196531 9.67 24.54 6.48 957.52 941.47 941.47 2,730,805 1.00
  //plot HLT_Mu17_Mu8_v19 198049-199608 7.55 21.66 6.21 1,775.11 1,751.83 1,751.83 4,614,995 1.00
  //plot HLT_Mu17_Mu8_v21 199698-205238 6.05 13.04 7.41 6,807.67 6,687.98 6,687.98 11,647,473 1.00
  //plot HLT_Mu17_Mu8_v22 205303-208686 6.44 13.60 7.53 5,879.01 5,773.42 5,773.42 9,917,189 1.00
  string MuTriggers[6]={"HLT_Mu17_Mu8_v16","HLT_Mu17_Mu8_v17","HLT_Mu17_Mu8_v18","HLT_Mu17_Mu8_v19","HLT_Mu17_Mu8_v21","HLT_Mu17_Mu8_v22"};
  //
  for(unsigned int itrig=0; itrig<HLTInsideDatasetTriggerNames->size(); itrig++){
    TString trigname=HLTInsideDatasetTriggerNames->at(itrig);
    for(int igoodtrig=0; igoodtrig<goodTrigNumber; igoodtrig++){
      if( trigname==MuTriggers[igoodtrig] ){
        if(isTrigFound_){cout<<" WARNING!!!  Good Triggers Overlap"<<endl; goodTrigOverlap_=true;}
        isTrigFound_=true;
	MuTrigDec=(igoodtrig+1);
	vector<bool>  * TriggerDecisions;
        TriggerDecisions=HLTInsideDatasetTriggerDecisions;
        trigDec_=(*TriggerDecisions)[itrig];
	// -------------------- debug -------------------- 
	//cout<<"Mu: "<<trigname<<" : "<<trigDec_<<" "<<endl;//debug
	// -------------------- debug -------------------- 	
      }
    }
  }//loop over all triggers per event
  //
  if(!isTrigFound_){cout<<" WARNING!!!  Trigger Not Found"<<endl; }
  //
  //return 5; //debugging
  if(    !isTrigFound_ ){ return -2;}
  if( goodTrigOverlap_ ){ return -1;}
  if(        !trigDec_ ){ return  0;}
  if(         trigDec_ ){ return  MuTrigDec;}// returns 1,2,3,4,5,6
  //
}
// // // 
bool analysisClass::isAllMuonsHLT( ){
  //
  int singlemutrig = SingleMu_passTrigger();
  if( singlemutrig < 1 ) return false;
  //
  for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
    if( !muRisoCheck(iMuR)        )continue;
    if( RecoHLTdeltaRmin_SingleMuTrigger( iMuR )>0.1 ) return false;
  }
  return true;
}
// // //
double analysisClass::RecoHLTdeltaRmin_DoubleMuTrigger( unsigned int iReco ){
  TLorentzVector HLTLep;
  TLorentzVector RecoLep;
  double deltaRmin=999999.9;
  RecoLep.SetPtEtaPhiM( muPtcorr(iReco),   MuonEta->at(iReco),   MuonPhi->at(iReco),   0);
  //
  for(unsigned int ifilter=0; ifilter<HLTFilterName->size(); ifilter++){  
    //
    // HLT_Mu17_Mu8_v16 hltL3pfL1DoubleMu10MuOpenL1f0L2pf0L3PreFiltered8 hltL3fL1DoubleMu10MuOpenL1f0L2f10L3Filtered17 hltDiMuonMu17Mu8DzFiltered0p2                         
    // HLT_Mu17_Mu8_v17 hltL3pfL1DoubleMu10MuOpenL1f0L2pf0L3PreFiltered8 hltL3fL1DoubleMu10MuOpenL1f0L2f10L3Filtered17 <<<< MC                                               
    // HLT_Mu17_Mu8_v18                                                                                                                                                      
    // HLT_Mu17_Mu8_v19 hltL3pfL1DoubleMu10MuOpenOR3p5L1f0L2pf0L3PreFiltered8 hltL3fL1DoubleMu10MuOpenOR3p5L1f0L2f10L3Filtered17                                             
    // HLT_Mu17_Mu8_v21 hltL3pfL1DoubleMu10MuOpenOR3p5L1f0L2pf0L3PreFiltered8 hltL3fL1DoubleMu10MuOpenOR3p5L1f0L2f10L3Filtered17 hltDiMuonGlb17Glb8DzFiltered0p2             
    // HLT_Mu17_Mu8_v22 hltL3pfL1DoubleMu10MuOpenOR3p5L1f0L2pf0L3PreFiltered8 hltL3fL1DoubleMu10MuOpenOR3p5L1f0L2f10L3Filtered17 hltDiMuonGlb17Glb8DzFiltered0p2
    //
    if( HLTFilterName->at(ifilter) == "hltL3pfL1DoubleMu10MuOpenL1f0L2pf0L3PreFiltered8"      ||
	HLTFilterName->at(ifilter) == "hltL3fL1DoubleMu10MuOpenL1f0L2f10L3Filtered17"         ||
	HLTFilterName->at(ifilter) == "hltL3pfL1DoubleMu10MuOpenOR3p5L1f0L2pf0L3PreFiltered8" ||
	HLTFilterName->at(ifilter) == "hltL3fL1DoubleMu10MuOpenOR3p5L1f0L2f10L3Filtered17"
	){
      //
      // -----  extract TrigObject 4-vector  -----
      std::vector<std::vector<float> > * TrigObjectPt_TrigArray;  vector<float> trigObjPtArray;  float trigObjPt_;
      std::vector<std::vector<float> > * TrigObjectEta_TrigArray; vector<float> trigObjEtaArray; float trigObjEta_;
      std::vector<std::vector<float> > * TrigObjectPhi_TrigArray; vector<float> trigObjPhiArray; float trigObjPhi_;
      TrigObjectPt_TrigArray  = HLTFilterObjPt;    trigObjPtArray  = (*TrigObjectPt_TrigArray)[ifilter];
      TrigObjectEta_TrigArray = HLTFilterObjEta;   trigObjEtaArray = (*TrigObjectEta_TrigArray)[ifilter];
      TrigObjectPhi_TrigArray = HLTFilterObjPhi;   trigObjPhiArray = (*TrigObjectPhi_TrigArray)[ifilter];
      // ----- ----- ----- ----- ----- ----- -----
      for(unsigned int iobj=0; iobj<trigObjPtArray.size(); iobj++){//loop over all trigObjects of selected Filter
	trigObjPt_ = (trigObjPtArray)[iobj];  trigObjEta_= (trigObjEtaArray)[iobj];  trigObjPhi_= (trigObjPhiArray)[iobj];
	//debug
	//std::cout<<HLTFilterName->at(ifilter)<<" "<< trigObjPt_<<" "<<trigObjEta_<<" "<<trigObjPhi_<<std::endl;
	HLTLep.SetPtEtaPhiM( trigObjPt_, trigObjEta_, trigObjPhi_, 0 );
	if(RecoLep.DeltaR(HLTLep)<deltaRmin) deltaRmin = RecoLep.DeltaR(HLTLep);
      }
      //debug
      //std::cout<<std::endl;
    }
  }
  //
  return deltaRmin;
}
// // //
double analysisClass::RecoHLTdeltaRmin_SingleMuTrigger( unsigned int iReco ){
  TLorentzVector HLTLep;
  TLorentzVector RecoLep;
  double deltaRmin=999999.9;
  RecoLep.SetPtEtaPhiM( muPtcorr(iReco),   MuonEta->at(iReco),   MuonPhi->at(iReco),   0);
  //
  // This is the trigger list for 2012 :: 190456-203002
  //
  //|  HLT_IsoMu24_eta2p1_v11 |        833 |  28.303(/pb) |  28.303(/pb) | "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoFiltered10"
  //|  HLT_IsoMu24_eta2p1_v12 |      11046 | 669.888(/pb) | 669.888(/pb) | "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoFiltered10"
  //|  HLT_IsoMu24_eta2p1_v13 |      51172 |   4.427(/fb) |   4.427(/fb) | "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15"
  //|  HLT_IsoMu24_eta2p1_v14 |      22079 |   1.783(/fb) |   1.783(/fb) | "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15"
  //|  HLT_IsoMu24_eta2p1_v15 |      54847 |   5.023(/fb) |   5.023(/fb) | "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15"
  //
  //|  HLT_IsoMu20_eta2p1_v3 |        833 |  28.303(/pb) | 346.522(/nb) |  <<< PRESCALED "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f20L3crIsoFiltered10"
  //|  HLT_IsoMu20_eta2p1_v4 |      11046 | 669.888(/pb) |   4.160(/pb) |  <<< PRESCALED "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f20L3crIsoFiltered10"
  //|  HLT_IsoMu20_eta2p1_v5 |      51172 |   4.427(/fb) |  19.806(/pb) |  <<< PRESCALED "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f20L3crIsoRhoFiltered0p15"
  //|  HLT_IsoMu20_eta2p1_v6 |      22079 |   1.783(/fb) |   7.169(/pb) |  <<< PRESCALED "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f20L3crIsoRhoFiltered0p15"
  //|  HLT_IsoMu20_eta2p1_v7 |      54847 |   5.023(/fb) |  19.991(/pb) |  <<< PRESCALED "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f20L3crIsoRhoFiltered0p15"
  //
  //
  for(unsigned int ifilter=0; ifilter<HLTFilterName->size(); ifilter++){  
    //
    // HLT_IsoMu24_eta2p1_v*
    if( HLTFilterName->at(ifilter) == "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoFiltered10"  ||  //v11,v12
	HLTFilterName->at(ifilter) == "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15" //v13, v14, v15
	){
      //
      // -----  extract TrigObject 4-vector  -----
      std::vector<std::vector<float> > * TrigObjectPt_TrigArray;  vector<float> trigObjPtArray;  float trigObjPt_;
      std::vector<std::vector<float> > * TrigObjectEta_TrigArray; vector<float> trigObjEtaArray; float trigObjEta_;
      std::vector<std::vector<float> > * TrigObjectPhi_TrigArray; vector<float> trigObjPhiArray; float trigObjPhi_;
      TrigObjectPt_TrigArray  = HLTFilterObjPt;    trigObjPtArray  = (*TrigObjectPt_TrigArray)[ifilter];
      TrigObjectEta_TrigArray = HLTFilterObjEta;   trigObjEtaArray = (*TrigObjectEta_TrigArray)[ifilter];
      TrigObjectPhi_TrigArray = HLTFilterObjPhi;   trigObjPhiArray = (*TrigObjectPhi_TrigArray)[ifilter];
      // ----- ----- ----- ----- ----- ----- -----
      for(unsigned int iobj=0; iobj<trigObjPtArray.size(); iobj++){//loop over all trigObjects of selected Filter
	trigObjPt_ = (trigObjPtArray)[iobj];  trigObjEta_= (trigObjEtaArray)[iobj];  trigObjPhi_= (trigObjPhiArray)[iobj];
	HLTLep.SetPtEtaPhiM( trigObjPt_, trigObjEta_, trigObjPhi_, 0 );
	if(RecoLep.DeltaR(HLTLep)<deltaRmin) deltaRmin = RecoLep.DeltaR(HLTLep);
      }
    }
  }
  //
  return deltaRmin;
}
// // //
double analysisClass::RecoHLTdeltaRmin_SingleMu40Trigger( unsigned int iReco ){
  TLorentzVector HLTLep;
  TLorentzVector RecoLep;
  double deltaRmin=999999.9;
  RecoLep.SetPtEtaPhiM( muPtcorr(iReco),   MuonEta->at(iReco),   MuonPhi->at(iReco),   0);
  //
  for(unsigned int ifilter=0; ifilter<HLTFilterName->size(); ifilter++){  
    //
    // HLT_Mu40_eta2p1_v*
    if( HLTFilterName->at(ifilter) == "hltL3fL1sMu16Eta2p1L1f0L2f16QL3Filtered40Q" //V9,V10,V11
	){
      //
      // -----  extract TrigObject 4-vector  -----
      std::vector<std::vector<float> > * TrigObjectPt_TrigArray;  vector<float> trigObjPtArray;  float trigObjPt_;
      std::vector<std::vector<float> > * TrigObjectEta_TrigArray; vector<float> trigObjEtaArray; float trigObjEta_;
      std::vector<std::vector<float> > * TrigObjectPhi_TrigArray; vector<float> trigObjPhiArray; float trigObjPhi_;
      TrigObjectPt_TrigArray  = HLTFilterObjPt;    trigObjPtArray  = (*TrigObjectPt_TrigArray)[ifilter];
      TrigObjectEta_TrigArray = HLTFilterObjEta;   trigObjEtaArray = (*TrigObjectEta_TrigArray)[ifilter];
      TrigObjectPhi_TrigArray = HLTFilterObjPhi;   trigObjPhiArray = (*TrigObjectPhi_TrigArray)[ifilter];
      // ----- ----- ----- ----- ----- ----- -----
      for(unsigned int iobj=0; iobj<trigObjPtArray.size(); iobj++){//loop over all trigObjects of selected Filter
	trigObjPt_ = (trigObjPtArray)[iobj];  trigObjEta_= (trigObjEtaArray)[iobj];  trigObjPhi_= (trigObjPhiArray)[iobj];
	HLTLep.SetPtEtaPhiM( trigObjPt_, trigObjEta_, trigObjPhi_, 0 );
	if(RecoLep.DeltaR(HLTLep)<deltaRmin) deltaRmin = RecoLep.DeltaR(HLTLep);
      }
    }
  }
  //
  return deltaRmin;
}
// // //
double analysisClass::RecoHLTdeltaRmin_SingleMu24Trigger( unsigned int iReco ){
  TLorentzVector HLTLep;
  TLorentzVector RecoLep;
  double deltaRmin=999999.9;
  RecoLep.SetPtEtaPhiM( muPtcorr(iReco),   MuonEta->at(iReco),   MuonPhi->at(iReco),   0);
  //
  for(unsigned int ifilter=0; ifilter<HLTFilterName->size(); ifilter++){  
    //
    // HLT_Mu24_eta2p1_v*
    if( HLTFilterName->at(ifilter) == "hltL3fL1sMu16Eta2p1L1f0L2f16QL3Filtered24Q" //V3,V4,V5
	){
      //
      // -----  extract TrigObject 4-vector  -----
      std::vector<std::vector<float> > * TrigObjectPt_TrigArray;  vector<float> trigObjPtArray;  float trigObjPt_;
      std::vector<std::vector<float> > * TrigObjectEta_TrigArray; vector<float> trigObjEtaArray; float trigObjEta_;
      std::vector<std::vector<float> > * TrigObjectPhi_TrigArray; vector<float> trigObjPhiArray; float trigObjPhi_;
      TrigObjectPt_TrigArray  = HLTFilterObjPt;    trigObjPtArray  = (*TrigObjectPt_TrigArray)[ifilter];
      TrigObjectEta_TrigArray = HLTFilterObjEta;   trigObjEtaArray = (*TrigObjectEta_TrigArray)[ifilter];
      TrigObjectPhi_TrigArray = HLTFilterObjPhi;   trigObjPhiArray = (*TrigObjectPhi_TrigArray)[ifilter];
      // ----- ----- ----- ----- ----- ----- -----
      for(unsigned int iobj=0; iobj<trigObjPtArray.size(); iobj++){//loop over all trigObjects of selected Filter
	trigObjPt_ = (trigObjPtArray)[iobj];  trigObjEta_= (trigObjEtaArray)[iobj];  trigObjPhi_= (trigObjPhiArray)[iobj];
	HLTLep.SetPtEtaPhiM( trigObjPt_, trigObjEta_, trigObjPhi_, 0 );
	if(RecoLep.DeltaR(HLTLep)<deltaRmin) deltaRmin = RecoLep.DeltaR(HLTLep);
      }
    }
  }
  //
  return deltaRmin;
}
// // //
double analysisClass::MuonForTrigEffAnalysis_SingleMuTrigger(  int probeCharge, TString OutputChoice_, int singleMuTrig ){
  //
  if( OutputChoice_ != "MuonNumber" && OutputChoice_ != "ZMass" ){ cout<<" WRONG OUTPUT CHOICE SELECTED!!!"<<endl; return (double)(999); }
  //
  if( !isZToMuMu() ){ //this checks to see if there is mu+ mu- pair in the events with |M(mu,mu)-Zmass|<5
    if( OutputChoice_ == "MuonNumber" ) return (double)(99);
    if( OutputChoice_ == "ZMass"      ) return (double)(0);
  }
  //
  double Zmass=91.1876;
  //
  TLorentzVector MuNeg;
  TLorentzVector MuPos;
  MuPos.SetPtEtaPhiM( 0, 0, 0, 0 );
  MuNeg.SetPtEtaPhiM( 0, 0, 0, 0 );
  double Abs_diMuDeltaMass=9999;
  double diMuMass=0;
  //
  unsigned int MuonToRemember=99;
  //
  // This code loops over mu+'s, picks the ones that match to an HLT mu with dR<0.1
  // then loops over mu-'s to pick up the one with the best DiMu mass
  // Repeats this over all eligible mu- and mu+ pairs to pick up the BEST DiMu mass pair. This is the PROBE muon.
  // OUTPUT: One PROBE Muon per Event
  // WANT TO MEASURE: probability that a real muon which gets reconstructed as Loose Muon fires the Trigger
  //
  //
  for(unsigned int iMuPR=0; iMuPR<MuonPt->size(); iMuPR++){//loop over all TAG muons
    if(  MuonCharge->at( iMuPR ) == probeCharge  ) continue;
    if(  muPtcorr(iMuPR)<35                    ) continue;// LQ Filter & HLT_IsoMu24_eta2p1_v*  
    if( !muRisoCheck( iMuPR )                    ) continue; 
    //
    if( RecoHLTdeltaRmin_SingleMuTrigger( iMuPR )>0.1 ) continue;
    //
    MuPos.SetPtEtaPhiM( muPtcorr(iMuPR), MuonEta->at(iMuPR), MuonPhi->at(iMuPR), 0 );
    //
    for(unsigned int iMuNR=0; iMuNR<MuonPt->size(); iMuNR++){//loop over all PROBE muons
      if(  MuonCharge->at(iMuNR) != probeCharge   ) continue; 
      if( !muRisoCheck( iMuNR )                   ) continue; 
      MuNeg.SetPtEtaPhiM( muPtcorr(iMuNR), MuonEta->at(iMuNR), MuonPhi->at(iMuNR), 0 ); 
      // -- -
      //
      if( fabs((MuPos+MuNeg).M()-Zmass)<Abs_diMuDeltaMass && MuPos.Pt()>0 && MuNeg.Pt()>0 && MuPos.DeltaR(MuNeg)>0.2 ){
	  Abs_diMuDeltaMass = fabs((MuPos+MuNeg).M()-Zmass);
	  diMuMass= (MuPos+MuNeg).M();
	  MuonToRemember=iMuNR;
	}
      // -- -
    }
  }
  //
  if( OutputChoice_ == "MuonNumber" ){
    if( Abs_diMuDeltaMass<5 ) return (double)(MuonToRemember);
    else return (double)(99);
  }
  if( OutputChoice_ == "ZMass"      ){
    if( MuonToRemember!=99  ) return (double)(diMuMass);
    else return (double)(0);
  }
}
// // //
void analysisClass::printTriggerObjects_MuonFilters( Long64_t jentry ){ //needs cleanup
  //
  //hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoFiltered10  <<< TauPlusX
  //hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f20L3crIsoFiltered10   <<< SingleMu
  //
  bool isThereMultipleTrigObjects_=false;
  for(unsigned int ifilter=0; ifilter<HLTFilterName->size(); ifilter++){
    if(  
       HLTFilterName->at(ifilter) != "hltL3crIsoL1sMu14erORMu16erL1f0L2f14QL3f17QL3crIsoRhoFiltered0p15"   && //IsoMu17 leg of Mu+Tau (v3)
       HLTFilterName->at(ifilter) != "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoFiltered10"            && //IsoMu18 leg of Mu+Tau (v3,v4,v5)
       HLTFilterName->at(ifilter) != "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f20L3crIsoFiltered10"             && //HLT_IsoMu20_eta2p1_v3, v4
       HLTFilterName->at(ifilter) != "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f20L3crIsoRhoFiltered0p15"        && //HLT_IsoMu20_eta2p1_v6
       HLTFilterName->at(ifilter) != "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoFiltered10"            && //HLT_IsoMu24_eta2p1  (v11,v12.v13 ?? )
       HLTFilterName->at(ifilter) != "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15"          //HLT_IsoMu24_eta2p1_v14
       ) continue;
    std::vector<std::vector<float> > * TrigObjectPt_TrigArray;  vector<float> trigObjPtArray;
    TrigObjectPt_TrigArray  = HLTFilterObjPt;    trigObjPtArray  = (*TrigObjectPt_TrigArray)[ifilter];
    if( trigObjPtArray.size() > 1 ) isThereMultipleTrigObjects_=true;
  }
  //
  if( !isThereMultipleTrigObjects_ ) return;
  //
  for(unsigned int ifilter=0; ifilter<HLTFilterName->size(); ifilter++){
    if(  
       HLTFilterName->at(ifilter) != "hltL3crIsoL1sMu14erORMu16erL1f0L2f14QL3f17QL3crIsoRhoFiltered0p15"   && 
       HLTFilterName->at(ifilter) != "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoFiltered10"            && 
       HLTFilterName->at(ifilter) != "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f20L3crIsoFiltered10"             &&
       HLTFilterName->at(ifilter) != "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f20L3crIsoRhoFiltered0p15"        &&
       HLTFilterName->at(ifilter) != "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoFiltered10"            &&
       HLTFilterName->at(ifilter) != "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15"
       ) continue;
    // -----  extract TrigObject 4-vector  ----- 
    std::vector<std::vector<float> > * TrigObjectPt_TrigArray;  vector<float> trigObjPtArray;  float trigObjPt_;
    std::vector<std::vector<float> > * TrigObjectEta_TrigArray; vector<float> trigObjEtaArray; float trigObjEta_;
    std::vector<std::vector<float> > * TrigObjectPhi_TrigArray; vector<float> trigObjPhiArray; float trigObjPhi_;
    TrigObjectPt_TrigArray  = HLTFilterObjPt;    trigObjPtArray  = (*TrigObjectPt_TrigArray)[ifilter];
    TrigObjectEta_TrigArray = HLTFilterObjEta;   trigObjEtaArray = (*TrigObjectEta_TrigArray)[ifilter];
    TrigObjectPhi_TrigArray = HLTFilterObjPhi;   trigObjPhiArray = (*TrigObjectPhi_TrigArray)[ifilter];
    // ----- ----- ----- ----- ----- ----- ----- 
    //
    std::cout<<"                jentry: "<<jentry<<std::endl;
    std::cout<<"SingleMu_passTrigger(): "<< SingleMu_passTrigger()<<std::endl;
    for(unsigned int iobj=0; iobj<trigObjPtArray.size(); iobj++){//loop over all trigObjects of selected Filter
      trigObjPt_ = (trigObjPtArray)[iobj];  trigObjEta_= (trigObjEtaArray)[iobj];  trigObjPhi_= (trigObjPhiArray)[iobj];
      if(HLTFilterName->at(ifilter) == "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f20L3crIsoFiltered10"){
	std::cout<< "Iso Muon -      IsoMu20(v3,4): "<<trigObjPt_<<" "<<trigObjEta_<<" "<<trigObjPhi_<<std::endl;
      }
      if(HLTFilterName->at(ifilter) == "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f20L3crIsoRhoFiltered0p15"){
	std::cout<< "Iso Muon -        IsoMu20(v6): "<<trigObjPt_<<" "<<trigObjEta_<<" "<<trigObjPhi_<<std::endl;
      }
      if(HLTFilterName->at(ifilter) == "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoFiltered10"){
	std::cout<< "Iso Muon - IsoMu24(v11,12,13): "<<trigObjPt_<<" "<<trigObjEta_<<" "<<trigObjPhi_<<std::endl;
      }
      if(HLTFilterName->at(ifilter) == "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15"){
	std::cout<< "Iso Muon -       IsoMu24(v14): "<<trigObjPt_<<" "<<trigObjEta_<<" "<<trigObjPhi_<<std::endl;
      }
      if( HLTFilterName->at(ifilter) == "hltL3crIsoL1sMu14erORMu16erL1f0L2f14QL3f17QL3crIsoRhoFiltered0p15" ){
	std::cout<< "Muon Leg -       IsoMu17Tau20: "<<trigObjPt_<<" "<<trigObjEta_<<" "<<trigObjPhi_<<std::endl;
      }
      if( HLTFilterName->at(ifilter) == "hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f18QL3crIsoFiltered10"          ){
	std::cout<< "Muon Leg -       IsoMu18Tau20: "<<trigObjPt_<<" "<<trigObjEta_<<" "<<trigObjPhi_<<std::endl;
      }
    }
  }
  std::cout<<std::endl;
}
// // //
