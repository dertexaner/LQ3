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
#include "LAC/ADDON5_LAC.C"/// TOPgen functions //fake  // <<<< NEW - changed!
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
   //MuonPtCut = 45;
   MuonEtaCut= 2.1;
   TauPtCut  = 20;
   ApplyAbsMuonIsolation=false;
   removeOverlaps=true;
   //
   LumiA=891; 
   LumiB=4428;
   LumiC=7018;
   LumiD=7248;

   //////////book histos here
   TH1D* ProvidedPileUpWeightshisto  = new TH1D("ProvidedPileUpWeightshisto","ProvidedPileUpWeightshisto",10000,0,100);//distribution of the provided PileUp-weights
   TH1D* AppliedPileUpWeightshisto   = new TH1D("AppliedPileUpWeightshisto", "AppliedPileUpWeightshisto", 10000,0,100);//distribution of the applied PileUp-weights
   TH1D* AppliedTrigEffWeightshisto  = new TH1D("AppliedTrigEffWeightshisto","AppliedTrigEffWeightshisto",10000,0,100);//distribution of the applied TrigEff-weights
   TH1D* Triggerhisto                = new TH1D("Triggerhisto",              "Triggerhisto",              10,-3.5,6.5);//distribution of triggers in the events
   TH1D* AppliedTotalWeightshisto    = new TH1D("AppliedTotalWeightshisto",  "AppliedTotalWeightshisto",  10000,0,100);//distribution of the applied Total-weights
   //
   TH1D* LeadMuTauDeltaRhisto = new TH1D("LeadMuTauDeltaRhisto","LeadMuTauDeltaRhisto",5000,0,50);
   TH1D* SThisto              = new TH1D("SThisto", "SThisto", 5000,0,5000);
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
   TH2D* LooseMuonPt2Dhisto  = new TH2D("LooseMuonPt2Dhisto", "LooseMuonPt2Dhisto", 2000,0,2000,100,0,5);
   TH2D* TightMuonPt2Dhisto  = new TH2D("TightMuonPt2Dhisto", "TightMuonPt2Dhisto", 2000,0,2000,100,0,5);
   TH2D* LooseMuonEta2Dhisto = new TH2D("LooseMuonEta2Dhisto","LooseMuonEta2Dhisto",500,-2.5,2.5,100,0,5);
   TH2D* TightMuonEta2Dhisto = new TH2D("TightMuonEta2Dhisto","TightMuonEta2Dhisto",500,-2.5,2.5,100,0,5);
   TH2D* LooseMuonPhi2Dhisto = new TH2D("LooseMuonPhi2Dhisto","LooseMuonPhi2Dhisto",700,-3.5,3.5,100,0,5);
   TH2D* TightMuonPhi2Dhisto = new TH2D("TightMuonPhi2Dhisto","TightMuonPhi2Dhisto",700,-3.5,3.5,100,0,5);
   TH2D* LooseMuonJetDeltaRmin2Dhisto = new TH2D("LooseMuonJetDeltaRmin2Dhisto","LooseMuonJetDeltaRmin2Dhisto",100,0,5,100,0,5);
   TH2D* TightMuonJetDeltaRmin2Dhisto = new TH2D("TightMuonJetDeltaRmin2Dhisto","TightMuonJetDeltaRmin2Dhisto",100,0,5,100,0,5);
   TH2D* LooseMuonMuDeltaR2Dhisto = new TH2D("LooseMuonMuDeltaR2Dhisto","LooseMuonMuDeltaR2Dhisto",100,0,5,100,0,5);
   TH2D* TightMuonMuDeltaR2Dhisto = new TH2D("TightMuonMuDeltaR2Dhisto","TightMuonMuDeltaR2Dhisto",100,0,5,100,0,5);
   TH2D* LooseMuonMuInvMass2Dhisto = new TH2D("LooseMuonMuInvMass2Dhisto","LooseMuonMuInvMass2Dhisto",200,0,200,100,0,5);
   TH2D* TightMuonMuInvMass2Dhisto = new TH2D("TightMuonMuInvMass2Dhisto","TightMuonMuInvMass2Dhisto",200,0,200,100,0,5);
   //
   TH2D* LooseFakeMuonPt2Dhisto  = new TH2D("LooseFakeMuonPt2Dhisto", "LooseFakeMuonPt2Dhisto", 2000,0,2000,100,0,5);
   TH2D* TightFakeMuonPt2Dhisto  = new TH2D("TightFakeMuonPt2Dhisto", "TightFakeMuonPt2Dhisto", 2000,0,2000,100,0,5);
   TH2D* LooseFakeMuonEta2Dhisto = new TH2D("LooseFakeMuonEta2Dhisto","LooseFakeMuonEta2Dhisto",500,-2.5,2.5,100,0,5);
   TH2D* TightFakeMuonEta2Dhisto = new TH2D("TightFakeMuonEta2Dhisto","TightFakeMuonEta2Dhisto",500,-2.5,2.5,100,0,5);
   TH2D* LooseFakeMuonPhi2Dhisto = new TH2D("LooseFakeMuonPhi2Dhisto","LooseFakeMuonPhi2Dhisto",700,-3.5,3.5,100,0,5);
   TH2D* TightFakeMuonPhi2Dhisto = new TH2D("TightFakeMuonPhi2Dhisto","TightFakeMuonPhi2Dhisto",700,-3.5,3.5,100,0,5);
   TH2D* LooseFakeMuonJetDeltaRmin2Dhisto = new TH2D("LooseFakeMuonJetDeltaRmin2Dhisto","LooseFakeMuonJetDeltaRmin2Dhisto",80,0,4,100,0,5);
   TH2D* TightFakeMuonJetDeltaRmin2Dhisto = new TH2D("TightFakeMuonJetDeltaRmin2Dhisto","TightFakeMuonJetDeltaRmin2Dhisto",80,0,4,100,0,5);
   TH2D* LooseFakeMuonMuDeltaR2Dhisto = new TH2D("LooseFakeMuonMuDeltaR2Dhisto","LooseFakeMuonMuDeltaR2Dhisto",80,0,4,100,0,5);
   TH2D* TightFakeMuonMuDeltaR2Dhisto = new TH2D("TightFakeMuonMuDeltaR2Dhisto","TightFakeMuonMuDeltaR2Dhisto",80,0,4,100,0,5);
   TH2D* LooseFakeMuonMuInvMass2Dhisto = new TH2D("LooseFakeMuonMuInvMass2Dhisto","LooseFakeMuonMuInvMass2Dhisto",200,0,200,100,0,5);
   TH2D* TightFakeMuonMuInvMass2Dhisto = new TH2D("TightFakeMuonMuInvMass2Dhisto","TightFakeMuonMuInvMass2Dhisto",200,0,200,100,0,5);
   //
   TH2D* LoosePromptMuonPt2Dhisto  = new TH2D("LoosePromptMuonPt2Dhisto", "LoosePromptMuonPt2Dhisto", 2000,0,2000,100,0,5);
   TH2D* TightPromptMuonPt2Dhisto  = new TH2D("TightPromptMuonPt2Dhisto", "TightPromptMuonPt2Dhisto", 2000,0,2000,100,0,5);
   TH2D* LoosePromptMuonEta2Dhisto = new TH2D("LoosePromptMuonEta2Dhisto","LoosePromptMuonEta2Dhisto",500,-2.5,2.5,100,0,5);
   TH2D* TightPromptMuonEta2Dhisto = new TH2D("TightPromptMuonEta2Dhisto","TightPromptMuonEta2Dhisto",500,-2.5,2.5,100,0,5);
   TH2D* LoosePromptMuonPhi2Dhisto = new TH2D("LoosePromptMuonPhi2Dhisto","LoosePromptMuonPhi2Dhisto",700,-3.5,3.5,100,0,5);
   TH2D* TightPromptMuonPhi2Dhisto = new TH2D("TightPromptMuonPhi2Dhisto","TightPromptMuonPhi2Dhisto",700,-3.5,3.5,100,0,5);
   TH2D* LoosePromptMuonJetDeltaRmin2Dhisto = new TH2D("LoosePromptMuonJetDeltaRmin2Dhisto","LoosePromptMuonJetDeltaRmin2Dhisto",80,0,4,100,0,5);
   TH2D* TightPromptMuonJetDeltaRmin2Dhisto = new TH2D("TightPromptMuonJetDeltaRmin2Dhisto","TightPromptMuonJetDeltaRmin2Dhisto",80,0,4,100,0,5);
   TH2D* LoosePromptMuonMuDeltaR2Dhisto = new TH2D("LoosePromptMuonMuDeltaR2Dhisto","LoosePromptMuonMuDeltaR2Dhisto",80,0,4,100,0,5);
   TH2D* TightPromptMuonMuDeltaR2Dhisto = new TH2D("TightPromptMuonMuDeltaR2Dhisto","TightPromptMuonMuDeltaR2Dhisto",80,0,4,100,0,5);
   TH2D* LoosePromptMuonMuInvMass2Dhisto = new TH2D("LoosePromptMuonMuInvMass2Dhisto","LoosePromptMuonMuInvMass2Dhisto",200,0,200,100,0,5);
   TH2D* TightPromptMuonMuInvMass2Dhisto = new TH2D("TightPromptMuonMuInvMass2Dhisto","TightPromptMuonMuInvMass2Dhisto",200,0,200,100,0,5);
   /*
   TH1D* LooseMuonPthisto  = new TH1D("LooseMuonPthisto", "LooseMuonPthisto", 2000,0,2000);
   TH1D* TightMuonPthisto  = new TH1D("TightMuonPthisto", "TightMuonPthisto", 2000,0,2000);
   TH1D* LooseMuonEtahisto = new TH1D("LooseMuonEtahisto","LooseMuonEtahisto",500,-2.5,2.5);
   TH1D* TightMuonEtahisto = new TH1D("TightMuonEtahisto","TightMuonEtahisto",500,-2.5,2.5);
   TH1D* LooseMuonPhihisto = new TH1D("LooseMuonPhihisto","LooseMuonPhihisto",700,-3.5,3.5);
   TH1D* TightMuonPhihisto = new TH1D("TightMuonPhihisto","TightMuonPhihisto",700,-3.5,3.5);
   TH1D* LooseMuonJetDeltaRminhisto = new TH1D("LooseMuonJetDeltaRminhisto","LooseMuonJetDeltaRminhisto",80,0,4);
   TH1D* TightMuonJetDeltaRminhisto = new TH1D("TightMuonJetDeltaRminhisto","TightMuonJetDeltaRminhisto",80,0,4);
   TH1D* LooseMuonMuDeltaRhisto = new TH1D("LooseMuonMuDeltaRhisto","LooseMuonMuDeltaRhisto",80,0,4);
   TH1D* TightMuonMuDeltaRhisto = new TH1D("TightMuonMuDeltaRhisto","TightMuonMuDeltaRhisto",80,0,4);
   TH1D* LooseMuonMuInvMasshisto = new TH1D("LooseMuonMuInvMasshisto","LooseMuonMuInvMasshisto",200,0,200);
   TH1D* TightMuonMuInvMasshisto = new TH1D("TightMuonMuInvMasshisto","TightMuonMuInvMasshisto",200,0,200);
   //
   TH1D* LooseFakeMuonPthisto  = new TH1D("LooseFakeMuonPthisto", "LooseFakeMuonPthisto", 2000,0,2000);
   TH1D* TightFakeMuonPthisto  = new TH1D("TightFakeMuonPthisto", "TightFakeMuonPthisto", 2000,0,2000);
   TH1D* LooseFakeMuonEtahisto = new TH1D("LooseFakeMuonEtahisto","LooseFakeMuonEtahisto",500,-2.5,2.5);
   TH1D* TightFakeMuonEtahisto = new TH1D("TightFakeMuonEtahisto","TightFakeMuonEtahisto",500,-2.5,2.5);
   TH1D* LooseFakeMuonPhihisto = new TH1D("LooseFakeMuonPhihisto","LooseFakeMuonPhihisto",700,-3.5,3.5);
   TH1D* TightFakeMuonPhihisto = new TH1D("TightFakeMuonPhihisto","TightFakeMuonPhihisto",700,-3.5,3.5);
   TH1D* LooseFakeMuonJetDeltaRminhisto = new TH1D("LooseFakeMuonJetDeltaRminhisto","LooseFakeMuonJetDeltaRminhisto",80,0,4);
   TH1D* TightFakeMuonJetDeltaRminhisto = new TH1D("TightFakeMuonJetDeltaRminhisto","TightFakeMuonJetDeltaRminhisto",80,0,4);
   TH1D* LooseFakeMuonMuDeltaRhisto = new TH1D("LooseFakeMuonMuDeltaRhisto","LooseFakeMuonMuDeltaRhisto",80,0,4);
   TH1D* TightFakeMuonMuDeltaRhisto = new TH1D("TightFakeMuonMuDeltaRhisto","TightFakeMuonMuDeltaRhisto",80,0,4);
   TH1D* LooseFakeMuonMuInvMasshisto = new TH1D("LooseFakeMuonMuInvMasshisto","LooseFakeMuonMuInvMasshisto",200,0,200);
   TH1D* TightFakeMuonMuInvMasshisto = new TH1D("TightFakeMuonMuInvMasshisto","TightFakeMuonMuInvMasshisto",200,0,200);
   //
   TH1D* LoosePromptMuonPthisto  = new TH1D("LoosePromptMuonPthisto", "LoosePromptMuonPthisto", 2000,0,2000);
   TH1D* TightPromptMuonPthisto  = new TH1D("TightPromptMuonPthisto", "TightPromptMuonPthisto", 2000,0,2000);
   TH1D* LoosePromptMuonEtahisto = new TH1D("LoosePromptMuonEtahisto","LoosePromptMuonEtahisto",500,-2.5,2.5);
   TH1D* TightPromptMuonEtahisto = new TH1D("TightPromptMuonEtahisto","TightPromptMuonEtahisto",500,-2.5,2.5);
   TH1D* LoosePromptMuonPhihisto = new TH1D("LoosePromptMuonPhihisto","LoosePromptMuonPhihisto",700,-3.5,3.5);
   TH1D* TightPromptMuonPhihisto = new TH1D("TightPromptMuonPhihisto","TightPromptMuonPhihisto",700,-3.5,3.5);
   TH1D* LoosePromptMuonJetDeltaRminhisto = new TH1D("LoosePromptMuonJetDeltaRminhisto","LoosePromptMuonJetDeltaRminhisto",80,0,4);
   TH1D* TightPromptMuonJetDeltaRminhisto = new TH1D("TightPromptMuonJetDeltaRminhisto","TightPromptMuonJetDeltaRminhisto",80,0,4);
   TH1D* LoosePromptMuonMuDeltaRhisto = new TH1D("LoosePromptMuonMuDeltaRhisto","LoosePromptMuonMuDeltaRhisto",80,0,4);
   TH1D* TightPromptMuonMuDeltaRhisto = new TH1D("TightPromptMuonMuDeltaRhisto","TightPromptMuonMuDeltaRhisto",80,0,4);
   TH1D* LoosePromptMuonMuInvMasshisto = new TH1D("LoosePromptMuonMuInvMasshisto","LoosePromptMuonMuInvMasshisto",200,0,200);
   TH1D* TightPromptMuonMuInvMasshisto = new TH1D("TightPromptMuonMuInvMasshisto","TightPromptMuonMuInvMasshisto",200,0,200);
   */
   //

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
     //which_MuTau(ltemMuTau);


     // WEIGHTS
     // ---- pileup
     double safePileupWeights_=0;
     safePileupWeights_=safePileupWeights();
     //
     // ---- trigger
     unsigned int tagMuon=99;
     unsigned int probeMuon=99;
     int probeCharge=1;//probe charge is +1
     for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
       if( !muRisoCheck(iMuR) ) continue;
       if( MuonCharge->at(iMuR)!=probeCharge ) continue; 
       probeMuon=iMuR;
       break;//take the leading positive charge, later it will be required to have only 2 loose OS muons!
     }
     for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
       if( !muRisoCheck(iMuR) ) continue;
       if( MuonCharge->at(iMuR)!=(-probeCharge) ) continue; 
       tagMuon=iMuR;
       break;//take the leading negative charge, later it will be required to have only 2 loose OS muons!
     }
     //
     if( tagMuon==99 || probeMuon==99 ) continue; //SKIP EVENTS WITH NO OS diMUON
     //
     int usedTrigger_=-5;
     usedTrigger_ = SingleMu_passTrigger();
     // usedTrigger_ = SingleMu40_passTrigger();
     // usedTrigger_ = HLT_MuPT_eta2p1_passTrigger();
     double TriggerEfficiencyWeights_;
     // use same weights for Mu24 and Mu40 triggers
     TriggerEfficiencyWeights_=IsoMu24e2p1_Eff( MuonPt->at(tagMuon), MuonEta->at(tagMuon) );
     // TriggerEfficiencyWeights_=Mu40e2p1_ScaleFactor( 50, MuonEta->at(tagMuon) );
     // TriggerEfficiencyWeights_=Mu40e2p1_ScaleFactor( 50, MuonEta->at(tagMuon) );
     AppliedTrigEffWeightshisto->Fill( TriggerEfficiencyWeights_ );
     // ---- total = pileup x trigger
     double w = 0;
     w=safePileupWeights_*TriggerEfficiencyWeights_;
     if( isData  ) w=1;

     //  -- JSON SKIM
     int passJSON_=0;
     if( isData  ) passJSON_ = passJSON(run,ls,isData);
     if( !isData ) passJSON_ = 1;
     fillVariableWithValue("PassJSON", passJSON_ );//..............// returns 0, 1                                                                                              

     //  -- TRIGGER SKIM
     int passTrigger_=0;
     passTrigger_ = SingleMu_passTrigger(); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<this is IsoMu24 trigger
     //if(  isMu40_ ) passTrigger_ = SingleMu40_passTrigger();        //<<<<<<<<<<<<<<<<< this is Mu40 trigger 
     //if( !isMu40_ ) passTrigger_ = HLT_MuPT_eta2p1_passTrigger(); //<<<<<<<<<<<<<<<<< this is Mu24 trigger 
     fillVariableWithValue("PassTrig", passTrigger_ );//...// returns -2, -1, 0, trig                                                                                   

     //  -- EVENT FILTER SKIM
     int passEventFilter_=0;
     if( passEventFilter() ) passEventFilter_ = 1;
     fillVariableWithValue("PassEventFilter", passEventFilter_ );//.// returns 0, 1 


     //  -- DILEPTON SKIM - MuMu
     int isOfflineDilepton_=0;
     int TotalMuonCharge=0;
     int TotalMuonCount=0;
     for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
       if( !muRisoCheck(iMuR)      ) continue;
       TotalMuonCharge+=MuonCharge->at(iMuR);
       TotalMuonCount++;
     }
     if( TotalMuonCount==2 && TotalMuonCharge==0 ) isOfflineDilepton_=1;
     fillVariableWithValue("PassOfflineDilepton", isOfflineDilepton_ );// Returns 0, 1          


     // -- HLT MATCHING SKIM
     int passAllMuHLTmatching_=0;
     if(  RecoHLTdeltaRmin_SingleMuTrigger(tagMuon)<0.15              ) passAllMuHLTmatching_=1;
     //if(  isMu40_ && RecoHLTdeltaRmin_SingleMu40Trigger(tagMuon)<0.15 ) passAllMuHLTmatching_=1;
     //if( !isMu40_ && RecoHLTdeltaRmin_SingleMu24Trigger(tagMuon)<0.15 ) passAllMuHLTmatching_=1;
     fillVariableWithValue("PassAllMuHLTmatching",passAllMuHLTmatching_);


     // -- OFFLINE SKIM
     int OfflineCuts_=0;
     double muMuInvMass_=999;
     if( tagMuon!=99 && probeMuon!=99 ){
       TLorentzVector tagMuonVector, probeMuonVector;
       tagMuonVector.SetPtEtaPhiM(  MuonPt->at(tagMuon),     MuonEta->at(tagMuon),    MuonPhi->at(tagMuon),    0 );
       probeMuonVector.SetPtEtaPhiM(  MuonPt->at(probeMuon),     MuonEta->at(probeMuon),    MuonPhi->at(probeMuon),    0 );
       muMuInvMass_=(tagMuonVector+probeMuonVector).M();
     }
     //
     if( tagMuon!=99 && probeMuon!=99 
	 && muRTightCheck(tagMuon)
	 && MuCounter()==2 && ElCounter()==0 
	 && JetCounter()>0	
	 && fabs((muMuInvMass_-91.2)/91.2)<0.1 //within 10% of Zmass
	 //&& muJetDeltaRmin(tagMuon)>0.7 // not needed, tag muon with Z diMu mass is good enough
	 //&& muJetDeltaRmin(probeMuon)>0.7 //not needed, the binning is done 2D ( some-param vs mujetDeltaR )
	 && PFMETType01XYCor->at(0)<50
	 //&& fabs(MuonEta->at(probeMuon))<1.5 //BARREL
	 && fabs(MuonEta->at(probeMuon))>=1.5 //ENDCAP
	 ) OfflineCuts_=1;
     fillVariableWithValue("PassOfflineCuts", OfflineCuts_ );// returns 0, 1 

     evaluateCuts();
     //---------------------------------------------------------------------------------
     if( passedCut("PassJSON") && passedCut("PassTrig") && passedCut("PassEventFilter") && passedCut("PassOfflineDilepton") &&
	 passedCut("PassAllMuHLTmatching") && passedCut("PassOfflineCuts") ){ 

       //for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
       //if( !muRisoCheck(iMuR) ) continue;
       //cout<<"Muon Pt/Eta/Q "<< MuonPt->at(iMuR)<<" "<<MuonEta->at(iMuR)<<" "<<MuonCharge->at(iMuR)<<endl;
       //}
       //cout<<endl;

       // Weight Histos
       Triggerhisto->Fill( usedTrigger_ );
       AppliedPileUpWeightshisto->Fill( safePileupWeights_ );
       AppliedTotalWeightshisto->Fill( w );

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

       //--------------------------------------------------------------
       double muJetDeltaRmin_ = muJetDeltaRmin(probeMuon);
       TLorentzVector tagMuonVec, probeMuonVec;
       tagMuonVec.SetPtEtaPhiM(  MuonPt->at(tagMuon),     MuonEta->at(tagMuon),    MuonPhi->at(tagMuon),    0 );
       probeMuonVec.SetPtEtaPhiM( MuonPt->at(probeMuon),     MuonEta->at(probeMuon),    MuonPhi->at(probeMuon),    0 );
       double muMuInvMass=(tagMuonVec+probeMuonVec).M();
       double muMuDeltaR=tagMuonVec.DeltaR(probeMuonVec);
       LooseMuonPt2Dhisto->Fill(  MuonPt ->at(probeMuon), muJetDeltaRmin_, w );
       LooseMuonEta2Dhisto->Fill( fabs(MuonEta->at(probeMuon)), muJetDeltaRmin_, w );
       LooseMuonPhi2Dhisto->Fill( MuonPhi->at(probeMuon), muJetDeltaRmin_, w );
       LooseMuonJetDeltaRmin2Dhisto->Fill( muJetDeltaRmin_, muJetDeltaRmin_, w );
       LooseMuonMuDeltaR2Dhisto->Fill( muMuDeltaR, muJetDeltaRmin_, w );
       LooseMuonMuInvMass2Dhisto->Fill( muMuInvMass, muJetDeltaRmin_, w );
       if( muRTightCheck(probeMuon) ){
	 TightMuonPt2Dhisto->Fill(  MuonPt ->at(probeMuon), muJetDeltaRmin_, w );
	 TightMuonEta2Dhisto->Fill( fabs(MuonEta->at(probeMuon)), muJetDeltaRmin_, w );
	 TightMuonPhi2Dhisto->Fill( MuonPhi->at(probeMuon), muJetDeltaRmin_, w );
	 TightMuonJetDeltaRmin2Dhisto->Fill( muJetDeltaRmin_, muJetDeltaRmin_, w );
	 TightMuonMuDeltaR2Dhisto->Fill( muMuDeltaR, muJetDeltaRmin_, w );
	 TightMuonMuInvMass2Dhisto->Fill( muMuInvMass, muJetDeltaRmin_, w );
       }
       // Fake Muon
       if( isRecoMuonFake(probeMuon) ){
	 LooseFakeMuonPt2Dhisto->Fill(  MuonPt ->at(probeMuon), muJetDeltaRmin_, w );
	 LooseFakeMuonEta2Dhisto->Fill( fabs(MuonEta->at(probeMuon)), muJetDeltaRmin_, w );
	 LooseFakeMuonPhi2Dhisto->Fill( MuonPhi->at(probeMuon), muJetDeltaRmin_, w );
	 LooseFakeMuonJetDeltaRmin2Dhisto->Fill( muJetDeltaRmin_, muJetDeltaRmin_, w );
	 LooseFakeMuonMuDeltaR2Dhisto->Fill( muMuDeltaR, muJetDeltaRmin_, w );
	 LooseFakeMuonMuInvMass2Dhisto->Fill( muMuInvMass, muJetDeltaRmin_, w );
	 if( muRTightCheck(probeMuon) ){
	   TightFakeMuonPt2Dhisto->Fill(  MuonPt ->at(probeMuon), muJetDeltaRmin_, w );
	   TightFakeMuonEta2Dhisto->Fill( fabs(MuonEta->at(probeMuon)), muJetDeltaRmin_, w );
	   TightFakeMuonPhi2Dhisto->Fill( MuonPhi->at(probeMuon), muJetDeltaRmin_, w );
	   TightFakeMuonJetDeltaRmin2Dhisto->Fill( muJetDeltaRmin_, muJetDeltaRmin_, w );
	   TightFakeMuonMuDeltaR2Dhisto->Fill( muMuDeltaR, muJetDeltaRmin_, w );
	   TightFakeMuonMuInvMass2Dhisto->Fill( muMuInvMass, muJetDeltaRmin_, w );
	 }
       }
       // Prompt Muon
       if( !isRecoMuonFake(probeMuon) ){
	 LoosePromptMuonPt2Dhisto->Fill(  MuonPt ->at(probeMuon), muJetDeltaRmin_, w );
	 LoosePromptMuonEta2Dhisto->Fill( fabs(MuonEta->at(probeMuon)), muJetDeltaRmin_, w );
	 LoosePromptMuonPhi2Dhisto->Fill( MuonPhi->at(probeMuon), muJetDeltaRmin_, w );
         LoosePromptMuonJetDeltaRmin2Dhisto->Fill( muJetDeltaRmin_, muJetDeltaRmin_, w );
	 LoosePromptMuonMuDeltaR2Dhisto->Fill( muMuDeltaR, muJetDeltaRmin_, w );
	 LoosePromptMuonMuInvMass2Dhisto->Fill( muMuInvMass, muJetDeltaRmin_, w );
	 if( muRTightCheck(probeMuon) ){
	   TightPromptMuonPt2Dhisto->Fill(  MuonPt ->at(probeMuon), muJetDeltaRmin_, w );
	   TightPromptMuonEta2Dhisto->Fill( fabs(MuonEta->at(probeMuon)), muJetDeltaRmin_, w );
	   TightPromptMuonPhi2Dhisto->Fill( MuonPhi->at(probeMuon), muJetDeltaRmin_, w );
	   TightPromptMuonJetDeltaRmin2Dhisto->Fill( muJetDeltaRmin_, muJetDeltaRmin_, w );
	   TightPromptMuonMuDeltaR2Dhisto->Fill( muMuDeltaR, muJetDeltaRmin_, w );
	   TightPromptMuonMuInvMass2Dhisto->Fill( muMuInvMass, muJetDeltaRmin_, w );
	 }
       }
       //--------------------------------------------------------------
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
   LooseMuonPt2Dhisto->Write();
   LooseMuonEta2Dhisto->Write();
   LooseMuonPhi2Dhisto->Write();
   TightMuonPt2Dhisto->Write();
   TightMuonEta2Dhisto->Write();
   TightMuonPhi2Dhisto->Write();
   //
   LooseMuonMuInvMass2Dhisto->Write();
   TightMuonMuInvMass2Dhisto->Write();
   LooseFakeMuonMuInvMass2Dhisto->Write();
   TightFakeMuonMuInvMass2Dhisto->Write();
   LoosePromptMuonMuInvMass2Dhisto->Write();
   TightPromptMuonMuInvMass2Dhisto->Write();
   //
   LooseFakeMuonPt2Dhisto->Write();
   LooseFakeMuonEta2Dhisto->Write();
   LooseFakeMuonPhi2Dhisto->Write();
   TightFakeMuonPt2Dhisto->Write();
   TightFakeMuonEta2Dhisto->Write();
   TightFakeMuonPhi2Dhisto->Write();
   //
   LoosePromptMuonPt2Dhisto->Write();
   LoosePromptMuonEta2Dhisto->Write();
   LoosePromptMuonPhi2Dhisto->Write();
   TightPromptMuonPt2Dhisto->Write();
   TightPromptMuonEta2Dhisto->Write();
   TightPromptMuonPhi2Dhisto->Write();
   //
   LooseMuonMuDeltaR2Dhisto->Write();
   TightMuonMuDeltaR2Dhisto->Write();
   LooseFakeMuonMuDeltaR2Dhisto->Write();
   TightFakeMuonMuDeltaR2Dhisto->Write();
   LoosePromptMuonMuDeltaR2Dhisto->Write();
   TightPromptMuonMuDeltaR2Dhisto->Write();
   //
   LooseMuonJetDeltaRmin2Dhisto->Write();
   TightMuonJetDeltaRmin2Dhisto->Write();
   LooseFakeMuonJetDeltaRmin2Dhisto->Write();
   TightFakeMuonJetDeltaRmin2Dhisto->Write();
   LoosePromptMuonJetDeltaRmin2Dhisto->Write();
   TightPromptMuonJetDeltaRmin2Dhisto->Write();

   //////////
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
// // //
