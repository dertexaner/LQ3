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
  //Check RecoTau (LQ3->Tau->Had_Tau)
  for( unsigned int iTauT=0; iTauT<GenLQTauTauPt->size(); iTauT++){
    if( GenLQTauTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
    GenTau.SetPtEtaPhiM( GenLQTauTauTauVisiblePt->at(iTauT), GenLQTauTauTauVisibleEta->at(iTauT), GenLQTauTauTauVisiblePhi->at(iTauT), 0 );
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
  //Check RecoTau (LQ3->Tau->Had_Tau)
  for( unsigned int iTauT=0; iTauT<GenLQTauTauPt->size(); iTauT++){
    if( GenLQTauTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
    GenTau.SetPtEtaPhiM( GenLQTauTauTauVisiblePt->at(iTauT), GenLQTauTauTauVisibleEta->at(iTauT), GenLQTauTauTauVisiblePhi->at(iTauT), 0 );
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
  //Check RecoTau (LQ3->Tau->Had_Tau)
  for( unsigned int iTauT=0; iTauT<GenLQTauTauPt->size(); iTauT++){
    if( GenLQTauTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
    GenTau.SetPtEtaPhiM( GenLQTauTauTauVisiblePt->at(iTauT), GenLQTauTauTauVisibleEta->at(iTauT), GenLQTauTauTauVisiblePhi->at(iTauT), 0 );
    if( GenTau.DeltaR(RecoTau)<0.15 ){
      if( HPSTauCharge->at(iTauR)*GenLQTauTauPdgId->at(iTauT)>0 ) isTauChargeFlip_ = true;
    }
  }
  //
  return isTauChargeFlip_;
}
// // //
//FOR GEN-RECO MATCHING IN SIGNAL:
/*
       //Check RecoMu  (LQ3->Tau->Muon)
       for( unsigned int iMuT=0; iMuT<GenLQTauMuonPt->size(); iMuT++){
         GenMu.SetPtEtaPhiM( GenLQTauMuonPt->at((iMuT),     GenLQTauMuonEta->at(iMuT),   GenLQTauMuonPhi->at(iMuT), 0 );
         if( GenMu.DeltaR(RecoMu)<0.15 ) isRecoMuPrompt_ = true;
       }
       //Check RecoMu  (LQ3->Top->Muon)
       for( unsigned int iMuT=0; iMuT<GenLQTopMuonPt->size(); iMuT++){
         GenMu.SetPtEtaPhiM( GenLQTopMuonPt->at((iMuT),     GenLQTopMuonEta->at(iMuT),   GenLQTopMuonPhi->at(iMuT), 0 );
         if( GenMu.DeltaR(RecoMu)<0.15 ) isRecoMuPrompt_ = true;
       }
       //Check RecoTau (LQ3->Tau->Had_Tau)
       for( unsigned int iTauT=0; iTauT<GenLQTauTauPt->size(); iTauT++){
         if( GenLQTauTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
         GenTau.SetPtEtaPhiM( GenLQTauTauTauVisiblePt->at(iTauT), GenLQTauTauTauVisibleEta->at(iTauT), GenLQTauTauTauVisiblePhi->at(iTauT), 0 );
         if( GenTau.DeltaR(RecoTau)<0.15 ){
           isRecoTauPrompt_ = true;
           if( HPSTauCharge->at(ltemMuTau[1])*GenLQTauTauPdgId->at(iTauT)>0 ) isTauChargeFlip_ = true;
         }
       }
       //Check RecoTau (LQ3->Top->Had_Tau)
       for( unsigned int iTauT=0; iTauT<GenLQTopTauPt->size(); iTauT++){
         if( GenLQTopTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
         GenTau.SetPtEtaPhiM( GenLQTopTauTauVisiblePt->at(iTauT), GenLQTopTauTauVisibleEta->at(iTauT), GenLQTopTauTauVisiblePhi->at(iTauT), 0 );
         if( GenTau.DeltaR(RecoTau)<0.15 ){
           isRecoTauPrompt_ = true;
           if( HPSTauCharge->at(ltemMuTau[1])*GenLQTopTauPdgId->at(iTauT)>0 ) isTauChargeFlip_ = true;
         }
       }
*/


