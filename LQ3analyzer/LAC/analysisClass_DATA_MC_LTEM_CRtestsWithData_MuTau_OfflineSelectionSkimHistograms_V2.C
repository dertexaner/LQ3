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
   TH1D* muPRwSFhisto   = new TH1D("muPRwSFhisto",  "muPRwSFhisto",    10000,0,1);
   TH1D* muFRwSFhisto   = new TH1D("muFRwSFhisto",  "muFRwSFhisto",    10000,0,1);
   TH1D* tauPRwSFhisto  = new TH1D("tauPRwSFhisto", "tauPRwSFhisto",   10000,0,1);
   TH1D* tauFRwSFhisto  = new TH1D("tauFRwSFhisto", "tauFRwSFhisto",   10000,0,1);
   TH1D* muPREwSFhisto  = new TH1D("muPREwSFhisto", "muPREwSFhisto",   10000,0,1);
   TH1D* muFREwSFhisto  = new TH1D("muFREwSFhisto", "muFREwSFhisto",   10000,0,1);
   TH1D* tauPREwSFhisto = new TH1D("tauPREwSFhisto","tauPREwSFhisto",  10000,0,1);
   TH1D* tauFREwSFhisto = new TH1D("tauFREwSFhisto", "tauFREwSFhisto", 10000,0,1);
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
   TH1D* LTEMMuEtahisto            = new TH1D("LTEMMuEtahisto",           "LTEMMuEtahisto",           600,-3,3);
   TH1D* LTEMTauEtahisto           = new TH1D("LTEMTauEtahisto",          "LTEMTauEtahisto",          600,-3,3);
   TH1D* LTEMMuPhihisto            = new TH1D("LTEMMuPhihisto",           "LTEMMuPhihisto",           800,-4,4);
   TH1D* LTEMTauPhihisto           = new TH1D("LTEMTauPhihisto",          "LTEMTauPhihisto",          800,-4,4);
   TH1D* LTEMMuTauDeltaRhisto      = new TH1D("LTEMMuTauDeltaRhisto",     "LTEMMuTauDeltaRhisto",     300,-5,10);
   //TH1D* LTEMMuTauDeltaRhisto      = new TH1D("LTEMMuTauDeltaRhisto",     "LTEMMuTauDeltaRhisto",     200,-5,5);
   TH1D* LTEMMuTauDeltaPhihisto    = new TH1D("LTEMMuTauDeltaPhihisto",   "LTEMMuTauDeltaPhihisto",   200,-5,5);
   TH1D* LTEMMuTauMasshisto        = new TH1D("LTEMMuTauMasshisto",       "LTEMMuTauMasshisto",       5000,0,5000);
   //
   //
   TH1D* ExpTotalBckg_VertexNhisto        = new TH1D("ExpTotalBckg_VertexNhisto",        "ExpTotalBckg_VertexNhisto",           62,-0.5,61.5);
   TH1D* ExpTotalBckgPPonly_VertexNhisto  = new TH1D("ExpTotalBckgPPonly_VertexNhisto",  "ExpTotalBckgPPonly_VertexNhisto",     62,-0.5,61.5);
   TH1D* ExpTotalBckgTauFRU_VertexNhisto  = new TH1D("ExpTotalBckgTauFRU_VertexNhisto",  "ExpTotalBckgTauFRU_VertexNhisto",     62,-0.5,61.5);
   TH1D* ExpTotalBckgTauFRD_VertexNhisto  = new TH1D("ExpTotalBckgTauFRD_VertexNhisto",  "ExpTotalBckgTauFRD_VertexNhisto",     62,-0.5,61.5);
   TH1D* ExpTotalBckgTauPRU_VertexNhisto  = new TH1D("ExpTotalBckgTauPRU_VertexNhisto",  "ExpTotalBckgTauPRU_VertexNhisto",     62,-0.5,61.5);
   TH1D* ExpTotalBckgTauPRD_VertexNhisto  = new TH1D("ExpTotalBckgTauPRD_VertexNhisto",  "ExpTotalBckgTauPRD_VertexNhisto",     62,-0.5,61.5);
   TH1D* ExpTotalBckgMuFRU_VertexNhisto   = new TH1D("ExpTotalBckgMuFRU_VertexNhisto",   "ExpTotalBckgMuFRU_VertexNhisto",      62,-0.5,61.5);
   TH1D* ExpTotalBckgMuFRD_VertexNhisto   = new TH1D("ExpTotalBckgMuFRD_VertexNhisto",   "ExpTotalBckgMuFRD_VertexNhisto",      62,-0.5,61.5);
   TH1D* ExpTotalBckgMuPRU_VertexNhisto   = new TH1D("ExpTotalBckgMuPRU_VertexNhisto",   "ExpTotalBckgMuPRU_VertexNhisto",      62,-0.5,61.5);
   TH1D* ExpTotalBckgMuPRD_VertexNhisto   = new TH1D("ExpTotalBckgMuPRD_VertexNhisto",   "ExpTotalBckgMuPRD_VertexNhisto",      62,-0.5,61.5);
   //
   TH1D* ExpTotalBckg_TauJetDeltaRminhisto        = new TH1D("ExpTotalBckg_TauJetDeltaRminhisto",        "ExpTotalBckg_TauJetDeltaRminhisto",           200,-5,5);
   TH1D* ExpTotalBckgPPonly_TauJetDeltaRminhisto  = new TH1D("ExpTotalBckgPPonly_TauJetDeltaRminhisto",  "ExpTotalBckgPPonly_TauJetDeltaRminhisto",     200,-5,5);
   TH1D* ExpTotalBckgTauFRU_TauJetDeltaRminhisto  = new TH1D("ExpTotalBckgTauFRU_TauJetDeltaRminhisto",  "ExpTotalBckgTauFRU_TauJetDeltaRminhisto",     200,-5,5);
   TH1D* ExpTotalBckgTauFRD_TauJetDeltaRminhisto  = new TH1D("ExpTotalBckgTauFRD_TauJetDeltaRminhisto",  "ExpTotalBckgTauFRD_TauJetDeltaRminhisto",     200,-5,5);
   TH1D* ExpTotalBckgTauPRU_TauJetDeltaRminhisto  = new TH1D("ExpTotalBckgTauPRU_TauJetDeltaRminhisto",  "ExpTotalBckgTauPRU_TauJetDeltaRminhisto",     200,-5,5);
   TH1D* ExpTotalBckgTauPRD_TauJetDeltaRminhisto  = new TH1D("ExpTotalBckgTauPRD_TauJetDeltaRminhisto",  "ExpTotalBckgTauPRD_TauJetDeltaRminhisto",     200,-5,5);
   TH1D* ExpTotalBckgMuFRU_TauJetDeltaRminhisto   = new TH1D("ExpTotalBckgMuFRU_TauJetDeltaRminhisto",   "ExpTotalBckgMuFRU_TauJetDeltaRminhisto",      200,-5,5);
   TH1D* ExpTotalBckgMuFRD_TauJetDeltaRminhisto   = new TH1D("ExpTotalBckgMuFRD_TauJetDeltaRminhisto",   "ExpTotalBckgMuFRD_TauJetDeltaRminhisto",      200,-5,5);
   TH1D* ExpTotalBckgMuPRU_TauJetDeltaRminhisto   = new TH1D("ExpTotalBckgMuPRU_TauJetDeltaRminhisto",   "ExpTotalBckgMuPRU_TauJetDeltaRminhisto",      200,-5,5);
   TH1D* ExpTotalBckgMuPRD_TauJetDeltaRminhisto   = new TH1D("ExpTotalBckgMuPRD_TauJetDeltaRminhisto",   "ExpTotalBckgMuPRD_TauJetDeltaRminhisto",      200,-5,5);
   //
   TH1D* ExpTotalBckg_MuJetDeltaRminhisto        = new TH1D("ExpTotalBckg_MuJetDeltaRminhisto",        "ExpTotalBckg_MuJetDeltaRminhisto",           200,-5,5);
   TH1D* ExpTotalBckgPPonly_MuJetDeltaRminhisto  = new TH1D("ExpTotalBckgPPonly_MuJetDeltaRminhisto",  "ExpTotalBckgPPonly_MuJetDeltaRminhisto",     200,-5,5);
   TH1D* ExpTotalBckgTauFRU_MuJetDeltaRminhisto  = new TH1D("ExpTotalBckgTauFRU_MuJetDeltaRminhisto",  "ExpTotalBckgTauFRU_MuJetDeltaRminhisto",     200,-5,5);
   TH1D* ExpTotalBckgTauFRD_MuJetDeltaRminhisto  = new TH1D("ExpTotalBckgTauFRD_MuJetDeltaRminhisto",  "ExpTotalBckgTauFRD_MuJetDeltaRminhisto",     200,-5,5);
   TH1D* ExpTotalBckgTauPRU_MuJetDeltaRminhisto  = new TH1D("ExpTotalBckgTauPRU_MuJetDeltaRminhisto",  "ExpTotalBckgTauPRU_MuJetDeltaRminhisto",     200,-5,5);
   TH1D* ExpTotalBckgTauPRD_MuJetDeltaRminhisto  = new TH1D("ExpTotalBckgTauPRD_MuJetDeltaRminhisto",  "ExpTotalBckgTauPRD_MuJetDeltaRminhisto",     200,-5,5);
   TH1D* ExpTotalBckgMuFRU_MuJetDeltaRminhisto   = new TH1D("ExpTotalBckgMuFRU_MuJetDeltaRminhisto",   "ExpTotalBckgMuFRU_MuJetDeltaRminhisto",      200,-5,5);
   TH1D* ExpTotalBckgMuFRD_MuJetDeltaRminhisto   = new TH1D("ExpTotalBckgMuFRD_MuJetDeltaRminhisto",   "ExpTotalBckgMuFRD_MuJetDeltaRminhisto",      200,-5,5);
   TH1D* ExpTotalBckgMuPRU_MuJetDeltaRminhisto   = new TH1D("ExpTotalBckgMuPRU_MuJetDeltaRminhisto",   "ExpTotalBckgMuPRU_MuJetDeltaRminhisto",      200,-5,5);
   TH1D* ExpTotalBckgMuPRD_MuJetDeltaRminhisto   = new TH1D("ExpTotalBckgMuPRD_MuJetDeltaRminhisto",   "ExpTotalBckgMuPRD_MuJetDeltaRminhisto",      200,-5,5);
   //
   TH1D* ExpTotalBckg_MuPthisto           = new TH1D("ExpTotalBckg_MuPthisto",           "ExpTotalBckg_MuPthisto",           2000,0,2000);
   TH1D* ExpTotalBckgPPonly_MuPthisto     = new TH1D("ExpTotalBckgPPonly_MuPthisto",     "ExpTotalBckgPPonly_MuPthisto",     2000,0,2000);
   TH1D* ExpTotalBckgTauFRU_MuPthisto     = new TH1D("ExpTotalBckgTauFRU_MuPthisto",     "ExpTotalBckgTauFRU_MuPthisto",     2000,0,2000);
   TH1D* ExpTotalBckgTauFRD_MuPthisto     = new TH1D("ExpTotalBckgTauFRD_MuPthisto",     "ExpTotalBckgTauFRD_MuPthisto",     2000,0,2000);
   TH1D* ExpTotalBckgTauPRU_MuPthisto     = new TH1D("ExpTotalBckgTauPRU_MuPthisto",     "ExpTotalBckgTauPRU_MuPthisto",     2000,0,2000);
   TH1D* ExpTotalBckgTauPRD_MuPthisto     = new TH1D("ExpTotalBckgTauPRD_MuPthisto",     "ExpTotalBckgTauPRD_MuPthisto",     2000,0,2000);
   TH1D* ExpTotalBckgMuFRU_MuPthisto      = new TH1D("ExpTotalBckgMuFRU_MuPthisto",      "ExpTotalBckgMuFRU_MuPthisto",      2000,0,2000);
   TH1D* ExpTotalBckgMuFRD_MuPthisto      = new TH1D("ExpTotalBckgMuFRD_MuPthisto",      "ExpTotalBckgMuFRD_MuPthisto",      2000,0,2000);
   TH1D* ExpTotalBckgMuPRU_MuPthisto      = new TH1D("ExpTotalBckgMuPRU_MuPthisto",      "ExpTotalBckgMuPRU_MuPthisto",      2000,0,2000);
   TH1D* ExpTotalBckgMuPRD_MuPthisto      = new TH1D("ExpTotalBckgMuPRD_MuPthisto",      "ExpTotalBckgMuPRD_MuPthisto",      2000,0,2000);
   //
   TH1D* ExpTotalBckg_MuEtahisto           = new TH1D("ExpTotalBckg_MuEtahisto",          "ExpTotalBckg_MuEtahisto",          600,-3,3);
   TH1D* ExpTotalBckgPPonly_MuEtahisto     = new TH1D("ExpTotalBckgPPonly_MuEtahisto",    "ExpTotalBckgPPonly_MuEtahisto",    600,-3,3);
   TH1D* ExpTotalBckgTauFRU_MuEtahisto     = new TH1D("ExpTotalBckgTauFRU_MuEtahisto",    "ExpTotalBckgTauFRU_MuEtahisto",    600,-3,3);
   TH1D* ExpTotalBckgTauFRD_MuEtahisto     = new TH1D("ExpTotalBckgTauFRD_MuEtahisto",    "ExpTotalBckgTauFRD_MuEtahisto",    600,-3,3);
   TH1D* ExpTotalBckgTauPRU_MuEtahisto     = new TH1D("ExpTotalBckgTauPRU_MuEtahisto",    "ExpTotalBckgTauPRU_MuEtahisto",    600,-3,3);
   TH1D* ExpTotalBckgTauPRD_MuEtahisto     = new TH1D("ExpTotalBckgTauPRD_MuEtahisto",    "ExpTotalBckgTauPRD_MuEtahisto",    600,-3,3);
   TH1D* ExpTotalBckgMuFRU_MuEtahisto      = new TH1D("ExpTotalBckgMuFRU_MuEtahisto",     "ExpTotalBckgMuFRU_MuEtahisto",     600,-3,3);
   TH1D* ExpTotalBckgMuFRD_MuEtahisto      = new TH1D("ExpTotalBckgMuFRD_MuEtahisto",     "ExpTotalBckgMuFRD_MuEtahisto",     600,-3,3);
   TH1D* ExpTotalBckgMuPRU_MuEtahisto      = new TH1D("ExpTotalBckgMuPRU_MuEtahisto",     "ExpTotalBckgMuPRU_MuEtahisto",     600,-3,3);
   TH1D* ExpTotalBckgMuPRD_MuEtahisto      = new TH1D("ExpTotalBckgMuPRD_MuEtahisto",     "ExpTotalBckgMuPRD_MuEtahisto",     600,-3,3);
   //
   TH1D* ExpTotalBckg_TauPthisto           = new TH1D("ExpTotalBckg_TauPthisto",           "ExpTotalBckg_TauPthisto",           2000,0,2000);
   TH1D* ExpTotalBckgPPonly_TauPthisto     = new TH1D("ExpTotalBckgPPonly_TauPthisto",     "ExpTotalBckgPPonly_TauPthisto",     2000,0,2000);
   TH1D* ExpTotalBckgTauFRU_TauPthisto     = new TH1D("ExpTotalBckgTauFRU_TauPthisto",     "ExpTotalBckgTauFRU_TauPthisto",     2000,0,2000);
   TH1D* ExpTotalBckgTauFRD_TauPthisto     = new TH1D("ExpTotalBckgTauFRD_TauPthisto",     "ExpTotalBckgTauFRD_TauPthisto",     2000,0,2000);
   TH1D* ExpTotalBckgTauPRU_TauPthisto     = new TH1D("ExpTotalBckgTauPRU_TauPthisto",     "ExpTotalBckgTauPRU_TauPthisto",     2000,0,2000);
   TH1D* ExpTotalBckgTauPRD_TauPthisto     = new TH1D("ExpTotalBckgTauPRD_TauPthisto",     "ExpTotalBckgTauPRD_TauPthisto",     2000,0,2000);
   TH1D* ExpTotalBckgMuFRU_TauPthisto      = new TH1D("ExpTotalBckgMuFRU_TauPthisto",      "ExpTotalBckgMuFRU_TauPthisto",      2000,0,2000);
   TH1D* ExpTotalBckgMuFRD_TauPthisto      = new TH1D("ExpTotalBckgMuFRD_TauPthisto",      "ExpTotalBckgMuFRD_TauPthisto",      2000,0,2000);
   TH1D* ExpTotalBckgMuPRU_TauPthisto      = new TH1D("ExpTotalBckgMuPRU_TauPthisto",      "ExpTotalBckgMuPRU_TauPthisto",      2000,0,2000);
   TH1D* ExpTotalBckgMuPRD_TauPthisto      = new TH1D("ExpTotalBckgMuPRD_TauPthisto",      "ExpTotalBckgMuPRD_TauPthisto",      2000,0,2000);
   //
   TH1D* ExpTotalBckg_TauEtahisto           = new TH1D("ExpTotalBckg_TauEtahisto",          "ExpTotalBckg_TauEtahisto",          600,-3,3);
   TH1D* ExpTotalBckgPPonly_TauEtahisto     = new TH1D("ExpTotalBckgPPonly_TauEtahisto",    "ExpTotalBckgPPonly_TauEtahisto",    600,-3,3);
   TH1D* ExpTotalBckgTauFRU_TauEtahisto     = new TH1D("ExpTotalBckgTauFRU_TauEtahisto",    "ExpTotalBckgTauFRU_TauEtahisto",    600,-3,3);
   TH1D* ExpTotalBckgTauFRD_TauEtahisto     = new TH1D("ExpTotalBckgTauFRD_TauEtahisto",    "ExpTotalBckgTauFRD_TauEtahisto",    600,-3,3);
   TH1D* ExpTotalBckgTauPRU_TauEtahisto     = new TH1D("ExpTotalBckgTauPRU_TauEtahisto",    "ExpTotalBckgTauPRU_TauEtahisto",    600,-3,3);
   TH1D* ExpTotalBckgTauPRD_TauEtahisto     = new TH1D("ExpTotalBckgTauPRD_TauEtahisto",    "ExpTotalBckgTauPRD_TauEtahisto",    600,-3,3);
   TH1D* ExpTotalBckgMuFRU_TauEtahisto      = new TH1D("ExpTotalBckgMuFRU_TauEtahisto",     "ExpTotalBckgMuFRU_TauEtahisto",     600,-3,3);
   TH1D* ExpTotalBckgMuFRD_TauEtahisto      = new TH1D("ExpTotalBckgMuFRD_TauEtahisto",     "ExpTotalBckgMuFRD_TauEtahisto",     600,-3,3);
   TH1D* ExpTotalBckgMuPRU_TauEtahisto      = new TH1D("ExpTotalBckgMuPRU_TauEtahisto",     "ExpTotalBckgMuPRU_TauEtahisto",     600,-3,3);
   TH1D* ExpTotalBckgMuPRD_TauEtahisto      = new TH1D("ExpTotalBckgMuPRD_TauEtahisto",     "ExpTotalBckgMuPRD_TauEtahisto",     600,-3,3);
   //
   TH1D* ExpTotalBckg_SThisto               = new TH1D("ExpTotalBckg_SThisto",              "ExpTotalBckg_SThisto",              5000,0,5000);
   TH1D* ExpTotalBckgPPonly_SThisto         = new TH1D("ExpTotalBckgPPonly_SThisto",        "ExpTotalBckgPPonly_SThisto",        5000,0,5000);
   TH1D* ExpTotalBckgTauFRU_SThisto         = new TH1D("ExpTotalBckgTauFRU_SThisto",        "ExpTotalBckgTauFRU_SThisto",        5000,0,5000);
   TH1D* ExpTotalBckgTauFRD_SThisto         = new TH1D("ExpTotalBckgTauFRD_SThisto",        "ExpTotalBckgTauFRD_SThisto",        5000,0,5000);
   TH1D* ExpTotalBckgTauPRU_SThisto         = new TH1D("ExpTotalBckgTauPRU_SThisto",        "ExpTotalBckgTauPRU_SThisto",        5000,0,5000);
   TH1D* ExpTotalBckgTauPRD_SThisto         = new TH1D("ExpTotalBckgTauPRD_SThisto",        "ExpTotalBckgTauPRD_SThisto",        5000,0,5000);
   TH1D* ExpTotalBckgMuFRU_SThisto          = new TH1D("ExpTotalBckgMuFRU_SThisto",         "ExpTotalBckgMuFRU_SThisto",         5000,0,5000);
   TH1D* ExpTotalBckgMuFRD_SThisto          = new TH1D("ExpTotalBckgMuFRD_SThisto",         "ExpTotalBckgMuFRD_SThisto",         5000,0,5000);
   TH1D* ExpTotalBckgMuPRU_SThisto          = new TH1D("ExpTotalBckgMuPRU_SThisto",         "ExpTotalBckgMuPRU_SThisto",         5000,0,5000);
   TH1D* ExpTotalBckgMuPRD_SThisto          = new TH1D("ExpTotalBckgMuPRD_SThisto",         "ExpTotalBckgMuPRD_SThisto",         5000,0,5000);
   //
   TH1D* ExpTotalBckg_JetNhisto             = new TH1D("ExpTotalBckg_JetNhisto",            "ExpTotalBckg_JetNhisto",            16,-0.5,15.5);
   TH1D* ExpTotalBckgPPonly_JetNhisto       = new TH1D("ExpTotalBckgPPonly_JetNhisto",      "ExpTotalBckgPPonly_JetNhisto",      16,-0.5,15.5);
   TH1D* ExpTotalBckgTauFRU_JetNhisto       = new TH1D("ExpTotalBckgTauFRU_JetNhisto",      "ExpTotalBckgTauFRU_JetNhisto",      16,-0.5,15.5);
   TH1D* ExpTotalBckgTauFRD_JetNhisto       = new TH1D("ExpTotalBckgTauFRD_JetNhisto",      "ExpTotalBckgTauFRD_JetNhisto",      16,-0.5,15.5);
   TH1D* ExpTotalBckgTauPRU_JetNhisto       = new TH1D("ExpTotalBckgTauPRU_JetNhisto",      "ExpTotalBckgTauPRU_JetNhisto",      16,-0.5,15.5);
   TH1D* ExpTotalBckgTauPRD_JetNhisto       = new TH1D("ExpTotalBckgTauPRD_JetNhisto",      "ExpTotalBckgTauPRD_JetNhisto",      16,-0.5,15.5);
   TH1D* ExpTotalBckgMuFRU_JetNhisto        = new TH1D("ExpTotalBckgMuFRU_JetNhisto",       "ExpTotalBckgMuFRU_JetNhisto",       16,-0.5,15.5);
   TH1D* ExpTotalBckgMuFRD_JetNhisto        = new TH1D("ExpTotalBckgMuFRD_JetNhisto",       "ExpTotalBckgMuFRD_JetNhisto",       16,-0.5,15.5);
   TH1D* ExpTotalBckgMuPRU_JetNhisto        = new TH1D("ExpTotalBckgMuPRU_JetNhisto",       "ExpTotalBckgMuPRU_JetNhisto",       16,-0.5,15.5);
   TH1D* ExpTotalBckgMuPRD_JetNhisto        = new TH1D("ExpTotalBckgMuPRD_JetNhisto",       "ExpTotalBckgMuPRD_JetNhisto",       16,-0.5,15.5);
   //
   //Two seach regions: Tau Barrel, Tau Endcap
   TH1D* Search1Binhisto                      = new TH1D("Search1Binhisto",                         "Search1Binhisto",                         5,0,5);
   TH1D* ExpTotalBckg_Search1Binhisto         = new TH1D("ExpTotalBckg_Search1Binhisto",            "ExpTotalBckg_Search1Binhisto",            5,0,5);
   TH1D* ExpTotalBckgPPonly_Search1Binhisto   = new TH1D("ExpTotalBckgPPonly_Search1Binhisto",      "ExpTotalBckgPPonly_Search1Binhisto",      5,0,5);
   TH1D* ExpTotalBckgTauFRU_Search1Binhisto   = new TH1D("ExpTotalBckgTauFRU_Search1Binhisto",      "ExpTotalBckgTauFRU_Search1Binhisto",      5,0,5);
   TH1D* ExpTotalBckgTauFRD_Search1Binhisto   = new TH1D("ExpTotalBckgTauFRD_Search1Binhisto",      "ExpTotalBckgTauFRD_Search1Binhisto",      5,0,5);
   TH1D* ExpTotalBckgTauPRU_Search1Binhisto   = new TH1D("ExpTotalBckgTauPRU_Search1Binhisto",      "ExpTotalBckgTauPRU_Search1Binhisto",      5,0,5);
   TH1D* ExpTotalBckgTauPRD_Search1Binhisto   = new TH1D("ExpTotalBckgTauPRD_Search1Binhisto",      "ExpTotalBckgTauPRD_Search1Binhisto",      5,0,5);
   TH1D* ExpTotalBckgMuFRU_Search1Binhisto    = new TH1D("ExpTotalBckgMuFRU_Search1Binhisto",       "ExpTotalBckgMuFRU_Search1Binhisto",       5,0,5);
   TH1D* ExpTotalBckgMuFRD_Search1Binhisto    = new TH1D("ExpTotalBckgMuFRD_Search1Binhisto",       "ExpTotalBckgMuFRD_Search1Binhisto",       5,0,5);
   TH1D* ExpTotalBckgMuPRU_Search1Binhisto    = new TH1D("ExpTotalBckgMuPRU_Search1Binhisto",       "ExpTotalBckgMuPRU_Search1Binhisto",       5,0,5);
   TH1D* ExpTotalBckgMuPRD_Search1Binhisto    = new TH1D("ExpTotalBckgMuPRD_Search1Binhisto",       "ExpTotalBckgMuPRD_Search1Binhisto",       5,0,5);
   //
   //Four seach regions: Tau Barrel, Tau Endcap x 0 bjets, 1>bjets
   TH1D* Search2Binhisto                      = new TH1D("Search2Binhisto",                         "Search2Binhisto",                         10,0,10);
   TH1D* ExpTotalBckg_Search2Binhisto         = new TH1D("ExpTotalBckg_Search2Binhisto",            "ExpTotalBckg_Search2Binhisto",            10,0,10);
   TH1D* ExpTotalBckgPPonly_Search2Binhisto   = new TH1D("ExpTotalBckgPPonly_Search2Binhisto",      "ExpTotalBckgPPonly_Search2Binhisto",      10,0,10);
   TH1D* ExpTotalBckgTauFRU_Search2Binhisto   = new TH1D("ExpTotalBckgTauFRU_Search2Binhisto",      "ExpTotalBckgTauFRU_Search2Binhisto",      10,0,10);
   TH1D* ExpTotalBckgTauFRD_Search2Binhisto   = new TH1D("ExpTotalBckgTauFRD_Search2Binhisto",      "ExpTotalBckgTauFRD_Search2Binhisto",      10,0,10);
   TH1D* ExpTotalBckgTauPRU_Search2Binhisto   = new TH1D("ExpTotalBckgTauPRU_Search2Binhisto",      "ExpTotalBckgTauPRU_Search2Binhisto",      10,0,10);
   TH1D* ExpTotalBckgTauPRD_Search2Binhisto   = new TH1D("ExpTotalBckgTauPRD_Search2Binhisto",      "ExpTotalBckgTauPRD_Search2Binhisto",      10,0,10);
   TH1D* ExpTotalBckgMuFRU_Search2Binhisto    = new TH1D("ExpTotalBckgMuFRU_Search2Binhisto",       "ExpTotalBckgMuFRU_Search2Binhisto",       10,0,10);
   TH1D* ExpTotalBckgMuFRD_Search2Binhisto    = new TH1D("ExpTotalBckgMuFRD_Search2Binhisto",       "ExpTotalBckgMuFRD_Search2Binhisto",       10,0,10);
   TH1D* ExpTotalBckgMuPRU_Search2Binhisto    = new TH1D("ExpTotalBckgMuPRU_Search2Binhisto",       "ExpTotalBckgMuPRU_Search2Binhisto",       10,0,10);
   TH1D* ExpTotalBckgMuPRD_Search2Binhisto    = new TH1D("ExpTotalBckgMuPRD_Search2Binhisto",       "ExpTotalBckgMuPRD_Search2Binhisto",       10,0,10);
   //
   //
   //TH1D* ExpTotalBckg_MuTauDeltaRhisto      = new TH1D("ExpTotalBckg_MuTauDeltaRhisto",     "ExpTotalBckg_MuTauDeltaRhisto",     300,-5,10);
   //
   TH1D* ExpSingleTauFake_MuTauDeltaRhisto      = new TH1D("ExpSingleTauFake_MuTauDeltaRhisto",     "ExpSingleTauFake_MuTauDeltaRhisto",     300,-5,10);
   TH1D* ExpSingleTauFake_TauJetDeltaRminhisto  = new TH1D("ExpSingleTauFake_TauJetDeltaRminhisto", "ExpSingleTauFake_TauJetDeltaRminhisto", 200,-5,5);
   TH1D* ExpSingleTauFake_TauPthisto            = new TH1D("ExpSingleTauFake_TauPthisto",           "ExpSingleTauFake_TauPthisto",           2000,0,2000);
   TH1D* ExpSingleTauFake_TauEtahisto           = new TH1D("ExpSingleTauFake_TauEtahisto",          "ExpSingleTauFake_TauEtahisto",          600,-3,3);
   TH1D* ExpSingleTauFake_TauPhihisto           = new TH1D("ExpSingleTauFake_TauPhihisto",          "ExpSingleTauFake_TauPhihisto",          800,-4,4);
   TH1D* ExpSingleTauFake_SThisto               = new TH1D("ExpSingleTauFake_SThisto",              "ExpSingleTauFake_SThisto",              5000,0,5000);
   TH1D* ExpSingleMuFake_SThisto                = new TH1D("ExpSingleMuFake_SThisto",               "ExpSingleMuFake_SThisto",               5000,0,5000);
   TH1D* ExpDoubleFake_SThisto                  = new TH1D("ExpDoubleFake_SThisto",                 "ExpDoubleFake_SThisto",                 5000,0,5000);
   TH1D* ExpSingleTauFake_JetNhisto             = new TH1D("ExpSingleTauFake_JetNhisto",            "ExpSingleTauFake_JetNhisto",            16,-0.5,15.5);
   //
   // For 2D optimization
   TH2D* LTEM2DMuPtTauPthisto = new TH2D("LTEM2DMuPtTauPthisto","LTEM2DMuPtTauPthisto", 120,0,600,120,0,600);
   TH2D* LTEM2DTauPtSThisto   = new TH2D("LTEM2DTauPtSThisto","LTEM2DTauPtSThisto", 120,0,600,500,0,5000);
   TH2D* LTEM2DTauPtNLepjethisto = new TH2D("LTEM2DTauPtNLepjethisto","LTEM2DTauPtNLepjethisto", 120,0,600,21,-0.5,20.5);
   TH2D* LTEM2DMuPtNLepjethisto  = new TH2D("LTEM2DMuPtNLepjethisto","LTEM2DMuPtNLepjethisto", 120,0,600,21,-0.5,20.5);
   TH2D* LTEM2DMuPtSThisto    = new TH2D("LTEM2DMuPtSThisto","LTEM2DMuPtSThisto", 120,0,600,500,0,5000);
   TH2D* LTEM2DNLepjetSThisto = new TH2D("LTEM2DNLepjetSThisto","LTEM2DNLepjetSThisto", 21,-0.5,20.5,500,0,5000);
   TH2D* LTEM2DNJetSThisto    = new TH2D("LTEM2DNJetSThisto","LTEM2DNJetSThisto", 21,-0.5,20.5,500,0,5000);
   // For 3D optimization
   TH3D* LTEM3DTauPtSTJetNhisto    = new TH3D("LTEM3DTauPtSTJetNhisto",   "LTEM3DTauPtSTJetNhisto",    120,0,600, 500,0,5000, 11,-0.5,10.5);
   TH3D* LTEM3DTauPtSTLepjetNhisto = new TH3D("LTEM3DTauPtSTLepjetNhisto","LTEM3DTauPtSTLepjetNhisto", 120,0,600, 500,0,5000, 16,-0.5,15.5);
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

     ApplyLepJetMetSystematics(0);//no systematics applied

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
     if( MuonPt->at(triggerMuon)> 45                                ) usedTrigger_ = SingleMu40_passTrigger();
     if( MuonPt->at(triggerMuon)<=45 &&  muRTightCheck(triggerMuon) ) usedTrigger_ = SingleMu_passTrigger();
     if( MuonPt->at(triggerMuon)<=45 && !muRTightCheck(triggerMuon) ) usedTrigger_ = HLT_MuPT_eta2p1_passTrigger();// prescale 167.2
     //
     double TriggerEfficiencyWeights_;
     if( MuonPt->at(triggerMuon)> 45                                ) TriggerEfficiencyWeights_ = Mu40e2p1_ScaleFactor( MuonPt->at(triggerMuon), MuonEta->at(triggerMuon) );
     if( MuonPt->at(triggerMuon)<=45 &&  muRTightCheck(triggerMuon) ) TriggerEfficiencyWeights_ = IsoMu24e2p1_Eff(      MuonPt->at(triggerMuon), MuonEta->at(triggerMuon) );     
     if( MuonPt->at(triggerMuon)<=45 && !muRTightCheck(triggerMuon) ) TriggerEfficiencyWeights_ = Mu24e2p1_ScaleFactor( MuonPt->at(triggerMuon), MuonEta->at(triggerMuon) );
     //
     AppliedTrigEffWeightshisto->Fill( TriggerEfficiencyWeights_ );
     // ---- total = pileup x trigger
     double w = 0;
     if( !isData  ){
       w=safePileupWeights_*TriggerEfficiencyWeights_; 
     }
     if( isData  ) w=1;
     if( isData && MuonPt->at(triggerMuon)<=45 && !muRTightCheck(triggerMuon) ){
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
     if( MuonPt->at(triggerMuon)> 45                                ) passTrigger_ = SingleMu40_passTrigger();//<<<<<<<<< this is Mu40 trigger
     if( MuonPt->at(triggerMuon)<=45 &&  muRTightCheck(triggerMuon) ) passTrigger_ = SingleMu_passTrigger();  //<<<<<<<<< this is IsoMu24 trigger
     if( MuonPt->at(triggerMuon)<=45 && !muRTightCheck(triggerMuon) ) HLT_MuPT_eta2p1_passTrigger();          //<<<<<<<<< this is Mu24 trigger  (no isolation)
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
     if( MuonPt->at(triggerMuon)> 45                                && RecoHLTdeltaRmin_SingleMu40Trigger(triggerMuon)<0.15 ) passAllMuHLTmatching_=1;
     if( MuonPt->at(triggerMuon)<=45 &&  muRTightCheck(triggerMuon) && RecoHLTdeltaRmin_SingleMuTrigger(triggerMuon)<0.15   ) passAllMuHLTmatching_=1;
     if( MuonPt->at(triggerMuon)<=45 && !muRTightCheck(triggerMuon) && RecoHLTdeltaRmin_SingleMu24Trigger(triggerMuon)<0.15 ) passAllMuHLTmatching_=1;
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
     if( ltemMuTau.size()==2 && //MuonPt->at(ltemMuTau[0])>45 &&
         pZeta>-10 &&
	 //fabs(HPSTauEta->at(ltemMuTau[1]))<1.5 && //barrel Taus only
	 //fabs(HPSTauEta->at(ltemMuTau[1]))>=1.5 && //endcap Taus only
	 HPSTauPt->at(ltemMuTau[1])>35 && //TauPt>35
	 ST()>150  && JetCounter()<=1 && NLepJet<=3 
	 //muJetDeltaRmin(ltemMuTau[0])>0.5 && tauJetDeltaRmin(ltemMuTau[1])>0.7 ) OfflineCuts_=1; //Not needed, since deltaR cuts are applied in ObjDef in LAC/ADDON1_LAC.C !
	 ) OfflineCuts_=1;
     fillVariableWithValue("PassOfflineCuts", OfflineCuts_ );// returns 0, 1 
     
     
     evaluateCuts();
     //---------------------------------------------------------------------------------
     if( passedCut("PassJSON") && passedCut("PassTrig") && passedCut("PassEventFilter") && passedCut("PassOfflineDilepton") &&
	 passedCut("PassAllMuHLTmatching") && passedCut("PassOfflineCuts") ){ 

       double nLepjet= (MuCounter()+ElCounter()+TauCounter()+JetCounter());
       double nJet   = JetCounter();
       double st     = ST();

       bool isRecoMuPrompt_  = false;
       bool isRecoTauPrompt_ = false;
       //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       // Check whether LTEM mu & tau are Fake or Prompt!!
       if( isRecoMuPrompt(ltemMuTau[0])  ) isRecoMuPrompt_=true;
       if( isRecoTauPrompt(ltemMuTau[1]) ) isRecoTauPrompt_=true;
       /*
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
       //
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
       // Prompt and Fake Rate WITHOUT Scale Factor Histograms
       muPRhisto  ->Fill( muPR(ltemMuTau[0])   );
       muFRhisto  ->Fill( muFR(ltemMuTau[0])   );
       muPREhisto ->Fill( muPRE(ltemMuTau[0])  );
       muFREhisto ->Fill( muFRE(ltemMuTau[0])  );
       tauPRhisto ->Fill( tauPR(ltemMuTau[1])  );
       tauFRhisto ->Fill( tauFR(ltemMuTau[1])  );
       tauPREhisto->Fill( tauPRE(ltemMuTau[1]) );
       tauFREhisto->Fill( tauFRE(ltemMuTau[1]) );
       //-----------------------------------
       InitMuonSFs(ltemMuTau[0]);
       InitTauSFs();
       if( !isData  ){ ResetAllSFs(); } // UNDO ALL SF set up above for MC
       //-----------------------------------
       // Prompt and Fake Rate WITH Scale Factor Histograms
       muPRwSFhisto  ->Fill( muPR(ltemMuTau[0])   );
       muFRwSFhisto  ->Fill( muFR(ltemMuTau[0])   );
       muPREwSFhisto ->Fill( muPRE(ltemMuTau[0])  );
       muFREwSFhisto ->Fill( muFRE(ltemMuTau[0])  );
       tauPRwSFhisto ->Fill( tauPR(ltemMuTau[1])  );
       tauFRwSFhisto ->Fill( tauFR(ltemMuTau[1])  );
       tauPREwSFhisto->Fill( tauPRE(ltemMuTau[1]) );
       tauFREwSFhisto->Fill( tauFRE(ltemMuTau[1]) );
       //-----------------------------------
       double LTEM_ScaleFactor_=1; //disabled !!! - not sure if this helps anyway for MC..
       ////if( !isData  ){//for MC: apply SF dependent event weights to TT box based on Gen info, set SFs back to one
       ////LTEM_ScaleFactor_=LTEM_ScaleFactor( ltemMuTau[0], isRecoMuPrompt_, ltemMuTau[1], isRecoTauPrompt_ );
       ////ResetAllSFs();
       ////}
       //-----------------------------------
       //
       //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

       // LTEM Mu Vector, and LTEM Tau Vector
       TLorentzVector LTEMMuVector, LTEMTauVector;
       LTEMMuVector.SetPtEtaPhiM(  MuonPt->at(ltemMuTau[0]),    MuonEta->at(ltemMuTau[0]),   MuonPhi->at(ltemMuTau[0]),   0 );
       LTEMTauVector.SetPtEtaPhiM( HPSTauPt->at(ltemMuTau[1]),  HPSTauEta->at(ltemMuTau[1]), HPSTauPhi->at(ltemMuTau[1]), 0 );


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

       //Creating SearchEventType variables:
       double Search1EventType=0;
       double Search2EventType=0;
       int NoOfBjets=0;
       if( fabs(HPSTauEta->at(ltemMuTau[1]))< 1.5 ) Search1EventType=1;//barrel tau
       if( fabs(HPSTauEta->at(ltemMuTau[1]))>=1.5 ) Search1EventType=3;//endcap tau
       for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
	 if( !jetRisoCheck(iJetR)    )continue;
	 if( !btag_jetRCheck(iJetR)  )continue;
	 NoOfBjets++;
       }	 
       if( fabs(HPSTauEta->at(ltemMuTau[1]))< 1.5 && NoOfBjets==0 ) Search2EventType=1;//barrel tau, 0  bjets
       if( fabs(HPSTauEta->at(ltemMuTau[1]))< 1.5 && NoOfBjets>0  ) Search2EventType=3;//barrel tau, 1> bjets
       if( fabs(HPSTauEta->at(ltemMuTau[1]))>=1.5 && NoOfBjets==0 ) Search2EventType=5;//endcap tau, 0  bjets
       if( fabs(HPSTauEta->at(ltemMuTau[1]))>=1.5 && NoOfBjets>0  ) Search2EventType=7;//endcap tau, 1> bjets


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
       // Need various distributions of Single Tau Fakes here.. Weight: TTSingleTauFake_*w
       ExpTotalBckg_Search1Binhisto->Fill( Search1EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckg_Search2Binhisto->Fill( Search2EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckg_SThisto->Fill(              st,                                 (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckg_JetNhisto->Fill(            nJet,                               (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckg_TauJetDeltaRminhisto->Fill( tauJetDeltaRmin(ltemMuTau[1]),      (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckg_MuJetDeltaRminhisto->Fill(  muJetDeltaRmin(ltemMuTau[0]),       (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckg_TauPthisto->Fill(           HPSTauPt->at(ltemMuTau[1]),         (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckg_TauEtahisto->Fill(          HPSTauEta->at(ltemMuTau[1]),        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckg_MuPthisto->Fill(            MuonPt->at(ltemMuTau[0]),           (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckg_MuEtahisto->Fill(           MuonEta->at(ltemMuTau[0]),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckg_VertexNhisto->Fill(         (double)(VertexX->size()),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       if( !isData && isMuTight_ && isTauTight_ && isRecoMuPrompt_ && isRecoTauPrompt_ ){//add PP contribution to TT box estimation (for MC obviously)
	 ExpTotalBckgPPonly_Search1Binhisto->Fill(      Search1EventType,                   w*LTEM_ScaleFactor_ );
	 ExpTotalBckgPPonly_Search2Binhisto->Fill(      Search2EventType,                   w*LTEM_ScaleFactor_ );
	 ExpTotalBckgPPonly_SThisto->Fill(              st,                                 w*LTEM_ScaleFactor_ );
	 ExpTotalBckgPPonly_JetNhisto->Fill(            nJet,                               w*LTEM_ScaleFactor_ );
	 ExpTotalBckgPPonly_TauJetDeltaRminhisto->Fill( tauJetDeltaRmin(ltemMuTau[1]),      w*LTEM_ScaleFactor_ );
	 ExpTotalBckgPPonly_MuJetDeltaRminhisto->Fill(  muJetDeltaRmin(ltemMuTau[0]),       w*LTEM_ScaleFactor_ );
	 ExpTotalBckgPPonly_TauPthisto->Fill(           HPSTauPt->at(ltemMuTau[1]),         w*LTEM_ScaleFactor_ );
	 ExpTotalBckgPPonly_TauEtahisto->Fill(          HPSTauEta->at(ltemMuTau[1]),        w*LTEM_ScaleFactor_ );
	 ExpTotalBckgPPonly_MuPthisto->Fill(            MuonPt->at(ltemMuTau[0]),           w*LTEM_ScaleFactor_ );
	 ExpTotalBckgPPonly_MuEtahisto->Fill(           MuonEta->at(ltemMuTau[0]),          w*LTEM_ScaleFactor_ );
	 ExpTotalBckgPPonly_VertexNhisto->Fill(         (double)(VertexX->size()),          w*LTEM_ScaleFactor_ );
       }
       //
       //
       //mini test
       ExpSingleMuFake_SThisto->Fill(               st,                               TTSingleMuFake_*w  );
       ExpDoubleFake_SThisto->Fill(                 st,                               TTDoubleFake_*w    );
       ExpSingleTauFake_SThisto->Fill(              st,                               TTSingleTauFake_*w );
       //
       ExpSingleTauFake_JetNhisto->Fill(            nJet,                       TTSingleTauFake_*w );
       ExpSingleTauFake_MuTauDeltaRhisto->Fill(     LTEMMuVector.DeltaR(LTEMTauVector), TTSingleTauFake_*w );
       ExpSingleTauFake_TauJetDeltaRminhisto->Fill( tauJetDeltaRmin(ltemMuTau[1]),      TTSingleTauFake_*w );
       ExpSingleTauFake_TauPthisto->Fill(           HPSTauPt->at(ltemMuTau[1]),         TTSingleTauFake_*w );
       ExpSingleTauFake_TauEtahisto->Fill(          HPSTauEta->at(ltemMuTau[1]),        TTSingleTauFake_*w );
       ExpSingleTauFake_TauPhihisto->Fill(          HPSTauPhi->at(ltemMuTau[1]),        TTSingleTauFake_*w );
       //
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
       ExpTotalBckgMuPRU_Search1Binhisto->Fill( Search1EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRU_Search2Binhisto->Fill( Search2EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRU_SThisto->Fill(              st,                                 (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRU_JetNhisto->Fill(            nJet,                               (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRU_TauJetDeltaRminhisto->Fill( tauJetDeltaRmin(ltemMuTau[1]),      (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRU_MuJetDeltaRminhisto->Fill(  muJetDeltaRmin(ltemMuTau[0]),       (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRU_TauPthisto->Fill(           HPSTauPt->at(ltemMuTau[1]),         (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRU_TauEtahisto->Fill(          HPSTauEta->at(ltemMuTau[1]),        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRU_MuPthisto->Fill(            MuonPt->at(ltemMuTau[0]),           (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRU_MuEtahisto->Fill(           MuonEta->at(ltemMuTau[0]),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRU_VertexNhisto->Fill(         (double)(VertexX->size()),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
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
       ExpTotalBckgMuPRD_Search1Binhisto->Fill( Search1EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRD_Search2Binhisto->Fill( Search2EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRD_SThisto->Fill(              st,                                 (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRD_JetNhisto->Fill(            nJet,                               (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRD_TauJetDeltaRminhisto->Fill( tauJetDeltaRmin(ltemMuTau[1]),      (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRD_MuJetDeltaRminhisto->Fill(  muJetDeltaRmin(ltemMuTau[0]),       (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRD_TauPthisto->Fill(           HPSTauPt->at(ltemMuTau[1]),         (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRD_TauEtahisto->Fill(          HPSTauEta->at(ltemMuTau[1]),        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRD_MuPthisto->Fill(            MuonPt->at(ltemMuTau[0]),           (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRD_MuEtahisto->Fill(           MuonEta->at(ltemMuTau[0]),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuPRD_VertexNhisto->Fill(         (double)(VertexX->size()),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
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
       ExpTotalBckgMuFRU_Search1Binhisto->Fill( Search1EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRU_Search2Binhisto->Fill( Search2EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRU_SThisto->Fill(              st,                                 (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRU_JetNhisto->Fill(            nJet,                               (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRU_TauJetDeltaRminhisto->Fill( tauJetDeltaRmin(ltemMuTau[1]),      (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRU_MuJetDeltaRminhisto->Fill(  muJetDeltaRmin(ltemMuTau[0]),       (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRU_TauPthisto->Fill(           HPSTauPt->at(ltemMuTau[1]),         (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRU_TauEtahisto->Fill(          HPSTauEta->at(ltemMuTau[1]),        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRU_MuPthisto->Fill(            MuonPt->at(ltemMuTau[0]),           (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRU_MuEtahisto->Fill(           MuonEta->at(ltemMuTau[0]),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRU_VertexNhisto->Fill(         (double)(VertexX->size()),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
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
       ExpTotalBckgMuFRD_Search1Binhisto->Fill( Search1EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRD_Search2Binhisto->Fill( Search2EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRD_SThisto->Fill(              st,                                 (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRD_JetNhisto->Fill(            nJet,                               (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRD_TauJetDeltaRminhisto->Fill( tauJetDeltaRmin(ltemMuTau[1]),      (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRD_MuJetDeltaRminhisto->Fill(  muJetDeltaRmin(ltemMuTau[0]),       (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRD_TauPthisto->Fill(           HPSTauPt->at(ltemMuTau[1]),         (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRD_TauEtahisto->Fill(          HPSTauEta->at(ltemMuTau[1]),        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRD_MuPthisto->Fill(            MuonPt->at(ltemMuTau[0]),           (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRD_MuEtahisto->Fill(           MuonEta->at(ltemMuTau[0]),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgMuFRD_VertexNhisto->Fill(         (double)(VertexX->size()),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
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
       ExpTotalBckgTauPRU_Search1Binhisto->Fill( Search1EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRU_Search2Binhisto->Fill( Search2EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRU_SThisto->Fill(              st,                                 (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRU_JetNhisto->Fill(            nJet,                               (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRU_TauJetDeltaRminhisto->Fill( tauJetDeltaRmin(ltemMuTau[1]),      (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRU_MuJetDeltaRminhisto->Fill(  muJetDeltaRmin(ltemMuTau[0]),       (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRU_TauPthisto->Fill(           HPSTauPt->at(ltemMuTau[1]),         (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRU_TauEtahisto->Fill(          HPSTauEta->at(ltemMuTau[1]),        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRU_MuPthisto->Fill(            MuonPt->at(ltemMuTau[0]),           (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRU_MuEtahisto->Fill(           MuonEta->at(ltemMuTau[0]),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRU_VertexNhisto->Fill(         (double)(VertexX->size()),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
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
       ExpTotalBckgTauPRD_Search1Binhisto->Fill( Search1EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRD_Search2Binhisto->Fill( Search2EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRD_SThisto->Fill(              st,                                 (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRD_JetNhisto->Fill(            nJet,                               (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRD_TauJetDeltaRminhisto->Fill( tauJetDeltaRmin(ltemMuTau[1]),      (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRD_MuJetDeltaRminhisto->Fill(  muJetDeltaRmin(ltemMuTau[0]),       (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRD_TauPthisto->Fill(           HPSTauPt->at(ltemMuTau[1]),         (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRD_TauEtahisto->Fill(          HPSTauEta->at(ltemMuTau[1]),        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRD_MuPthisto->Fill(            MuonPt->at(ltemMuTau[0]),           (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRD_MuEtahisto->Fill(           MuonEta->at(ltemMuTau[0]),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauPRD_VertexNhisto->Fill(         (double)(VertexX->size()),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
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
       ExpTotalBckgTauFRU_Search1Binhisto->Fill( Search1EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRU_Search2Binhisto->Fill( Search2EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRU_SThisto->Fill(              st,                                 (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRU_JetNhisto->Fill(            nJet,                               (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRU_TauJetDeltaRminhisto->Fill( tauJetDeltaRmin(ltemMuTau[1]),      (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRU_MuJetDeltaRminhisto->Fill(  muJetDeltaRmin(ltemMuTau[0]),       (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRU_TauPthisto->Fill(           HPSTauPt->at(ltemMuTau[1]),         (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRU_TauEtahisto->Fill(          HPSTauEta->at(ltemMuTau[1]),        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRU_MuPthisto->Fill(            MuonPt->at(ltemMuTau[0]),           (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRU_MuEtahisto->Fill(           MuonEta->at(ltemMuTau[0]),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRU_VertexNhisto->Fill(         (double)(VertexX->size()),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
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
       ExpTotalBckgTauFRD_Search1Binhisto->Fill( Search1EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRD_Search2Binhisto->Fill( Search2EventType,                        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRD_SThisto->Fill(              st,                                 (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRD_JetNhisto->Fill(            nJet,                               (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRD_TauJetDeltaRminhisto->Fill( tauJetDeltaRmin(ltemMuTau[1]),      (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRD_MuJetDeltaRminhisto->Fill(  muJetDeltaRmin(ltemMuTau[0]),       (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRD_TauPthisto->Fill(           HPSTauPt->at(ltemMuTau[1]),         (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRD_TauEtahisto->Fill(          HPSTauEta->at(ltemMuTau[1]),        (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRD_MuPthisto->Fill(            MuonPt->at(ltemMuTau[0]),           (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRD_MuEtahisto->Fill(           MuonEta->at(ltemMuTau[0]),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       ExpTotalBckgTauFRD_VertexNhisto->Fill(         (double)(VertexX->size()),          (TTSingleTauFake_+TTSingleMuFake_+TTDoubleFake_)*w );
       
       //
       //...........................................................................// Fake Background Estimation - End


       // FILL OUT THE REMAINING HISTOGRAMS ONLY FOR TT EVENTS!
       if( !isMuTight_ || !isTauTight_ ) continue;

       //Apply tau prompt and fake rate corrections!!
       w=w*LTEM_ScaleFactor_;

       Search1Binhisto->Fill(  Search1EventType, w );
       Search2Binhisto->Fill(  Search2EventType, w );

       LTEMMuJetDeltaRminhisto->Fill( muJetDeltaRmin(ltemMuTau[0]), w );
       LTEMTauJetDeltaRminhisto->Fill( tauJetDeltaRmin(ltemMuTau[1]), w );
       LTEMMuPthisto->Fill( MuonPt->at(ltemMuTau[0]), w );
       LTEMTauPthisto->Fill( HPSTauPt->at(ltemMuTau[1]), w );
       LTEMMuEtahisto->Fill( MuonEta->at(ltemMuTau[0]), w );
       LTEMTauEtahisto->Fill( HPSTauEta->at(ltemMuTau[1]), w );
       //LTEMAbsTauEtahisto->Fill( fabs(HPSTauEta->at(ltemMuTau[1])), w );
       LTEMMuPhihisto->Fill( MuonPhi->at(ltemMuTau[0]), w );
       LTEMTauPhihisto->Fill( HPSTauPhi->at(ltemMuTau[1]), w );
       LTEMMuTauDeltaRhisto->Fill( LTEMMuVector.DeltaR(LTEMTauVector), w );
       LTEMMuTauDeltaPhihisto->Fill( LTEMMuVector.DeltaPhi(LTEMTauVector), w );
       LTEMMuTauMasshisto->Fill( (LTEMMuVector+LTEMTauVector).M(), w );
       //
       // For 2D optimization
       LTEM2DMuPtTauPthisto->Fill( MuonPt->at(ltemMuTau[0]), HPSTauPt->at(ltemMuTau[1]), w);
       LTEM2DTauPtSThisto->Fill( HPSTauPt->at(ltemMuTau[1]), st, w);
       LTEM2DTauPtNLepjethisto->Fill( HPSTauPt->at(ltemMuTau[1]), nLepjet, w);
       LTEM2DMuPtNLepjethisto->Fill( MuonPt->at(ltemMuTau[0]), nLepjet, w);
       LTEM2DMuPtSThisto->Fill( MuonPt->at(ltemMuTau[0]), st, w);
       LTEM2DNLepjetSThisto->Fill( nLepjet, st, w);
       LTEM2DNJetSThisto->Fill( nJet, st, w);
       // For 3D optimization
       LTEM3DTauPtSTJetNhisto->Fill( HPSTauPt->at(ltemMuTau[1]), st, nJet, w);
       LTEM3DTauPtSTLepjetNhisto->Fill( HPSTauPt->at(ltemMuTau[1]), st, nLepjet, w);


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
       SThisto->Fill( ST(), w );
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
   muPRwSFhisto->Write();
   muFRwSFhisto->Write();
   tauPRwSFhisto->Write();
   tauFRwSFhisto->Write();
   muPREwSFhisto->Write();
   muFREwSFhisto->Write();
   tauPREwSFhisto->Write();
   tauFREwSFhisto->Write();
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
   LTEM3DTauPtSTJetNhisto->Write();
   LTEM3DTauPtSTLepjetNhisto->Write();
   //
   //ExpTotalBckg_MuTauDeltaRhisto->Write();
   //
   ExpTotalBckgPPonly_SThisto->Write();
   ExpTotalBckgPPonly_JetNhisto->Write();
   ExpTotalBckgPPonly_TauJetDeltaRminhisto->Write();
   ExpTotalBckgPPonly_MuJetDeltaRminhisto->Write();
   ExpTotalBckgPPonly_TauPthisto->Write();
   ExpTotalBckgPPonly_TauEtahisto->Write();
   ExpTotalBckgPPonly_MuPthisto->Write();
   ExpTotalBckgPPonly_MuEtahisto->Write();
   ExpTotalBckgPPonly_VertexNhisto->Write();
   //
   ExpTotalBckg_VertexNhisto->Write();
   ExpTotalBckgTauFRU_VertexNhisto->Write();
   ExpTotalBckgTauFRD_VertexNhisto->Write();
   ExpTotalBckgTauPRU_VertexNhisto->Write();
   ExpTotalBckgTauPRD_VertexNhisto->Write();
   ExpTotalBckgMuFRU_VertexNhisto->Write();
   ExpTotalBckgMuFRD_VertexNhisto->Write();
   ExpTotalBckgMuPRU_VertexNhisto->Write();
   ExpTotalBckgMuPRD_VertexNhisto->Write();
   //
   ExpTotalBckg_TauJetDeltaRminhisto->Write();        
   ExpTotalBckgTauFRU_TauJetDeltaRminhisto->Write();  
   ExpTotalBckgTauFRD_TauJetDeltaRminhisto->Write();  
   ExpTotalBckgTauPRU_TauJetDeltaRminhisto->Write();  
   ExpTotalBckgTauPRD_TauJetDeltaRminhisto->Write();  
   ExpTotalBckgMuFRU_TauJetDeltaRminhisto->Write();   
   ExpTotalBckgMuFRD_TauJetDeltaRminhisto->Write();   
   ExpTotalBckgMuPRU_TauJetDeltaRminhisto->Write();   
   ExpTotalBckgMuPRD_TauJetDeltaRminhisto->Write();   
   //
   ExpTotalBckg_MuJetDeltaRminhisto->Write();        
   ExpTotalBckgTauFRU_MuJetDeltaRminhisto->Write();  
   ExpTotalBckgTauFRD_MuJetDeltaRminhisto->Write();  
   ExpTotalBckgTauPRU_MuJetDeltaRminhisto->Write();  
   ExpTotalBckgTauPRD_MuJetDeltaRminhisto->Write();  
   ExpTotalBckgMuFRU_MuJetDeltaRminhisto->Write();   
   ExpTotalBckgMuFRD_MuJetDeltaRminhisto->Write();   
   ExpTotalBckgMuPRU_MuJetDeltaRminhisto->Write();   
   ExpTotalBckgMuPRD_MuJetDeltaRminhisto->Write();
   //
   ExpTotalBckg_MuPthisto->Write();           
   ExpTotalBckgTauFRU_MuPthisto->Write();     
   ExpTotalBckgTauFRD_MuPthisto->Write();     
   ExpTotalBckgTauPRU_MuPthisto->Write();     
   ExpTotalBckgTauPRD_MuPthisto->Write();     
   ExpTotalBckgMuFRU_MuPthisto->Write();      
   ExpTotalBckgMuFRD_MuPthisto->Write();      
   ExpTotalBckgMuPRU_MuPthisto->Write();      
   ExpTotalBckgMuPRD_MuPthisto->Write();      
   //
   ExpTotalBckg_MuEtahisto->Write();           
   ExpTotalBckgTauFRU_MuEtahisto->Write();     
   ExpTotalBckgTauFRD_MuEtahisto->Write();     
   ExpTotalBckgTauPRU_MuEtahisto->Write();     
   ExpTotalBckgTauPRD_MuEtahisto->Write();     
   ExpTotalBckgMuFRU_MuEtahisto->Write();      
   ExpTotalBckgMuFRD_MuEtahisto->Write();      
   ExpTotalBckgMuPRU_MuEtahisto->Write();      
   ExpTotalBckgMuPRD_MuEtahisto->Write();      
   //
   ExpTotalBckg_TauPthisto->Write();           
   ExpTotalBckgTauFRU_TauPthisto->Write();     
   ExpTotalBckgTauFRD_TauPthisto->Write();     
   ExpTotalBckgTauPRU_TauPthisto->Write();     
   ExpTotalBckgTauPRD_TauPthisto->Write();     
   ExpTotalBckgMuFRU_TauPthisto->Write();      
   ExpTotalBckgMuFRD_TauPthisto->Write();      
   ExpTotalBckgMuPRU_TauPthisto->Write();      
   ExpTotalBckgMuPRD_TauPthisto->Write();      
   //
   ExpTotalBckg_TauEtahisto->Write();           
   ExpTotalBckgTauFRU_TauEtahisto->Write();     
   ExpTotalBckgTauFRD_TauEtahisto->Write();     
   ExpTotalBckgTauPRU_TauEtahisto->Write();     
   ExpTotalBckgTauPRD_TauEtahisto->Write();     
   ExpTotalBckgMuFRU_TauEtahisto->Write();      
   ExpTotalBckgMuFRD_TauEtahisto->Write();      
   ExpTotalBckgMuPRU_TauEtahisto->Write();      
   ExpTotalBckgMuPRD_TauEtahisto->Write();      
   //
   ExpTotalBckg_SThisto->Write();               
   ExpTotalBckgTauFRU_SThisto->Write();         
   ExpTotalBckgTauFRD_SThisto->Write();         
   ExpTotalBckgTauPRU_SThisto->Write();         
   ExpTotalBckgTauPRD_SThisto->Write();         
   ExpTotalBckgMuFRU_SThisto->Write();          
   ExpTotalBckgMuFRD_SThisto->Write();          
   ExpTotalBckgMuPRU_SThisto->Write();          
   ExpTotalBckgMuPRD_SThisto->Write();          
   //
   ExpTotalBckg_JetNhisto->Write();             
   ExpTotalBckgTauFRU_JetNhisto->Write();       
   ExpTotalBckgTauFRD_JetNhisto->Write();       
   ExpTotalBckgTauPRU_JetNhisto->Write();       
   ExpTotalBckgTauPRD_JetNhisto->Write();       
   ExpTotalBckgMuFRU_JetNhisto->Write();        
   ExpTotalBckgMuFRD_JetNhisto->Write();        
   ExpTotalBckgMuPRU_JetNhisto->Write();        
   ExpTotalBckgMuPRD_JetNhisto->Write();        
   //
   Search1Binhisto->Write();                      
   ExpTotalBckg_Search1Binhisto->Write();   
   ExpTotalBckgPPonly_Search1Binhisto->Write();
   ExpTotalBckgTauFRU_Search1Binhisto->Write();
   ExpTotalBckgTauFRD_Search1Binhisto->Write();
   ExpTotalBckgTauPRU_Search1Binhisto->Write();
   ExpTotalBckgTauPRD_Search1Binhisto->Write();
   ExpTotalBckgMuFRU_Search1Binhisto->Write(); 
   ExpTotalBckgMuFRD_Search1Binhisto->Write(); 
   ExpTotalBckgMuPRU_Search1Binhisto->Write(); 
   ExpTotalBckgMuPRD_Search1Binhisto->Write(); 
   //
   Search2Binhisto->Write();                      
   ExpTotalBckg_Search2Binhisto->Write();         
   ExpTotalBckgPPonly_Search2Binhisto->Write();   
   ExpTotalBckgTauFRU_Search2Binhisto->Write();   
   ExpTotalBckgTauFRD_Search2Binhisto->Write();   
   ExpTotalBckgTauPRU_Search2Binhisto->Write();   
   ExpTotalBckgTauPRD_Search2Binhisto->Write();   
   ExpTotalBckgMuFRU_Search2Binhisto->Write();    
   ExpTotalBckgMuFRD_Search2Binhisto->Write();    
   ExpTotalBckgMuPRU_Search2Binhisto->Write();    
   ExpTotalBckgMuPRD_Search2Binhisto->Write();
   //
   //
   ExpSingleMuFake_SThisto->Write();
   ExpDoubleFake_SThisto->Write();
   ExpSingleTauFake_SThisto->Write();
   ExpSingleTauFake_JetNhisto->Write();
   ExpSingleTauFake_MuTauDeltaRhisto->Write();
   ExpSingleTauFake_TauJetDeltaRminhisto->Write();
   ExpSingleTauFake_TauPthisto->Write();
   ExpSingleTauFake_TauEtahisto->Write();
   ExpSingleTauFake_TauPhihisto->Write();
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
