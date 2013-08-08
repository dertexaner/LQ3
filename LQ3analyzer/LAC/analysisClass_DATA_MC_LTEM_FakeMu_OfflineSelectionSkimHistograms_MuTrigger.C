#define analysisClass_cxx
#include "analysisClass.h"
#include <TH1.h>
#include <TH2.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TVector3.h>
#include "TGraphAsymmErrors.h"
//
//LAC
//#include "LAC/ADDON1_LAC.C" //this is included in LAC/ADDON2_LAC.C
#include "LAC/ADDON2_LAC.C"
#include "LAC/ADDON3_LAC.C"
//#include "LAC/ADDON4_LAC.C"///  LQgen functions //fake
//#include "LAC/ADDON5_LAC.C"/// TOPgen functions //fake
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

   ////////// Set analysis Object ID variables here
   ObjDef  = "LI";
   BTagDef = "CSVL";
   idDef   = "CUT";
   MuonPtCut = 20;
   MuonEtaCut= 2.1;
   TauPtCut  = 20;
   ApplyAbsMuonIsolation=false;
   removeOverlaps=false;
   //
   LumiA=891; 
   LumiB=4428;
   LumiC=7018;
   LumiD=7248;
   //
   bool debug_=true;

   //////////book histos here
   TH1D* ProvidedPileUpWeightshisto  = new TH1D("ProvidedPileUpWeightshisto","ProvidedPileUpWeightshisto",10000,0,100);//distribution of the provided PileUp-weights
   TH1D* AppliedPileUpWeightshisto   = new TH1D("AppliedPileUpWeightshisto", "AppliedPileUpWeightshisto", 10000,0,100);//distribution of the applied PileUp-weights
   TH1D* AppliedTrigEffWeightshisto  = new TH1D("AppliedTrigEffWeightshisto","AppliedTrigEffWeightshisto",10000,0,100);//distribution of the applied TrigEff-weights
   TH1D* Triggerhisto                = new TH1D("Triggerhisto",              "Triggerhisto",              10,-3.5,6.5);//distribution of triggers in the events
   TH1D* AppliedTotalWeightshisto    = new TH1D("AppliedTotalWeightshisto",  "AppliedTotalWeightshisto",  10000,0,100);//distribution of the applied Total-weights
   //
   TH1D* LeadMuTauDeltaRhisto = new TH1D("LeadMuTauDeltaRhisto","LeadMuTauDeltaRhisto",5000,0,50);
   TH1D* SThisto              = new TH1D("SThisto", "SThisto", 5000,0,5000);
   TH1D* STwFakeshisto        = new TH1D("STwFakeshisto", "STwFakeshisto", 5000,0,5000);//fake
   TH1D* STwRealshisto        = new TH1D("STwRealshisto", "STwRealshisto", 5000,0,5000);//fake
   TH1D* STotherhisto         = new TH1D("STotherhisto", "STotherhisto", 5000,0,5000);//fake
   TH1D* FakesDistrhisto      = new TH1D("FakesDistrhisto", "FakesDistrhisto",7,0.25,3.75);//fake
   TH1D* SJThisto             = new TH1D("SJThisto","SJThisto",5000,0,5000);
   TH1D* METhisto             = new TH1D("METhisto","METhisto",2000,0,2000);
   //
   TH1D* MaxDiLepMasshisto     = new TH1D("MaxDiLepMasshisto",    "MaxDiLepMasshisto",    2000,0,2000);
   TH1D* AllMuMuInvMasshisto   = new TH1D("AllMuMuInvMasshisto",  "AllMuMuInvMasshisto",  2000,0,2000);
   TH1D* AllMuTauInvMasshisto  = new TH1D("AllMuTauInvMasshisto", "AllMuTauInvMasshisto", 2000,0,2000);
   TH1D* MaxMuTauInvMasshisto  = new TH1D("MaxMuTauInvMasshisto","MaxMuTauInvMasshisto",  2000,0,2000);
   TH1D* AllTauTauInvMasshisto = new TH1D("AllTauTauInvMasshisto","AllTauTauInvMasshisto",2000,0,2000);
   //
   TH1D* METmuTransMasshisto  = new TH1D("METmuTransMasshisto", "METmuTransMasshisto",500,0,1000);
   TH1D* METtauTransMasshisto = new TH1D("METtauTransMasshisto","METtauTransMasshisto",500,0,1000);
   //
   TH1D* METtauDeltaPhihisto = new TH1D("METtauDeltaPhihisto","METtauDeltaPhihisto",800,-4,4);
   TH1D* METmuDeltaPhihisto  = new TH1D("METmuDeltaPhihisto", "METmuDeltaPhihisto", 800,-4,4);
   TH1D* METelDeltaPhihisto  = new TH1D("METelDeltaPhihisto", "METelDeltaPhihisto", 800,-4,4);
   //
   TH1D* LeadMuPthisto    = new TH1D("LeadMuPthisto",   "LeadMuPthisto",   1000,0,1000);
   TH1D* LeadTauPthisto   = new TH1D("LeadTauPthisto",  "LeadTauPthisto",  1000,0,1000);
   TH1D* LeadElPthisto    = new TH1D("LeadElPthisto",   "LeadElPthisto",   1000,0,1000);
   TH1D* LeadJetPthisto   = new TH1D("LeadJetPthisto",  "LeadJetPthisto",  1000,0,1000);
   TH1D* LeadBJetPthisto  = new TH1D("LeadBJetPthisto", "LeadBJetPthisto", 1000,0,1000);
   //
   TH1D* LeadMuEtahisto   = new TH1D("LeadMuEtahisto",  "LeadMuEtahisto",  500,-2.5,2.5);
   TH1D* LeadTauEtahisto  = new TH1D("LeadTauEtahisto", "LeadTauEtahisto", 500,-2.5,2.5);
   TH1D* LeadElEtahisto   = new TH1D("LeadElEtahisto",  "LeadElEtahisto",  500,-2.5,2.5);
   TH1D* LeadJetEtahisto  = new TH1D("LeadJetEtahisto", "LeadJetEtahisto", 700,-3.5,3.5);
   TH1D* LeadBJetEtahisto = new TH1D("LeadBJetEtahisto","LeadBJetEtahisto",700,-3.5,3.5);
   //
   TH1D* LeadMuPhihisto   = new TH1D("LeadMuPhihisto",  "LeadMuPhihisto",  800,-4,4);
   TH1D* LeadTauPhihisto  = new TH1D("LeadTauPhihisto", "LeadTauPhihisto", 800,-4,4);
   TH1D* LeadElPhihisto   = new TH1D("LeadElPhihisto",  "LeadElPhihisto",  800,-4,4);
   TH1D* LeadJetPhihisto  = new TH1D("LeadJetPhihisto", "LeadJetPhihisto", 800,-4,4);
   TH1D* LeadBJetPhihisto = new TH1D("LeadBJetPhihisto","LeadBJetPhihisto",800,-4,4);
   //
   TH1D* MuPthisto    = new TH1D("MuPthisto",   "MuPthisto",   200, 0,1000);
   TH1D* TauPthisto   = new TH1D("TauPthisto",  "TauPthisto",  200, 0,1000);
   TH1D* ElPthisto    = new TH1D("ElPthisto",   "ElPthisto",   200, 0,1000);
   TH1D* JetPthisto   = new TH1D("JetPthisto",  "JetPthisto",  200, 0,1000);
   TH1D* BJetPthisto  = new TH1D("BJetPthisto", "BJetPthisto", 200, 0,1000);
   //
   TH1D* MuEtahisto   = new TH1D("MuEtahisto",  "MuEtahisto",  500,-2.5,2.5);
   TH1D* TauEtahisto  = new TH1D("TauEtahisto", "TauEtahisto", 500,-2.5,2.5);
   TH1D* ElEtahisto   = new TH1D("ElEtahisto",  "ElEtahisto",  500,-2.5,2.5);
   TH1D* JetEtahisto  = new TH1D("JetEtahisto", "JetEtahisto", 700,-3.5,3.5);
   TH1D* BJetEtahisto = new TH1D("BJetEtahisto","BJetEtahisto",700,-3.5,3.5);
   //
   TH1D* MuPhihisto   = new TH1D("MuPhihisto",  "MuPhihisto",  800,-4,4);
   TH1D* TauPhihisto  = new TH1D("TauPhihisto", "TauPhihisto", 800,-4,4);
   TH1D* ElPhihisto   = new TH1D("ElPhihisto",  "ElPhihisto",  800,-4,4);
   TH1D* JetPhihisto  = new TH1D("JetPhihisto", "JetPhihisto", 800,-4,4);
   TH1D* BJetPhihisto = new TH1D("BJetPhihisto","BJetPhihisto",800,-4,4);
   //
   // from here on, it used to be TH1I
   TH1D* MuNhisto      = new TH1D("MuNhisto",    "MuNhisto",    16,-0.5,15.5);
   TH1D* TauNhisto     = new TH1D("TauNhisto",   "TauNhisto",   16,-0.5,15.5);
   TH1D* ElNhisto      = new TH1D("ElNhisto",    "ElNhisto",    16,-0.5,15.5);
   TH1D* JetNhisto     = new TH1D("JetNhisto",   "JetNhisto",   26,-0.5,25.5);
   TH1D* BJetNhisto    = new TH1D("BJetNhisto",  "BJetNhisto",  16,-0.5,15.5);
   TH1D* TotalNhisto   = new TH1D("TotalNhisto", "TotalNhisto", 41,-0.5,40.5);
   TH1D* VertexNhisto  = new TH1D("VertexNhisto","VertexNhisto",61,-0.5,60.5);
   //
   TH1D* RecoSignalTypehisto         = new TH1D("RecoSignalTypehisto",        "RecoSignalTypehisto",        5,-2.5,2.5);
   TH1D* MuBasedRecoSignalTypehisto  = new TH1D("MuBasedRecoSignalTypehisto", "MuBasedRecoSignalTypehisto", 9,-4.5,4.5);
   TH1D* TauBasedRecoSignalTypehisto = new TH1D("TauBasedRecoSignalTypehisto","TauBasedRecoSignalTypehisto",9,-4.5,4.5);
   //
   TH2D* LooseToTightMatrixRAW2Dhisto = new TH2D("LooseToTightMatrixRAW2Dhisto","LooseToTightMatrixRAW2Dhisto", 2,0.5,2.5, 2,0.5,2.5);
   TH2D* LooseToTightMatrix2Dhisto    = new TH2D("LooseToTightMatrix2Dhisto",   "LooseToTightMatrix2Dhisto",    2,0.5,2.5, 2,0.5,2.5);
   //
   TH1D* LooseMuonPthisto  = new TH1D("LooseMuonPthisto", "LooseMuonPthisto", 2000,0,2000);
   TH1D* TightMuonPthisto  = new TH1D("TightMuonPthisto", "TightMuonPthisto", 2000,0,2000);
   TH1D* LooseMuonEtahisto = new TH1D("LooseMuonEtahisto","LooseMuonEtahisto",500,-2.5,2.5);
   TH1D* TightMuonEtahisto = new TH1D("TightMuonEtahisto","TightMuonEtahisto",500,-2.5,2.5);
   TH1D* LooseMuonPhihisto = new TH1D("LooseMuonPhihisto","LooseMuonPhihisto",700,-3.5,3.5);
   TH1D* TightMuonPhihisto = new TH1D("TightMuonPhihisto","TightMuonPhihisto",700,-3.5,3.5);
   TGraphAsymmErrors* LooseTightRatioPtMuonhisto  = new TGraphAsymmErrors(200);
   TGraphAsymmErrors* LooseTightRatioEtaMuonhisto = new TGraphAsymmErrors(50);
   TGraphAsymmErrors* LooseTightRatioPhiMuonhisto = new TGraphAsymmErrors(70);
   //
   TH1D* LooseFakeMuonPthisto  = new TH1D("LooseFakeMuonPthisto", "LooseFakeMuonPthisto", 2000,0,2000);
   TH1D* TightFakeMuonPthisto  = new TH1D("TightFakeMuonPthisto", "TightFakeMuonPthisto", 2000,0,2000);
   TH1D* LooseFakeMuonEtahisto = new TH1D("LooseFakeMuonEtahisto","LooseFakeMuonEtahisto",500,-2.5,2.5);
   TH1D* TightFakeMuonEtahisto = new TH1D("TightFakeMuonEtahisto","TightFakeMuonEtahisto",500,-2.5,2.5);
   TH1D* LooseFakeMuonPhihisto = new TH1D("LooseFakeMuonPhihisto","LooseFakeMuonPhihisto",700,-3.5,3.5);
   TH1D* TightFakeMuonPhihisto = new TH1D("TightFakeMuonPhihisto","TightFakeMuonPhihisto",700,-3.5,3.5);
   //
   TH1D* LoosePromptMuonPthisto  = new TH1D("LoosePromptMuonPthisto", "LoosePromptMuonPthisto", 2000,0,2000);
   TH1D* TightPromptMuonPthisto  = new TH1D("TightPromptMuonPthisto", "TightPromptMuonPthisto", 2000,0,2000);
   TH1D* LoosePromptMuonEtahisto = new TH1D("LoosePromptMuonEtahisto","LoosePromptMuonEtahisto",500,-2.5,2.5);
   TH1D* TightPromptMuonEtahisto = new TH1D("TightPromptMuonEtahisto","TightPromptMuonEtahisto",500,-2.5,2.5);
   TH1D* LoosePromptMuonPhihisto = new TH1D("LoosePromptMuonPhihisto","LoosePromptMuonPhihisto",700,-3.5,3.5);
   TH1D* TightPromptMuonPhihisto = new TH1D("TightPromptMuonPhihisto","TightPromptMuonPhihisto",700,-3.5,3.5);

   /////////initialize variables
   TLorentzVector allmu;
   TLorentzVector alltau;
   TLorentzVector allmu2;
   TLorentzVector alltau2;
   //
   int MuN;
   int TauN;
   int ElN;
   int JetN;
   int BJetN;
   int VertexN;
   double SJT;
   TLorentzVector Mu;
   TLorentzVector Tau;
   TLorentzVector El;
   TLorentzVector Jet;
   TLorentzVector BJet;
   //
   std::vector<unsigned int> triMu;
   std::vector<unsigned int> triMu_Zstatus;

   //
   // Get the provided PileUp Weights for MC.
   if( !isData ){
     std::cout<<"-------- Verifying Pileup Weights --------"<<std::endl;
     for(int pu=0; pu<60; pu++){//[0-60)
       ProvidedPileUpWeightshisto->Fill( pu, getPileupWeight(pu,false) );
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
     //

     // Set the evaluation of the cuts to false and clear the variable values and filled status
     resetCuts();
     //
     ltemMuMu.clear();
     ltemMuTau.clear();
     triMu.clear();
     triMu_Zstatus.clear();
     unsigned int probeMuon=99;
     unsigned int trigMuon=99;
     double mu_zmass=99999;

     // -- WHICH PROBE MUON
     double Zmass=91.1876;
     TLorentzVector Mu1; TLorentzVector Mu2;
     unsigned int zstatus=0;
     Mu1.SetPtEtaPhiM( 0, 0, 0, 0 ); Mu2.SetPtEtaPhiM( 0, 0, 0, 0 );
     //
     for(unsigned int iMu1=0; iMu1<MuonPt->size(); iMu1++){
       if( !muRisoCheck(iMu1) ) continue;
       triMu.push_back( iMu1 );
       zstatus=0;
       mu_zmass=0;
       //-------------------------- Check if from Z
       for(unsigned int iMu2=0; iMu2<MuonPt->size(); iMu2++){
	 if( iMu2==iMu1 ) continue;
	 if(  MuonCharge->at( iMu2 ) == MuonCharge->at( iMu1 ) ) continue;
	 if( !muRisoCheck(iMu2) ) continue;
	 Mu1.SetPtEtaPhiM( MuonPt->at(iMu1), MuonEta->at(iMu1), MuonPhi->at(iMu1), 0 );
	 Mu2.SetPtEtaPhiM( MuonPt->at(iMu2), MuonEta->at(iMu2), MuonPhi->at(iMu2), 0 );
	 //-- --
	 //cout<<iMu1<<" "<<iMu2<<" "<<fabs((Mu1+Mu2).M()-Zmass)<<" "<<fabs(mu_zmass-Zmass)<<endl;
	 if( fabs((Mu1+Mu2).M()-Zmass) < fabs(mu_zmass-Zmass) ){ mu_zmass=fabs((Mu1+Mu2).M()); }
	 //-- --
	 if( fabs((Mu1+Mu2).M()-Zmass)<5 ){ zstatus=1; }
       }
       //--------------------------
       triMu_Zstatus.push_back( zstatus );
       if( debug_ ) cout<<"("<<jentry<<") muRiso pass: "<<MuonPt->at(iMu1)<<" "<<MuonEta->at(iMu1)<<" "<<MuonPhi->at(iMu1)<<"  Z:"<<zstatus<<" "<<fabs(mu_zmass-Zmass)<<endl;
     }

     unsigned int triMu_Zstatus_SUM=0;
     for( unsigned int imu=0; imu<triMu_Zstatus.size(); imu++){
       triMu_Zstatus_SUM+=triMu_Zstatus[imu];
     }
     if( triMu.size()==3 && triMu_Zstatus.size()==3 && triMu_Zstatus_SUM==2 ){
       for( unsigned int imu=0; imu<triMu.size(); imu++){
	 if( triMu_Zstatus[imu]==0 ) probeMuon=triMu[imu];
	 if( triMu_Zstatus[imu]==1 && trigMuon==99 ) trigMuon=triMu[imu];//only pick the first zmuon (higher pt)
       }
       //
       TLorentzVector probeMu;
       TLorentzVector ZMu;
       probeMu.SetPtEtaPhiM( MuonPt->at(probeMuon), MuonEta->at(probeMuon), MuonPhi->at(probeMuon), 0 );
       for( unsigned int imu=0; imu<triMu.size(); imu++){
	 if( triMu_Zstatus[imu]==0 ) continue;//skip the probe muon
	 ZMu.SetPtEtaPhiM( MuonPt->at(imu), MuonEta->at(imu), MuonPhi->at(imu), 0 );
	 if( probeMu.DeltaR(ZMu)<0.8 ) probeMuon=99;
       }
     }

     if( debug_ && trigMuon!=99 && probeMuon!=99 ) cout<<"("<<jentry<<")  TrigMu: "<<MuonPt->at(trigMuon) <<" "<<MuonEta->at(trigMuon) <<" "<<MuonPhi->at(trigMuon)<<endl;
     if( debug_ && trigMuon!=99 && probeMuon!=99 ) cout<<"("<<jentry<<") ProbeMu: "<<MuonPt->at(probeMuon)<<" "<<MuonEta->at(probeMuon)<<" "<<MuonPhi->at(probeMuon)<<endl;

     // -- WEIGHTS - see below

     //  -- JSON SKIM
     int passJSON_=0;
     if( isData  ) passJSON_ = passJSON(run,ls,isData);
     if( !isData ) passJSON_ = 1;
     fillVariableWithValue("PassJSON", passJSON_ );//..............// returns 0, 1                                                                                              
     //  -- TRIGGER SKIM
     int passTrigger_=0;
     passTrigger_ = SingleMu_passTrigger();
     fillVariableWithValue("PassTrig", passTrigger_ );//...// returns -2, -1, 0, trig                                                                                   

     //  -- EVENT FILTER SKIM
     int passEventFilter_=0;
     if( passEventFilter() ) passEventFilter_ = 1;
     fillVariableWithValue("PassEventFilter", passEventFilter_ );//.// returns 0, 1 

     //  -- LEPTON SKIM - Three or more LI Muons & ZtoMuMu 
     int isOfflineLepton_=0;
     int CountLooseLeptons=0;
     for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
       if( muRLooseCheck(iMuR) ) CountLooseLeptons+=1;
     }
     if( CountLooseLeptons>2 && isZToMuMu() ) isOfflineLepton_=1;
     fillVariableWithValue("PassOfflineLepton", isOfflineLepton_ );// Returns 0, 1

     // -- HLT MATCHING SKIM
     int passHLTmatching_=0;
     if( trigMuon!=99 && RecoHLTdeltaRmin_SingleMuTrigger( trigMuon )<0.15 ) passHLTmatching_=1;
     fillVariableWithValue("PassHLTmatching",passHLTmatching_);

     // -- OFFLINE SKIM
     int OfflineCuts_=0;
     if( //ElCounter()==0  &&  
	 MuCounter()==3  &&  JetCounter()>=1
	 //&&  PFMETType1Cor->at(0)<20
	 && trigMuon<99 && probeMuon<99
	 ) OfflineCuts_=1;
     fillVariableWithValue("PassOfflineCuts", OfflineCuts_ );// returns 0, 1 

     evaluateCuts();
     //---------------------------------------------------------------------------------
     if( passedCut("PassJSON") && passedCut("PassTrig") && passedCut("PassEventFilter") && passedCut("PassOfflineLepton") &&
	 passedCut("PassHLTmatching") && passedCut("PassOfflineCuts") ){ 

       // WEIGHTS
       // ---- pileup
       double safePileupWeights_=0;
       safePileupWeights_=safePileupWeights();
       AppliedPileUpWeightshisto->Fill( safePileupWeights_ );
       // ---- trigger
       int usedTrigger_=-5;
       double TriggerEfficiencyWeights_=1;
       usedTrigger_ = SingleMu_passTrigger();
       Triggerhisto->Fill( usedTrigger_ );
       if( !isData ) TriggerEfficiencyWeights_=IsoMu24e2p1_Eff( MuonPt->at(trigMuon), MuonEta->at(trigMuon) );
       AppliedTrigEffWeightshisto->Fill( TriggerEfficiencyWeights_ );
       // ---- total = pileup x trigger
       double w = 0;
       w=safePileupWeights_*TriggerEfficiencyWeights_;
       AppliedTotalWeightshisto->Fill( w );
       //
       
       // Probe Muon
       LooseMuonPthisto->Fill(  MuonPt ->at(probeMuon), w );
       LooseMuonEtahisto->Fill( MuonEta->at(probeMuon), w );
       LooseMuonPhihisto->Fill( MuonPhi->at(probeMuon), w );
       if( muRTightCheck(probeMuon) ){
	 TightMuonPthisto->Fill(  MuonPt ->at(probeMuon), w );
	 TightMuonEtahisto->Fill( MuonEta->at(probeMuon), w );
	 TightMuonPhihisto->Fill( MuonPhi->at(probeMuon), w );
       }

       // is probeMuon Fake ?
       if( !isData ){
	 bool isProbeMuonFake_=true;
	 TLorentzVector mureco;
	 TLorentzVector mugen;
	 mureco.SetPtEtaPhiM( MuonPt->at(probeMuon), MuonEta->at(probeMuon), MuonPhi->at(probeMuon), 0 );
	 //
	 for( unsigned int iMuT=0; iMuT<GenWMuPt->size(); iMuT++){
	   mugen.SetPtEtaPhiM( GenWMuPt->at(iMuT),     GenWMuEta->at(iMuT),   GenWMuPhi->at(iMuT), 0 );
	   if( fabs(mureco.DeltaR(mugen))<0.15 )  isProbeMuonFake_=false;
	 }
	 for( unsigned int iMuT=0; iMuT<GenZMuPt->size(); iMuT++){
	   mugen.SetPtEtaPhiM( GenZMuPt->at(iMuT),     GenZMuEta->at(iMuT),   GenZMuPhi->at(iMuT), 0 );
	   if( fabs(mureco.DeltaR(mugen))<0.15 )  isProbeMuonFake_=false;
	 }
	 // Fake Muon
	 if( isProbeMuonFake_ ){
	   LooseFakeMuonPthisto->Fill(  MuonPt ->at(probeMuon), w );
	   LooseFakeMuonEtahisto->Fill( MuonEta->at(probeMuon), w );
	   LooseFakeMuonPhihisto->Fill( MuonPhi->at(probeMuon), w );
	   if( muRTightCheck(probeMuon) ){
	     TightFakeMuonPthisto->Fill(  MuonPt ->at(probeMuon), w );
	     TightFakeMuonEtahisto->Fill( MuonEta->at(probeMuon), w );
	     TightFakeMuonPhihisto->Fill( MuonPhi->at(probeMuon), w );
	   }
	 }
         // Prompt Muon
         if( !isProbeMuonFake_ ){
           LoosePromptMuonPthisto->Fill(  MuonPt ->at(probeMuon), w );
           LoosePromptMuonEtahisto->Fill( MuonEta->at(probeMuon), w );
           LoosePromptMuonPhihisto->Fill( MuonPhi->at(probeMuon), w );
           if( muRTightCheck(probeMuon) ){
             TightPromptMuonPthisto->Fill(  MuonPt ->at(probeMuon), w );
             TightPromptMuonEtahisto->Fill( MuonEta->at(probeMuon), w );
             TightPromptMuonPhihisto->Fill( MuonPhi->at(probeMuon), w );
           }
         }
	 //
       }


       // >>>>>>>>>>>>>>>>>>>>>> Histograms - multiple times per event. 
       allmu.SetPtEtaPhiM( 0, 0, 0, 0 );
       alltau.SetPtEtaPhiM( 0, 0, 0, 0 );
       allmu2.SetPtEtaPhiM( 0, 0, 0, 0 );
       alltau2.SetPtEtaPhiM( 0, 0, 0, 0 );
       //
       for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
	 if(!muRisoCheck(iMuR))continue;
	 //
	 METmuTransMasshisto->Fill( METlepMT("Mu",iMuR),       w );
	 METmuDeltaPhihisto ->Fill( METlepDeltaPhi("Mu",iMuR), w );
	 //
	 allmu.SetPtEtaPhiM( MuonPt->at(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0);
	 for(unsigned int iMuR2=(iMuR+1); iMuR2<MuonPt->size(); iMuR2++){
	   if(!muRisoCheck(iMuR2))continue;
	   allmu2.SetPtEtaPhiM( MuonPt->at(iMuR2), MuonEta->at(iMuR2), MuonPhi->at(iMuR2), 0);
	   AllMuMuInvMasshisto->Fill( (double)((allmu+allmu2).M()), w );
	 }
       }
       //
       for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
	 if(!muRisoCheck(iMuR))continue;
	 allmu.SetPtEtaPhiM( MuonPt->at(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0);
	 for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
	   if(!tauRisoCheck(iTauR))continue;
	   alltau.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR) , HPSTauPhi->at(iTauR), 0);
	   AllMuTauInvMasshisto->Fill( (double)((allmu+alltau).M()), w );
	 }
       }
       //
       for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
	 if(!tauRisoCheck(iTauR))continue;
	 //
	 METtauTransMasshisto->Fill( METlepMT("Tau",iTauR),       w );
	 METtauDeltaPhihisto ->Fill( METlepDeltaPhi("Tau",iTauR), w );
	 //
	 alltau.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR) , HPSTauPhi->at(iTauR), 0);
	 for(unsigned int iTauR2=(iTauR+1); iTauR2<HPSTauPt->size(); iTauR2++){
	   if(!tauRisoCheck(iTauR2))continue;
	   alltau2.SetPtEtaPhiM( HPSTauPt->at(iTauR2), HPSTauEta->at(iTauR2) , HPSTauPhi->at(iTauR2), 0);
	   AllTauTauInvMasshisto->Fill( (double)((alltau+alltau2).M()), w );
	 }
       }
       //

       // >>>>>>>>>>>>>>>>>>>>>> Histograms - once per event. 
       Mu.SetPtEtaPhiM(0,0,0,0);
       Tau.SetPtEtaPhiM(0,0,0,0);
       El.SetPtEtaPhiM(0,0,0,0);
       Jet.SetPtEtaPhiM(0,0,0,0);
       BJet.SetPtEtaPhiM(0,0,0,0);
       MuN=0;
       TauN=0;
       ElN=0;
       JetN=0;
       BJetN=0;
       SJT=0;
       VertexN=0;
       //
       //
       for(unsigned int iVertexR=0; iVertexR<VertexX->size(); iVertexR++){
	 if( vertexRCheck(iVertexR) ) VertexN++;
       }
       VertexNhisto->Fill((double)VertexN, w);
       //
       for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
	 if( !tauRisoCheck(iTauR) )continue;  
	 TauN++;
	 TauPthisto->Fill(HPSTauPt->at(iTauR), w );
	 TauEtahisto->Fill(HPSTauEta->at(iTauR), w );
	 TauPhihisto->Fill(HPSTauPhi->at(iTauR), w );
	 if( HPSTauPt->at(iTauR)>Tau.Pt() ){ 
	   Tau.SetPtEtaPhiM(HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0);
	 }
       }
       for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
	 if( !muRisoCheck(iMuR)        )continue;  
	 MuN++;
	 MuPthisto->Fill(MuonPt->at(iMuR), w );
	 MuEtahisto->Fill(MuonEta->at(iMuR), w );
	 MuPhihisto->Fill(MuonPhi->at(iMuR), w );
	 if( MuonPt->at(iMuR)>Mu.Pt() ){ 
	   Mu.SetPtEtaPhiM(MuonPt->at(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0);
	 }
       }
       for(unsigned int iElR=0; iElR<ElectronPt->size(); iElR++){
	 if( !elRisoCheck(iElR)        )continue;  
	 ElN++;
	 ElPthisto->Fill(ElectronPt->at(iElR), w );
	 ElEtahisto->Fill(ElectronEta->at(iElR), w );
	 ElPhihisto->Fill(ElectronPhi->at(iElR), w );
	 if( ElectronPt->at(iElR)>El.Pt() ){
	   El.SetPtEtaPhiM(ElectronPt->at(iElR), ElectronEta->at(iElR), ElectronPhi->at(iElR), 0);
	 }
       }
       for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
	 if( !jetRisoCheck(iJetR)   )continue;  
	 JetN++;
	 SJT+=PFJetPt->at(iJetR);
	 JetPthisto->Fill(PFJetPt->at(iJetR), w );
	 JetEtahisto->Fill(PFJetEta->at(iJetR), w );
	 JetPhihisto->Fill(PFJetPhi->at(iJetR), w );
	 if(PFJetPt->at(iJetR)>Jet.Pt() ){
	   Jet.SetPtEtaPhiM(PFJetPt->at(iJetR),PFJetEta->at(iJetR),PFJetPhi->at(iJetR),0);
	 }
	 //BJet
	 if(  btag_jetRCheck(iJetR)  ){
	   BJetN++;
	   BJetPthisto->Fill(PFJetPt->at(iJetR), w );
	   BJetEtahisto->Fill(PFJetEta->at(iJetR), w );
	   BJetPhihisto->Fill(PFJetPhi->at(iJetR), w );
	   if(PFJetPt->at(iJetR)>BJet.Pt() ){
	     BJet.SetPtEtaPhiM(PFJetPt->at(iJetR),PFJetEta->at(iJetR),PFJetPhi->at(iJetR),0);
	   }
	 }
	 //
       }
       //
       //
       //
       // ------ Filling histograms
       //
       //////  recoSignalType_ = (1000*TotalLepCtr) + (100*IsoTauCtr) + (10*IsoMuCtr) + (IsoElCtr) //////
       int RecoSignalType_=RecoSignalType();
       if( RecoSignalType_>2000      ){RecoSignalTypehisto->Fill((double)1, w );   } // 2,3,4 leptons, including ssdl at given ObjDef (++|--|++-|+--|++--)
       if( abs(RecoSignalType_)<2000 ){RecoSignalTypehisto->Fill((double)0, w );   } // 1 or 0 leptons at given ObjDef
       if( RecoSignalType_<-2000     ){RecoSignalTypehisto->Fill((double)(-1), w );} // 2 leptons opp. sign, sstrilep, etc. at given ObjDef (+-|+++|---|+++-|+---|++++|----)
       //
       //..............................................................................................................................//(++|--|++-|+--|++--) 
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%100-RecoSignalType_%10)==40 ){MuBasedRecoSignalTypehisto->Fill((double)(+4), w ); }//4mu
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%100-RecoSignalType_%10)==30 ){MuBasedRecoSignalTypehisto->Fill((double)(+3), w ); }//3mu
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%100-RecoSignalType_%10)==20 ){MuBasedRecoSignalTypehisto->Fill((double)(+2), w ); }//2mu
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%100-RecoSignalType_%10)==10 ){MuBasedRecoSignalTypehisto->Fill((double)(+1), w ); }//1mu
       //..............................................................................................................................//(+-|+++|---|+++-|+---|++++|----) 
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%100-RecoSignalType_%10)==10 ){MuBasedRecoSignalTypehisto->Fill((double)(-1), w ); }//1mu       
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%100-RecoSignalType_%10)==20 ){MuBasedRecoSignalTypehisto->Fill((double)(-2), w ); }//2mu
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%100-RecoSignalType_%10)==30 ){MuBasedRecoSignalTypehisto->Fill((double)(-3), w ); }//3mu 
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%100-RecoSignalType_%10)==40 ){MuBasedRecoSignalTypehisto->Fill((double)(-4), w ); }//4mu 
       if( (RecoSignalType_%100-RecoSignalType_%10)==0   ){MuBasedRecoSignalTypehisto->Fill((double)(0),  w ); }//0mu
       //
       //..............................................................................................................................//(++|--|++-|+--|++--) 
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==400 ){TauBasedRecoSignalTypehisto->Fill((double)(+4), w ); }//4tau
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==300 ){TauBasedRecoSignalTypehisto->Fill((double)(+3), w ); }//3tau
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==200 ){TauBasedRecoSignalTypehisto->Fill((double)(+2), w ); }//2tau
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==100 ){TauBasedRecoSignalTypehisto->Fill((double)(+1), w ); }//1tau
       //..............................................................................................................................//(+-|+++|---|+++-|+---|++++|----) 
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==100 ){TauBasedRecoSignalTypehisto->Fill((double)(-1), w ); }//1tau       
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==200 ){TauBasedRecoSignalTypehisto->Fill((double)(-2), w ); }//2tau
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==300 ){TauBasedRecoSignalTypehisto->Fill((double)(-3), w ); }//3tau 
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==400 ){TauBasedRecoSignalTypehisto->Fill((double)(-4), w ); }//4tau 
       if( (RecoSignalType_%1000-RecoSignalType_%100)==0   ){TauBasedRecoSignalTypehisto->Fill((double)(0),  w ); }//0tau
       //
       METhisto->Fill(PFMETType1Cor->at(0), w );
       //
       if( MuN>0   )LeadMuPthisto->Fill(Mu.Pt(), w );
       if( TauN>0  )LeadTauPthisto->Fill(Tau.Pt(), w );
       if( ElN>0   )LeadElPthisto->Fill(El.Pt(), w );
       if( JetN>0  )LeadJetPthisto->Fill(Jet.Pt(), w );
       if( BJetN>0 )LeadBJetPthisto->Fill(BJet.Pt(), w );
       //
       if( MuN>0   )LeadMuEtahisto->Fill(Mu.Eta(), w );
       if( TauN>0  )LeadTauEtahisto->Fill(Tau.Eta(), w );
       if( ElN>0   )LeadElEtahisto->Fill(El.Eta(), w );
       if( JetN>0  )LeadJetEtahisto->Fill(Jet.Eta(), w );
       if( BJetN>0 )LeadBJetEtahisto->Fill(BJet.Eta(), w );
       //
       if( MuN>0   )LeadMuPhihisto->Fill(Mu.Phi(), w );
       if( TauN>0  )LeadTauPhihisto->Fill(Tau.Phi(), w );
       if( ElN>0   )LeadElPhihisto->Fill(El.Phi(), w );
       if( JetN>0  )LeadJetPhihisto->Fill(Jet.Phi(), w );
       if( BJetN>0 )LeadBJetPhihisto->Fill(BJet.Phi(), w );
       //
       //
       MaxMuTauInvMasshisto->Fill( (double)(MaxMuTauInvMass()) );
       MaxDiLepMasshisto->Fill(MaxDiLepInvMass(), w );
       SThisto->Fill(  ST(), w );
       //-----------------
       /*if( isFakeMu_TOP() ||  isFakeTau_TOP() ){ 
	 if(  isFakeMu_TOP() && !isFakeTau_TOP()  ){FakesDistrhisto->Fill( (double)1, w);}//fake mu
	 if(  isFakeMu_TOP() &&  isFakeTau_TOP()  ){FakesDistrhisto->Fill( (double)2, w);}//fake both
	 if( !isFakeMu_TOP() &&  isFakeTau_TOP()  ){FakesDistrhisto->Fill( (double)3, w);}//fake tau
	 }*/
       /*if( isFakeMu_LQ() ||  isFakeTau_LQ() ){ 
	 if(  isFakeMu_LQ() && !isFakeTau_LQ()  ){FakesDistrhisto->Fill( (double)1, w);}//fake mu
	 if(  isFakeMu_LQ() &&  isFakeTau_LQ()  ){FakesDistrhisto->Fill( (double)2, w);}//fake both
	 if( !isFakeMu_LQ() &&  isFakeTau_LQ()  ){FakesDistrhisto->Fill( (double)3, w);}//fake tau
	 }*/
       //if(  isFakeMu_TOP() ||  isFakeTau_TOP() ){ STwFakeshisto->Fill(  ST(), w ); } if( !isFakeMu_TOP() && !isFakeTau_TOP() ){ STwRealshisto->Fill( ST(), w ); }//fake
       //if(  isFakeMu_LQ()  ||  isFakeTau_LQ()  ){ STwFakeshisto->Fill(  ST(), w ); } if( !isFakeMu_LQ()  && !isFakeTau_LQ()  ){ STwRealshisto->Fill( ST(), w ); }//fake
       STotherhisto->Fill( ST(), w );//fake
       //-----------------
       SJThisto->Fill( SJT, w  );
       if( MuN>0 && TauN>0 )LeadMuTauDeltaRhisto->Fill(Mu.DeltaR(Tau), w );
       //
       MuNhisto->Fill((double)MuN, w );
       TauNhisto->Fill((double)TauN, w );
       ElNhisto->Fill((double)ElN, w );
       JetNhisto->Fill((double)JetN, w );
       BJetNhisto->Fill((double)BJetN, w );
       TotalNhisto->Fill((double)(MuN+TauN+ElN+JetN), w );
       //

     }
     //---------------------------------------------------------------------------------

     ////////////////////// User's code ends here ///////////////////////
   } // End loop over events

   //////////write histos 
   //
   ProvidedPileUpWeightshisto->Write();
   AppliedPileUpWeightshisto->Write();
   AppliedTrigEffWeightshisto->Write();
   Triggerhisto->Write();
   AppliedTotalWeightshisto->Write();
   //
   RecoSignalTypehisto->Write();
   MuBasedRecoSignalTypehisto->Write();
   TauBasedRecoSignalTypehisto->Write();
   //
   METhisto->Write();
   MaxMuTauInvMasshisto->Write();
   MaxDiLepMasshisto->Write();
   AllMuMuInvMasshisto->Write();
   AllMuTauInvMasshisto->Write();
   AllTauTauInvMasshisto->Write();
   METmuTransMasshisto->Write();
   METtauTransMasshisto->Write();
   //
   SThisto->Write();
   STwFakeshisto->Write();//fake
   STwRealshisto->Write();//fake
   STotherhisto->Write(); //fake
   FakesDistrhisto->Write();//fake
   SJThisto->Write();
   LeadMuTauDeltaRhisto->Write();
   //
   METtauDeltaPhihisto->Write();
   METmuDeltaPhihisto ->Write();
   METelDeltaPhihisto ->Write();
   //
   LeadMuPthisto->Write();
   LeadTauPthisto->Write();
   LeadElPthisto->Write();
   LeadJetPthisto->Write();
   LeadBJetPthisto->Write();
   LeadMuEtahisto->Write();
   LeadTauEtahisto->Write();
   LeadElEtahisto->Write();
   LeadJetEtahisto->Write();
   LeadBJetEtahisto->Write();
   LeadMuPhihisto->Write();
   LeadTauPhihisto->Write();
   LeadElPhihisto->Write();
   LeadJetPhihisto->Write();
   LeadBJetPhihisto->Write();
   //
   MuPthisto->Write();
   TauPthisto->Write();
   ElPthisto->Write();
   JetPthisto->Write();
   BJetPthisto->Write();
   MuEtahisto->Write();
   TauEtahisto->Write();
   ElEtahisto->Write();
   JetEtahisto->Write();
   BJetEtahisto->Write();
   MuPhihisto->Write();
   TauPhihisto->Write();
   ElPhihisto->Write();
   JetPhihisto->Write();
   BJetPhihisto->Write();
   //
   MuNhisto->Write();
   TauNhisto->Write();
   ElNhisto->Write();
   JetNhisto->Write();
   BJetNhisto->Write();
   TotalNhisto->Write();
   VertexNhisto->Write();
   //
   LooseToTightMatrixRAW2Dhisto->Write();
   LooseToTightMatrix2Dhisto->Write();
   //
   LooseFakeMuonPthisto ->Write();
   LooseFakeMuonEtahisto->Write();
   LooseFakeMuonPhihisto->Write();
   TightFakeMuonPthisto ->Write();
   TightFakeMuonEtahisto->Write();
   TightFakeMuonPhihisto->Write();
   //
   LoosePromptMuonPthisto ->Write();
   LoosePromptMuonEtahisto->Write();
   LoosePromptMuonPhihisto->Write();
   TightPromptMuonPthisto ->Write();
   TightPromptMuonEtahisto->Write();
   TightPromptMuonPhihisto->Write();
   //
   LooseMuonPthisto ->Write();
   LooseMuonEtahisto->Write();
   LooseMuonPhihisto->Write();
   TightMuonPthisto ->Write();
   TightMuonEtahisto->Write();
   TightMuonPhihisto->Write();
   //
   LooseTightRatioPtMuonhisto ->TGraphAsymmErrors::Divide( TightMuonPthisto->Rebin(10),  LooseMuonPthisto->Rebin(10),  "vcp" );
   LooseTightRatioEtaMuonhisto->TGraphAsymmErrors::Divide( TightMuonEtahisto->Rebin(10), LooseMuonEtahisto->Rebin(10), "vcp" );
   LooseTightRatioPhiMuonhisto->TGraphAsymmErrors::Divide( TightMuonPhihisto->Rebin(10), LooseMuonPhihisto->Rebin(10), "vcp" );
   LooseTightRatioPtMuonhisto ->Write();
   LooseTightRatioEtaMuonhisto->Write();
   LooseTightRatioPhiMuonhisto->Write();

   //////////
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
// // //
