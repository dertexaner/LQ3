//////////////////////////////////////////////////////////
// Systematic Uncertainty Tools
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
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
using namespace std;
//
// // //
double analysisClass::METcorr( TString PtPhi ){
  //
  if( isData && PtPhi=="Pt"  ) return PFMETType01XYCor->at(0);    // Only for MC
  if( isData && PtPhi=="Phi" ) return PFMETPhiType01XYCor->at(0); // Only for MC
  //
  TVector3 oldMETvector, sumLepJetvector, sumLepJetvectorPrime, LepJetvector;
  oldMETvector.SetPtEtaPhi( PFMETType01XYCor->at(0), 0, PFMETPhiType01XYCor->at(0) );
  sumLepJetvector.SetPtEtaPhi( 0, 0, 0 );
  sumLepJetvectorPrime.SetPtEtaPhi( 0, 0, 0 );
  //
  for( unsigned int iElR=0; iElR<ElectronPt->size(); iElR++){
    if( !elRisoCheck(iElR) ) continue;
    LepJetvector.SetPtEtaPhi( ElectronPt->at(iElR), ElectronEta->at(iElR), ElectronPhi->at(iElR) );
    sumLepJetvector+=LepJetvector;
    LepJetvector.SetPtEtaPhi( elPtcorr(iElR), ElectronEta->at(iElR), ElectronPhi->at(iElR) );
    sumLepJetvectorPrime+=LepJetvector;
  }
  //
  for( unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
    if( !muRisoCheck(iMuR) ) continue;
    LepJetvector.SetPtEtaPhi( MuonPt->at(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR) );
    sumLepJetvector+=LepJetvector;
    LepJetvector.SetPtEtaPhi( muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR) );
    sumLepJetvectorPrime+=LepJetvector;
  }
  //
  for( unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
    if( !tauRisoCheck(iTauR) ) continue;
    LepJetvector.SetPtEtaPhi( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR) );
    sumLepJetvector+=LepJetvector;
    LepJetvector.SetPtEtaPhi( tauPtcorr(iTauR),  HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR) );
    sumLepJetvectorPrime+=LepJetvector;
  }
  //
  for( unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
    if( !jetROverlapCheck(iJetR)      ) continue;
    if( fabs(PFJetEta->at(iJetR))>3   ) continue;
    if( PFJetPt->at(iJetR)<15         ) continue;
    LepJetvector.SetPtEtaPhi( PFJetPt->at(iJetR), PFJetEta->at(iJetR), PFJetPhi->at(iJetR) );
    sumLepJetvector+=LepJetvector;
    LepJetvector.SetPtEtaPhi( jetPtcorr(iJetR), PFJetEta->at(iJetR), PFJetPhi->at(iJetR) );
    sumLepJetvectorPrime+=LepJetvector;
  }
  //
  if( PtPhi=="Pt"   ) return (oldMETvector+sumLepJetvector-sumLepJetvectorPrime).Pt();
  if( PtPhi=="Phi"  ) return (oldMETvector+sumLepJetvector-sumLepJetvectorPrime).Phi();
}
// // //
double analysisClass::elPtcorr(unsigned int iElR){
  //
  if( isData ) return ElectronPt->at(iElR); // Only for MC
  //
  double elUnc=0;
  if( fabs(ElectronEta->at(iElR))<1.442 ) elUnc=0.004;//0.4% for barrel
  if( fabs(ElectronEta->at(iElR))>1.442 ) elUnc=0.041;//4.1% for endcap
  //
  double ElPt  = ElectronPt->at(iElR);
  double ElPt_1sigmaUP   = ElectronPt->at(iElR)*(1+elUnc);
  double ElPt_1sigmaDOWN = ElectronPt->at(iElR)*(1-elUnc);
  //
  if( !El_1sigmaUP  && !El_1sigmaDOWN ) return ElPt;            //ElPt
  if(  El_1sigmaUP  && !El_1sigmaDOWN ) return ElPt_1sigmaUP;   //ElPt_1sigmaUP
  if( !El_1sigmaUP  &&  El_1sigmaDOWN ) return ElPt_1sigmaDOWN; //ElPt_1sigmaDOWN
}
// // //
double analysisClass::muPtcorr(unsigned int iMuR){
  //
  if( isData ) return MuonPt->at(iMuR); // Only for MC
  //
  double MuPt  = MuonPt->at(iMuR);
  double muSigma;
  if( MuPt>=200 ) muSigma= TMath::Power( TMath::Power((MuPt*0.05/1000.0),2) + TMath::Power(0.006,2) , 0.5 );
  if( MuPt< 200 ) muSigma= 0.0063; // 0.2% oplus 0.6% = 0.63%
  double MuPt_1sigmaUP   = MuonPt->at(iMuR)*(1+muSigma);
  double MuPt_1sigmaDOWN = MuonPt->at(iMuR)*(1-muSigma);
  //
  if( !Mu_1sigmaUP  && !Mu_1sigmaDOWN ) return MuPt;            //MuPt
  if(  Mu_1sigmaUP  && !Mu_1sigmaDOWN ) return MuPt_1sigmaUP;   //MuPt_1sigmaUP
  if( !Mu_1sigmaUP  &&  Mu_1sigmaDOWN ) return MuPt_1sigmaDOWN; //MuPt_1sigmaDOWN
}
// // //
double analysisClass::tauPtcorr(unsigned int iTauR){
  //
  if( isData ) return HPSTauPt->at(iTauR); // Only for MC
  //
  double TauPt               = HPSTauPt->at(iTauR);
  double TauPt_1sigmaUP      = HPSTauPt->at(iTauR)*1.03;
  double TauPt_1sigmaDOWN    = HPSTauPt->at(iTauR)*0.97;
  double TauPt_RES1sigmaUP   = HPSTauPt->at(iTauR);
  double TauPt_RES1sigmaDOWN = HPSTauPt->at(iTauR);
  //
  if( Tau_RES1sigmaUP || Tau_RES1sigmaDOWN ){
    double getRecoTauGenPt_ = getRecoTauGenPt(iTauR); // this function is different for SIGNAL and BCKG:: Use 
    if( getRecoTauGenPt_>0 ){ //if there is a matching gen-tau
      TauPt_RES1sigmaUP   = TMath::Max( 0.0, getRecoTauGenPt_+(1.1)*(HPSTauPt->at(iTauR)-getRecoTauGenPt_) );//Max is used to protect negative PTs!
      TauPt_RES1sigmaDOWN = TMath::Max( 0.0, getRecoTauGenPt_+(0.9)*(HPSTauPt->at(iTauR)-getRecoTauGenPt_) );//Max is used to protect negative PTs!
    }
  }
  //
  if( !Tau_1sigmaUP  && !Tau_1sigmaDOWN && !Tau_RES1sigmaUP && !Tau_RES1sigmaDOWN ) return TauPt;            //TauPt
  if(  Tau_1sigmaUP  && !Tau_1sigmaDOWN && !Tau_RES1sigmaUP && !Tau_RES1sigmaDOWN ) return TauPt_1sigmaUP;   //TauPt_1sigmaUP
  if( !Tau_1sigmaUP  &&  Tau_1sigmaDOWN && !Tau_RES1sigmaUP && !Tau_RES1sigmaDOWN ) return TauPt_1sigmaDOWN; //TauPt_1sigmaDOWN
  if( !Tau_1sigmaUP  && !Tau_1sigmaDOWN &&  Tau_RES1sigmaUP && !Tau_RES1sigmaDOWN ) return TauPt_RES1sigmaUP;   //TauPt_RES1sigmaUP
  if( !Tau_1sigmaUP  && !Tau_1sigmaDOWN && !Tau_RES1sigmaUP &&  Tau_RES1sigmaDOWN ) return TauPt_RES1sigmaDOWN; //TauPt_RES1sigmaDOWN
}
// // //
double analysisClass::jetPtREScorr(unsigned int iJetR, int sigma_ ){
  // 
  if( isData ) return PFJetPt->at(iJetR); // Only for MC
  if( PFJetPt->at(iJetR)<15         ) return PFJetPt->at(iJetR);
  if( fabs(PFJetEta->at(iJetR))>3   ) return PFJetPt->at(iJetR);
  //
  double newJetPt  = PFJetPt->at(iJetR);
  if( noJERcorr )  return newJetPt;
  //
  // Jet Energy Resolution Matching
  // https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution#Recommendations_for_7_and_8_TeV
  //  Eta  | RATIO |     ERROR
  //0.0-0.5 1.052+-0.012+0.062-0.061 = 1.052 +/- 0.063 
  //0.5-1.1 1.057+-0.012+0.056-0.055 = 1.057 +/- 0.057
  //1.1-1.7 1.096+-0.017+0.063-0.062 = 1.096 +/- 0.065
  //1.7-2.3 1.134+-0.035+0.087-0.085 = 1.134 +/- 0.094
  //2.3-5.0 1.288+-0.127+0.155-0.153 = 1.288 +/- 0.200
  // Formula: newJetPt = jetPtGEN+c*(jetPt-jetPtGEN) = c*jetPt + (1-c)*jetPtGEN
  //------------------------------------------//
  //
  TLorentzVector jetReco;  jetReco.SetPtEtaPhiM( PFJetPt->at(iJetR),  PFJetEta->at(iJetR),  PFJetPhi->at(iJetR), 0 );
  TLorentzVector jetGen;
  double JER_sf     = 0;
  double JER_1sigma = 0;
  //
  if( fabs(jetReco.Eta())<5.0 && fabs(jetReco.Eta())>=2.3 ){ JER_sf=1.288; JER_1sigma=0.200; }
  if( fabs(jetReco.Eta())<2.3 && fabs(jetReco.Eta())>=1.7 ){ JER_sf=1.134; JER_1sigma=0.094; }
  if( fabs(jetReco.Eta())<1.7 && fabs(jetReco.Eta())>=1.1 ){ JER_sf=1.096; JER_1sigma=0.065; }
  if( fabs(jetReco.Eta())<1.1 && fabs(jetReco.Eta())>=0.5 ){ JER_sf=1.057; JER_1sigma=0.057; }
  if( fabs(jetReco.Eta())<0.5                             ){ JER_sf=1.052; JER_1sigma=0.063; }
  //
  //----------------------------
  double deltaR_  = 9999;
  double jetGenPt = 0;
  for(unsigned int iJetT=0; iJetT<GenJetPt->size(); iJetT++){
    jetGen.SetPtEtaPhiM( GenJetPt->at(iJetT),  GenJetEta->at(iJetT),  GenJetPhi->at(iJetT), 0 );
    if( jetReco.DeltaR(jetGen)<deltaR_ ){ deltaR_=jetReco.DeltaR(jetGen); jetGenPt=jetGen.Pt(); }
  }
  if( deltaR_<0.5 && JER_sf>0 && jetGenPt>0 ){ //if there is a matching gen-jet
    if( sigma_==0  ) newJetPt = TMath::Max( 0.0, jetGenPt+(JER_sf)*(jetReco.Pt()-jetGenPt) );           //Max is used to protect negative PTs!
    if( sigma_==+1 ) newJetPt = TMath::Max( 0.0, jetGenPt+(JER_sf+JER_1sigma)*(jetReco.Pt()-jetGenPt) );//Max is used to protect negative PTs!
    if( sigma_==-1 ) newJetPt = TMath::Max( 0.0, jetGenPt+(JER_sf-JER_1sigma)*(jetReco.Pt()-jetGenPt) );//Max is used to protect negative PTs!
  }
  return newJetPt;
}
// // //
double analysisClass::jetPtcorr(unsigned int iJetR){
  // 
  if( isData ) return PFJetPt->at(iJetR); // Only for MC
  if( PFJetPt->at(iJetR)<15         ) return PFJetPt->at(iJetR);
  if( fabs(PFJetEta->at(iJetR))>3   ) return PFJetPt->at(iJetR);
  //
  double newJetPt               = jetPtREScorr(iJetR, 0);//apply res correction
  double newJetPt_JER1sigmaUP;   if( JER_1sigmaUP   ){ newJetPt_JER1sigmaUP   = jetPtREScorr(iJetR,+1); }
  double newJetPt_JER1sigmaDOWN; if( JER_1sigmaDOWN ){ newJetPt_JER1sigmaDOWN = jetPtREScorr(iJetR,-1); }
  double newJetPt_JEC1sigmaUP   = newJetPt;
  double newJetPt_JEC1sigmaDOWN = newJetPt;
  //
  double JEC_1sigma = 0;
  if( PFJetJECUnc->at(iJetR)>=0 ){ //if there is a valid JEC uncertainty
    JEC_1sigma=PFJetJECUnc->at(iJetR);
    newJetPt_JEC1sigmaUP   = newJetPt*(1+JEC_1sigma);
    newJetPt_JEC1sigmaDOWN = newJetPt*(1-JEC_1sigma);
  }
  //
  //----------------------------
  // Output
  if( !JER_1sigmaUP  && !JER_1sigmaDOWN  && !JEC_1sigmaUP  && !JEC_1sigmaDOWN  ) return newJetPt; //newJetPt
  if(  JER_1sigmaUP  && !JER_1sigmaDOWN  && !JEC_1sigmaUP  && !JEC_1sigmaDOWN  ) return newJetPt_JER1sigmaUP;   //JER_1sigmaUP
  if( !JER_1sigmaUP  &&  JER_1sigmaDOWN  && !JEC_1sigmaUP  && !JEC_1sigmaDOWN  ) return newJetPt_JER1sigmaDOWN; //JER_1sigmaDOWN
  if( !JER_1sigmaUP  && !JER_1sigmaDOWN  &&  JEC_1sigmaUP  && !JEC_1sigmaDOWN  ) return newJetPt_JEC1sigmaUP;   //JEC_1sigmaUP
  if( !JER_1sigmaUP  && !JER_1sigmaDOWN  && !JEC_1sigmaUP  &&  JEC_1sigmaDOWN  ) return newJetPt_JEC1sigmaDOWN; //JEC_1sigmaDOWN
}
// // //
void analysisClass::ResetLepJetMetSystematics(){
  noJERcorr      = false;
  JER_1sigmaUP   = false;
  JER_1sigmaDOWN = false;
  JEC_1sigmaUP   = false;
  JEC_1sigmaDOWN = false;
  Tau_1sigmaUP   = false;
  Tau_1sigmaDOWN = false;
  El_1sigmaUP    = false;
  El_1sigmaDOWN  = false;
  Mu_1sigmaUP    = false;
  Mu_1sigmaDOWN  = false;
  Tau_RES1sigmaUP   = false;
  Tau_RES1sigmaDOWN = false;
}
// // //
void analysisClass::ApplyLepJetMetSystematics(unsigned int iSys){
  ResetLepJetMetSystematics();//reset all first
  if( iSys==0  ){ ResetLepJetMetSystematics(); noJERcorr=true; }//no JER correction, no nothing
  if( iSys==1  ){ ResetLepJetMetSystematics(); }//JER correction 
  if( iSys==2  ){ ResetLepJetMetSystematics(); JER_1sigmaUP   = true;}//JER correction + 1 sigma
  if( iSys==3  ){ ResetLepJetMetSystematics(); JER_1sigmaDOWN = true;}//JER correction - 1 sigma
  if( iSys==4  ){ ResetLepJetMetSystematics(); noJERcorr=true; JEC_1sigmaUP   = true;}
  if( iSys==5  ){ ResetLepJetMetSystematics(); noJERcorr=true; JEC_1sigmaDOWN = true;}
  if( iSys==6  ){ ResetLepJetMetSystematics(); noJERcorr=true; Tau_1sigmaUP   = true;}
  if( iSys==7  ){ ResetLepJetMetSystematics(); noJERcorr=true; Tau_1sigmaDOWN = true;}
  if( iSys==8  ){ ResetLepJetMetSystematics(); noJERcorr=true; El_1sigmaUP    = true;}
  if( iSys==9  ){ ResetLepJetMetSystematics(); noJERcorr=true; El_1sigmaDOWN  = true;}
  if( iSys==10 ){ ResetLepJetMetSystematics(); noJERcorr=true; Mu_1sigmaUP    = true;}
  if( iSys==11 ){ ResetLepJetMetSystematics(); noJERcorr=true; Mu_1sigmaDOWN  = true;}
  if( iSys==12 ){ ResetLepJetMetSystematics(); noJERcorr=true; Tau_RES1sigmaUP   = true;}
  if( iSys==13 ){ ResetLepJetMetSystematics(); noJERcorr=true; Tau_RES1sigmaDOWN = true;}
}
// // //
