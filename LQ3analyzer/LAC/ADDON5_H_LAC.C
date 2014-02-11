/////////////////////////////////////////////////////////
// Gen-Reco tools
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
bool analysisClass::isRecoMuPrompt( unsigned int iMuR ){
  TLorentzVector RecoMu, GenMu;
  bool isRecoMuPrompt_ = false;
  RecoMu.SetPtEtaPhiM( MuonPt->at(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0 );
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
  //Check RecoMu  ( H )
  for( unsigned int iMuT=0; iMuT<GenHMuPt->size(); iMuT++){
    GenMu.SetPtEtaPhiM( GenHMuPt->at(iMuT),     GenHMuEta->at(iMuT),   GenHMuPhi->at(iMuT), 0 );
    if( GenMu.DeltaR(RecoMu)<0.15 ) isRecoMuPrompt_ = true;
  }
  //
  return isRecoMuPrompt_;
}
// // //
bool analysisClass::isRecoTauPrompt( unsigned int iTauR ){
  TLorentzVector RecoTau, GenTau;
  bool isRecoTauPrompt_ = false;
  RecoTau.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
  //Check RecoTau ( W )
  for( unsigned int iTauT=0; iTauT<GenWTauPt->size(); iTauT++){
    if( GenWTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
    GenTau.SetPtEtaPhiM( GenWTauTauVisiblePt->at(iTauT),     GenWTauTauVisibleEta->at(iTauT),   GenWTauTauVisiblePhi->at(iTauT), 0 );
    if( GenTau.DeltaR(RecoTau)<0.15 ){
      isRecoTauPrompt_ = true;
    }
  }
  //Check RecoTau ( Z )
  for( unsigned int iTauT=0; iTauT<GenZTauPt->size(); iTauT++){
    if( GenZTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
    GenTau.SetPtEtaPhiM( GenZTauTauVisiblePt->at(iTauT),     GenZTauTauVisibleEta->at(iTauT),   GenZTauTauVisiblePhi->at(iTauT), 0 );
    if( GenTau.DeltaR(RecoTau)<0.15 ){
      isRecoTauPrompt_ = true;
    }
  }
  //Check RecoTau ( H )
  for( unsigned int iTauT=0; iTauT<GenHTauPt->size(); iTauT++){
    if( GenHTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
    GenTau.SetPtEtaPhiM( GenHTauTauVisiblePt->at(iTauT),     GenHTauTauVisibleEta->at(iTauT),   GenHTauTauVisiblePhi->at(iTauT), 0 );
    if( GenTau.DeltaR(RecoTau)<0.15 ){
      isRecoTauPrompt_ = true;
    }
  }
  //
  return isRecoTauPrompt_;
}
// // //
double analysisClass::getRecoTauGenPt( unsigned int iTauR ){
  TLorentzVector RecoTau, GenTau;
  double genTauPT_=0;
  RecoTau.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
  //Check RecoTau ( W )
  for( unsigned int iTauT=0; iTauT<GenWTauPt->size(); iTauT++){
    if( GenWTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
    GenTau.SetPtEtaPhiM( GenWTauTauVisiblePt->at(iTauT),     GenWTauTauVisibleEta->at(iTauT),   GenWTauTauVisiblePhi->at(iTauT), 0 );
    if( GenTau.DeltaR(RecoTau)<0.15 ){
      genTauPT_ = GenTau.Pt();
    }
  }
  //Check RecoTau ( Z )
  for( unsigned int iTauT=0; iTauT<GenZTauPt->size(); iTauT++){
    if( GenZTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
    GenTau.SetPtEtaPhiM( GenZTauTauVisiblePt->at(iTauT),     GenZTauTauVisibleEta->at(iTauT),   GenZTauTauVisiblePhi->at(iTauT), 0 );
    if( GenTau.DeltaR(RecoTau)<0.15 ){
      genTauPT_ = GenTau.Pt();
    }
  }
  //Check RecoTau ( H )
  for( unsigned int iTauT=0; iTauT<GenHTauPt->size(); iTauT++){
    if( GenHTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
    GenTau.SetPtEtaPhiM( GenHTauTauVisiblePt->at(iTauT),     GenHTauTauVisibleEta->at(iTauT),   GenHTauTauVisiblePhi->at(iTauT), 0 );
    if( GenTau.DeltaR(RecoTau)<0.15 ){
      genTauPT_ = GenTau.Pt();
    }
  }
  //
  return genTauPT_;
}
// // //
// // //
// GenWTauPdgId = 15 for tau charge -1  ,  Thus:
// --                       RecoQ  pdg  RecoQ GenQ
// RecoCharge x PdgID > 0  -> +1 x  15 -> +1 x -1 -> FLIP
// RecoCharge x PdgID > 0  -> -1 x -15 -> -1 x +1 -> FLIP
// --
// RecoCharge x PdgID < 0  -> +1 x -15 -> +1 x +1 -> NO-FLIP
// RecoCharge x PdgID < 0  -> -1 x  15 -> -1 x -1 -> NO-FLIP
bool analysisClass::isRecoTauChargeFlip( unsigned int iTauR ){ //has to be prompt
  TLorentzVector RecoTau, GenTau;
  bool isTauChargeFlip_ = false;
  RecoTau.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
  //Check RecoTau ( W )
  for( unsigned int iTauT=0; iTauT<GenWTauPt->size(); iTauT++){
    if( GenWTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
    GenTau.SetPtEtaPhiM( GenWTauTauVisiblePt->at(iTauT),     GenWTauTauVisibleEta->at(iTauT),   GenWTauTauVisiblePhi->at(iTauT), 0 );
    if( GenTau.DeltaR(RecoTau)<0.15 ){
      if( HPSTauCharge->at(iTauR)*GenWTauPdgId->at(iTauT)>0 ) isTauChargeFlip_ = true;
    }
  }
  //Check RecoTau ( Z )
  for( unsigned int iTauT=0; iTauT<GenZTauPt->size(); iTauT++){
    if( GenZTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
    GenTau.SetPtEtaPhiM( GenZTauTauVisiblePt->at(iTauT),     GenZTauTauVisibleEta->at(iTauT),   GenZTauTauVisiblePhi->at(iTauT), 0 );
    if( GenTau.DeltaR(RecoTau)<0.15 ){
      if( HPSTauCharge->at(iTauR)*GenZTauPdgId->at(iTauT)>0 ) isTauChargeFlip_ = true;
    }
  }
  //Check RecoTau ( H )
  for( unsigned int iTauT=0; iTauT<GenHTauPt->size(); iTauT++){
    if( GenHTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
    GenTau.SetPtEtaPhiM( GenHTauTauVisiblePt->at(iTauT),     GenHTauTauVisibleEta->at(iTauT),   GenHTauTauVisiblePhi->at(iTauT), 0 );
    if( GenTau.DeltaR(RecoTau)<0.15 ){
      if( HPSTauCharge->at(iTauR)*GenHTauPdgId->at(iTauT)>0 ) isTauChargeFlip_ = true;
    }
  }
  //
  return isTauChargeFlip_;
}
// // //


