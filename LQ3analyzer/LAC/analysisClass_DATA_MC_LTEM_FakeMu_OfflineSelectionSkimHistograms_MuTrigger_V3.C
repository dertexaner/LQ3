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
   LumiA=891; 
   LumiB=4428;
   LumiC=7018;
   LumiD=7248;
   //
   /**/
   tauPRdR3B_SF    = 1;   tauPRdR3B_SFerr = 0;
   tauPRdR2B_SF    = 1;   tauPRdR2B_SFerr = 0;
   tauPRdR1B_SF    = 1;   tauPRdR1B_SFerr = 0;
   tauPRdR3E_SF    = 1;   tauPRdR3E_SFerr = 0;
   tauPRdR2E_SF    = 1;   tauPRdR2E_SFerr = 0;
   tauPRdR1E_SF    = 1;   tauPRdR1E_SFerr = 0;
   //
   tauFRdR3B_SF    = 1;   tauFRdR3B_SFerr = 0;
   tauFRdR2B_SF    = 1;   tauFRdR2B_SFerr = 0;
   tauFRdR1B_SF    = 1;   tauFRdR1B_SFerr = 0;
   tauFRdR3E_SF    = 1;   tauFRdR3E_SFerr = 0;
   tauFRdR2E_SF    = 1;   tauFRdR2E_SFerr = 0;
   tauFRdR1E_SF    = 1;   tauFRdR1E_SFerr = 0;
   /**/
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
   TH1D* LeadMuTauDeltaRhisto = new TH1D("LeadMuTauDeltaRhisto","LeadMuTauDeltaRhisto",5000,0,50);
   TH1D* SThisto              = new TH1D("SThisto", "SThisto", 5000,0,5000);
   TH1D* SJThisto             = new TH1D("SJThisto","SJThisto",5000,0,5000);
   TH1D* METhisto             = new TH1D("METhisto","METhisto",2000,0,2000);
   //TH1D* PZetahisto           = new TH1D("PZetahisto","PZetahisto",8000,-2000,2000);
   //
   TH1D* AllJetJetInvMasshisto = new TH1D("AllJetJetInvMasshisto",  "AllJetJetInvMasshisto",  4000,0,4000);
   TH1D* MaxJetJetInvMasshisto = new TH1D("MaxJetJetInvMasshisto",  "MaxJetJetInvMasshisto",  4000,0,4000);
   TH1D* MinJetJetInvMasshisto = new TH1D("MinJetJetInvMasshisto",  "MinJetJetInvMasshisto",  4000,0,4000);
   TH1D* AllMuJetInvMasshisto  = new TH1D("AllMuJetInvMasshisto",   "AllMuJetInvMasshisto",   4000,0,4000);
   TH1D* MaxMuJetInvMasshisto  = new TH1D("MaxMuJetInvMasshisto",   "MaxMuJetInvMasshisto",   4000,0,4000);
   TH1D* MaxMuJetDeltaPhihisto = new TH1D("MaxMuJetDeltaPhihisto",  "MaxMuJetDeltaPhihisto",  1100,-1,10);
   TH1D* MaxMuJetDeltaRhisto   = new TH1D("MaxMuJetDeltaRhisto",    "MaxMuJetDeltaRhisto",    1100,-1,10);
   TH1D* AveMuJetDeltaRhisto   = new TH1D("AveMuJetDeltaRhisto",    "AveMuJetDeltaRhisto",    1100,-1,10);
   TH1D* jetsWDeltaPhihisto    = new TH1D("jetsWDeltaPhihisto",     "jetsWDeltaPhihisto",     1100,-1,10);
   TH1D* jetsWVecPthisto       = new TH1D("jetsWVecPthisto",        "jetsWVecPthisto",        4000,0,4000);
   TH1D* jetsWVecPhisto        = new TH1D("jetsWVecPhisto",         "jetsWVecPhisto",         4000,0,4000);
   //
   TH1D* MaxDiLepMasshisto     = new TH1D("MaxDiLepMasshisto",    "MaxDiLepMasshisto",    2000,0,2000);
   TH1D* AllMuMuInvMasshisto   = new TH1D("AllMuMuInvMasshisto",  "AllMuMuInvMasshisto",  2000,0,2000);
   TH1D* AllMuTauInvMasshisto  = new TH1D("AllMuTauInvMasshisto", "AllMuTauInvMasshisto", 2000,0,2000);
   TH1D* MaxMuTauInvMasshisto  = new TH1D("MaxMuTauInvMasshisto","MaxMuTauInvMasshisto",  2000,0,2000);
   TH1D* AllTauTauInvMasshisto = new TH1D("AllTauTauInvMasshisto","AllTauTauInvMasshisto",2000,0,2000);
   //
   TH1D* METmuTransMasshisto  = new TH1D("METmuTransMasshisto", "METmuTransMasshisto",2000,0,2000);
   TH1D* METtauTransMasshisto = new TH1D("METtauTransMasshisto","METtauTransMasshisto",2000,0,2000);
   //
   TH1D* METtauDeltaPhihisto = new TH1D("METtauDeltaPhihisto","METtauDeltaPhihisto",800,-4,4);
   TH1D* METmuDeltaPhihisto  = new TH1D("METmuDeltaPhihisto", "METmuDeltaPhihisto", 800,-4,4);
   TH1D* METelDeltaPhihisto  = new TH1D("METelDeltaPhihisto", "METelDeltaPhihisto", 800,-4,4);
   //
   TH1D* LeadMuPthisto    = new TH1D("LeadMuPthisto",   "LeadMuPthisto",   5000,0,5000);
   TH1D* LeadTauPthisto   = new TH1D("LeadTauPthisto",  "LeadTauPthisto",  5000,0,5000);
   TH1D* LeadElPthisto    = new TH1D("LeadElPthisto",   "LeadElPthisto",   5000,0,5000);
   TH1D* LeadJetPthisto   = new TH1D("LeadJetPthisto",  "LeadJetPthisto",  5000,0,5000);
   TH1D* LeadBJetPthisto  = new TH1D("LeadBJetPthisto", "LeadBJetPthisto", 5000,0,5000);
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
   TH1D* MuPthisto    = new TH1D("MuPthisto",   "MuPthisto",   5000, 0,5000);
   TH1D* TauPthisto   = new TH1D("TauPthisto",  "TauPthisto",  5000, 0,5000);
   TH1D* ElPthisto    = new TH1D("ElPthisto",   "ElPthisto",   5000, 0,5000);
   TH1D* JetPthisto   = new TH1D("JetPthisto",  "JetPthisto",  5000, 0,5000);
   TH1D* BJetPthisto  = new TH1D("BJetPthisto", "BJetPthisto", 5000, 0,5000);
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
   //
   // Fake Prompt Histos - 1D
   /*
   TH1D* LooseMuPthisto  = new TH1D("LooseMuPthisto", "LooseMuPthisto", 20000,0,2000);
   TH1D* TightMuPthisto  = new TH1D("TightMuPthisto", "TightMuPthisto", 20000,0,2000);
   TH1D* LooseMuEtahisto = new TH1D("LooseMuEtahisto","LooseMuEtahisto",500,-2.5,2.5);
   TH1D* TightMuEtahisto = new TH1D("TightMuEtahisto","TightMuEtahisto",500,-2.5,2.5);
   TH1D* LooseMuPhihisto = new TH1D("LooseMuPhihisto","LooseMuPhihisto",700,-3.5,3.5);
   TH1D* TightMuPhihisto = new TH1D("TightMuPhihisto","TightMuPhihisto",700,-3.5,3.5);
   TH1D* LooseMuJetDeltaRminhisto = new TH1D("LooseMuJetDeltaRminhisto","LooseMuJetDeltaRminhisto",160,0,8);
   TH1D* TightMuJetDeltaRminhisto = new TH1D("TightMuJetDeltaRminhisto","TightMuJetDeltaRminhisto",160,0,8);
   //
   TH1D* LooseFakeMuPthisto  = new TH1D("LooseFakeMuPthisto", "LooseFakeMuPthisto", 20000,0,2000);
   TH1D* TightFakeMuPthisto  = new TH1D("TightFakeMuPthisto", "TightFakeMuPthisto", 20000,0,2000);
   TH1D* LooseFakeMuEtahisto = new TH1D("LooseFakeMuEtahisto","LooseFakeMuEtahisto",500,-2.5,2.5);
   TH1D* TightFakeMuEtahisto = new TH1D("TightFakeMuEtahisto","TightFakeMuEtahisto",500,-2.5,2.5);
   TH1D* LooseFakeMuPhihisto = new TH1D("LooseFakeMuPhihisto","LooseFakeMuPhihisto",700,-3.5,3.5);
   TH1D* TightFakeMuPhihisto = new TH1D("TightFakeMuPhihisto","TightFakeMuPhihisto",700,-3.5,3.5);
   TH1D* LooseFakeMuJetDeltaRminhisto = new TH1D("LooseFakeMuJetDeltaRminhisto","LooseFakeMuJetDeltaRminhisto",160,0,8);
   TH1D* TightFakeMuJetDeltaRminhisto = new TH1D("TightFakeMuJetDeltaRminhisto","TightFakeMuJetDeltaRminhisto",160,0,8);
   //
   TH1D* LoosePromptMuPthisto  = new TH1D("LoosePromptMuPthisto", "LoosePromptMuPthisto", 20000,0,2000);
   TH1D* TightPromptMuPthisto  = new TH1D("TightPromptMuPthisto", "TightPromptMuPthisto", 20000,0,2000);
   TH1D* LoosePromptMuEtahisto = new TH1D("LoosePromptMuEtahisto","LoosePromptMuEtahisto",500,-2.5,2.5);
   TH1D* TightPromptMuEtahisto = new TH1D("TightPromptMuEtahisto","TightPromptMuEtahisto",500,-2.5,2.5);
   TH1D* LoosePromptMuPhihisto = new TH1D("LoosePromptMuPhihisto","LoosePromptMuPhihisto",700,-3.5,3.5);
   TH1D* TightPromptMuPhihisto = new TH1D("TightPromptMuPhihisto","TightPromptMuPhihisto",700,-3.5,3.5);
   TH1D* LoosePromptMuJetDeltaRminhisto = new TH1D("LoosePromptMuJetDeltaRminhisto","LoosePromptMuJetDeltaRminhisto",160,0,8);
   TH1D* TightPromptMuJetDeltaRminhisto = new TH1D("TightPromptMuJetDeltaRminhisto","TightPromptMuJetDeltaRminhisto",160,0,8);
   */
   //
   // Fake Prompt Histos - 3D
   // not using deltaR_jet_min binning for Muon Fake Rate so ignore it, add ST instead.
   //TH3D* LooseMuPt3Dhisto        = new TH3D("LooseMuPt3Dhisto",       "LooseMuPt3Dhisto",       10000,0,1000,200,0,10,2,0,3);
   //TH3D* TightMuPt3Dhisto        = new TH3D("TightMuPt3Dhisto",       "TightMuPt3Dhisto",       10000,0,1000,200,0,10,2,0,3);
   //TH3D* LooseFakeMuPt3Dhisto    = new TH3D("LooseFakeMuPt3Dhisto",   "LooseFakeMuPt3Dhisto",   10000,0,1000,200,0,10,2,0,3);
   //TH3D* TightFakeMuPt3Dhisto    = new TH3D("TightFakeMuPt3Dhisto",   "TightFakeMuPt3Dhisto",   10000,0,1000,200,0,10,2,0,3);
   //TH3D* LoosePromptMuPt3Dhisto  = new TH3D("LoosePromptMuPt3Dhisto", "LoosePromptMuPt3Dhisto", 10000,0,1000,200,0,10,2,0,3);
   //TH3D* TightPromptMuPt3Dhisto  = new TH3D("TightPromptMuPt3Dhisto", "TightPromptMuPt3Dhisto", 10000,0,1000,200,0,10,2,0,3);
   //
   TH3D* LooseMuPt3Dhisto        = new TH3D("LooseMuPt3Dhisto",       "LooseMuPt3Dhisto",       10000,0,1000,50,0,5000,2,0,3);
   TH3D* TightMuPt3Dhisto        = new TH3D("TightMuPt3Dhisto",       "TightMuPt3Dhisto",       10000,0,1000,50,0,5000,2,0,3);
   TH3D* LooseFakeMuPt3Dhisto    = new TH3D("LooseFakeMuPt3Dhisto",   "LooseFakeMuPt3Dhisto",   10000,0,1000,50,0,5000,2,0,3);
   TH3D* TightFakeMuPt3Dhisto    = new TH3D("TightFakeMuPt3Dhisto",   "TightFakeMuPt3Dhisto",   10000,0,1000,50,0,5000,2,0,3);
   TH3D* LoosePromptMuPt3Dhisto  = new TH3D("LoosePromptMuPt3Dhisto", "LoosePromptMuPt3Dhisto", 10000,0,1000,50,0,5000,2,0,3);
   TH3D* TightPromptMuPt3Dhisto  = new TH3D("TightPromptMuPt3Dhisto", "TightPromptMuPt3Dhisto", 10000,0,1000,50,0,5000,2,0,3);
   //
   TH1D* LooseMuPtDr0Bhisto  = new TH1D("LooseMuPtDr0Bhisto", "LooseMuPtDr0Bhisto", 10000,0,1000);
   TH1D* TightMuPtDr0Bhisto  = new TH1D("TightMuPtDr0Bhisto", "TightMuPtDr0Bhisto", 10000,0,1000);
   TH1D* LooseMuPtDr1Bhisto  = new TH1D("LooseMuPtDr1Bhisto", "LooseMuPtDr1Bhisto", 10000,0,1000);
   TH1D* TightMuPtDr1Bhisto  = new TH1D("TightMuPtDr1Bhisto", "TightMuPtDr1Bhisto", 10000,0,1000);
   TH1D* LooseMuPtDr2Bhisto  = new TH1D("LooseMuPtDr2Bhisto", "LooseMuPtDr2Bhisto", 10000,0,1000);
   TH1D* TightMuPtDr2Bhisto  = new TH1D("TightMuPtDr2Bhisto", "TightMuPtDr2Bhisto", 10000,0,1000);
   TH1D* LooseMuPtDr3Bhisto  = new TH1D("LooseMuPtDr3Bhisto", "LooseMuPtDr3Bhisto", 10000,0,1000);
   TH1D* TightMuPtDr3Bhisto  = new TH1D("TightMuPtDr3Bhisto", "TightMuPtDr3Bhisto", 10000,0,1000);
   //
   TH1D* LooseFakeMuPtDr0Bhisto  = new TH1D("LooseFakeMuPtDr0Bhisto", "LooseFakeMuPtDr0Bhisto", 10000,0,1000);
   TH1D* TightFakeMuPtDr0Bhisto  = new TH1D("TightFakeMuPtDr0Bhisto", "TightFakeMuPtDr0Bhisto", 10000,0,1000);
   TH1D* LooseFakeMuPtDr1Bhisto  = new TH1D("LooseFakeMuPtDr1Bhisto", "LooseFakeMuPtDr1Bhisto", 10000,0,1000);
   TH1D* TightFakeMuPtDr1Bhisto  = new TH1D("TightFakeMuPtDr1Bhisto", "TightFakeMuPtDr1Bhisto", 10000,0,1000);
   TH1D* LooseFakeMuPtDr2Bhisto  = new TH1D("LooseFakeMuPtDr2Bhisto", "LooseFakeMuPtDr2Bhisto", 10000,0,1000);
   TH1D* TightFakeMuPtDr2Bhisto  = new TH1D("TightFakeMuPtDr2Bhisto", "TightFakeMuPtDr2Bhisto", 10000,0,1000);
   TH1D* LooseFakeMuPtDr3Bhisto  = new TH1D("LooseFakeMuPtDr3Bhisto", "LooseFakeMuPtDr3Bhisto", 10000,0,1000);
   TH1D* TightFakeMuPtDr3Bhisto  = new TH1D("TightFakeMuPtDr3Bhisto", "TightFakeMuPtDr3Bhisto", 10000,0,1000);
   //
   TH1D* LoosePromptMuPtDr0Bhisto  = new TH1D("LoosePromptMuPtDr0Bhisto", "LoosePromptMuPtDr0Bhisto", 10000,0,1000);
   TH1D* TightPromptMuPtDr0Bhisto  = new TH1D("TightPromptMuPtDr0Bhisto", "TightPromptMuPtDr0Bhisto", 10000,0,1000);
   TH1D* LoosePromptMuPtDr1Bhisto  = new TH1D("LoosePromptMuPtDr1Bhisto", "LoosePromptMuPtDr1Bhisto", 10000,0,1000);
   TH1D* TightPromptMuPtDr1Bhisto  = new TH1D("TightPromptMuPtDr1Bhisto", "TightPromptMuPtDr1Bhisto", 10000,0,1000);
   TH1D* LoosePromptMuPtDr2Bhisto  = new TH1D("LoosePromptMuPtDr2Bhisto", "LoosePromptMuPtDr2Bhisto", 10000,0,1000);
   TH1D* TightPromptMuPtDr2Bhisto  = new TH1D("TightPromptMuPtDr2Bhisto", "TightPromptMuPtDr2Bhisto", 10000,0,1000);
   TH1D* LoosePromptMuPtDr3Bhisto  = new TH1D("LoosePromptMuPtDr3Bhisto", "LoosePromptMuPtDr3Bhisto", 10000,0,1000);
   TH1D* TightPromptMuPtDr3Bhisto  = new TH1D("TightPromptMuPtDr3Bhisto", "TightPromptMuPtDr3Bhisto", 10000,0,1000);
   //
   TH1D* LooseMuPtDr0Ehisto  = new TH1D("LooseMuPtDr0Ehisto", "LooseMuPtDr0Ehisto", 10000,0,1000);
   TH1D* TightMuPtDr0Ehisto  = new TH1D("TightMuPtDr0Ehisto", "TightMuPtDr0Ehisto", 10000,0,1000);
   TH1D* LooseMuPtDr1Ehisto  = new TH1D("LooseMuPtDr1Ehisto", "LooseMuPtDr1Ehisto", 10000,0,1000);
   TH1D* TightMuPtDr1Ehisto  = new TH1D("TightMuPtDr1Ehisto", "TightMuPtDr1Ehisto", 10000,0,1000);
   TH1D* LooseMuPtDr2Ehisto  = new TH1D("LooseMuPtDr2Ehisto", "LooseMuPtDr2Ehisto", 10000,0,1000);
   TH1D* TightMuPtDr2Ehisto  = new TH1D("TightMuPtDr2Ehisto", "TightMuPtDr2Ehisto", 10000,0,1000);
   TH1D* LooseMuPtDr3Ehisto  = new TH1D("LooseMuPtDr3Ehisto", "LooseMuPtDr3Ehisto", 10000,0,1000);
   TH1D* TightMuPtDr3Ehisto  = new TH1D("TightMuPtDr3Ehisto", "TightMuPtDr3Ehisto", 10000,0,1000);
   //
   TH1D* LooseFakeMuPtDr0Ehisto  = new TH1D("LooseFakeMuPtDr0Ehisto", "LooseFakeMuPtDr0Ehisto", 10000,0,1000);
   TH1D* TightFakeMuPtDr0Ehisto  = new TH1D("TightFakeMuPtDr0Ehisto", "TightFakeMuPtDr0Ehisto", 10000,0,1000);
   TH1D* LooseFakeMuPtDr1Ehisto  = new TH1D("LooseFakeMuPtDr1Ehisto", "LooseFakeMuPtDr1Ehisto", 10000,0,1000);
   TH1D* TightFakeMuPtDr1Ehisto  = new TH1D("TightFakeMuPtDr1Ehisto", "TightFakeMuPtDr1Ehisto", 10000,0,1000);
   TH1D* LooseFakeMuPtDr2Ehisto  = new TH1D("LooseFakeMuPtDr2Ehisto", "LooseFakeMuPtDr2Ehisto", 10000,0,1000);
   TH1D* TightFakeMuPtDr2Ehisto  = new TH1D("TightFakeMuPtDr2Ehisto", "TightFakeMuPtDr2Ehisto", 10000,0,1000);
   TH1D* LooseFakeMuPtDr3Ehisto  = new TH1D("LooseFakeMuPtDr3Ehisto", "LooseFakeMuPtDr3Ehisto", 10000,0,1000);
   TH1D* TightFakeMuPtDr3Ehisto  = new TH1D("TightFakeMuPtDr3Ehisto", "TightFakeMuPtDr3Ehisto", 10000,0,1000);
   //
   TH1D* LoosePromptMuPtDr0Ehisto  = new TH1D("LoosePromptMuPtDr0Ehisto", "LoosePromptMuPtDr0Ehisto", 10000,0,1000);
   TH1D* TightPromptMuPtDr0Ehisto  = new TH1D("TightPromptMuPtDr0Ehisto", "TightPromptMuPtDr0Ehisto", 10000,0,1000);
   TH1D* LoosePromptMuPtDr1Ehisto  = new TH1D("LoosePromptMuPtDr1Ehisto", "LoosePromptMuPtDr1Ehisto", 10000,0,1000);
   TH1D* TightPromptMuPtDr1Ehisto  = new TH1D("TightPromptMuPtDr1Ehisto", "TightPromptMuPtDr1Ehisto", 10000,0,1000);
   TH1D* LoosePromptMuPtDr2Ehisto  = new TH1D("LoosePromptMuPtDr2Ehisto", "LoosePromptMuPtDr2Ehisto", 10000,0,1000);
   TH1D* TightPromptMuPtDr2Ehisto  = new TH1D("TightPromptMuPtDr2Ehisto", "TightPromptMuPtDr2Ehisto", 10000,0,1000);
   TH1D* LoosePromptMuPtDr3Ehisto  = new TH1D("LoosePromptMuPtDr3Ehisto", "LoosePromptMuPtDr3Ehisto", 10000,0,1000);
   TH1D* TightPromptMuPtDr3Ehisto  = new TH1D("TightPromptMuPtDr3Ehisto", "TightPromptMuPtDr3Ehisto", 10000,0,1000);


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
     // ---- trigger 
     unsigned int triggerMuon=99;
     for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
       if( !muRisoCheck(iMuR) || MuonPt->at(iMuR)<45 ) continue;
       triggerMuon=iMuR;
       break;//take the leading one
     }
     //
     int usedTrigger_=-5;
     //usedTrigger_ = SingleMu_passTrigger();//IsoMu24_eta2p1 trigger
     usedTrigger_ = SingleMu40_passTrigger();
     //usedTrigger_ = HLT_MuPT_eta2p1_passTrigger();
     double TriggerEfficiencyWeights_=0;
     // use same weights for Mu24 and Mu40 triggers
     //TriggerEfficiencyWeights_=IsoMu24e2p1_Eff( MuonPt->at(triggerMuon), MuonEta->at(triggerMuon) );
     if( triggerMuon!=99 ){
       TriggerEfficiencyWeights_=Mu40e2p1_ScaleFactor( MuonPt->at(triggerMuon), MuonEta->at(triggerMuon) );
     }
     //
     AppliedTrigEffWeightshisto->Fill( TriggerEfficiencyWeights_ );
     // ---- total = pileup x trigger
     double w = 0;
     if( !isData  ){
       w=safePileupWeights_*TriggerEfficiencyWeights_; 
       // LTEM_ScaleFactor( ltemMuTau ) weights are applied below..
     }
     if( isData  ) w=1;


     //  -- JSON SKIM
     int passJSON_=0;
     if( isData  ) passJSON_ = passJSON(run,ls,isData);
     if( !isData ) passJSON_ = 1;
     fillVariableWithValue("PassJSON", passJSON_ );//..............// returns 0, 1                                                                      


     //  -- TRIGGER SKIM
     int passTrigger_=0;
     //passTrigger_ = SingleMu_passTrigger(); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<this is IsoMu24 trigger
     passTrigger_ = SingleMu40_passTrigger();        //<<<<<<<<<<<<<<<<< this is Mu40 trigger 
     //passTrigger_ = HLT_MuPT_eta2p1_passTrigger(); //<<<<<<<<<<<<<<<<< this is Mu24 trigger  (no isolation)
     fillVariableWithValue("PassTrig", passTrigger_ );//...// returns -2, -1, 0, trig                                                                         

     //  -- EVENT FILTER SKIM
     int passEventFilter_=0;
     if( passEventFilter() ) passEventFilter_ = 1;
     fillVariableWithValue("PassEventFilter", passEventFilter_ );//.// returns 0, 1 


     //  -- DILEPTON SKIM - MuTau
     int isOfflineDilepton_=0;
     //if( isMuTauDR0p30() ) isOfflineDilepton_ = 1;
     //Make Trigger Muon Tight
     //if( isMuTauDR0p30() && muRTightCheck(triggerMuon) ) isOfflineDilepton_ = 1;
     isOfflineDilepton_ = 1;
     fillVariableWithValue("PassOfflineDilepton", isOfflineDilepton_ );// Returns 0, 1          


     // -- HLT MATCHING SKIM
     int passAllMuHLTmatching_=0;
     //passAllMuHLTmatching_=1; // this is done in the skimming step already.
     //if( RecoHLTdeltaRmin_SingleMuTrigger(triggerMuon)<0.15   ) passAllMuHLTmatching_=1;
     if( triggerMuon!=99 ){
       if( RecoHLTdeltaRmin_SingleMu40Trigger(triggerMuon)<0.15 ) passAllMuHLTmatching_=1;
     }
     //if( RecoHLTdeltaRmin_SingleMu24Trigger(triggerMuon)<0.15 ) passAllMuHLTmatching_=1;
     fillVariableWithValue("PassAllMuHLTmatching",passAllMuHLTmatching_);

     
     // 
     // Computing Pzeta quantity
     /*
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
     */

     //computing sjt quantity
     double sjt_=0;
     for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
       if( !jetRisoCheck(iJetR)   )continue;
       sjt_+=PFJetPt->at(iJetR);
     }

     // mu-jet quantities
     double MaxMuJetInvMass = 0;
     double MaxMuJetDeltaPhi= 0;
     double MaxMuJetDeltaR  = 0;
     double AveMuJetDeltaR  = 0;
     TLorentzVector jetVec, muVec, sumJetVec, metVec;
     sumJetVec.SetPtEtaPhiM( 0 , 0 , 0 , 0 );
     double jetsWDeltaPhi = 0;
     double jetsWVecPt    = 0;
     double jetsWVecP     = 0;
     if( triggerMuon!=99 ){
       int nIsoJets=0;
       for(unsigned int iJetR1=0; iJetR1<PFJetPt->size(); iJetR1++){
	 if( !jetRisoCheck(iJetR1)   )continue;
	 nIsoJets++;
	 //
	 jetVec.SetPtEtaPhiM( PFJetPt->at(iJetR1), PFJetEta->at(iJetR1), PFJetPhi->at(iJetR1), 0 );
	 muVec.SetPtEtaPhiM( MuonPt->at(triggerMuon), MuonEta->at(triggerMuon), MuonPhi->at(triggerMuon), 0 );
	 //
	 sumJetVec+=jetVec;
	 //
	 if( MaxMuJetInvMass<(jetVec+muVec).M()            ) MaxMuJetInvMass  = (jetVec+muVec).M();
	 if( MaxMuJetDeltaPhi<fabs(jetVec.DeltaPhi(muVec)) ) MaxMuJetDeltaPhi = fabs(jetVec.DeltaPhi(muVec));
	 if( MaxMuJetDeltaR<fabs(jetVec.DeltaR(muVec))     ) MaxMuJetDeltaR   = fabs(jetVec.DeltaR(muVec));
	 AveMuJetDeltaR+=fabs(jetVec.DeltaR(muVec));
	 //
       }
       AveMuJetDeltaR=(double)(AveMuJetDeltaR/nIsoJets);
       //
       metVec.SetPtEtaPhiM( PFMETType01XYCor->at(0), 0, PFMETPhiType01XYCor->at(0), 0 );
       jetsWDeltaPhi = sumJetVec.DeltaPhi( (muVec+metVec) );
       jetsWVecPt    = (sumJetVec+muVec+metVec).Pt();
       jetsWVecP     = (sumJetVec+muVec+metVec).P();
     }


     //checking to see if there is a tight tau
     bool zeroTightTau_=true;
     for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
       if( tauRisoCheck(iTauR) && tauRTightCheck(iTauR) ){
	 zeroTightTau_=false;
	 break;
       }
     }


     // -- OFFLINE SKIM
     int OfflineCuts_=0;
     //int NLepJet= MuCounter()+ElCounter()+TauCounter()+JetCounter();
     //
     if( triggerMuon!=99  && 
	 MuCounter()==1   &&
	 LeadingMuPt()>45 &&
	 ElCounter()==0   &&
	 TauCounter()==0  &&
	 METlepMT("Mu",triggerMuon)<20 &&
	 sjt_>90 && PFMETType01XYCor->at(0)<60 &&
	 MaxMuJetDeltaR>TMath::Pi() 
	 //jetsWVecPt>40
	 //jetsWVecPt>50
	 //muJetDeltaRmin(ltemMuTau[0])>0.5 && tauJetDeltaRmin(ltemMuTau[1])>0.7 ) OfflineCuts_=1;
	 //muJetDeltaRmin(ltemMuTau[0])>0.5 && tauJetDeltaRmin(ltemMuTau[1])>1.0 ) OfflineCuts_=1;
	) OfflineCuts_=1;
     fillVariableWithValue("PassOfflineCuts", OfflineCuts_ );// returns 0, 1 
     
     
     evaluateCuts();
     //---------------------------------------------------------------------------------
     if( passedCut("PassJSON") && passedCut("PassTrig") && passedCut("PassEventFilter") && passedCut("PassOfflineDilepton") &&
	 passedCut("PassAllMuHLTmatching") && passedCut("PassOfflineCuts") ){ 
       

       //cout<< "muJetDeltaRmin(triggerMuon): "<<muJetDeltaRmin(triggerMuon)<<endl;

       bool isRecoMuPrompt_ = false;
       bool isRecoMuTight_  = false;
       //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       // Check whether mu is Loose or Tight!!
       if( muRTightCheck(triggerMuon) ) isRecoMuTight_=true;

       // Check whether mu is Fake or Prompt!!
       TLorentzVector RecoMu, GenMu;
       RecoMu.SetPtEtaPhiM( MuonPt->at(triggerMuon), MuonEta->at(triggerMuon), MuonPhi->at(triggerMuon), 0 );
       //Check RecoMu  ( W )
       for( unsigned int iMuT=0; iMuT<GenWMuPt->size(); iMuT++){
	 GenMu.SetPtEtaPhiM( GenWMuPt->at(iMuT),     GenWMuEta->at(iMuT),   GenWMuPhi->at(iMuT), 0 );
	 if( GenMu.DeltaR(RecoMu)<0.15 ) isRecoMuPrompt_ = true;
       }
       //Check RecoMu  ( Z )
       for( unsigned int iMuT=0; iMuT<GenZMuPt->size(); iMuT++){
	 GenMu.SetPtEtaPhiM( GenZMuPt->at(iMuT),     GenZMuEta->at(iMuT),   GenZMuPhi->at(iMuT), 0 );
	 if( GenMu.DeltaR(RecoMu)<0.15 ) isRecoMuPrompt_ = true;
       }
       //
       /*
       //Check RecoMu  (LQ3->Tau)
       for( unsigned int iMuT=0; iMuT<GenLQTauMuonPt->size(); iMuT++){
	 GenMu.SetPtEtaPhiM( GenLQTauMuonPt->at(iMuT),     GenLQTauMuonEta->at(iMuT),   GenLQTauMuonPhi->at(iMuT), 0 );
	 if( GenMu.DeltaR(RecoMu)<0.15 ) isRecoMuPrompt_ = true;
       }
       */
       //
       //
       /*
       tauPRdR3B_SF    = 0.981;   tauPRdR3B_SFerr = 0.028;
       tauPRdR2B_SF    = 0.960;   tauPRdR2B_SFerr = 0.092;
       tauPRdR1B_SF    = 0.960;   tauPRdR1B_SFerr = 0.092;
       tauPRdR3E_SF    = 1.097;   tauPRdR3E_SFerr = 0.083;
       tauPRdR2E_SF    = 1.097;   tauPRdR2E_SFerr = 0.083;
       tauPRdR1E_SF    = 1.097;   tauPRdR1E_SFerr = 0.083;
       //
       //tauFRdR3B_SF    = 1.083;   tauFRdR3B_SFerr = 0.063;
       //tauFRdR2B_SF    = 1.119;   tauFRdR2B_SFerr = 0.066;
       //tauFRdR1B_SF    = 1.261;   tauFRdR1B_SFerr = 0.111;
       tauFRdR3B_SF    = 1.000;   tauFRdR3B_SFerr = 0.022;//for TauPt>32.5
       tauFRdR2B_SF    = 1.000;   tauFRdR2B_SFerr = 0.106;
       tauFRdR1B_SF    = 1.000;   tauFRdR1B_SFerr = 0.177;
       //tauFRdR3E_SF    = 1.079;   tauFRdR3E_SFerr = 0.077;
       tauFRdR3E_SF    = 1.000;   tauFRdR3E_SFerr = 0.065;//for TauPt>32.5
       tauFRdR2E_SF    = 1.101;   tauFRdR2E_SFerr = 0.114;
       tauFRdR1E_SF    = 1.117;   tauFRdR1E_SFerr = 0.173;
       //
       double LTEM_ScaleFactor_=1;
       if( !isData  ){//for MC: apply SF dependent event weights to TT box based on Gen info, set SFs back to one
	 LTEM_ScaleFactor_=LTEM_ScaleFactor( ltemMuTau[1], isRecoTauPrompt_ );
	 //
	 tauPRdR3B_SF    = 1;   tauPRdR3B_SFerr = 0;
	 tauPRdR2B_SF    = 1;   tauPRdR2B_SFerr = 0;
	 tauPRdR1B_SF    = 1;   tauPRdR1B_SFerr = 0;
	 tauPRdR3E_SF    = 1;   tauPRdR3E_SFerr = 0;
	 tauPRdR2E_SF    = 1;   tauPRdR2E_SFerr = 0;
	 tauPRdR1E_SF    = 1;   tauPRdR1E_SFerr = 0;
	 //
	 tauFRdR3B_SF    = 1;   tauFRdR3B_SFerr = 0;
	 tauFRdR2B_SF    = 1;   tauFRdR2B_SFerr = 0;
	 tauFRdR1B_SF    = 1;   tauFRdR1B_SFerr = 0;
	 tauFRdR3E_SF    = 1;   tauFRdR3E_SFerr = 0;
	 tauFRdR2E_SF    = 1;   tauFRdR2E_SFerr = 0;
	 tauFRdR1E_SF    = 1;   tauFRdR1E_SFerr = 0;
       }
       */
       //
       //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

       // Weight Histos
       Triggerhisto->Fill( usedTrigger_ );
       AppliedPileUpWeightshisto->Fill( safePileupWeights_ );
       AppliedTotalWeightshisto->Fill( w );


       double MuonPt_        = MuonPt->at(triggerMuon);
       double AbsMuonEta_    = fabs(MuonEta->at(triggerMuon));
       double MuonPhi_       = MuonPhi->at(triggerMuon);
       double muJetDeltaRmin_= muJetDeltaRmin(triggerMuon);
       double st_            = ST();
       //
       // Loose
       //if( !isRecoMuTight_ ){//this is loose-not-tight
       /////LooseMuPt3Dhisto->Fill(  MuonPt_  , muJetDeltaRmin_ , AbsMuonEta_ , w );
       LooseMuPt3Dhisto->Fill(  MuonPt_  , st_ , AbsMuonEta_ , w );
       //LooseMuEta3Dhisto->Fill( MuonEta_ , muJetDeltaRmin_ , w );
       //LooseMuPhi3Dhisto->Fill( MuonPhi_ , muJetDeltaRmin_ , w );
       //LooseMuJetDeltaRmin3Dhisto->Fill( muJetDeltaRmin_ , muJetDeltaRmin_ , w );
       if( isRecoMuPrompt_  ){ //Prompt
	 /////LoosePromptMuPt3Dhisto->Fill(  MuonPt_  , muJetDeltaRmin_ , AbsMuonEta_ , w );
	 LoosePromptMuPt3Dhisto->Fill(  MuonPt_  , st_ , AbsMuonEta_ , w );
	 //LoosePromptMuEta3Dhisto->Fill( MuonEta_ , muJetDeltaRmin_ , w );
	 //LoosePromptMuPhi3Dhisto->Fill( MuonPhi_ , muJetDeltaRmin_ , w );
	 //LoosePromptMuJetDeltaRmin3Dhisto->Fill( muJetDeltaRmin_ , muJetDeltaRmin_ , w );
       }
       if( !isRecoMuPrompt_ ){ //Fake
	 /////LooseFakeMuPt3Dhisto->Fill(  MuonPt_  , muJetDeltaRmin_ , AbsMuonEta_ , w );
	 LooseFakeMuPt3Dhisto->Fill(  MuonPt_  , st_ , AbsMuonEta_ , w );
	 //LooseFakeMuEta3Dhisto->Fill( MuonEta_ , muJetDeltaRmin_ , w );
	 //LooseFakeMuPhi3Dhisto->Fill( MuonPhi_ , muJetDeltaRmin_ , w );
	 //LooseFakeMuJetDeltaRmin3Dhisto->Fill( muJetDeltaRmin_ , muJetDeltaRmin_ , w );
       }
       //}
       // Tight
       if( isRecoMuTight_ ){
	 /////TightMuPt3Dhisto->Fill(  MuonPt_  , muJetDeltaRmin_ , AbsMuonEta_ , w );
	 TightMuPt3Dhisto->Fill(  MuonPt_  , st_ , AbsMuonEta_ , w );
	 //TightMuEta3Dhisto->Fill( MuonEta_ , muJetDeltaRmin_ , w );
	 //TightMuPhi3Dhisto->Fill( MuonPhi_ , muJetDeltaRmin_ , w );
	 //TightMuJetDeltaRmin3Dhisto->Fill( muJetDeltaRmin_ , muJetDeltaRmin_ , w );
	 if( isRecoMuPrompt_  ){ //Prompt
	   /////TightPromptMuPt3Dhisto->Fill(  MuonPt_  , muJetDeltaRmin_ , AbsMuonEta_ , w );
	   TightPromptMuPt3Dhisto->Fill(  MuonPt_  , st_ , AbsMuonEta_ , w );
	   //TightPromptMuEta3Dhisto->Fill( MuonEta_ , muJetDeltaRmin_ , w );
	   //TightPromptMuPhi3Dhisto->Fill( MuonPhi_ , muJetDeltaRmin_ , w );
	   //TightPromptMuJetDeltaRmin3Dhisto->Fill( muJetDeltaRmin_ , muJetDeltaRmin_ , w );
	 }
	 if( !isRecoMuPrompt_ ){ //Fake
	   /////TightFakeMuPt3Dhisto->Fill(  MuonPt_  , muJetDeltaRmin_ , AbsMuonEta_ , w );
	   TightFakeMuPt3Dhisto->Fill(  MuonPt_  , st_ , AbsMuonEta_ , w );
	   //TightFakeMuEta3Dhisto->Fill( MuonEta_ , muJetDeltaRmin_ , w );
	   //TightFakeMuPhi3Dhisto->Fill( MuonPhi_ , muJetDeltaRmin_ , w );
	   //TightFakeMuJetDeltaRmin3Dhisto->Fill( muJetDeltaRmin_ , muJetDeltaRmin_ , w );
	 }
       }
       //
	

       //PZetahisto->Fill( pZeta, w );
       
       // >>>>>>>>>>>>>>>>>>>>>> Histograms - multiple times per event. 
       allmu.SetPtEtaPhiM( 0, 0, 0, 0 );
       alltau.SetPtEtaPhiM( 0, 0, 0, 0 );
       allmu2.SetPtEtaPhiM( 0, 0, 0, 0 );
       alltau2.SetPtEtaPhiM( 0, 0, 0, 0 );
       //
       for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
	 if(!muRisoCheck(iMuR))continue;
	 //
	 if( triggerMuon!=iMuR ){
	   //NON triggerMuon MUON!!!
	   //triggerMuon: 0
	   //iMuR: 1
	   cout<<"NON triggerMuon MUON!!!"<<endl;
	   cout<<"triggerMuon: "<<triggerMuon<<"  muRisoCheck:"<<muRisoCheck(triggerMuon)<<"  Pt"<< MuonPt->at(triggerMuon)<<endl;
	   cout<<"       iMuR: "<<iMuR<<"  muRisoCheck:"<<muRisoCheck(iMuR)<<"  Pt"<< MuonPt->at(iMuR)<<endl;
	   cout<<"MuCounter(): "<<MuCounter()<<endl;
	   cout<<endl;
	 }
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
       TLorentzVector jet1Vec, jet2Vec;
       double MaxJetJetInvMass=0;
       double MinJetJetInvMass=9999999;
       for(unsigned int iJetR1=0; iJetR1<PFJetPt->size(); iJetR1++){
         if( !jetRisoCheck(iJetR1)   )continue;
	 for(unsigned int iJetR2=(iJetR1+1); iJetR2<PFJetPt->size(); iJetR2++){
	   if( !jetRisoCheck(iJetR2)   )continue;
	   //
	   jet1Vec.SetPtEtaPhiM( PFJetPt->at(iJetR1), PFJetEta->at(iJetR1), PFJetPhi->at(iJetR1), 0 );
	   jet2Vec.SetPtEtaPhiM( PFJetPt->at(iJetR2), PFJetEta->at(iJetR2), PFJetPhi->at(iJetR2), 0 );
	   AllJetJetInvMasshisto->Fill( (jet1Vec+jet2Vec).M(), w );
	   if( MaxJetJetInvMass<(jet1Vec+jet2Vec).M() ) MaxJetJetInvMass=(jet1Vec+jet2Vec).M();
	   if( MinJetJetInvMass>(jet1Vec+jet2Vec).M() ) MinJetJetInvMass=(jet1Vec+jet2Vec).M();
	   //
	 }
       }
       MaxJetJetInvMasshisto->Fill( MaxJetJetInvMass, w );
       MinJetJetInvMasshisto->Fill( MinJetJetInvMass, w );
	 

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
       // if( vertexRCheck(iVertexR) ) VertexN++;
       //}
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
       for(unsigned int iJetR1=0; iJetR1<PFJetPt->size(); iJetR1++){
         if( !jetRisoCheck(iJetR1)   )continue;
	 //
         for(unsigned int iJetR2=(iJetR1+1); iJetR2<PFJetPt->size(); iJetR2++){
           if( !jetRisoCheck(iJetR2)   )continue;
           //
           jet1Vec.SetPtEtaPhiM( PFJetPt->at(iJetR1), PFJetEta->at(iJetR1), PFJetPhi->at(iJetR1), 0 );
           jet2Vec.SetPtEtaPhiM( PFJetPt->at(iJetR2), PFJetEta->at(iJetR2), PFJetPhi->at(iJetR2), 0 );
           AllJetJetInvMasshisto->Fill( (jet1Vec+jet2Vec).M(), w );
           //
         }
       }
       //
       for(unsigned int iJetR1=0; iJetR1<PFJetPt->size(); iJetR1++){
	 if( !jetRisoCheck(iJetR1)   )continue;
	 //
	 jetVec.SetPtEtaPhiM( PFJetPt->at(iJetR1), PFJetEta->at(iJetR1), PFJetPhi->at(iJetR1), 0 );
	 muVec.SetPtEtaPhiM( MuonPt->at(triggerMuon), MuonEta->at(triggerMuon), MuonPhi->at(triggerMuon), 0 );
	 //
	 AllMuJetInvMasshisto->Fill( (muVec+jetVec).M(), w );
       }
       MaxMuJetInvMasshisto->Fill(  MaxMuJetInvMass,  w );
       MaxMuJetDeltaPhihisto->Fill( MaxMuJetDeltaPhi, w );
       MaxMuJetDeltaRhisto->Fill(   MaxMuJetDeltaR,   w );
       AveMuJetDeltaRhisto->Fill(   AveMuJetDeltaR,   w );
       jetsWDeltaPhihisto->Fill( jetsWDeltaPhi, w );
       jetsWVecPthisto->Fill(    jetsWVecPt,    w );
       jetsWVecPhisto->Fill(     jetsWVecP,     w );


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
   //PZetahisto->Write();
   //
   METtauDeltaPhihisto->Write();
   METmuDeltaPhihisto ->Write();
   METelDeltaPhihisto ->Write();
   //
   AllJetJetInvMasshisto->Write();
   MaxJetJetInvMasshisto->Write();
   MinJetJetInvMasshisto->Write();
   AllMuJetInvMasshisto->Write();
   MaxMuJetInvMasshisto->Write();
   MaxMuJetDeltaPhihisto->Write();
   MaxMuJetDeltaRhisto->Write();
   AveMuJetDeltaRhisto->Write();
   jetsWDeltaPhihisto->Write();
   jetsWVecPthisto->Write();
   jetsWVecPhisto->Write();
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
   /*
   LooseMuPthisto->Write();
   TightMuPthisto->Write();
   LooseMuEtahisto->Write();
   TightMuEtahisto->Write();
   LooseMuPhihisto->Write();
   TightMuPhihisto->Write();
   LooseMuJetDeltaRminhisto->Write();
   TightMuJetDeltaRminhisto->Write();
   //
   LooseFakeMuPthisto->Write();
   TightFakeMuPthisto->Write();
   LooseFakeMuEtahisto->Write();
   TightFakeMuEtahisto->Write();
   LooseFakeMuPhihisto->Write();
   TightFakeMuPhihisto->Write();
   LooseFakeMuJetDeltaRminhisto->Write();
   TightFakeMuJetDeltaRminhisto->Write();
   //
   LoosePromptMuPthisto->Write();
   TightPromptMuPthisto->Write();
   LoosePromptMuEtahisto->Write();
   TightPromptMuEtahisto->Write();
   LoosePromptMuPhihisto->Write();
   TightPromptMuPhihisto->Write();
   LoosePromptMuJetDeltaRminhisto->Write();
   TightPromptMuJetDeltaRminhisto->Write();
   */
   //
   /*
   LooseMuPt2Dhisto->Write();
   TightMuPt2Dhisto->Write();
   LooseMuEta2Dhisto->Write();
   TightMuEta2Dhisto->Write();
   LooseMuPhi2Dhisto->Write();
   TightMuPhi2Dhisto->Write();
   LooseMuJetDeltaRmin2Dhisto->Write();
   TightMuJetDeltaRmin2Dhisto->Write();
   //
   LooseFakeMuPt2Dhisto->Write();
   TightFakeMuPt2Dhisto->Write();
   LooseFakeMuEta2Dhisto->Write();
   TightFakeMuEta2Dhisto->Write();
   LooseFakeMuPhi2Dhisto->Write();
   TightFakeMuPhi2Dhisto->Write();
   LooseFakeMuJetDeltaRmin2Dhisto->Write();
   TightFakeMuJetDeltaRmin2Dhisto->Write();
   //
   LoosePromptMuPt2Dhisto->Write();
   TightPromptMuPt2Dhisto->Write();
   LoosePromptMuEta2Dhisto->Write();
   TightPromptMuEta2Dhisto->Write();
   LoosePromptMuPhi2Dhisto->Write();
   TightPromptMuPhi2Dhisto->Write();
   LoosePromptMuJetDeltaRmin2Dhisto->Write();
   TightPromptMuJetDeltaRmin2Dhisto->Write();
   */
   //
   /*
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr0Bhisto",1, 200,1,1, "e");//All
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr1Bhisto",11,14, 1,1, "e");//0.5-0.7
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr2Bhisto",15,20, 1,1, "e");//0.7-1.0
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr3Bhisto",21,200,1,1, "e");//1.0>
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr0Bhisto",1, 200,1,1, "e");
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr1Bhisto",11,14, 1,1, "e");
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr2Bhisto",15,20, 1,1, "e");
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr3Bhisto",21,200,1,1, "e");
   //
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr0Bhisto",1, 200,1,1, "e");
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr1Bhisto",11,14, 1,1, "e");
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr2Bhisto",15,20, 1,1, "e");
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr3Bhisto",21,200,1,1, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr0Bhisto",1, 200,1,1, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr1Bhisto",11,14, 1,1, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr2Bhisto",15,20, 1,1, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr3Bhisto",21,200,1,1, "e");
   //
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr0Bhisto",1, 200,1,1, "e");
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr1Bhisto",11,14, 1,1, "e");
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr2Bhisto",15,20, 1,1, "e");
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr3Bhisto",21,200,1,1, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr0Bhisto",1, 200,1,1, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr1Bhisto",11,14, 1,1, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr2Bhisto",15,20, 1,1, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr3Bhisto",21,200,1,1, "e");
   // ----
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr0Ehisto",1, 200,2,2, "e");//All
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr1Ehisto",11,14, 2,2, "e");//0.5-0.7
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr2Ehisto",15,20, 2,2, "e");//0.7-1.0
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr3Ehisto",21,200,2,2, "e");//1.0>
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr0Ehisto",1, 200,2,2, "e");
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr1Ehisto",11,14, 2,2, "e");
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr2Ehisto",15,20, 2,2, "e");
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr3Ehisto",21,200,2,2, "e");
   //
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr0Ehisto",1, 200,2,2, "e");
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr1Ehisto",11,14, 2,2, "e");
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr2Ehisto",15,20, 2,2, "e");
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr3Ehisto",21,200,2,2, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr0Ehisto",1, 200,2,2, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr1Ehisto",11,14, 2,2, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr2Ehisto",15,20, 2,2, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr3Ehisto",21,200,2,2, "e");
   //
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr0Ehisto",1, 200,2,2, "e");
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr1Ehisto",11,14, 2,2, "e");
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr2Ehisto",15,20, 2,2, "e");
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr3Ehisto",21,200,2,2, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr0Ehisto",1, 200,2,2, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr1Ehisto",11,14, 2,2, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr2Ehisto",15,20, 2,2, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr3Ehisto",21,200,2,2, "e");
   //# DR 0-5 with 100 bins
   //JetDeltaRminBoundaryLow  = [ 11, 11,15,21  ]
   //JetDeltaRminBoundaryHigh = [ 100,14,20,100 ]
   */
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr0Bhisto",1,50, 1,1, "e");//All
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr1Bhisto",2, 4, 1,1, "e");//100-400
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr2Bhisto",5,50, 1,1, "e");//400<
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr3Bhisto",1,50, 1,1, "e");
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr0Bhisto",1,50, 1,1, "e");
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr1Bhisto",2, 4, 1,1, "e");
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr2Bhisto",5,50, 1,1, "e");
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr3Bhisto",1,50, 1,1, "e");
   //
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr0Bhisto",1,50, 1,1, "e");
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr1Bhisto",2, 4, 1,1, "e");
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr2Bhisto",5,50, 1,1, "e");
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr3Bhisto",1,50, 1,1, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr0Bhisto",1,50, 1,1, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr1Bhisto",2, 4, 1,1, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr2Bhisto",5,50, 1,1, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr3Bhisto",1,50, 1,1, "e");
   //
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr0Bhisto",1,50, 1,1, "e");
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr1Bhisto",2, 4, 1,1, "e");
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr2Bhisto",5,50, 1,1, "e");
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr3Bhisto",1,50, 1,1, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr0Bhisto",1,50, 1,1, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr1Bhisto",2, 4, 1,1, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr2Bhisto",5,50, 1,1, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr3Bhisto",1,50, 1,1, "e");
   // ----
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr0Ehisto",1,50, 2,2, "e");//All
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr1Ehisto",2, 4, 2,2, "e");//100-400
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr2Ehisto",5,50, 2,2, "e");//400<
   LooseMuPt3Dhisto->ProjectionX("LooseMuPtDr3Ehisto",1,50, 2,2, "e");//
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr0Ehisto",1,50, 2,2, "e");
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr1Ehisto",2, 4, 2,2, "e");
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr2Ehisto",5,50, 2,2, "e");
   TightMuPt3Dhisto->ProjectionX("TightMuPtDr3Ehisto",1,50, 2,2, "e");
   //
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr0Ehisto",1,50, 2,2, "e");
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr1Ehisto",2, 4, 2,2, "e");
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr2Ehisto",5,50, 2,2, "e");
   LooseFakeMuPt3Dhisto->ProjectionX("LooseFakeMuPtDr3Ehisto",1,50, 2,2, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr0Ehisto",1,50, 2,2, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr1Ehisto",2, 4, 2,2, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr2Ehisto",5,50, 2,2, "e");
   TightFakeMuPt3Dhisto->ProjectionX("TightFakeMuPtDr3Ehisto",1,50, 2,2, "e");
   //
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr0Ehisto",1,50, 2,2, "e");
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr1Ehisto",2, 4, 2,2, "e");
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr2Ehisto",5,50, 2,2, "e");
   LoosePromptMuPt3Dhisto->ProjectionX("LoosePromptMuPtDr3Ehisto",1,50, 2,2, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr0Ehisto",1,50, 2,2, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr1Ehisto",2, 4, 2,2, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr2Ehisto",5,50, 2,2, "e");
   TightPromptMuPt3Dhisto->ProjectionX("TightPromptMuPtDr3Ehisto",1,50, 2,2, "e");
   //# ST 0-5000 with 50 bins, binwidth 100 GeV
   //
   cout<<"LooseMuPtDr0123Bhisto PRINT"<<endl;
   LooseMuPtDr0Bhisto->Print();
   LooseMuPtDr1Bhisto->Print();
   LooseMuPtDr2Bhisto->Print();
   LooseMuPtDr3Bhisto->Print();
   cout<<"LooseMuPtDr0123Bhisto PRINT END"<<endl;
   cout<<"LooseMuPtDr0123Ehisto PRINT"<<endl;
   LooseMuPtDr0Ehisto->Print();
   LooseMuPtDr1Ehisto->Print();
   LooseMuPtDr2Ehisto->Print();
   LooseMuPtDr3Ehisto->Print();
   cout<<"LooseMuPtDr0123Ehisto PRINT END"<<endl;
   //
   LooseMuPtDr0Bhisto->Write();
   LooseMuPtDr1Bhisto->Write();
   LooseMuPtDr2Bhisto->Write();
   LooseMuPtDr3Bhisto->Write();
   TightMuPtDr0Bhisto->Write();
   TightMuPtDr1Bhisto->Write();
   TightMuPtDr2Bhisto->Write();
   TightMuPtDr3Bhisto->Write();
   LooseFakeMuPtDr0Bhisto->Write();
   LooseFakeMuPtDr1Bhisto->Write();
   LooseFakeMuPtDr2Bhisto->Write();
   LooseFakeMuPtDr3Bhisto->Write();
   TightFakeMuPtDr0Bhisto->Write();
   TightFakeMuPtDr1Bhisto->Write();
   TightFakeMuPtDr2Bhisto->Write();
   TightFakeMuPtDr3Bhisto->Write();
   LoosePromptMuPtDr0Bhisto->Write();
   LoosePromptMuPtDr1Bhisto->Write();
   LoosePromptMuPtDr2Bhisto->Write();
   LoosePromptMuPtDr3Bhisto->Write();
   TightPromptMuPtDr0Bhisto->Write();
   TightPromptMuPtDr1Bhisto->Write();
   TightPromptMuPtDr2Bhisto->Write();
   TightPromptMuPtDr3Bhisto->Write();
   //
   LooseMuPtDr0Ehisto->Write();
   LooseMuPtDr1Ehisto->Write();
   LooseMuPtDr2Ehisto->Write();
   LooseMuPtDr3Ehisto->Write();
   TightMuPtDr0Ehisto->Write();
   TightMuPtDr1Ehisto->Write();
   TightMuPtDr2Ehisto->Write();
   TightMuPtDr3Ehisto->Write();
   LooseFakeMuPtDr0Ehisto->Write();
   LooseFakeMuPtDr1Ehisto->Write();
   LooseFakeMuPtDr2Ehisto->Write();
   LooseFakeMuPtDr3Ehisto->Write();
   TightFakeMuPtDr0Ehisto->Write();
   TightFakeMuPtDr1Ehisto->Write();
   TightFakeMuPtDr2Ehisto->Write();
   TightFakeMuPtDr3Ehisto->Write();
   LoosePromptMuPtDr0Ehisto->Write();
   LoosePromptMuPtDr1Ehisto->Write();
   LoosePromptMuPtDr2Ehisto->Write();
   LoosePromptMuPtDr3Ehisto->Write();
   TightPromptMuPtDr0Ehisto->Write();
   TightPromptMuPtDr1Ehisto->Write();
   TightPromptMuPtDr2Ehisto->Write();
   TightPromptMuPtDr3Ehisto->Write();



   //////////
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
// // //
