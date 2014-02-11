/////////////////////////////////////////////////////////
// Event-object Functions : HT, ST, DeltaR(jet,tau)_min, etc..
//
// H.S  - Feb 2012
//////////////////////////////////////////////////////////
#define analysisClass_cxx
#include "analysisClass.h"
#include <TH2.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TVector3.h>
#include <iostream>
using namespace std;
// // //
bool analysisClass::PreSelection(TString Process){
  //
  int TotalN=0;
  int TotalTau=0;
  int TotalMu=0;
  int TotalEl=0;
  int TotalJet=0;
  int TotalBJet=0;
  TotalTau  = TauCounter();
  TotalMu   = MuCounter();
  TotalEl   = ElCounter();
  TotalJet  = JetCounter();
  TotalBJet = BJetCounter();
  TotalN = TotalTau + TotalMu + TotalEl + TotalJet;
  //
  double LeadMuTauDeltaR=0;
  TLorentzVector Mu;
  TLorentzVector Tau;
  Mu.SetPtEtaPhiM(0,0,0,0);
  Tau.SetPtEtaPhiM(0,0,0,0);
  for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
    if( !tauRisoCheck(iTauR) )continue;
    if( tauPtcorr(iTauR)>Tau.Pt() ){
      Tau.SetPtEtaPhiM(tauPtcorr(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0);
    }
  }
  for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
    if( !muRisoCheck(iMuR) )continue;
    if( muPtcorr(iMuR)>Mu.Pt() ){
      Mu.SetPtEtaPhiM(muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0);
    }
  }
  if( TotalMu>0 && TotalTau>0 ) LeadMuTauDeltaR=Mu.DeltaR(Tau);
  //
  //
  if( Process != "Neutr" && Process != "Neutr2Jet" && Process != "NeutrNoQCD" && Process != "Neutr2JetNoQCD" && Process != "Neutr2Jet250ST" && Process != "Neutr2Jet250STtuneZ"
      && Process != "Neutr0Btag" && Process != "Neutr0BtagNoQCD" && Process != "Neutr2Jet350STtuneZ" && Process != "Neutr1Jet350STtuneZ"
      && Process != "Neutr1Jet350ST" && Process != "Neutr1Jet300ST" && Process != "Neutr1Jet500ST"
      && Process != "ZToMuMuAnalysis" && Process != "ZToMuTauAnalysis" && Process != "TTBar" && Process != "WJets" && Process != "FakeTaus" && Process != "ControlRegion1"
      && Process != "FakeMuons" && Process != "FakeMuonsV2" && Process != "FakeMuonsV3" && Process != "ZJets" && Process != "QCD" && Process != "LQ3M400" ){ return false; }
  //
  if( Process == "Neutr" ){
    //if( MaxDiLepInvMass()<55    ) return false;
    if( MaxMuMuInvMass()>0 && MaxMuMuInvMass()<50 ) return false;
    //if( isZToMuMu()             ) return false;//required to exclude events in MuTrig Calculation
    if( TotalJet<1              ) return false;
  }
  if( Process == "Neutr2Jet" ){
    if( MaxMuMuInvMass()>0 && MaxMuMuInvMass()<50 ) return false;
    if( TotalJet<2              ) return false;
  }
  if( Process == "Neutr2Jet250ST" ){
    if( MaxMuMuInvMass()>0 && MaxMuMuInvMass()<50 ) return false;
    if( TotalJet<2              ) return false;
    if( ST()<250                ) return false;
  }
  if( Process == "Neutr2Jet250STtuneZ" ){
    if( MaxMuMuInvMass()>0   && MaxMuMuInvMass()<55   ) return false;
    if( MaxMuTauInvMass()>0  && MaxMuTauInvMass()<55  ) return false;
    if( MaxTauTauInvMass()>0 && MaxTauTauInvMass()<55 ) return false;
    if( TotalJet<2              ) return false;
    if( ST()<250                ) return false;
  }
  if( Process == "Neutr2Jet350STtuneZ" ){
    if( MaxMuMuInvMass()>0   && MaxMuMuInvMass()<55   ) return false;
    if( MaxMuTauInvMass()>0  && MaxMuTauInvMass()<55  ) return false;
    if( MaxTauTauInvMass()>0 && MaxTauTauInvMass()<55 ) return false;
    if( TotalJet<2              ) return false;
    if( ST()<350                ) return false;
  }
  if( Process == "Neutr1Jet350STtuneZ" ){
    if( MaxMuMuInvMass()>0   && MaxMuMuInvMass()<55   ) return false;
    if( MaxMuTauInvMass()>0  && MaxMuTauInvMass()<55  ) return false;
    if( MaxTauTauInvMass()>0 && MaxTauTauInvMass()<55 ) return false;
    if( TotalJet<1              ) return false;
    if( ST()<350                ) return false;
  }
  if( Process == "Neutr1Jet350ST" ){
    if( TotalJet<1              ) return false;
    if( ST()<350                ) return false;
  }
  if( Process == "Neutr1Jet500ST" ){
    if( TotalJet<1              ) return false;
    if( ST()<500                ) return false;
  }
  if( Process == "Neutr1Jet300ST" ){
    if( TotalJet<1              ) return false;
    if( ST()<300                ) return false;
  }
  if( Process == "Neutr0Btag"){
    if( MaxMuMuInvMass()>0 && MaxMuMuInvMass()<50 ) return false;
    if( TotalBJet>0             ) return false;
  }
  if( Process == "Neutr0BtagNoQCD"){
    if( MaxMuMuInvMass()>0 && MaxMuMuInvMass()<50 ) return false;
    if( TotalBJet>0             ) return false;
    if( METlepMT("Mu")<75       ) return false;
  }
  if( Process == "NeutrNoQCD" ){
    if( MaxMuMuInvMass()>0 && MaxMuMuInvMass()<50 ) return false;
    if( isZToMuMu()             ) return false;//required to exclude events in MuTrig Calculation
    if( TotalJet<1              ) return false;
    if( METlepMT("Mu")<60       ) return false;
  }
  if( Process == "Neutr2JetNoQCD" ){
    if( MaxMuMuInvMass()>0 && MaxMuMuInvMass()<50 ) return false;
    if( isZToMuMu()             ) return false;//required to exclude events in MuTrig Calculation
    if( TotalJet<2              ) return false;
    if( METlepMT("Mu")<60       ) return false;
  }
  //
  if( Process == "FakeTaus" ){
    if( TotalBJet>0              ) return false;
    if( METlepMT("Mu")<40        ) return false;
    if( TotalJet<1               ) return false;
    //if( RecoSignalType()!=-2110  ) return false;//this is mu,tau opp sign
    if( RecoSignalType()!=+2110  ) return false;//this is mu,tau same sign
    if( MaxMuTauInvMass()<80     ) return false;
    //
    if( !isOneMuonSuperIso()     ) return false;
  }
  if( Process == "ZToMuTauAnalysis" ){
    if( MaxMuTauInvMass()<20     ) return false;
    if( TotalBJet>0              ) return false;
    if( METlepMT("Mu")>40        ) return false;
    if( METlepMT("Tau")>90       ) return false;
    if( TotalJet<1               ) return false;
    if( RecoSignalType()!=-2110  ) return false;//this is mu,tau opp sign
    if( MuTaudeltaPzeta()<-15    ) return false;
    //
    if( !isOneMuonSuperIso()     ) return false;
    //
    if( MaxMuTauInvMass()<45 || MaxMuTauInvMass()>75 ) return false;
  }
  if( Process == "ZToMuMuAnalysis" ){
    double MaxMuMuInvMass_ = MaxMuMuInvMass();
    double LeadingMuPt_ =LeadingMuPt();
    if( LeadingMuPt_<35          ) return false;//LJ filter
    if( MaxMuMuInvMass_<80 || MaxMuMuInvMass_>100 ) return false;
    if( TotalJet<1               ) return false;
    if( ST()<300                 ) return false;
    if( TotalBJet>0              ) return false;
    if( RecoSignalType()!=-2020  ) return false;//this is mu,mu opp sign
  }
  if( Process == "FakeMuons" ){
    double LeadingMuPt_  = LeadingMuPt();
    if( LeadingMuPt_<35          ) return false;//LJ filter
    if( TotalJet<1               ) return false;
    double LeadingJetPt_ = LeadingJetPt();
    if( LeadingJetPt_<50         ) return false;
    if( ST()<400                 ) return false;
    if( METlepMT("Mu")>20        ) return false;
    if( METcorr("Pt")>20  ) return false;
    //if( TotalBJet>0              ) return false;
    if( abs(RecoSignalType())!=1010  ) return false;//this is one mu
  }
  if( Process == "FakeMuonsV2" ){
    ////double LeadingMuPt_  = LeadingMuPt();
    ////if( LeadingMuPt_<35          ) return false;//LJ filter removed
    if( TotalJet<1                   ) return false;
    //double LeadingJetPt_ = LeadingJetPt();
    //if( LeadingJetPt_<50         ) return false;
    //if( !isZToMuMu()             ) return false;
    //if( ST()>350                 ) return false;
    if( METlepMT("Mu")>20            ) return false;
    if( METcorr("Pt")>20      ) return false;
    if( abs(RecoSignalType())!=1010  ) return false;//this is one mu
  }
  if( Process == "FakeMuonsV3" ){
    if( METlepMT("Mu")>10            ) return false;
    if( METcorr("Pt")>10      ) return false;
    if( TotalJet<1                   ) return false;//
    if( abs(RecoSignalType())!=1010  ) return false;//this is one mu
    if( !isAllMuonsHLT()             ) return false;//mu has to match the trigger
    //
    //--------- Veto on 2nd Global Muon ---------------------
    int nGlobalMuons=0;
    for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
      if( muPtcorr(iMuR)>MuonPtCut && fabs(MuonEta->at(iMuR))<2.1 && MuonIsGlobal->at(iMuR)==1 ) nGlobalMuons++;
    }
    if( nGlobalMuons>1               ) return false;
    //
    //--------- Check for back-to-back Jet-Muon -------------
    bool isBackToBackJet=false;
    TLorentzVector Mu;
    TLorentzVector Jet;
    for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
      if( !muRisoCheck(iMuR)     ) continue;
      Mu.SetPtEtaPhiM(muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0);
      for(unsigned int iJetR=0;  iJetR<PFJetPt->size();     iJetR++){
	if( !jetRTightCheck(iJetR) ) continue;
	Jet.SetPtEtaPhiM( jetPtcorr(iJetR), PFJetEta->at(iJetR), PFJetPhi->at(iJetR), 0 );
	if( (fabs(fabs(Mu.DeltaPhi(Jet))-TMath::Pi())/TMath::Pi())<0.1 )  isBackToBackJet=true;
      }
    }
    //--------- Check for back-to-back Jet-Muon ------------- 
    if( !isBackToBackJet             ) return false;
  }
  if( Process == "ControlRegion1" ){
    //low in signal contamination, to check tau fakes loose->tight ratio
    if( TotalBJet<1                    ) return false;
    if( TotalJet<2                     ) return false;
    ///ST cut reverted
    ///remove MaxMuTauInvMass CUT:  /*if( MaxMuTauInvMass()<100          ) return false;*/
    if( LeadingTauPt()<50              ) return false;
    if( RecoSignalType()!=2110         ) return false;// SS selection
    if( isZToMuMu()                    ) return false;// required to exclude events in MuTrig Calculation  
    //
    if( ST()>400                       ) return false;// Revert the ST cut
  }
  if( Process == "TTBar" ){
    double ZToMuMuCentral = 90;
    double ZToMuTauCentral = 75;
    if( MaxDiLepInvMass()<65       ) return false;
    if( TotalN<5                 ) return false;
    if( ST()<250           ) return false;
    if( RecoSignalType()>0 ) return false;
    if( TotalJet<3               ) return false;
    if( fabs(MuTauInZpeak("ZToMuMu")-ZToMuMuCentral)<10  ) return false; //exclue 80-100
    if( fabs(MuTauInZpeak("ZToMuTau")-ZToMuTauCentral)<15 ) return false; //exclude 60-90
  }
  if( Process == "WJets" ){
    double ZToMuMuCentral = 90;
    double ZToMuTauCentral = 75;
    if( MaxDiLepInvMass()<65       ) return false;
    if( TotalMu!=1          ) return false;
    if( TotalJet!=0         ) return false;
    if( ST()>250      ) return false;
    if( fabs(MuTauInZpeak("ZToMuMu")-ZToMuMuCentral)<10  ) return false; //exclue 80-100
    if( fabs(MuTauInZpeak("ZToMuTau")-ZToMuTauCentral)<15 ) return false; //exclude 60-90
  }
  if( Process == "ZJets" ){
    if( MaxDiLepInvMass()<65 ) return false;
    if( METlepMT("Mu")>50  ) return false;
    if( TotalMu < 2                ) return false;
    double ZToMuMuCentral  = 90;
    double ZToMuTauCentral = 75;
    bool   dilepZpeak_     = false;
    if( fabs(MuTauInZpeak("ZToMuMu")-ZToMuMuCentral)<10  ) dilepZpeak_=true; //accept 80-100
    if( fabs(MuTauInZpeak("ZToMuTau")-ZToMuTauCentral)<15 ) dilepZpeak_=true; //accept 60-90
    if( !dilepZpeak_               ) return false;
  }
  if( Process == "LQ3M400" ){
    if( MaxDiLepInvMass()<65       ) return false;
    if( LeadMuTauDeltaR>4          ) return false;
    if( TotalN<4                   ) return false;
    if( METcorr("Pt")<30    ) return false;
    if( ST()<400             ) return false;
    if( RecoSignalType()<0   ) return false;
  }
  //
  return true;
  //
}
// // //
bool analysisClass::DiscoverySelection( TString SignalChannel ){
  //
  int TotalN=0;
  int TotalTau=0;
  int TotalMu=0;
  int TotalEl=0;
  int TotalJet=0;
  int TotalBJet=0;
  TotalTau  = TauCounter();
  TotalMu   = MuCounter();
  TotalEl   = ElCounter();
  TotalJet  = JetCounter();
  TotalBJet = BJetCounter();
  TotalN    = TotalTau + TotalMu + TotalEl + TotalJet;
  //
  double LeadMuTauDeltaR=0;
  TLorentzVector Mu;
  TLorentzVector Tau;
  Mu.SetPtEtaPhiM(0,0,0,0);
  Tau.SetPtEtaPhiM(0,0,0,0);
  for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
    if( !tauRisoCheck(iTauR) )continue;
    if( tauPtcorr(iTauR)>Tau.Pt() ){
      Tau.SetPtEtaPhiM(tauPtcorr(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0);
    }
  }
  for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
    if( !muRisoCheck(iMuR)        )continue;
    if( muPtcorr(iMuR)>Mu.Pt() ){
      Mu.SetPtEtaPhiM(muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0);
    }
  }
  if( TotalMu>0 && TotalTau>0 ) LeadMuTauDeltaR=Mu.DeltaR(Tau);
  //
  //
  // ASSUME PRESELECTION IS ALREADY APPLIED
  if( SignalChannel == "MuMu" ){
    if( TotalBJet<1                    ) return false;
    if( TotalJet<2                     ) return false;
    if( ST()<400                       ) return false;
    if( LeadingMuPt()<20               ) return false;
    if( RecoSignalType()!=2020 && TotalMu<3 ) return false;//mu-mu SS selection
    if( isZToMuMu()                    ) return false;//required to exclude events in MuTrig Calculation
  }
  if( SignalChannel == "MuTau" ){
    if( TotalBJet<1                    ) return false;
    if( TotalJet<2                     ) return false;//SS selection
    if( ST()<500                       ) return false;
    if( MaxMuTauInvMass()<100          ) return false;
    if( LeadingTauPt()<50              ) return false;
    if( RecoSignalType()<2000          ) return false;//mu-tau+X OS selection
    if( isZToMuMu()                    ) return false;//required to exclude events in MuTrig Calculation
  }
  //
  return true;
  //
}
// // //
void analysisClass::which_MuMu( std::vector<unsigned int>& mumuVector){
  //check for muons
  std::vector<std::pair<double, unsigned int> > loosePMuons;
  std::vector<std::pair<double, unsigned int> > looseNMuons;
  std::vector<std::pair<double, unsigned int> >  looseMuons;
  //
  for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
    if( muRisoCheck(iMuR) ){
      if( MuonCharge->at(iMuR)>0 ) loosePMuons.push_back( std::make_pair(muPtcorr(iMuR),iMuR) );
      if( MuonCharge->at(iMuR)<0 ) looseNMuons.push_back( std::make_pair(muPtcorr(iMuR),iMuR) );
    }
  }
  //
  double STmumu=0;
  if( loosePMuons.size()>1 ){
    STmumu=loosePMuons[0].first+loosePMuons[1].first;
    looseMuons.push_back( std::make_pair(loosePMuons[0].first,loosePMuons[0].second) );
    looseMuons.push_back( std::make_pair(loosePMuons[1].first,loosePMuons[1].second) );
  }
  if( looseNMuons.size()>1 ){
    if( (looseNMuons[0].first+looseNMuons[1].first)>STmumu ){
      STmumu=looseNMuons[0].first+looseNMuons[1].first;
      looseMuons.clear();
      looseMuons.push_back( std::make_pair(looseNMuons[0].first,looseNMuons[0].second) );
      looseMuons.push_back( std::make_pair(looseNMuons[1].first,looseNMuons[1].second) );
    }
  }
  //
  mumuVector.clear();
  if( looseMuons.size()>1 ){
    mumuVector.push_back( looseMuons[0].second );
    mumuVector.push_back( looseMuons[1].second );
  }
  //
}
// // //
int analysisClass::LTEM_MuMu(){
  //
  if( !PreSelection("Neutr2Jet350STtuneZ") ) return 0;
  if( !DiscoverySelection("MuMu")          ) return 0;
  //
  //check for muons
  //ltemMuMu -- globally computed for every event
  //
  if( ltemMuMu.size()<2 ) return 0;
  else{
    //           Leading Muon M1            ----           Second Muon M2
    if( !muRTightCheck(ltemMuMu[0]) && !muRTightCheck(ltemMuMu[1]) ) return 11;// Second (y)
    if( !muRTightCheck(ltemMuMu[0]) &&  muRTightCheck(ltemMuMu[1]) ) return 12;//  T 1,2  2,2
    if(  muRTightCheck(ltemMuMu[0]) && !muRTightCheck(ltemMuMu[1]) ) return 21;//  L 1,1  2,1
    if(  muRTightCheck(ltemMuMu[0]) &&  muRTightCheck(ltemMuMu[1]) ) return 22;//     L    T  Lead (x)
  }
  //
}
// // //
void analysisClass::which_MuTau( std::vector<unsigned int>& mutauVector){
  //check for muon+tau
  std::vector<std::pair<double, unsigned int> > loosePMuons;
  std::vector<std::pair<double, unsigned int> > looseNMuons;
  std::vector<std::pair<double, unsigned int> > loosePTaus;
  std::vector<std::pair<double, unsigned int> > looseNTaus;
  std::vector<std::pair<double, unsigned int> > looseMuonsTaus;
  //
  for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
    if( muRisoCheck(iMuR) ){
      if( MuonCharge->at(iMuR)>0 ) loosePMuons.push_back( std::make_pair(muPtcorr(iMuR),iMuR) );
      if( MuonCharge->at(iMuR)<0 ) looseNMuons.push_back( std::make_pair(muPtcorr(iMuR),iMuR) );
    }
  }
  //
  for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
    if( tauRisoCheck(iTauR) ){
      if( HPSTauCharge->at(iTauR)>0 ) loosePTaus.push_back( std::make_pair(tauPtcorr(iTauR),iTauR) );
      if( HPSTauCharge->at(iTauR)<0 ) looseNTaus.push_back( std::make_pair(tauPtcorr(iTauR),iTauR) );
    }
  }
  //
  double STmutau=0;
  if( loosePMuons.size()>0 && loosePTaus.size()>0 ){
    STmutau=loosePMuons[0].first+loosePTaus[0].first;
    looseMuonsTaus.push_back( std::make_pair(loosePMuons[0].first,loosePMuons[0].second) );
    looseMuonsTaus.push_back( std::make_pair( loosePTaus[0].first, loosePTaus[0].second) );
  }
  if( looseNMuons.size()>0 && looseNTaus.size()>0 ){
    if( (looseNMuons[0].first+looseNTaus[0].first)>STmutau ){
      STmutau=looseNMuons[0].first+looseNTaus[0].first;
      looseMuonsTaus.clear();
      looseMuonsTaus.push_back( std::make_pair(looseNMuons[0].first,looseNMuons[0].second) );
      looseMuonsTaus.push_back( std::make_pair( looseNTaus[0].first, looseNTaus[0].second) );
    }
  }
  //
  mutauVector.clear();
  if( looseMuonsTaus.size()>1 ){
    mutauVector.push_back( looseMuonsTaus[0].second );
    mutauVector.push_back( looseMuonsTaus[1].second );
  }
  //
}
// // //
void analysisClass::which_MuTauOS( std::vector<unsigned int>& mutauVector){
  //check for muon+tau
  std::vector<std::pair<double, unsigned int> > loosePMuons;
  std::vector<std::pair<double, unsigned int> > looseNMuons;
  std::vector<std::pair<double, unsigned int> > loosePTaus;
  std::vector<std::pair<double, unsigned int> > looseNTaus;
  std::vector<std::pair<double, unsigned int> > looseMuonsTaus;
  //
  for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
    if( muRisoCheck(iMuR) ){
      if( MuonCharge->at(iMuR)>0 ) loosePMuons.push_back( std::make_pair(muPtcorr(iMuR),iMuR) );
      if( MuonCharge->at(iMuR)<0 ) looseNMuons.push_back( std::make_pair(muPtcorr(iMuR),iMuR) );
    }
  }
  //
  for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
    if( tauRisoCheck(iTauR) ){
      if( HPSTauCharge->at(iTauR)>0 ) loosePTaus.push_back( std::make_pair(tauPtcorr(iTauR),iTauR) );
      if( HPSTauCharge->at(iTauR)<0 ) looseNTaus.push_back( std::make_pair(tauPtcorr(iTauR),iTauR) );
    }
  }
  //
  double STmutau=0;
  if( loosePMuons.size()>0 && looseNTaus.size()>0 ){
    STmutau=loosePMuons[0].first+looseNTaus[0].first;
    looseMuonsTaus.push_back( std::make_pair(loosePMuons[0].first,loosePMuons[0].second) );
    looseMuonsTaus.push_back( std::make_pair( looseNTaus[0].first, looseNTaus[0].second) );
  }
  if( looseNMuons.size()>0 && loosePTaus.size()>0 ){
    if( (looseNMuons[0].first+loosePTaus[0].first)>STmutau ){
      STmutau=looseNMuons[0].first+loosePTaus[0].first;
      looseMuonsTaus.clear();
      looseMuonsTaus.push_back( std::make_pair(looseNMuons[0].first,looseNMuons[0].second) );
      looseMuonsTaus.push_back( std::make_pair( loosePTaus[0].first, loosePTaus[0].second) );
    }
  }
  //
  mutauVector.clear();
  if( looseMuonsTaus.size()>1 ){
    mutauVector.push_back( looseMuonsTaus[0].second );
    mutauVector.push_back( looseMuonsTaus[1].second );
  }
  //
}
// // //
int analysisClass::LTEM_MuTau(){
  //
  if( !PreSelection("Neutr2Jet350STtuneZ") ) return 0;
  if( !DiscoverySelection("MuTau")         ) return 0;
  //
  //check for muon+tau
  //ltemMuTau -- globally computed for every event
  //
  if( ltemMuTau.size()<2 ) return 0;
  else{
    //            Muon M            ----           Tau T
    if( !muRTightCheck(ltemMuTau[0]) && !tauRTightCheck(ltemMuTau[1]) ) return 11;// Tau (y)
    if( !muRTightCheck(ltemMuTau[0]) &&  tauRTightCheck(ltemMuTau[1]) ) return 12;//  T 1,2  2,2
    if(  muRTightCheck(ltemMuTau[0]) && !tauRTightCheck(ltemMuTau[1]) ) return 21;//  L 1,1  2,1
    if(  muRTightCheck(ltemMuTau[0]) &&  tauRTightCheck(ltemMuTau[1]) ) return 22;//     L    T  Mu (x)
  }
  //
}
// // //
bool analysisClass::isOneMuonSuperIso(){
  //
  for( unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
    if( !muRisoCheck(iMuR) ) continue;
    double pfIso = 0;
    pfIso = MuonPFIsoR04ChargedHadron->at(iMuR) + TMath::Max( 0.0 , (MuonPFIsoR04NeutralHadron->at(iMuR)+MuonPFIsoR04Photon->at(iMuR)-0.5*MuonPFIsoR04PU->at(iMuR)) );
    // Tight working point is 0.12 ( https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId )
    if( pfIso/muPtcorr(iMuR)<0.04 ) return true;
  }
  //
  return false;
}
//
double analysisClass::MuTaudeltaPzeta(){
  if( TauCounter()!=1 || MuCounter()!=1 ) return -999;
  //
  for( unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
    if( !muRisoCheck(iMuR) ) continue;
    for( unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
      if( !tauRisoCheck(iTauR) ) continue;
      return deltaPzeta( iMuR, iTauR );
    }
  }
  //
}
// // //
double analysisClass::deltaPzeta( unsigned int iMuR, unsigned int iTauR ){
  TVector3 muT; TVector3 tauT; TVector3 unitmuT; TVector3 unittauT; TVector3 unitbisecT; TVector3 MET;
  muT.SetPtEtaPhi(  muPtcorr(iMuR),    0, MuonPhi->at(iMuR)    );
  tauT.SetPtEtaPhi( tauPtcorr(iTauR), 0, HPSTauPhi->at(iTauR) );
  unitmuT=muT*(1./muT.Mag());  unittauT=tauT*(1./tauT.Mag());
  unitbisecT=(unitmuT+unittauT)*(1./((unitmuT+unittauT).Mag()));
  MET.SetPtEtaPhi( METcorr("Pt"), 0, METcorr("Phi")  );
  double pZeta;  double pZetaVis;
  pZeta    = unitbisecT.Dot( (muT+tauT+MET) );
  pZetaVis = unitbisecT.Dot( (muT+tauT)     );
  /*std::cout<<" TauMag, MuMag: "<<unitmuT.Mag()<<" "<<unittauT.Mag()<<std::endl;
  std::cout<<"  Mu-bisec: "<<muT.DeltaPhi(unitbisecT)<<std::endl;
  std::cout<<" Tau-bisec: "<<tauT.DeltaPhi(unitbisecT)<<std::endl;
  std::cout<<"    Tau-Mu: "<<tauT.DeltaPhi(muT)<<std::endl;
  std::cout<<"(pZeta-1.5*pZetaVis): "<<(pZeta-1.5*pZetaVis)<<std::endl;
  std::cout<<std::endl;*/
  return (pZeta-1.5*pZetaVis);
}
// // //
double analysisClass::Pzeta( unsigned int iMuR, unsigned int iTauR ){
  TVector3 muT; TVector3 tauT; TVector3 unitmuT; TVector3 unittauT; TVector3 unitbisecT; TVector3 MET;
  muT.SetPtEtaPhi(  muPtcorr(iMuR),    0, MuonPhi->at(iMuR)    );
  tauT.SetPtEtaPhi( tauPtcorr(iTauR), 0, HPSTauPhi->at(iTauR) );
  unitmuT=muT*(1./muT.Mag());  unittauT=tauT*(1./tauT.Mag());
  unitbisecT=(unitmuT+unittauT)*(1./((unitmuT+unittauT).Mag()));
  MET.SetPtEtaPhi( METcorr("Pt"), 0, METcorr("Phi")  );
  double pZeta; 
  pZeta    = unitbisecT.Dot( (muT+tauT+MET) );
  return pZeta;
}
// // //
double analysisClass::visPzeta( unsigned int iMuR, unsigned int iTauR ){
  TVector3 muT; TVector3 tauT; TVector3 unitmuT; TVector3 unittauT; TVector3 unitbisecT; TVector3 MET;
  muT.SetPtEtaPhi(  muPtcorr(iMuR),    0, MuonPhi->at(iMuR)    );
  tauT.SetPtEtaPhi( tauPtcorr(iTauR), 0, HPSTauPhi->at(iTauR) );
  unitmuT=muT*(1./muT.Mag());  unittauT=tauT*(1./tauT.Mag());
  unitbisecT=(unitmuT+unittauT)*(1./((unitmuT+unittauT).Mag()));
  MET.SetPtEtaPhi( METcorr("Pt"), 0, METcorr("Phi")  );
  double pZetaVis;
  pZetaVis = unitbisecT.Dot( (muT+tauT)     );
  return pZetaVis;
}
// // //
double analysisClass::LeadingMuPt( ){
  TLorentzVector LeadLep;
  LeadLep.SetPtEtaPhiM( 0, 0, 0, 0 );
  //
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if(!muRisoCheck(iMuR)       ) continue;
    if( LeadLep.Pt()<muPtcorr(iMuR)  ) LeadLep.SetPtEtaPhiM( muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0 );
  }
  return LeadLep.Pt();
}
// // //
double analysisClass::LeadingTauPt(){
  TLorentzVector LeadLep;
  LeadLep.SetPtEtaPhiM( 0, 0, 0, 0 );
  //
  for(unsigned int iTauR=0;  iTauR<HPSTauPt->size();     iTauR++){
    if(!tauRisoCheck(iTauR)       ) continue;
    if( LeadLep.Pt()<tauPtcorr(iTauR)  ) LeadLep.SetPtEtaPhiM( tauPtcorr(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
  }
  return LeadLep.Pt();
}
// // //
double analysisClass::LeadingJetPt(){
  TLorentzVector LeadJet;
  LeadJet.SetPtEtaPhiM( 0, 0, 0, 0 );
  //
  for(unsigned int iJetR=0;  iJetR<PFJetPt->size();     iJetR++){
    if(!jetRisoCheck(iJetR)       ) continue;
    if( LeadJet.Pt()<jetPtcorr(iJetR)  ) LeadJet.SetPtEtaPhiM( jetPtcorr(iJetR), PFJetEta->at(iJetR), PFJetPhi->at(iJetR), 0 );
  }
  return LeadJet.Pt();
}
// // //
double analysisClass::tauGenMatchDeltaR(unsigned int iTauR){
  TLorentzVector tauMgen;
  TLorentzVector taureco;
  if(HPSTauMatchedGenParticlePt->at(iTauR)<=0) return 9;
  tauMgen.SetPtEtaPhiM( HPSTauMatchedGenParticlePt->at(iTauR), HPSTauMatchedGenParticleEta->at(iTauR), HPSTauMatchedGenParticlePhi->at(iTauR), 0 );
  taureco.SetPtEtaPhiM( tauPtcorr(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
  return taureco.DeltaR(tauMgen);
}
// // //
double analysisClass::muGenMatchDeltaR(unsigned int iMuR){
  TLorentzVector muMgen;
  TLorentzVector mureco;
  if(MuonMatchedGenParticlePt->at(iMuR)<=0) return 9;
  muMgen.SetPtEtaPhiM( MuonMatchedGenParticlePt->at(iMuR), MuonMatchedGenParticleEta->at(iMuR), MuonMatchedGenParticlePhi->at(iMuR), 0 );
  mureco.SetPtEtaPhiM( muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0 );
  return mureco.DeltaR(muMgen);
}
// // //
bool analysisClass::isMuTau(){ // Checks for a Mu and a Tau wOUT DeltaR requirements
  //
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if(!muRisoCheck(iMuR)       ) continue;
    for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
      if(!tauRisoCheck(iTauR)) continue;
      return true;
    }
  }
  // no mu+tau found
  return false;
}
// // //
double analysisClass::MaxDiLepInvMass(){
  //
  TLorentzVector lep1;
  TLorentzVector lep2;
  double mass=0;
  //
  // mu-mu
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if( !muRisoCheck(iMuR) ) continue;
    for(unsigned int iMuR2=iMuR+1;  iMuR2<MuonPt->size();     iMuR2++){
      if( !muRisoCheck(iMuR2) ) continue;
      lep1.SetPtEtaPhiM( muPtcorr(iMuR),  MuonEta->at(iMuR),  MuonPhi->at(iMuR),  0);
      lep2.SetPtEtaPhiM( muPtcorr(iMuR2), MuonEta->at(iMuR2), MuonPhi->at(iMuR2), 0);
      if( (lep2+lep1).M() > mass ) mass=(lep2+lep1).M();
    }
  }
  //
  // el-el
  for(unsigned int iElR=0;  iElR<ElectronPt->size(); iElR++){
    if( !elRisoCheck(iElR) ) continue;
    for(unsigned int iElR2=iElR+1;  iElR2<ElectronPt->size(); iElR2++){
      if( !elRisoCheck(iElR2) ) continue;
      lep1.SetPtEtaPhiM( elPtcorr(iElR),  ElectronEta->at(iElR),  ElectronPhi->at(iElR),  0);
      lep2.SetPtEtaPhiM( elPtcorr(iElR2), ElectronEta->at(iElR2), ElectronPhi->at(iElR2), 0);
      if( (lep2+lep1).M() > mass ) mass=(lep2+lep1).M();
    }
  }
  //
  // tau-tau
  for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
    if( !tauRisoCheck(iTauR) ) continue;
    for(unsigned int iTauR2=iTauR+1; iTauR2<HPSTauPt->size(); iTauR2++){
      if( !tauRisoCheck(iTauR2) ) continue;
      lep1.SetPtEtaPhiM( tauPtcorr(iTauR),  HPSTauEta->at(iTauR),  HPSTauPhi->at(iTauR),  0);
      lep2.SetPtEtaPhiM( tauPtcorr(iTauR2), HPSTauEta->at(iTauR2), HPSTauPhi->at(iTauR2), 0);
      if( (lep2+lep1).M() > mass ) mass=(lep2+lep1).M();
    }
  }
  // mu-el
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if( !muRisoCheck(iMuR) ) continue;
    for(unsigned int iElR2=0;  iElR2<ElectronPt->size();     iElR2++){
      if( !elRisoCheck(iElR2) ) continue;
      lep1.SetPtEtaPhiM( muPtcorr(iMuR),      MuonEta->at(iMuR),      MuonPhi->at(iMuR),      0);
      lep2.SetPtEtaPhiM( elPtcorr(iElR2), ElectronEta->at(iElR2), ElectronPhi->at(iElR2), 0);
      if( (lep2+lep1).M() > mass ) mass=(lep2+lep1).M();
    }
  }
  //
  // mu-tau
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if( !muRisoCheck(iMuR) ) continue ;
    for(unsigned int iTauR2=0;  iTauR2<HPSTauPt->size();     iTauR2++){
      if( !tauRisoCheck(iTauR2) ) continue;
      lep1.SetPtEtaPhiM( muPtcorr(iMuR),     MuonEta->at(iMuR),     MuonPhi->at(iMuR),     0);
      lep2.SetPtEtaPhiM( tauPtcorr(iTauR2), HPSTauEta->at(iTauR2), HPSTauPhi->at(iTauR2), 0);
      if( (lep2+lep1).M() > mass ) mass=(lep2+lep1).M();
    }
  }
  //
  // el-tau
  for(unsigned int iElR=0;  iElR<ElectronPt->size();     iElR++){
    if( !elRisoCheck(iElR) ) continue;
    for(unsigned int iTauR2=0;  iTauR2<HPSTauPt->size();     iTauR2++){
      if( !tauRisoCheck(iTauR2) ) continue;
      lep1.SetPtEtaPhiM( elPtcorr(iElR), ElectronEta->at(iElR), ElectronPhi->at(iElR), 0);
      lep2.SetPtEtaPhiM( tauPtcorr(iTauR2), HPSTauEta->at(iTauR2), HPSTauPhi->at(iTauR2), 0);
      if( (lep2+lep1).M() > mass ) mass=(lep2+lep1).M();
    }
  }
  //
  return mass;
}
// // //
double analysisClass::MaxMuMuInvMass(){
  //
  TLorentzVector lep1;
  TLorentzVector lep2;
  double mass=0;
  //
  // mu-mu
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if( !muRisoCheck(iMuR) ) continue;
    for(unsigned int iMuR2=iMuR+1;  iMuR2<MuonPt->size();     iMuR2++){
      if( !muRisoCheck(iMuR2) ) continue;
      lep1.SetPtEtaPhiM( muPtcorr(iMuR),  MuonEta->at(iMuR),  MuonPhi->at(iMuR),  0);
      lep2.SetPtEtaPhiM( muPtcorr(iMuR2), MuonEta->at(iMuR2), MuonPhi->at(iMuR2), 0);
      if( (lep2+lep1).M() > mass ) mass=(lep2+lep1).M();
    }
  }
  //
  return mass;
}
// // //
double analysisClass::MaxMuTauInvMass(){
  //
  TLorentzVector lep1;
  TLorentzVector lep2;
  double mass=0;
  //
  // mu-tau
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if( !muRisoCheck(iMuR) ) continue;
    for(unsigned int iTauR=0;  iTauR<HPSTauPt->size();     iTauR++){
      if( !tauRisoCheck(iTauR) ) continue;
      lep1.SetPtEtaPhiM( muPtcorr(iMuR),  MuonEta->at(iMuR),  MuonPhi->at(iMuR),  0);
      lep2.SetPtEtaPhiM( tauPtcorr(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0);
      if( (lep2+lep1).M() > mass ) mass=(lep2+lep1).M();
    }
  }
  //
  return mass;
}
// // //
double analysisClass::MaxTauTauInvMass(){
  //
  TLorentzVector lep1;
  TLorentzVector lep2;
  double mass=0;
  //
  // tau-tau
  for(unsigned int iTauR=0;  iTauR<HPSTauPt->size();     iTauR++){
    if( !tauRisoCheck(iTauR) ) continue;
    for(unsigned int iTauR2=(iTauR+1);  iTauR2<HPSTauPt->size();     iTauR2++){
      if( !tauRisoCheck(iTauR2) ) continue;
      lep1.SetPtEtaPhiM( tauPtcorr(iTauR),  HPSTauEta->at(iTauR),  HPSTauPhi->at(iTauR),  0);
      lep2.SetPtEtaPhiM( tauPtcorr(iTauR2), HPSTauEta->at(iTauR2), HPSTauPhi->at(iTauR2), 0);
      if( (lep2+lep1).M() > mass ) mass=(lep2+lep1).M();
    }
  }
  //
  return mass;
}
// // //
double analysisClass::METMuTauDeltaPhi(){
  if( TauCounter()!=1 || MuCounter()!=1 ) return -999;
  //
  TVector3 Mu;  TVector3 Tau;  TVector3 MET;
  MET.SetPtEtaPhi( METcorr("Pt"), 0, METcorr("Phi") );
  for(unsigned int iTauR=0;  iTauR<HPSTauPt->size();     iTauR++){
    if( !tauRisoCheck(iTauR) ) continue;
    Tau.SetPtEtaPhi( tauPtcorr(iTauR), 0, HPSTauPhi->at(iTauR) );
  }
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if( !muRisoCheck(iMuR)   ) continue;
    Mu.SetPtEtaPhi( muPtcorr(iMuR), 0, MuonPhi->at(iMuR) );
  }
  //
  return (Mu+Tau).DeltaPhi(MET);
}
// // //
double analysisClass::METlepDeltaPhi(TString Lep, int iLep){
  TLorentzVector lep;
  lep.SetPtEtaPhiM( 0, 0, 0, 0 );
  TLorentzVector met;
  met.SetPtEtaPhiM( METcorr("Pt"), 0, METcorr("Phi"), 0 );
  //std::cout<< "analysisClass::METlepDeltaPhi  iLep: "<<iLep<<std::endl;
  //
  if( Lep != "Mu" && Lep != "Tau" ){ cout<<" WRONG LEPTON TYPE SPECIFIED!! analysisClass::METlepDeltaPhi(TString Lep, int iLep) "<<endl; return 0; }
  //
  if( iLep>-1 ){
    if( Lep=="Mu" ) lep.SetPtEtaPhiM( muPtcorr(iLep), MuonEta->at(iLep), MuonPhi->at(iLep), 0 );
    if( Lep=="Tau") lep.SetPtEtaPhiM( tauPtcorr(iLep), HPSTauEta->at(iLep), HPSTauPhi->at(iLep), 0 );
  }
  //
  if( iLep==-1 ){
    if( Lep=="Mu"){
      for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
	if(!muRisoCheck(iMuR))continue;
	if( lep.Pt()<muPtcorr(iMuR)     ) lep.SetPtEtaPhiM( muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0 );
      }
    }
    //------ ------
    if( Lep=="Tau"){
      for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
	if(!tauRisoCheck(iTauR))continue;
	if( lep.Pt()<tauPtcorr(iTauR)  ) lep.SetPtEtaPhiM( tauPtcorr(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
      }
    }
  }
  //
  return fabs(met.DeltaPhi(lep));
}
// // //
double analysisClass::ST(){ //defined as the scalar PT sum of all objetcs ( JETS, MUONS, ELECTRONS, TAUS, MET )
  //
  //require distinct isoJets, isoTaus, isoMuons, isoElectrons to eliminate double-counting 
  //
  //
  double ST=0;
  //
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if(!muRisoCheck(iMuR)       ) continue;
    ST+=muPtcorr(iMuR);
  }
  //
  for(unsigned int iElR=0;  iElR<ElectronPt->size(); iElR++){
    if(!elRisoCheck(iElR)       ) continue;
    ST+=elPtcorr(iElR);
  }
  //
  for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
    if(!tauRisoCheck(iTauR)) continue;
    ST+=tauPtcorr(iTauR);
  }
  //
  for(unsigned int iJetR=0; iJetR<PFJetPt->size();  iJetR++){
    if(!jetRisoCheck(iJetR)) continue;
    ST+=jetPtcorr(iJetR);
  }
  //
  ST+=METcorr("Pt");
  //
  return ST;
}
// // // 
double analysisClass::DeltaRminObjects(int Obj1, int Obj2){// MET=0, jet=1, el=11, mu=13, tau=15 
  TLorentzVector Obj1Cand;
  TLorentzVector Obj2Cand;
  double Pt1, Eta1, Phi1;
  double Pt2, Eta2, Phi2;
  unsigned int Obj1size;
  unsigned int Obj2size;
  double deltaRmin=99999;
  bool TwoObjFound_=false;
  //
  if(Obj1==0 ){ Obj1size=1; }
  if(Obj1==1 ){ Obj1size=PFJetPt->size(); }
  if(Obj1==11){ Obj1size=ElectronPt->size(); }
  if(Obj1==13){ Obj1size=MuonPt->size(); }
  if(Obj1==15){ Obj1size=HPSTauPt->size(); }
  //
  if(Obj2==0 ){ Obj2size=1; }
  if(Obj2==1 ){ Obj2size=PFJetPt->size(); }
  if(Obj2==11){ Obj2size=ElectronPt->size(); }
  if(Obj2==13){ Obj2size=MuonPt->size(); }
  if(Obj2==15){ Obj2size=HPSTauPt->size(); }
  //
  for(unsigned int iobj1=0; iobj1<Obj1size; iobj1++){
    if(Obj1==0 ){ Pt1=METcorr("Pt"); Eta1=0; Phi1=METcorr("Phi"); }
    if(Obj1==1 ){ if(!jetRisoCheck(iobj1)){continue;} Pt1=jetPtcorr(iobj1); Eta1= PFJetEta->at(iobj1); Phi1=PFJetPhi->at(iobj1); }
    if(Obj1==11){ if(!elRisoCheck(iobj1)){continue;} Pt1=elPtcorr(iobj1); Eta1=ElectronEta->at(iobj1); Phi1=ElectronPhi->at(iobj1); }
    if(Obj1==13){ if(!muRisoCheck(iobj1)){continue;} Pt1=muPtcorr(iobj1); Eta1=MuonEta->at(iobj1); Phi1=MuonPhi->at(iobj1); }
    if(Obj1==15){ if(!tauRisoCheck(iobj1)){continue;} Pt1=tauPtcorr(iobj1); Eta1=HPSTauEta->at(iobj1); Phi1=HPSTauPhi->at(iobj1); }
    Obj1Cand.SetPtEtaPhiM( Pt1, Eta1, Phi1, 0 );
    //
    for(unsigned int iobj2=0; iobj2<Obj2size; iobj2++){
    if(Obj2==0 ){ Pt2=METcorr("Pt"); Eta2=0; Phi2=METcorr("Phi"); }
    if(Obj2==1 ){ if(!jetRisoCheck(iobj2)){continue;} Pt2=jetPtcorr(iobj2); Eta2= PFJetEta->at(iobj2); Phi2=PFJetPhi->at(iobj2); }
    if(Obj2==11){ if(!elRisoCheck(iobj2)){continue;} Pt2=elPtcorr(iobj2); Eta2=ElectronEta->at(iobj2); Phi2=ElectronPhi->at(iobj2); }
    if(Obj2==13){ if(!muRisoCheck(iobj2)){continue;} Pt2=muPtcorr(iobj2); Eta2=MuonEta->at(iobj2); Phi2=MuonPhi->at(iobj2); }
    if(Obj2==15){ if(!tauRisoCheck(iobj2)){continue;} Pt2=tauPtcorr(iobj2); Eta2=HPSTauEta->at(iobj2); Phi2=HPSTauPhi->at(iobj2); }
    Obj2Cand.SetPtEtaPhiM( Pt2, Eta2, Phi2, 0 );
    //
    TwoObjFound_=true;
    if(Obj1Cand.DeltaR(Obj2Cand)<deltaRmin){ deltaRmin=Obj1Cand.DeltaR(Obj2Cand); }
    //
   } 
  }
  //
  if(TwoObjFound_)return deltaRmin;
  else return -1;
}
// // //
int analysisClass::TauCounter(){
  int IsoTauCtr=0;
  for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
    if(!tauRisoCheck(iTauR)){continue;} 
    IsoTauCtr++;
  }
  //
  return IsoTauCtr;
}
// // //
int analysisClass::MuCounter(){
  int IsoMuCtr=0;
  for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
    if(!muRisoCheck(iMuR)){continue;}
    IsoMuCtr++;
  }
  //
  return IsoMuCtr;
}
// // //
int analysisClass::ElCounter(){
  int IsoElCtr=0;
  for(unsigned int iElR=0; iElR<ElectronPt->size(); iElR++){
    if(!elRisoCheck(iElR)){continue;}
    IsoElCtr++;
  }
  //
  return IsoElCtr;
}
// // //
int analysisClass::JetCounter(){
  int IsoJetCtr=0;
  for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
    if(!jetRisoCheck(iJetR)){continue;}
    IsoJetCtr++;
  }
  //
  return IsoJetCtr;
}
// // //
int analysisClass::BJetCounter(){
  int IsoBJetCtr=0;
  for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
    if( !jetRisoCheck(iJetR) )  continue;
    if( !btag_jetRCheck(iJetR)           )  continue;
   IsoBJetCtr++;
  }
  //
  return IsoBJetCtr;
}
// // //
double analysisClass::METlepMT(TString Lep, int iLep){
  TLorentzVector MET;
  TLorentzVector lep;
  MET.SetPtEtaPhiM( 0 , 0 , 0 , 0 );
  lep.SetPtEtaPhiM( 0 , 0 , 0 , 0 );
  double maxPt=0;
  //std::cout<< "analysisClass::METlepMT  iLep: "<<iLep<<std::endl;
  //
  if( Lep != "Mu" && Lep != "Tau" ){ cout<<" WRONG LEPTON TYPE SPECIFIED!! analysisClass::METlepMT(TString Lep, int iLep) "<<endl; return 0; }
  //
  if( iLep>-1 ){
    if( Lep == "Mu"  ) lep.SetPtEtaPhiM( muPtcorr(iLep), MuonEta->at(iLep), MuonPhi->at(iLep), 0 );
    if( Lep == "Tau" ) lep.SetPtEtaPhiM( tauPtcorr(iLep), HPSTauEta->at(iLep), HPSTauPhi->at(iLep), 0 );
  }
  if( iLep==-1){
    if( Lep == "Mu"  ){
      maxPt=0;
      for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
	if(!muRisoCheck(iMuR))continue;
	if( muPtcorr(iMuR)>maxPt ){ maxPt=muPtcorr(iMuR);  lep.SetPtEtaPhiM( muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0 ); }
      }
    }
    //------- -------
    if( Lep == "Tau" ){ 
      maxPt=0;
      for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
	if(!tauRisoCheck(iTauR))continue;
	if( tauPtcorr(iTauR)>maxPt ){ maxPt=tauPtcorr(iTauR);  lep.SetPtEtaPhiM( tauPtcorr(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 ); }
      }
    }
  }
  //
  MET.SetPtEtaPhiM( METcorr("Pt"), 0 , METcorr("Phi") , 0 );
  double M_T=TMath::Sqrt(2*fabs(MET.Pt())*fabs(lep.Pt())*(1-TMath::Cos(MET.DeltaPhi(lep))));
  //
  return M_T;
}
// // //
double analysisClass::MuTauInZpeak(TString Pair){
  TLorentzVector mu;
  TLorentzVector mu2;
  TLorentzVector tau;
  TLorentzVector tau2;
  double Zmass = 91.188;
  double MuPt = 0;
  double TauPt = 0;
  double Dilepmass = 9999999;
  bool pairfound_ = false;
  //
  /*
  if( Pair == "ZToMuTau" ){ // MET is redistributed to Mu and Tau.. 
    TLorentzVector MET;
    MET.SetPtEtaPhiM( METcorr("Pt"), 0 , METcorr("Phi") , 0 );
    for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
      if(!muRisoCheck(iMuR)){continue;}
      mu.SetPtEtaPhiM( muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0);
      MuPt = MET.Pt()*TMath::Cos(mu.DeltaPhi(MET)) + muPtcorr(iMuR); //adding projected MET on transverse plane Muon.
      mu.SetPtEtaPhiM( MuPt , MuonEta->at(iMuR), MuonPhi->at(iMuR), 0);
      for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
	if(!tauRisoCheck(iTauR)){continue;}
	tau.SetPtEtaPhiM( tauPtcorr(iTauR), HPSTauEta->at(iTauR) , HPSTauPhi->at(iTauR), 0); 
	TauPt = MET.Pt()*TMath::Cos(tau.DeltaPhi(MET)) + tauPtcorr(iTauR); //adding projected MET on transverse plane Tau.
	tau.SetPtEtaPhiM( TauPt, HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0);
	pairfound_=true;
	if( fabs((mu+tau).M()-Zmass) < fabs(Dilepmass-Zmass) ) Dilepmass=(mu+tau).M();
      }
    }
  }
  */
  //
  if( Pair == "ZToMuTau" ){ // MET is added to TauPt..         mu <-- Z --> Tau (Tau_vis, neutrino :: collinear)
    TLorentzVector MET;
    MET.SetPtEtaPhiM( METcorr("Pt"), 0 , METcorr("Phi") , 0 );
    for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
      if(!muRisoCheck(iMuR)){continue;}
      mu.SetPtEtaPhiM( muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0);
      //MuPt = MET.Pt()*TMath::Cos(mu.DeltaPhi(MET)) + muPtcorr(iMuR); //adding projected MET on transverse plane Muon.
      // mu.SetPtEtaPhiM( MuPt , MuonEta->at(iMuR), MuonPhi->at(iMuR), 0);
      for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
	if(!tauRisoCheck(iTauR)){continue;}
	tau.SetPtEtaPhiM( tauPtcorr(iTauR), HPSTauEta->at(iTauR) , HPSTauPhi->at(iTauR), 0); 
	//TauPt = MET.Pt()*TMath::Cos(tau.DeltaPhi(MET)) + tauPtcorr(iTauR); //adding projected MET on transverse plane Tau.
	TauPt = MET.Pt()*TMath::Cos(tau.DeltaPhi(MET)) + tauPtcorr(iTauR); //adding MET to TauPt.
	tau.SetPtEtaPhiM( TauPt, HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0);
	pairfound_=true;
	if( fabs((mu+tau).M()-Zmass) < fabs(Dilepmass-Zmass) ) Dilepmass=(mu+tau).M();
      }
    }
  }
  //
  if( Pair == "ZToMuMu" ){
    for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
      if(!muRisoCheck(iMuR)){continue;}
      mu.SetPtEtaPhiM( muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0);
      for(unsigned int iMuR2=iMuR+1; iMuR2<MuonPt->size(); iMuR2++){
	if(!muRisoCheck(iMuR2)){continue;}
	mu2.SetPtEtaPhiM( muPtcorr(iMuR2), MuonEta->at(iMuR2), MuonPhi->at(iMuR2), 0);
	pairfound_=true;
	if( fabs((mu+mu2).M()-Zmass) < fabs(Dilepmass-Zmass) ) Dilepmass=(mu+mu2).M();
      }
    }
  }
  //
  if( Pair == "ZToTauTau" ){
    for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
      if(!tauRisoCheck(iTauR)){continue;}
      tau.SetPtEtaPhiM( tauPtcorr(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0);
      for(unsigned int iTauR2=iTauR+1; iTauR2<HPSTauPt->size(); iTauR2++){
	if(!tauRisoCheck(iTauR2)){continue;}
	tau2.SetPtEtaPhiM( tauPtcorr(iTauR2), HPSTauEta->at(iTauR2), HPSTauPhi->at(iTauR2), 0);
	pairfound_=true;
	if( fabs((tau+tau2).M()-Zmass) < fabs(Dilepmass-Zmass) ) Dilepmass=(tau+tau2).M();
      }
    }
  }
  //
  if( pairfound_ ) return Dilepmass;
  else return 0;
}
// // //
int analysisClass::RecoSignalType(){
  int recoSignalType_=0;
  int AbsSumQ=0;
  int SumQ=0;
  //
  int TotalLepCtr=0;
  int IsoTauCtr=0;
  for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
    if(!tauRisoCheck(iTauR)){continue;}
    SumQ+=HPSTauCharge->at(iTauR);
    IsoTauCtr++;
    TotalLepCtr++;
  }
  int IsoMuCtr=0;
  for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
    if(!muRisoCheck(iMuR)){continue;}
    SumQ+=MuonCharge->at(iMuR);
    IsoMuCtr++;
    TotalLepCtr++;
  }
  int IsoElCtr=0;
  for(unsigned int iElR=0; iElR<ElectronPt->size(); iElR++){
    if(!elRisoCheck(iElR)){continue;}
    SumQ+=ElectronCharge->at(iElR);
    IsoElCtr++;
    TotalLepCtr++;
  }
  AbsSumQ=abs(SumQ);
  //
  if(TotalLepCtr>9){cout<< "WARNING!! Too many leptons found - Check RecoSignalType() "<<endl; return -999;}
  //
  recoSignalType_ = (1000*TotalLepCtr) + (100*IsoTauCtr) + (10*IsoMuCtr) + (IsoElCtr);
  // 2: --|++
  // 3: ++-|+--
  // 4: ++--
  if( (AbsSumQ==2 && TotalLepCtr==2) || (AbsSumQ==1 && TotalLepCtr==3) || (AbsSumQ==0 && TotalLepCtr==4) ) return recoSignalType_;// at least one pair of ssdl
  else return ((-1)*recoSignalType_); 
}
// // //
bool analysisClass::isMu15Tau20_MigrationSkim(){
  //
  bool isTau=false;
  for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
    if( tauRMigrationCheck(iTauR) ){ isTau==true; break; }
  }
  if( !isTau ) return false;
  //
  for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
    if( muRMigrationCheck(iMuR)        ) return true;
  }
  //
  return false;
}
// // //
bool analysisClass::isRecoMuMatchedTO( double genPt , double genEta , double genPhi , double dRcut ){
  double  deltaR=999;
  TLorentzVector RecoMu;
  TLorentzVector GenMu;
  GenMu.SetPtEtaPhiM( genPt, genEta, genPhi, 0 );
  //
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if( !muRCheck( iMuR )    ) continue;
    RecoMu.SetPtEtaPhiM( muPtcorr(iMuR) , MuonEta->at(iMuR) , MuonPhi->at(iMuR) , 0 );
    if( RecoMu.DeltaR(GenMu)<deltaR ) deltaR=RecoMu.DeltaR(GenMu);
  }
  //
  if( deltaR<dRcut ) return true;
  return false;
}
// // //
bool analysisClass::isRecoTauMatchedTO( double genPt , double genEta , double genPhi , double dRcut ){
  double  deltaR=999;
  TLorentzVector RecoTau;
  TLorentzVector GenTau;
  GenTau.SetPtEtaPhiM( genPt, genEta, genPhi, 0 );
  //
  for(unsigned int iTauR=0;  iTauR<HPSTauPt->size();     iTauR++){
    if( !tauRCheck( iTauR )    ) continue;
    RecoTau.SetPtEtaPhiM( tauPtcorr(iTauR) , HPSTauEta->at(iTauR) , HPSTauPhi->at(iTauR) , 0 );
    if( fabs(RecoTau.DeltaR(GenTau))<deltaR ) deltaR=fabs(RecoTau.DeltaR(GenTau));
  }
  //
  if( deltaR<dRcut ) return true;
  return false;
}
// // //
