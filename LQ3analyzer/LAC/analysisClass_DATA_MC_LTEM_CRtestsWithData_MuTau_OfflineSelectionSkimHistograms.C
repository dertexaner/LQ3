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
//
//LAC
//#include "LAC/ADDON1_LAC.C" //this is included in LAC/ADDON2_LAC.C
#include "LAC/ADDON2_LAC.C"
#include "LAC/ADDON3_LAC.C"
//#include "LAC/ADDON4_LAC.C"///  LQgen functions //fake
//#include "LAC/ADDON5_LAC.C" //gen-reco matching 
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
   LumiA=891; 
   LumiB=4428;
   LumiC=7018;
   LumiD=7248;
   //
   ResetAllSFs();
   //


   //////////book histos here
   //
   //distribution of the provided PileUp-weights
   TH1D* ProvidedPileUpWeightshisto  = new TH1D("ProvidedPileUpWeightshisto","ProvidedPileUpWeightshisto",10000,0,100);
   //distribution of the applied PileUp-weights
   TH1D* AppliedPileUpWeightshisto   = new TH1D("AppliedPileUpWeightshisto", "AppliedPileUpWeightshisto", 10000,0,100);
   //distribution of the applied TrigEff-weights
   TH1D* AppliedTrigEffWeightshisto  = new TH1D("AppliedTrigEffWeightshisto","AppliedTrigEffWeightshisto",10000,0,100);
   //distribution of triggers in the events
   TH1D* Triggerhisto                = new TH1D("Triggerhisto",              "Triggerhisto",              10,-3.5,6.5);
   //distribution of the applied Total-weights
   TH1D* AppliedTotalWeightshisto    = new TH1D("AppliedTotalWeightshisto",  "AppliedTotalWeightshisto",  10000,0,100);
   //
   TH1D* muPRhisto   = new TH1D("muPRhisto",  "muPRhisto",   2000,0,2);
   TH1D* muFRhisto   = new TH1D("muFRhisto",  "muFRhisto",   2000,0,2);
   TH1D* tauPRhisto  = new TH1D("tauPRhisto", "tauPRhisto",  2000,0,2);
   TH1D* tauFRhisto  = new TH1D("tauFRhisto", "tauFRhisto",  2000,0,2);
   TH1D* muPREhisto  = new TH1D("muPREhisto", "muPREhisto",  1000,-0.25,0.75);
   TH1D* muFREhisto  = new TH1D("muFREhisto", "muFREhisto",  1000,-0.25,0.75);
   TH1D* tauPREhisto = new TH1D("tauPREhisto","tauPREhisto", 1000,-0.25,0.75);
   TH1D* tauFREhisto = new TH1D("tauFRhisto", "tauFREhisto", 1000,-0.25,0.75);
   //
   TH1D* LeadMuTauDeltaRhisto = new TH1D("LeadMuTauDeltaRhisto","LeadMuTauDeltaRhisto",5000,0,50);
   TH1D* SThisto              = new TH1D("SThisto", "SThisto", 5000,0,5000);
   TH1D* SJThisto             = new TH1D("SJThisto","SJThisto",5000,0,5000);
   TH1D* METhisto             = new TH1D("METhisto","METhisto",2000,0,2000);
   TH1D* PZetahisto           = new TH1D("PZetahisto","PZetahisto",8000,-2000,2000);
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
   TH1D* LTEMMuJetDeltaRminhisto   = new TH1D("LTEMMuJetDeltaRminhisto",  "LTEMMuJetDeltaRminhisto",  200,-5,5);
   TH1D* LTEMTauJetDeltaRminhisto  = new TH1D("LTEMTauJetDeltaRminhisto", "LTEMTauJetDeltaRminhisto", 200,-5,5);
   TH1D* LTEMMuPthisto             = new TH1D("LTEMMuPthisto",            "LTEMMuPthisto",            2000,0,2000);
   TH1D* LTEMTauPthisto            = new TH1D("LTEMTauPthisto",           "LTEMTauPthisto",           2000,0,2000);
   TH1D* LTEMMuEtahisto            = new TH1D("LTEMMuEtahisto",           "LTEMMuEtahisto",           800,-4,4);
   TH1D* LTEMTauEtahisto           = new TH1D("LTEMTauEtahisto",          "LTEMTauEtahisto",          800,-4,4);
   TH1D* LTEMMuPhihisto            = new TH1D("LTEMMuPhihisto",           "LTEMMuPhihisto",           800,-4,4);
   TH1D* LTEMTauPhihisto           = new TH1D("LTEMTauPhihisto",          "LTEMTauPhihisto",          800,-4,4);
   TH1D* LTEMMuTauDeltaRhisto      = new TH1D("LTEMMuTauDeltaRhisto",     "LTEMMuTauDeltaRhisto",     300,-5,10);
   //TH1D* LTEMMuTauDeltaRhisto      = new TH1D("LTEMMuTauDeltaRhisto",     "LTEMMuTauDeltaRhisto",     200,-5,5);
   TH1D* LTEMMuTauDeltaPhihisto    = new TH1D("LTEMMuTauDeltaPhihisto",   "LTEMMuTauDeltaPhihisto",   200,-5,5);
   TH1D* LTEMMuTauMasshisto        = new TH1D("LTEMMuTauMasshisto",       "LTEMMuTauMasshisto",       5000,0,5000);
   //
   // For 2D optimization
   TH2D* LTEM2DMuPtTauPthisto = new TH2D("LTEM2DMuPtTauPthisto","LTEM2DMuPtTauPthisto", 120,0,600,120,0,600);
   TH2D* LTEM2DTauPtSThisto   = new TH2D("LTEM2DTauPtSThisto","LTEM2DTauPtSThisto", 120,0,600,500,0,5000);
   TH2D* LTEM2DTauPtNLepjethisto = new TH2D("LTEM2DTauPtNLepjethisto","LTEM2DTauPtNLepjethisto", 120,0,600,21,-0.5,20.5);
   TH2D* LTEM2DMuPtNLepjethisto  = new TH2D("LTEM2DMuPtNLepjethisto","LTEM2DMuPtNLepjethisto", 120,0,600,21,-0.5,20.5);
   TH2D* LTEM2DMuPtSThisto    = new TH2D("LTEM2DMuPtSThisto","LTEM2DMuPtSThisto", 120,0,600,500,0,5000);
   TH2D* LTEM2DNLepjetSThisto = new TH2D("LTEM2DNLepjetSThisto","LTEM2DNLepjetSThisto", 21,-0.5,20.5,500,0,5000);
   TH2D* LTEM2DNJetSThisto    = new TH2D("LTEM2DNJetSThisto","LTEM2DNJetSThisto", 21,-0.5,20.5,500,0,5000);
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
   TH1D* AllGenhisto  = new TH1D("AllGenhisto","AllGenhisto",4,0.5,4.5);//MuTau
   TH1D* TTGenhisto   = new TH1D("TTGenhisto","TTGenhisto",4,0.5,4.5);//LL LT TL TT
   TH1D* TTGenWithSFhisto   = new TH1D("TTGenWithSFhisto","TTGenWithSFhisto",4,0.5,4.5);//LL LT TL TT // tau fake, prompt rate SF's applied
   //
   TH1D* LTmatrixhisto = new TH1D("LTmatrixhisto","LTmatrixhisto",4,0.5,4.5);
   //
   TH1D* FPmatrixhisto = new TH1D("FPmatrixhisto","FPmatrixhisto",4,0.5,4.5);
   TH1D* FPmatrixMuPRUhisto  = new TH1D("FPmatrixMuPRUhisto","FPmatrixMuPRUhisto",4,0.5,4.5);
   TH1D* FPmatrixMuPRDhisto  = new TH1D("FPmatrixMuPRDhisto","FPmatrixMuPRDhisto",4,0.5,4.5);
   TH1D* FPmatrixMuFRUhisto  = new TH1D("FPmatrixMuFRUhisto","FPmatrixMuFRUhisto",4,0.5,4.5);
   TH1D* FPmatrixMuFRDhisto  = new TH1D("FPmatrixMuFRDhisto","FPmatrixMuFRDhisto",4,0.5,4.5);
   TH1D* FPmatrixTauPRUhisto = new TH1D("FPmatrixTauPRUhisto","FPmatrixTauPRUhisto",4,0.5,4.5);
   TH1D* FPmatrixTauPRDhisto = new TH1D("FPmatrixTauPRDhisto","FPmatrixTauPRDhisto",4,0.5,4.5);
   TH1D* FPmatrixTauFRUhisto = new TH1D("FPmatrixTauFRUhisto","FPmatrixTauFRUhisto",4,0.5,4.5);
   TH1D* FPmatrixTauFRDhisto = new TH1D("FPmatrixTauFRDhisto","FPmatrixTauFRDhisto",4,0.5,4.5);
   //
   TH1D* TTBackgroundhisto = new TH1D("TTBackgroundhisto","TTBackgroundhisto",4,0.5,4.5);
   TH1D* TTBackgroundMuPRUhisto = new TH1D("TTBackgroundMuPRUhisto","TTBackgroundMuPRUhisto",4,0.5,4.5);
   TH1D* TTBackgroundMuPRDhisto = new TH1D("TTBackgroundMuPRDhisto","TTBackgroundMuPRDhisto",4,0.5,4.5);
   TH1D* TTBackgroundMuFRUhisto = new TH1D("TTBackgroundMuFRUhisto","TTBackgroundMuFRUhisto",4,0.5,4.5);
   TH1D* TTBackgroundMuFRDhisto = new TH1D("TTBackgroundMuFRDhisto","TTBackgroundMuFRDhisto",4,0.5,4.5);
   TH1D* TTBackgroundTauPRUhisto = new TH1D("TTBackgroundTauPRUhisto","TTBackgroundTauPRUhisto",4,0.5,4.5);
   TH1D* TTBackgroundTauPRDhisto = new TH1D("TTBackgroundTauPRDhisto","TTBackgroundTauPRDhisto",4,0.5,4.5);
   TH1D* TTBackgroundTauFRUhisto = new TH1D("TTBackgroundTauFRUhisto","TTBackgroundTauFRUhisto",4,0.5,4.5);
   TH1D* TTBackgroundTauFRDhisto = new TH1D("TTBackgroundTauFRDhisto","TTBackgroundTauFRDhisto",4,0.5,4.5);
   //
   LTmatrixhisto->GetXaxis()->SetBinLabel( 1, "LL" );   LTmatrixhisto->GetXaxis()->SetBinLabel( 2, "LT" );
   LTmatrixhisto->GetXaxis()->SetBinLabel( 3, "TL" );   LTmatrixhisto->GetXaxis()->SetBinLabel( 4, "TT" );
   //
   FPmatrixhisto->GetXaxis()->SetBinLabel( 1, "FF" );   FPmatrixhisto->GetXaxis()->SetBinLabel( 2, "FP" );
   FPmatrixhisto->GetXaxis()->SetBinLabel( 3, "PF" );   FPmatrixhisto->GetXaxis()->SetBinLabel( 4, "" );
   //
   TTBackgroundhisto->GetXaxis()->SetBinLabel( 1, "TTfromFF" );   TTBackgroundhisto->GetXaxis()->SetBinLabel( 2, "TTfromFP" );
   TTBackgroundhisto->GetXaxis()->SetBinLabel( 3, "TTfromPF" );   TTBackgroundhisto->GetXaxis()->SetBinLabel( 4, "TTminusFakes" );

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

     // -- RUN LTEM 
     which_MuTau(ltemMuTau);

     // WEIGHTS
     // ---- pileup
     double safePileupWeights_=0;
     safePileupWeights_=safePileupWeights();
     // ---- trigger 
     unsigned int triggerMuon=99;
     for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
	 if( !muRisoCheck(iMuR) ) continue;
	 triggerMuon=iMuR;
	 break;//take the leading one
     }
     if( ltemMuTau.size()==2 ) triggerMuon=ltemMuTau[0];
     //
     if( ltemMuTau.size()!=2 ) continue; 
     //
     int usedTrigger_=-5;
     //usedTrigger_ = SingleMu_passTrigger();
     usedTrigger_ = SingleMu40_passTrigger();
     //if( !isMu40_ ) usedTrigger_ = HLT_MuPT_eta2p1_passTrigger();
     //usedTrigger_ = HLT_MuPT_eta2p1_passTrigger();
     double TriggerEfficiencyWeights_;
     // use same weights for Mu24 and Mu40 triggers
     //TriggerEfficiencyWeights_=IsoMu24e2p1_Eff( MuonPt->at(triggerMuon), MuonEta->at(triggerMuon) );
     //if(  isMu40_ ) TriggerEfficiencyWeights_=Mu40e2p1_ScaleFactor( 50, MuonEta->at(triggerMuon) );
     //if( !isMu40_ ) TriggerEfficiencyWeights_=Mu40e2p1_ScaleFactor( 50, MuonEta->at(triggerMuon) );
     TriggerEfficiencyWeights_=Mu40e2p1_ScaleFactor( MuonPt->at(triggerMuon), MuonEta->at(triggerMuon) );
     AppliedTrigEffWeightshisto->Fill( TriggerEfficiencyWeights_ );
     // ---- total = pileup x trigger
     double w = 0;
     if( !isData  ){
       w=safePileupWeights_*TriggerEfficiencyWeights_; 
       // w=w*LTEM_ScaleFactor( ltemMuTau );
     }
     if( isData  ) w=1;
     //w=1;//set all weights to one!!
     //w=safePileupWeights_; // PU weights only

     //  -- JSON SKIM
     int passJSON_=0;
     if( isData  ) passJSON_ = passJSON(run,ls,isData);
     if( !isData ) passJSON_ = 1;
     fillVariableWithValue("PassJSON", passJSON_ );//..............// returns 0, 1                                                                      

     //  -- TRIGGER SKIM
     int passTrigger_=0;
     //passTrigger_ = SingleMu_passTrigger(); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<this is IsoMu24 trigger
     passTrigger_ = SingleMu40_passTrigger();        //<<<<<<<<<<<<<<<<< this is Mu40 trigger 
     //if( !isMu40_ ) passTrigger_ = HLT_MuPT_eta2p1_passTrigger(); //<<<<<<<<<<<<<<<<< this is Mu24 trigger  (no isolation)
     //passTrigger_ = HLT_MuPT_eta2p1_passTrigger(); //<<<<<<<<<<<<<<<<< this is Mu24 trigger  (no isolation)
     fillVariableWithValue("PassTrig", passTrigger_ );//...// returns -2, -1, 0, trig                                                                         

     //  -- EVENT FILTER SKIM
     int passEventFilter_=0;
     if( passEventFilter() ) passEventFilter_ = 1;
     fillVariableWithValue("PassEventFilter", passEventFilter_ );//.// returns 0, 1 


     //  -- DILEPTON SKIM - MuTau
     int isOfflineDilepton_=0;
     if( isMuTauDR0p30() ) isOfflineDilepton_ = 1;
     //Make Trigger Muon Tight
     //if( isMuTauDR0p30() && muRTightCheck(triggerMuon) ) isOfflineDilepton_ = 1;
     fillVariableWithValue("PassOfflineDilepton", isOfflineDilepton_ );// Returns 0, 1          


     // -- HLT MATCHING SKIM
     int passAllMuHLTmatching_=0;
     //passAllMuHLTmatching_=1; // this is done in the skimming step already.
     //if(  RecoHLTdeltaRmin_SingleMuTrigger(triggerMuon)<0.15              ) passAllMuHLTmatching_=1;
     if( RecoHLTdeltaRmin_SingleMu40Trigger(triggerMuon)<0.15 ) passAllMuHLTmatching_=1;
     //if( !isMu40_ && RecoHLTdeltaRmin_SingleMu24Trigger(triggerMuon)<0.15 ) passAllMuHLTmatching_=1;
     //if( RecoHLTdeltaRmin_SingleMu24Trigger(triggerMuon)<0.15 ) passAllMuHLTmatching_=1;
     fillVariableWithValue("PassAllMuHLTmatching",passAllMuHLTmatching_);


     
     // 
     // Computing Pzeta quantity
     double pZeta=-99999;
     if( ltemMuTau.size()==2 ){
       unsigned int tagMuon=ltemMuTau[0];
       unsigned int probeTau=ltemMuTau[1];
       //
       TLorentzVector muTransVector, tauTransVector, metTransVector;
       muTransVector.SetPtEtaPhiM(  MuonPt->at(tagMuon),     0, MuonPhi->at(tagMuon),       0 );
       tauTransVector.SetPtEtaPhiM( HPSTauPt->at(probeTau),  0, HPSTauPhi->at(probeTau),    0 );
       metTransVector.SetPtEtaPhiM( PFMETType01XYCor->at(0), 0, PFMETPhiType01XYCor->at(0), 0 );
       //
       TLorentzVector muTransUnitVector, tauTransUnitVector, zTransVector;
       muTransUnitVector.SetPtEtaPhiM(  1, 0, MuonPhi->at(tagMuon),    0 );
       tauTransUnitVector.SetPtEtaPhiM( 1, 0, HPSTauPhi->at(probeTau), 0 );
       zTransVector.SetPtEtaPhiM( 1, 0, (muTransUnitVector+tauTransUnitVector).Phi(), 0 );
       //
       TVector2 pz2D, pzvis2D, zeta2D;
       pz2D.SetMagPhi(    (muTransVector+tauTransVector+metTransVector).Pt(), (muTransVector+tauTransVector+metTransVector).Phi() );
       pzvis2D.SetMagPhi( (muTransVector+tauTransVector).Pt(),                (muTransVector+tauTransVector).Phi()                );
       zeta2D.SetMagPhi(  1,                                                  zTransVector.Phi()                                  );
       //
       pZeta=((pz2D.Mod())*(zeta2D.Mod())*TMath::Cos(pz2D.DeltaPhi(zeta2D)))-1.5*((pzvis2D.Mod())*(zeta2D.Mod())*TMath::Cos(pzvis2D.DeltaPhi(zeta2D)));
     }

     // -- OFFLINE SKIM
     int OfflineCuts_=0;
     int NLepJet= MuCounter()+ElCounter()+TauCounter()+JetCounter();
     //
     if( ltemMuTau.size()==2 && MuonPt->at(ltemMuTau[0])>45 &&
	 pZeta>-10 &&
	 //fabs(HPSTauEta->at(ltemMuTau[1]))<1.5 && //barrel Taus only
	 fabs(HPSTauEta->at(ltemMuTau[1]))>=1.5 && //endcap Taus only
	 HPSTauPt->at(ltemMuTau[1])>35 && //TauPt>35
	 //fabs(MuonEta->at(ltemMuTau[0]))<1.5 && //barrel Muons only
	 ST()>150  && JetCounter()<=1 && NLepJet<=3 && // <<<< CONTROL REGION with DATA
	 //ST()>300  && JetCounter()>1 && NLepJet>3 && // <<<< CONTROL REGION like SIGNAL selection!! (for mixing the two)
	 //ST()>500  && JetCounter()>1 &&  ltemMuTau.size()==2 &&	 NLepJet>4 && // <<<< SIGNAL REGION no DATA
	 muJetDeltaRmin(ltemMuTau[0])>0.5 && tauJetDeltaRmin(ltemMuTau[1])>0.7 ) OfflineCuts_=1;
	 //muJetDeltaRmin(ltemMuTau[0])>0.5 && tauJetDeltaRmin(ltemMuTau[1])>1.0 ) OfflineCuts_=1;
     fillVariableWithValue("PassOfflineCuts", OfflineCuts_ );// returns 0, 1 
     
     
     evaluateCuts();
     //---------------------------------------------------------------------------------
     if( passedCut("PassJSON") && passedCut("PassTrig") && passedCut("PassEventFilter") && passedCut("PassOfflineDilepton") &&
	 passedCut("PassAllMuHLTmatching") && passedCut("PassOfflineCuts") ){ 
       

       bool isRecoMuPrompt_  = false;
       bool isRecoTauPrompt_ = false;
       //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       // Check whether LTEM mu & tau are Fake or Prompt!!
       TLorentzVector RecoMu, RecoTau, GenMu, GenTau;
       RecoMu.SetPtEtaPhiM( MuonPt->at(ltemMuTau[0]), MuonEta->at(ltemMuTau[0]), MuonPhi->at(ltemMuTau[0]), 0 );
       RecoTau.SetPtEtaPhiM( HPSTauPt->at(ltemMuTau[1]), HPSTauEta->at(ltemMuTau[1]), HPSTauPhi->at(ltemMuTau[1]), 0 );
       //Check RecoMu  ( W )
       for( unsigned int iMuT=0; iMuT<GenWMuPt->size(); iMuT++){
	 GenMu.SetPtEtaPhiM( GenWMuPt->at(iMuT),     GenWMuEta->at(iMuT),   GenWMuPhi->at(iMuT), 0 );
	 if( GenMu.DeltaR(RecoMu)<0.15 ) isRecoMuPrompt_ = true;
       }
       //Check RecoTau ( W )
       for( unsigned int iTauT=0; iTauT<GenWTauPt->size(); iTauT++){
	 if( GenWTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
	 GenTau.SetPtEtaPhiM( GenWTauTauVisiblePt->at(iTauT),     GenWTauTauVisibleEta->at(iTauT),   GenWTauTauVisiblePhi->at(iTauT), 0 );
	 if( GenTau.DeltaR(RecoTau)<0.15 ) isRecoTauPrompt_ = true;
       }
       //
       /**/
       //Check RecoMu  ( Z )
       for( unsigned int iMuT=0; iMuT<GenZMuPt->size(); iMuT++){
	 GenMu.SetPtEtaPhiM( GenZMuPt->at(iMuT),     GenZMuEta->at(iMuT),   GenZMuPhi->at(iMuT), 0 );
	 if( GenMu.DeltaR(RecoMu)<0.15 ) isRecoMuPrompt_ = true;
       }
       //Check RecoTau ( Z )
       for( unsigned int iTauT=0; iTauT<GenZTauPt->size(); iTauT++){
	 if( GenZTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
	 GenTau.SetPtEtaPhiM( GenZTauTauVisiblePt->at(iTauT),     GenZTauTauVisibleEta->at(iTauT),   GenZTauTauVisiblePhi->at(iTauT), 0 );
	 if( GenTau.DeltaR(RecoTau)<0.15 ) isRecoTauPrompt_ = true;
       }
       /**/
       //
       /*
       //Check RecoMu  (LQ3->Tau)
       for( unsigned int iMuT=0; iMuT<GenLQTauMuonPt->size(); iMuT++){
	 GenMu.SetPtEtaPhiM( GenLQTauMuonPt->at(iMuT),     GenLQTauMuonEta->at(iMuT),   GenLQTauMuonPhi->at(iMuT), 0 );
	 if( GenMu.DeltaR(RecoMu)<0.15 ) isRecoMuPrompt_ = true;
       }
       //Check RecoTau (LQ3->Tau)
       for( unsigned int iTauT=0; iTauT<GenLQTauTauPt->size(); iTauT++){
	 if( GenLQTauTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
	 GenTau.SetPtEtaPhiM( GenLQTauTauTauVisiblePt->at(iTauT),     GenLQTauTauTauVisibleEta->at(iTauT),   GenLQTauTauTauVisiblePhi->at(iTauT), 0 );
	 if( GenTau.DeltaR(RecoTau)<0.15 ) isRecoTauPrompt_ = true;
       }
       */
       //
       //-----------------------------------
       // Muon Scale Factors
       InitMuonFunctionParameters();
       InitMuonSFs(ltemMuTau[0]);
       //-----------------------------------
       // Tau Scale Factors
       InitTauSFs();
       /*
       tauPRdR3B_SF    = 0.981;   tauPRdR3B_SFerr = 0.028;
       tauPRdR2B_SF    = 0.960;   tauPRdR2B_SFerr = 0.092;
       tauPRdR1B_SF    = 0.960;   tauPRdR1B_SFerr = 0.092;
       tauPRdR3E_SF    = 1.097;   tauPRdR3E_SFerr = 0.083;
       tauPRdR2E_SF    = 1.097;   tauPRdR2E_SFerr = 0.083;
       tauPRdR1E_SF    = 1.097;   tauPRdR1E_SFerr = 0.083;
       //-------
       //tauFRdR3B_SF    = 1.083;   tauFRdR3B_SFerr = 0.063;
       //tauFRdR2B_SF    = 1.119;   tauFRdR2B_SFerr = 0.066;
       //tauFRdR1B_SF    = 1.261;   tauFRdR1B_SFerr = 0.111;
       //tauFRdR3B_SF    = 1.000;   tauFRdR3B_SFerr = 0.022;//for TauPt>32.5
       //tauFRdR2B_SF    = 1.000;   tauFRdR2B_SFerr = 0.106;
       //tauFRdR1B_SF    = 1.000;   tauFRdR1B_SFerr = 0.177;
       tauFRdR3B_SF    = 1.048;   tauFRdR3B_SFerr = 0.042;//for TauPt>35 7-Aug-2013
       tauFRdR2B_SF    = 1.140;   tauFRdR2B_SFerr = 0.175;
       tauFRdR1B_SF    = 1.195;   tauFRdR1B_SFerr = 0.197;
       // fake rates in endcap not updated..
       //tauFRdR3E_SF    = 1.079;   tauFRdR3E_SFerr = 0.077;
       tauFRdR3E_SF    = 1.000;   tauFRdR3E_SFerr = 0.065;//for TauPt>32.5
       tauFRdR2E_SF    = 1.101;   tauFRdR2E_SFerr = 0.114;
       tauFRdR1E_SF    = 1.117;   tauFRdR1E_SFerr = 0.173;
       */
       //-----------------------------------
       //
       double LTEM_ScaleFactor_=1;
       if( !isData  ){//for MC: apply SF dependent event weights to TT box based on Gen info, set SFs back to one
	 LTEM_ScaleFactor_=LTEM_ScaleFactor( ltemMuTau[0], isRecoMuPrompt_, ltemMuTau[1], isRecoTauPrompt_ );
	 //
	 ResetAllSFs();
       }
       //
       //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

       //cout<<"muPR: "<<muPR(ltemMuTau[0])<<"   Pt/|Eta|: "<<MuonPt->at(ltemMuTau[0])<<"/"<<fabs(MuonEta->at(ltemMuTau[0]))<<endl;
       //
       muPRhisto->Fill(muPR(ltemMuTau[0]));
       muFRhisto->Fill(muFR(ltemMuTau[0]));
       tauPRhisto->Fill(tauPR(ltemMuTau[1]));
       tauFRhisto->Fill(tauFR(ltemMuTau[1]));
       muPREhisto->Fill(muPRE(ltemMuTau[0]));
       muFREhisto->Fill(muFRE(ltemMuTau[0]));
       tauPREhisto->Fill(tauPRE(ltemMuTau[1]));
       tauFREhisto->Fill(tauFRE(ltemMuTau[1]));


       // Weight Histos
       Triggerhisto->Fill( usedTrigger_ );
       AppliedPileUpWeightshisto->Fill( safePileupWeights_ );
       AppliedTotalWeightshisto->Fill( w );


       bool isMuTight_       = false;
       bool isTauTight_      = false;
       //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       // Check whether LTEM mu & tau are Loose or Tight!!
       if( muRTightCheck(ltemMuTau[0])  ) isMuTight_=true;
       if( tauRTightCheck(ltemMuTau[1]) ) isTauTight_=true;
       //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


       //--- Fill in LT reco, gen matricies
       //..........................................//MT
       if( !isMuTight_ && !isTauTight_ ) LTmatrixhisto->Fill( 1, w ); //LL
       if( !isMuTight_ &&  isTauTight_ ) LTmatrixhisto->Fill( 2, w ); //LT
       if(  isMuTight_ && !isTauTight_ ) LTmatrixhisto->Fill( 3, w ); //TL
       if(  isMuTight_ &&  isTauTight_ ) LTmatrixhisto->Fill( 4, w ); //TT
       //
       if( !isRecoMuPrompt_ && !isRecoTauPrompt_ ) AllGenhisto->Fill( 1, w );//FF
       if( !isRecoMuPrompt_ &&  isRecoTauPrompt_ ) AllGenhisto->Fill( 2, w );//FP
       if(  isRecoMuPrompt_ && !isRecoTauPrompt_ ) AllGenhisto->Fill( 3, w );//PF
       if(  isRecoMuPrompt_ &&  isRecoTauPrompt_ ) AllGenhisto->Fill( 4, w );//PP
       //
       if(  isMuTight_ &&  isTauTight_ ){
	 if( !isRecoMuPrompt_ && !isRecoTauPrompt_ ) TTGenhisto->Fill( 1, w );//FF
	 if( !isRecoMuPrompt_ &&  isRecoTauPrompt_ ) TTGenhisto->Fill( 2, w );//FP
	 if(  isRecoMuPrompt_ && !isRecoTauPrompt_ ) TTGenhisto->Fill( 3, w );//PF
	 if(  isRecoMuPrompt_ &&  isRecoTauPrompt_ ) TTGenhisto->Fill( 4, w );//PP
       }
       //
       if(  isMuTight_ &&  isTauTight_ ){
	 if( !isRecoMuPrompt_ && !isRecoTauPrompt_ ) TTGenWithSFhisto->Fill( 1, w*LTEM_ScaleFactor_ );//FF
	 if( !isRecoMuPrompt_ &&  isRecoTauPrompt_ ) TTGenWithSFhisto->Fill( 2, w*LTEM_ScaleFactor_ );//FP
	 if(  isRecoMuPrompt_ && !isRecoTauPrompt_ ) TTGenWithSFhisto->Fill( 3, w*LTEM_ScaleFactor_ );//PF
	 if(  isRecoMuPrompt_ &&  isRecoTauPrompt_ ) TTGenWithSFhisto->Fill( 4, w*LTEM_ScaleFactor_ );//PP
       }
       //
       

       //
       //...........................................................................// Fake Background Estimation - Begin
       double DoubleFakeWeight_=0;
       double SingleFakeMuWeight_=0;
       double SingleFakeTauWeight_=0;
       double TTDoubleFake_=0;
       double TTSingleMuFake_=0;
       double TTSingleTauFake_=0;
       //
       // Mean
       muPR_  = muPR(ltemMuTau[0]);
       muFR_  = muFR(ltemMuTau[0]);
       tauPR_ = tauPR(ltemMuTau[1]);
       tauFR_ = tauFR(ltemMuTau[1]);
       DoubleFakeWeight_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] );
       SingleFakeMuWeight_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] );
       SingleFakeTauWeight_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] );
       TTDoubleFake_   = DoubleFakeWeight_*muFR_*tauFR_;
       TTSingleMuFake_ = SingleFakeMuWeight_*muFR_*tauPR_;
       TTSingleTauFake_= SingleFakeTauWeight_*muPR_*tauFR_;
       FPmatrixhisto->Fill( 1, DoubleFakeWeight_*w    );//FF
       FPmatrixhisto->Fill( 2, SingleFakeMuWeight_*w  );//FP
       FPmatrixhisto->Fill( 3, SingleFakeTauWeight_*w );//PF
       TTBackgroundhisto->Fill( 1, TTDoubleFake_*w    );//DoubleFake    in TT
       TTBackgroundhisto->Fill( 2, TTSingleMuFake_*w  );//SingleFakeMu  in TT
       TTBackgroundhisto->Fill( 3, TTSingleTauFake_*w );//SingleFakeTau in TT
       if(  isMuTight_ &&  isTauTight_ ) TTBackgroundhisto->Fill( 4, w );
       //
       // Mu Prompt Rate Up
       muPR_  = muPR(ltemMuTau[0])+muPRE(ltemMuTau[0]);
       muFR_  = muFR(ltemMuTau[0]);
       tauPR_ = tauPR(ltemMuTau[1]);
       tauFR_ = tauFR(ltemMuTau[1]);
       DoubleFakeWeight_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] );
       SingleFakeMuWeight_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] );
       SingleFakeTauWeight_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] );
       TTDoubleFake_   = DoubleFakeWeight_*muFR_*tauFR_;
       TTSingleMuFake_ = SingleFakeMuWeight_*muFR_*tauPR_;
       TTSingleTauFake_= SingleFakeTauWeight_*muPR_*tauFR_;
       FPmatrixMuPRUhisto->Fill( 1, DoubleFakeWeight_*w    );//FF
       FPmatrixMuPRUhisto->Fill( 2, SingleFakeMuWeight_*w  );//FP
       FPmatrixMuPRUhisto->Fill( 3, SingleFakeTauWeight_*w );//PF
       TTBackgroundMuPRUhisto->Fill( 1, TTDoubleFake_*w    );//DoubleFake    in TT
       TTBackgroundMuPRUhisto->Fill( 2, TTSingleMuFake_*w  );//SingleFakeMu  in TT
       TTBackgroundMuPRUhisto->Fill( 3, TTSingleTauFake_*w );//SingleFakeTau in TT
       if(  isMuTight_ &&  isTauTight_ ) TTBackgroundMuPRUhisto->Fill( 4, w );
       //
       // Mu Prompt Rate Down
       muPR_  = muPR(ltemMuTau[0])-muPRE(ltemMuTau[0]);
       muFR_  = muFR(ltemMuTau[0]);
       tauPR_ = tauPR(ltemMuTau[1]);
       tauFR_ = tauFR(ltemMuTau[1]);
       DoubleFakeWeight_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] );
       SingleFakeMuWeight_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] );
       SingleFakeTauWeight_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] );
       TTDoubleFake_   = DoubleFakeWeight_*muFR_*tauFR_;
       TTSingleMuFake_ = SingleFakeMuWeight_*muFR_*tauPR_;
       TTSingleTauFake_= SingleFakeTauWeight_*muPR_*tauFR_;
       FPmatrixMuPRDhisto->Fill( 1, DoubleFakeWeight_*w    );//FF
       FPmatrixMuPRDhisto->Fill( 2, SingleFakeMuWeight_*w  );//FP
       FPmatrixMuPRDhisto->Fill( 3, SingleFakeTauWeight_*w );//PF
       TTBackgroundMuPRDhisto->Fill( 1, TTDoubleFake_*w    );//DoubleFake    in TT
       TTBackgroundMuPRDhisto->Fill( 2, TTSingleMuFake_*w  );//SingleFakeMu  in TT
       TTBackgroundMuPRDhisto->Fill( 3, TTSingleTauFake_*w );//SingleFakeTau in TT
       if(  isMuTight_ &&  isTauTight_ ) TTBackgroundMuPRDhisto->Fill( 4, w );
       //
       // Mu Fake Rate Up
       muPR_  = muPR(ltemMuTau[0]);
       muFR_  = muFR(ltemMuTau[0])+muFRE(ltemMuTau[0]);
       tauPR_ = tauPR(ltemMuTau[1]);
       tauFR_ = tauFR(ltemMuTau[1]);
       DoubleFakeWeight_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] );
       SingleFakeMuWeight_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] );
       SingleFakeTauWeight_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] );
       TTDoubleFake_   = DoubleFakeWeight_*muFR_*tauFR_;
       TTSingleMuFake_ = SingleFakeMuWeight_*muFR_*tauPR_;
       TTSingleTauFake_= SingleFakeTauWeight_*muPR_*tauFR_;
       FPmatrixMuFRUhisto->Fill( 1, DoubleFakeWeight_*w    );//FF
       FPmatrixMuFRUhisto->Fill( 2, SingleFakeMuWeight_*w  );//FP
       FPmatrixMuFRUhisto->Fill( 3, SingleFakeTauWeight_*w );//PF
       TTBackgroundMuFRUhisto->Fill( 1, TTDoubleFake_*w    );//DoubleFake    in TT
       TTBackgroundMuFRUhisto->Fill( 2, TTSingleMuFake_*w  );//SingleFakeMu  in TT
       TTBackgroundMuFRUhisto->Fill( 3, TTSingleTauFake_*w );//SingleFakeTau in TT
       if(  isMuTight_ &&  isTauTight_ ) TTBackgroundMuFRUhisto->Fill( 4, w );
       //
       // Mu Fake Rate Down
       muPR_  = muPR(ltemMuTau[0]);
       muFR_  = muFR(ltemMuTau[0])-muFRE(ltemMuTau[0]);
       tauPR_ = tauPR(ltemMuTau[1]);
       tauFR_ = tauFR(ltemMuTau[1]);
       DoubleFakeWeight_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] );
       SingleFakeMuWeight_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] );
       SingleFakeTauWeight_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] );
       TTDoubleFake_   = DoubleFakeWeight_*muFR_*tauFR_;
       TTSingleMuFake_ = SingleFakeMuWeight_*muFR_*tauPR_;
       TTSingleTauFake_= SingleFakeTauWeight_*muPR_*tauFR_;
       FPmatrixMuFRDhisto->Fill( 1, DoubleFakeWeight_*w    );//FF
       FPmatrixMuFRDhisto->Fill( 2, SingleFakeMuWeight_*w  );//FP
       FPmatrixMuFRDhisto->Fill( 3, SingleFakeTauWeight_*w );//PF
       TTBackgroundMuFRDhisto->Fill( 1, TTDoubleFake_*w    );//DoubleFake    in TT
       TTBackgroundMuFRDhisto->Fill( 2, TTSingleMuFake_*w  );//SingleFakeMu  in TT
       TTBackgroundMuFRDhisto->Fill( 3, TTSingleTauFake_*w );//SingleFakeTau in TT
       if(  isMuTight_ &&  isTauTight_ ) TTBackgroundMuFRDhisto->Fill( 4, w );
       //
       // Tau Prompt Rate Up
       muPR_  = muPR(ltemMuTau[0]);
       muFR_  = muFR(ltemMuTau[0]);
       tauPR_ = tauPR(ltemMuTau[1])+tauPRE(ltemMuTau[1]);
       tauFR_ = tauFR(ltemMuTau[1]);
       DoubleFakeWeight_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] );
       SingleFakeMuWeight_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] );
       SingleFakeTauWeight_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] );
       TTDoubleFake_   = DoubleFakeWeight_*muFR_*tauFR_;
       TTSingleMuFake_ = SingleFakeMuWeight_*muFR_*tauPR_;
       TTSingleTauFake_= SingleFakeTauWeight_*muPR_*tauFR_;
       FPmatrixTauPRUhisto->Fill( 1, DoubleFakeWeight_*w    );//FF
       FPmatrixTauPRUhisto->Fill( 2, SingleFakeMuWeight_*w  );//FP
       FPmatrixTauPRUhisto->Fill( 3, SingleFakeTauWeight_*w );//PF
       TTBackgroundTauPRUhisto->Fill( 1, TTDoubleFake_*w    );//DoubleFake    in TT
       TTBackgroundTauPRUhisto->Fill( 2, TTSingleMuFake_*w  );//SingleFakeMu  in TT
       TTBackgroundTauPRUhisto->Fill( 3, TTSingleTauFake_*w );//SingleFakeTau in TT
       if(  isMuTight_ &&  isTauTight_ ) TTBackgroundTauPRUhisto->Fill( 4, w );
       //
       // Tau Prompt Rate Down
       muPR_  = muPR(ltemMuTau[0]);
       muFR_  = muFR(ltemMuTau[0]);
       tauPR_ = tauPR(ltemMuTau[1])-tauPRE(ltemMuTau[1]);
       tauFR_ = tauFR(ltemMuTau[1]);
       DoubleFakeWeight_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] );
       SingleFakeMuWeight_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] );
       SingleFakeTauWeight_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] );
       TTDoubleFake_   = DoubleFakeWeight_*muFR_*tauFR_;
       TTSingleMuFake_ = SingleFakeMuWeight_*muFR_*tauPR_;
       TTSingleTauFake_= SingleFakeTauWeight_*muPR_*tauFR_;
       FPmatrixTauPRDhisto->Fill( 1, DoubleFakeWeight_*w    );//FF
       FPmatrixTauPRDhisto->Fill( 2, SingleFakeMuWeight_*w  );//FP
       FPmatrixTauPRDhisto->Fill( 3, SingleFakeTauWeight_*w );//PF
       TTBackgroundTauPRDhisto->Fill( 1, TTDoubleFake_*w    );//DoubleFake    in TT
       TTBackgroundTauPRDhisto->Fill( 2, TTSingleMuFake_*w  );//SingleFakeMu  in TT
       TTBackgroundTauPRDhisto->Fill( 3, TTSingleTauFake_*w );//SingleFakeTau in TT
       if(  isMuTight_ &&  isTauTight_ ) TTBackgroundTauPRDhisto->Fill( 4, w );
       //
       // Tau Fake Rate Up
       muPR_  = muPR(ltemMuTau[0]);
       muFR_  = muFR(ltemMuTau[0]);
       tauPR_ = tauPR(ltemMuTau[1]);
       tauFR_ = tauFR(ltemMuTau[1])+tauFRE(ltemMuTau[1]);
       DoubleFakeWeight_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] );
       SingleFakeMuWeight_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] );
       SingleFakeTauWeight_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] );
       TTDoubleFake_   = DoubleFakeWeight_*muFR_*tauFR_;
       TTSingleMuFake_ = SingleFakeMuWeight_*muFR_*tauPR_;
       TTSingleTauFake_= SingleFakeTauWeight_*muPR_*tauFR_;
       FPmatrixTauFRUhisto->Fill( 1, DoubleFakeWeight_*w    );//FF
       FPmatrixTauFRUhisto->Fill( 2, SingleFakeMuWeight_*w  );//FP
       FPmatrixTauFRUhisto->Fill( 3, SingleFakeTauWeight_*w );//PF
       TTBackgroundTauFRUhisto->Fill( 1, TTDoubleFake_*w    );//DoubleFake    in TT
       TTBackgroundTauFRUhisto->Fill( 2, TTSingleMuFake_*w  );//SingleFakeMu  in TT
       TTBackgroundTauFRUhisto->Fill( 3, TTSingleTauFake_*w );//SingleFakeTau in TT
       if(  isMuTight_ &&  isTauTight_ ) TTBackgroundTauFRUhisto->Fill( 4, w );
       //
       // Tau Fake Rate Down
       muPR_  = muPR(ltemMuTau[0]);
       muFR_  = muFR(ltemMuTau[0]);
       tauPR_ = tauPR(ltemMuTau[1]);
       tauFR_ = tauFR(ltemMuTau[1])-tauFRE(ltemMuTau[1]);
       DoubleFakeWeight_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] );
       SingleFakeMuWeight_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] );
       SingleFakeTauWeight_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] );
       TTDoubleFake_   = DoubleFakeWeight_*muFR_*tauFR_;
       TTSingleMuFake_ = SingleFakeMuWeight_*muFR_*tauPR_;
       TTSingleTauFake_= SingleFakeTauWeight_*muPR_*tauFR_;
       FPmatrixTauFRDhisto->Fill( 1, DoubleFakeWeight_*w    );//FF
       FPmatrixTauFRDhisto->Fill( 2, SingleFakeMuWeight_*w  );//FP
       FPmatrixTauFRDhisto->Fill( 3, SingleFakeTauWeight_*w );//PF
       TTBackgroundTauFRDhisto->Fill( 1, TTDoubleFake_*w    );//DoubleFake    in TT
       TTBackgroundTauFRDhisto->Fill( 2, TTSingleMuFake_*w  );//SingleFakeMu  in TT
       TTBackgroundTauFRDhisto->Fill( 3, TTSingleTauFake_*w );//SingleFakeTau in TT
       if(  isMuTight_ &&  isTauTight_ ) TTBackgroundTauFRDhisto->Fill( 4, w );

       
       //
       //...........................................................................// Fake Background Estimation - End


       // FILL OUT THE REMAINING HISTOGRAMS ONLY FOR TT EVENTS!
       if( !isMuTight_ || !isTauTight_ ) continue;

       //Apply tau prompt and fake rate corrections!!
       w=w*LTEM_ScaleFactor_;

       LTEMMuJetDeltaRminhisto->Fill( muJetDeltaRmin(ltemMuTau[0]), w );
       LTEMTauJetDeltaRminhisto->Fill( tauJetDeltaRmin(ltemMuTau[1]), w );
       LTEMMuPthisto->Fill( MuonPt->at(ltemMuTau[0]), w );
       LTEMTauPthisto->Fill( HPSTauPt->at(ltemMuTau[1]), w );
       LTEMMuEtahisto->Fill( MuonEta->at(ltemMuTau[0]), w );
       LTEMTauEtahisto->Fill( HPSTauEta->at(ltemMuTau[1]), w );
       //LTEMAbsTauEtahisto->Fill( fabs(HPSTauEta->at(ltemMuTau[1])), w );
       LTEMMuPhihisto->Fill( MuonPhi->at(ltemMuTau[0]), w );
       LTEMTauPhihisto->Fill( HPSTauPhi->at(ltemMuTau[1]), w );
       TLorentzVector LTEMMuVector, LTEMTauVector;
       LTEMMuVector.SetPtEtaPhiM(  MuonPt->at(ltemMuTau[0]),    MuonEta->at(ltemMuTau[0]),   MuonPhi->at(ltemMuTau[0]),   0 );
       LTEMTauVector.SetPtEtaPhiM( HPSTauPt->at(ltemMuTau[1]),  HPSTauEta->at(ltemMuTau[1]), HPSTauPhi->at(ltemMuTau[1]), 0 );
       LTEMMuTauDeltaRhisto->Fill( LTEMMuVector.DeltaR(LTEMTauVector), w );
       LTEMMuTauDeltaPhihisto->Fill( LTEMMuVector.DeltaPhi(LTEMTauVector), w );
       LTEMMuTauMasshisto->Fill( (LTEMMuVector+LTEMTauVector).M(), w );
       //
       // For 2D optimization
       double nLepjet=(MuCounter()+ElCounter()+TauCounter()+JetCounter());
       double st=ST();
       LTEM2DMuPtTauPthisto->Fill( MuonPt->at(ltemMuTau[0]), HPSTauPt->at(ltemMuTau[1]), w);
       LTEM2DTauPtSThisto->Fill( HPSTauPt->at(ltemMuTau[1]), st, w);
       LTEM2DTauPtNLepjethisto->Fill( HPSTauPt->at(ltemMuTau[1]), nLepjet, w);
       LTEM2DMuPtNLepjethisto->Fill( MuonPt->at(ltemMuTau[0]), nLepjet, w);
       LTEM2DMuPtSThisto->Fill( MuonPt->at(ltemMuTau[0]), st, w);
       LTEM2DNLepjetSThisto->Fill( nLepjet, st, w);
       LTEM2DNJetSThisto->Fill( JetCounter(), st, w);


       PZetahisto->Fill( pZeta, w );

       
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
       //for(unsigned int iVertexR=0; iVertexR<VertexX->size(); iVertexR++){
       //if( vertexRCheck(iVertexR) ) VertexN++;
       //}
       //VertexNhisto->Fill((double)VertexN, w);
       VertexNhisto->Fill((double)(VertexX->size()), w);
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
       METhisto->Fill(PFMETType01XYCor->at(0), w );
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
   muPRhisto->Write();
   muFRhisto->Write();
   tauPRhisto->Write();
   tauFRhisto->Write();
   muPREhisto->Write();
   muFREhisto->Write();
   tauPREhisto->Write();
   tauFREhisto->Write();
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
   SJThisto->Write();
   LeadMuTauDeltaRhisto->Write();
   PZetahisto->Write();
   //
   METtauDeltaPhihisto->Write();
   METmuDeltaPhihisto ->Write();
   METelDeltaPhihisto ->Write();
   //
   LTEMMuJetDeltaRminhisto->Write();
   LTEMTauJetDeltaRminhisto->Write();
   LTEMMuPthisto->Write();
   LTEMTauPthisto->Write();
   LTEMMuEtahisto->Write();
   LTEMTauEtahisto->Write();
   LTEMMuPhihisto->Write();
   LTEMTauPhihisto->Write();
   LTEMMuTauDeltaRhisto->Write();
   LTEMMuTauDeltaPhihisto->Write();
   LTEMMuTauMasshisto->Write();
   //
   LTEM2DMuPtTauPthisto->Write();
   LTEM2DTauPtSThisto->Write();
   LTEM2DTauPtNLepjethisto->Write();
   LTEM2DMuPtNLepjethisto->Write();
   LTEM2DMuPtSThisto->Write();
   LTEM2DNLepjetSThisto->Write();
   LTEM2DNJetSThisto->Write();
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
   LTmatrixhisto->Write();
   //
   FPmatrixhisto->Write();
   FPmatrixMuPRUhisto->Write();
   FPmatrixMuPRDhisto->Write();
   FPmatrixMuFRUhisto->Write();
   FPmatrixMuFRDhisto->Write();
   FPmatrixTauPRUhisto->Write();
   FPmatrixTauPRDhisto->Write();
   FPmatrixTauFRUhisto->Write();
   FPmatrixTauFRDhisto->Write();
   //
   TTBackgroundhisto->Write();
   TTBackgroundMuPRUhisto->Write();
   TTBackgroundMuPRDhisto->Write();
   TTBackgroundMuFRUhisto->Write();
   TTBackgroundMuFRDhisto->Write();
   TTBackgroundTauPRUhisto->Write();
   TTBackgroundTauPRDhisto->Write();
   TTBackgroundTauFRUhisto->Write();
   TTBackgroundTauFRDhisto->Write();
   //
   AllGenhisto->Write();
   TTGenhisto->Write();
   TTGenWithSFhisto->Write();


   //////////
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
// // //
