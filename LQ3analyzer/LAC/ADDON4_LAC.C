/////////////////////////////////////////////////////////
// Gen-Reco LQ tools
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
double analysisClass::tauGenMatchDeltaR_LQ(unsigned int iTauR){
  double dR=9;
  int GenQmatch=0;
  int GenQ=0;
  TLorentzVector tauMgen;
  TLorentzVector taureco;
  tauMgen.SetPtEtaPhiM( 0 , 0 , 0 , 0 );
  taureco.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
  //
  for( unsigned int iTauT=0; iTauT<GenLQTopTauPt->size(); iTauT++){
    if( GenLQTopTauTauDecayMode->at(iTauT)<3 ) continue;//skip leptonic taus
    tauMgen.SetPtEtaPhiM( GenLQTopTauPt->at(iTauT), GenLQTopTauEta->at(iTauT), GenLQTopTauPhi->at(iTauT), 0 );
    if( GenLQTopTauPdgId->at(iTauT)==+15 )GenQ=-1;
    if( GenLQTopTauPdgId->at(iTauT)==-15 )GenQ=+1;
    if( taureco.DeltaR(tauMgen)<dR ){ dR=taureco.DeltaR(tauMgen); GenQmatch=GenQ; }
  }
  for( unsigned int iTauT=0; iTauT<GenLQTauPt->size(); iTauT++){
    if( GenLQTauTauDecayMode->at(iTauT)<3 ) continue;//skip leptonic taus
    tauMgen.SetPtEtaPhiM( GenLQTauPt->at(iTauT), GenLQTauEta->at(iTauT), GenLQTauPhi->at(iTauT), 0 );
    if( GenLQTauPdgId->at(iTauT)==+15 )GenQ=-1;
    if( GenLQTauPdgId->at(iTauT)==-15 )GenQ=+1;
    if( taureco.DeltaR(tauMgen)<dR ){ dR=taureco.DeltaR(tauMgen); GenQmatch=GenQ; }
  }
  //
  if( HPSTauCharge->at(iTauR)!=GenQmatch ) dR=(-1)*dR;
  return dR;
}
// // //
double analysisClass::elGenMatchDeltaR_LQ(unsigned int iElR){
  double dR=9;
  int GenQmatch=0;
  int GenQ=0;
  TLorentzVector elMgen;
  TLorentzVector elreco;
  elMgen.SetPtEtaPhiM( 0 , 0 , 0 , 0 );
  elreco.SetPtEtaPhiM( ElectronPt->at(iElR), ElectronEta->at(iElR), ElectronPhi->at(iElR), 0 );
  //
  for( unsigned int iElT=0; iElT<GenLQTopXElectronPt->size(); iElT++){
    elMgen.SetPtEtaPhiM( GenLQTopXElectronPt->at(iElT), GenLQTopXElectronEta->at(iElT), GenLQTopXElectronPhi->at(iElT), 0 );
    if( GenLQTopXElectronPdgId->at(iElT)==+11 )GenQ=-1;
    if( GenLQTopXElectronPdgId->at(iElT)==-11 )GenQ=+1;
    if( elreco.DeltaR(elMgen)<dR ){ dR=elreco.DeltaR(elMgen); GenQmatch=GenQ; }
  }
  for( unsigned int iElT=0; iElT<GenLQTauElectronPt->size(); iElT++){
    elMgen.SetPtEtaPhiM( GenLQTauElectronPt->at(iElT), GenLQTauElectronEta->at(iElT), GenLQTauElectronPhi->at(iElT), 0 );
    if( GenLQTauElectronPdgId->at(iElT)==+11 )GenQ=-1;
    if( GenLQTauElectronPdgId->at(iElT)==-11 )GenQ=+1;
    if( elreco.DeltaR(elMgen)<dR ){ dR=elreco.DeltaR(elMgen); GenQmatch=GenQ; }
  }
  //
  if( ElectronCharge->at(iElR)!=GenQmatch ) dR=(-1)*dR;
  return dR;
}
// // //
double analysisClass::muGenMatchDeltaR_LQ(unsigned int iMuR){
  double dR=9;
  int GenQmatch=0;
  int GenQ=0;
  TLorentzVector muMgen;
  TLorentzVector mureco;
  muMgen.SetPtEtaPhiM( 0 , 0 , 0 , 0 );
  mureco.SetPtEtaPhiM( MuonPt->at(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0 );
  //
  for( unsigned int iMuT=0; iMuT<GenLQTopXMuonPt->size(); iMuT++){
    muMgen.SetPtEtaPhiM( GenLQTopXMuonPt->at(iMuT), GenLQTopXMuonEta->at(iMuT), GenLQTopXMuonPhi->at(iMuT), 0 );
    if( GenLQTopXMuonPdgId->at(iMuT)==+13 )GenQ=-1;
    if( GenLQTopXMuonPdgId->at(iMuT)==-13 )GenQ=+1;
    if( mureco.DeltaR(muMgen)<dR ){ dR=mureco.DeltaR(muMgen); GenQmatch=GenQ; }
  }
  for( unsigned int iMuT=0; iMuT<GenLQTauMuonPt->size(); iMuT++){
    muMgen.SetPtEtaPhiM( GenLQTauMuonPt->at(iMuT), GenLQTauMuonEta->at(iMuT), GenLQTauMuonPhi->at(iMuT), 0 );
    if( GenLQTauMuonPdgId->at(iMuT)==+13 )GenQ=-1;
    if( GenLQTauMuonPdgId->at(iMuT)==-13 )GenQ=+1;
    if( mureco.DeltaR(muMgen)<dR ){ dR=mureco.DeltaR(muMgen); GenQmatch=GenQ; }
  }
  //
  if( MuonCharge->at(iMuR)!=GenQmatch ) dR=(-1)*dR;
  return dR;
}
// // //
bool analysisClass::isFakeTau_LQ(){
  //
  for(unsigned int iTauR=0;  iTauR<HPSTauPt->size();     iTauR++){
    if( !tauRisoCheck(iTauR) ) continue;  
    //---------
    if( tauGenMatchDeltaR_LQ(iTauR)>=0.3 || tauGenMatchDeltaR_LQ(iTauR)<0 ) return true;
    //---------
  }
  //
  return false;// if no reco tau is missed wrt gen taus -> no fake taus
  //
}
// // //
bool analysisClass::isFakeEl_LQ(){
  //
  for(unsigned int iElR=0;  iElR<ElectronPt->size();     iElR++){
    if( !elRisoCheck(iElR) ) continue;  
    //---------
    if( elGenMatchDeltaR_LQ(iElR)>=0.3 || elGenMatchDeltaR_LQ(iElR)<0 ) return true;
    //---------
  }
  //
  return false;// if no reco el is missed wrt gen els -> no fake els
  //
}
// // //
bool analysisClass::isFakeMu_LQ(){
  //
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if( !muRisoCheck(iMuR) ) continue;  
    //---------
    if( muGenMatchDeltaR_LQ(iMuR)>=0.3 || muGenMatchDeltaR_LQ(iMuR)<0 ) return true;
    //---------
  }
  //
  return false;// if no reco mu is missed wrt gen mus -> no fake mus
  //
}
// // //
