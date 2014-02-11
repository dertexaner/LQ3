#define analysisClass_cxx
#include "analysisClass.h"
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TVector3.h>
//
//LAC
//#include "LAC/ADDON1_LAC.C" //this is included in LAC/ADDON2_LAC.C
#include "LAC/ADDON2_LAC.C"
#include "LAC/ADDON3_LAC.C"
//#include "LAC/ADDON5_LAC.C" //gen-reco matching 
#include "LAC/ADDON5_LQ_LAC.C" //gen-reco matching for LQ
//#include "LAC/ADDON5_H_LAC.C" //gen-reco matching for H
#include "LAC/ADDON6_LAC.C"
// // //
analysisClass::analysisClass(string * inputList, string * cutFile, string * treeName, string * outputFileName, string * cutEfficFile)
  :baseClass(inputList, cutFile, treeName, outputFileName, cutEfficFile){
  std::cout << "analysisClass::analysisClass(): begins " << std::endl;
  std::cout << "analysisClass::analysisClass(): ends " << std::endl;
}
analysisClass::~analysisClass(){
  std::cout << "analysisClass::~analysisClass(): begins " << std::endl;
  std::cout << "analysisClass::~analysisClass(): ends " << std::endl;
}
// // //
void analysisClass::Loop()
{
   std::cout << "analysisClass::Loop() begins" <<std::endl;   
    
   if (fChain == 0) return;

   ////////// Enable Sumw2
   TH1::SetDefaultSumw2();
   TH2::SetDefaultSumw2();
   TH3::SetDefaultSumw2();
   
   ////////// Set analysis Object ID variables here
   ObjDef  = "LI";
   BTagDef = "CSVM";
   idDef   = "CUT";
   MuonPtCut = 25;
   MuonEtaCut= 2.1;
   TauPtCut  = 20;
   ApplyAbsMuonIsolation=false;
   removeOverlaps=true;
   //
   // Jet Systematics
   ResetLepJetMetSystematics(); //Reset all to FALSE

   //
   LumiA=891; 
   LumiB=4428;
   LumiC=7018;
   LumiD=7248;
   //
   ResetAllSFs();
   //--// InitTauFunctionParameters(); //This is hard-coded!  (no such function)
   InitMuonFunctionParameters();
   //

   //JER_1sigmaUP   = false; //1 --> ST
   //JER_1sigmaDOWN = false; //2 --> ST
   //JEC_1sigmaUP   = false; //3 --> ST
   //JEC_1sigmaDOWN = false; //4 --> ST
   //Tau_1sigmaUP   = false; //5 --> event selection, ST
   //Tau_1sigmaDOWN = false; //6 --> event selection, ST
   //El_1sigmaUP    = false; //7 --> ST
   //El_1sigmaDOWN  = false; //8 --> ST
   //Mu_1sigmaUP    = false; //9 --> event selection, ST (small)
   //Mu_1sigmaDOWN  = false; //10--> event selection, ST (small)

   //////////book histos here
   TH1D* BarrelSignalRegionTThisto       = new TH1D("BarrelSignalRegionTThisto",      "BarrelSignalRegionTThisto",      10,0.5,10.5 );
   TH1D* BarrelSignalRegionTTW1histo     = new TH1D("BarrelSignalRegionTTW1histo",    "BarrelSignalRegionTTW1histo",    10,0.5,10.5 );
   TH1D* BarrelSignalRegionTTPPhisto     = new TH1D("BarrelSignalRegionTTPPhisto",    "BarrelSignalRegionTTPPhisto",    10,0.5,10.5 );
   TH1D* BarrelSignalRegionTTPPW1histo   = new TH1D("BarrelSignalRegionTTPPW1histo",  "BarrelSignalRegionTTPPW1histo",  10,0.5,10.5 );
   TH1D* BarrelSignalRegionTTPPQFhisto   = new TH1D("BarrelSignalRegionTTPPQFhisto",  "BarrelSignalRegionTTPPQFhisto",  10,0.5,10.5 );
   TH1D* BarrelSignalRegionTTPPQFW1histo = new TH1D("BarrelSignalRegionTTPPQFW1histo","BarrelSignalRegionTTPPQFW1histo",10,0.5,10.5 );
   TH1D* BarrelSignalRegionTTPPQChisto   = new TH1D("BarrelSignalRegionTTPPQChisto",  "BarrelSignalRegionTTPPQChisto",  10,0.5,10.5 );
   TH1D* BarrelSignalRegionTTPPQCW1histo = new TH1D("BarrelSignalRegionTTPPQCW1histo","BarrelSignalRegionTTPPQCW1histo",10,0.5,10.5 );
   //
   TH1D* EndcapSignalRegionTThisto       = new TH1D("EndcapSignalRegionTThisto",      "EndcapSignalRegionTThisto",      10,0.5,10.5 );
   TH1D* EndcapSignalRegionTTW1histo     = new TH1D("EndcapSignalRegionTTW1histo",    "EndcapSignalRegionTTW1histo",    10,0.5,10.5 );
   TH1D* EndcapSignalRegionTTPPhisto     = new TH1D("EndcapSignalRegionTTPPhisto",    "EndcapSignalRegionTTPPhisto",    10,0.5,10.5 );
   TH1D* EndcapSignalRegionTTPPW1histo   = new TH1D("EndcapSignalRegionTTPPW1histo",  "EndcapSignalRegionTTPPW1histo",  10,0.5,10.5 );
   TH1D* EndcapSignalRegionTTPPQFhisto   = new TH1D("EndcapSignalRegionTTPPQFhisto",  "EndcapSignalRegionTTPPQFhisto",  10,0.5,10.5 );
   TH1D* EndcapSignalRegionTTPPQFW1histo = new TH1D("EndcapSignalRegionTTPPQFW1histo","EndcapSignalRegionTTPPQFW1histo",10,0.5,10.5 );
   TH1D* EndcapSignalRegionTTPPQChisto   = new TH1D("EndcapSignalRegionTTPPQChisto",  "EndcapSignalRegionTTPPQChisto",  10,0.5,10.5 );
   TH1D* EndcapSignalRegionTTPPQCW1histo = new TH1D("EndcapSignalRegionTTPPQCW1histo","EndcapSignalRegionTTPPQCW1histo",10,0.5,10.5 );
   //
   TH1D* JEC1sigmaUP_METcorrOverMEThisto       = new TH1D("JEC1sigmaUP_METcorrOverMEThisto",      "JEC1sigmaUP_METcorrOverMEThisto"      ,1000,0,10);
   TH1D* JEC1sigmaUP_jetPtcorrOverjetPthisto   = new TH1D("JEC1sigmaUP_jetPtcorrOverjetPthisto",  "JEC1sigmaUP_jetPtcorrOverjetPthisto"  ,1000,0,10);
   TH1D* JEC1sigmaDOWN_METcorrOverMEThisto     = new TH1D("JEC1sigmaDOWN_METcorrOverMEThisto",    "JEC1sigmaDOWN_METcorrOverMEThisto"    ,1000,0,10);
   TH1D* JEC1sigmaDOWN_jetPtcorrOverjetPthisto = new TH1D("JEC1sigmaDOWN_jetPtcorrOverjetPthisto","JEC1sigmaDOWN_jetPtcorrOverjetPthisto",1000,0,10);


   /////////initialize variables

   // Get the provided PileUp Weights for MC.
   if( !isData ){
     std::cout<<"-------- Verifying Pileup Weights --------"<<std::endl;
     for(int pu=0; pu<60; pu++){//[0-60)
       std::cout<<"Pileup Weight("<<pu<<"): "<<getPileupWeight(pu,false)<<std::endl;
     }
     std::cout<<"-------- ------------------------ --------"<<std::endl;
   }

   Long64_t nentries = fChain->GetEntries();
   std::cout << "analysisClass::Loop(): nentries = " << nentries << std::endl;   

   ////// The following ~7 lines have been taken from rootNtupleClass->Loop() /////
   ////// If the root version is updated and rootNtupleClass regenerated,     /////
   ////// these lines may need to be updated.                                 /////    
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     if(jentry < 10 || jentry%1000 == 0) std::cout << "analysisClass::Loop(): jentry = " << jentry << std::endl;   
     // if (Cut(ientry) < 0) continue;
     //
     ////////////////////// User's code starts here ///////////////////////
     ///Stuff to be done every event

     // Set the evaluation of the cuts to false and clear the variable values and filled status
     resetCuts();
     //

     // WEIGHTS
     // ---- pileup
     double safePileupWeights_=0;
     safePileupWeights_=safePileupWeights();

     //  -- JSON SKIM -- unaffected by energy systematics
     int passJSON_=0;
     if( isData  ) passJSON_ = passJSON(run,ls,isData);
     if( !isData ) passJSON_ = 1;
     fillVariableWithValue("PassJSON", passJSON_ );//..............// returns 0, 1                             

     //  -- EVENT FILTER SKIM -- unaffected by energy systematics
     int passEventFilter_=0;
     if( passEventFilter() ) passEventFilter_ = 1;
     fillVariableWithValue("PassEventFilter", passEventFilter_ );//.// returns 0, 1 


     // Disabled..
     fillVariableWithValue("PassTrig",             1 );
     fillVariableWithValue("PassOfflineDilepton",  1 );
     fillVariableWithValue("PassAllMuHLTmatching", 1 );
     fillVariableWithValue("PassOfflineCuts",      1 );
     evaluateCuts();
     //---------------------------------------------------------------------------------
     if( !passedCut("PassJSON") || !passedCut("PassTrig") || !passedCut("PassEventFilter") || !passedCut("PassOfflineDilepton") ||
	 !passedCut("PassAllMuHLTmatching") || !passedCut("PassOfflineCuts") ) continue;

     ApplyLepJetMetSystematics(0);
     
     ltemMuTau.clear();
     // -- RUN LTEM 
     which_MuTau(ltemMuTau);
     
     // WEIGHTS
     // ---- trigger 
     unsigned int triggerMuon=99;
     if( ltemMuTau.size()==2 ) triggerMuon=ltemMuTau[0];
     if( ltemMuTau.size()!=2 ) continue;
     //
     double TriggerEfficiencyWeights_;
     if( muPtcorr(triggerMuon)> 45                                ){
       TriggerEfficiencyWeights_ = Mu40e2p1_ScaleFactor( muPtcorr(triggerMuon), MuonEta->at(triggerMuon) ); }
     if( muPtcorr(triggerMuon)<=45 &&  muRTightCheck(triggerMuon) ){
       TriggerEfficiencyWeights_ = IsoMu24e2p1_Eff(      muPtcorr(triggerMuon), MuonEta->at(triggerMuon) ); }
     if( muPtcorr(triggerMuon)<=45 && !muRTightCheck(triggerMuon) ){
       TriggerEfficiencyWeights_ = Mu24e2p1_ScaleFactor( muPtcorr(triggerMuon), MuonEta->at(triggerMuon) ); }
     //
     // ---- total = pileup x trigger
     double w = 0;
     if( !isData  ){
       w=safePileupWeights_*TriggerEfficiencyWeights_; 
     }
     if( isData  ) w=1;
     if( isData && muPtcorr(triggerMuon)<=45 && !muRTightCheck(triggerMuon) ){
       double trigPrescale_=167.2;
       w=(double)(w*trigPrescale_);
     }
     
     //  -- TRIGGER SKIM -- APPLIED HERE NOW
     int passTrigger_=0;
     if( muPtcorr(triggerMuon)> 45                                ) passTrigger_ = SingleMu40_passTrigger();//<<<<<<<<< this is Mu40 trigger
     if( muPtcorr(triggerMuon)<=45 &&  muRTightCheck(triggerMuon) ) passTrigger_ = SingleMu_passTrigger();  //<<<<<<<<< this is IsoMu24 trigger
     if( muPtcorr(triggerMuon)<=45 && !muRTightCheck(triggerMuon) ) passTrigger_ = HLT_MuPT_eta2p1_passTrigger(); //<<<<<<<<< this is Mu24 trigger
     
     //  -- DILEPTON SKIM - MuTau -- APPLIED HERE NOW
     int isOfflineDilepton_=0;
     if( isMuTauDR0p30() ) isOfflineDilepton_ = 1;
     
     // -- HLT MATCHING SKIM -- APPLIED HERE NOW
     int passAllMuHLTmatching_=0;
     if(muPtcorr(triggerMuon)> 45                                && RecoHLTdeltaRmin_SingleMu40Trigger(triggerMuon)<0.15) passAllMuHLTmatching_=1;
     if(muPtcorr(triggerMuon)<=45 &&  muRTightCheck(triggerMuon) && RecoHLTdeltaRmin_SingleMuTrigger(triggerMuon)<0.15  ) passAllMuHLTmatching_=1;
     if(muPtcorr(triggerMuon)<=45 && !muRTightCheck(triggerMuon) && RecoHLTdeltaRmin_SingleMu24Trigger(triggerMuon)<0.15) passAllMuHLTmatching_=1;
     
     // -- OFFLINE SKIM -- APPLIED HERE NOW
     int OfflineCuts_=0;
     if( ltemMuTau.size()==2 && //muPtcorr(ltemMuTau[0])>45 &&
	 //ST()>400  && JetCounter()>=2 &&
	 JetCounter()>=2 &&
	 !isZToMuMu() &&
	 tauPtcorr(ltemMuTau[1])>35 
	 // 
	 // Optimized Cuts -- SEE BELOW
	 //&& ST()>410 && tauPtcorr(ltemMuTau[1])>35 && GetAveAbsEtaV3()<0.9 //LQM200, 250   1
	 //&& ST()>470 && tauPtcorr(ltemMuTau[1])>50 && GetAveAbsEtaV3()<0.9 //LQM300        2
	 //&& ST()>490 && tauPtcorr(ltemMuTau[1])>50 && GetAveAbsEtaV3()<0.9 //LQM350        3
	 //&& ST()>680 && tauPtcorr(ltemMuTau[1])>65 && GetAveAbsEtaV3()<0.9 //LQM400        4
	 //&& ST()>700 && tauPtcorr(ltemMuTau[1])>70 && GetAveAbsEtaV3()<0.9 //LQM450        5
	 //&& ST()>770 && tauPtcorr(ltemMuTau[1])>70 && GetAveAbsEtaV3()<0.9 //LQM500        6
	 //&& ST()>800 && tauPtcorr(ltemMuTau[1])>65 && GetAveAbsEtaV3()<0.9 //LQM550        7
	 //&& ST()>850 && tauPtcorr(ltemMuTau[1])>65 && GetAveAbsEtaV3()<0.9 //LQM600, 650   8
	 //&& ST()>850 && tauPtcorr(ltemMuTau[1])>85 && GetAveAbsEtaV3()<0.9 //LQM700, 750   9
	 //
	 ) OfflineCuts_=1;
     //
     
     // standard cuts - now affected by energy systematics
     if( passTrigger_==0          ) continue;  
     if( isOfflineDilepton_==0    ) continue;  
     if( passAllMuHLTmatching_==0 ) continue;  
     if( OfflineCuts_==0          ) continue;  
     if( !muRTightCheck(ltemMuTau[0]) || !tauRTightCheck(ltemMuTau[1]) ) continue; //require TT  (signal box)
     
     double ST_          = ST();
     double LTEMTauPt_   = tauPtcorr(ltemMuTau[1]);
     double AveAbsEtaV3_ = GetAveAbsEtaV3();
     //
     bool isRecoMuPrompt_      = isRecoMuPrompt(ltemMuTau[0]);
     bool isRecoTauPrompt_     = isRecoTauPrompt(ltemMuTau[1]);
     bool isRecoTauChargeFlip_ = isRecoTauChargeFlip(ltemMuTau[1]);
     //
     unsigned int iLQ=0;
     
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>410 && LTEMTauPt_>35 && AveAbsEtaV3_<0.9 ){ //LQM200, 250
       BarrelSignalRegionTThisto->Fill( iLQ, w );         BarrelSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 BarrelSignalRegionTTPPhisto->Fill( iLQ, w );	  BarrelSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   BarrelSignalRegionTTPPQFhisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   BarrelSignalRegionTTPPQChisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++; 
     if( ST_>470 && LTEMTauPt_>50  && AveAbsEtaV3_<0.9 ){ //LQM300
       BarrelSignalRegionTThisto->Fill( iLQ, w );         BarrelSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 BarrelSignalRegionTTPPhisto->Fill( iLQ, w );	  BarrelSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   BarrelSignalRegionTTPPQFhisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   BarrelSignalRegionTTPPQChisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>490 && LTEMTauPt_>50  && AveAbsEtaV3_<0.9 ){ //LQM350
       BarrelSignalRegionTThisto->Fill( iLQ, w );         BarrelSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 BarrelSignalRegionTTPPhisto->Fill( iLQ, w );	  BarrelSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   BarrelSignalRegionTTPPQFhisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   BarrelSignalRegionTTPPQChisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>680 && LTEMTauPt_>65  && AveAbsEtaV3_<0.9 ){ //LQM400
       BarrelSignalRegionTThisto->Fill( iLQ, w );         BarrelSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 BarrelSignalRegionTTPPhisto->Fill( iLQ, w );     BarrelSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   BarrelSignalRegionTTPPQFhisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   BarrelSignalRegionTTPPQChisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>700 && LTEMTauPt_>65  && AveAbsEtaV3_<0.9 ){ //LQM450 //<< pt 70 -> 65
       BarrelSignalRegionTThisto->Fill( iLQ, w );         BarrelSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 BarrelSignalRegionTTPPhisto->Fill( iLQ, w );     BarrelSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   BarrelSignalRegionTTPPQFhisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   BarrelSignalRegionTTPPQChisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>770 && LTEMTauPt_>65  && AveAbsEtaV3_<0.9 ){ //LQM500 //<< pt 70 -> 65
       BarrelSignalRegionTThisto->Fill( iLQ, w );         BarrelSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 BarrelSignalRegionTTPPhisto->Fill( iLQ, w );	  BarrelSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   BarrelSignalRegionTTPPQFhisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   BarrelSignalRegionTTPPQChisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>800 && LTEMTauPt_>65  && AveAbsEtaV3_<0.9 ){ //LQM550
       BarrelSignalRegionTThisto->Fill( iLQ, w );         BarrelSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 BarrelSignalRegionTTPPhisto->Fill( iLQ, w );	  BarrelSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   BarrelSignalRegionTTPPQFhisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   BarrelSignalRegionTTPPQChisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>850 && LTEMTauPt_>65  && AveAbsEtaV3_<0.9 ){ //LQM600, 650
       BarrelSignalRegionTThisto->Fill( iLQ, w );         BarrelSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 BarrelSignalRegionTTPPhisto->Fill( iLQ, w );     BarrelSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   BarrelSignalRegionTTPPQFhisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   BarrelSignalRegionTTPPQChisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>850 && LTEMTauPt_>85  && AveAbsEtaV3_<0.9 ){ //LQM700, 750
       BarrelSignalRegionTThisto->Fill( iLQ, w );         BarrelSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 BarrelSignalRegionTTPPhisto->Fill( iLQ, w );	  BarrelSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   BarrelSignalRegionTTPPQFhisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   BarrelSignalRegionTTPPQChisto->Fill( iLQ, w ); BarrelSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------

     //----------------------------------------------------------------------------------------------------------------------------
     iLQ=0;

     //--------------------------------------------------------------
     iLQ++;
     if( ST_>410 && LTEMTauPt_>35 && AveAbsEtaV3_>=0.9 ){ //LQM200, 250
       EndcapSignalRegionTThisto->Fill( iLQ, w );         EndcapSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 EndcapSignalRegionTTPPhisto->Fill( iLQ, w );	  EndcapSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   EndcapSignalRegionTTPPQFhisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   EndcapSignalRegionTTPPQChisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>470 && LTEMTauPt_>50 && AveAbsEtaV3_>=0.9 ){ //LQM300
       EndcapSignalRegionTThisto->Fill( iLQ, w );         EndcapSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 EndcapSignalRegionTTPPhisto->Fill( iLQ, w );	  EndcapSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   EndcapSignalRegionTTPPQFhisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   EndcapSignalRegionTTPPQChisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>490 && LTEMTauPt_>50 && AveAbsEtaV3_>=0.9 ){ //LQM350
       EndcapSignalRegionTThisto->Fill( iLQ, w );         EndcapSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 EndcapSignalRegionTTPPhisto->Fill( iLQ, w );	  EndcapSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   EndcapSignalRegionTTPPQFhisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   EndcapSignalRegionTTPPQChisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>680 && LTEMTauPt_>65 && AveAbsEtaV3_>=0.9 ){ //LQM400
       EndcapSignalRegionTThisto->Fill( iLQ, w );         EndcapSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 EndcapSignalRegionTTPPhisto->Fill( iLQ, w );     EndcapSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   EndcapSignalRegionTTPPQFhisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   EndcapSignalRegionTTPPQChisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>700 && LTEMTauPt_>65 && AveAbsEtaV3_>=0.9 ){ //LQM450 //<< pt 70 -> 65
       EndcapSignalRegionTThisto->Fill( iLQ, w );         EndcapSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 EndcapSignalRegionTTPPhisto->Fill( iLQ, w );     EndcapSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   EndcapSignalRegionTTPPQFhisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   EndcapSignalRegionTTPPQChisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>770 && LTEMTauPt_>65 && AveAbsEtaV3_>=0.9 ){ //LQM500 //<< pt 70 -> 65
       EndcapSignalRegionTThisto->Fill( iLQ, w );         EndcapSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 EndcapSignalRegionTTPPhisto->Fill( iLQ, w );	  EndcapSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   EndcapSignalRegionTTPPQFhisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   EndcapSignalRegionTTPPQChisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>800 && LTEMTauPt_>65 && AveAbsEtaV3_>=0.9 ){ //LQM550
       EndcapSignalRegionTThisto->Fill( iLQ, w );         EndcapSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 EndcapSignalRegionTTPPhisto->Fill( iLQ, w );	  EndcapSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   EndcapSignalRegionTTPPQFhisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   EndcapSignalRegionTTPPQChisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>850 && LTEMTauPt_>65 && AveAbsEtaV3_>=0.9 ){ //LQM600, 650
       EndcapSignalRegionTThisto->Fill( iLQ, w );         EndcapSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 EndcapSignalRegionTTPPhisto->Fill( iLQ, w );	  EndcapSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   EndcapSignalRegionTTPPQFhisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   EndcapSignalRegionTTPPQChisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------
     iLQ++;
     if( ST_>850 && LTEMTauPt_>85 && AveAbsEtaV3_>=0.9 ){ //LQM700, 750
       EndcapSignalRegionTThisto->Fill( iLQ, w );         EndcapSignalRegionTTW1histo->Fill( iLQ, 1 );
       if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	 EndcapSignalRegionTTPPhisto->Fill( iLQ, w );	  EndcapSignalRegionTTPPW1histo->Fill( iLQ, 1 );
	 if( isRecoTauChargeFlip_ ){ // if tau charge flip
	   EndcapSignalRegionTTPPQFhisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQFW1histo->Fill( iLQ, 1 );
	 }
	 if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	   EndcapSignalRegionTTPPQChisto->Fill( iLQ, w ); EndcapSignalRegionTTPPQCW1histo->Fill( iLQ, 1 );
	 }
       }
     }
     //--------------------------------------------------------------


     ////////////////////// User's code ends here ///////////////////////
   } // End loop over events
   
   //////////write histos 
   BarrelSignalRegionTThisto->Write();
   BarrelSignalRegionTTW1histo->Write();
   BarrelSignalRegionTTPPhisto->Write();
   BarrelSignalRegionTTPPW1histo->Write();
   BarrelSignalRegionTTPPQFhisto->Write();
   BarrelSignalRegionTTPPQFW1histo->Write();
   BarrelSignalRegionTTPPQChisto->Write();
   BarrelSignalRegionTTPPQCW1histo->Write();
   //
   EndcapSignalRegionTThisto->Write();
   EndcapSignalRegionTTW1histo->Write();
   EndcapSignalRegionTTPPhisto->Write();
   EndcapSignalRegionTTPPW1histo->Write();
   EndcapSignalRegionTTPPQFhisto->Write();
   EndcapSignalRegionTTPPQFW1histo->Write();
   EndcapSignalRegionTTPPQChisto->Write();
   EndcapSignalRegionTTPPQCW1histo->Write();
   //
   JEC1sigmaUP_METcorrOverMEThisto->Write();
   JEC1sigmaUP_jetPtcorrOverjetPthisto->Write();
   JEC1sigmaDOWN_METcorrOverMEThisto->Write();
   JEC1sigmaDOWN_jetPtcorrOverjetPthisto->Write();

   //////////
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
// // //
