/////////////////////////////////////////////////////////
// Gen-Reco TOP tools
//
// H.S  - Dec 2012
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
bool analysisClass::isRecoTauFake( unsigned int iTauR ){
  TLorentzVector taugen, taureco;
  taureco.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
  double DeltaRminTau=9999;
  for( unsigned int iTauT=0; iTauT<GenWTauPt->size(); iTauT++){
    if( GenWTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
    taugen.SetPtEtaPhiM( GenWTauPt->at(iTauT),     GenWTauEta->at(iTauT),   GenWTauPhi->at(iTauT), 0 );
    //if( GenWTauPdgId->at(iTauT)>0 ) charge=-1;
    //if( GenWTauPdgId->at(iTauT)<0 ) charge=+1;
    //if( fabs(taureco.DeltaR(taugen))<fabs(DeltaRminTau) ) DeltaRminTau=fabs(taureco.DeltaR(taugen))*charge;
    //if( fabs(DeltaRminTau)<0.15 ) isProbeTauFake_=false;
    if( DeltaRminTau>taureco.DeltaR(taugen) ) DeltaRminTau=taureco.DeltaR(taugen);
  }
  for( unsigned int iTauT=0; iTauT<GenZTauPt->size(); iTauT++){
    if( GenZTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
    taugen.SetPtEtaPhiM( GenZTauPt->at(iTauT),     GenZTauEta->at(iTauT),   GenZTauPhi->at(iTauT), 0 );
    //if( GenZTauPdgId->at(iTauT)>0 ) charge=-1;
    //if( GenZTauPdgId->at(iTauT)<0 ) charge=+1;
    //if( fabs(taureco.DeltaR(taugen))<fabs(DeltaRminTau) ) DeltaRminTau=fabs(taureco.DeltaR(taugen))*charge;
    //if( fabs(DeltaRminTau)<0.15 ) isProbeTauFake_=false;
    if( DeltaRminTau>taureco.DeltaR(taugen) ) DeltaRminTau=taureco.DeltaR(taugen);
  }
  /* // this part doesnt quite work..
  for( unsigned int iTauT=0; iTauT<GenLQTauTauPt->size(); iTauT++){
    if( GenLQTauTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
    taugen.SetPtEtaPhiM( GenLQTauTauTauVisiblePt->at(iTauT),     GenLQTauTauTauVisibleEta->at(iTauT),   GenLQTauTauTauVisiblePhi->at(iTauT), 0 );
    if( DeltaRminTau>taureco.DeltaR(taugen) ) DeltaRminTau=taureco.DeltaR(taugen);
  }
  */
  //
  if( DeltaRminTau<0.15 ) return false;
  return true;
}
// // //
bool analysisClass::isRecoMuonFake( unsigned int iMuR ){
  TLorentzVector mugen, mureco;
  mureco.SetPtEtaPhiM( MuonPt->at(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0 );
  double DeltaRminMuon=9999;
  //
  //Check RecoMu  ( W )
  for( unsigned int iMuT=0; iMuT<GenWMuPt->size(); iMuT++){
    mugen.SetPtEtaPhiM( GenWMuPt->at(iMuT),     GenWMuEta->at(iMuT),   GenWMuPhi->at(iMuT), 0 );
    if( mugen.DeltaR(mureco)<DeltaRminMuon ) DeltaRminMuon=mugen.DeltaR(mureco);
  }
  //Check RecoMu  ( Z )
  for( unsigned int iMuT=0; iMuT<GenZMuPt->size(); iMuT++){
    mugen.SetPtEtaPhiM( GenZMuPt->at(iMuT),     GenZMuEta->at(iMuT),   GenZMuPhi->at(iMuT), 0 );
    if( mugen.DeltaR(mureco)<DeltaRminMuon ) DeltaRminMuon=mugen.DeltaR(mureco);
  }
  //
  if( DeltaRminMuon<0.15 ) return false;
  return true;
}
// // //




/*
for( unsigned int iMuT=0; iMuT<GenWMuPt->size(); iMuT++){
  mugen.SetPtEtaPhiM( GenWMuPt->at(iMuT),     GenWMuEta->at(iMuT),   GenWMuPhi->at(iMuT), 0 );
  if( GenWMuPdgId->at(iMuT)>0 ) charge=-1;
  if( GenWMuPdgId->at(iMuT)<0 ) charge=+1;
  if( fabs(taureco.DeltaR(mugen))<fabs(DeltaRminMu) ) DeltaRminMu=fabs(taureco.DeltaR(mugen))*charge;
 }
for( unsigned int iMuT=0; iMuT<GenZMuPt->size(); iMuT++){
  mugen.SetPtEtaPhiM( GenZMuPt->at(iMuT),     GenZMuEta->at(iMuT),   GenZMuPhi->at(iMuT), 0 );
  if( GenZMuPdgId->at(iMuT)>0 ) charge=-1;
  if( GenZMuPdgId->at(iMuT)<0 ) charge=+1;
  if( fabs(taureco.DeltaR(mugen))<fabs(DeltaRminMu) ) DeltaRminMu=fabs(taureco.DeltaR(mugen))*charge;
 }
//
for( unsigned int iElT=0; iElT<GenWElectronPt->size(); iElT++){
  elgen.SetPtEtaPhiM( GenWElectronPt->at(iElT),     GenWElectronEta->at(iElT),   GenWElectronPhi->at(iElT), 0 );
  if( GenWElectronPdgId->at(iElT)>0 ) charge=-1;
  if( GenWElectronPdgId->at(iElT)<0 ) charge=+1;
  if( fabs(taureco.DeltaR(elgen))<fabs(DeltaRminEl) ) DeltaRminEl=fabs(taureco.DeltaR(elgen))*charge;
 }
*/
// // //
/*
double analysisClass::tauGenMatchDeltaR_TOP(unsigned int iTauR){
  double dR=9;
  int GenQmatch=0;
  int GenQ=0;
  TLorentzVector tauMgen;
  TLorentzVector taureco;
  tauMgen.SetPtEtaPhiM( 0 , 0 , 0 , 0 );
  taureco.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
  //
  for( unsigned int iTauT=0; iTauT<GenWTauPt->size(); iTauT++){
    if( GenWTauTauDecayMode->at(iTauT)<3 ) continue;//skip leptonic taus
    tauMgen.SetPtEtaPhiM( GenWTauPt->at(iTauT), GenWTauEta->at(iTauT), GenWTauPhi->at(iTauT), 0 );
    if( GenWTauPdgId->at(iTauT)==+15 )GenQ=-1;
    if( GenWTauPdgId->at(iTauT)==-15 )GenQ=+1;
    if( taureco.DeltaR(tauMgen)<dR ){ dR=taureco.DeltaR(tauMgen); GenQmatch=GenQ; }
  }
  //
  if( HPSTauCharge->at(iTauR)!=GenQmatch ) dR=(-1)*dR;
  return dR;
}
// // //
double analysisClass::elGenMatchDeltaR_TOP(unsigned int iElR){
  double dR=9;
  int GenQmatch=0;
  int GenQ=0;
  TLorentzVector elMgen;
  TLorentzVector elreco;
  elMgen.SetPtEtaPhiM( 0 , 0 , 0 , 0 );
  elreco.SetPtEtaPhiM( ElectronPt->at(iElR), ElectronEta->at(iElR), ElectronPhi->at(iElR), 0 );
  //
  for( unsigned int iElT=0; iElT<GenWElectronPt->size(); iElT++){
    elMgen.SetPtEtaPhiM( GenWElectronPt->at(iElT), GenWElectronEta->at(iElT), GenWElectronPhi->at(iElT), 0 );
    if( GenWElectronPdgId->at(iElT)==+11 )GenQ=-1;
    if( GenWElectronPdgId->at(iElT)==-11 )GenQ=+1;
    if( elreco.DeltaR(elMgen)<dR ){ dR=elreco.DeltaR(elMgen); GenQmatch=GenQ; }
  }
  //
  if( ElectronCharge->at(iElR)!=GenQmatch ) dR=(-1)*dR;
  return dR;
}
// // //
double analysisClass::muGenMatchDeltaR_TOP(unsigned int iMuR){
  double dR=9;
  int GenQmatch=0;
  int GenQ=0;
  TLorentzVector muMgen;
  TLorentzVector mureco;
  muMgen.SetPtEtaPhiM( 0 , 0 , 0 , 0 );
  mureco.SetPtEtaPhiM( MuonPt->at(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0 );
  //
  for( unsigned int iMuT=0; iMuT<GenWMuPt->size(); iMuT++){
    muMgen.SetPtEtaPhiM( GenWMuPt->at(iMuT), GenWMuEta->at(iMuT), GenWMuPhi->at(iMuT), 0 );
    if( GenWMuPdgId->at(iMuT)==+13 )GenQ=-1;
    if( GenWMuPdgId->at(iMuT)==-13 )GenQ=+1;
    if( mureco.DeltaR(muMgen)<dR ){ dR=mureco.DeltaR(muMgen); GenQmatch=GenQ; }
  }
  //
  if( MuonCharge->at(iMuR)!=GenQmatch ) dR=(-1)*dR;
  return dR;
}
// // //
bool analysisClass::isFakeTau_TOP(){
  //
  for(unsigned int iTauR=0;  iTauR<HPSTauPt->size();     iTauR++){
    if( !tauRisoCheck(iTauR) ) continue;  
    //---------
    if( tauGenMatchDeltaR_TOP(iTauR)>=0.3 || tauGenMatchDeltaR_TOP(iTauR)<0 ) return true;
    //---------
  }
  //
  return false;// if no reco tau is missed wrt gen taus -> no fake taus
  //
}
// // //
bool analysisClass::isFakeEl_TOP(){
  //
  for(unsigned int iElR=0;  iElR<ElectronPt->size();     iElR++){
    if( !elRisoCheck(iElR) ) continue;  
    //---------
    if( elGenMatchDeltaR_TOP(iElR)>=0.3 || elGenMatchDeltaR_TOP(iElR)<0 ) return true;
    //---------
  }
  //
  return false;// if no reco el is missed wrt gen els -> no fake els
  //
}
// // //
bool analysisClass::isFakeMu_TOP(){
  //
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if( !muRisoCheck(iMuR) ) continue;  
    //---------
    if( muGenMatchDeltaR_TOP(iMuR)>=0.3 || muGenMatchDeltaR_TOP(iMuR)<0 ) return true;
    //---------
  }
  //
  return false;// if no reco mu is missed wrt gen mus -> no fake mus
  //
}
*/
// // //
