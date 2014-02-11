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
//#include "LAC/ADDON4_LAC.C"///  LQgen functions //fake
#include "LAC/ADDON5_LAC.C" //gen-reco matching 
//#include "LAC/ADDON5_LQ_LAC.C" //gen-reco matching for LQ
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
   TH2D* SystematicsTT2Dhisto       = new TH2D("SystematicsTT2Dhisto",       "SystematicsTT2Dhisto",       10,0.5,10.5, 195,-0.5,194.5 );
   TH2D* SystematicsTTW12Dhisto     = new TH2D("SystematicsTTW12Dhisto",     "SystematicsTTW12Dhisto",     10,0.5,10.5, 195,-0.5,194.5 );
   TH2D* SystematicsTTPP2Dhisto     = new TH2D("SystematicsTTPP2Dhisto",     "SystematicsTTPP2Dhisto",     10,0.5,10.5, 195,-0.5,194.5 );
   TH2D* SystematicsTTPPW12Dhisto   = new TH2D("SystematicsTTPPW12Dhisto",   "SystematicsTTPPW12Dhisto",   10,0.5,10.5, 195,-0.5,194.5 );
   TH2D* SystematicsTTPPQF2Dhisto   = new TH2D("SystematicsTTPPQF2Dhisto",   "SystematicsTTPPQF2Dhisto",   10,0.5,10.5, 195,-0.5,194.5 );
   TH2D* SystematicsTTPPQFW12Dhisto = new TH2D("SystematicsTTPPQFW12Dhisto", "SystematicsTTPPQFW12Dhisto", 10,0.5,10.5, 195,-0.5,194.5 );
   TH2D* SystematicsTTPPQC2Dhisto   = new TH2D("SystematicsTTPPQC2Dhisto",   "SystematicsTTPPQC2Dhisto",   10,0.5,10.5, 195,-0.5,194.5 );
   TH2D* SystematicsTTPPQCW12Dhisto = new TH2D("SystematicsTTPPQCW12Dhisto", "SystematicsTTPPQCW12Dhisto", 10,0.5,10.5, 195,-0.5,194.5 );
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


     unsigned int Ncteq  = PDFCTEQWeights->size();
     unsigned int Nmstw  = PDFMSTWWeights->size()+Ncteq;
     unsigned int Nnnpdf = PDFNNPDFWeights->size()+Nmstw;
     //cout<<PDFCTEQWeights->size()<<" "<<PDFMSTWWeights->size()<<" "<<PDFNNPDFWeights->size()<<endl;
     if( Nnnpdf!=195 ) cout<<"Total: "<<Nnnpdf<<endl;
     for(unsigned int iPdfSys=0; iPdfSys<Nnnpdf; iPdfSys++){ 
       ApplyLepJetMetSystematics(0);//default setting
        
       ltemMuMu.clear();
       ltemMuTau.clear();
       ltemMuTauOS.clear();
       // -- RUN LTEM 
       which_MuTau(ltemMuTau);

       // WEIGHTS
       // ---- trigger 
       unsigned int triggerMuon=99;
       if( ltemMuTau.size()==2 ) triggerMuon=ltemMuTau[0];
       //
       /**/
       if( ltemMuTau.size()!=2 ){
	 which_MuTauOS(ltemMuTauOS);
	 if( ltemMuTauOS.size()==2 ){
	   ltemMuTau.clear();
	   ltemMuTau.push_back(ltemMuTauOS[0]);
	   ltemMuTau.push_back(ltemMuTauOS[1]);
	   triggerMuon=ltemMuTau[0];
	 }
       }
       /**/
       //
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
       // apply pdf weights..
       double wpdf=1;
       if( !isData  ){
	 double CTEQsf       = 1;//0.6079; // 90%CL -> 68%CL (1.0/1.645)  see: http://arxiv.org/pdf/1101.0538v1.pdf
	 //
	 if( iPdfSys>=0     && iPdfSys<Ncteq  ) wpdf = PDFCTEQWeights->at(iPdfSys)/PDFCTEQWeights->at(0);
	 if( iPdfSys>=Ncteq && iPdfSys<Nmstw  ) wpdf = PDFMSTWWeights->at(iPdfSys-Ncteq)/PDFCTEQWeights->at(0);
	 if( iPdfSys>=Nmstw && iPdfSys<Nnnpdf ) wpdf = PDFNNPDFWeights->at(iPdfSys-Nmstw)/PDFCTEQWeights->at(0);
	 //
	 w=w*wpdf;
	 //
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
       bool isRecoTauPrompt_     = isRecoTauPrompt(ltemMuTau[1]);
       bool isRecoMuPrompt_      = isRecoMuPrompt(ltemMuTau[0]);
       bool isRecoTauChargeFlip_ = isRecoTauChargeFlip(ltemMuTau[1]);
       //
       unsigned int iLQ=0;

       if( ST_>410 && LTEMTauPt_>35 && AveAbsEtaV3_<0.9 ){ //LQM200, 250
	 iLQ++;
	 SystematicsTT2Dhisto->Fill( iLQ, iPdfSys, w );
	 SystematicsTTW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	 if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	   SystematicsTTPP2Dhisto->Fill( iLQ, iPdfSys, w );
	   SystematicsTTPPW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   if( isRecoTauChargeFlip_ ){ // if tau charge flip
	     SystematicsTTPPQF2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQFW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	   if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	     SystematicsTTPPQC2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQCW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	 }
       }
       //
       if( ST_>470 && LTEMTauPt_>50 && AveAbsEtaV3_<0.9 ){ //LQM300
	 iLQ++;
	 SystematicsTT2Dhisto->Fill( iLQ, iPdfSys, w );
	 SystematicsTTW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	 if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	   SystematicsTTPP2Dhisto->Fill( iLQ, iPdfSys, w );
	   SystematicsTTPPW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   if( isRecoTauChargeFlip_ ){ // if tau charge flip
	     SystematicsTTPPQF2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQFW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	   if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	     SystematicsTTPPQC2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQCW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	 }
       }
       //
       if( ST_>490 && LTEMTauPt_>50 && AveAbsEtaV3_<0.9 ){ //LQM350
	 iLQ++;
	 SystematicsTT2Dhisto->Fill( iLQ, iPdfSys, w );
	 SystematicsTTW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	 if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	   SystematicsTTPP2Dhisto->Fill( iLQ, iPdfSys, w );
	   SystematicsTTPPW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   if( isRecoTauChargeFlip_ ){ // if tau charge flip
	     SystematicsTTPPQF2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQFW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	   if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	     SystematicsTTPPQC2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQCW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	 }
       }
       //
       if( ST_>680 && LTEMTauPt_>65 && AveAbsEtaV3_<0.9 ){ //LQM400
	 iLQ++;
	 SystematicsTT2Dhisto->Fill( iLQ, iPdfSys, w );
	 SystematicsTTW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	 if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	   SystematicsTTPP2Dhisto->Fill( iLQ, iPdfSys, w );
	   SystematicsTTPPW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   if( isRecoTauChargeFlip_ ){ // if tau charge flip
	     SystematicsTTPPQF2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQFW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	   if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	     SystematicsTTPPQC2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQCW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	 }
       }
       //
       if( ST_>700 && LTEMTauPt_>70 && AveAbsEtaV3_<0.9 ){ //LQM450
	 iLQ++;
	 SystematicsTT2Dhisto->Fill( iLQ, iPdfSys, w );
	 SystematicsTTW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	 if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	   SystematicsTTPP2Dhisto->Fill( iLQ, iPdfSys, w );
	   SystematicsTTPPW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   if( isRecoTauChargeFlip_ ){ // if tau charge flip
	     SystematicsTTPPQF2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQFW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	   if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	     SystematicsTTPPQC2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQCW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	 }
       }
       //
       if( ST_>770 && LTEMTauPt_>70 && AveAbsEtaV3_<0.9 ){ //LQM500
	 iLQ++;
	 SystematicsTT2Dhisto->Fill( iLQ, iPdfSys, w );
	 SystematicsTTW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	 if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	   SystematicsTTPP2Dhisto->Fill( iLQ, iPdfSys, w );
	   SystematicsTTPPW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   if( isRecoTauChargeFlip_ ){ // if tau charge flip
	     SystematicsTTPPQF2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQFW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	   if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	     SystematicsTTPPQC2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQCW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	 }
       }
       //
       if( ST_>800 && LTEMTauPt_>65 && AveAbsEtaV3_<0.9 ){ //LQM550
	 iLQ++;
	 SystematicsTT2Dhisto->Fill( iLQ, iPdfSys, w );
	 SystematicsTTW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	 if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	   SystematicsTTPP2Dhisto->Fill( iLQ, iPdfSys, w );
	   SystematicsTTPPW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   if( isRecoTauChargeFlip_ ){ // if tau charge flip
	     SystematicsTTPPQF2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQFW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	   if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	     SystematicsTTPPQC2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQCW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	 }
       }
       //
       if( ST_>850 && LTEMTauPt_>65 && AveAbsEtaV3_<0.9 ){ //LQM600, 650
	 iLQ++;
	 SystematicsTT2Dhisto->Fill( iLQ, iPdfSys, w );
	 SystematicsTTW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	 if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	   SystematicsTTPP2Dhisto->Fill( iLQ, iPdfSys, w );
	   SystematicsTTPPW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   if( isRecoTauChargeFlip_ ){ // if tau charge flip
	     SystematicsTTPPQF2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQFW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	   if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	     SystematicsTTPPQC2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQCW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	 }
       }
       //
       if( ST_>850 && LTEMTauPt_>85 && AveAbsEtaV3_<0.9 ){ //LQM700, 750
	 iLQ++;
	 SystematicsTT2Dhisto->Fill( iLQ, iPdfSys, w );
	 SystematicsTTW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	 if( isRecoMuPrompt_ && isRecoTauPrompt_ ){ //require PP
	   SystematicsTTPP2Dhisto->Fill( iLQ, iPdfSys, w );
	   SystematicsTTPPW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   if( isRecoTauChargeFlip_ ){ // if tau charge flip
	     SystematicsTTPPQF2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQFW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	   if( !isRecoTauChargeFlip_ ){//if genuine same-sign
	     SystematicsTTPPQC2Dhisto->Fill( iLQ, iPdfSys, w );
	     SystematicsTTPPQCW12Dhisto->Fill( iLQ, iPdfSys, 1 );
	   }
	 }
       }




     }//loop over systematics
     //---------------------------------------------------------------------------------

     ////////////////////// User's code ends here ///////////////////////
   } // End loop over events

   //////////write histos 
   SystematicsTT2Dhisto->Write();
   SystematicsTTW12Dhisto->Write();
   SystematicsTTPP2Dhisto->Write();
   SystematicsTTPPW12Dhisto->Write();
   SystematicsTTPPQF2Dhisto->Write();
   SystematicsTTPPQFW12Dhisto->Write();
   SystematicsTTPPQC2Dhisto->Write();
   SystematicsTTPPQCW12Dhisto->Write();
   //
   JEC1sigmaUP_METcorrOverMEThisto->Write();
   JEC1sigmaUP_jetPtcorrOverjetPthisto->Write();
   JEC1sigmaDOWN_METcorrOverMEThisto->Write();
   JEC1sigmaDOWN_jetPtcorrOverjetPthisto->Write();

   //////////
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
// // //
