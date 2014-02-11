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
#include "LAC/ADDON5_LAC.C"
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
   /////ObjDef  = "LI";
   ObjDef  = "LI_VLIforTau";
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
   TH1D* STwFakeshisto        = new TH1D("STwFakeshisto", "STwFakeshisto", 5000,0,5000);//fake
   TH1D* STwRealshisto        = new TH1D("STwRealshisto", "STwRealshisto", 5000,0,5000);//fake
   TH1D* STotherhisto         = new TH1D("STotherhisto", "STotherhisto", 5000,0,5000);//fake
   TH1D* FakesDistrhisto      = new TH1D("FakesDistrhisto", "FakesDistrhisto",7,0.25,3.75);//fake
   TH1D* SJThisto             = new TH1D("SJThisto","SJThisto",5000,0,5000);
   TH1D* METhisto             = new TH1D("METhisto","METhisto",2000,0,2000);
   TH1D* PZetahisto           = new TH1D("PZetahisto","PZetahisto",8000,-2000,2000);
   TH1D* TauJetDeltaRminhisto = new TH1D("TauJetDeltaRminhisto","TauJetDeltaRminhisto",80,0,4);
   //
   TH1D* MaxDiLepMasshisto     = new TH1D("MaxDiLepMasshisto",    "MaxDiLepMasshisto",    2000,0,2000);
   TH1D* AllMuMuInvMasshisto   = new TH1D("AllMuMuInvMasshisto",  "AllMuMuInvMasshisto",  2000,0,2000);
   TH1D* AllMuTauInvMasshisto  = new TH1D("AllMuTauInvMasshisto", "AllMuTauInvMasshisto", 2000,0,2000);
   TH1D* MaxMuTauInvMasshisto  = new TH1D("MaxMuTauInvMasshisto","MaxMuTauInvMasshisto",  2000,0,2000);
   TH1D* AllTauTauInvMasshisto = new TH1D("AllTauTauInvMasshisto","AllTauTauInvMasshisto",2000,0,2000);
   //
   TH1D* METmuTransMasshisto  = new TH1D("METmuTransMasshisto", "METmuTransMasshisto",500,0,1000);
   TH1D* METtauTransMasshisto = new TH1D("METtauTransMasshisto","METtauTransMasshisto",500,0,1000);
   TH1D* JetLepMetVectorPthisto = new TH1D("JetLepMetVectorPthisto","JetLepMetVectorPthisto",1000,0,1000);
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
   TH1D* TauJetPthisto   = new TH1D("TauJetPthisto",  "TauJetPthisto",  200, 0,1000);
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
   TH1D* LooseTauJetPthisto  = new TH1D("LooseTauJetPthisto", "LooseTauJetPthisto", 2000,0,2000);
   TH1D* TightTauJetPthisto  = new TH1D("TightTauJetPthisto", "TightTauJetPthisto", 2000,0,2000);
   TH1D* LooseTauPthisto  = new TH1D("LooseTauPthisto", "LooseTauPthisto", 2000,0,2000);
   TH1D* TightTauPthisto  = new TH1D("TightTauPthisto", "TightTauPthisto", 2000,0,2000);
   TH1D* LooseTauEtahisto = new TH1D("LooseTauEtahisto","LooseTauEtahisto",500,-2.5,2.5);
   TH1D* TightTauEtahisto = new TH1D("TightTauEtahisto","TightTauEtahisto",500,-2.5,2.5);
   TH1D* LooseTauPhihisto = new TH1D("LooseTauPhihisto","LooseTauPhihisto",700,-3.5,3.5);
   TH1D* TightTauPhihisto = new TH1D("TightTauPhihisto","TightTauPhihisto",700,-3.5,3.5);
   TH1D* LooseTauJetDeltaRminhisto = new TH1D("LooseTauJetDeltaRminhisto","LooseTauJetDeltaRminhisto",80,0,4);
   TH1D* TightTauJetDeltaRminhisto = new TH1D("TightTauJetDeltaRminhisto","TightTauJetDeltaRminhisto",80,0,4);
   TH1D* LooseTauMuInvMasshisto = new TH1D("LooseTauMuInvMasshisto","LooseTauMuInvMasshisto",200,0,200);
   TH1D* TightTauMuInvMasshisto = new TH1D("TightTauMuInvMasshisto","TightTauMuInvMasshisto",200,0,200);
   //
   TH1D* LooseFakeTauJetPthisto  = new TH1D("LooseFakeTauJetPthisto", "LooseFakeTauJetPthisto", 2000,0,2000);
   TH1D* TightFakeTauJetPthisto  = new TH1D("TightFakeTauJetPthisto", "TightFakeTauJetPthisto", 2000,0,2000);
   TH1D* LooseFakeTauPthisto  = new TH1D("LooseFakeTauPthisto", "LooseFakeTauPthisto", 2000,0,2000);
   TH1D* TightFakeTauPthisto  = new TH1D("TightFakeTauPthisto", "TightFakeTauPthisto", 2000,0,2000);
   TH1D* LooseFakeTauEtahisto = new TH1D("LooseFakeTauEtahisto","LooseFakeTauEtahisto",500,-2.5,2.5);
   TH1D* TightFakeTauEtahisto = new TH1D("TightFakeTauEtahisto","TightFakeTauEtahisto",500,-2.5,2.5);
   TH1D* LooseFakeTauPhihisto = new TH1D("LooseFakeTauPhihisto","LooseFakeTauPhihisto",700,-3.5,3.5);
   TH1D* TightFakeTauPhihisto = new TH1D("TightFakeTauPhihisto","TightFakeTauPhihisto",700,-3.5,3.5);
   TH1D* LooseFakeTauJetDeltaRminhisto = new TH1D("LooseFakeTauJetDeltaRminhisto","LooseFakeTauJetDeltaRminhisto",80,0,4);
   TH1D* TightFakeTauJetDeltaRminhisto = new TH1D("TightFakeTauJetDeltaRminhisto","TightFakeTauJetDeltaRminhisto",80,0,4);
   TH1D* LooseFakeTauMuInvMasshisto = new TH1D("LooseFakeTauMuInvMasshisto","LooseFakeTauMuInvMasshisto",200,0,200);
   TH1D* TightFakeTauMuInvMasshisto = new TH1D("TightFakeTauMuInvMasshisto","TightFakeTauMuInvMasshisto",200,0,200);
   //
   TH1D* LoosePromptTauJetPthisto  = new TH1D("LoosePromptTauJetPthisto", "LoosePromptTauJetPthisto", 2000,0,2000);
   TH1D* TightPromptTauJetPthisto  = new TH1D("TightPromptTauJetPthisto", "TightPromptTauJetPthisto", 2000,0,2000);
   TH1D* LoosePromptTauPthisto  = new TH1D("LoosePromptTauPthisto", "LoosePromptTauPthisto", 2000,0,2000);
   TH1D* TightPromptTauPthisto  = new TH1D("TightPromptTauPthisto", "TightPromptTauPthisto", 2000,0,2000);
   TH1D* LoosePromptTauEtahisto = new TH1D("LoosePromptTauEtahisto","LoosePromptTauEtahisto",500,-2.5,2.5);
   TH1D* TightPromptTauEtahisto = new TH1D("TightPromptTauEtahisto","TightPromptTauEtahisto",500,-2.5,2.5);
   TH1D* LoosePromptTauPhihisto = new TH1D("LoosePromptTauPhihisto","LoosePromptTauPhihisto",700,-3.5,3.5);
   TH1D* TightPromptTauPhihisto = new TH1D("TightPromptTauPhihisto","TightPromptTauPhihisto",700,-3.5,3.5);
   TH1D* LoosePromptTauJetDeltaRminhisto = new TH1D("LoosePromptTauJetDeltaRminhisto","LoosePromptTauJetDeltaRminhisto",80,0,4);
   TH1D* TightPromptTauJetDeltaRminhisto = new TH1D("TightPromptTauJetDeltaRminhisto","TightPromptTauJetDeltaRminhisto",80,0,4);
   TH1D* LoosePromptTauMuInvMasshisto = new TH1D("LoosePromptTauMuInvMasshisto","LoosePromptTauMuInvMasshisto",200,0,200);
   TH1D* TightPromptTauMuInvMasshisto = new TH1D("TightPromptTauMuInvMasshisto","TightPromptTauMuInvMasshisto",200,0,200);
   //
   //
   TH3D* LooseTauPt3Dhisto        = new TH3D("LooseTauPt3Dhisto",       "LooseTauPt3Dhisto",       1000,0,1000,200,0,10,2,0,3);
   TH3D* TightTauPt3Dhisto        = new TH3D("TightTauPt3Dhisto",       "TightTauPt3Dhisto",       1000,0,1000,200,0,10,2,0,3);
   TH3D* LooseFakeTauPt3Dhisto    = new TH3D("LooseFakeTauPt3Dhisto",   "LooseFakeTauPt3Dhisto",   1000,0,1000,200,0,10,2,0,3);
   TH3D* TightFakeTauPt3Dhisto    = new TH3D("TightFakeTauPt3Dhisto",   "TightFakeTauPt3Dhisto",   1000,0,1000,200,0,10,2,0,3);
   TH3D* LoosePromptTauPt3Dhisto  = new TH3D("LoosePromptTauPt3Dhisto", "LoosePromptTauPt3Dhisto", 1000,0,1000,200,0,10,2,0,3);
   TH3D* TightPromptTauPt3Dhisto  = new TH3D("TightPromptTauPt3Dhisto", "TightPromptTauPt3Dhisto", 1000,0,1000,200,0,10,2,0,3);
   //
   TH1D* LooseTauPtDr0Bhisto  = new TH1D("LooseTauPtDr0Bhisto", "LooseTauPtDr0Bhisto", 1000,0,1000);
   TH1D* TightTauPtDr0Bhisto  = new TH1D("TightTauPtDr0Bhisto", "TightTauPtDr0Bhisto", 1000,0,1000);
   TH1D* LooseTauPtDr1Bhisto  = new TH1D("LooseTauPtDr1Bhisto", "LooseTauPtDr1Bhisto", 1000,0,1000);
   TH1D* TightTauPtDr1Bhisto  = new TH1D("TightTauPtDr1Bhisto", "TightTauPtDr1Bhisto", 1000,0,1000);
   TH1D* LooseTauPtDr2Bhisto  = new TH1D("LooseTauPtDr2Bhisto", "LooseTauPtDr2Bhisto", 1000,0,1000);
   TH1D* TightTauPtDr2Bhisto  = new TH1D("TightTauPtDr2Bhisto", "TightTauPtDr2Bhisto", 1000,0,1000);
   TH1D* LooseTauPtDr3Bhisto  = new TH1D("LooseTauPtDr3Bhisto", "LooseTauPtDr3Bhisto", 1000,0,1000);
   TH1D* TightTauPtDr3Bhisto  = new TH1D("TightTauPtDr3Bhisto", "TightTauPtDr3Bhisto", 1000,0,1000);
   //
   TH1D* LooseFakeTauPtDr0Bhisto  = new TH1D("LooseFakeTauPtDr0Bhisto", "LooseFakeTauPtDr0Bhisto", 1000,0,1000);
   TH1D* TightFakeTauPtDr0Bhisto  = new TH1D("TightFakeTauPtDr0Bhisto", "TightFakeTauPtDr0Bhisto", 1000,0,1000);
   TH1D* LooseFakeTauPtDr1Bhisto  = new TH1D("LooseFakeTauPtDr1Bhisto", "LooseFakeTauPtDr1Bhisto", 1000,0,1000);
   TH1D* TightFakeTauPtDr1Bhisto  = new TH1D("TightFakeTauPtDr1Bhisto", "TightFakeTauPtDr1Bhisto", 1000,0,1000);
   TH1D* LooseFakeTauPtDr2Bhisto  = new TH1D("LooseFakeTauPtDr2Bhisto", "LooseFakeTauPtDr2Bhisto", 1000,0,1000);
   TH1D* TightFakeTauPtDr2Bhisto  = new TH1D("TightFakeTauPtDr2Bhisto", "TightFakeTauPtDr2Bhisto", 1000,0,1000);
   TH1D* LooseFakeTauPtDr3Bhisto  = new TH1D("LooseFakeTauPtDr3Bhisto", "LooseFakeTauPtDr3Bhisto", 1000,0,1000);
   TH1D* TightFakeTauPtDr3Bhisto  = new TH1D("TightFakeTauPtDr3Bhisto", "TightFakeTauPtDr3Bhisto", 1000,0,1000);
   //
   TH1D* LoosePromptTauPtDr0Bhisto  = new TH1D("LoosePromptTauPtDr0Bhisto", "LoosePromptTauPtDr0Bhisto", 1000,0,1000);
   TH1D* TightPromptTauPtDr0Bhisto  = new TH1D("TightPromptTauPtDr0Bhisto", "TightPromptTauPtDr0Bhisto", 1000,0,1000);
   TH1D* LoosePromptTauPtDr1Bhisto  = new TH1D("LoosePromptTauPtDr1Bhisto", "LoosePromptTauPtDr1Bhisto", 1000,0,1000);
   TH1D* TightPromptTauPtDr1Bhisto  = new TH1D("TightPromptTauPtDr1Bhisto", "TightPromptTauPtDr1Bhisto", 1000,0,1000);
   TH1D* LoosePromptTauPtDr2Bhisto  = new TH1D("LoosePromptTauPtDr2Bhisto", "LoosePromptTauPtDr2Bhisto", 1000,0,1000);
   TH1D* TightPromptTauPtDr2Bhisto  = new TH1D("TightPromptTauPtDr2Bhisto", "TightPromptTauPtDr2Bhisto", 1000,0,1000);
   TH1D* LoosePromptTauPtDr3Bhisto  = new TH1D("LoosePromptTauPtDr3Bhisto", "LoosePromptTauPtDr3Bhisto", 1000,0,1000);
   TH1D* TightPromptTauPtDr3Bhisto  = new TH1D("TightPromptTauPtDr3Bhisto", "TightPromptTauPtDr3Bhisto", 1000,0,1000);
   //
   TH1D* LooseTauPtDr0Ehisto  = new TH1D("LooseTauPtDr0Ehisto", "LooseTauPtDr0Ehisto", 1000,0,1000);
   TH1D* TightTauPtDr0Ehisto  = new TH1D("TightTauPtDr0Ehisto", "TightTauPtDr0Ehisto", 1000,0,1000);
   TH1D* LooseTauPtDr1Ehisto  = new TH1D("LooseTauPtDr1Ehisto", "LooseTauPtDr1Ehisto", 1000,0,1000);
   TH1D* TightTauPtDr1Ehisto  = new TH1D("TightTauPtDr1Ehisto", "TightTauPtDr1Ehisto", 1000,0,1000);
   TH1D* LooseTauPtDr2Ehisto  = new TH1D("LooseTauPtDr2Ehisto", "LooseTauPtDr2Ehisto", 1000,0,1000);
   TH1D* TightTauPtDr2Ehisto  = new TH1D("TightTauPtDr2Ehisto", "TightTauPtDr2Ehisto", 1000,0,1000);
   TH1D* LooseTauPtDr3Ehisto  = new TH1D("LooseTauPtDr3Ehisto", "LooseTauPtDr3Ehisto", 1000,0,1000);
   TH1D* TightTauPtDr3Ehisto  = new TH1D("TightTauPtDr3Ehisto", "TightTauPtDr3Ehisto", 1000,0,1000);
   //
   TH1D* LooseFakeTauPtDr0Ehisto  = new TH1D("LooseFakeTauPtDr0Ehisto", "LooseFakeTauPtDr0Ehisto", 1000,0,1000);
   TH1D* TightFakeTauPtDr0Ehisto  = new TH1D("TightFakeTauPtDr0Ehisto", "TightFakeTauPtDr0Ehisto", 1000,0,1000);
   TH1D* LooseFakeTauPtDr1Ehisto  = new TH1D("LooseFakeTauPtDr1Ehisto", "LooseFakeTauPtDr1Ehisto", 1000,0,1000);
   TH1D* TightFakeTauPtDr1Ehisto  = new TH1D("TightFakeTauPtDr1Ehisto", "TightFakeTauPtDr1Ehisto", 1000,0,1000);
   TH1D* LooseFakeTauPtDr2Ehisto  = new TH1D("LooseFakeTauPtDr2Ehisto", "LooseFakeTauPtDr2Ehisto", 1000,0,1000);
   TH1D* TightFakeTauPtDr2Ehisto  = new TH1D("TightFakeTauPtDr2Ehisto", "TightFakeTauPtDr2Ehisto", 1000,0,1000);
   TH1D* LooseFakeTauPtDr3Ehisto  = new TH1D("LooseFakeTauPtDr3Ehisto", "LooseFakeTauPtDr3Ehisto", 1000,0,1000);
   TH1D* TightFakeTauPtDr3Ehisto  = new TH1D("TightFakeTauPtDr3Ehisto", "TightFakeTauPtDr3Ehisto", 1000,0,1000);
   //
   TH1D* LoosePromptTauPtDr0Ehisto  = new TH1D("LoosePromptTauPtDr0Ehisto", "LoosePromptTauPtDr0Ehisto", 1000,0,1000);
   TH1D* TightPromptTauPtDr0Ehisto  = new TH1D("TightPromptTauPtDr0Ehisto", "TightPromptTauPtDr0Ehisto", 1000,0,1000);
   TH1D* LoosePromptTauPtDr1Ehisto  = new TH1D("LoosePromptTauPtDr1Ehisto", "LoosePromptTauPtDr1Ehisto", 1000,0,1000);
   TH1D* TightPromptTauPtDr1Ehisto  = new TH1D("TightPromptTauPtDr1Ehisto", "TightPromptTauPtDr1Ehisto", 1000,0,1000);
   TH1D* LoosePromptTauPtDr2Ehisto  = new TH1D("LoosePromptTauPtDr2Ehisto", "LoosePromptTauPtDr2Ehisto", 1000,0,1000);
   TH1D* TightPromptTauPtDr2Ehisto  = new TH1D("TightPromptTauPtDr2Ehisto", "TightPromptTauPtDr2Ehisto", 1000,0,1000);
   TH1D* LoosePromptTauPtDr3Ehisto  = new TH1D("LoosePromptTauPtDr3Ehisto", "LoosePromptTauPtDr3Ehisto", 1000,0,1000);
   TH1D* TightPromptTauPtDr3Ehisto  = new TH1D("TightPromptTauPtDr3Ehisto", "TightPromptTauPtDr3Ehisto", 1000,0,1000);
   //
   // Replace Pt with PileUp, PT CUT >35 GeV
   TH3D* LooseTauPU3Dhisto        = new TH3D("LooseTauPU3Dhisto",       "LooseTauPU3Dhisto",       62,-0.5,61.5,200,0,10,2,0,3);
   TH3D* TightTauPU3Dhisto        = new TH3D("TightTauPU3Dhisto",       "TightTauPU3Dhisto",       62,-0.5,61.5,200,0,10,2,0,3);
   TH3D* LooseFakeTauPU3Dhisto    = new TH3D("LooseFakeTauPU3Dhisto",   "LooseFakeTauPU3Dhisto",   62,-0.5,61.5,200,0,10,2,0,3);
   TH3D* TightFakeTauPU3Dhisto    = new TH3D("TightFakeTauPU3Dhisto",   "TightFakeTauPU3Dhisto",   62,-0.5,61.5,200,0,10,2,0,3);
   TH3D* LoosePromptTauPU3Dhisto  = new TH3D("LoosePromptTauPU3Dhisto", "LoosePromptTauPU3Dhisto", 62,-0.5,61.5,200,0,10,2,0,3);
   TH3D* TightPromptTauPU3Dhisto  = new TH3D("TightPromptTauPU3Dhisto", "TightPromptTauPU3Dhisto", 62,-0.5,61.5,200,0,10,2,0,3);
   //
   TH1D* LooseTauPUDr0Bhisto  = new TH1D("LooseTauPUDr0Bhisto", "LooseTauPUDr0Bhisto", 62,-0.5,61.5);
   TH1D* TightTauPUDr0Bhisto  = new TH1D("TightTauPUDr0Bhisto", "TightTauPUDr0Bhisto", 62,-0.5,61.5);
   TH1D* LooseTauPUDr1Bhisto  = new TH1D("LooseTauPUDr1Bhisto", "LooseTauPUDr1Bhisto", 62,-0.5,61.5);
   TH1D* TightTauPUDr1Bhisto  = new TH1D("TightTauPUDr1Bhisto", "TightTauPUDr1Bhisto", 62,-0.5,61.5);
   TH1D* LooseTauPUDr2Bhisto  = new TH1D("LooseTauPUDr2Bhisto", "LooseTauPUDr2Bhisto", 62,-0.5,61.5);
   TH1D* TightTauPUDr2Bhisto  = new TH1D("TightTauPUDr2Bhisto", "TightTauPUDr2Bhisto", 62,-0.5,61.5);
   TH1D* LooseTauPUDr3Bhisto  = new TH1D("LooseTauPUDr3Bhisto", "LooseTauPUDr3Bhisto", 62,-0.5,61.5);
   TH1D* TightTauPUDr3Bhisto  = new TH1D("TightTauPUDr3Bhisto", "TightTauPUDr3Bhisto", 62,-0.5,61.5);
   //
   TH1D* LooseFakeTauPUDr0Bhisto  = new TH1D("LooseFakeTauPUDr0Bhisto", "LooseFakeTauPUDr0Bhisto", 62,-0.5,61.5);
   TH1D* TightFakeTauPUDr0Bhisto  = new TH1D("TightFakeTauPUDr0Bhisto", "TightFakeTauPUDr0Bhisto", 62,-0.5,61.5);
   TH1D* LooseFakeTauPUDr1Bhisto  = new TH1D("LooseFakeTauPUDr1Bhisto", "LooseFakeTauPUDr1Bhisto", 62,-0.5,61.5);
   TH1D* TightFakeTauPUDr1Bhisto  = new TH1D("TightFakeTauPUDr1Bhisto", "TightFakeTauPUDr1Bhisto", 62,-0.5,61.5);
   TH1D* LooseFakeTauPUDr2Bhisto  = new TH1D("LooseFakeTauPUDr2Bhisto", "LooseFakeTauPUDr2Bhisto", 62,-0.5,61.5);
   TH1D* TightFakeTauPUDr2Bhisto  = new TH1D("TightFakeTauPUDr2Bhisto", "TightFakeTauPUDr2Bhisto", 62,-0.5,61.5);
   TH1D* LooseFakeTauPUDr3Bhisto  = new TH1D("LooseFakeTauPUDr3Bhisto", "LooseFakeTauPUDr3Bhisto", 62,-0.5,61.5);
   TH1D* TightFakeTauPUDr3Bhisto  = new TH1D("TightFakeTauPUDr3Bhisto", "TightFakeTauPUDr3Bhisto", 62,-0.5,61.5);
   //
   TH1D* LoosePromptTauPUDr0Bhisto  = new TH1D("LoosePromptTauPUDr0Bhisto", "LoosePromptTauPUDr0Bhisto", 62,-0.5,61.5);
   TH1D* TightPromptTauPUDr0Bhisto  = new TH1D("TightPromptTauPUDr0Bhisto", "TightPromptTauPUDr0Bhisto", 62,-0.5,61.5);
   TH1D* LoosePromptTauPUDr1Bhisto  = new TH1D("LoosePromptTauPUDr1Bhisto", "LoosePromptTauPUDr1Bhisto", 62,-0.5,61.5);
   TH1D* TightPromptTauPUDr1Bhisto  = new TH1D("TightPromptTauPUDr1Bhisto", "TightPromptTauPUDr1Bhisto", 62,-0.5,61.5);
   TH1D* LoosePromptTauPUDr2Bhisto  = new TH1D("LoosePromptTauPUDr2Bhisto", "LoosePromptTauPUDr2Bhisto", 62,-0.5,61.5);
   TH1D* TightPromptTauPUDr2Bhisto  = new TH1D("TightPromptTauPUDr2Bhisto", "TightPromptTauPUDr2Bhisto", 62,-0.5,61.5);
   TH1D* LoosePromptTauPUDr3Bhisto  = new TH1D("LoosePromptTauPUDr3Bhisto", "LoosePromptTauPUDr3Bhisto", 62,-0.5,61.5);
   TH1D* TightPromptTauPUDr3Bhisto  = new TH1D("TightPromptTauPUDr3Bhisto", "TightPromptTauPUDr3Bhisto", 62,-0.5,61.5);
   //
   TH1D* LooseTauPUDr0Ehisto  = new TH1D("LooseTauPUDr0Ehisto", "LooseTauPUDr0Ehisto", 62,-0.5,61.5);
   TH1D* TightTauPUDr0Ehisto  = new TH1D("TightTauPUDr0Ehisto", "TightTauPUDr0Ehisto", 62,-0.5,61.5);
   TH1D* LooseTauPUDr1Ehisto  = new TH1D("LooseTauPUDr1Ehisto", "LooseTauPUDr1Ehisto", 62,-0.5,61.5);
   TH1D* TightTauPUDr1Ehisto  = new TH1D("TightTauPUDr1Ehisto", "TightTauPUDr1Ehisto", 62,-0.5,61.5);
   TH1D* LooseTauPUDr2Ehisto  = new TH1D("LooseTauPUDr2Ehisto", "LooseTauPUDr2Ehisto", 62,-0.5,61.5);
   TH1D* TightTauPUDr2Ehisto  = new TH1D("TightTauPUDr2Ehisto", "TightTauPUDr2Ehisto", 62,-0.5,61.5);
   TH1D* LooseTauPUDr3Ehisto  = new TH1D("LooseTauPUDr3Ehisto", "LooseTauPUDr3Ehisto", 62,-0.5,61.5);
   TH1D* TightTauPUDr3Ehisto  = new TH1D("TightTauPUDr3Ehisto", "TightTauPUDr3Ehisto", 62,-0.5,61.5);
   //
   TH1D* LooseFakeTauPUDr0Ehisto  = new TH1D("LooseFakeTauPUDr0Ehisto", "LooseFakeTauPUDr0Ehisto", 62,-0.5,61.5);
   TH1D* TightFakeTauPUDr0Ehisto  = new TH1D("TightFakeTauPUDr0Ehisto", "TightFakeTauPUDr0Ehisto", 62,-0.5,61.5);
   TH1D* LooseFakeTauPUDr1Ehisto  = new TH1D("LooseFakeTauPUDr1Ehisto", "LooseFakeTauPUDr1Ehisto", 62,-0.5,61.5);
   TH1D* TightFakeTauPUDr1Ehisto  = new TH1D("TightFakeTauPUDr1Ehisto", "TightFakeTauPUDr1Ehisto", 62,-0.5,61.5);
   TH1D* LooseFakeTauPUDr2Ehisto  = new TH1D("LooseFakeTauPUDr2Ehisto", "LooseFakeTauPUDr2Ehisto", 62,-0.5,61.5);
   TH1D* TightFakeTauPUDr2Ehisto  = new TH1D("TightFakeTauPUDr2Ehisto", "TightFakeTauPUDr2Ehisto", 62,-0.5,61.5);
   TH1D* LooseFakeTauPUDr3Ehisto  = new TH1D("LooseFakeTauPUDr3Ehisto", "LooseFakeTauPUDr3Ehisto", 62,-0.5,61.5);
   TH1D* TightFakeTauPUDr3Ehisto  = new TH1D("TightFakeTauPUDr3Ehisto", "TightFakeTauPUDr3Ehisto", 62,-0.5,61.5);
   //
   TH1D* LoosePromptTauPUDr0Ehisto  = new TH1D("LoosePromptTauPUDr0Ehisto", "LoosePromptTauPUDr0Ehisto", 62,-0.5,61.5);
   TH1D* TightPromptTauPUDr0Ehisto  = new TH1D("TightPromptTauPUDr0Ehisto", "TightPromptTauPUDr0Ehisto", 62,-0.5,61.5);
   TH1D* LoosePromptTauPUDr1Ehisto  = new TH1D("LoosePromptTauPUDr1Ehisto", "LoosePromptTauPUDr1Ehisto", 62,-0.5,61.5);
   TH1D* TightPromptTauPUDr1Ehisto  = new TH1D("TightPromptTauPUDr1Ehisto", "TightPromptTauPUDr1Ehisto", 62,-0.5,61.5);
   TH1D* LoosePromptTauPUDr2Ehisto  = new TH1D("LoosePromptTauPUDr2Ehisto", "LoosePromptTauPUDr2Ehisto", 62,-0.5,61.5);
   TH1D* TightPromptTauPUDr2Ehisto  = new TH1D("TightPromptTauPUDr2Ehisto", "TightPromptTauPUDr2Ehisto", 62,-0.5,61.5);
   TH1D* LoosePromptTauPUDr3Ehisto  = new TH1D("LoosePromptTauPUDr3Ehisto", "LoosePromptTauPUDr3Ehisto", 62,-0.5,61.5);
   TH1D* TightPromptTauPUDr3Ehisto  = new TH1D("TightPromptTauPUDr3Ehisto", "TightPromptTauPUDr3Ehisto", 62,-0.5,61.5);


   //
   TH1D* AllGenhisto  = new TH1D("AllGenhisto","AllGenhisto",4,0.5,4.5);//MuTau
   TH1D* TTGenhisto   = new TH1D("TTGenhisto","TTGenhisto",4,0.5,4.5);//LL LT TL TT
   //
   TH1D* LTmatrixhisto = new TH1D("LTmatrixhisto","LTmatrixhisto",4,0.5,4.5);
   TH1D* FPmatrixError2histo = new TH1D("FPmatrixError2histo","FPmatrixError2histo",4,0.5,4.5);
   TH1D* FPmatrixhisto = new TH1D("FPmatrixhisto","FPmatrixhisto",4,0.5,4.5);
   TH1D* TTBackgroundError2histo = new TH1D("TTBackgroundError2histo","TTBackgroundError2histo",4,0.5,4.5);
   TH1D* TTBackgroundhisto = new TH1D("TTBackgroundhisto","TTBackgroundhisto",4,0.5,4.5);
   LTmatrixhisto->GetXaxis()->SetBinLabel( 1, "LL" );   LTmatrixhisto->GetXaxis()->SetBinLabel( 2, "LT" );
   LTmatrixhisto->GetXaxis()->SetBinLabel( 3, "TL" );   LTmatrixhisto->GetXaxis()->SetBinLabel( 4, "TT" );
   FPmatrixhisto->GetXaxis()->SetBinLabel( 1, "FF" );   FPmatrixhisto->GetXaxis()->SetBinLabel( 2, "FP" );
   FPmatrixhisto->GetXaxis()->SetBinLabel( 3, "PF" );   FPmatrixhisto->GetXaxis()->SetBinLabel( 4, "" );
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
     // Define Systematic Scheme to be used:
     ApplyLepJetMetSystematics(0);//this is no correction, should be used if object pt functions in this code are to be kept as is..

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

     unsigned int triggerMuon=99;

     for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
       if( !muRisoCheck(iMuR) ) continue;
       if( !muRTightCheck(iMuR) ) continue; // Tight PFiso (for ZtoMuTau selection)
       triggerMuon=iMuR;
       break;//take the leading one
     }
     //}
     //if( isSS_ ) triggerMuon=ltemMuTau[0];
     // ---- trigger
     int usedTrigger_=-5;
     usedTrigger_ = SingleMu_passTrigger();
     double TriggerEfficiencyWeights_;
     //
     if( triggerMuon!=99 ){
       TriggerEfficiencyWeights_=IsoMu24e2p1_Eff( MuonPt->at(triggerMuon), MuonEta->at(triggerMuon) );
       AppliedTrigEffWeightshisto->Fill( TriggerEfficiencyWeights_ );
     }
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
     fillVariableWithValue("PassTrig", passTrigger_ );//...// returns -2, -1, 0, trig                                                                                   

     //  -- EVENT FILTER SKIM
     int passEventFilter_=0;
     if( passEventFilter() ) passEventFilter_ = 1;
     fillVariableWithValue("PassEventFilter", passEventFilter_ );//.// returns 0, 1 


     //  -- DILEPTON SKIM - MuTau
     int isOfflineDilepton_=0;
     // edit
     if( isMuTauDR0p30() && ST()>100 ) isOfflineDilepton_ = 1;
     fillVariableWithValue("PassOfflineDilepton", isOfflineDilepton_ );// Returns 0, 1          


     // -- HLT MATCHING SKIM
     int passAllMuHLTmatching_=0;
     if(  triggerMuon!=99 && RecoHLTdeltaRmin_SingleMuTrigger(triggerMuon)<0.15              ) passAllMuHLTmatching_=1;
     fillVariableWithValue("PassAllMuHLTmatching",passAllMuHLTmatching_);


     // -- OFFLINE SKIM
     //-----------------------------------------------------------------------------------------------------------------------------------------//
     // Computing Pzeta quantity
     unsigned int tagMuon=99;
     for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
       if( !muRisoCheck(iMuR)   ) continue; //Tight Muon POG ID -- no isolation
       if( !muRTightCheck(iMuR) ) continue; // Tight PFiso (for ZtoMuTau selection)
       tagMuon=iMuR;
       break; //there should be one loose (and then tight)  muon anyways
     }
     unsigned int probeTau=99;
     for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
       if( !tauRisoCheck(iTauR)   ) continue;
       //
       probeTau=iTauR;
       break;//can be more than one tau, pick the highest Pt one.
     }
     //
     // SKIP EVENTS WITH NO GOOD TAU or GOOD MUON
     if( probeTau==99 || tagMuon==99 ) continue;
     //
     //
     //computing pZeta
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
     double pZeta=((pz2D.Mod())*(zeta2D.Mod())*TMath::Cos(pz2D.DeltaPhi(zeta2D)))-1.5*((pzvis2D.Mod())*(zeta2D.Mod())*TMath::Cos(pzvis2D.DeltaPhi(zeta2D)));

     //computing JetSys+Mu+Tau Pt
     TLorentzVector jetVector, jetSysVector, muVector, tauVector, metVector;
     muVector.SetPtEtaPhiM(  MuonPt->at(tagMuon),     MuonEta->at(tagMuon),    MuonPhi->at(tagMuon),       0 );
     tauVector.SetPtEtaPhiM( HPSTauPt->at(probeTau),  HPSTauEta->at(probeTau), HPSTauPhi->at(probeTau),    0 );
     metVector.SetPtEtaPhiM( PFMETType01XYCor->at(0), 0,                       PFMETPhiType01XYCor->at(0), 0 );
     jetSysVector.SetPtEtaPhiM( 0 , 0 , 0 , 0 );
     for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
       jetVector.SetPtEtaPhiM( PFJetPt->at(iJetR), PFJetEta->at(iJetR), PFJetPhi->at(iJetR), 0 );
       jetSysVector+=jetVector;
     }
     double JetLepMetVectorPt = (jetSysVector+muVector+tauVector+metVector).Pt();

     //-----------------------------------------------------------------------------------------------------------------------------------------//
     int OfflineCuts_=0;
     int LTEM_MuTau_ = LTEM_MuTau(); 
     //int  NLepJet= MuCounter()+ElCounter()+TauCounter()+JetCounter();
     TLorentzVector tagMuonVector, probeTauVector;
     tagMuonVector.SetPtEtaPhiM(  MuonPt->at(tagMuon),     MuonEta->at(tagMuon),    MuonPhi->at(tagMuon),    0 );
     probeTauVector.SetPtEtaPhiM( HPSTauPt ->at(probeTau), HPSTauEta->at(probeTau), HPSTauPhi->at(probeTau), 0 );
     double mutauInvMass_=(tagMuonVector+probeTauVector).M();
     // need to select one of these:
     // edit 
     if( triggerMuon!=99 //these two should be the same
	 && tagMuon!=99  // tagmuon = triggermuon
	 && MuCounter()==1 
	 && MuonPt->at(tagMuon)>35
	 && TauCounter()==1 
	 && ElCounter()==0 
	 && probeTau!=99
	 && JetCounter()<2 && ST()>100
	 //&& JetCounter()<2 && JetCounter()>0 && ST()>100
	 && pZeta<-10 
	 && METlepMT("Mu",triggerMuon)>50
	 //&& METlepMT("Mu",triggerMuon)<40 //&& METlepMT("Mu",triggerMuon)<130 //&& tauJetDeltaRmin(probeTau)<0.8
	 //&& fabs(HPSTauCharge->at(probeTau)+MuonCharge->at(tagMuon))==0//OS selection
	 && fabs(HPSTauCharge->at(probeTau)+MuonCharge->at(tagMuon))==2//SS selection
	 ) OfflineCuts_=1;
     fillVariableWithValue("PassOfflineCuts", OfflineCuts_ );// returns 0, 1 

     evaluateCuts();
     //---------------------------------------------------------------------------------
     if( passedCut("PassJSON") && passedCut("PassTrig") && passedCut("PassEventFilter") && passedCut("PassOfflineDilepton") &&
	 passedCut("PassAllMuHLTmatching") && passedCut("PassOfflineCuts") ){ 

       if( triggerMuon!=tagMuon ) cout<<"triggerMuon!=tagMuon"<<endl;

       // Weight Histos
       Triggerhisto->Fill( usedTrigger_ );
       AppliedPileUpWeightshisto->Fill( safePileupWeights_ );
       AppliedTotalWeightshisto->Fill( w );


       PZetahisto->Fill( pZeta, w );
       TauJetDeltaRminhisto->Fill( tauJetDeltaRmin(probeTau), w );
       
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
	 //
	 TLorentzVector TauVec, JetVec;
	 TauVec.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
	 double minTauJetDeltaR=9999;
	 unsigned int iTauJet=99;
	 for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
	   JetVec.SetPtEtaPhiM( PFJetPt->at(iJetR), PFJetEta->at(iJetR), PFJetPhi->at(iJetR), 0 );
	   if( minTauJetDeltaR>TauVec.DeltaR(JetVec) && TauVec.DeltaR(JetVec)<0.5 ){ minTauJetDeltaR=TauVec.DeltaR(JetVec); iTauJet=iJetR; }
	 }
	 //
	 if( iTauJet==99 ) cout<<"NoJET! TauPt: "<<HPSTauPt->at(iTauR)<<endl;
	 if( probeTau==iTauR  && iTauJet!=99 ){ LooseTauJetPthisto->Fill(  PFJetPt->at(iTauJet), w ); }
	 if( probeTau==iTauR  && iTauJet!=99 && tauRLooseCheck(probeTau) ){ TightTauJetPthisto->Fill(  PFJetPt->at(iTauJet), w ); }
	 if( probeTau==iTauR && !isRecoTauPrompt(probeTau)  && iTauJet!=99 ){
	   LooseFakeTauJetPthisto->Fill(  PFJetPt->at(iTauJet), w );
	   if( tauRLooseCheck(probeTau) ){
	     TightFakeTauJetPthisto->Fill(  PFJetPt->at(iTauJet), w );
	   }
	 }
         if( probeTau==iTauR && isRecoTauPrompt(probeTau) && iTauJet!=99 ){
           LoosePromptTauJetPthisto->Fill(  PFJetPt->at(iTauJet), w );
           if( tauRLooseCheck(probeTau) ){
             TightPromptTauJetPthisto->Fill(  PFJetPt->at(iTauJet), w );
           }
         }
	 //
	 if( iTauJet!=99 ) TauJetPthisto->Fill( PFJetPt->at(iTauJet), w );
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

       //--------------------------------------------------------------
       double tauJetDeltaRmin_ = tauJetDeltaRmin(probeTau);
       TLorentzVector tagMuonVec, probeTauVec;
       tagMuonVec.SetPtEtaPhiM(  MuonPt->at(tagMuon),     MuonEta->at(tagMuon),    MuonPhi->at(tagMuon),    0 );
       probeTauVec.SetPtEtaPhiM( HPSTauPt ->at(probeTau), HPSTauEta->at(probeTau), HPSTauPhi->at(probeTau), 0 );
       double mutauInvMass=(tagMuonVec+probeTauVec).M();

       // Loose - Tight Tau Histograms -- 1D
       LooseTauPthisto->Fill(  HPSTauPt ->at(probeTau), w );
       LooseTauEtahisto->Fill( fabs(HPSTauEta->at(probeTau)), w );
       if( tauRLooseCheck(probeTau) ){
	 TightTauPthisto->Fill(  HPSTauPt ->at(probeTau), w );
	 TightTauEtahisto->Fill( fabs(HPSTauEta->at(probeTau)), w );
       }
       // Fake Tau
       if( !isRecoTauPrompt(probeTau) ){
	 LooseFakeTauPthisto->Fill(  HPSTauPt ->at(probeTau), w );
	 LooseFakeTauEtahisto->Fill( fabs(HPSTauEta->at(probeTau)), w );
	 if( tauRLooseCheck(probeTau) ){
	   TightFakeTauPthisto->Fill(  HPSTauPt ->at(probeTau), w );
	   TightFakeTauEtahisto->Fill( fabs(HPSTauEta->at(probeTau)), w );
	 }
       }
       // Prompt Tau
       if( isRecoTauPrompt(probeTau) ){
	 LoosePromptTauPthisto->Fill(  HPSTauPt ->at(probeTau), w );
	 LoosePromptTauEtahisto->Fill( fabs(HPSTauEta->at(probeTau)), w );
	 if( tauRLooseCheck(probeTau) ){
	   TightPromptTauPthisto->Fill(  HPSTauPt ->at(probeTau), w );
	   TightPromptTauEtahisto->Fill( fabs(HPSTauEta->at(probeTau)), w );
	 }
       }

       double probeTauPt_    = HPSTauPt->at(probeTau);
       double probeTauAbsEta_= fabs(HPSTauEta->at(probeTau));
       double probeTauPU_    = (double)(VertexX->size());
       // Loose - Tight Tau Histograms -- 3D
       LooseTauPt3Dhisto->Fill( probeTauPt_, tauJetDeltaRmin_, probeTauAbsEta_, w );
       if( probeTauPt_>35 ) LooseTauPU3Dhisto->Fill( probeTauPU_, tauJetDeltaRmin_, probeTauAbsEta_, w );
       if( tauRLooseCheck(probeTau) ){
	 TightTauPt3Dhisto->Fill( probeTauPt_, tauJetDeltaRmin_, probeTauAbsEta_, w );
	 if( probeTauPt_>35 ) TightTauPU3Dhisto->Fill( probeTauPU_, tauJetDeltaRmin_, probeTauAbsEta_, w );
       }
       // Fake Tau
       if( !isRecoTauPrompt(probeTau) ){
	 LooseFakeTauPt3Dhisto->Fill( probeTauPt_, tauJetDeltaRmin_, probeTauAbsEta_, w );
	 if( probeTauPt_>35 ) LooseFakeTauPU3Dhisto->Fill( probeTauPU_, tauJetDeltaRmin_, probeTauAbsEta_, w );
	 if( tauRLooseCheck(probeTau) ){
	   TightFakeTauPt3Dhisto->Fill( probeTauPt_, tauJetDeltaRmin_, probeTauAbsEta_, w );
	   if( probeTauPt_>35 ) TightFakeTauPU3Dhisto->Fill( probeTauPU_, tauJetDeltaRmin_, probeTauAbsEta_, w );
	 }
       }
       // Prompt Tau
       if( isRecoTauPrompt(probeTau) ){
	 LoosePromptTauPt3Dhisto->Fill( probeTauPt_, tauJetDeltaRmin_, probeTauAbsEta_, w );
	 if( probeTauPt_>35 ) LoosePromptTauPU3Dhisto->Fill( probeTauPU_, tauJetDeltaRmin_, probeTauAbsEta_, w );
	 if( tauRLooseCheck(probeTau) ){
	   TightPromptTauPt3Dhisto->Fill( probeTauPt_, tauJetDeltaRmin_, probeTauAbsEta_, w );
	   if( probeTauPt_>35 ) TightPromptTauPU3Dhisto->Fill( probeTauPU_, tauJetDeltaRmin_, probeTauAbsEta_, w );
	 }
       }

       JetLepMetVectorPthisto->Fill( JetLepMetVectorPt, w );

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
   STwFakeshisto->Write();//fake
   STwRealshisto->Write();//fake
   STotherhisto->Write(); //fake
   FakesDistrhisto->Write();//fake
   SJThisto->Write();
   LeadMuTauDeltaRhisto->Write();
   PZetahisto->Write();
   TauJetDeltaRminhisto->Write();
   JetLepMetVectorPthisto->Write();
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
   TauJetPthisto->Write();
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
   FPmatrixhisto->Write();
   TTBackgroundhisto->Write();
   FPmatrixError2histo->Write();
   TTBackgroundError2histo->Write();
   //
   AllGenhisto->Write();
   TTGenhisto->Write();
   //
   LooseTauJetPthisto->Write();
   LooseTauPthisto->Write();
   LooseTauEtahisto->Write();
   LooseTauPhihisto->Write();
   TightTauJetPthisto->Write();
   TightTauPthisto->Write();
   TightTauEtahisto->Write();
   TightTauPhihisto->Write();
   //
   LooseTauMuInvMasshisto->Write();
   TightTauMuInvMasshisto->Write();
   LooseFakeTauMuInvMasshisto->Write();
   TightFakeTauMuInvMasshisto->Write();
   LoosePromptTauMuInvMasshisto->Write();
   TightPromptTauMuInvMasshisto->Write();
   //
   LoosePromptTauJetPthisto->Write();
   LooseFakeTauJetPthisto->Write();
   TightPromptTauJetPthisto->Write();
   TightFakeTauJetPthisto->Write();
   LooseFakeTauPthisto->Write();
   LooseFakeTauEtahisto->Write();
   LooseFakeTauPhihisto->Write();
   TightFakeTauPthisto->Write();
   TightFakeTauEtahisto->Write();
   TightFakeTauPhihisto->Write();
   LoosePromptTauPthisto->Write();
   LoosePromptTauEtahisto->Write();
   LoosePromptTauPhihisto->Write();
   TightPromptTauPthisto->Write();
   TightPromptTauEtahisto->Write();
   TightPromptTauPhihisto->Write();
   //
   LooseTauJetDeltaRminhisto->Write();
   TightTauJetDeltaRminhisto->Write();
   LooseFakeTauJetDeltaRminhisto->Write();
   TightFakeTauJetDeltaRminhisto->Write();
   LoosePromptTauJetDeltaRminhisto->Write();
   TightPromptTauJetDeltaRminhisto->Write();
   //
   //LooseTauPt3Dhisto->ProjectionX("LooseTauPtDr1Bhisto",11,14, 1,1, "e");//0.5-0.7
   //LooseTauPt3Dhisto->ProjectionX("LooseTauPtDr2Bhisto",15,20, 1,1, "e");//0.7-1.0
   //LooseTauPt3Dhisto->ProjectionX("LooseTauPtDr3Bhisto",21,200,1,1, "e");//1.0>
   //
   //
   LooseTauPt3Dhisto->ProjectionX("LooseTauPtDr0Bhisto",1, 200,1,1, "e");//All
   LooseTauPt3Dhisto->ProjectionX("LooseTauPtDr1Bhisto",15,16, 1,1, "e");//0.7-0.8
   LooseTauPt3Dhisto->ProjectionX("LooseTauPtDr2Bhisto",17,20, 1,1, "e");//0.8-1.0
   LooseTauPt3Dhisto->ProjectionX("LooseTauPtDr3Bhisto",21,200,1,1, "e");//1.0>
   TightTauPt3Dhisto->ProjectionX("TightTauPtDr0Bhisto",1, 200,1,1, "e");
   TightTauPt3Dhisto->ProjectionX("TightTauPtDr1Bhisto",15,16, 1,1, "e");
   TightTauPt3Dhisto->ProjectionX("TightTauPtDr2Bhisto",17,20, 1,1, "e");
   TightTauPt3Dhisto->ProjectionX("TightTauPtDr3Bhisto",21,200,1,1, "e");
   //
   LooseFakeTauPt3Dhisto->ProjectionX("LooseFakeTauPtDr0Bhisto",1, 200,1,1, "e");
   LooseFakeTauPt3Dhisto->ProjectionX("LooseFakeTauPtDr1Bhisto",15,16, 1,1, "e");
   LooseFakeTauPt3Dhisto->ProjectionX("LooseFakeTauPtDr2Bhisto",17,20, 1,1, "e");
   LooseFakeTauPt3Dhisto->ProjectionX("LooseFakeTauPtDr3Bhisto",21,200,1,1, "e");
   TightFakeTauPt3Dhisto->ProjectionX("TightFakeTauPtDr0Bhisto",1, 200,1,1, "e");
   TightFakeTauPt3Dhisto->ProjectionX("TightFakeTauPtDr1Bhisto",15,16, 1,1, "e");
   TightFakeTauPt3Dhisto->ProjectionX("TightFakeTauPtDr2Bhisto",17,20, 1,1, "e");
   TightFakeTauPt3Dhisto->ProjectionX("TightFakeTauPtDr3Bhisto",21,200,1,1, "e");
   //
   LoosePromptTauPt3Dhisto->ProjectionX("LoosePromptTauPtDr0Bhisto",1, 200,1,1, "e");
   LoosePromptTauPt3Dhisto->ProjectionX("LoosePromptTauPtDr1Bhisto",15,16, 1,1, "e");
   LoosePromptTauPt3Dhisto->ProjectionX("LoosePromptTauPtDr2Bhisto",17,20, 1,1, "e");
   LoosePromptTauPt3Dhisto->ProjectionX("LoosePromptTauPtDr3Bhisto",21,200,1,1, "e");
   TightPromptTauPt3Dhisto->ProjectionX("TightPromptTauPtDr0Bhisto",1, 200,1,1, "e");
   TightPromptTauPt3Dhisto->ProjectionX("TightPromptTauPtDr1Bhisto",15,16, 1,1, "e");
   TightPromptTauPt3Dhisto->ProjectionX("TightPromptTauPtDr2Bhisto",17,20, 1,1, "e");
   TightPromptTauPt3Dhisto->ProjectionX("TightPromptTauPtDr3Bhisto",21,200,1,1, "e");
   //
   LooseTauPt3Dhisto->ProjectionX("LooseTauPtDr0Ehisto",1, 200,2,2, "e");//All
   LooseTauPt3Dhisto->ProjectionX("LooseTauPtDr1Ehisto",15,16, 2,2, "e");//0.7-0.8
   LooseTauPt3Dhisto->ProjectionX("LooseTauPtDr2Ehisto",17,20, 2,2, "e");//0.8-1.0
   LooseTauPt3Dhisto->ProjectionX("LooseTauPtDr3Ehisto",21,200,2,2, "e");//1.0>
   TightTauPt3Dhisto->ProjectionX("TightTauPtDr0Ehisto",1, 200,2,2, "e");
   TightTauPt3Dhisto->ProjectionX("TightTauPtDr1Ehisto",15,16, 2,2, "e");
   TightTauPt3Dhisto->ProjectionX("TightTauPtDr2Ehisto",17,20, 2,2, "e");
   TightTauPt3Dhisto->ProjectionX("TightTauPtDr3Ehisto",21,200,2,2, "e");
   //
   LooseFakeTauPt3Dhisto->ProjectionX("LooseFakeTauPtDr0Ehisto",1, 200,2,2, "e");
   LooseFakeTauPt3Dhisto->ProjectionX("LooseFakeTauPtDr1Ehisto",15,16, 2,2, "e");
   LooseFakeTauPt3Dhisto->ProjectionX("LooseFakeTauPtDr2Ehisto",17,20, 2,2, "e");
   LooseFakeTauPt3Dhisto->ProjectionX("LooseFakeTauPtDr3Ehisto",21,200,2,2, "e");
   TightFakeTauPt3Dhisto->ProjectionX("TightFakeTauPtDr0Ehisto",1, 200,2,2, "e");
   TightFakeTauPt3Dhisto->ProjectionX("TightFakeTauPtDr1Ehisto",15,16, 2,2, "e");
   TightFakeTauPt3Dhisto->ProjectionX("TightFakeTauPtDr2Ehisto",17,20, 2,2, "e");
   TightFakeTauPt3Dhisto->ProjectionX("TightFakeTauPtDr3Ehisto",21,200,2,2, "e");
   //
   LoosePromptTauPt3Dhisto->ProjectionX("LoosePromptTauPtDr0Ehisto",1, 200,2,2, "e");
   LoosePromptTauPt3Dhisto->ProjectionX("LoosePromptTauPtDr1Ehisto",15,16, 2,2, "e");
   LoosePromptTauPt3Dhisto->ProjectionX("LoosePromptTauPtDr2Ehisto",17,20, 2,2, "e");
   LoosePromptTauPt3Dhisto->ProjectionX("LoosePromptTauPtDr3Ehisto",21,200,2,2, "e");
   TightPromptTauPt3Dhisto->ProjectionX("TightPromptTauPtDr0Ehisto",1, 200,2,2, "e");
   TightPromptTauPt3Dhisto->ProjectionX("TightPromptTauPtDr1Ehisto",15,16, 2,2, "e");
   TightPromptTauPt3Dhisto->ProjectionX("TightPromptTauPtDr2Ehisto",17,20, 2,2, "e");
   TightPromptTauPt3Dhisto->ProjectionX("TightPromptTauPtDr3Ehisto",21,200,2,2, "e");
   //
   //
   // Change Pt to PileUp -- PT CUT >35 GeV
   LooseTauPU3Dhisto->ProjectionX("LooseTauPUDr0Bhisto",1, 200,1,1, "e");//All
   LooseTauPU3Dhisto->ProjectionX("LooseTauPUDr1Bhisto",15,16, 1,1, "e");//0.7-0.8
   LooseTauPU3Dhisto->ProjectionX("LooseTauPUDr2Bhisto",17,20, 1,1, "e");//0.8-1.0
   LooseTauPU3Dhisto->ProjectionX("LooseTauPUDr3Bhisto",21,200,1,1, "e");//1.0>
   TightTauPU3Dhisto->ProjectionX("TightTauPUDr0Bhisto",1, 200,1,1, "e");
   TightTauPU3Dhisto->ProjectionX("TightTauPUDr1Bhisto",15,16, 1,1, "e");
   TightTauPU3Dhisto->ProjectionX("TightTauPUDr2Bhisto",17,20, 1,1, "e");
   TightTauPU3Dhisto->ProjectionX("TightTauPUDr3Bhisto",21,200,1,1, "e");
   //
   LooseFakeTauPU3Dhisto->ProjectionX("LooseFakeTauPUDr0Bhisto",1, 200,1,1, "e");
   LooseFakeTauPU3Dhisto->ProjectionX("LooseFakeTauPUDr1Bhisto",15,16, 1,1, "e");
   LooseFakeTauPU3Dhisto->ProjectionX("LooseFakeTauPUDr2Bhisto",17,20, 1,1, "e");
   LooseFakeTauPU3Dhisto->ProjectionX("LooseFakeTauPUDr3Bhisto",21,200,1,1, "e");
   TightFakeTauPU3Dhisto->ProjectionX("TightFakeTauPUDr0Bhisto",1, 200,1,1, "e");
   TightFakeTauPU3Dhisto->ProjectionX("TightFakeTauPUDr1Bhisto",15,16, 1,1, "e");
   TightFakeTauPU3Dhisto->ProjectionX("TightFakeTauPUDr2Bhisto",17,20, 1,1, "e");
   TightFakeTauPU3Dhisto->ProjectionX("TightFakeTauPUDr3Bhisto",21,200,1,1, "e");
   //
   LoosePromptTauPU3Dhisto->ProjectionX("LoosePromptTauPUDr0Bhisto",1, 200,1,1, "e");
   LoosePromptTauPU3Dhisto->ProjectionX("LoosePromptTauPUDr1Bhisto",15,16, 1,1, "e");
   LoosePromptTauPU3Dhisto->ProjectionX("LoosePromptTauPUDr2Bhisto",17,20, 1,1, "e");
   LoosePromptTauPU3Dhisto->ProjectionX("LoosePromptTauPUDr3Bhisto",21,200,1,1, "e");
   TightPromptTauPU3Dhisto->ProjectionX("TightPromptTauPUDr0Bhisto",1, 200,1,1, "e");
   TightPromptTauPU3Dhisto->ProjectionX("TightPromptTauPUDr1Bhisto",15,16, 1,1, "e");
   TightPromptTauPU3Dhisto->ProjectionX("TightPromptTauPUDr2Bhisto",17,20, 1,1, "e");
   TightPromptTauPU3Dhisto->ProjectionX("TightPromptTauPUDr3Bhisto",21,200,1,1, "e");
   //
   LooseTauPU3Dhisto->ProjectionX("LooseTauPUDr0Ehisto",1, 200,2,2, "e");//All
   LooseTauPU3Dhisto->ProjectionX("LooseTauPUDr1Ehisto",15,16, 2,2, "e");//0.7-0.8
   LooseTauPU3Dhisto->ProjectionX("LooseTauPUDr2Ehisto",17,20, 2,2, "e");//0.8-1.0
   LooseTauPU3Dhisto->ProjectionX("LooseTauPUDr3Ehisto",21,200,2,2, "e");//1.0>
   TightTauPU3Dhisto->ProjectionX("TightTauPUDr0Ehisto",1, 200,2,2, "e");
   TightTauPU3Dhisto->ProjectionX("TightTauPUDr1Ehisto",15,16, 2,2, "e");
   TightTauPU3Dhisto->ProjectionX("TightTauPUDr2Ehisto",17,20, 2,2, "e");
   TightTauPU3Dhisto->ProjectionX("TightTauPUDr3Ehisto",21,200,2,2, "e");
   //
   LooseFakeTauPU3Dhisto->ProjectionX("LooseFakeTauPUDr0Ehisto",1, 200,2,2, "e");
   LooseFakeTauPU3Dhisto->ProjectionX("LooseFakeTauPUDr1Ehisto",15,16, 2,2, "e");
   LooseFakeTauPU3Dhisto->ProjectionX("LooseFakeTauPUDr2Ehisto",17,20, 2,2, "e");
   LooseFakeTauPU3Dhisto->ProjectionX("LooseFakeTauPUDr3Ehisto",21,200,2,2, "e");
   TightFakeTauPU3Dhisto->ProjectionX("TightFakeTauPUDr0Ehisto",1, 200,2,2, "e");
   TightFakeTauPU3Dhisto->ProjectionX("TightFakeTauPUDr1Ehisto",15,16, 2,2, "e");
   TightFakeTauPU3Dhisto->ProjectionX("TightFakeTauPUDr2Ehisto",17,20, 2,2, "e");
   TightFakeTauPU3Dhisto->ProjectionX("TightFakeTauPUDr3Ehisto",21,200,2,2, "e");
   //
   LoosePromptTauPU3Dhisto->ProjectionX("LoosePromptTauPUDr0Ehisto",1, 200,2,2, "e");
   LoosePromptTauPU3Dhisto->ProjectionX("LoosePromptTauPUDr1Ehisto",15,16, 2,2, "e");
   LoosePromptTauPU3Dhisto->ProjectionX("LoosePromptTauPUDr2Ehisto",17,20, 2,2, "e");
   LoosePromptTauPU3Dhisto->ProjectionX("LoosePromptTauPUDr3Ehisto",21,200,2,2, "e");
   TightPromptTauPU3Dhisto->ProjectionX("TightPromptTauPUDr0Ehisto",1, 200,2,2, "e");
   TightPromptTauPU3Dhisto->ProjectionX("TightPromptTauPUDr1Ehisto",15,16, 2,2, "e");
   TightPromptTauPU3Dhisto->ProjectionX("TightPromptTauPUDr2Ehisto",17,20, 2,2, "e");
   TightPromptTauPU3Dhisto->ProjectionX("TightPromptTauPUDr3Ehisto",21,200,2,2, "e");
   //
   //

   // ------------------------------------------
   cout<<"LooseTauPtDr0123Bhisto PRINT"<<endl;
   LooseTauPtDr0Bhisto->Print();
   LooseTauPtDr1Bhisto->Print();
   LooseTauPtDr2Bhisto->Print();
   LooseTauPtDr3Bhisto->Print();
   cout<<"LooseTauPtDr0123Bhisto PRINT END"<<endl;
   cout<<"LooseTauPtDr0123Ehisto PRINT"<<endl;
   LooseTauPtDr0Ehisto->Print();
   LooseTauPtDr1Ehisto->Print();
   LooseTauPtDr2Ehisto->Print();
   LooseTauPtDr3Ehisto->Print();
   cout<<"LooseTauPtDr0123Ehisto PRINT END"<<endl;
   //
   cout<<"LooseTauPUDr0123Bhisto PRINT"<<endl;
   LooseTauPUDr0Bhisto->Print();
   LooseTauPUDr1Bhisto->Print();
   LooseTauPUDr2Bhisto->Print();
   LooseTauPUDr3Bhisto->Print();
   cout<<"LooseTauPUDr0123Bhisto PRINT END"<<endl;
   cout<<"LooseTauPUDr0123Ehisto PRINT"<<endl;
   LooseTauPUDr0Ehisto->Print();
   LooseTauPUDr1Ehisto->Print();
   LooseTauPUDr2Ehisto->Print();
   LooseTauPUDr3Ehisto->Print();
   cout<<"LooseTauPUDr0123Ehisto PRINT END"<<endl;
   // ------------------------------------------

   LooseTauPtDr0Bhisto->Write();        LooseTauPtDr1Bhisto->Write();        LooseTauPtDr2Bhisto->Write();        LooseTauPtDr3Bhisto->Write();
   TightTauPtDr0Bhisto->Write();        TightTauPtDr1Bhisto->Write();        TightTauPtDr2Bhisto->Write();        TightTauPtDr3Bhisto->Write();
   LooseFakeTauPtDr0Bhisto->Write();    LooseFakeTauPtDr1Bhisto->Write();    LooseFakeTauPtDr2Bhisto->Write();    LooseFakeTauPtDr3Bhisto->Write();
   TightFakeTauPtDr0Bhisto->Write();    TightFakeTauPtDr1Bhisto->Write();    TightFakeTauPtDr2Bhisto->Write();    TightFakeTauPtDr3Bhisto->Write();
   LoosePromptTauPtDr0Bhisto->Write();  LoosePromptTauPtDr1Bhisto->Write();  LoosePromptTauPtDr2Bhisto->Write();  LoosePromptTauPtDr3Bhisto->Write();
   TightPromptTauPtDr0Bhisto->Write();  TightPromptTauPtDr1Bhisto->Write();  TightPromptTauPtDr2Bhisto->Write();  TightPromptTauPtDr3Bhisto->Write();
   //
   LooseTauPtDr0Ehisto->Write();        LooseTauPtDr1Ehisto->Write();        LooseTauPtDr2Ehisto->Write();        LooseTauPtDr3Ehisto->Write();
   TightTauPtDr0Ehisto->Write();        TightTauPtDr1Ehisto->Write();        TightTauPtDr2Ehisto->Write();        TightTauPtDr3Ehisto->Write();
   LooseFakeTauPtDr0Ehisto->Write();    LooseFakeTauPtDr1Ehisto->Write();    LooseFakeTauPtDr2Ehisto->Write();    LooseFakeTauPtDr3Ehisto->Write();
   TightFakeTauPtDr0Ehisto->Write();    TightFakeTauPtDr1Ehisto->Write();    TightFakeTauPtDr2Ehisto->Write();    TightFakeTauPtDr3Ehisto->Write();
   LoosePromptTauPtDr0Ehisto->Write();  LoosePromptTauPtDr1Ehisto->Write();  LoosePromptTauPtDr2Ehisto->Write();  LoosePromptTauPtDr3Ehisto->Write();
   TightPromptTauPtDr0Ehisto->Write();  TightPromptTauPtDr1Ehisto->Write();  TightPromptTauPtDr2Ehisto->Write();  TightPromptTauPtDr3Ehisto->Write();


   LooseTauPUDr0Bhisto->Write();        LooseTauPUDr1Bhisto->Write();        LooseTauPUDr2Bhisto->Write();        LooseTauPUDr3Bhisto->Write();
   TightTauPUDr0Bhisto->Write();        TightTauPUDr1Bhisto->Write();        TightTauPUDr2Bhisto->Write();        TightTauPUDr3Bhisto->Write();
   LooseFakeTauPUDr0Bhisto->Write();    LooseFakeTauPUDr1Bhisto->Write();    LooseFakeTauPUDr2Bhisto->Write();    LooseFakeTauPUDr3Bhisto->Write();
   TightFakeTauPUDr0Bhisto->Write();    TightFakeTauPUDr1Bhisto->Write();    TightFakeTauPUDr2Bhisto->Write();    TightFakeTauPUDr3Bhisto->Write();
   LoosePromptTauPUDr0Bhisto->Write();  LoosePromptTauPUDr1Bhisto->Write();  LoosePromptTauPUDr2Bhisto->Write();  LoosePromptTauPUDr3Bhisto->Write();
   TightPromptTauPUDr0Bhisto->Write();  TightPromptTauPUDr1Bhisto->Write();  TightPromptTauPUDr2Bhisto->Write();  TightPromptTauPUDr3Bhisto->Write();
   //
   LooseTauPUDr0Ehisto->Write();        LooseTauPUDr1Ehisto->Write();        LooseTauPUDr2Ehisto->Write();        LooseTauPUDr3Ehisto->Write();
   TightTauPUDr0Ehisto->Write();        TightTauPUDr1Ehisto->Write();        TightTauPUDr2Ehisto->Write();        TightTauPUDr3Ehisto->Write();
   LooseFakeTauPUDr0Ehisto->Write();    LooseFakeTauPUDr1Ehisto->Write();    LooseFakeTauPUDr2Ehisto->Write();    LooseFakeTauPUDr3Ehisto->Write();
   TightFakeTauPUDr0Ehisto->Write();    TightFakeTauPUDr1Ehisto->Write();    TightFakeTauPUDr2Ehisto->Write();    TightFakeTauPUDr3Ehisto->Write();
   LoosePromptTauPUDr0Ehisto->Write();  LoosePromptTauPUDr1Ehisto->Write();  LoosePromptTauPUDr2Ehisto->Write();  LoosePromptTauPUDr3Ehisto->Write();
   TightPromptTauPUDr0Ehisto->Write();  TightPromptTauPUDr1Ehisto->Write();  TightPromptTauPUDr2Ehisto->Write();  TightPromptTauPUDr3Ehisto->Write();


   //////////
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
// // //
