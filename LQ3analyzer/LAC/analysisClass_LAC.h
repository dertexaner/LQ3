#ifndef analysisClass_h
#define analysisClass_h

#include "baseClass.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <memory>
#include "TString.h"
#include "TLorentzVector.h"
#include <vector>
using namespace std;

class analysisClass : public baseClass {
public :
  analysisClass(string * inputList, string * cutFile, string * treeName,  string *outputFileName=0, string * cutEfficFile=0);
  virtual ~analysisClass();
  void Loop();
  //
  // Object ID variables
  TString ObjDef;  // LI, TI
  TString BTagDef; // TCHPT, JPL, JPM, JPT, CSVL, CSVM, CSVT
  TString idDef;   // HEEP, CUT, MVA
  //
  // Global Variables
  double MuonPtCut; // >20
  double MuonEtaCut;// <2.4
  double TauPtCut;  // >20
  bool   ApplyAbsMuonIsolation;
  bool   removeOverlaps;
  std::vector<unsigned int> ltemMuMu;
  std::vector<unsigned int> ltemMuTau;
  std::vector<unsigned int> ltemMuTauOS;
  //
  // Prompt & Fake Rates
  double muPR_;
  double muFR_;
  double tauPR_; 
  double tauFR_;
  //
  // Prompt & Fake Scale Factors
  //-------
  double muPRdR3B_SF;  double muPRdR3B_SFerr;
  double muPRdR2B_SF;  double muPRdR2B_SFerr;
  double muPRdR1B_SF;  double muPRdR1B_SFerr;
  //-------
  double muPRdR3E_SF;  double muPRdR3E_SFerr;
  double muPRdR2E_SF;  double muPRdR2E_SFerr;
  double muPRdR1E_SF;  double muPRdR1E_SFerr;
  //------- ------- -------
  double tauPRdR3B_SF;  double tauPRdR3B_SFerr;
  double tauPRdR2B_SF;  double tauPRdR2B_SFerr;
  double tauPRdR1B_SF;  double tauPRdR1B_SFerr;
  //-------
  double tauPRdR3E_SF;  double tauPRdR3E_SFerr;
  double tauPRdR2E_SF;  double tauPRdR2E_SFerr;
  double tauPRdR1E_SF;  double tauPRdR1E_SFerr;
  //-------
  double tauFRdR3B_SF;  double tauFRdR3B_SFerr;
  double tauFRdR2B_SF;  double tauFRdR2B_SFerr;
  double tauFRdR1B_SF;  double tauFRdR1B_SFerr;
  //-------
  double tauFRdR3E_SF;  double tauFRdR3E_SFerr;
  double tauFRdR2E_SF;  double tauFRdR2E_SFerr;
  double tauFRdR1E_SF;  double tauFRdR1E_SFerr;
  //

  //
  // Muon Prompt Rate Function Parameters
  double LQdr3Bpar1;    double LQdr3Bpar2;    double LQdr3Bpar3;
  double LQdr2Bpar1;    double LQdr2Bpar2;    double LQdr2Bpar3;
  double LQdr1Bpar1;    double LQdr1Bpar2;    double LQdr1Bpar3;
  double DATAdr3Bpar1;  double DATAdr3Bpar2;  double DATAdr3Bpar3;
  double DATAdr2Bpar1;  double DATAdr2Bpar2;  double DATAdr2Bpar3;
  double DATAdr1Bpar1;  double DATAdr1Bpar2;  double DATAdr1Bpar3;
  double Wdr3Bpar1;     double Wdr3Bpar2;     double Wdr3Bpar3;
  double Wdr2Bpar1;     double Wdr2Bpar2;     double Wdr2Bpar3;
  double Wdr1Bpar1;     double Wdr1Bpar2;     double Wdr1Bpar3;
  double LQdr3BparErr1;    double LQdr3BparErr2;    double LQdr3BparErr3;
  double LQdr2BparErr1;    double LQdr2BparErr2;    double LQdr2BparErr3;
  double LQdr1BparErr1;    double LQdr1BparErr2;    double LQdr1BparErr3;
  double DATAdr3BparErr1;  double DATAdr3BparErr2;  double DATAdr3BparErr3;
  double DATAdr2BparErr1;  double DATAdr2BparErr2;  double DATAdr2BparErr3;
  double DATAdr1BparErr1;  double DATAdr1BparErr2;  double DATAdr1BparErr3;
  double Wdr3BparErr1;     double Wdr3BparErr2;     double Wdr3BparErr3;
  double Wdr2BparErr1;     double Wdr2BparErr2;     double Wdr2BparErr3;
  double Wdr1BparErr1;     double Wdr1BparErr2;     double Wdr1BparErr3;
  double LQdr3Epar1;    double LQdr3Epar2;    double LQdr3Epar3;
  double LQdr2Epar1;    double LQdr2Epar2;    double LQdr2Epar3;
  double LQdr1Epar1;    double LQdr1Epar2;    double LQdr1Epar3;
  double DATAdr3Epar1;  double DATAdr3Epar2;  double DATAdr3Epar3;
  double DATAdr2Epar1;  double DATAdr2Epar2;  double DATAdr2Epar3;
  double DATAdr1Epar1;  double DATAdr1Epar2;  double DATAdr1Epar3;
  double Wdr3Epar1;     double Wdr3Epar2;     double Wdr3Epar3;
  double Wdr2Epar1;     double Wdr2Epar2;     double Wdr2Epar3;
  double Wdr1Epar1;     double Wdr1Epar2;     double Wdr1Epar3;
  double LQdr3EparErr1;    double LQdr3EparErr2;    double LQdr3EparErr3;
  double LQdr2EparErr1;    double LQdr2EparErr2;    double LQdr2EparErr3;
  double LQdr1EparErr1;    double LQdr1EparErr2;    double LQdr1EparErr3;
  double DATAdr3EparErr1;  double DATAdr3EparErr2;  double DATAdr3EparErr3;
  double DATAdr2EparErr1;  double DATAdr2EparErr2;  double DATAdr2EparErr3;
  double DATAdr1EparErr1;  double DATAdr1EparErr2;  double DATAdr1EparErr3;
  double Wdr3EparErr1;     double Wdr3EparErr2;     double Wdr3EparErr3;
  double Wdr2EparErr1;     double Wdr2EparErr2;     double Wdr2EparErr3;
  double Wdr1EparErr1;     double Wdr1EparErr2;     double Wdr1EparErr3;

  //
  // Lumi
  double LumiA;
  double LumiB;
  double LumiC;
  double LumiD;
  //

  //
  // --- ADDON1 ---
  bool   muRCheck( unsigned int );     
  bool   muRLooseCheck( unsigned int );
  bool   muRTightCheck( unsigned int );  
  bool   tauRCheck( unsigned int );
  bool   tauRLooseCheck( unsigned int );
  bool   tauRTightCheck( unsigned int );
  bool   elRCheck( unsigned int );
  bool   elRLooseCheck( unsigned int );
  bool   elRTightCheck( unsigned int );
  bool   jetRCheck( unsigned int );
  bool   jetRLooseCheck(unsigned int );
  bool   jetRTightCheck(unsigned int );
  bool   muRisoCheck( unsigned int );
  bool   tauRisoCheck( unsigned int );
  bool   elRisoCheck( unsigned int );
  bool   jetRisoCheck( unsigned int );
  bool   btag_jetRCheck( unsigned int );
  bool   vertexRCheck( unsigned int );
  //
  bool   passEventFilter();
  bool   isZToMuMu();
  bool   isMuTauDR0p30(); 
  bool   isMuMuDR0p30();
  //
  double safePileupWeights();
  //
  double SingleMu_TriggerEfficiencyWeights_Preselection();
  double IsoMu24e2p1_Eff_Preselection();
  double IsoMu24e2p1_Eff_MC( double, double );
  double IsoMu24e2p1_Eff_Data( double, double );
  //
  double Mu40e2p1_Eff_MC( double, double );
  double Mu40e2p1_Eff_Data( double, double );
  double Mu40e2p1_ScaleFactor( double, double );
  //
  double SingleMu_TriggerEfficiencyWeights();
  double DoubleMu_TriggerEfficiencyWeights();
  double IsoMu24e2p1_Eff( double, double );
  double Mu17_Mu8_Eff( double, double );
  double jetBValue( unsigned int );
  double elISOEffectiveAreaCone04( double );
  double elISOEffectiveAreaCone03( double );
  //
  double LTEM_ScaleFactor( unsigned int, bool, unsigned int, bool );
  double muJetDeltaRmin( unsigned int );
  double tauJetDeltaRmin( unsigned int );
  double tauFR( unsigned int );
  double tauPR( unsigned int );
  double muFR( unsigned int );
  double muPR( unsigned int );
  double tauFRE( unsigned int );
  double tauPRE( unsigned int );
  double muFRE( unsigned int );
  double muPRE( unsigned int );
  double QuadSum( double, double );
  double pol0w_pol1tt_Err( double, double, double, double, double, double, double, double, unsigned int );
  double pol0tt_pol1w_Err( double, double, double, double, double, double, double, double, unsigned int );
  double PromptRateFunction( double, double, double, double );
  double PromptRateFunctionError( double, double, double, double, double, double, double );
  double AvePromptRate(double, double, double, double, double, double, double);
  double ErrPromptRate(double, double, double, double, double, double, double);
  void   InitMuonFunctionParameters();
  double CorrFactor(  double, double, double, double,  double, double, double );
  double CorrFactorError( double, double, double, double, double, double, double, double, double, double, double, double, double );
  //
  double DoubleFakeWeight( unsigned int, unsigned int );
  double SingleFakeMuWeight( unsigned int, unsigned int );
  double SingleFakeTauWeight( unsigned int, unsigned int );
  //
  //
  bool   isMuTauDR0p35();                    //obsolete
  bool   isMuEl_NoTau_DR0p30();              //obsolete
  bool   isZToTauTau();                      //obsolete
  bool   tauRMigrationCheck( unsigned int ); //obsolete
  bool   muRMigrationCheck( unsigned int );  //obsolete
  bool   Wpeak_jetRCheck(unsigned int );     //obsolete
  //
  // --- ADDON2 ---
  int    SingleMu_passTrigger();
  int    DoubleMu_passTrigger();
  int    HLT_MuPT_eta2p1_passTrigger();
  int    SingleMu40_passTrigger();
  int    Jet_passTrigger();
  bool   isAllMuonsHLT( );
  double RecoHLTdeltaRmin_DoubleMuTrigger( unsigned int );
  double RecoHLTdeltaRmin_SingleMuTrigger( unsigned int );
  double RecoHLTdeltaRmin_SingleMu40Trigger( unsigned int );
  double RecoHLTdeltaRmin_SingleMu24Trigger( unsigned int );
  double MuonForTrigEffAnalysis_SingleMuTrigger( int, TString, int );
  //
  bool isIsoMuMatchedToHLTMu(unsigned int );                             //obsolete
  bool isIsoMuMatchedToHLTMu_SingleMuDataset(unsigned int );             //obsolete
  bool isHLTMuMatchedToIsoMu( unsigned int );                            //obsolete
  bool isIsoMuMatchedToHLTTau(unsigned int);                             //obsolete
  bool isTauForTrigEffAnalysis( unsigned int );                          //obsolete
  void CheckOfflinevsTrigger();                                          //obsolete
  unsigned int TauForTrigEffAnalysis_MuTauTrigger();                     //obsolete
  int MuTau_passTrigger();                                               //obsolete
  int SingleMuMET_passTrigger();                                         //obsolete
  int MC_MuTau_passTrigger( double muPtCUT );                            //obsolete
  int availableMuTauTrigger();                                           //obsolete
  double RecoHLTdeltaRmin_MuTauTrigger(unsigned int, int );              //obsolete
  double RecoHLTdeltaRmin_SingleMuMETTrigger( unsigned int, int );       //obsolete
  bool SingleMu_SingleMuMET_isOverlap( int );                            //obsolete
  double MuonForTrigEffAnalysis_SingleMuMETTrigger( int, TString, int ); //obsolete
  double MuonForTrigEffAnalysis_MuTauTrigger(  int, TString );           //obsolete
  void printTriggerObjects_MuonFilters( Long64_t jentry );               //obsolete
  void TriggerObjectViewer();                                            //obsolete
  int availableTrigger();                                                //obsolete
  void TriggerDecisionViewer();                                          //obsolete
  bool areHPSTausAwayfromHLTMu( TLorentzVector, TLorentzVector );        //obsolete
  bool areMuonsAwayfromHLTTau( TLorentzVector, TLorentzVector );         //obsolete
  double deltaR_IsoTauMatchedToHLTTau_dR0p2fromHLTMu( unsigned int );    //obsolete
  double deltaR_IsoMuMatchedToHLTMu_dR0p3fromHLTTau( unsigned int );     //obsolete
  double DeltaRminIsoMuHLTMu(unsigned int , double );                    //obsolete
  //
  // --- ADDON3 ---
  bool   PreSelection( TString );
  bool   DiscoverySelection( TString );
  void   which_MuMu( std::vector<unsigned int>& );
  void   which_MuTau( std::vector<unsigned int>& );
  void   which_MuTauOS( std::vector<unsigned int>& );
  int    RecoSignalType();
  int    LTEM_MuMu();
  int    LTEM_MuTau();
  int    TauCounter();
  int    MuCounter();
  int    ElCounter();
  int    JetCounter();
  int    BJetCounter();
  double LeadingMuPt();
  double LeadingTauPt();
  double LeadingJetPt();
  double ST();
  double MaxDiLepInvMass();
  double MaxMuMuInvMass();
  double MaxMuTauInvMass();
  double MaxTauTauInvMass();
  double METMuTauDeltaPhi();
  double METlepDeltaPhi( TString, int );
  double METlepMT( TString, int = -1 );
  bool   isMu15Tau20_MigrationSkim();
  bool   isRecoMuMatchedTO( double , double , double , double );
  bool   isRecoTauMatchedTO( double , double , double , double );
  bool   isOneMuonSuperIso();
  double DeltaRminObjects( int, int );
  double MuTaudeltaPzeta();
  double deltaPzeta( unsigned int, unsigned int );
  double Pzeta( unsigned int, unsigned int );
  double visPzeta( unsigned int, unsigned int );
  //
  double MuTauInZpeak( TString );                //obsolete
  double tauGenMatchDeltaR(unsigned int iTauR);  //obsolete
  double muGenMatchDeltaR(unsigned int iMuR);    //obsolete
  bool   isMuTau();                              //obsolete

  //
  //
  //ADDON4,5
  bool isRecoTauFake( unsigned int );
  bool isRecoMuonFake( unsigned int );
  //
  bool   isFakeMu_LQ();
  bool   isFakeMu_TOP();
  bool   isFakeEl_LQ();
  bool   isFakeEl_TOP();
  bool   isFakeTau_LQ();
  bool   isFakeTau_TOP();
  double tauGenMatchDeltaR_LQ( unsigned int );
  double tauGenMatchDeltaR_TOP( unsigned int );
  double muGenMatchDeltaR_LQ( unsigned int );
  double muGenMatchDeltaR_TOP( unsigned int );
  double elGenMatchDeltaR_LQ( unsigned int );
  double elGenMatchDeltaR_TOP( unsigned int );
  //
};

#endif

#ifdef analysisClass_cxx

#endif // #ifdef analysisClass_cxx
