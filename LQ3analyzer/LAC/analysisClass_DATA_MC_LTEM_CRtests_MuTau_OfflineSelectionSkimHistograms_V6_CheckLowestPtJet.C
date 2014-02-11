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
#include "LAC/ADDON5_LAC.C" //gen-reco matching 
//#include "LAC/ADDON5_LQ_LAC.C" //gen-reco matching 
//#include "LAC/ADDON5_H_LAC.C" //gen-reco matching 
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
   InitMuonFunctionParameters(); //initialize muon prompt rate function parameters!
   InitTauFakeRates(); //initialize tau prompt and fake rates!
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
   TH1D* muPRhisto   = new TH1D("muPRhisto",  "muPRhisto",    10000,0,1);
   TH1D* muFRhisto   = new TH1D("muFRhisto",  "muFRhisto",    10000,0,1);
   TH1D* tauPRhisto  = new TH1D("tauPRhisto", "tauPRhisto",   10000,0,1);
   TH1D* tauFRhisto  = new TH1D("tauFRhisto", "tauFRhisto",   10000,0,1);
   TH1D* muPREhisto  = new TH1D("muPREhisto", "muPREhisto",   10000,0,1);
   TH1D* muFREhisto  = new TH1D("muFREhisto", "muFREhisto",   10000,0,1);
   TH1D* tauPREhisto = new TH1D("tauPREhisto","tauPREhisto",  10000,0,1);
   TH1D* tauFREhisto = new TH1D("tauFREhisto", "tauFREhisto", 10000,0,1);
   //
   // Barrel - Endcap Histos separate
   //
   // 1    2    3      4          5          6          7          8         9         10        11
   // Obs, Exp, ExpPP, ExpTauFRU, ExpTauFRD, ExpTauPRU, ExpTauPRD, ExpMuFRU, ExpMuFRD, ExpMuPRU, ExpMuPRD
   TH2D* BarrelTTBckgSyshisto     = new TH2D("BarrelTTBckgSyshisto",      "BarrelTTBckgSyshisto",      10,0.5,10.5, 11,0.5,11.5);
   TH2D* EndcapTTBckgSyshisto     = new TH2D("EndcapTTBckgSyshisto",      "EndcapTTBckgSyshisto",      10,0.5,10.5, 11,0.5,11.5);
   TH2D* BarrelTTBckgSysW1histo     = new TH2D("BarrelTTBckgSysW1histo",      "BarrelTTBckgSysW1histo",      10,0.5,10.5, 11,0.5,11.5);
   TH2D* EndcapTTBckgSysW1histo     = new TH2D("EndcapTTBckgSysW1histo",      "EndcapTTBckgSysW1histo",      10,0.5,10.5, 11,0.5,11.5);
   //
   // Exp: FF, FP, PF 
   TH2D* BarrelExpFakeBreakdownhisto = new TH2D("BarrelExpFakeBreakdownhisto", "BarrelExpFakeBreakdownhisto", 10,0.5,10.5, 3,0.5,3.5);
   TH2D* EndcapExpFakeBreakdownhisto = new TH2D("EndcapExpFakeBreakdownhisto", "EndcapExpFakeBreakdownhisto", 10,0.5,10.5, 3,0.5,3.5);
   TH2D* BarrelExpFakeBreakdownW1histo = new TH2D("BarrelExpFakeBreakdownW1histo", "BarrelExpFakeBreakdownW1histo", 10,0.5,10.5, 3,0.5,3.5);
   TH2D* EndcapExpFakeBreakdownW1histo = new TH2D("EndcapExpFakeBreakdownW1histo", "EndcapExpFakeBreakdownW1histo", 10,0.5,10.5, 3,0.5,3.5);
   //
   // Obs, Exp, ExpPP, ExpTauFRU&ExpTauPRU, ExpTauFRD&ExpTauPRD, ExpMuFRU&ExpMuPRU, ExpMuFRD&ExpMuPRD
   TH2D* BarrelTTBckgSysV2histo   = new TH2D("BarrelTTBckgSysV2histo",    "BarrelTTBckgSysV2histo",    10,0.5,10.5, 7,0.5,7.5);
   TH2D* EndcapTTBckgSysV2histo   = new TH2D("EndcapTTBckgSysV2histo",    "EndcapTTBckgSysV2histo",    10,0.5,10.5, 7,0.5,7.5);
   //
   //
   TH2D* BarrelLTmatrixhisto      = new TH2D("BarrelLTmatrixhisto",       "BarrelLTmatrixhisto",       10,0.5,10.5, 4,0.5,4.5);
   TH2D* EndcapLTmatrixhisto      = new TH2D("EndcapLTmatrixhisto",       "EndcapLTmatrixhisto",       10,0.5,10.5, 4,0.5,4.5);
   //
   // in TT box..
   TH2D* BarrelFPmatrixhisto      = new TH2D("BarrelFPmatrixhisto",       "BarrelFPmatrixhisto",       10,0.5,10.5, 6,0.5,6.5);
   TH2D* EndcapFPmatrixhisto      = new TH2D("EndcapFPmatrixhisto",       "EndcapFPmatrixhisto",       10,0.5,10.5, 6,0.5,6.5);
   TH2D* BarrelFPmatrixW1histo      = new TH2D("BarrelFPmatrixW1histo",       "BarrelFPmatrixW1histo",       10,0.5,10.5, 6,0.5,6.5);
   TH2D* EndcapFPmatrixW1histo      = new TH2D("EndcapFPmatrixW1histo",       "EndcapFPmatrixW1histo",       10,0.5,10.5, 6,0.5,6.5);
   //
   //  M200 |M400 |M600
   //   B E | B E | B E
   // 0 1-2 3 4-5 6 7-8 9
   TH1D* BackgroundDecompositionhisto = new TH1D("BackgroundDecompositionhisto", "BackgroundDecompositionhisto", 10,-0.5,9.5 );
   //
   TH2D* BarrelLL_SThisto      = new TH2D("BarrelLL_SThisto",       "BarrelLL_SThisto",       10,0.5,10.5, 3000,0,3000);
   TH2D* BarrelLT_SThisto      = new TH2D("BarrelLT_SThisto",       "BarrelLT_SThisto",       10,0.5,10.5, 3000,0,3000);
   TH2D* BarrelTL_SThisto      = new TH2D("BarrelTL_SThisto",       "BarrelTL_SThisto",       10,0.5,10.5, 3000,0,3000);
   TH2D* BarrelTT_SThisto      = new TH2D("BarrelTT_SThisto",       "BarrelTT_SThisto",       10,0.5,10.5, 3000,0,3000);
   TH2D* BarrelLL_TAUETAhisto  = new TH2D("BarrelLL_TAUETAhisto",   "BarrelLL_TAUETAhisto",   10,0.5,10.5, 16,-2.4,2.4);
   TH2D* BarrelLT_TAUETAhisto  = new TH2D("BarrelLT_TAUETAhisto",   "BarrelLT_TAUETAhisto",   10,0.5,10.5, 16,-2.4,2.4);
   TH2D* BarrelTL_TAUETAhisto  = new TH2D("BarrelTL_TAUETAhisto",   "BarrelTL_TAUETAhisto",   10,0.5,10.5, 16,-2.4,2.4);
   TH2D* BarrelTT_TAUETAhisto  = new TH2D("BarrelTT_TAUETAhisto",   "BarrelTT_TAUETAhisto",   10,0.5,10.5, 16,-2.4,2.4);
   TH2D* BarrelLL_MUETAhisto   = new TH2D("BarrelLL_MUETAhisto",    "BarrelLL_MUETAhisto",    10,0.5,10.5, 16,-2.4,2.4);
   TH2D* BarrelLT_MUETAhisto   = new TH2D("BarrelLT_MUETAhisto",    "BarrelLT_MUETAhisto",    10,0.5,10.5, 16,-2.4,2.4);
   TH2D* BarrelTL_MUETAhisto   = new TH2D("BarrelTL_MUETAhisto",    "BarrelTL_MUETAhisto",    10,0.5,10.5, 16,-2.4,2.4);
   TH2D* BarrelTT_MUETAhisto   = new TH2D("BarrelTT_MUETAhisto",    "BarrelTT_MUETAhisto",    10,0.5,10.5, 16,-2.4,2.4);
   TH2D* BarrelLL_TAUPThisto   = new TH2D("BarrelLL_TAUPThisto",    "BarrelLL_TAUPThisto",    10,0.5,10.5, 3000,0,3000);
   TH2D* BarrelLT_TAUPThisto   = new TH2D("BarrelLT_TAUPThisto",    "BarrelLT_TAUPThisto",    10,0.5,10.5, 3000,0,3000);
   TH2D* BarrelTL_TAUPThisto   = new TH2D("BarrelTL_TAUPThisto",    "BarrelTL_TAUPThisto",    10,0.5,10.5, 3000,0,3000);
   TH2D* BarrelTT_TAUPThisto   = new TH2D("BarrelTT_TAUPThisto",    "BarrelTT_TAUPThisto",    10,0.5,10.5, 3000,0,3000);
   TH2D* BarrelLL_MUPThisto    = new TH2D("BarrelLL_MUPThisto",     "BarrelLL_MUPThisto",     10,0.5,10.5, 3000,0,3000);
   TH2D* BarrelLT_MUPThisto    = new TH2D("BarrelLT_MUPThisto",     "BarrelLT_MUPThisto",     10,0.5,10.5, 3000,0,3000);
   TH2D* BarrelTL_MUPThisto    = new TH2D("BarrelTL_MUPThisto",     "BarrelTL_MUPThisto",     10,0.5,10.5, 3000,0,3000);
   TH2D* BarrelTT_MUPThisto    = new TH2D("BarrelTT_MUPThisto",     "BarrelTT_MUPThisto",     10,0.5,10.5, 3000,0,3000);
   TH2D* BarrelLL_MUJETDRhisto = new TH2D("BarrelLL_MUJETDRhisto",     "BarrelLL_MUJETDRhisto",     10,0.5,10.5, 100,0,10);
   TH2D* BarrelLT_MUJETDRhisto = new TH2D("BarrelLT_MUJETDRhisto",     "BarrelLT_MUJETDRhisto",     10,0.5,10.5, 100,0,10);
   TH2D* BarrelTL_MUJETDRhisto = new TH2D("BarrelTL_MUJETDRhisto",     "BarrelTL_MUJETDRhisto",     10,0.5,10.5, 100,0,10);
   TH2D* BarrelTT_MUJETDRhisto = new TH2D("BarrelTT_MUJETDRhisto",     "BarrelTT_MUJETDRhisto",     10,0.5,10.5, 100,0,10);
   TH2D* BarrelLL_TAUJETDRhisto  = new TH2D("BarrelLL_TAUJETDRhisto",    "BarrelLL_TAUJETDRhisto",    10,0.5,10.5, 100,0,10);
   TH2D* BarrelLT_TAUJETDRhisto  = new TH2D("BarrelLT_TAUJETDRhisto",    "BarrelLT_TAUJETDRhisto",    10,0.5,10.5, 100,0,10);
   TH2D* BarrelTL_TAUJETDRhisto  = new TH2D("BarrelTL_TAUJETDRhisto",    "BarrelTL_TAUJETDRhisto",    10,0.5,10.5, 100,0,10);
   TH2D* BarrelTT_TAUJETDRhisto  = new TH2D("BarrelTT_TAUJETDRhisto",    "BarrelTT_TAUJETDRhisto",    10,0.5,10.5, 100,0,10);
   TH2D* BarrelLL_BJETNhisto    = new TH2D("BarrelLL_BJETNhisto",        "BarrelLL_BJETNhisto",        10,0.5,10.5, 21,-0.5,20.5);
   TH2D* BarrelLT_BJETNhisto    = new TH2D("BarrelLT_BJETNhisto",        "BarrelLT_BJETNhisto",        10,0.5,10.5, 21,-0.5,20.5);
   TH2D* BarrelTL_BJETNhisto    = new TH2D("BarrelTL_BJETNhisto",        "BarrelTL_BJETNhisto",        10,0.5,10.5, 21,-0.5,20.5);
   TH2D* BarrelTT_BJETNhisto    = new TH2D("BarrelTT_BJETNhisto",        "BarrelTT_BJETNhisto",        10,0.5,10.5, 21,-0.5,20.5);
   TH2D* BarrelLL_JETNhisto    = new TH2D("BarrelLL_JETNhisto",        "BarrelLL_JETNhisto",        10,0.5,10.5, 21,-0.5,20.5);
   TH2D* BarrelLT_JETNhisto    = new TH2D("BarrelLT_JETNhisto",        "BarrelLT_JETNhisto",        10,0.5,10.5, 21,-0.5,20.5);
   TH2D* BarrelTL_JETNhisto    = new TH2D("BarrelTL_JETNhisto",        "BarrelTL_JETNhisto",        10,0.5,10.5, 21,-0.5,20.5);
   TH2D* BarrelTT_JETNhisto    = new TH2D("BarrelTT_JETNhisto",        "BarrelTT_JETNhisto",        10,0.5,10.5, 21,-0.5,20.5);
   TH2D* BarrelLL_PUhisto      = new TH2D("BarrelLL_PUhisto",        "BarrelLL_PUhisto",        10,0.5,10.5, 61,-0.5,60.5);
   TH2D* BarrelLT_PUhisto      = new TH2D("BarrelLT_PUhisto",        "BarrelLT_PUhisto",        10,0.5,10.5, 61,-0.5,60.5);
   TH2D* BarrelTL_PUhisto      = new TH2D("BarrelTL_PUhisto",        "BarrelTL_PUhisto",        10,0.5,10.5, 61,-0.5,60.5);
   TH2D* BarrelTT_PUhisto      = new TH2D("BarrelTT_PUhisto",        "BarrelTT_PUhisto",        10,0.5,10.5, 61,-0.5,60.5);
   TH2D* BarrelLL_METhisto      = new TH2D("BarrelLL_METhisto",        "BarrelLL_METhisto",        10,0.5,10.5, 3000,0,3000);
   TH2D* BarrelLT_METhisto      = new TH2D("BarrelLT_METhisto",        "BarrelLT_METhisto",        10,0.5,10.5, 3000,0,3000);
   TH2D* BarrelTL_METhisto      = new TH2D("BarrelTL_METhisto",        "BarrelTL_METhisto",        10,0.5,10.5, 3000,0,3000);
   TH2D* BarrelTT_METhisto      = new TH2D("BarrelTT_METhisto",        "BarrelTT_METhisto",        10,0.5,10.5, 3000,0,3000);
   //
   TH2D* EndcapLL_SThisto      = new TH2D("EndcapLL_SThisto",        "EndcapLL_SThisto",        10,0.5,10.5, 3000,0,3000);
   TH2D* EndcapLT_SThisto      = new TH2D("EndcapLT_SThisto",        "EndcapLT_SThisto",        10,0.5,10.5, 3000,0,3000);
   TH2D* EndcapTL_SThisto      = new TH2D("EndcapTL_SThisto",        "EndcapTL_SThisto",        10,0.5,10.5, 3000,0,3000);
   TH2D* EndcapTT_SThisto      = new TH2D("EndcapTT_SThisto",        "EndcapTT_SThisto",        10,0.5,10.5, 3000,0,3000);
   TH2D* EndcapLL_TAUETAhisto  = new TH2D("EndcapLL_TAUETAhisto",    "EndcapLL_TAUETAhisto",    10,0.5,10.5, 16,-2.4,2.4);
   TH2D* EndcapLT_TAUETAhisto  = new TH2D("EndcapLT_TAUETAhisto",    "EndcapLT_TAUETAhisto",    10,0.5,10.5, 16,-2.4,2.4);
   TH2D* EndcapTL_TAUETAhisto  = new TH2D("EndcapTL_TAUETAhisto",    "EndcapTL_TAUETAhisto",    10,0.5,10.5, 16,-2.4,2.4);
   TH2D* EndcapTT_TAUETAhisto  = new TH2D("EndcapTT_TAUETAhisto",    "EndcapTT_TAUETAhisto",    10,0.5,10.5, 16,-2.4,2.4);
   TH2D* EndcapLL_MUETAhisto   = new TH2D("EndcapLL_MUETAhisto",     "EndcapLL_MUETAhisto",     10,0.5,10.5, 16,-2.4,2.4);
   TH2D* EndcapLT_MUETAhisto   = new TH2D("EndcapLT_MUETAhisto",     "EndcapLT_MUETAhisto",     10,0.5,10.5, 16,-2.4,2.4);
   TH2D* EndcapTL_MUETAhisto   = new TH2D("EndcapTL_MUETAhisto",     "EndcapTL_MUETAhisto",     10,0.5,10.5, 16,-2.4,2.4);
   TH2D* EndcapTT_MUETAhisto   = new TH2D("EndcapTT_MUETAhisto",     "EndcapTT_MUETAhisto",     10,0.5,10.5, 16,-2.4,2.4);
   TH2D* EndcapLL_TAUPThisto   = new TH2D("EndcapLL_TAUPThisto",     "EndcapLL_TAUPThisto",     10,0.5,10.5, 3000,0,3000);
   TH2D* EndcapLT_TAUPThisto   = new TH2D("EndcapLT_TAUPThisto",     "EndcapLT_TAUPThisto",     10,0.5,10.5, 3000,0,3000);
   TH2D* EndcapTL_TAUPThisto   = new TH2D("EndcapTL_TAUPThisto",     "EndcapTL_TAUPThisto",     10,0.5,10.5, 3000,0,3000);
   TH2D* EndcapTT_TAUPThisto   = new TH2D("EndcapTT_TAUPThisto",     "EndcapTT_TAUPThisto",     10,0.5,10.5, 3000,0,3000);
   TH2D* EndcapLL_MUPThisto    = new TH2D("EndcapLL_MUPThisto",      "EndcapLL_MUPThisto",      10,0.5,10.5, 3000,0,3000);
   TH2D* EndcapLT_MUPThisto    = new TH2D("EndcapLT_MUPThisto",      "EndcapLT_MUPThisto",      10,0.5,10.5, 3000,0,3000);
   TH2D* EndcapTL_MUPThisto    = new TH2D("EndcapTL_MUPThisto",      "EndcapTL_MUPThisto",      10,0.5,10.5, 3000,0,3000);
   TH2D* EndcapTT_MUPThisto    = new TH2D("EndcapTT_MUPThisto",      "EndcapTT_MUPThisto",      10,0.5,10.5, 3000,0,3000);
   TH2D* EndcapLL_MUJETDRhisto    = new TH2D("EndcapLL_MUJETDRhisto",     "EndcapLL_MUJETDRhisto",     10,0.5,10.5, 100,0,10);
   TH2D* EndcapLT_MUJETDRhisto    = new TH2D("EndcapLT_MUJETDRhisto",     "EndcapLT_MUJETDRhisto",     10,0.5,10.5, 100,0,10);
   TH2D* EndcapTL_MUJETDRhisto    = new TH2D("EndcapTL_MUJETDRhisto",     "EndcapTL_MUJETDRhisto",     10,0.5,10.5, 100,0,10);
   TH2D* EndcapTT_MUJETDRhisto    = new TH2D("EndcapTT_MUJETDRhisto",     "EndcapTT_MUJETDRhisto",     10,0.5,10.5, 100,0,10);
   TH2D* EndcapLL_TAUJETDRhisto   = new TH2D("EndcapLL_TAUJETDRhisto",    "EndcapLL_TAUJETDRhisto",    10,0.5,10.5, 100,0,10);
   TH2D* EndcapLT_TAUJETDRhisto   = new TH2D("EndcapLT_TAUJETDRhisto",    "EndcapLT_TAUJETDRhisto",    10,0.5,10.5, 100,0,10);
   TH2D* EndcapTL_TAUJETDRhisto   = new TH2D("EndcapTL_TAUJETDRhisto",    "EndcapTL_TAUJETDRhisto",    10,0.5,10.5, 100,0,10);
   TH2D* EndcapTT_TAUJETDRhisto   = new TH2D("EndcapTT_TAUJETDRhisto",    "EndcapTT_TAUJETDRhisto",    10,0.5,10.5, 100,0,10);
   TH2D* EndcapLL_BJETNhisto    = new TH2D("EndcapLL_BJETNhisto",      "EndcapLL_BJETNhisto",       10,0.5,10.5, 21,-0.5,20.5);
   TH2D* EndcapLT_BJETNhisto    = new TH2D("EndcapLT_BJETNhisto",      "EndcapLT_BJETNhisto",       10,0.5,10.5, 21,-0.5,20.5);
   TH2D* EndcapTL_BJETNhisto    = new TH2D("EndcapTL_BJETNhisto",      "EndcapTL_BJETNhisto",       10,0.5,10.5, 21,-0.5,20.5);
   TH2D* EndcapTT_BJETNhisto    = new TH2D("EndcapTT_BJETNhisto",      "EndcapTT_BJETNhisto",       10,0.5,10.5, 21,-0.5,20.5);
   TH2D* EndcapLL_JETNhisto    = new TH2D("EndcapLL_JETNhisto",      "EndcapLL_JETNhisto",       10,0.5,10.5, 21,-0.5,20.5);
   TH2D* EndcapLT_JETNhisto    = new TH2D("EndcapLT_JETNhisto",      "EndcapLT_JETNhisto",       10,0.5,10.5, 21,-0.5,20.5);
   TH2D* EndcapTL_JETNhisto    = new TH2D("EndcapTL_JETNhisto",      "EndcapTL_JETNhisto",       10,0.5,10.5, 21,-0.5,20.5);
   TH2D* EndcapTT_JETNhisto    = new TH2D("EndcapTT_JETNhisto",      "EndcapTT_JETNhisto",       10,0.5,10.5, 21,-0.5,20.5);
   TH2D* EndcapLL_PUhisto      = new TH2D("EndcapLL_PUhisto",        "EndcapLL_PUhisto",         10,0.5,10.5, 61,-0.5,60.5);
   TH2D* EndcapLT_PUhisto      = new TH2D("EndcapLT_PUhisto",        "EndcapLT_PUhisto",         10,0.5,10.5, 61,-0.5,60.5);
   TH2D* EndcapTL_PUhisto      = new TH2D("EndcapTL_PUhisto",        "EndcapTL_PUhisto",         10,0.5,10.5, 61,-0.5,60.5);
   TH2D* EndcapTT_PUhisto      = new TH2D("EndcapTT_PUhisto",        "EndcapTT_PUhisto",         10,0.5,10.5, 61,-0.5,60.5);
   TH2D* EndcapLL_METhisto      = new TH2D("EndcapLL_METhisto",        "EndcapLL_METhisto",         10,0.5,10.5, 3000,0,3000);
   TH2D* EndcapLT_METhisto      = new TH2D("EndcapLT_METhisto",        "EndcapLT_METhisto",         10,0.5,10.5, 3000,0,3000);
   TH2D* EndcapTL_METhisto      = new TH2D("EndcapTL_METhisto",        "EndcapTL_METhisto",         10,0.5,10.5, 3000,0,3000);
   TH2D* EndcapTT_METhisto      = new TH2D("EndcapTT_METhisto",        "EndcapTT_METhisto",         10,0.5,10.5, 3000,0,3000);
   //
   // 1    2          3       4       5       6       7       8      9      10     11 
   // OBS, FakesMEAN, EXP PP, TauFRU, TauFRD, TauPRU, TauPRD, MuFRU, MuFRD, MuPRU, MuPRD
   TH2D* BarrelEXP_SThisto     = new TH2D("BarrelEXP_SThisto",    "BarrelEXP_SThisto",    11,0.5,11.5, 3000,0,3000);
   TH2D* EndcapEXP_SThisto     = new TH2D("EndcapEXP_SThisto",    "EndcapEXP_SThisto",    11,0.5,11.5, 3000,0,3000);
   TH2D* BarrelEXP_TAUETAhisto = new TH2D("BarrelEXP_TAUETAhisto", "BarrelEXP_TAUETAhisto", 11,0.5,11.5, 16,-2.4,2.4);
   TH2D* EndcapEXP_TAUETAhisto = new TH2D("EndcapEXP_TAUETAhisto", "EndcapEXP_TAUETAhisto", 11,0.5,11.5, 16,-2.4,2.4);
   TH2D* BarrelEXP_MUETAhisto  = new TH2D("BarrelEXP_MUETAhisto",  "BarrelEXP_MUETAhisto",  11,0.5,11.5, 16,-2.4,2.4);
   TH2D* EndcapEXP_MUETAhisto  = new TH2D("EndcapEXP_MUETAhisto",  "EndcapEXP_MUETAhisto",  11,0.5,11.5, 16,-2.4,2.4);
   TH2D* BarrelEXP_TAUPThisto  = new TH2D("BarrelEXP_TAUPThisto", "BarrelEXP_TAUPThisto", 11,0.5,11.5, 3000,0,3000);
   TH2D* EndcapEXP_TAUPThisto  = new TH2D("EndcapEXP_TAUPThisto", "EndcapEXP_TAUPThisto", 11,0.5,11.5, 3000,0,3000);
   TH2D* BarrelEXP_MUPThisto   = new TH2D("BarrelEXP_MUPThisto",  "BarrelEXP_MUPThisto",  11,0.5,11.5, 3000,0,3000);
   TH2D* EndcapEXP_MUPThisto   = new TH2D("EndcapEXP_MUPThisto",  "EndcapEXP_MUPThisto",  11,0.5,11.5, 3000,0,3000);
   TH2D* BarrelEXP_TAUJETDRhisto  = new TH2D("BarrelEXP_TAUJETDRhisto",  "BarrelEXP_TAUJETDRhisto",  11,0.5,11.5, 100,0,10);//0.1 bins
   TH2D* EndcapEXP_TAUJETDRhisto  = new TH2D("EndcapEXP_TAUJETDRhisto",  "EndcapEXP_TAUJETDRhisto",  11,0.5,11.5, 100,0,10);//0.1 bins
   TH2D* BarrelEXP_MUJETDRhisto   = new TH2D("BarrelEXP_MUJETDRhisto",   "BarrelEXP_MUJETDRhisto",   11,0.5,11.5, 100,0,10);//0.1 bins
   TH2D* EndcapEXP_MUJETDRhisto   = new TH2D("EndcapEXP_MUJETDRhisto",   "EndcapEXP_MUJETDRhisto",   11,0.5,11.5, 100,0,10);//0.1 bins
   TH2D* BarrelEXP_JETNhisto  = new TH2D("BarrelEXP_JETNhisto",  "BarrelEXP_JETNhisto",  11,0.5,11.5, 21,-0.5,20.5);
   TH2D* EndcapEXP_JETNhisto  = new TH2D("EndcapEXP_JETNhisto",  "EndcapEXP_JETNhisto",  11,0.5,11.5, 21,-0.5,20.5);
   TH2D* BarrelEXP_BJETNhisto  = new TH2D("BarrelEXP_BJETNhisto",  "BarrelEXP_BJETNhisto",  11,0.5,11.5, 21,-0.5,20.5);
   TH2D* EndcapEXP_BJETNhisto  = new TH2D("EndcapEXP_BJETNhisto",  "EndcapEXP_BJETNhisto",  11,0.5,11.5, 21,-0.5,20.5);
   TH2D* BarrelEXP_PUhisto    = new TH2D("BarrelEXP_PUhisto",  "BarrelEXP_PUhisto",  11,0.5,11.5, 61,-0.5,60.5);
   TH2D* EndcapEXP_PUhisto    = new TH2D("EndcapEXP_PUhisto",  "EndcapEXP_PUhisto",  11,0.5,11.5, 61,-0.5,60.5);
   TH2D* BarrelEXP_METhisto    = new TH2D("BarrelEXP_METhisto",  "BarrelEXP_METhisto",  11,0.5,11.5, 3000,0,3000);
   TH2D* EndcapEXP_METhisto    = new TH2D("EndcapEXP_METhisto",  "EndcapEXP_METhisto",  11,0.5,11.5, 3000,0,3000);
   //
   TH1D* LowestJetPtLLhisto =  new TH1D("LowestJetPtLLhisto","LowestJetPtLLhisto",600,0,3000);
   TH1D* LowestJetPtLThisto =  new TH1D("LowestJetPtLThisto","LowestJetPtLThisto",600,0,3000);
   TH1D* LowestJetPtTLhisto =  new TH1D("LowestJetPtTLhisto","LowestJetPtTLhisto",600,0,3000);
   TH1D* LowestJetPtTThisto =  new TH1D("LowestJetPtTThisto","LowestJetPtTThisto",600,0,3000);
   TH1D* LowestJetPtTTbarrelhisto =  new TH1D("LowestJetPtTTbarrelhisto","LowestJetPtTTbarrelhisto",600,0,3000);
   TH1D* LowestJetPtTTendcaphisto =  new TH1D("LowestJetPtTTendcaphisto","LowestJetPtTTendcaphisto",600,0,3000);


   //
   /////////initialize variables

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

     ApplyLepJetMetSystematics(0);//no systematics applied

     // -- RUN LTEM 
     which_MuTau(ltemMuTau);

     // WEIGHTS
     // ---- pileup
     double safePileupWeights_=0;
     safePileupWeights_=safePileupWeights();
     // ---- trigger 
     unsigned int triggerMuon=99;
     if( ltemMuTau.size()==2 ) triggerMuon=ltemMuTau[0];
     //
     if( ltemMuTau.size()!=2 ) continue;
     //
     int usedTrigger_=-5;
     if( muPtcorr(triggerMuon)> 45                                ) usedTrigger_ = SingleMu40_passTrigger();
     if( muPtcorr(triggerMuon)<=45 &&  muRTightCheck(triggerMuon) ) usedTrigger_ = SingleMu_passTrigger();
     if( muPtcorr(triggerMuon)<=45 && !muRTightCheck(triggerMuon) ) usedTrigger_ = HLT_MuPT_eta2p1_passTrigger();// prescale 167.2
     //
     double TriggerEfficiencyWeights_;
     if( muPtcorr(triggerMuon)> 45                                ) TriggerEfficiencyWeights_ = Mu40e2p1_ScaleFactor( muPtcorr(triggerMuon), MuonEta->at(triggerMuon) );
     if( muPtcorr(triggerMuon)<=45 &&  muRTightCheck(triggerMuon) ) TriggerEfficiencyWeights_ = IsoMu24e2p1_Eff(      muPtcorr(triggerMuon), MuonEta->at(triggerMuon) );     
     if( muPtcorr(triggerMuon)<=45 && !muRTightCheck(triggerMuon) ) TriggerEfficiencyWeights_ = Mu24e2p1_ScaleFactor( muPtcorr(triggerMuon), MuonEta->at(triggerMuon) );
     //
     AppliedTrigEffWeightshisto->Fill( TriggerEfficiencyWeights_ );
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
     //w=1;//set all weights to one!!
     //w=safePileupWeights_; // PU weights only

     //  -- JSON SKIM
     int passJSON_=0;
     if( isData  ) passJSON_ = passJSON(run,ls,isData);
     if( !isData ) passJSON_ = 1;
     fillVariableWithValue("PassJSON", passJSON_ );//..............// returns 0, 1                                                      

     //  -- TRIGGER SKIM
     int passTrigger_=0;
     if( muPtcorr(triggerMuon)> 45                                ) passTrigger_ = SingleMu40_passTrigger();//<<<<<<<<< this is Mu40 trigger
     if( muPtcorr(triggerMuon)<=45 &&  muRTightCheck(triggerMuon) ) passTrigger_ = SingleMu_passTrigger();  //<<<<<<<<< this is IsoMu24 trigger
     if( muPtcorr(triggerMuon)<=45 && !muRTightCheck(triggerMuon) ) passTrigger_ = HLT_MuPT_eta2p1_passTrigger(); //<<<<<<<<< this is Mu24 trigger
     fillVariableWithValue("PassTrig", passTrigger_ );//...// returns -2, -1, 0, trig                                  

     //  -- EVENT FILTER SKIM
     int passEventFilter_=0;
     if( passEventFilter() ) passEventFilter_ = 1;
     fillVariableWithValue("PassEventFilter", passEventFilter_ );//.// returns 0, 1 


     //  -- DILEPTON SKIM - MuTau
     int isOfflineDilepton_=0;
     if( isMuTauDR0p30() ) isOfflineDilepton_ = 1;
     fillVariableWithValue("PassOfflineDilepton", isOfflineDilepton_ );// Returns 0, 1          


     // -- HLT MATCHING SKIM
     int passAllMuHLTmatching_=0;
     if( muPtcorr(triggerMuon)> 45                                && RecoHLTdeltaRmin_SingleMu40Trigger(triggerMuon)<0.15 ) passAllMuHLTmatching_=1;
     if( muPtcorr(triggerMuon)<=45 &&  muRTightCheck(triggerMuon) && RecoHLTdeltaRmin_SingleMuTrigger(triggerMuon)<0.15   ) passAllMuHLTmatching_=1;
     if( muPtcorr(triggerMuon)<=45 && !muRTightCheck(triggerMuon) && RecoHLTdeltaRmin_SingleMu24Trigger(triggerMuon)<0.15 ) passAllMuHLTmatching_=1;
     fillVariableWithValue("PassAllMuHLTmatching",passAllMuHLTmatching_);


     // -- OFFLINE SKIM
     int    OfflineCuts_ = 0;
     double ST_          = ST();
     //int    nLepjet      = (MuCounter()+ElCounter()+TauCounter()+JetCounter());
     int    nJet         = JetCounter();
     double LTEMmuPt_    = muPtcorr(ltemMuTau[0]);
     double LTEMtauPt_   = tauPtcorr(ltemMuTau[1]);
     double LTEMmuEta_   = MuonEta->at(ltemMuTau[0]);
     double LTEMtauEta_  = HPSTauEta->at(ltemMuTau[1]);
     double MuJetDR_     = muJetDeltaRmin(ltemMuTau[0]);
     double TauJetDR_    = tauJetDeltaRmin(ltemMuTau[1]);
     double JetN_        = (double)(nJet);
     double BJetN_       = (double)(BJetCounter());
     double MET_         = (double)(METcorr("Pt"));
     double pu_          = (double)(VertexX->size());
     int    EtaRegion    = 0;
     double AveAbsEtaV3_ = GetAveAbsEtaV3();
     if( AveAbsEtaV3_< 0.9 ) EtaRegion=1;
     if( AveAbsEtaV3_>=0.9 ) EtaRegion=2;
     //
     if( ltemMuTau.size()==2 && !isZToMuMu() &&
         ST_>400  && nJet>=2 &&
	 LTEMtauPt_>35 
         ) OfflineCuts_=1;
     //
     fillVariableWithValue("PassOfflineCuts", OfflineCuts_ );// returns 0, 1 
     
     
     evaluateCuts();
     //---------------------------------------------------------------------------------
     if( passedCut("PassJSON") && passedCut("PassTrig") && passedCut("PassEventFilter") && passedCut("PassOfflineDilepton") &&
	 passedCut("PassAllMuHLTmatching") && passedCut("PassOfflineCuts") ){ 


       //-----------------------------------
       InitMuonSFs(ltemMuTau[0]);
       InitTauSFs();
       if( !isData ) ResetAllSFs(); // noSF for MC!
       //if(  isData ) ResetAllSFs_KEEPERROR();// SFs set to 1, keep the SF errors
       //
       // Prompt and Fake Rates
       muPRhisto  ->Fill( muPR(ltemMuTau[0])   );
       muFRhisto  ->Fill( muFR(ltemMuTau[0])   );
       muPREhisto ->Fill( muPRE(ltemMuTau[0])  );
       muFREhisto ->Fill( muFRE(ltemMuTau[0])  );
       tauPRhisto ->Fill( tauPR(ltemMuTau[1])  );
       tauFRhisto ->Fill( tauFR(ltemMuTau[1])  );
       tauPREhisto->Fill( tauPRE(ltemMuTau[1]) );
       tauFREhisto->Fill( tauFRE(ltemMuTau[1]) );
       //-----------------------------------


       //
       /*
       if( jentry%1000 == 0 ){
	 cout<<"-----------------------"<<endl;
	 cout<<"SCALE FACTORS: "<<endl;
	 cout<<" muPRdR3B_SF: "<<muPRdR3B_SF<<endl;  cout<<" muPRdR2B_SF: "<<muPRdR2B_SF<<endl;  cout<<" muPRdR1B_SF: "<<muPRdR1B_SF<<endl;   
	 cout<<" muPRdR3E_SF: "<<muPRdR3E_SF<<endl;  cout<<" muPRdR2E_SF: "<<muPRdR2E_SF<<endl;  cout<<" muPRdR1E_SF: "<<muPRdR1E_SF<<endl;   
	 cout<<"tauPRdR3B_SF: "<<tauPRdR3B_SF<<endl; cout<<"tauPRdR2B_SF: "<<tauPRdR2B_SF<<endl; cout<<"tauPRdR1B_SF: "<<tauPRdR1B_SF<<endl;  
	 cout<<"tauPRdR3E_SF: "<<tauPRdR3E_SF<<endl; cout<<"tauPRdR2E_SF: "<<tauPRdR2E_SF<<endl; cout<<"tauPRdR1E_SF: "<<tauPRdR1E_SF<<endl;  
	 cout<<"tauFRdR3B_SF: "<<tauFRdR3B_SF<<endl; cout<<"tauFRdR2B_SF: "<<tauFRdR2B_SF<<endl; cout<<"tauFRdR1B_SF: "<<tauFRdR1B_SF<<endl;  
	 cout<<"tauFRdR3E_SF: "<<tauFRdR3E_SF<<endl; cout<<"tauFRdR2E_SF: "<<tauFRdR2E_SF<<endl; cout<<"tauFRdR1E_SF: "<<tauFRdR1E_SF<<endl;   
	 cout<<"-----------------------"<<endl;
       }
       */
       //-----------------------------------


       //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

       // LTEM Mu Vector, and LTEM Tau Vector
       TLorentzVector LTEMMuVector, LTEMTauVector;
       LTEMMuVector.SetPtEtaPhiM(  muPtcorr(ltemMuTau[0]),    MuonEta->at(ltemMuTau[0]),   MuonPhi->at(ltemMuTau[0]),   0 );
       LTEMTauVector.SetPtEtaPhiM( tauPtcorr(ltemMuTau[1]),  HPSTauEta->at(ltemMuTau[1]), HPSTauPhi->at(ltemMuTau[1]), 0 );


       // Weight Histos
       Triggerhisto->Fill( usedTrigger_ );
       AppliedPileUpWeightshisto->Fill( safePileupWeights_ );
       AppliedTotalWeightshisto->Fill( w );


       bool isMuTight_       = false;
       bool isTauTight_      = false;
       bool isRecoMuPrompt_  = false;
       bool isRecoTauPrompt_ = false;
       //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       // Check whether LTEM mu & tau are Loose or Tight!!
       if( muRTightCheck(ltemMuTau[0])  ) isMuTight_=true;
       if( tauRTightCheck(ltemMuTau[1]) ) isTauTight_=true;
       // Check whether LTEM mu & tau are Prompt or Fake
       if( isRecoMuPrompt(ltemMuTau[0])  ) isRecoMuPrompt_=true;
       if( isRecoTauPrompt(ltemMuTau[1]) ) isRecoTauPrompt_=true;
       //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

       int iLQlimit=0; //this finds the last cut the event satisfies
       if( ST_ > 410 && LTEMtauPt_>35 ) iLQlimit++; //M200,M250..........1
       if( ST_ > 470 && LTEMtauPt_>50 ) iLQlimit++; //M300...............2
       if( ST_ > 490 && LTEMtauPt_>50 ) iLQlimit++; //M350...............3
       if( ST_ > 680 && LTEMtauPt_>65 ) iLQlimit++; //M400...............4
       if( ST_ > 700 && LTEMtauPt_>65 ) iLQlimit++; //M450...............5
       if( ST_ > 770 && LTEMtauPt_>65 ) iLQlimit++; //M500...............6
       if( ST_ > 800 && LTEMtauPt_>65 ) iLQlimit++; //M550...............7
       if( ST_ > 850 && LTEMtauPt_>65 ) iLQlimit++; //M600,M650..........8
       if( ST_ > 850 && LTEMtauPt_>85 ) iLQlimit++; //M700,M750,M800.....9

       for( int iLQ=1; iLQ<10; iLQ++){ //there are 9 cuts...

	 if( iLQ>iLQlimit ) continue;

	 //if( iLQ!=1 ) continue;
	 //if( ST_ <= 410 || LTEMtauPt_<=35 ) continue;
	 if( iLQ==1 ){
	   unsigned int iLastJet=PFJetPt->size()-1;
	   if( !isMuTight_ &&  !isTauTight_ ){ LowestJetPtLLhisto->Fill( PFJetPt->at(iLastJet) ); }
	   if( !isMuTight_ &&   isTauTight_ ){ LowestJetPtLThisto->Fill( PFJetPt->at(iLastJet) ); }
	   if(  isMuTight_ &&  !isTauTight_ ){ LowestJetPtTLhisto->Fill( PFJetPt->at(iLastJet) ); }
	   if(  isMuTight_ &&   isTauTight_ ){ LowestJetPtTThisto->Fill( PFJetPt->at(iLastJet) ); }
	   if(  isMuTight_ &&   isTauTight_ && EtaRegion==1 ){ LowestJetPtTTbarrelhisto->Fill( PFJetPt->at(iLastJet) ); }
	   if(  isMuTight_ &&   isTauTight_ && EtaRegion==2 ){ LowestJetPtTTendcaphisto->Fill( PFJetPt->at(iLastJet) ); }
	   //
	   //for( unsigned int ijet=0; ijet<PFJetPt->size(); ijet++){
	   //cout<<"Jets: "<<PFJetPt->at(ijet)<<endl;
	   //}
	   //cout<<endl;
	 }

	 if( iLQ==1 && isMuTight_ &&   isTauTight_ && EtaRegion==1 ){ 
	   cout<<"greper Barrel Event  WEIGHT:"<<w<<"  PU:"<< PileUpInteractionsTrue->at(1)<<endl;
	 }
	 if( iLQ==1 && isMuTight_ &&   isTauTight_ && EtaRegion==2 ){ 
	   cout<<"greper Endcap Event  WEIGHT:"<<w<<"  PU:"<< PileUpInteractionsTrue->at(1)<<endl;
	 }

	 if( !isMuTight_ &&  !isTauTight_ ){//LL
	   if( EtaRegion==1 ) BarrelLL_SThisto->Fill( iLQ, ST_, w );
	   if( EtaRegion==1 ) BarrelLL_TAUPThisto->Fill( iLQ, LTEMtauPt_, w );
	   if( EtaRegion==1 ) BarrelLL_TAUETAhisto->Fill( iLQ, LTEMtauEta_, w );
	   if( EtaRegion==1 ) BarrelLL_MUPThisto->Fill( iLQ, LTEMmuPt_, w );
	   if( EtaRegion==1 ) BarrelLL_MUETAhisto->Fill( iLQ, LTEMmuEta_, w );
	   if( EtaRegion==1 ) BarrelLL_MUJETDRhisto->Fill( iLQ, MuJetDR_, w );
	   if( EtaRegion==1 ) BarrelLL_TAUJETDRhisto->Fill( iLQ, TauJetDR_, w );
	   if( EtaRegion==1 ) BarrelLL_JETNhisto->Fill( iLQ, JetN_, w );
	   if( EtaRegion==1 ) BarrelLL_BJETNhisto->Fill( iLQ, BJetN_, w );
	   if( EtaRegion==1 ) BarrelLL_PUhisto->Fill( iLQ, pu_, w );
	   if( EtaRegion==1 ) BarrelLL_METhisto->Fill( iLQ, MET_, w );
	   if( EtaRegion==2 ) EndcapLL_SThisto->Fill( iLQ, ST_, w );
	   if( EtaRegion==2 ) EndcapLL_TAUPThisto->Fill( iLQ, LTEMtauPt_, w );
	   if( EtaRegion==2 ) EndcapLL_TAUETAhisto->Fill( iLQ, LTEMtauEta_, w );
	   if( EtaRegion==2 ) EndcapLL_MUPThisto->Fill( iLQ, LTEMmuPt_, w );
	   if( EtaRegion==2 ) EndcapLL_MUETAhisto->Fill( iLQ, LTEMmuEta_, w );
           if( EtaRegion==2 ) EndcapLL_MUJETDRhisto->Fill( iLQ, MuJetDR_, w );
	   if( EtaRegion==2 ) EndcapLL_TAUJETDRhisto->Fill( iLQ, TauJetDR_, w );
           if( EtaRegion==2 ) EndcapLL_JETNhisto->Fill( iLQ, JetN_, w );
           if( EtaRegion==2 ) EndcapLL_BJETNhisto->Fill( iLQ, BJetN_, w );
           if( EtaRegion==2 ) EndcapLL_PUhisto->Fill( iLQ, pu_, w );
           if( EtaRegion==2 ) EndcapLL_METhisto->Fill( iLQ, MET_, w );
	 }
	 if( !isMuTight_ &&   isTauTight_ ){//LT
	   if( EtaRegion==1 ) BarrelLT_SThisto->Fill( iLQ, ST_, w );
	   if( EtaRegion==1 ) BarrelLT_TAUPThisto->Fill( iLQ, LTEMtauPt_, w );
	   if( EtaRegion==1 ) BarrelLT_TAUETAhisto->Fill( iLQ, LTEMtauEta_, w );
	   if( EtaRegion==1 ) BarrelLT_MUPThisto->Fill( iLQ, LTEMmuPt_, w );
	   if( EtaRegion==1 ) BarrelLT_MUETAhisto->Fill( iLQ, LTEMmuEta_, w );
           if( EtaRegion==1 ) BarrelLT_MUJETDRhisto->Fill( iLQ, MuJetDR_, w );
	   if( EtaRegion==1 ) BarrelLT_TAUJETDRhisto->Fill( iLQ, TauJetDR_, w );
           if( EtaRegion==1 ) BarrelLT_JETNhisto->Fill( iLQ, JetN_, w );
           if( EtaRegion==1 ) BarrelLT_BJETNhisto->Fill( iLQ, BJetN_, w );
           if( EtaRegion==1 ) BarrelLT_PUhisto->Fill( iLQ, pu_, w );
           if( EtaRegion==1 ) BarrelLT_METhisto->Fill( iLQ, MET_, w );
	   if( EtaRegion==2 ) EndcapLT_SThisto->Fill( iLQ, ST_, w );
	   if( EtaRegion==2 ) EndcapLT_TAUPThisto->Fill( iLQ, LTEMtauPt_, w );
	   if( EtaRegion==2 ) EndcapLT_TAUETAhisto->Fill( iLQ, LTEMtauEta_, w );
	   if( EtaRegion==2 ) EndcapLT_MUPThisto->Fill( iLQ, LTEMmuPt_, w );
	   if( EtaRegion==2 ) EndcapLT_MUETAhisto->Fill( iLQ, LTEMmuEta_, w );
	   if( EtaRegion==2 ) EndcapLT_MUJETDRhisto->Fill( iLQ, MuJetDR_, w );
           if( EtaRegion==2 ) EndcapLT_TAUJETDRhisto->Fill( iLQ, TauJetDR_, w );
           if( EtaRegion==2 ) EndcapLT_JETNhisto->Fill( iLQ, JetN_, w );
           if( EtaRegion==2 ) EndcapLT_BJETNhisto->Fill( iLQ, BJetN_, w );
           if( EtaRegion==2 ) EndcapLT_PUhisto->Fill( iLQ, pu_, w );
           if( EtaRegion==2 ) EndcapLT_METhisto->Fill( iLQ, MET_, w );
	 }
	 if(  isMuTight_ &&  !isTauTight_ ){//TL
	   if( EtaRegion==1 ) BarrelTL_SThisto->Fill( iLQ, ST_, w );
	   if( EtaRegion==1 ) BarrelTL_TAUPThisto->Fill( iLQ, LTEMtauPt_, w );
	   if( EtaRegion==1 ) BarrelTL_TAUETAhisto->Fill( iLQ, LTEMtauEta_, w );
	   if( EtaRegion==1 ) BarrelTL_MUPThisto->Fill( iLQ, LTEMmuPt_, w );
	   if( EtaRegion==1 ) BarrelTL_MUETAhisto->Fill( iLQ, LTEMmuEta_, w );
           if( EtaRegion==1 ) BarrelTL_MUJETDRhisto->Fill( iLQ, MuJetDR_, w );
	   if( EtaRegion==1 ) BarrelTL_TAUJETDRhisto->Fill( iLQ, TauJetDR_, w );
           if( EtaRegion==1 ) BarrelTL_JETNhisto->Fill( iLQ, JetN_, w );
           if( EtaRegion==1 ) BarrelTL_BJETNhisto->Fill( iLQ, BJetN_, w );
           if( EtaRegion==1 ) BarrelTL_PUhisto->Fill( iLQ, pu_, w );
           if( EtaRegion==1 ) BarrelTL_METhisto->Fill( iLQ, MET_, w );
	   if( EtaRegion==2 ) EndcapTL_SThisto->Fill( iLQ, ST_, w );
	   if( EtaRegion==2 ) EndcapTL_TAUPThisto->Fill( iLQ, LTEMtauPt_, w );
	   if( EtaRegion==2 ) EndcapTL_TAUETAhisto->Fill( iLQ, LTEMtauEta_, w );
	   if( EtaRegion==2 ) EndcapTL_MUPThisto->Fill( iLQ, LTEMmuPt_, w );
	   if( EtaRegion==2 ) EndcapTL_MUETAhisto->Fill( iLQ, LTEMmuEta_, w );
	   if( EtaRegion==2 ) EndcapTL_MUJETDRhisto->Fill( iLQ, MuJetDR_, w );
           if( EtaRegion==2 ) EndcapTL_TAUJETDRhisto->Fill( iLQ, TauJetDR_, w );
           if( EtaRegion==2 ) EndcapTL_JETNhisto->Fill( iLQ, JetN_, w );
           if( EtaRegion==2 ) EndcapTL_BJETNhisto->Fill( iLQ, BJetN_, w );
           if( EtaRegion==2 ) EndcapTL_PUhisto->Fill( iLQ, pu_, w );
           if( EtaRegion==2 ) EndcapTL_METhisto->Fill( iLQ, MET_, w );
	 }
	 if(  isMuTight_ &&   isTauTight_ ){//TT
	   if( EtaRegion==1 ) BarrelTT_SThisto->Fill( iLQ, ST_, w );
	   if( EtaRegion==1 ) BarrelTT_TAUPThisto->Fill( iLQ, LTEMtauPt_, w );
	   if( EtaRegion==1 ) BarrelTT_TAUETAhisto->Fill( iLQ, LTEMtauEta_, w );
	   if( EtaRegion==1 ) BarrelTT_MUPThisto->Fill( iLQ, LTEMmuPt_, w );
	   if( EtaRegion==1 ) BarrelTT_MUETAhisto->Fill( iLQ, LTEMmuEta_, w );
	   if( EtaRegion==1 ) BarrelTT_MUJETDRhisto->Fill( iLQ, MuJetDR_, w );
           if( EtaRegion==1 ) BarrelTT_TAUJETDRhisto->Fill( iLQ, TauJetDR_, w );
           if( EtaRegion==1 ) BarrelTT_JETNhisto->Fill( iLQ, JetN_, w );
           if( EtaRegion==1 ) BarrelTT_BJETNhisto->Fill( iLQ, BJetN_, w );
           if( EtaRegion==1 ) BarrelTT_PUhisto->Fill( iLQ, pu_, w );
           if( EtaRegion==1 ) BarrelTT_METhisto->Fill( iLQ, MET_, w );
	   if( EtaRegion==2 ) EndcapTT_SThisto->Fill( iLQ, ST_, w );
	   if( EtaRegion==2 ) EndcapTT_TAUPThisto->Fill( iLQ, LTEMtauPt_, w );
	   if( EtaRegion==2 ) EndcapTT_TAUETAhisto->Fill( iLQ, LTEMtauEta_, w );
	   if( EtaRegion==2 ) EndcapTT_MUPThisto->Fill( iLQ, LTEMmuPt_, w );
	   if( EtaRegion==2 ) EndcapTT_MUETAhisto->Fill( iLQ, LTEMmuEta_, w );
           if( EtaRegion==2 ) EndcapTT_MUJETDRhisto->Fill( iLQ, MuJetDR_, w );
           if( EtaRegion==2 ) EndcapTT_TAUJETDRhisto->Fill( iLQ, TauJetDR_, w );
           if( EtaRegion==2 ) EndcapTT_JETNhisto->Fill( iLQ, JetN_, w );
           if( EtaRegion==2 ) EndcapTT_BJETNhisto->Fill( iLQ, BJetN_, w );
           if( EtaRegion==2 ) EndcapTT_PUhisto->Fill( iLQ, pu_, w );
           if( EtaRegion==2 ) EndcapTT_METhisto->Fill( iLQ, MET_, w );
	 }
	 //if( isData && iLQ==1 && isMuTight_ &&   isTauTight_ ){//TT
	 /*
	 if( isData && isMuTight_ &&   isTauTight_ ){//TT
	   if( EtaRegion==1 ){ 
	     cout<<"("<<event<<")iEBar    ID: "<<event<<" "<<run<<" "<<ls<<" "<<orbit<<"   iLQ"<<iLQ<<endl;
	     cout<<"("<<event<<")iEBar    ST: "<<ST_<<"   iLQ"<<iLQ<<endl;
	     cout<<"("<<event<<")iEBar TAUPT: "<<LTEMtauPt_<<" "<<HPSTauEta->at(ltemMuTau[1])<<" "<<HPSTauPhi->at(ltemMuTau[1])<<"   iLQ"<<iLQ<<endl;
	     cout<<"("<<event<<")iEBar  MUPT: "<<LTEMmuPt_<<" "<<MuonEta->at(ltemMuTau[0])<<" "<<MuonPhi->at(ltemMuTau[0])<<"   iLQ"<<iLQ<<endl;
	     cout<<"("<<event<<")iEBar  NJET: "<<JetN_<<"   iLQ"<<iLQ<<endl;
	     cout<<"("<<event<<")iEBar  SumQ: "<<MuonCharge->at(ltemMuTau[0])+HPSTauCharge->at(ltemMuTau[1])<<"   iLQ"<<iLQ<<endl;
	   }
	   if( EtaRegion==2 ){ 
	     cout<<"("<<event<<")iEEnd    ID: "<<event<<" "<<run<<" "<<ls<<" "<<orbit<<"   iLQ"<<iLQ<<endl;
	     cout<<"("<<event<<")iEEnd    ST: "<<ST_<<"   iLQ"<<iLQ<<endl;
	     cout<<"("<<event<<")iEEnd TAUPT: "<<LTEMtauPt_<<" "<<HPSTauEta->at(ltemMuTau[1])<<" "<<HPSTauPhi->at(ltemMuTau[1])<<"   iLQ"<<iLQ<<endl;
	     cout<<"("<<event<<")iEEnd  MUPT: "<<LTEMmuPt_<<" "<<MuonEta->at(ltemMuTau[0])<<" "<<MuonPhi->at(ltemMuTau[0])<<"   iLQ"<<iLQ<<endl;
	     cout<<"("<<event<<")iEEnd  NJET: "<<JetN_<<"   iLQ"<<iLQ<<endl;
	     cout<<"("<<event<<")iEEnd  SumQ: "<<MuonCharge->at(ltemMuTau[0])+HPSTauCharge->at(ltemMuTau[1])<<"   iLQ"<<iLQ<<endl;
	   }
	   for(unsigned int iJetR=0; iJetR<PFJetPt->size();  iJetR++){
	     if(!jetRisoCheck(iJetR)) continue;
	     cout<<"("<<event<<")AllJet: "<<PFJetPt->at(iJetR)<<" "<<PFJetEta->at(iJetR)<<" "<<PFJetPhi->at(iJetR)<<endl;
	   }
	   for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
	     if(!muRisoCheck(iMuR)       ) continue;
	     cout<<"("<<event<<") AllMu: "<<MuonPt->at(iMuR)<<" "<<MuonEta->at(iMuR)<<" "<<MuonPhi->at(iMuR)<<endl;
	   }
	   for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
	     if(!tauRisoCheck(iTauR)) continue;
	     cout<<"("<<event<<")AllTau: "<<HPSTauPt->at(iTauR)<<" "<<HPSTauEta->at(iTauR)<<" "<<HPSTauPhi->at(iTauR)<<endl;
	   }
	 }
	 */
	 //
	 if( isMuTight_ && isTauTight_ ){
	   if(iLQ==1 && EtaRegion==1 ) BackgroundDecompositionhisto->Fill(1,w);//barrel M200
	   if(iLQ==4 && EtaRegion==1 ) BackgroundDecompositionhisto->Fill(4,w);//barrel M400
	   if(iLQ==8 && EtaRegion==1 ) BackgroundDecompositionhisto->Fill(7,w);//barrel M600
	   if(iLQ==1 && EtaRegion==2 ) BackgroundDecompositionhisto->Fill(2,w);//endcap M200
	   if(iLQ==4 && EtaRegion==2 ) BackgroundDecompositionhisto->Fill(5,w);//endcap M400
	   if(iLQ==8 && EtaRegion==2 ) BackgroundDecompositionhisto->Fill(8,w);//endcap M600
	 }
	 //
	 if( EtaRegion==1 && isMuTight_ &&  isTauTight_ ){//gen level breakdown  in the TT box
	   if( !isRecoMuPrompt_ && !isRecoTauPrompt_ ) BarrelFPmatrixhisto->Fill( iLQ, 1, w ); //FF
	   if( !isRecoMuPrompt_ &&  isRecoTauPrompt_ ) BarrelFPmatrixhisto->Fill( iLQ, 2, w ); //FP
	   if(  isRecoMuPrompt_ && !isRecoTauPrompt_ ) BarrelFPmatrixhisto->Fill( iLQ, 3, w ); //PF
	   if(  isRecoMuPrompt_ &&  isRecoTauPrompt_ ) BarrelFPmatrixhisto->Fill( iLQ, 4, w ); //PP
	   if(  isRecoMuPrompt_ &&  isRecoTauPrompt_ && !isRecoTauChargeFlip(ltemMuTau[1]) ) BarrelFPmatrixhisto->Fill( iLQ, 5, w ); //PP SS
	   if(  isRecoMuPrompt_ &&  isRecoTauPrompt_ &&  isRecoTauChargeFlip(ltemMuTau[1]) ) BarrelFPmatrixhisto->Fill( iLQ, 6, w ); //PP OS
	   if( !isRecoMuPrompt_ && !isRecoTauPrompt_ ) BarrelFPmatrixW1histo->Fill( iLQ, 1, 1 ); //FF
	   if( !isRecoMuPrompt_ &&  isRecoTauPrompt_ ) BarrelFPmatrixW1histo->Fill( iLQ, 2, 1 ); //FP
	   if(  isRecoMuPrompt_ && !isRecoTauPrompt_ ) BarrelFPmatrixW1histo->Fill( iLQ, 3, 1 ); //PF
	   if(  isRecoMuPrompt_ &&  isRecoTauPrompt_ ) BarrelFPmatrixW1histo->Fill( iLQ, 4, 1 ); //PP
	   if(  isRecoMuPrompt_ &&  isRecoTauPrompt_ && !isRecoTauChargeFlip(ltemMuTau[1]) ) BarrelFPmatrixW1histo->Fill( iLQ, 5, 1 ); //PP SS
	   if(  isRecoMuPrompt_ &&  isRecoTauPrompt_ &&  isRecoTauChargeFlip(ltemMuTau[1]) ) BarrelFPmatrixW1histo->Fill( iLQ, 6, 1 ); //PP OS
	 }
	 if( EtaRegion==2 && isMuTight_ &&  isTauTight_ ){//gen level breakdown  in the TT box
	   if( !isRecoMuPrompt_ && !isRecoTauPrompt_ ) EndcapFPmatrixhisto->Fill( iLQ, 1, w ); //FF
	   if( !isRecoMuPrompt_ &&  isRecoTauPrompt_ ) EndcapFPmatrixhisto->Fill( iLQ, 2, w ); //FP
	   if(  isRecoMuPrompt_ && !isRecoTauPrompt_ ) EndcapFPmatrixhisto->Fill( iLQ, 3, w ); //PF
	   if(  isRecoMuPrompt_ &&  isRecoTauPrompt_ ) EndcapFPmatrixhisto->Fill( iLQ, 4, w ); //PP
	   if(  isRecoMuPrompt_ &&  isRecoTauPrompt_ && !isRecoTauChargeFlip(ltemMuTau[1]) ) EndcapFPmatrixhisto->Fill( iLQ, 5, w ); //PP SS
	   if(  isRecoMuPrompt_ &&  isRecoTauPrompt_ &&  isRecoTauChargeFlip(ltemMuTau[1]) ) EndcapFPmatrixhisto->Fill( iLQ, 6, w ); //PP OS
	   if( !isRecoMuPrompt_ && !isRecoTauPrompt_ ) EndcapFPmatrixW1histo->Fill( iLQ, 1, 1 ); //FF
	   if( !isRecoMuPrompt_ &&  isRecoTauPrompt_ ) EndcapFPmatrixW1histo->Fill( iLQ, 2, 1 ); //FP
	   if(  isRecoMuPrompt_ && !isRecoTauPrompt_ ) EndcapFPmatrixW1histo->Fill( iLQ, 3, 1 ); //PF
	   if(  isRecoMuPrompt_ &&  isRecoTauPrompt_ ) EndcapFPmatrixW1histo->Fill( iLQ, 4, 1 ); //PP
	   if(  isRecoMuPrompt_ &&  isRecoTauPrompt_ && !isRecoTauChargeFlip(ltemMuTau[1]) ) EndcapFPmatrixW1histo->Fill( iLQ, 5, 1 ); //PP SS
	   if(  isRecoMuPrompt_ &&  isRecoTauPrompt_ &&  isRecoTauChargeFlip(ltemMuTau[1]) ) EndcapFPmatrixW1histo->Fill( iLQ, 6, 1 ); //PP OS
	 }
	 //
	 if( EtaRegion==1 ){
	   if( !isMuTight_ && !isTauTight_ ) BarrelLTmatrixhisto->Fill( iLQ, 1, w ); //LL
	   if( !isMuTight_ &&  isTauTight_ ) BarrelLTmatrixhisto->Fill( iLQ, 2, w ); //LT
	   if(  isMuTight_ && !isTauTight_ ) BarrelLTmatrixhisto->Fill( iLQ, 3, w ); //TL
	   if(  isMuTight_ &&  isTauTight_ ) BarrelLTmatrixhisto->Fill( iLQ, 4, w ); //TT
	 }
	 if( EtaRegion==2 ){
	   if( !isMuTight_ && !isTauTight_ ) EndcapLTmatrixhisto->Fill( iLQ, 1, w ); //LL
	   if( !isMuTight_ &&  isTauTight_ ) EndcapLTmatrixhisto->Fill( iLQ, 2, w ); //LT
	   if(  isMuTight_ && !isTauTight_ ) EndcapLTmatrixhisto->Fill( iLQ, 3, w ); //TL
	   if(  isMuTight_ &&  isTauTight_ ) EndcapLTmatrixhisto->Fill( iLQ, 4, w ); //TT
	 }
	 //
	 double TTDoubleFake_=0;
	 double TTSingleMuFake_=0;
	 double TTSingleTauFake_=0;
	 double TTtotalBckg_=0;
	 
	 //
	 // Obs
	 if( EtaRegion==1 && isMuTight_ && isTauTight_ ) BarrelTTBckgSyshisto->Fill( iLQ, 1, w );
	 if( EtaRegion==2 && isMuTight_ && isTauTight_ ) EndcapTTBckgSyshisto->Fill( iLQ, 1, w );
	 if( EtaRegion==1 && isMuTight_ && isTauTight_ ) BarrelTTBckgSysW1histo->Fill( iLQ, 1, 1 );
	 if( EtaRegion==2 && isMuTight_ && isTauTight_ ) EndcapTTBckgSysW1histo->Fill( iLQ, 1, 1 );
         if( EtaRegion==1 && isMuTight_ && isTauTight_ && iLQ==1 ) BarrelEXP_SThisto->Fill( 1, ST_, w );
         if( EtaRegion==2 && isMuTight_ && isTauTight_ && iLQ==1 ) EndcapEXP_SThisto->Fill( 1, ST_, w );
         if( EtaRegion==1 && isMuTight_ && isTauTight_ && iLQ==1 ) BarrelEXP_TAUPThisto->Fill( 1, LTEMtauPt_, w );
         if( EtaRegion==2 && isMuTight_ && isTauTight_ && iLQ==1 ) EndcapEXP_TAUPThisto->Fill( 1, LTEMtauPt_, w );
         if( EtaRegion==1 && isMuTight_ && isTauTight_ && iLQ==1 ) BarrelEXP_MUPThisto->Fill( 1, LTEMmuPt_, w );
         if( EtaRegion==2 && isMuTight_ && isTauTight_ && iLQ==1 ) EndcapEXP_MUPThisto->Fill( 1, LTEMmuPt_, w );
         if( EtaRegion==1 && isMuTight_ && isTauTight_ && iLQ==1 ) BarrelEXP_TAUETAhisto->Fill( 1, LTEMtauEta_, w );
         if( EtaRegion==2 && isMuTight_ && isTauTight_ && iLQ==1 ) EndcapEXP_TAUETAhisto->Fill( 1, LTEMtauEta_, w );
         if( EtaRegion==1 && isMuTight_ && isTauTight_ && iLQ==1 ) BarrelEXP_MUETAhisto->Fill( 1, LTEMmuEta_, w );
         if( EtaRegion==2 && isMuTight_ && isTauTight_ && iLQ==1 ) EndcapEXP_MUETAhisto->Fill( 1, LTEMmuEta_, w );
         if( EtaRegion==1 && isMuTight_ && isTauTight_ && iLQ==1 ) BarrelEXP_MUJETDRhisto->Fill( 1, MuJetDR_, w );
         if( EtaRegion==2 && isMuTight_ && isTauTight_ && iLQ==1 ) EndcapEXP_MUJETDRhisto->Fill( 1, MuJetDR_, w );
         if( EtaRegion==1 && isMuTight_ && isTauTight_ && iLQ==1 ) BarrelEXP_TAUJETDRhisto->Fill( 1, TauJetDR_, w );
         if( EtaRegion==2 && isMuTight_ && isTauTight_ && iLQ==1 ) EndcapEXP_TAUJETDRhisto->Fill( 1, TauJetDR_, w );
         if( EtaRegion==1 && isMuTight_ && isTauTight_ && iLQ==1 ) BarrelEXP_JETNhisto->Fill( 1, JetN_, w );
         if( EtaRegion==2 && isMuTight_ && isTauTight_ && iLQ==1 ) EndcapEXP_JETNhisto->Fill( 1, JetN_, w );
         if( EtaRegion==1 && isMuTight_ && isTauTight_ && iLQ==1 ) BarrelEXP_BJETNhisto->Fill( 1, BJetN_, w );
         if( EtaRegion==2 && isMuTight_ && isTauTight_ && iLQ==1 ) EndcapEXP_BJETNhisto->Fill( 1, BJetN_, w );
         if( EtaRegion==1 && isMuTight_ && isTauTight_ && iLQ==1 ) BarrelEXP_PUhisto->Fill( 1, pu_, w );
         if( EtaRegion==2 && isMuTight_ && isTauTight_ && iLQ==1 ) EndcapEXP_PUhisto->Fill( 1, pu_, w );
         if( EtaRegion==1 && isMuTight_ && isTauTight_ && iLQ==1 ) BarrelEXP_METhisto->Fill( 1, MET_, w );
         if( EtaRegion==2 && isMuTight_ && isTauTight_ && iLQ==1 ) EndcapEXP_METhisto->Fill( 1, MET_, w );
	 // Exp Mean
	 muPR_  = muPR(ltemMuTau[0]);
	 muFR_  = muFR(ltemMuTau[0]);
	 tauPR_ = tauPR(ltemMuTau[1]);
	 tauFR_ = tauFR(ltemMuTau[1]);
	 //
	 TTDoubleFake_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] )*muFR_*tauFR_;
	 TTSingleMuFake_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] )*muFR_*tauPR_;
	 TTSingleTauFake_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] )*muPR_*tauFR_;
	 TTtotalBckg_     = (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_);
	 if( EtaRegion==1 ) BarrelTTBckgSyshisto->Fill( iLQ, 2, TTtotalBckg_*w );
	 if( EtaRegion==2 ) EndcapTTBckgSyshisto->Fill( iLQ, 2, TTtotalBckg_*w );
	 if( EtaRegion==1 ) BarrelTTBckgSysW1histo->Fill( iLQ, 2, 1 );
	 if( EtaRegion==2 ) EndcapTTBckgSysW1histo->Fill( iLQ, 2, 1 );
	 //..
	 if( EtaRegion==1 ) BarrelExpFakeBreakdownhisto->Fill( iLQ, 1, TTDoubleFake_*w );   //FFinTT
	 if( EtaRegion==1 ) BarrelExpFakeBreakdownhisto->Fill( iLQ, 2, TTSingleMuFake_*w ); //FPinTT
	 if( EtaRegion==1 ) BarrelExpFakeBreakdownhisto->Fill( iLQ, 3, TTSingleTauFake_*w );//PFinTT
	 if( EtaRegion==2 ) EndcapExpFakeBreakdownhisto->Fill( iLQ, 1, TTDoubleFake_*w );   //FFinTT
	 if( EtaRegion==2 ) EndcapExpFakeBreakdownhisto->Fill( iLQ, 2, TTSingleMuFake_*w ); //FPinTT
	 if( EtaRegion==2 ) EndcapExpFakeBreakdownhisto->Fill( iLQ, 3, TTSingleTauFake_*w );//PFinTT
	 if( EtaRegion==1 ) BarrelExpFakeBreakdownW1histo->Fill( iLQ, 1, 1 );
	 if( EtaRegion==1 ) BarrelExpFakeBreakdownW1histo->Fill( iLQ, 2, 1 );
	 if( EtaRegion==1 ) BarrelExpFakeBreakdownW1histo->Fill( iLQ, 3, 1 );
	 if( EtaRegion==2 ) EndcapExpFakeBreakdownW1histo->Fill( iLQ, 1, 1 );
	 if( EtaRegion==2 ) EndcapExpFakeBreakdownW1histo->Fill( iLQ, 2, 1 );
	 if( EtaRegion==2 ) EndcapExpFakeBreakdownW1histo->Fill( iLQ, 3, 1 );
	 // ..
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_SThisto->Fill( 2, ST_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_SThisto->Fill( 2, ST_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUPThisto->Fill( 2, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUPThisto->Fill( 2, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUPThisto->Fill( 2, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUPThisto->Fill( 2, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUETAhisto->Fill( 2, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUETAhisto->Fill( 2, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUETAhisto->Fill( 2, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUETAhisto->Fill( 2, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUJETDRhisto->Fill( 2, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUJETDRhisto->Fill( 2, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUJETDRhisto->Fill( 2, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUJETDRhisto->Fill( 2, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_JETNhisto->Fill( 2, JetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_JETNhisto->Fill( 2, JetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_BJETNhisto->Fill( 2, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_BJETNhisto->Fill( 2, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_PUhisto->Fill( 2, pu_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_PUhisto->Fill( 2, pu_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_METhisto->Fill( 2, MET_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_METhisto->Fill( 2, MET_, TTtotalBckg_*w );

	 //
	 // PP
	 if( EtaRegion==1 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ ) BarrelTTBckgSyshisto->Fill( iLQ, 3, w );
	 if( EtaRegion==2 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ ) EndcapTTBckgSyshisto->Fill( iLQ, 3, w );
	 if( EtaRegion==1 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ ) BarrelTTBckgSysW1histo->Fill( iLQ, 3, 1 );
	 if( EtaRegion==2 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ ) EndcapTTBckgSysW1histo->Fill( iLQ, 3, 1 );
         if( EtaRegion==1 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )BarrelEXP_SThisto->Fill( 3, ST_, w );
         if( EtaRegion==2 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )EndcapEXP_SThisto->Fill( 3, ST_, w );
         if( EtaRegion==1 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )BarrelEXP_TAUPThisto->Fill( 3,LTEMtauPt_,w );
         if( EtaRegion==2 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )EndcapEXP_TAUPThisto->Fill( 3,LTEMtauPt_,w );
         if( EtaRegion==1 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )BarrelEXP_MUPThisto->Fill( 3,LTEMmuPt_,w );
         if( EtaRegion==2 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )EndcapEXP_MUPThisto->Fill( 3,LTEMmuPt_,w );
         if( EtaRegion==1 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )BarrelEXP_TAUETAhisto->Fill(3,LTEMtauEta_,w);
         if( EtaRegion==2 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )EndcapEXP_TAUETAhisto->Fill(3,LTEMtauEta_,w);
         if( EtaRegion==1 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )BarrelEXP_MUETAhisto->Fill( 3,LTEMmuEta_, w);
         if( EtaRegion==2 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )EndcapEXP_MUETAhisto->Fill( 3,LTEMmuEta_, w);
         if( EtaRegion==1 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )BarrelEXP_MUJETDRhisto->Fill( 3,MuJetDR_,w );
         if( EtaRegion==2 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )EndcapEXP_MUJETDRhisto->Fill( 3,MuJetDR_,w );
         if( EtaRegion==1 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )BarrelEXP_TAUJETDRhisto->Fill( 3,TauJetDR_,w );
         if( EtaRegion==2 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )EndcapEXP_TAUJETDRhisto->Fill( 3,TauJetDR_,w );
         if( EtaRegion==1 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )BarrelEXP_JETNhisto->Fill( 3,JetN_,w );
         if( EtaRegion==2 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )EndcapEXP_JETNhisto->Fill( 3,JetN_,w );
         if( EtaRegion==1 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )BarrelEXP_BJETNhisto->Fill( 3,BJetN_,w );
         if( EtaRegion==2 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )EndcapEXP_BJETNhisto->Fill( 3,BJetN_,w );
         if( EtaRegion==1 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )BarrelEXP_PUhisto->Fill( 3,pu_,w );
         if( EtaRegion==2 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )EndcapEXP_PUhisto->Fill( 3,pu_,w );
         if( EtaRegion==1 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )BarrelEXP_METhisto->Fill( 3,MET_,w );
         if( EtaRegion==2 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ && iLQ==1 )EndcapEXP_METhisto->Fill( 3,MET_,w );

	 //
	 /*
	 std::cout<<"debug: muPR:: "<<muPR_<<" +/- "<<muPRE(ltemMuTau[0])<<" R:"<<muPRE(ltemMuTau[0])/muPR_<<endl;
	 std::cout<<"debug: muFR:: "<<muFR_<<" +/- "<<muFRE(ltemMuTau[0])<<" R:"<<muFRE(ltemMuTau[0])/muFR_<<endl;
	 std::cout<<"debug:tauPR:: "<<tauPR_<<" +/- "<<tauPRE(ltemMuTau[1])<<" R:"<<tauPRE(ltemMuTau[1])/tauPR_<<endl;
	 std::cout<<"debug:tauFR:: "<<tauFR_<<" +/- "<<tauFRE(ltemMuTau[1])<<" R:"<<tauFRE(ltemMuTau[1])/tauFR_<<endl;
	 cout<<endl;
	 */
	 //
	 // TauFRU
	 muPR_  = muPR(ltemMuTau[0]);
	 muFR_  = muFR(ltemMuTau[0]);
	 tauPR_ = tauPR(ltemMuTau[1]);
	 tauFR_ = tauFR(ltemMuTau[1])+tauFRE(ltemMuTau[1]);
	 TTDoubleFake_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] )*muFR_*tauFR_;
	 TTSingleMuFake_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] )*muFR_*tauPR_;
	 TTSingleTauFake_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] )*muPR_*tauFR_;
	 TTtotalBckg_     = (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_);
	 if( EtaRegion==1 ) BarrelTTBckgSyshisto->Fill( iLQ, 4, TTtotalBckg_*w );
	 if( EtaRegion==2 ) EndcapTTBckgSyshisto->Fill( iLQ, 4, TTtotalBckg_*w );
	 if( EtaRegion==1 ) BarrelTTBckgSysW1histo->Fill( iLQ, 4, 1 );
	 if( EtaRegion==2 ) EndcapTTBckgSysW1histo->Fill( iLQ, 4, 1 );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_SThisto->Fill( 4, ST_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_SThisto->Fill( 4, ST_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUPThisto->Fill( 4, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUPThisto->Fill( 4, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUPThisto->Fill( 4, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUPThisto->Fill( 4, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUETAhisto->Fill( 4, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUETAhisto->Fill( 4, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUETAhisto->Fill( 4, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUETAhisto->Fill( 4, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUJETDRhisto->Fill( 4, MuJetDR_,  TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUJETDRhisto->Fill( 4, MuJetDR_,  TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUJETDRhisto->Fill( 4, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUJETDRhisto->Fill( 4, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_JETNhisto->Fill( 4, JetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_JETNhisto->Fill( 4, JetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_BJETNhisto->Fill( 4, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_BJETNhisto->Fill( 4, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_PUhisto->Fill( 4, pu_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_PUhisto->Fill( 4, pu_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_METhisto->Fill( 4, MET_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_METhisto->Fill( 4, MET_, TTtotalBckg_*w );

	 //
	 // TauFRD
	 muPR_  = muPR(ltemMuTau[0]);
	 muFR_  = muFR(ltemMuTau[0]);
	 tauPR_ = tauPR(ltemMuTau[1]);
	 tauFR_ = tauFR(ltemMuTau[1])-tauFRE(ltemMuTau[1]);
	 TTDoubleFake_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] )*muFR_*tauFR_;
	 TTSingleMuFake_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] )*muFR_*tauPR_;
	 TTSingleTauFake_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] )*muPR_*tauFR_;
	 TTtotalBckg_     = (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_);
	 if( EtaRegion==1 ) BarrelTTBckgSyshisto->Fill( iLQ, 5, TTtotalBckg_*w );
	 if( EtaRegion==2 ) EndcapTTBckgSyshisto->Fill( iLQ, 5, TTtotalBckg_*w );
	 if( EtaRegion==1 ) BarrelTTBckgSysW1histo->Fill( iLQ, 5, 1 );
	 if( EtaRegion==2 ) EndcapTTBckgSysW1histo->Fill( iLQ, 5, 1 );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_SThisto->Fill( 5, ST_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_SThisto->Fill( 5, ST_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUPThisto->Fill( 5, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUPThisto->Fill( 5, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUPThisto->Fill( 5, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUPThisto->Fill( 5, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUETAhisto->Fill( 5, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUETAhisto->Fill( 5, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUETAhisto->Fill( 5, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUETAhisto->Fill( 5, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUJETDRhisto->Fill( 5, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUJETDRhisto->Fill( 5, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUJETDRhisto->Fill( 5, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUJETDRhisto->Fill( 5, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_JETNhisto->Fill( 5, JetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_JETNhisto->Fill( 5, JetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_BJETNhisto->Fill( 5, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_BJETNhisto->Fill( 5, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_PUhisto->Fill( 5, pu_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_PUhisto->Fill( 5, pu_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_METhisto->Fill( 5, MET_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_METhisto->Fill( 5, MET_, TTtotalBckg_*w );

	 //
	 // TauPRU
	 muPR_  = muPR(ltemMuTau[0]);
	 muFR_  = muFR(ltemMuTau[0]);
	 tauPR_ = tauPR(ltemMuTau[1])+tauPRE(ltemMuTau[1]);
	 tauFR_ = tauFR(ltemMuTau[1]);
	 TTDoubleFake_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] )*muFR_*tauFR_;
	 TTSingleMuFake_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] )*muFR_*tauPR_;
	 TTSingleTauFake_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] )*muPR_*tauFR_;
	 TTtotalBckg_     = (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_);
	 if( EtaRegion==1 ) BarrelTTBckgSyshisto->Fill( iLQ, 6, TTtotalBckg_*w );
	 if( EtaRegion==2 ) EndcapTTBckgSyshisto->Fill( iLQ, 6, TTtotalBckg_*w );
	 if( EtaRegion==1 ) BarrelTTBckgSysW1histo->Fill( iLQ, 6, 1 );
	 if( EtaRegion==2 ) EndcapTTBckgSysW1histo->Fill( iLQ, 6, 1 );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_SThisto->Fill( 6, ST_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_SThisto->Fill( 6, ST_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUPThisto->Fill( 6, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUPThisto->Fill( 6, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUPThisto->Fill( 6, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUPThisto->Fill( 6, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUETAhisto->Fill( 6, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUETAhisto->Fill( 6, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUETAhisto->Fill( 6, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUETAhisto->Fill( 6, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUJETDRhisto->Fill( 6, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUJETDRhisto->Fill( 6, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUJETDRhisto->Fill( 6, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUJETDRhisto->Fill( 6, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_JETNhisto->Fill( 6, JetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_JETNhisto->Fill( 6, JetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_BJETNhisto->Fill( 6, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_BJETNhisto->Fill( 6, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_PUhisto->Fill( 6, pu_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_PUhisto->Fill( 6, pu_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_METhisto->Fill( 6, MET_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_METhisto->Fill( 6, MET_, TTtotalBckg_*w );

	 //
	 // TauPRD
	 muPR_  = muPR(ltemMuTau[0]);
	 muFR_  = muFR(ltemMuTau[0]);
	 tauPR_ = tauPR(ltemMuTau[1])-tauPRE(ltemMuTau[1]);
	 tauFR_ = tauFR(ltemMuTau[1]);
	 TTDoubleFake_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] )*muFR_*tauFR_;
	 TTSingleMuFake_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] )*muFR_*tauPR_;
	 TTSingleTauFake_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] )*muPR_*tauFR_;
	 TTtotalBckg_     = (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_);
	 if( EtaRegion==1 ) BarrelTTBckgSyshisto->Fill( iLQ, 7, TTtotalBckg_*w );
	 if( EtaRegion==2 ) EndcapTTBckgSyshisto->Fill( iLQ, 7, TTtotalBckg_*w );
	 if( EtaRegion==1 ) BarrelTTBckgSysW1histo->Fill( iLQ, 7, 1 );
	 if( EtaRegion==2 ) EndcapTTBckgSysW1histo->Fill( iLQ, 7, 1 );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_SThisto->Fill( 7, ST_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_SThisto->Fill( 7, ST_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUPThisto->Fill( 7, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUPThisto->Fill( 7, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUPThisto->Fill( 7, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUPThisto->Fill( 7, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUETAhisto->Fill( 7, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUETAhisto->Fill( 7, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUETAhisto->Fill( 7, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUETAhisto->Fill( 7, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUJETDRhisto->Fill( 7, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUJETDRhisto->Fill( 7, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUJETDRhisto->Fill( 7, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUJETDRhisto->Fill( 7, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_JETNhisto->Fill( 7, JetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_JETNhisto->Fill( 7, JetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_BJETNhisto->Fill( 7, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_BJETNhisto->Fill( 7, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_PUhisto->Fill( 7, pu_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_PUhisto->Fill( 7, pu_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_METhisto->Fill( 7, MET_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_METhisto->Fill( 7, MET_, TTtotalBckg_*w );

	 //
	 // MuFRU
	 muPR_  = muPR(ltemMuTau[0]);
	 muFR_  = muFR(ltemMuTau[0])+muFRE(ltemMuTau[0]);
	 tauPR_ = tauPR(ltemMuTau[1]);
	 tauFR_ = tauFR(ltemMuTau[1]);
	 TTDoubleFake_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] )*muFR_*tauFR_;
	 TTSingleMuFake_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] )*muFR_*tauPR_;
	 TTSingleTauFake_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] )*muPR_*tauFR_;
	 TTtotalBckg_     = (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_);
	 if( EtaRegion==1 ) BarrelTTBckgSyshisto->Fill( iLQ, 8, TTtotalBckg_*w );
	 if( EtaRegion==2 ) EndcapTTBckgSyshisto->Fill( iLQ, 8, TTtotalBckg_*w );
	 if( EtaRegion==1 ) BarrelTTBckgSysW1histo->Fill( iLQ, 8, 1 );
	 if( EtaRegion==2 ) EndcapTTBckgSysW1histo->Fill( iLQ, 8, 1 );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_SThisto->Fill( 8, ST_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_SThisto->Fill( 8, ST_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUPThisto->Fill( 8, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUPThisto->Fill( 8, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUPThisto->Fill( 8, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUPThisto->Fill( 8, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUETAhisto->Fill( 8, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUETAhisto->Fill( 8, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUETAhisto->Fill( 8, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUETAhisto->Fill( 8, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUJETDRhisto->Fill( 8, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUJETDRhisto->Fill( 8, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUJETDRhisto->Fill( 8, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUJETDRhisto->Fill( 8, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_JETNhisto->Fill( 8, JetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_JETNhisto->Fill( 8, JetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_BJETNhisto->Fill( 8, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_BJETNhisto->Fill( 8, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_PUhisto->Fill( 8, pu_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_PUhisto->Fill( 8, pu_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_METhisto->Fill( 8, MET_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_METhisto->Fill( 8, MET_, TTtotalBckg_*w );

	 //
	 // MuFRD
	 muPR_  = muPR(ltemMuTau[0]);
	 muFR_  = muFR(ltemMuTau[0])-muFRE(ltemMuTau[0]);
	 tauPR_ = tauPR(ltemMuTau[1]);
	 tauFR_ = tauFR(ltemMuTau[1]);
	 TTDoubleFake_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] )*muFR_*tauFR_;
	 TTSingleMuFake_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] )*muFR_*tauPR_;
	 TTSingleTauFake_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] )*muPR_*tauFR_;
	 TTtotalBckg_     = (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_);
	 if( EtaRegion==1 ) BarrelTTBckgSyshisto->Fill( iLQ, 9, TTtotalBckg_*w );
	 if( EtaRegion==2 ) EndcapTTBckgSyshisto->Fill( iLQ, 9, TTtotalBckg_*w );
	 if( EtaRegion==1 ) BarrelTTBckgSysW1histo->Fill( iLQ, 9, 1 );
	 if( EtaRegion==2 ) EndcapTTBckgSysW1histo->Fill( iLQ, 9, 1 );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_SThisto->Fill( 9, ST_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_SThisto->Fill( 9, ST_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUPThisto->Fill( 9, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUPThisto->Fill( 9, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUPThisto->Fill( 9, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUPThisto->Fill( 9, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUETAhisto->Fill( 9, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUETAhisto->Fill( 9, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUETAhisto->Fill( 9, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUETAhisto->Fill( 9, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUJETDRhisto->Fill( 9, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUJETDRhisto->Fill( 9, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUJETDRhisto->Fill( 9, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUJETDRhisto->Fill( 9, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_JETNhisto->Fill( 9, JetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_JETNhisto->Fill( 9, JetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_BJETNhisto->Fill( 9, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_BJETNhisto->Fill( 9, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_PUhisto->Fill( 9, pu_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_PUhisto->Fill( 9, pu_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_METhisto->Fill( 9, MET_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_METhisto->Fill( 9, MET_, TTtotalBckg_*w );

	 //
	 // MuPRU
	 muPR_  = muPR(ltemMuTau[0])+muPRE(ltemMuTau[0]);
	 muFR_  = muFR(ltemMuTau[0]);
	 tauPR_ = tauPR(ltemMuTau[1]);
	 tauFR_ = tauFR(ltemMuTau[1]);
	 TTDoubleFake_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] )*muFR_*tauFR_;
	 TTSingleMuFake_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] )*muFR_*tauPR_;
	 TTSingleTauFake_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] )*muPR_*tauFR_;
	 TTtotalBckg_     = (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_);
	 if( EtaRegion==1 ) BarrelTTBckgSyshisto->Fill( iLQ, 10, TTtotalBckg_*w );
	 if( EtaRegion==2 ) EndcapTTBckgSyshisto->Fill( iLQ, 10, TTtotalBckg_*w );
	 if( EtaRegion==1 ) BarrelTTBckgSysW1histo->Fill( iLQ, 10, 1 );
	 if( EtaRegion==2 ) EndcapTTBckgSysW1histo->Fill( iLQ, 10, 1 );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_SThisto->Fill( 10, ST_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_SThisto->Fill( 10, ST_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUPThisto->Fill( 10, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUPThisto->Fill( 10, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUPThisto->Fill( 10, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUPThisto->Fill( 10, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUETAhisto->Fill( 10, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUETAhisto->Fill( 10, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUETAhisto->Fill( 10, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUETAhisto->Fill( 10, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUJETDRhisto->Fill( 10, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUJETDRhisto->Fill( 10, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUJETDRhisto->Fill( 10, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUJETDRhisto->Fill( 10, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_JETNhisto->Fill( 10, JetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_JETNhisto->Fill( 10, JetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_BJETNhisto->Fill( 10, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_BJETNhisto->Fill( 10, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_PUhisto->Fill( 10, pu_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_PUhisto->Fill( 10, pu_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_METhisto->Fill( 10, MET_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_METhisto->Fill( 10, MET_, TTtotalBckg_*w );

	 //
	 // MuPRD
	 muPR_  = muPR(ltemMuTau[0])-muPRE(ltemMuTau[0]);
	 muFR_  = muFR(ltemMuTau[0]);
	 tauPR_ = tauPR(ltemMuTau[1]);
	 tauFR_ = tauFR(ltemMuTau[1]);
	 TTDoubleFake_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] )*muFR_*tauFR_;
	 TTSingleMuFake_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] )*muFR_*tauPR_;
	 TTSingleTauFake_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] )*muPR_*tauFR_;
	 TTtotalBckg_     = (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_);
	 if( EtaRegion==1 ) BarrelTTBckgSyshisto->Fill( iLQ, 11, TTtotalBckg_*w );
	 if( EtaRegion==2 ) EndcapTTBckgSyshisto->Fill( iLQ, 11, TTtotalBckg_*w );
	 if( EtaRegion==1 ) BarrelTTBckgSysW1histo->Fill( iLQ, 11, 1 );
	 if( EtaRegion==2 ) EndcapTTBckgSysW1histo->Fill( iLQ, 11, 1 );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_SThisto->Fill( 11, ST_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_SThisto->Fill( 11, ST_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUPThisto->Fill( 11, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUPThisto->Fill( 11, LTEMtauPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUPThisto->Fill( 11, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUPThisto->Fill( 11, LTEMmuPt_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUETAhisto->Fill( 11, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUETAhisto->Fill( 11, LTEMtauEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUETAhisto->Fill( 11, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUETAhisto->Fill( 11, LTEMmuEta_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_MUJETDRhisto->Fill( 11, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_MUJETDRhisto->Fill( 11, MuJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_TAUJETDRhisto->Fill( 11, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_TAUJETDRhisto->Fill( 11, TauJetDR_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_JETNhisto->Fill( 11, JetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_JETNhisto->Fill( 11, JetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_BJETNhisto->Fill( 11, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_BJETNhisto->Fill( 11, BJetN_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_PUhisto->Fill( 11, pu_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_PUhisto->Fill( 11, pu_, TTtotalBckg_*w );
         if( EtaRegion==1 && iLQ==1 ) BarrelEXP_METhisto->Fill( 11, MET_, TTtotalBckg_*w );
         if( EtaRegion==2 && iLQ==1 ) EndcapEXP_METhisto->Fill( 11, MET_, TTtotalBckg_*w );
	 //
	 //

	 // If probe fake rate is up, means event is less busy around the probe, so probe prompt rate also goes up! << CORRELATION!

	 //
	 // Obs
	 if( EtaRegion==1 && isMuTight_ && isTauTight_ ) BarrelTTBckgSysV2histo->Fill( iLQ, 1, w );
         if( EtaRegion==2 && isMuTight_ && isTauTight_ ) EndcapTTBckgSysV2histo->Fill( iLQ, 1, w );

	 //	
	 // Exp Mean Fakes
	 muPR_  = muPR(ltemMuTau[0]);
	 muFR_  = muFR(ltemMuTau[0]);
	 tauPR_ = tauPR(ltemMuTau[1]);
	 tauFR_ = tauFR(ltemMuTau[1]);
	 TTDoubleFake_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] )*muFR_*tauFR_;
	 TTSingleMuFake_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] )*muFR_*tauPR_;
	 TTSingleTauFake_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] )*muPR_*tauFR_;
	 TTtotalBckg_     = (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_);
	 if( EtaRegion==1 ) BarrelTTBckgSysV2histo->Fill( iLQ, 2, TTtotalBckg_*w );
	 if( EtaRegion==2 ) EndcapTTBckgSysV2histo->Fill( iLQ, 2, TTtotalBckg_*w );	 

	 //
	 // PP
	 if( EtaRegion==1 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ ) BarrelTTBckgSysV2histo->Fill( iLQ, 3, w );
         if( EtaRegion==2 && isRecoMuPrompt_ && isRecoTauPrompt_ && isMuTight_ && isTauTight_ ) EndcapTTBckgSysV2histo->Fill( iLQ, 3, w );

	 //
	 // TauFRU & TauPRU 
	 muPR_  = muPR(ltemMuTau[0]);
	 muFR_  = muFR(ltemMuTau[0]);
	 tauPR_ = tauPR(ltemMuTau[1])+tauPRE(ltemMuTau[1]);
	 tauFR_ = tauFR(ltemMuTau[1])+tauFRE(ltemMuTau[1]);
	 TTDoubleFake_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] )*muFR_*tauFR_;
	 TTSingleMuFake_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] )*muFR_*tauPR_;
	 TTSingleTauFake_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] )*muPR_*tauFR_;
	 TTtotalBckg_     = (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_);
	 if( EtaRegion==1 ) BarrelTTBckgSysV2histo->Fill( iLQ, 4, TTtotalBckg_*w );
	 if( EtaRegion==2 ) EndcapTTBckgSysV2histo->Fill( iLQ, 4, TTtotalBckg_*w );	 

	 //
	 // TauFRD & TauPRD
	 muPR_  = muPR(ltemMuTau[0]);
	 muFR_  = muFR(ltemMuTau[0]);
	 tauPR_ = tauPR(ltemMuTau[1])-tauPRE(ltemMuTau[1]);
	 tauFR_ = tauFR(ltemMuTau[1])-tauFRE(ltemMuTau[1]);
	 TTDoubleFake_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] )*muFR_*tauFR_;
	 TTSingleMuFake_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] )*muFR_*tauPR_;
	 TTSingleTauFake_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] )*muPR_*tauFR_;
	 TTtotalBckg_     = (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_);
	 if( EtaRegion==1 ) BarrelTTBckgSysV2histo->Fill( iLQ, 5, TTtotalBckg_*w );
	 if( EtaRegion==2 ) EndcapTTBckgSysV2histo->Fill( iLQ, 5, TTtotalBckg_*w );	 
	 //
	 // MuFRU & MuPRU 
	 muPR_  = muPR(ltemMuTau[0])+muPRE(ltemMuTau[0]);
	 muFR_  = muFR(ltemMuTau[0])+muPRE(ltemMuTau[0]);
	 tauPR_ = tauPR(ltemMuTau[1]);
	 tauFR_ = tauFR(ltemMuTau[1]);
	 TTDoubleFake_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] )*muFR_*tauFR_;
	 TTSingleMuFake_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] )*muFR_*tauPR_;
	 TTSingleTauFake_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] )*muPR_*tauFR_;
	 TTtotalBckg_     = (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_);
	 if( EtaRegion==1 ) BarrelTTBckgSysV2histo->Fill( iLQ, 6, TTtotalBckg_*w );
	 if( EtaRegion==2 ) EndcapTTBckgSysV2histo->Fill( iLQ, 6, TTtotalBckg_*w );	 

	 //
	 // MuFRD & MuPRD
	 muPR_  = muPR(ltemMuTau[0])-muPRE(ltemMuTau[0]);
	 muFR_  = muFR(ltemMuTau[0])-muPRE(ltemMuTau[0]);
	 tauPR_ = tauPR(ltemMuTau[1]);
	 tauFR_ = tauFR(ltemMuTau[1]);
	 TTDoubleFake_    = DoubleFakeWeight(    ltemMuTau[0], ltemMuTau[1] )*muFR_*tauFR_;
	 TTSingleMuFake_  = SingleFakeMuWeight(  ltemMuTau[0], ltemMuTau[1] )*muFR_*tauPR_;
	 TTSingleTauFake_ = SingleFakeTauWeight( ltemMuTau[0], ltemMuTau[1] )*muPR_*tauFR_;
	 TTtotalBckg_     = (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_);
	 if( EtaRegion==1 ) BarrelTTBckgSysV2histo->Fill( iLQ, 7, TTtotalBckg_*w );
	 if( EtaRegion==2 ) EndcapTTBckgSysV2histo->Fill( iLQ, 7, TTtotalBckg_*w );	 

       }//closes for( int iLQ=1; iLQ<10; iLQ++)
       

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
   BarrelExpFakeBreakdownhisto->Write();
   EndcapExpFakeBreakdownhisto->Write();
   BarrelExpFakeBreakdownW1histo->Write();
   EndcapExpFakeBreakdownW1histo->Write();
   //
   BackgroundDecompositionhisto->Write();
   //
   BarrelTTBckgSyshisto->Write();  
   EndcapTTBckgSyshisto->Write();
   BarrelTTBckgSysW1histo->Write();  
   EndcapTTBckgSysW1histo->Write();
   BarrelTTBckgSysV2histo->Write();
   EndcapTTBckgSysV2histo->Write();
   BarrelLTmatrixhisto->Write();
   EndcapLTmatrixhisto->Write();
   BarrelFPmatrixhisto->Write();
   EndcapFPmatrixhisto->Write();
   BarrelFPmatrixW1histo->Write();
   EndcapFPmatrixW1histo->Write();
   //
   BarrelLL_SThisto->Write();       BarrelLT_SThisto->Write();       BarrelTL_SThisto->Write();       BarrelTT_SThisto->Write();
   BarrelLL_TAUPThisto->Write();    BarrelLT_TAUPThisto->Write();    BarrelTL_TAUPThisto->Write();    BarrelTT_TAUPThisto->Write();
   BarrelLL_TAUETAhisto->Write();   BarrelLT_TAUETAhisto->Write();   BarrelTL_TAUETAhisto->Write();   BarrelTT_TAUETAhisto->Write();
   BarrelLL_MUPThisto->Write();     BarrelLT_MUPThisto->Write();     BarrelTL_MUPThisto->Write();     BarrelTT_MUPThisto->Write();
   BarrelLL_MUETAhisto->Write();    BarrelLT_MUETAhisto->Write();    BarrelTL_MUETAhisto->Write();    BarrelTT_MUETAhisto->Write();
   BarrelLL_MUJETDRhisto->Write();  BarrelLT_MUJETDRhisto->Write();  BarrelTL_MUJETDRhisto->Write();  BarrelTT_MUJETDRhisto->Write();
   BarrelLL_TAUJETDRhisto->Write(); BarrelLT_TAUJETDRhisto->Write(); BarrelTL_TAUJETDRhisto->Write(); BarrelTT_TAUJETDRhisto->Write();
   BarrelLL_JETNhisto->Write();     BarrelLT_JETNhisto->Write();     BarrelTL_JETNhisto->Write();     BarrelTT_JETNhisto->Write();
   BarrelLL_BJETNhisto->Write();    BarrelLT_BJETNhisto->Write();    BarrelTL_BJETNhisto->Write();    BarrelTT_BJETNhisto->Write();
   BarrelLL_PUhisto->Write();       BarrelLT_PUhisto->Write();       BarrelTL_PUhisto->Write();       BarrelTT_PUhisto->Write();
   BarrelLL_METhisto->Write();      BarrelLT_METhisto->Write();      BarrelTL_METhisto->Write();      BarrelTT_METhisto->Write();
   //
   EndcapLL_SThisto->Write();       EndcapLT_SThisto->Write();       EndcapTL_SThisto->Write();       EndcapTT_SThisto->Write();
   EndcapLL_TAUPThisto->Write();    EndcapLT_TAUPThisto->Write();    EndcapTL_TAUPThisto->Write();    EndcapTT_TAUPThisto->Write();
   EndcapLL_TAUETAhisto->Write();   EndcapLT_TAUETAhisto->Write();   EndcapTL_TAUETAhisto->Write();   EndcapTT_TAUETAhisto->Write();
   EndcapLL_MUPThisto->Write();     EndcapLT_MUPThisto->Write();     EndcapTL_MUPThisto->Write();     EndcapTT_MUPThisto->Write();
   EndcapLL_MUETAhisto->Write();    EndcapLT_MUETAhisto->Write();    EndcapTL_MUETAhisto->Write();    EndcapTT_MUETAhisto->Write();
   EndcapLL_MUJETDRhisto->Write();  EndcapLT_MUJETDRhisto->Write();  EndcapTL_MUJETDRhisto->Write();  EndcapTT_MUJETDRhisto->Write();
   EndcapLL_TAUJETDRhisto->Write(); EndcapLT_TAUJETDRhisto->Write(); EndcapTL_TAUJETDRhisto->Write(); EndcapTT_TAUJETDRhisto->Write();
   EndcapLL_JETNhisto->Write();     EndcapLT_JETNhisto->Write();     EndcapTL_JETNhisto->Write();     EndcapTT_JETNhisto->Write();
   EndcapLL_BJETNhisto->Write();    EndcapLT_BJETNhisto->Write();    EndcapTL_BJETNhisto->Write();    EndcapTT_BJETNhisto->Write();
   EndcapLL_PUhisto->Write();       EndcapLT_PUhisto->Write();       EndcapTL_PUhisto->Write();       EndcapTT_PUhisto->Write();
   EndcapLL_METhisto->Write();      EndcapLT_METhisto->Write();      EndcapTL_METhisto->Write();      EndcapTT_METhisto->Write();
   //
   BarrelEXP_SThisto->Write();
   EndcapEXP_SThisto->Write();
   BarrelEXP_TAUPThisto->Write();
   EndcapEXP_TAUPThisto->Write();
   BarrelEXP_TAUETAhisto->Write();
   EndcapEXP_TAUETAhisto->Write();
   BarrelEXP_MUPThisto->Write();
   EndcapEXP_MUPThisto->Write();
   BarrelEXP_MUETAhisto->Write();
   EndcapEXP_MUETAhisto->Write();
   BarrelEXP_MUJETDRhisto->Write();
   EndcapEXP_MUJETDRhisto->Write();
   BarrelEXP_TAUJETDRhisto->Write();
   EndcapEXP_TAUJETDRhisto->Write();
   BarrelEXP_JETNhisto->Write();
   EndcapEXP_JETNhisto->Write();
   BarrelEXP_BJETNhisto->Write();
   EndcapEXP_BJETNhisto->Write();
   BarrelEXP_PUhisto->Write();
   EndcapEXP_PUhisto->Write();
   BarrelEXP_METhisto->Write();
   EndcapEXP_METhisto->Write();

   LowestJetPtLLhisto->Write();
   LowestJetPtLThisto->Write();
   LowestJetPtTLhisto->Write();
   LowestJetPtTThisto->Write();
   LowestJetPtTTbarrelhisto->Write();
   LowestJetPtTTendcaphisto->Write();

   //////////
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
// // //
