//////////////////////////////////////////////////////////
// Object Selector Functions
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
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
using namespace std;
//
// // //
bool analysisClass::tauRCheck(unsigned int iTauR){
  if( ObjDef == "LI_VLIforTau"  ) return tauRVLooseCheck(iTauR);
  if( ObjDef == "LI"            ) return tauRLooseCheck(iTauR);
  if( ObjDef == "TI"            ) return tauRTightCheck(iTauR);
  else{
    cout<<"ERROR: WRONG Object Definition at tauRCheck!"<<endl; return false; 
  }
}
// // //
bool analysisClass::tauRVLooseCheck(unsigned int iTauR){
  //
  if( tauPtcorr(iTauR)<=TauPtCut                                        ) return false;
  if( fabs(HPSTauEta->at(iTauR))>2.1                                    ) return false;
  //if( HPSTauAgainstElectronTightMVA3Discr->at(iTauR)!=1                 ) return false;
  //if( HPSTauAgainstMuonTight2Discr->at(iTauR)!=1                        ) return false;
  //if( HPSTauDecayModeFindingDiscr->at(iTauR)!=1                         ) return false;
  if( tauJetDeltaRmin(iTauR)<0.7 ) return false;//jul-31-2013 (loose-tight) //---Taken out temporarily, Seem to affect yields in control region.. Sep-5-2013
  //
  return true;
}
// // //
bool analysisClass::tauRLooseCheck(unsigned int iTauR){
  //
  if( tauPtcorr(iTauR)<=TauPtCut                                        ) return false;
  if( fabs(HPSTauEta->at(iTauR))>2.1                                    ) return false;
  if( HPSTauAgainstElectronTightMVA3Discr->at(iTauR)!=1                 ) return false;
  if( HPSTauAgainstMuonTight2Discr->at(iTauR)!=1                        ) return false;
  if( HPSTauDecayModeFindingDiscr->at(iTauR)!=1                         ) return false;
  //if( HPSTauLooseCombinedIsolationDeltaBetaCorr3HitsDiscr->at(iTauR)!=1 ) return false; // to be taken out as a test 09-Apr-2013
  if( HPSTauVLooseCombinedIsolationDeltaBetaCorrDiscr->at(iTauR)!=1     ) return false;  
  //
  //if( tauJetDeltaRmin(iTauR)<0.6 ) return false; //initially used for vloose-medium
  //if( tauJetDeltaRmin(iTauR)<0.5 ) return false;//loose-tight
  if( tauJetDeltaRmin(iTauR)<0.7 ) return false;//jul-31-2013 (loose-tight) //---Taken out temporarily, Seem to affect yields in control region.. Sep-5-2013
  //
  return true;
}
// // //
bool analysisClass::tauRTightCheck(unsigned int iTauR){
  //
  if( !tauRLooseCheck(iTauR)                                             ) return false;
  //
  //if( HPSTauMediumCombinedIsolationDeltaBetaCorr3HitsDiscr->at(iTauR)!=1 ) return false;
  if( HPSTauTightCombinedIsolationDeltaBetaCorr3HitsDiscr->at(iTauR)!=1 ) return false;
  //
  return true;
}
// // //
bool analysisClass::elRCheck(unsigned int iElR){
  //
  if( idDef != "HEEP" && idDef != "CUT" && idDef != "MVA" ){ 
    cout<<"ERROR: WRONG Electron ID Definition at elRCheck !"<<endl; return false; 
  }
  return elRLooseCheck( iElR );
  //
}
// // //
bool analysisClass::elRLooseCheck(unsigned int iElR){
  //
  if( idDef != "HEEP" && idDef != "CUT" && idDef != "MVA" ) return false;
  //
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // HEEP Electron ID v4.1 ( https://twiki.cern.ch/twiki/bin/view/CMS/HEEPElectronID - Oct 31, 2012 ) 
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Cut Based Electron ID ( https://twiki.cern.ch/twiki/bin/view/CMS/EgammaCutBasedIdentification - Sep 30, 2012)
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // MVA Electron ID ( https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentification - Sep 30, 2012)
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // ------- Loose Selection ------- 
  //
  // --- --- --- ---  
  bool isBarrel_=false;
  bool isEndCap_=false;
  bool isEndCapPtHeepBelow50_=false;
  bool isEndCapPtHeepAbove50_=false;
  if( fabs(ElectronSCEta->at(iElR))<1.442                                      ) isBarrel_=true;
  if( fabs(ElectronSCEta->at(iElR))>1.560 && fabs(ElectronSCEta->at(iElR))<2.5 ) isEndCap_=true;
  if( isEndCap_ && ElectronPtHeep->at(iElR)<50.0                               ) isEndCapPtHeepBelow50_=true;
  if( isEndCap_ && ElectronPtHeep->at(iElR)>=50.0                              ) isEndCapPtHeepAbove50_=true;
  //
  // --- --- --- ---  
  if( idDef == "CUT" ){
    //
    if( !isBarrel_ && !isEndCap_ )return false;
    //
    if(isBarrel_){
      if( ElectronPtHeep->at(iElR)<=15                   ) return false;
      if( fabs(ElectronDeltaEtaTrkSC->at(iElR))>=0.007   ) return false;
      if( fabs(ElectronDeltaPhiTrkSC->at(iElR))>=0.15    ) return false;
      if( ElectronSigmaIEtaIEta->at(iElR)>=0.01          ) return false;
      if( ElectronHoE->at(iElR)>=0.12                    ) return false;
      if( ElectronVtxDistXY->at(iElR)>0.02               ) return false;
      if( ElectronVtxDistZ->at(iElR)>0.2                 ) return false;
      if( fabs( (1/ElectronCaloEnergy->at(iElR))-(ElectronESuperClusterOverP->at(iElR)/ElectronCaloEnergy->at(iElR)))>0.05 ) return false;
      if( ElectronMissingHits->at(iElR)>1                ) return false; 
      if( ElectronConvFitProb->at(iElR)>0.000001         ) return false;
    }
    if(isEndCap_){
      if( ElectronPtHeep->at(iElR)<=15                   ) return false;
      if( fabs(ElectronDeltaEtaTrkSC->at(iElR))>=0.009   ) return false;
      if( fabs(ElectronDeltaPhiTrkSC->at(iElR))>=0.10    ) return false;
      if( ElectronSigmaIEtaIEta->at(iElR)>=0.03          ) return false;
      if( ElectronHoE->at(iElR)>=0.10                    ) return false;
      if( ElectronVtxDistXY->at(iElR)>0.02               ) return false;
      if( ElectronVtxDistZ->at(iElR)>0.2                 ) return false;
      if( fabs( (1/ElectronCaloEnergy->at(iElR))-(ElectronESuperClusterOverP->at(iElR)/ElectronCaloEnergy->at(iElR)))>0.05 ) return false;
      if( ElectronMissingHits->at(iElR)>1                ) return false; 
      if( ElectronConvFitProb->at(iElR)>0.000001         ) return false;
    }
    //
    // relPF Isolation - Follows from Higgs AN2012_228_v7 & https://twiki.cern.ch/twiki/bin/view/CMS/EgammaEARhoCorrection#Rho_for_2012_Effective_Areas
    //double pfIso= ElectronPFChargedHadronIso04->at(iElR) + TMath::Max( 0.0 , ElectronPFNeutralHadronIso04->at(iElR) + ElectronPFPhotonIso04->at(iElR)
    //								       - rhoJets*elISOEffectiveAreaCone04( ElectronEta->at(iElR) ) );
    //switch to dR:0.3
    double pfIso= ElectronPFChargedHadronIso03->at(iElR) + TMath::Max( 0.0 , ElectronPFNeutralHadronIso03->at(iElR) + ElectronPFPhotonIso03->at(iElR)
								       - rhoJets*elISOEffectiveAreaCone03( ElectronEta->at(iElR) ) );
    if( ElectronPtHeep->at(iElR)<20 && isEndCap_ && pfIso/elPtcorr(iElR) > 0.10 ) return false;
    if( pfIso/elPtcorr(iElR) > 0.15 ) return false;
    //
  }
  // --- --- --- ---  
  if( idDef == "HEEP" ){
    //
    if( !isBarrel_ && !isEndCap_ )return false;
    //
    if(isBarrel_){
      if( ElectronPtHeep->at(iElR)<=35                                                 ) return false;
      if( fabs(ElectronDeltaEtaTrkSC->at(iElR))>=0.005                                 ) return false;
      if( ElectronE1x5OverE5x5->at(iElR)<=0.83 && ElectronE2x5OverE5x5->at(iElR)<=0.94 ) return false;
      double heepIsoCut = 2.0 + (0.03*ElectronPtHeep->at(iElR)) + (0.28*rhoForHEEP);
      if( (ElectronEcalIsoDR03->at(iElR)+ElectronHcalIsoD1DR03->at(iElR))>=heepIsoCut  ) return false;
      if( ElectronVtxDistXY->at(iElR)>=0.02            ) return false;
    }
    if(isEndCap_){
      if( ElectronPtHeep->at(iElR)<=35                 ) return false;
      if( fabs(ElectronDeltaEtaTrkSC->at(iElR))>=0.007 ) return false;
      if( ElectronSigmaIEtaIEta->at(iElR)>=0.03        ) return false;
      double heepIsoCut_below50 = 2.5 + (0.28*rhoForHEEP); 
      if( isEndCapPtHeepBelow50_ && (ElectronEcalIsoDR03->at(iElR)+ElectronHcalIsoD1DR03->at(iElR))>= heepIsoCut_below50 ) return false;
      double heepIsoCut_above50 = 2.5 + (0.03*(ElectronPtHeep->at(iElR)-50.0)) + (0.28*rhoForHEEP); 
      if( isEndCapPtHeepAbove50_ && (ElectronEcalIsoDR03->at(iElR)+ElectronHcalIsoD1DR03->at(iElR))>= heepIsoCut_above50 ) return false;	  
      if( ElectronVtxDistXY->at(iElR)>=0.05            ) return false;
    }
    //BARREL&ENDCAP COMMON CUTS
    if( ElectronHasEcalDrivenSeed->at(iElR)==false  ) return false;
    if( fabs(ElectronDeltaPhiTrkSC->at(iElR))>=0.06 ) return false;
    if( ElectronHoE->at(iElR)>=0.05                 ) return false;
    if( ElectronTrkIsoDR03->at(iElR)>=5             ) return false;
    if( ElectronMissingHits->at(iElR)>1             ) return false;
    //
  }
  // --- --- --- ---
  if( idDef == "MVA" ){
    //BARREL&ENDCAP COMMON CUTS
    if( (ElectronPassId->at(iElR) & ( 1 << 6 )) >> 6 == 0 ) return false;//MVA ID check
    if( ElectronPtHeep->at(iElR)<=10                      ) return false;
    //if( ElectronConvFitProb->at(iElR)>0.000001          ) return false;
    if( ElectronConvFitProb->at(iElR)<=10e-7              ) return false;
    if( ElectronMissingHits->at(iElR)>0                   ) return false;
    if( ElectronVtxDistXY->at(iElR)>=0.02                 ) return false;
    if( ElectronVtxDistZ->at(iElR)>=0.1                   ) return false;
    //
    //if(isBarrel_){
    //if( ElectronSigmaIEtaIEta->at(iElR)>=0.01           ) return false;
    //if( fabs(ElectronDeltaPhiTrkSC->at(iElR))>=0.15     ) return false;
    //if( fabs(ElectronDeltaEtaTrkSC->at(iElR))>=0.007    ) return false;
    //if( ElectronHoE->at(iElR)>=0.12                     ) return false;
    //}
    //if(isEndCap_){
    //if( ElectronSigmaIEtaIEta->at(iElR)>=0.03           ) return false;
    //if( fabs(ElectronDeltaPhiTrkSC->at(iElR))>=0.10     ) return false;
    //if( fabs(ElectronDeltaEtaTrkSC->at(iElR))>=0.009    ) return false;
    //if( ElectronHoE->at(iElR)>=0.10                     ) return false;
    //}
    //
    // relPF Isolation - Follows from Higgs AN2012_228_v7 & https://twiki.cern.ch/twiki/bin/view/CMS/EgammaEARhoCorrection#Rho_for_2012_Effective_Areas
    //double pfIso= ElectronPFChargedHadronIso04->at(iElR) + TMath::Max( 0.0 , ElectronPFNeutralHadronIso04->at(iElR) + ElectronPFPhotonIso04->at(iElR)
    //								       - rhoJets*elISOEffectiveAreaCone04( ElectronEta->at(iElR) ) );
    //switch to dR:0.3
    double pfIso= ElectronPFChargedHadronIso03->at(iElR) + TMath::Max( 0.0 , ElectronPFNeutralHadronIso03->at(iElR) + ElectronPFPhotonIso03->at(iElR)
								       - rhoJets*elISOEffectiveAreaCone03( ElectronEta->at(iElR) ) );
    if( pfIso/elPtcorr(iElR) > 0.15 ) return false;
    //
  }
  // --- --- --- ---
  //
  return true;
}
// // //
bool analysisClass::elRTightCheck(unsigned int iElR){
  //
  if( !elRLooseCheck( iElR ) ) return false;
  //
  if( idDef == "CUT" ){
    bool isBarrel_=false;
    bool isEndCap_=false;
    if( fabs(ElectronSCEta->at(iElR)                                     )<1.442 ) isBarrel_=true;
    if( fabs(ElectronSCEta->at(iElR))>1.560 && fabs(ElectronSCEta->at(iElR))<2.5 ) isEndCap_=true;
    //
    if(isBarrel_){
      if( fabs(ElectronDeltaEtaTrkSC->at(iElR))>=0.004   ) return false;
      if( fabs(ElectronDeltaPhiTrkSC->at(iElR))>=0.06    ) return false;
      if( ElectronVtxDistZ->at(iElR)>0.1                 ) return false;
    }
    if(isEndCap_){
      if( fabs(ElectronDeltaEtaTrkSC->at(iElR))>=0.007   ) return false;
      if( fabs(ElectronDeltaPhiTrkSC->at(iElR))>=0.03    ) return false;
      if( ElectronVtxDistZ->at(iElR)>0.1                 ) return false;
    }
    //
    // relPF Isolation - Follows from Higgs AN2012_228_v7 & https://twiki.cern.ch/twiki/bin/view/CMS/EgammaEARhoCorrection#Rho_for_2012_Effective_Areas
    //    double pfIso= ElectronPFChargedHadronIso04->at(iElR) + TMath::Max( 0.0 , ElectronPFNeutralHadronIso04->at(iElR) + ElectronPFPhotonIso04->at(iElR)
    //								       - rhoJets*elISOEffectiveAreaCone04( ElectronEta->at(iElR) ) );
    //switch to dR:0.3
    double pfIso= ElectronPFChargedHadronIso03->at(iElR) + TMath::Max( 0.0 , ElectronPFNeutralHadronIso03->at(iElR) + ElectronPFPhotonIso03->at(iElR)
								       - rhoJets*elISOEffectiveAreaCone03( ElectronEta->at(iElR) ) );
    if( pfIso/elPtcorr(iElR) > 0.15 ) return false;
    //
    return true;
  }
  // -------------
}
// // //
bool analysisClass::muRCheck(unsigned int iMuR){     
  if( ObjDef == "LI_VLIforTau" ){ return muRLooseCheck(iMuR);      }
  if( ObjDef == "LI"           ){ return muRLooseCheck(iMuR);      }
  if( ObjDef == "TI"           ){ return muRTightCheck(iMuR);      }
  else{
    cout<<"ERROR: WRONG Object Definition at muRCheck !"<<endl; return false;
  }
}
// // //
bool analysisClass::muRLooseCheck(unsigned int iMuR){
  //
  if( muPtcorr(iMuR)<=MuonPtCut                 ) return false;
  if( fabs(MuonEta->at(iMuR))>MuonEtaCut          ) return false;
  //
  //For Loose ID -- not compatible with the trigger (global is needed)
  //if( MuonIsGlobal->at(iMuR)!=1 && MuonIsTracker->at(iMuR)!=1 ) return false; 
  //
  //For Tight ID
  if( MuonIsPF->at(iMuR)!=1                       ) return false; 
  if( MuonIsGlobal->at(iMuR)!=1                   ) return false;//L3 requires global
  if( MuonGlobalChi2->at(iMuR)>=10                ) return false;//L3 (<20)
  if( MuonGlobalTrkValidHits->at(iMuR)<1          ) return false;//L2 (exact)
  if( MuonStationMatches->at(iMuR)<2              ) return false;//L2 (exact)
  if( fabs(MuonBestTrackVtxDistXY->at(iMuR))>=0.2 ) return false;//L3 (<0.1)
  if( fabs(MuonBestTrackVtxDistZ->at(iMuR))>=0.5  ) return false;
  if( MuonTrkPixelHits->at(iMuR)<1                ) return false;
  if( MuonTrackLayersWithMeasurement->at(iMuR)<6  ) return false;
  //
  //if( muJetDeltaRmin(iMuR)<0.4 ) return false;
  if( muJetDeltaRmin(iMuR)<0.5 ) return false;//jul-31-2013 (loose-tight)  --- Taken out temporarily, Seem to affect yields in control region.. Sep-5-2013
  //
  return true;
}
// // //
bool analysisClass::muRTightCheck(unsigned int iMuR){        
  //
  if( !muRLooseCheck(iMuR)                        ) return false; 
  //
  // PF Rel Isolation:  I = [sumChargedHadronPt+ max(0.,sumNeutralHadronPt+sumPhotonPt-0.5sumPUPt)]/pt
  double pfIso = 0;
  pfIso = MuonPFIsoR04ChargedHadron->at(iMuR) + TMath::Max( 0.0 , (MuonPFIsoR04NeutralHadron->at(iMuR)+MuonPFIsoR04Photon->at(iMuR)-0.5*MuonPFIsoR04PU->at(iMuR)) );
  //if( pfIso/muPtcorr(iMuR) > 0.20 ) return false;//loose WP
  if( pfIso/muPtcorr(iMuR) > 0.12 ) return false;//tight WP
  //
  return true;
}
// // //
/*
double analysisClass::jetPtMCcorr(unsigned int iJetR){
  // 
  if( isData ) return PFJetPt->at(iJetR); // This resolution correction only applied to MC jets..
  //
  // Jet Energy Resolution Matching
  // https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution#Recommendations_for_7_and_8_TeV
  //  Eta  | RATIO |     ERROR
  //0.0-0.5 1.052+-0.012+0.062-0.061
  //0.5-1.1 1.057+-0.012+0.056-0.055 
  //1.1-1.7 1.096+-0.017+0.063-0.062 
  //1.7-2.3 1.134+-0.035+0.087-0.085 
  //2.3-5.0 1.288+-0.127+0.155-0.153
  // Formula: jetPt+MAX(0,c*(jetPt-jetPtGEN)
  //------------------------------------------//
  //
  TLorentzVector jetReco;
  TLorentzVector jetGen;
  jetReco.SetPtEtaPhiM( PFJetPt->at(iJetR),  PFJetEta->at(iJetR),  PFJetPhi->at(iJetR), 0 );
  double SF=0;
  //
  if( fabs(jetReco.Eta())<0.5 ) SF=1.052;
  if( fabs(jetReco.Eta())<1.1 ) SF=1.057;
  if( fabs(jetReco.Eta())<1.7 ) SF=1.096;
  if( fabs(jetReco.Eta())<2.3 ) SF=1.134;
  if( fabs(jetReco.Eta())<5.0 ) SF=1.288;
  //
  double deltaR_ = 9999;
  double jetGenPt=0;
  for(unsigned int iJetT=0; iJetT<GenJetPt->size(); iJetT++){
    jetGen.SetPtEtaPhiM( GenJetPt->at(iJetT),  GenJetEta->at(iJetT),  GenJetPhi->at(iJetT), 0 );
    if( jetReco.DeltaR(jetGen)<deltaR_ ){ deltaR_=jetReco.DeltaR(jetGen); jetGenPt=jetGen.Pt(); }
  }
  if( deltaR_<0.5 && fabs(jetReco.Eta())<5.0 && jetReco.Pt()>10 && SF>0 && jetGenPt>0 ){
    //cout<<"RECO/GEN: "<<jetReco.Pt()/jetGenPt<<endl;
    return jetGenPt+SF*(jetReco.Pt()-jetGenPt);
  }
  else
    return jetReco.Pt();
}
*/
// // //
bool analysisClass::jetRCheck(unsigned int iJetR){
    return jetRLooseCheck(iJetR);
}
// // //
bool analysisClass::jetRLooseCheck(unsigned int iJetR){
  //
  //PFJet Loose ( https://twiki.cern.ch/twiki/bin/view/CMS/JetID#Documentation - Sep 30, 2012 )
  //
  if( PFJetNeutralHadronEnergyFraction->at(iJetR)>=0.99 ) return false;
  if( PFJetNeutralEmEnergyFraction->at(iJetR)>=0.99     ) return false;
  if( PFJetNConstituents->at(iJetR)<2                   ) return false;
  if( fabs(PFJetEta->at(iJetR))<2.4 ){
    if( PFJetChargedHadronEnergyFraction->at(iJetR)<=0  ) return false;
    if( PFJetChargedMultiplicity->at(iJetR)<=0          ) return false;
    if( PFJetChargedEmEnergyFraction->at(iJetR)>=0.99   ) return false;
  }
  //
  //if( jetPtcorr(iJetR)<=35    ) return false;
  if( jetPtcorr(iJetR)<=40      ) return false;
  if( fabs(PFJetEta->at(iJetR))>3 ) return false;
  //
  return true;
}
// // //
bool analysisClass::jetRTightCheck(unsigned int iJetR){
  //
  if( !jetRLooseCheck(iJetR)                            ) return false;
  //
  if( PFJetNeutralHadronEnergyFraction->at(iJetR)>=0.95 ) return false;
  if( PFJetNeutralEmEnergyFraction->at(iJetR)>=0.95     ) return false;
  //
  return true;
}
// // //
bool analysisClass::Wpeak_jetRCheck(unsigned int iJetR){
  if( !jetRCheck(iJetR) ) return false;
  //
  TLorentzVector Dijet; TLorentzVector jet1;  TLorentzVector jet2;  float Wmass=80.4; float Wwidth=2.1;
  jet1.SetPtEtaPhiM( jetPtcorr(iJetR), PFJetEta->at(iJetR), PFJetPhi->at(iJetR), 0 );
  //
  for(unsigned int ijet=0; ijet<PFJetPt->size(); ijet++){
    if( ijet==iJetR ) continue;
    if( jetRCheck(ijet) ){
      jet2.SetPtEtaPhiM( jetPtcorr(ijet), PFJetEta->at(ijet), PFJetPhi->at(ijet), 0 );
      Dijet=jet1+jet2;
      if( fabs(Dijet.M()-Wmass)<(Wwidth*3) ) return true;
    }
  }
  //
  return false;
}
// // //
bool analysisClass::btag_jetRCheck(unsigned int iJetR){
  //
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG
  // https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookBTagging
  // 
  // Identification of b-quark jets in the CMS experiment BTV-12-001 : combined BTV-11-003 and BTV-11-004 paper 
  // http://cms.cern.ch/iCMS/analysisadmin/cadi?ancode=BTV-11-003
  // http://cms.cern.ch/iCMS/analysisadmin/cadi?ancode=BTV-11-004
  // http://cms.cern.ch/iCMS/analysisadmin/cadi?ancode=BTV-12-001
  //
  // ------------------------------------------------ //
  //     Tagger name        | WP name | WP Discr cut  //
  //  TrackCountingHighPur     TCHPT     3.41         //
  //  JetProbability           JPL       0.275        //
  //  JetProbability           JPM       0.545        //
  //  JetProbability           JPT       0.790        //
  //  CombinedSecondaryVertex  CSVL      0.244        //
  //  CombinedSecondaryVertex  CSVM      0.679        //
  //  CombinedSecondaryVertex  CSVT      0.898        //
  // ------------------------------------------------ //
  //
  // Operating Points-->>  L: 0.1  M: 0.01  T:0.001 
  //
  if( !jetRCheck( iJetR )  ) return false;
  //  
  if( BTagDef == "TCHPT" && PFJetTrackCountingHighPurBTag->at(iJetR)>3.41     ) return true;//TCHPT
  //
  if( BTagDef == "JPL"   && PFJetJetProbabilityBTag->at(iJetR)>0.275          ) return true;//JPL
  if( BTagDef == "JPM"   && PFJetJetProbabilityBTag->at(iJetR)>0.545          ) return true;//JPM
  if( BTagDef == "JPT"   && PFJetJetProbabilityBTag->at(iJetR)>0.790          ) return true;//JPT
  //
  if( BTagDef == "CSVL"  && PFJetCombinedSecondaryVertexBTag->at(iJetR)>0.244 ) return true;//CSVL
  if( BTagDef == "CSVM"  && PFJetCombinedSecondaryVertexBTag->at(iJetR)>0.679 ) return true;//CSVM
  if( BTagDef == "CSVT"  && PFJetCombinedSecondaryVertexBTag->at(iJetR)>0.898 ) return true;//CSVT
  //
  return false;
}
// // //
double analysisClass::jetBValue(unsigned int iJetR){
  if( BTagDef == "TCHPT"                                          ) return PFJetTrackCountingHighPurBTag->at(iJetR);
  if( BTagDef == "JPL"  || BTagDef == "JPM"  || BTagDef == "JPT"  ) return PFJetJetProbabilityBTag->at(iJetR);
  if( BTagDef == "CSVL" || BTagDef == "CSVM" || BTagDef == "CSVT" ) return PFJetCombinedSecondaryVertexBTag->at(iJetR);
  return -99;
}
// // //
bool analysisClass::muRisoCheck(unsigned int iMuR){
  if( !muRCheck(iMuR) ) return false;
  if( !removeOverlaps ) return true;
  //  
  TLorentzVector MuonCand;
  MuonCand.SetPtEtaPhiM(  muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0  );
  //    
  for(unsigned int ireco=0; ireco<ElectronPt->size(); ireco++){
    if(  elRCheck(ireco)  ){
      TLorentzVector ElectronCand;
      ElectronCand.SetPtEtaPhiM(  elPtcorr(ireco), ElectronEta->at(ireco), ElectronPhi->at(ireco), 0  );
      if( MuonCand.DeltaR(ElectronCand)<0.3 ) return false;
    }
  }
  //
  return true;
}
// // //
bool analysisClass::elRisoCheck(unsigned int iElR){
  if( !elRCheck(iElR) ) return false;
  if( !removeOverlaps ) return true;
  //
  TLorentzVector ElectronCand;
  ElectronCand.SetPtEtaPhiM(  elPtcorr(iElR), ElectronEta->at(iElR), ElectronPhi->at(iElR), 0  );
  // 
  for(unsigned int ireco=0; ireco<MuonPt->size(); ireco++){
    if( muRCheck(ireco) ){
      TLorentzVector MuonCand;
      MuonCand.SetPtEtaPhiM(    muPtcorr(ireco),    MuonEta->at(ireco),    MuonPhi->at(ireco),    0);
      if( ElectronCand.DeltaR(MuonCand)<0.3 ) return false;
    }
  }
  //
  return true;
}
// // //
bool analysisClass::jetRisoCheck(unsigned int iJetR){
  if( !jetRCheck(iJetR) ) return false;
  if( !removeOverlaps ) return true;
  //
  TLorentzVector JetCand;
  JetCand.SetPtEtaPhiM( jetPtcorr(iJetR), PFJetEta->at(iJetR), PFJetPhi->at(iJetR), 0  );
  //
  for(unsigned int ireco=0; ireco<HPSTauPt->size(); ireco++){
    if( tauRisoCheck(ireco) ){
      TLorentzVector TauCand;
      TauCand.SetPtEtaPhiM(  tauPtcorr(ireco), HPSTauEta->at(ireco), HPSTauPhi->at(ireco), 0  ); 
      if( JetCand.DeltaR(TauCand)<0.5 ) return false;
    }
  }
  //
  for(unsigned int ireco=0; ireco<MuonPt->size(); ireco++){
    if( muRisoCheck(ireco) ){
      TLorentzVector MuonCand;
      MuonCand.SetPtEtaPhiM(  muPtcorr(ireco), MuonEta->at(ireco), MuonPhi->at(ireco), 0  );
      if( JetCand.DeltaR(MuonCand)<0.5 ) return false;
    }
  }
  //
  for(unsigned int ireco=0; ireco<ElectronPt->size(); ireco++){
    if( elRisoCheck(ireco) ){
      TLorentzVector ElectronCand;
      ElectronCand.SetPtEtaPhiM(  elPtcorr(ireco), ElectronEta->at(ireco), ElectronPhi->at(ireco), 0  );
      if( JetCand.DeltaR(ElectronCand)<0.5 ) return false;
    }
  }
  //
  return true;
}
// // //
bool analysisClass::jetROverlapCheck(unsigned int iJetR){
  //
  TLorentzVector JetCand;
  JetCand.SetPtEtaPhiM( jetPtcorr(iJetR), PFJetEta->at(iJetR), PFJetPhi->at(iJetR), 0  );
  //
  for(unsigned int ireco=0; ireco<HPSTauPt->size(); ireco++){
    if( tauRisoCheck(ireco) ){
      TLorentzVector TauCand;
      TauCand.SetPtEtaPhiM(  tauPtcorr(ireco), HPSTauEta->at(ireco), HPSTauPhi->at(ireco), 0  ); 
      if( JetCand.DeltaR(TauCand)<0.5 ) return false;//overlap with a tau
    }
  }
  //
  for(unsigned int ireco=0; ireco<MuonPt->size(); ireco++){
    if( muRisoCheck(ireco) ){
      TLorentzVector MuonCand;
      MuonCand.SetPtEtaPhiM(  muPtcorr(ireco), MuonEta->at(ireco), MuonPhi->at(ireco), 0  );
      if( JetCand.DeltaR(MuonCand)<0.5 ) return false;//overlap with a mu
    }
  }
  //
  for(unsigned int ireco=0; ireco<ElectronPt->size(); ireco++){
    if( elRisoCheck(ireco) ){
      TLorentzVector ElectronCand;
      ElectronCand.SetPtEtaPhiM(  elPtcorr(ireco), ElectronEta->at(ireco), ElectronPhi->at(ireco), 0  );
      if( JetCand.DeltaR(ElectronCand)<0.5 ) return false;//overlap with an electron
    }
  }
  //
  return true;//no overlap
}
// // //
bool analysisClass::tauRisoCheck(unsigned int iTauR){
  if( !tauRCheck(iTauR) ) return false;
  if( !removeOverlaps ) return true;
  //
  //https://hypernews.cern.ch/HyperNews/CMS/get/AUX/2012/09/09/11:07:15-23577-slides_Tau2012_v1.pdf -- page 6
  // All charged hadrons and strips are required to be contained within a cone of size  DeltaR = 2.8GeV / pT
  //
  TLorentzVector TauCand;
  TauCand.SetPtEtaPhiM(  tauPtcorr(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0  ); 
  //
  for(unsigned int ireco=0; ireco<MuonPt->size(); ireco++){
    if( muRisoCheck(ireco) ){
      TLorentzVector MuonCand;
      MuonCand.SetPtEtaPhiM(  muPtcorr(ireco), MuonEta->at(ireco), MuonPhi->at(ireco), 0  );
      if( TauCand.DeltaR(MuonCand)<0.3 ) return false;
    }
  }
  for(unsigned int ireco=0; ireco<ElectronPt->size(); ireco++){
    if( elRisoCheck(ireco) ){
      TLorentzVector ElectronCand;
      ElectronCand.SetPtEtaPhiM(  elPtcorr(ireco), ElectronEta->at(ireco), ElectronPhi->at(ireco), 0  );
      if( TauCand.DeltaR(ElectronCand)<0.3 ) return false;
    }
  }
  //
  return true;
}
// // //
bool analysisClass::vertexRCheck(unsigned int iVertexR){
  //
  //http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/Leptoquarks/RootTupleMakerV2/src/RootTupleMakerV2_EventSelection.cc?revision=1.11&view=markup
  //http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/Leptoquarks/RootTupleMakerV2/src/RootTupleMakerV2_Vertex.cc?revision=1.1&view=markup
  //http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/Leptoquarks/RootTupleMakerV2/src/RootTupleMakerV2_EventSelection.cc?revision=1.11&view=markup
  // VertexMinimumNDOF = cms.uint32(4),
  // VertexMaxAbsZ     = cms.double(24.),
  // VertexMaxd0       = cms.double(2.),
  //for( reco::VertexCollection::const_iterator it=primaryVertices->begin() ; it!=primaryVertices->end() ; ++it ) {
  //  if( !(it->isFake()) && it->ndof() > vtxMinNDOF &&
  //	fabs(it->z()) <= vtxMaxAbsZ && fabs(it->position().rho()) <= vtxMaxd0
  //	) *isprimaryvertex.get() = true;
  //}
  //
  if( VertexIsFake->at(iVertexR)        ) return false;
  if( VertexNDF->at(iVertexR)<=4        ) return false;
  if( fabs(VertexZ->at(iVertexR))>24.0  ) return false;
  if( fabs(VertexRho->at(iVertexR))>2.0 ) return false;
  //
  return true;
}
// // //
double analysisClass::elISOEffectiveAreaCone04( double eta ){
  // 2012 values
  if( fabs(eta)<=1.0   ) return (double)(0.21);
  if( fabs(eta)<=1.479 ) return (double)(0.21);
  if( fabs(eta)<=2.0   ) return (double)(0.11);
  if( fabs(eta)<=2.2   ) return (double)(0.14);
  if( fabs(eta)<=2.3   ) return (double)(0.18);
  if( fabs(eta)<=2.4   ) return (double)(0.19);
  return (double)(0.26);
}
// // //
double analysisClass::elISOEffectiveAreaCone03( double eta ){
  // 2012 values
  if( fabs(eta)<=1.0   ) return (double)(0.13);
  if( fabs(eta)<=1.479 ) return (double)(0.14);
  if( fabs(eta)<=2.0   ) return (double)(0.07);
  if( fabs(eta)<=2.2   ) return (double)(0.09);
  if( fabs(eta)<=2.3   ) return (double)(0.11);
  if( fabs(eta)<=2.4   ) return (double)(0.11);
  return (double)(0.14);
}
// // //
bool analysisClass::passEventFilter(){
  ////isBPTX0           // (BPTX: Beam Pickup Timing eXperiment) coincidence firing of the BPTX technical triggers on Zplus & Zminus sides        
  ////isPrimaryVertex   // at least one well identified primary vertex                                                                    
  ////isBeamScraping    // ignore beam scraping events                                                                                      
  ////passBeamHaloFilterTight // CSCTightHaloId
  //
  //return true;
  if( isData==true ){ //Applied only to data.
    if( isBPTX0==false                               ) return false; 
    //
    if( passEcalDeadCellTriggerPrimitiveFilter==true ) return false;
    if( passEcalDeadCellBoundaryEnergyFilter==true   ) return false;
    if( passEcalLaserCorrFilter==true                ) return false;
    if( passTrackingFailureFilter==true              ) return false;
    if( passBadEESupercrystalFilter==true            ) return false;
  }
  //
  if( passLogErrorTooManyClusters==false ) return false; 
  if( passManyStripClus53X==false        ) return false; 
  if( passTooManyStripClus53X==false     ) return false; 
  //
  if( isPrimaryVertex==false         ) return false;
  if( isBeamScraping==true           ) return false;
  if( passHBHENoiseFilter==false     ) return false;
  if( passBeamHaloFilterTight==false ) return false;
  //
  //
  return true;
}
// // //
bool analysisClass::isMuTauDR0p35() //uses given Definition, including DeltaR separation
{
  TLorentzVector Tau;
  TLorentzVector Mu;
  //
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if(!muRisoCheck(iMuR)       ) continue;
    for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
      if(!tauRisoCheck(iTauR)) continue;
      Mu.SetPtEtaPhiM( muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0 );
      Tau.SetPtEtaPhiM( tauPtcorr(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
      if( Mu.DeltaR(Tau)>0.35 ) return true;
    }
  }
  // if no mu_tau found    
  return false;
}
// // //
bool analysisClass::isMuTauDR0p30() //uses given Definition, including DeltaR separation
{
  TLorentzVector Tau;
  TLorentzVector Mu;
  //
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if(!muRisoCheck(iMuR)       ) continue;
    for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
      if(!tauRisoCheck(iTauR)) continue;
      Mu.SetPtEtaPhiM( muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0 );
      Tau.SetPtEtaPhiM( tauPtcorr(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
      if( Mu.DeltaR(Tau)>0.30 ) return true;
    }
  }
  // if no mu_tau found    
  return false;
}
// // //
bool analysisClass::isMuMuDR0p30() //uses given Definition, including DeltaR separation
{
  TLorentzVector Mu1;
  TLorentzVector Mu2;
  //
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if( !muRisoCheck(iMuR)       ) continue;
    for(unsigned int iMuR2=(iMuR+1);  iMuR2<MuonPt->size();     iMuR2++){
      if( !muRisoCheck(iMuR2)       ) continue;
      Mu1.SetPtEtaPhiM( muPtcorr(iMuR),  MuonEta->at(iMuR),  MuonPhi->at(iMuR), 0 );
      Mu2.SetPtEtaPhiM( muPtcorr(iMuR2), MuonEta->at(iMuR2), MuonPhi->at(iMuR2), 0 );
      if( Mu1.DeltaR(Mu2)>0.30 ) return true;
    }
  }
  // if no mu_mu found    
  return false;
}
// // //
bool analysisClass::isMuEl_NoTau_DR0p30() //uses given Definition, including DeltaR separation
{
  TLorentzVector El;
  TLorentzVector Mu;
  //
  for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
    if( tauRisoCheck(iTauR) ) return false;
  }
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if(!muRisoCheck(iMuR)       ) continue;
    for(unsigned int iElR=0; iElR<ElectronPt->size(); iElR++){
      if(!elRisoCheck(iElR)) continue;
      Mu.SetPtEtaPhiM( muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0 );
      El.SetPtEtaPhiM( elPtcorr(iElR), ElectronEta->at(iElR), ElectronPhi->at(iElR), 0 );
      if( Mu.DeltaR(El)>0.30 ) return true;
    }
  }
  // if no mu_el found    
  return false;
}
// // //
bool analysisClass::isZToMuMu(){
  //
  double Zmass=91.1876;
  TLorentzVector Mu1;
  TLorentzVector Mu2;
  Mu1.SetPtEtaPhiM( 0, 0, 0, 0 );
  Mu2.SetPtEtaPhiM( 0, 0, 0, 0 );
  //
  for(unsigned int iMu1=0; iMu1<MuonPt->size(); iMu1++){//loop over all mu1
    if( !muRisoCheck( iMu1 )  ) continue;
    Mu1.SetPtEtaPhiM( muPtcorr(iMu1), MuonEta->at(iMu1), MuonPhi->at(iMu1), 0 );
    //
    for(unsigned int iMu2=0; iMu2<MuonPt->size(); iMu2++){//loop over all mu2
      if(  iMu2 == iMu1                                     ) continue;
      if(  MuonCharge->at( iMu2 ) == MuonCharge->at( iMu1 ) ) continue;
      if( !muRisoCheck( iMu2 )                   ) continue;
      Mu2.SetPtEtaPhiM( muPtcorr(iMu2), MuonEta->at(iMu2), MuonPhi->at(iMu2), 0 );
      //
      //if( fabs((Mu1+Mu2).M()-Zmass)<5 ) return true; //check if Mu-Mu Mass is within 5 GeV of Zmass.
      if( fabs((Mu1+Mu2).M()-Zmass)<Zmass*0.10 ) return true; //check if Mu-Mu Mass is within 10% of Zmass.
    }
  }
  return false;
}
// // //
bool analysisClass::isZToTauTau(){
  //
  double Zmass=91.1876;
  TLorentzVector Tau1;
  TLorentzVector Tau2;
  Tau1.SetPtEtaPhiM( 0, 0, 0, 0 );
  Tau2.SetPtEtaPhiM( 0, 0, 0, 0 );
  //
  for(unsigned int iTau1=0; iTau1<HPSTauPt->size(); iTau1++){//loop over all tau1
    if( !tauRisoCheck( iTau1 )  ) continue;
    Tau1.SetPtEtaPhiM( tauPtcorr(iTau1), HPSTauEta->at(iTau1), HPSTauPhi->at(iTau1), 0 );
    //
    for(unsigned int iTau2=0; iTau2<HPSTauPt->size(); iTau2++){//loop over all tau2
      if(  iTau2 == iTau1                                         ) continue;
      if(  HPSTauCharge->at( iTau2 ) == HPSTauCharge->at( iTau1 ) ) continue;
      if( !tauRisoCheck( iTau2 )                 ) continue;
      Tau2.SetPtEtaPhiM( tauPtcorr(iTau2), HPSTauEta->at(iTau2), HPSTauPhi->at(iTau2), 0 );
      //
      if( fabs((Tau1+Tau2).M()-Zmass)<5 ) return true; //check if Tau-Tau Mass is within 5 GeV of Zmass.
    }
  }
  return true;
}
// // //
double analysisClass::safePileupWeights()
{ 
  if( isData                              ) return 1;       // if DATA
  //
  if( PileUpInteractionsTrue->at(1)>=60.0 ){                // if TRUEinTimePU >= 60
    std::cout<<"WARNING::PileUpInteractionsTrue: "<<PileUpInteractionsTrue->at(1)<<"  getPileupWeight: "<<getPileupWeight( PileUpInteractionsTrue->at(1), isData )<<std::endl; 
    return 0;
  }
  // TRUEinTimePU >= 60 should NEVER happen in 2012: In MC2012 DR53X, PU S10 is used with PU [0,59.9999]
  // see: https://twiki.cern.ch/twiki/bin/view/CMS/Pileup_MC_Gen_Scenarios#2012_Pileup_Scenario_s
  double PileUpInteractionsTrue_=-1;
  //PileUpInteractionsTrue_=ceil(PileUpInteractionsTrue->at(1));
  //if( fabs(ceil(PileUpInteractionsTrue->at(1))-PileUpInteractionsTrue->at(1)) > fabs(floor(PileUpInteractionsTrue->at(1))-PileUpInteractionsTrue->at(1)) ){
  //PileUpInteractionsTrue_=floor(PileUpInteractionsTrue->at(1)); 
  //}
  //PileUpInteractionsTrue_=ceil(PileUpInteractionsTrue->at(1));
  //return getPileupWeight( PileUpInteractionsTrue_, isData );  // if MC
  return getPileupWeight( PileUpInteractionsTrue->at(1), isData);
}
// // //
double analysisClass::SingleMu_TriggerEfficiencyWeights_Preselection(){
  //
  if( isData ) return 1;
  //
  return IsoMu24e2p1_Eff_Preselection();
}
// // //
double analysisClass::SingleMu_TriggerEfficiencyWeights(){
  //
  if( isData             ) return 1;
  if( ltemMuTau.size()<2 ) return 0;
  //
  return IsoMu24e2p1_Eff( muPtcorr(ltemMuTau[0]), MuonEta->at(ltemMuTau[0]) );
}
// // //
double analysisClass::IsoMu24e2p1_Eff_Preselection( ){
  //
  double allFail_MC=1;
  double allFail_Data=1;
  for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
    if( !muRisoCheck(iMuR) ) continue;
    if( RecoHLTdeltaRmin_SingleMuTrigger(iMuR)<0.15 ){
      allFail_MC*=(1-IsoMu24e2p1_Eff_MC( muPtcorr(iMuR), MuonEta->at(iMuR) ));
      allFail_Data*=(1-IsoMu24e2p1_Eff_Data( muPtcorr(iMuR), MuonEta->at(iMuR) ));
    }
  }
  //
  return (double)((1-allFail_Data)/(1-allFail_MC));
}
// // //
double analysisClass::Mu40e2p1_ScaleFactor( double MuPt, double MuEta ){
  //
  if( fabs(MuEta)<0.9 && MuPt>45 ) return (double)(Mu40e2p1_Eff_Data(MuPt,MuEta)/Mu40e2p1_Eff_MC(MuPt,MuEta));
  if( fabs(MuEta)<1.2 && MuPt>45 ) return (double)(Mu40e2p1_Eff_Data(MuPt,MuEta)/Mu40e2p1_Eff_MC(MuPt,MuEta));
  if( fabs(MuEta)<2.1 && MuPt>45 ) return (double)(Mu40e2p1_Eff_Data(MuPt,MuEta)/Mu40e2p1_Eff_MC(MuPt,MuEta));
  //
  return 0;
}
// // //
double analysisClass::Mu40e2p1_Eff_MC( double MuPt, double MuEta ){
  //
  // https://indico.cern.ch/getFile.py/access?contribId=2&resId=0&materialId=slides&confId=228197
  double mc_eta0p9 = ((0.9593*LumiA)+(0.9593*LumiB)+(0.9592*LumiC)+(0.9830*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  double mc_eta1p2 = ((0.8770*LumiA)+(0.8769*LumiB)+(0.8779*LumiC)+(0.8730*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  double mc_eta2p1 = ((0.8342*LumiA)+(0.8342*LumiB)+(0.8336*LumiC)+(0.8304*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  //
  if( fabs(MuEta)<0.9 && MuPt>45 ) return mc_eta0p9;
  if( fabs(MuEta)<1.2 && MuPt>45 ) return mc_eta1p2;
  if( fabs(MuEta)<2.1 && MuPt>45 ) return mc_eta2p1;
  //
  return 0;
}
// // //
double analysisClass::Mu40e2p1_Eff_Data( double MuPt, double MuEta ){
  //
  // https://indico.cern.ch/getFile.py/access?contribId=2&resId=0&materialId=slides&confId=228197
  double data_eta0p9 = ((0.9401*LumiA)+(0.9376*LumiB)+(0.9417*LumiC)+(0.9425*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  double data_eta1p2 = ((0.8437*LumiA)+(0.8396*LumiB)+(0.8463*LumiC)+(0.8384*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  double data_eta2p1 = ((0.8217*LumiA)+(0.8136*LumiB)+(0.8314*LumiC)+(0.8266*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  //
  if( fabs(MuEta)<0.9 && MuPt>45 ) return data_eta0p9;
  if( fabs(MuEta)<1.2 && MuPt>45 ) return data_eta1p2;
  if( fabs(MuEta)<2.1 && MuPt>45 ) return data_eta2p1;
  //
  return 0;
}
// // //
double analysisClass::IsoMu24e2p1_Eff_MC( double MuPt, double MuEta ){
  //
  // https://indico.cern.ch/getFile.py/access?contribId=2&resId=0&materialId=slides&confId=228197
  double mc_eta0p9 = ((0.9346*LumiA)+(0.9346*LumiB)+(0.9492*LumiC)+(0.93125*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  double mc_eta1p2 = ((0.8573*LumiA)+(0.8572*LumiB)+(0.8679*LumiC)+(0.85067*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  double mc_eta2p1 = ((0.8077*LumiA)+(0.8077*LumiB)+(0.8160*LumiC)+(0.80169*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  //
  if( fabs(MuEta)<0.9 && MuPt>25 ) return mc_eta0p9;
  if( fabs(MuEta)<1.2 && MuPt>25 ) return mc_eta1p2;
  if( fabs(MuEta)<2.1 && MuPt>25 ) return mc_eta2p1;
  //
  return 0;
}
// // //
double analysisClass::IsoMu24e2p1_Eff_Data( double MuPt, double MuEta ){
  //
  // https://indico.cern.ch/getFile.py/access?contribId=2&resId=0&materialId=slides&confId=228197
  double data_eta0p9 = ((0.8934*LumiA)+(0.9157*LumiB)+(0.9341*LumiC)+(0.9140*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  double data_eta1p2 = ((0.8168*LumiA)+(0.8245*LumiB)+(0.8408*LumiC)+(0.8180*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  double data_eta2p1 = ((0.7923*LumiA)+(0.7927*LumiB)+(0.8177*LumiC)+(0.7995*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  //
  if( fabs(MuEta)<0.9 && MuPt>25 ) return data_eta0p9;
  if( fabs(MuEta)<1.2 && MuPt>25 ) return data_eta1p2;
  if( fabs(MuEta)<2.1 && MuPt>25 ) return data_eta2p1;
  //
  return 0;
}
// // //
double analysisClass::IsoMu24e2p1_Eff( double MuPt, double MuEta ){ // these are the scale factors
  // For 2012BCD DATA/MC scale factors  
  // HLT_IsoMu24_eta2p1_v13_MC_v13v14v15_DATA :: hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15
  // My calculation
  //if( fabs(MuEta)<1.2  && MuPt>24 ) return 0.9790;//.................//barrel
  //if( fabs(MuEta)>=1.2 && fabs(MuEta)<2.1 &&  MuPt>24) return 0.9967;//endcap
  //
  // Efficiencies: https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceEffs#2012_data
  // Run Ranges:   https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2012Analysis#Analysis_using_the_Golden_JSON_f
  //
  // 2012A 190456-193621                     // 2012B 193833-196531
  // fabs(MuEta)<0.9 & MuPt>25 : 0.9560      // fabs(MuEta)<0.9 & MuPt>25 : 0.9798
  // fabs(MuEta)<1.2 & MuPt>25 : 0.9528      // fabs(MuEta)<1.2 & MuPt>25 : 0.9618
  // fabs(MuEta)<2.1 & MuPt>25 : 0.9809      // fabs(MuEta)<2.1 & MuPt>25 : 0.9814
  //
  // 2012C 198022-203746                     // 2012D 203768-208686
  // fabs(MuEta)<0.9 & MuPt>25 : 0.9841      // fabs(MuEta)<0.9 & MuPt>25 : 0.9815
  // fabs(MuEta)<1.2 & MuPt>25 : 0.9688      // fabs(MuEta)<1.2 & MuPt>25 : 0.9616
  // fabs(MuEta)<2.1 & MuPt>25 : 1.0021      // fabs(MuEta)<2.1 & MuPt>25 : 0.9972
  //
  double sf_eta0p9 = ((0.9560*LumiA)+(0.9798*LumiB)+(0.9841*LumiC)+(0.9815*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  double sf_eta1p2 = ((0.9528*LumiA)+(0.9618*LumiB)+(0.9688*LumiC)+(0.9616*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  double sf_eta2p1 = ((0.9809*LumiA)+(0.9814*LumiB)+(1.0021*LumiC)+(0.9972*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  //
  if( fabs(MuEta)<0.9 && MuPt>25 ) return sf_eta0p9;
  if( fabs(MuEta)<1.2 && MuPt>25 ) return sf_eta1p2;
  if( fabs(MuEta)<2.1 && MuPt>25 ) return sf_eta2p1;
  //
  return 0;
}
// // //
double analysisClass::Mu24e2p1_Eff_MC( double MuPt, double MuEta ){
  //
  // https://hypernews.cern.ch/HyperNews/CMS/get/muon/901/1/1/1/1/1.html
  //> x min   x avg   x max           data     - err / + err           mc      - err / + err          scale    - err / + err  
  //> ------- ------- -------         ------  --------/--------       ------  --------/--------       ------- --------/--------
  //>  -2.1   -1.84   -1.60          0.7864  -0.0005 / +0.0005       0.7928  -0.0007 / +0.0007       0.9920  -0.0011 / +0.0011       
  //> -1.60   -1.39   -1.20          0.8470  -0.0004 / +0.0004       0.8565  -0.0006 / +0.0006       0.9889  -0.0009 / +0.0009       
  //
  //> -1.20   -1.05   -0.90          0.8478  -0.0005 / +0.0005       0.8804  -0.0006 / +0.0006       0.9629  -0.0009 / +0.0009       
  //
  //> -0.90   -0.75   -0.60          0.9426  -0.0003 / +0.0003       0.9604  --0.0000 / +0.0004      0.9815  -0.0003 / +0.0005       
  //> -0.60   -0.45   -0.30          0.9612  -0.0002 / +0.0002       0.9730  -0.0003 / +0.0003       0.9879  -0.0004 / +0.0004       
  //> -0.30   -0.25   -0.20          0.8898  -0.0007 / +0.0007       0.9370  -0.0007 / +0.0007       0.9496  -0.0010 / +0.0010       
  //> -0.20    0.00    0.20          0.9568  -0.0002 / +0.0002       0.9703  -0.0003 / +0.0003       0.9862  -0.0003 / +0.0003       
  //>  0.20    0.25    0.30          0.9079  -0.0006 / +0.0006       0.9432  -0.0007 / +0.0007       0.9626  -0.0010 / +0.0010       
  //>  0.30    0.45    0.60          0.9599  -0.0002 / +0.0002       0.9737  -0.0003 / +0.0003       0.9858  -0.0004 / +0.0004       
  //>  0.60    0.75    0.90          0.9444  -0.0003 / +0.0003       0.9595  -0.0004 / +0.0004       0.9842  -0.0005 / +0.0005       
  //  
  //>  0.90    1.05    1.20          0.8452  -0.0005 / +0.0005       0.8778  -0.0006 / +0.0006       0.9628  -0.0009 / +0.0009       
  //
  //>  1.20    1.39    1.60          0.8200  -0.0005 / +0.0005       0.8477  -0.0007 / +0.0006       0.9674  -0.0009 / +0.0009       
  //>  1.60    1.84    2.10          0.8420  -0.0004 / +0.0004       0.8272  -0.0007 / +0.0007       1.0179  -0.0010 / +0.0010  
  double mc_eta0p9 = (0.9604+0.9730+0.9370+0.9703+0.9432+0.9737+0.9595)/7.0;//0.9595857  +/- 0.0002
  double mc_eta1p2 = (0.8778+0.8804)/2.0;//0.8791
  double mc_eta2p1 = (0.7928+0.8565+0.8477+0.8272)/4.0;//0.83105
  //
  if( fabs(MuEta)<0.9 && MuPt>25 ) return mc_eta0p9;
  if( fabs(MuEta)<1.2 && MuPt>25 ) return mc_eta1p2;
  if( fabs(MuEta)<2.1 && MuPt>25 ) return mc_eta2p1;
  //
  return 0;
}
// // //
double analysisClass::Mu24e2p1_Eff_Data( double MuPt, double MuEta ){
  //
  // https://hypernews.cern.ch/HyperNews/CMS/get/muon/901/1/1/1/1/1.html
  double mc_eta0p9 = (0.9426+0.9612+0.8898+0.9568+0.9079+0.9599+0.9444)/7.0;//0.93751
  double mc_eta1p2 = (0.8478+0.8452)/2.0;//0.8465
  double mc_eta2p1 = (0.7864+0.8470+0.8200+0.8420)/4.0;//0.82385
  //
  if( fabs(MuEta)<0.9 && MuPt>25 ) return mc_eta0p9;
  if( fabs(MuEta)<1.2 && MuPt>25 ) return mc_eta1p2;
  if( fabs(MuEta)<2.1 && MuPt>25 ) return mc_eta2p1;
  //
  return 0;
}
// // //
double analysisClass::Mu24e2p1_ScaleFactor( double MuPt, double MuEta ){
  //
  if( fabs(MuEta)<0.9 && MuPt>25 ) return (double)(Mu24e2p1_Eff_Data(MuPt,MuEta)/Mu24e2p1_Eff_MC(MuPt,MuEta));
  if( fabs(MuEta)<1.2 && MuPt>25 ) return (double)(Mu24e2p1_Eff_Data(MuPt,MuEta)/Mu24e2p1_Eff_MC(MuPt,MuEta));
  if( fabs(MuEta)<2.1 && MuPt>25 ) return (double)(Mu24e2p1_Eff_Data(MuPt,MuEta)/Mu24e2p1_Eff_MC(MuPt,MuEta));
  //
  return 0;
}
// // //
double analysisClass::DoubleMu_TriggerEfficiencyWeights(){
  //
  if( isData            ) return 1;
  if( ltemMuMu.size()<2 ) return 0;
  //
  double TotalSF=1;
  double SFmu1=Mu17_Mu8_Eff( muPtcorr(ltemMuMu[0]), MuonEta->at(ltemMuMu[0]) );
  double SFmu2=Mu17_Mu8_Eff( muPtcorr(ltemMuMu[1]), MuonEta->at(ltemMuMu[1]) );
  TotalSF=SFmu1*SFmu2;
  //
  return TotalSF;
}
// // //
double analysisClass::Mu17_Mu8_Eff( double MuPt, double MuEta ){
  //
  // Using Mu40_eta2p1 for ACD
  // Using Mu17lef for B
  double sf_eta0p9 = ((0.9799*LumiA)+(0.9884*LumiB)+(0.9817*LumiC)+(0.9830*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  double sf_eta1p2 = ((0.9621*LumiA)+(0.9836*LumiB)+(0.9640*LumiC)+(0.9604*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  double sf_eta2p1 = ((0.9851*LumiA)+(0.9879*LumiB)+(0.9973*LumiC)+(0.9955*LumiD))/(LumiA+LumiB+LumiC+LumiD);
  //
  if( fabs(MuEta)<0.9 && MuPt>20 ) return sf_eta0p9;
  if( fabs(MuEta)<1.2 && MuPt>20 ) return sf_eta1p2;
  if( fabs(MuEta)<2.4 && MuPt>20 ) return sf_eta2p1;
  //
  return 0;
}
// // //
bool  analysisClass::tauRMigrationCheck( unsigned int x ){};
bool  analysisClass::muRMigrationCheck( unsigned int x){};
// // //
double analysisClass::muJetDeltaRmin( unsigned int iMuR ){
  TLorentzVector mucand, jetcand;
  double muJetDeltaRmin=9999.0;
  mucand.SetPtEtaPhiM( muPtcorr(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0 );
  for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
    jetcand.SetPtEtaPhiM( jetPtcorr(iJetR),  PFJetEta->at(iJetR),  PFJetPhi->at(iJetR),  0 );
    if( mucand.DeltaR(jetcand)<0.05 || (PFJetMuonEnergyFraction->at(iJetR)>0.70 && mucand.DeltaR(jetcand)<0.5) )  continue;
    if( mucand.DeltaR(jetcand)<0.5 ){
      jetcand=(jetcand-mucand);
    }
    if( jetcand.Pt()<20       ) continue;
    if( fabs(jetcand.Eta())>4 ) continue;
    if( mucand.DeltaR(jetcand)<muJetDeltaRmin ){ muJetDeltaRmin=mucand.DeltaR(jetcand);}
  }
  //
  return muJetDeltaRmin;
}
// // //
double analysisClass::tauJetDeltaRmin( unsigned int iTauR ){
  TLorentzVector taucand, jetcand;
  double tauJetDeltaRmin=9999.0;
  taucand.SetPtEtaPhiM( tauPtcorr(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
  for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
    if( jetPtcorr(iJetR)<20       ) continue;
    if( fabs(PFJetEta->at(iJetR))>4 ) continue;
    jetcand.SetPtEtaPhiM( jetPtcorr(iJetR),  PFJetEta->at(iJetR),  PFJetPhi->at(iJetR),  0 );
    if( taucand.DeltaR(jetcand)<tauJetDeltaRmin && taucand.DeltaR(jetcand)>0.2 ){ tauJetDeltaRmin=taucand.DeltaR(jetcand);}
  }
  //
  return tauJetDeltaRmin;
}
// // //
void analysisClass::ResetAllSFs_KEEPERROR(){
  //muon prompt rate scale factors (none for muon fake rate)
  //if( muPRdR3B_SF>1.0 ){ muPRdR3B_SFerr = muPRdR3B_SFerr*(1.0/muPRdR3B_SF); } 
  //muPRdR3B_SF    = 1; 
  //if( muPRdR2B_SF>1.0 ){ muPRdR2B_SFerr = muPRdR2B_SFerr*(1.0/muPRdR2B_SF); } 
  //muPRdR2B_SF    = 1;
  //if( muPRdR1B_SF>1.0 ){ muPRdR1B_SFerr = muPRdR1B_SFerr*(1.0/muPRdR1B_SF); } 
  //muPRdR1B_SF    = 1;
  //if( muPRdR3E_SF>1.0 ){ muPRdR3E_SFerr = muPRdR3E_SFerr*(1.0/muPRdR3E_SF); } 
  //muPRdR3E_SF    = 1;
  //if( muPRdR2E_SF>1.0 ){ muPRdR2E_SFerr = muPRdR2E_SFerr*(1.0/muPRdR2E_SF); } 
  //muPRdR2E_SF    = 1;
  //if( muPRdR1E_SF>1.0 ){ muPRdR1E_SFerr = muPRdR2E_SFerr*(1.0/muPRdR1E_SF); } 
  //muPRdR1E_SF    = 1;
  //
  //tau prompt rate scale factors
  //if( tauPRdR3B_SF>1.0 ){ tauPRdR3B_SFerr = tauPRdR3B_SFerr*(1.0/tauPRdR3B_SF); }
  //tauPRdR3B_SF    = 1;
  //if( tauPRdR2B_SF>1.0 ){ tauPRdR2B_SFerr = tauPRdR2B_SFerr*(1.0/tauPRdR2B_SF); }
  //tauPRdR2B_SF    = 1;
  //if( tauPRdR1B_SF>1.0 ){ tauPRdR1B_SFerr = tauPRdR1B_SFerr*(1.0/tauPRdR1B_SF); }
  //tauPRdR1B_SF    = 1;
  //if( tauPRdR3E_SF>1.0 ){ tauPRdR3E_SFerr = tauPRdR3E_SFerr*(1.0/tauPRdR3E_SF); }
  //tauPRdR3E_SF    = 1;
  //if( tauPRdR2E_SF>1.0 ){ tauPRdR2E_SFerr = tauPRdR2E_SFerr*(1.0/tauPRdR2E_SF); }
  //tauPRdR2E_SF    = 1;
  //if( tauPRdR1E_SF>1.0 ){ tauPRdR1E_SFerr = tauPRdR1E_SFerr*(1.0/tauPRdR1E_SF); }
  //tauPRdR1E_SF    = 1;
  //tauPRdR2B_SF    = tauPRdR3B_SF;
  //tauPRdR2B_SFerr = tauPRdR3B_SFerr;
  //tauPRdR1B_SF    = tauPRdR3B_SF;
  //tauPRdR1B_SFerr = tauPRdR3B_SFerr;
  //
  //tauPRdR2E_SF    = tauPRdR3E_SF;
  //tauPRdR2E_SFerr = tauPRdR3E_SFerr;
  //tauPRdR1E_SF    = tauPRdR3E_SF;
  //tauPRdR1E_SFerr = tauPRdR3E_SFerr;
  //
  //
  //tau fake rate scale factors
  //if( tauFRdR3B_SF>1.0 ){ tauFRdR3B_SFerr = tauFRdR3B_SFerr*(1.0/tauFRdR3B_SF); }
  //tauFRdR3B_SF    = 1;
  //if( tauFRdR2B_SF>1.0 ){ tauFRdR2B_SFerr = tauFRdR2B_SFerr*(1.0/tauFRdR2B_SF); }
  //tauFRdR2B_SF    = 1;
  //if( tauFRdR1B_SF>1.0 ){ tauFRdR1B_SFerr = tauFRdR1B_SFerr*(1.0/tauFRdR1B_SF); }
  //tauFRdR1B_SF    = 1;
  //if( tauFRdR3E_SF>1.0 ){ tauFRdR3E_SFerr = tauFRdR3E_SFerr*(1.0/tauFRdR3E_SF); }
  //tauFRdR3E_SF    = 1;
  //if( tauFRdR2E_SF>1.0 ){ tauFRdR2E_SFerr = tauFRdR2E_SFerr*(1.0/tauFRdR2E_SF); }
  //tauFRdR2E_SF    = 1;
  //if( tauFRdR1E_SF>1.0 ){ tauFRdR1E_SFerr = tauFRdR1E_SFerr*(1.0/tauFRdR1E_SF); }
  //tauFRdR1E_SF    = 1;
  tauFRdR2B_SF    = tauFRdR3B_SF;
  tauFRdR2B_SFerr = tauFRdR3B_SFerr;
  tauFRdR1B_SF    = tauFRdR3B_SF;
  tauFRdR1B_SFerr = tauFRdR3B_SFerr;
  //
  tauFRdR2E_SF    = tauFRdR3E_SF;
  tauFRdR2E_SFerr = tauFRdR3E_SFerr;
  tauFRdR1E_SF    = tauFRdR3E_SF;
  tauFRdR1E_SFerr = tauFRdR3E_SFerr;
}
// // //
void analysisClass::ResetAllSFs(){
  muPRdR3B_SF    = 1;   muPRdR3B_SFerr = 0;//muon prompt rate scale factors (none for muon fake rate)
  muPRdR2B_SF    = 1;   muPRdR2B_SFerr = 0;
  muPRdR1B_SF    = 1;   muPRdR1B_SFerr = 0;
  muPRdR3E_SF    = 1;   muPRdR3E_SFerr = 0;
  muPRdR2E_SF    = 1;   muPRdR2E_SFerr = 0;
  muPRdR1E_SF    = 1;   muPRdR1E_SFerr = 0;
  //
  tauPRdR3B_SF    = 1;   tauPRdR3B_SFerr = 0;//tau prompt rate scale factors
  tauPRdR2B_SF    = 1;   tauPRdR2B_SFerr = 0;
  tauPRdR1B_SF    = 1;   tauPRdR1B_SFerr = 0;
  tauPRdR3E_SF    = 1;   tauPRdR3E_SFerr = 0;
  tauPRdR2E_SF    = 1;   tauPRdR2E_SFerr = 0;
  tauPRdR1E_SF    = 1;   tauPRdR1E_SFerr = 0;
  //
  tauFRdR3B_SF    = 1;   tauFRdR3B_SFerr = 0;//tau fake rate scale factors
  tauFRdR2B_SF    = 1;   tauFRdR2B_SFerr = 0;
  tauFRdR1B_SF    = 1;   tauFRdR1B_SFerr = 0;
  tauFRdR3E_SF    = 1;   tauFRdR3E_SFerr = 0;
  tauFRdR2E_SF    = 1;   tauFRdR2E_SFerr = 0;
  tauFRdR1E_SF    = 1;   tauFRdR1E_SFerr = 0;
}
// // //
void analysisClass::InitMuonSFs(unsigned int ltemMu){
  InitMuonFunctionParameters();
  //double muPt=muPtcorr(ltemMuTau[0]);
  double muPt=muPtcorr(ltemMu);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
  // CorrFactor( double A0, double A1, double A2, double pt, double B0, double B1, double B2 ){  //
  // CorrFactor=DATA/Wjets                                                                       //
  // return (PromptRateFunction( A0, A1, A2, pt )/PromptRateFunction( B0, B1, B2, pt ));         //
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
  muPRdR3B_SF    = CorrFactor( DATAdr3Bpar1, DATAdr3Bpar2, DATAdr3Bpar3, muPt, Wdr3Bpar1, Wdr3Bpar2, Wdr3Bpar3 );
  muPRdR3B_SFerr = CorrFactorError( DATAdr3Bpar1, DATAdr3Bpar2, DATAdr3Bpar3, DATAdr3BparErr1, DATAdr3BparErr2, DATAdr3BparErr3, muPt,
				    Wdr3Bpar1, Wdr3Bpar2, Wdr3Bpar3, Wdr3BparErr1, Wdr3BparErr2, Wdr3BparErr3 );
  muPRdR2B_SF    = CorrFactor( DATAdr2Bpar1, DATAdr2Bpar2, DATAdr2Bpar3, muPt, Wdr2Bpar1, Wdr2Bpar2, Wdr2Bpar3 );
  muPRdR2B_SFerr = CorrFactorError( DATAdr2Bpar1, DATAdr2Bpar2, DATAdr2Bpar3, DATAdr2BparErr1, DATAdr2BparErr2, DATAdr2BparErr3, muPt,
				    Wdr2Bpar1, Wdr2Bpar2, Wdr2Bpar3, Wdr2BparErr1, Wdr2BparErr2, Wdr2BparErr3 );
  muPRdR1B_SF    = CorrFactor( DATAdr1Bpar1, DATAdr1Bpar2, DATAdr1Bpar3, muPt, Wdr1Bpar1, Wdr1Bpar2, Wdr1Bpar3 );
  muPRdR1B_SFerr = CorrFactorError( DATAdr1Bpar1, DATAdr1Bpar2, DATAdr1Bpar3, DATAdr1BparErr1, DATAdr1BparErr2, DATAdr1BparErr3, muPt,
				    Wdr1Bpar1, Wdr1Bpar2, Wdr1Bpar3, Wdr1BparErr1, Wdr1BparErr2, Wdr1BparErr3 );
  //-------
  muPRdR3E_SF    = CorrFactor( DATAdr3Epar1, DATAdr3Epar2, DATAdr3Epar3, muPt, Wdr3Epar1, Wdr3Epar2, Wdr3Epar3 );
  muPRdR3E_SFerr = CorrFactorError( DATAdr3Epar1, DATAdr3Epar2, DATAdr3Epar3, DATAdr3EparErr1, DATAdr3EparErr2, DATAdr3EparErr3, muPt,
				    Wdr3Epar1, Wdr3Epar2, Wdr3Epar3, Wdr3EparErr1, Wdr3EparErr2, Wdr3EparErr3 );
  muPRdR2E_SF    = CorrFactor( DATAdr2Epar1, DATAdr2Epar2, DATAdr2Epar3, muPt, Wdr2Epar1, Wdr2Epar2, Wdr2Epar3 );
  muPRdR2E_SFerr = CorrFactorError( DATAdr2Epar1, DATAdr2Epar2, DATAdr2Epar3, DATAdr2EparErr1, DATAdr2EparErr2, DATAdr2EparErr3, muPt,
				    Wdr2Epar1, Wdr2Epar2, Wdr2Epar3, Wdr2EparErr1, Wdr2EparErr2, Wdr2EparErr3 );
  muPRdR1E_SF    = CorrFactor( DATAdr1Epar1, DATAdr1Epar2, DATAdr1Epar3, muPt, Wdr1Epar1, Wdr1Epar2, Wdr1Epar3 );
  muPRdR1E_SFerr = CorrFactorError( DATAdr1Epar1, DATAdr1Epar2, DATAdr1Epar3, DATAdr1EparErr1, DATAdr1EparErr2, DATAdr1EparErr3, muPt,
				    Wdr1Epar1, Wdr1Epar2, Wdr1Epar3, Wdr1EparErr1, Wdr1EparErr2, Wdr1EparErr3 );
}
// // //
void analysisClass::InitTauSFs(){
  // Tau Prompt Rate Scale Factors (4-Sep-2013)......
  tauPRdR3B_SF    = 0.984;   tauPRdR3B_SFerr = 0.057;
  tauPRdR2B_SF    = 0.880;   tauPRdR2B_SFerr = 0.102;
  tauPRdR1B_SF    = 0.880;   tauPRdR1B_SFerr = 0.102;
  tauPRdR3E_SF    = 1.064;   tauPRdR3E_SFerr = 0.086;
  tauPRdR2E_SF    = 1.064;   tauPRdR2E_SFerr = 0.086;
  tauPRdR1E_SF    = 1.064;   tauPRdR1E_SFerr = 0.086;
  //-------
  // Tau Fake Rate Scale Factors (4-Sep-2013)
  tauFRdR3B_SF    = 1.056;   tauFRdR3B_SFerr = 0.041;
  tauFRdR2B_SF    = 1.157;   tauFRdR2B_SFerr = 0.177;
  tauFRdR1B_SF    = 1.265;   tauFRdR1B_SFerr = 0.201;
  tauFRdR3E_SF    = 0.982;   tauFRdR3E_SFerr = 0.067;
  tauFRdR2E_SF    = 0.890;   tauFRdR2E_SFerr = 0.262;
  tauFRdR1E_SF    = 0.597;   tauFRdR1E_SFerr = 0.214;
}
// // //
double analysisClass::tauFR_STcorrection( unsigned int iTauR ){
  double ST_ = ST();
  if( ST_<400 ) return 0;
  bool   isBarrel_=false;
  if( fabs(HPSTauEta->at(iTauR))<1.5 ) isBarrel_=true;
  double tauJetDeltaRmin_=tauJetDeltaRmin(iTauR);
  double slope_=0;
  //Barrel
  //3) %decrease:  1.04937916111 +/- 1.96913214217 : slope:  1.44995439579 +/- 2.72080093757 e-05
  //2) %decrease:  3.76629993651 +/- 2.97441820927 : slope:  4.64899405799 +/- 3.67152186867 e-05
  //1) %decrease:  1.61859936226 +/- 4.2575632317  : slope:  1.72144823901 +/- 4.52809688337 e-05
  //Endcap
  //1) N/A
  //2) %decrease:  8.48449655111 +/- 7.15889810989
  //3) N/A
  if( tauJetDeltaRmin_>=0.7 && isBarrel_ ) slope_=-1.5e-5;
  return slope_*(ST_-400.0);
}
// // //
double analysisClass::tauFR_STcorrectionError( unsigned int iTauR ){
  return fabs(tauFR_STcorrection( iTauR )); //100% on the slope
}
// // //
double analysisClass::tauPR_STcorrection( unsigned int iTauR ){
  double ST_ = ST();
  if( ST_<400 ) return 0;
  bool   isBarrel_=false;
  if( fabs(HPSTauEta->at(iTauR))<1.5 ) isBarrel_=true;
  double tauJetDeltaRmin_=tauJetDeltaRmin(iTauR);
  double slope_=0;
  //dr3) %decrease:  1.31878004647 +/- 0.114942769887  : slope:  7.98897373973 +/- 0.696306235944 e-05
  //dr2) %decrease:  1.64741377278 +/- 0.206149830536  : slope:  8.73170447916 +/- 1.09264559299 e-05
  //dr1) %decrease:  2.53529667149 +/- 0.34006408606   : slope:  11.2601614215 +/- 1.51034651911 e-05
  if(                          tauJetDeltaRmin_>=1.0 && isBarrel_ ) slope_=-8.0e-5;
  if( tauJetDeltaRmin_< 1.0 && tauJetDeltaRmin_>=0.8 && isBarrel_ ) slope_=-8.7e-5;
  if( tauJetDeltaRmin_< 0.8 && tauJetDeltaRmin_>=0.7 && isBarrel_ ) slope_=-11.3e-5;
  //
  //3) %decrease:  0.663781078609 +/- 0.383807341426        : slope:  4.28354916833 +/- 2.47680699427 e-05
  //2) %decrease:  N/A (0.00487712462848 +/- 31.5495380986) : slope:  0.02893124009 +/- 187.152744902 e-05
  //1) %decrease:  2.94732920613 +/- 1.21800191413          : slope:  14.953039925  +/- 6.17943567784 e-05
  if(                          tauJetDeltaRmin_>=1.0 && !isBarrel_ ) slope_=-4.3e-5;
  if( tauJetDeltaRmin_< 1.0 && tauJetDeltaRmin_>=0.8 && !isBarrel_ ) slope_=-4.3e-5;
  if( tauJetDeltaRmin_< 0.8 && tauJetDeltaRmin_>=0.7 && !isBarrel_ ) slope_=-15.0e-5;
  return slope_*(ST_-400.0);
}
// // //
double analysisClass::tauPR_STcorrectionError( unsigned int iTauR ){
  double ST_ = ST();
  if( ST_<400 ) return 0;
  bool   isBarrel_=false;
  if( fabs(HPSTauEta->at(iTauR))<1.5 ) isBarrel_=true;
  double tauJetDeltaRmin_=tauJetDeltaRmin(iTauR);
  double slope_=0;
  //dr3) %decrease:  1.31878004647 +/- 0.114942769887  : slope:  7.98897373973 +/- 0.696306235944 e-05
  //dr2) %decrease:  1.64741377278 +/- 0.206149830536  : slope:  8.73170447916 +/- 1.09264559299 e-05
  //dr1) %decrease:  2.53529667149 +/- 0.34006408606   : slope:  11.2601614215 +/- 1.51034651911 e-05
  if(                          tauJetDeltaRmin_>=1.0 && isBarrel_ ) slope_=0.7e-5;
  if( tauJetDeltaRmin_< 1.0 && tauJetDeltaRmin_>=0.8 && isBarrel_ ) slope_=1.1e-5;
  if( tauJetDeltaRmin_< 0.8 && tauJetDeltaRmin_>=0.7 && isBarrel_ ) slope_=1.5e-5;
  //
  //3) %decrease:  0.663781078609 +/- 0.383807341426        : slope:  4.28354916833 +/- 2.47680699427 e-05
  //2) %decrease:  N/A (0.00487712462848 +/- 31.5495380986) : slope:  0.02893124009 +/- 187.152744902 e-05
  //1) %decrease:  2.94732920613 +/- 1.21800191413          : slope:  14.953039925  +/- 6.17943567784 e-05
  if(                          tauJetDeltaRmin_>=1.0 && !isBarrel_ ) slope_=2.5e-5;
  if( tauJetDeltaRmin_< 1.0 && tauJetDeltaRmin_>=0.8 && !isBarrel_ ) slope_=2.5e-5;
  if( tauJetDeltaRmin_< 0.8 && tauJetDeltaRmin_>=0.7 && !isBarrel_ ) slope_=6.2e-5;
  return fabs(slope_*(ST_-400.0));
}
// // //
void analysisClass::InitTauFakeRates(){
  tauFRw_dR1B=0.113;  tauFRw_dR1B_Err=0.011;
  tauFRw_dR2B=0.114;  tauFRw_dR2B_Err=0.014;
  tauFRw_dR3B=0.124;  tauFRw_dR3B_Err=0.004;
  tauFRw_dR1E=0.154;  tauFRw_dR1E_Err=0.019;
  tauFRw_dR2E=0.173;  tauFRw_dR2E_Err=0.041;
  tauFRw_dR3E=0.169;  tauFRw_dR3E_Err=0.009;
  //
  tauFRtt_dR1B=0.113;  tauFRtt_dR1B_Err=0.008;
  tauFRtt_dR2B=0.127;  tauFRtt_dR2B_Err=0.006;
  tauFRtt_dR3B=0.140;  tauFRtt_dR3B_Err=0.003;
  tauFRtt_dR1E=0.112;  tauFRtt_dR1E_Err=0.010;
  tauFRtt_dR2E=0.132;  tauFRtt_dR2E_Err=0.011;
  tauFRtt_dR3E=0.157;  tauFRtt_dR3E_Err=0.007;
  //
  tauFRdata_dR1B=0.143;  tauFRdata_dR1B_Err=0.018;
  tauFRdata_dR2B=0.132;  tauFRdata_dR2B_Err=0.012;
  tauFRdata_dR3B=0.131;  tauFRdata_dR3B_Err=0.003;
  tauFRdata_dR1E=0.092;  tauFRdata_dR1E_Err=0.031;
  tauFRdata_dR2E=0.154;  tauFRdata_dR2E_Err=0.027;
  tauFRdata_dR3E=0.166;  tauFRdata_dR3E_Err=0.007;
}
// // //
double analysisClass::QuadSum( double A, double B ){
  return TMath::Power( (TMath::Power(A,2)+TMath::Power(B,2)), 0.5 );
}
// // //
double analysisClass::TauFakeRateCalculator( double ttrate, double ttrateErr, double wrate, double wrateErr, 
					     double stcorr, double stcorrErr, double SF, double SFerr ){
  double highBound = 0;
  double lowBound  = 0;
  double rate      = 0;
  //
  highBound= TMath::Max( ttrate+stcorr, wrate+stcorr );//pick the largest  of the two as upper bound
  lowBound = TMath::Min( ttrate+stcorr, wrate+stcorr );//pick the smallest of the two as lower bound
  rate    = (highBound+lowBound)*0.5;
  //
  if( !isData ) return rate;//for MC
  //
  // If using data!
  rate = rate*SF;
  return rate;
}
// // //
double analysisClass::TauFakeRateErrorCalculator( double ttrate, double ttrateErr, double wrate, double wrateErr,
						  double stcorr, double stcorrErr, double SF, double SFerr ){
  double highBound = 0;
  double lowBound  = 0;
  double rate      = 0;
  double rateErr   = 0;
  //
  highBound= TMath::Max( ttrate+stcorr, wrate+stcorr );//pick the largest  of the two as upper bound
  lowBound = TMath::Min( ttrate+stcorr, wrate+stcorr );//pick the smallest of the two as lower bound
  rate    = (highBound+lowBound)*0.5;
  //
  highBound= TMath::Max( ttrate+stcorr+QuadSum(stcorrErr,ttrateErr), wrate+stcorr+QuadSum(stcorrErr,wrateErr) );
  lowBound = TMath::Min( ttrate+stcorr-QuadSum(stcorrErr,ttrateErr), wrate+stcorr-QuadSum(stcorrErr,wrateErr) );
  //rateErr = (highBound-lowBound)*0.5;
  rateErr = TMath::Max( fabs(highBound-rate), fabs(rate-lowBound) );
  //
  if( !isData ) return rateErr;// for MC
  //
  // If using data!
  if( SF>0 && rate>0 )  rateErr = QuadSum( SFerr/SF, rateErr/rate )*rate*SF;
  return rateErr;
}
// // //
double analysisClass::tauFR( unsigned int iTauR ){
  double tauJetDeltaRmin_=tauJetDeltaRmin(iTauR);
  bool   isBarrel_=false;
  if( fabs(HPSTauEta->at(iTauR))<1.5 ) isBarrel_=true;
  //
  double stcorr_    = tauFR_STcorrection(iTauR);
  double stcorrErr_ = tauFR_STcorrectionError(iTauR);
  //
  if(                          tauJetDeltaRmin_>=1.0 && isBarrel_ ){
    return TauFakeRateCalculator( tauFRtt_dR3B,tauFRtt_dR3B_Err, tauFRw_dR3B,tauFRw_dR3B_Err,
				  stcorr_,stcorrErr_, tauFRdR3B_SF,tauFRdR3B_SFerr );
  }
  if( tauJetDeltaRmin_< 1.0 && tauJetDeltaRmin_>=0.8 && isBarrel_ ){
    return TauFakeRateCalculator( tauFRtt_dR2B,tauFRtt_dR2B_Err, tauFRw_dR2B,tauFRw_dR2B_Err,
                                  stcorr_,stcorrErr_, tauFRdR2B_SF,tauFRdR2B_SFerr );
  }
  if( tauJetDeltaRmin_< 0.8 && tauJetDeltaRmin_>=0.7 && isBarrel_ ){
    return TauFakeRateCalculator( tauFRtt_dR1B,tauFRtt_dR1B_Err, tauFRw_dR1B,tauFRw_dR1B_Err,
                                  stcorr_,stcorrErr_, tauFRdR1B_SF,tauFRdR1B_SFerr );
  }
  //
  if(                          tauJetDeltaRmin_>=1.0 && !isBarrel_ ){
    return TauFakeRateCalculator( tauFRtt_dR3E,tauFRtt_dR3E_Err, tauFRw_dR3E,tauFRw_dR3E_Err,
                                  stcorr_,stcorrErr_, tauFRdR3E_SF,tauFRdR3E_SFerr );
  }
  if( tauJetDeltaRmin_< 1.0 && tauJetDeltaRmin_>=0.8 && !isBarrel_ ){
    return TauFakeRateCalculator( tauFRtt_dR2E,tauFRtt_dR2E_Err, tauFRw_dR2E,tauFRw_dR2E_Err,
                                  stcorr_,stcorrErr_, tauFRdR2E_SF,tauFRdR2E_SFerr );
  }
  if( tauJetDeltaRmin_< 0.8 && tauJetDeltaRmin_>=0.7 && !isBarrel_ ){
    return TauFakeRateCalculator( tauFRtt_dR1E,tauFRtt_dR1E_Err, tauFRw_dR1E,tauFRw_dR1E_Err,
                                  stcorr_,stcorrErr_, tauFRdR1E_SF,tauFRdR1E_SFerr );
  }
  //
  return 0;
}
// // //
double analysisClass::tauFRE( unsigned int iTauR ){
  double tauJetDeltaRmin_=tauJetDeltaRmin(iTauR);
  bool   isBarrel_=false;
  if( fabs(HPSTauEta->at(iTauR))<1.5 ) isBarrel_=true;
  //
  double stcorr_    = tauFR_STcorrection(iTauR);
  double stcorrErr_ = tauFR_STcorrectionError(iTauR);
  //
  if(                          tauJetDeltaRmin_>=1.0 && isBarrel_ ){
    return TauFakeRateErrorCalculator( tauFRtt_dR3B,tauFRtt_dR3B_Err, tauFRw_dR3B,tauFRw_dR3B_Err,
				       stcorr_,stcorrErr_, tauFRdR3B_SF,tauFRdR3B_SFerr );
  }
  if( tauJetDeltaRmin_< 1.0 && tauJetDeltaRmin_>=0.8 && isBarrel_ ){
    return TauFakeRateErrorCalculator( tauFRtt_dR2B,tauFRtt_dR2B_Err, tauFRw_dR2B,tauFRw_dR2B_Err,
				       stcorr_,stcorrErr_, tauFRdR2B_SF,tauFRdR2B_SFerr );
  }
  if( tauJetDeltaRmin_< 0.8 && tauJetDeltaRmin_>=0.7 && isBarrel_ ){
    return TauFakeRateErrorCalculator( tauFRtt_dR1B,tauFRtt_dR1B_Err, tauFRw_dR1B,tauFRw_dR1B_Err,
				       stcorr_,stcorrErr_, tauFRdR1B_SF,tauFRdR1B_SFerr );
  }
  //
  if(                          tauJetDeltaRmin_>=1.0 && !isBarrel_ ){
    return TauFakeRateErrorCalculator( tauFRtt_dR3E,tauFRtt_dR3E_Err, tauFRw_dR3E,tauFRw_dR3E_Err,
				       stcorr_,stcorrErr_, tauFRdR3E_SF,tauFRdR3E_SFerr );
  }
  if( tauJetDeltaRmin_< 1.0 && tauJetDeltaRmin_>=0.8 && !isBarrel_ ){
    return TauFakeRateErrorCalculator( tauFRtt_dR2E,tauFRtt_dR2E_Err, tauFRw_dR2E,tauFRw_dR2E_Err,
				       stcorr_,stcorrErr_, tauFRdR2E_SF,tauFRdR2E_SFerr );
  }
  if( tauJetDeltaRmin_< 0.8 && tauJetDeltaRmin_>=0.7 && !isBarrel_ ){
    return TauFakeRateErrorCalculator( tauFRtt_dR1E,tauFRtt_dR1E_Err, tauFRw_dR1E,tauFRw_dR1E_Err,
				       stcorr_,stcorrErr_, tauFRdR1E_SF,tauFRdR1E_SFerr );
  }
  //
  return 0;
}
// // //
double analysisClass::tauPR( unsigned int iTauR ){
  double tauJetDeltaRmin_=tauJetDeltaRmin(iTauR);
  bool   isBarrel_=false;
  if( fabs(HPSTauEta->at(iTauR))<1.5 ) isBarrel_=true;
  double TauPt=tauPtcorr(iTauR);
  //
  double stcorr_    = tauPR_STcorrection(iTauR);
  double stcorrErr_ = tauPR_STcorrectionError(iTauR);
  //
  double tauPR_=0;
  if(                          tauJetDeltaRmin_>=1.0 && isBarrel_ ){
    tauPR_  = pol1UP(   0.672,0.001,  0,0,             iTauR, stcorr_,stcorrErr_ );//wjets
    tauPR_ += pol1DOWN( 0.618,0.001, -0.00018,0.00003, iTauR, stcorr_,stcorrErr_ );//ttjets
    tauPR_  = (tauPR_*0.5)*tauPRdR3B_SF;
  }
  if( tauJetDeltaRmin_< 1.0 && tauJetDeltaRmin_>=0.8 && isBarrel_ ){
    tauPR_  = pol1UP(   0.598,0.003,  0,0,             iTauR, stcorr_,stcorrErr_ );//wjets
    tauPR_ += pol1DOWN( 0.539,0.002, -0.00018,0.00004, iTauR, stcorr_,stcorrErr_ );//ttjets
    tauPR_  = (tauPR_*0.5)*tauPRdR2B_SF;
  }
  if( tauJetDeltaRmin_< 0.8 && tauJetDeltaRmin_>=0.7 && isBarrel_ ){
    tauPR_  = pol1UP(   0.525,0.005,  0,0,             iTauR, stcorr_,stcorrErr_ );//wjets
    tauPR_ += pol1DOWN( 0.463,0.002, -0.00032,0.00005, iTauR, stcorr_,stcorrErr_ );//ttjets
    tauPR_  = (tauPR_*0.5)*tauPRdR1B_SF;
  }
  //
  if(                          tauJetDeltaRmin_>=1.0 && !isBarrel_ ){
    tauPR_  = pol1UP(   0.692,0.002, 0.00018,0.00005, iTauR, stcorr_,stcorrErr_ ); //wjets
    tauPR_ += pol1DOWN( 0.650,0.002, 0,0,             iTauR, stcorr_,stcorrErr_ );//ttjets
    tauPR_  = (tauPR_*0.5)*tauPRdR3E_SF;
  }
  if( tauJetDeltaRmin_< 1.0 && tauJetDeltaRmin_>=0.8 && !isBarrel_ ){
    tauPR_  = pol1UP(   0.634,0.007, 0,0, iTauR, stcorr_,stcorrErr_ );//wjets
    tauPR_ += pol1DOWN( 0.587,0.005, 0,0, iTauR, stcorr_,stcorrErr_ );//ttjets
    tauPR_  = (tauPR_*0.5)*tauPRdR2E_SF;
  }
  if( tauJetDeltaRmin_< 0.8 && tauJetDeltaRmin_>=0.7 && !isBarrel_ ){
    tauPR_  = pol1UP(   0.559,0.012, 0,0, iTauR, stcorr_,stcorrErr_ );//wjets - No data rate, SF is used!
    tauPR_ += pol1DOWN( 0.507,0.009, 0,0, iTauR, stcorr_,stcorrErr_ );//ttjets
    tauPR_  = (tauPR_*0.5)*tauPRdR1E_SF;
  }
  //
  return tauPR_;
}
// // //
double analysisClass::pol1UP( double c1, double Sc1, double m1, double Sm1, unsigned int iTauR, 
			      double stcorr, double stcorrErr ){
  double TauPt=tauPtcorr(iTauR);
  //
  double mean      = c1 + m1*TauPt + stcorr;
  double meanError =  TMath::Power( (TMath::Power(Sc1,2) + TMath::Power(Sm1*TauPt,2) + TMath::Power(stcorrErr,2)), 0.5 );
  //
  return (mean+meanError);
}
// // //
double analysisClass::pol1DOWN(double c1, double Sc1, double m1, double Sm1, unsigned int iTauR,
			       double stcorr, double stcorrErr){
  double TauPt=tauPtcorr(iTauR);
  //
  double mean      = c1 + m1*TauPt + stcorr;
  double meanError =  TMath::Power( (TMath::Power(Sc1,2) + TMath::Power(Sm1*TauPt,2) + TMath::Power(stcorrErr,2)), 0.5 );
  //
  return (mean-meanError);
}
// // //
/*
double analysisClass::pol0w_pol1tt_Err( double c1, double Sc1, double c2, double Sc2, double m2, double Sm2, double sf, double Ssf, unsigned int iTauR){
  double Slambda=0.5;
  double lambda=0.5;
  double TauPt=tauPtcorr(iTauR);
  double c1e2=TMath::Power( Sc1*(1-lambda), 2);
  double c2e2=TMath::Power( Sc2*(lambda), 2);
  double m2e2=TMath::Power( Sm2*(lambda)*TauPt, 2);
  double lambdae2=TMath::Power( Slambda*(-c1+c2+m2*tauPtcorr(iTauR)), 2);
  double sfe2=TMath::Power( (c1*(1-lambda)+(c2+m2*TauPt)*lambda)*Ssf, 2);
  return TMath::Power( c1e2+c2e2+m2e2+lambdae2+sfe2, 0.5 );
}
*/
// // //
/*
double analysisClass::pol0tt_pol1w_Err( double c1, double Sc1, double c2, double Sc2, double m2, double Sm2, double sf, double Ssf, unsigned int iTauR){
  double Slambda=0.5;
  double lambda=0.5;
  double TauPt=tauPtcorr(iTauR);
  double c1e2=TMath::Power( Sc1*(lambda), 2);
  double c2e2=TMath::Power( Sc2*(1-lambda), 2);
  double m2e2=TMath::Power( Sm2*(1-lambda)*TauPt, 2);
  double lambdae2=TMath::Power( Slambda*(c1-c2-m2*tauPtcorr(iTauR)), 2);
  double sfe2=TMath::Power( (c1*lambda+(c2+m2*TauPt)*(1-lambda))*Ssf, 2);
  return TMath::Power( c1e2+c2e2+m2e2+lambdae2+sfe2, 0.5 );
}
*/
// // //
double analysisClass::tauPRE( unsigned int iTauR ){
  double tauJetDeltaRmin_=tauJetDeltaRmin(iTauR);
  bool   isBarrel_=false;
  if( fabs(HPSTauEta->at(iTauR))<1.5 ) isBarrel_=true;
  //
  double stcorr_    =tauPR_STcorrection(iTauR);
  double stcorrErr_ =tauPR_STcorrectionError(iTauR);
  double tauPR_=tauPR(iTauR);//if isData, this already has the SF included!
  double DeltatauPR_=0;
  double tauPRE_=0;
  //
  if(                          tauJetDeltaRmin_>=1.0 && isBarrel_ ){
    DeltatauPR_ = fabs( pol1UP( 0.672,0.001, 0,0, iTauR, stcorr_,stcorrErr_ ) - tauPR_/tauPRdR3B_SF );//wjets minus the mean (undo SF)
    tauPRE_ =  QuadSum( DeltatauPR_/(tauPR_/tauPRdR3B_SF), tauPRdR3B_SFerr/tauPRdR3B_SF )*tauPR_; 
  }
  if( tauJetDeltaRmin_< 1.0 && tauJetDeltaRmin_>=0.8 && isBarrel_ ){
    DeltatauPR_ = fabs( pol1UP( 0.598,0.003, 0,0, iTauR, stcorr_,stcorrErr_ ) - tauPR_/tauPRdR2B_SF );//wjets minus the mean (undo SF)
    tauPRE_ =  QuadSum( DeltatauPR_/(tauPR_/tauPRdR2B_SF), tauPRdR2B_SFerr/tauPRdR2B_SF )*tauPR_; 
  }
  if( tauJetDeltaRmin_< 0.8 && tauJetDeltaRmin_>=0.7 && isBarrel_ ){
    DeltatauPR_ = fabs( pol1UP( 0.525,0.005, 0,0, iTauR, stcorr_,stcorrErr_ ) - tauPR_/tauPRdR1B_SF );//wjets minus the mean (undo SF)
    tauPRE_ =  QuadSum( DeltatauPR_/(tauPR_/tauPRdR1B_SF), tauPRdR1B_SFerr/tauPRdR1B_SF )*tauPR_;
  }
  //
  if(                          tauJetDeltaRmin_>=1.0 && !isBarrel_ ){
    DeltatauPR_ = fabs( pol1UP( 0.692,0.002, 0.00018,0.00005, iTauR, stcorr_,stcorrErr_ ) - tauPR_/tauPRdR3E_SF );//wjets minus the mean (undo SF)
    tauPRE_ =  QuadSum( DeltatauPR_/(tauPR_/tauPRdR3E_SF), tauPRdR3E_SFerr/tauPRdR3E_SF )*tauPR_;
  }
  if( tauJetDeltaRmin_< 1.0 && tauJetDeltaRmin_>=0.8 && !isBarrel_ ){
    DeltatauPR_ = fabs( pol1UP( 0.634,0.007, 0,0, iTauR, stcorr_,stcorrErr_ ) - tauPR_/tauPRdR2E_SF );//wjets minus the mean (undo SF)
    tauPRE_ =  QuadSum( DeltatauPR_/(tauPR_/tauPRdR2E_SF), tauPRdR2E_SFerr/tauPRdR2E_SF )*tauPR_;
  }
  if( tauJetDeltaRmin_< 0.8 && tauJetDeltaRmin_>=0.7 && !isBarrel_ ){
    DeltatauPR_ = fabs( pol1UP( 0.559,0.012, 0,0, iTauR, stcorr_,stcorrErr_ ) - tauPR_/tauPRdR1E_SF );//wjets minus the mean (undo SF)
    tauPRE_ =  QuadSum( DeltatauPR_/(tauPR_/tauPRdR1E_SF), tauPRdR1E_SFerr/tauPRdR1E_SF )*tauPR_;
  }
  //
  return tauPRE_;
}
double analysisClass::muFR( unsigned int iMuR ){
  double st_ = ST();
  if( fabs(MuonEta->at(iMuR))< 1.5 && st_< 400 ) return 0.010;//barrel
  if( fabs(MuonEta->at(iMuR))< 1.5 && st_>=400 ) return 0.005;
  //
  if( fabs(MuonEta->at(iMuR))>=1.5 && st_< 400 ) return 0.030;//endcap
  if( fabs(MuonEta->at(iMuR))>=1.5 && st_>=400 ) return 0.010;
}
// // //
double analysisClass::muFRE( unsigned int iMuR ){
  return muFR( iMuR ); // >> 100% error
}
// // //
double analysisClass::PromptRateFunction( double param0, double param1, double param2, double pt ){
  //[0]-TMath::Exp(-x**(0.8)*[1]-x*x*[2])
  return (param0-TMath::Exp(-TMath::Power(pt,0.8)*param1-pt*pt*param2));
}
// // //
double analysisClass::PromptRateFunctionError( double A0, double A1, double A2, double A0err, double A1err, double A2err, double pt ){
  double Err2=( (A0err*A0err) + 
		TMath::Power( TMath::Exp(-TMath::Power(pt,0.8)*A1-pt*pt*A2)*TMath::Power(pt,0.8)*A1err, 2 ) + 
	        TMath::Power( TMath::Exp(-TMath::Power(pt,0.8)*A1-pt*pt*A2)*pt*pt*A2err, 2 )  
		);
  return TMath::Power( Err2, 0.5 );
}
// // //
double analysisClass::AvePromptRate( double A0, double A1, double A2, double pt, double B0, double B1, double B2 ){
  // Average of two: [0]-TMath::Exp(-x**(0.8)*[1]-x*x*[2]) 
  return 0.5*(PromptRateFunction( A0, A1, A2, pt )+PromptRateFunction( B0, B1, B2, pt ));
}
// // //
double analysisClass::ErrPromptRate( double A0, double A1, double A2, double pt, double B0, double B1, double B2 ){
  // Wjets - (Average of two: [0]-TMath::Exp(-x**(0.8)*[1]-x*x*[2]) 
  return fabs((PromptRateFunction( A0, A1, A2, pt )-AvePromptRate( A0, A1, A2, pt, B0, B1, B2 )));
}
// // //
double analysisClass::CorrFactor( double A0, double A1, double A2, double pt, double B0, double B1, double B2 ){
  // DATA/Wjets
  return (PromptRateFunction( A0, A1, A2, pt )/PromptRateFunction( B0, B1, B2, pt ));
}
// // //
double analysisClass::CorrFactorError( double A0, double A1, double A2, double A0err, double A1err, double A2err, 
				       double pt, 
				       double B0, double B1, double B2, double B0err, double B1err, double B2err ){
  // DATA/Wjets
  double dataRate = PromptRateFunction( A0, A1, A2, pt );
  double dataErr  = PromptRateFunctionError( A0, A1, A2, A0err, A1err, A2err, pt );
  double wRate    = PromptRateFunction( B0, B1, B2, pt );
  double wErr     = PromptRateFunctionError( B0, B1, B2, B0err, B1err, B2err, pt );
  return QuadSum( dataErr/dataRate, wErr/wRate )*(dataRate/wRate);
}
// // //
void analysisClass::InitMuonFunctionParameters(){
  //Muon
  // ------------------------------------------------------------------------------------------------------------------------
  //dR>1.0 -- BARREL
  // 0.992042009148 +/- 0.000216869841705    0.141016360602 +/- 0.000529319174907    7.36696505353e-05 +/- 7.6174002264e-06
  Wdr3Bpar1    = 0.992042009148;     Wdr3Bpar2    = 0.141016360602;     Wdr3Bpar3    = 7.36696505353e-05;
  Wdr3BparErr1 = 0.000216869841705;  Wdr3BparErr2 = 0.000529319174907;  Wdr3BparErr3 = 7.6174002264e-06;
  // 0.952563258809 +/- 0.000844144014814    0.140048905136 +/- 0.00148435931245   0
  LQdr3Bpar1    = 0.952563258809;     LQdr3Bpar2    = 0.140048905136;    LQdr3Bpar3    = 0;
  LQdr3BparErr1 = 0.000844144014814;  LQdr3BparErr2 = 0.00148435931245;  LQdr3BparErr3 = 0;
  // 0.993349781988 +/- 0.000808674458852  0.144327297694 +/- 0.00144433203035  3.99562296097e-05 +/- 2.32997317735e-05
  DATAdr3Bpar1    = 0.993349781988;     DATAdr3Bpar2    = 0.144327297694;    DATAdr3Bpar3    = 3.99562296097e-05;
  DATAdr3BparErr1 = 0.000808674458852;  DATAdr3BparErr2 = 0.00144433203035;  DATAdr3BparErr3 = 2.32997317735e-05;
  //
  //1.0>dR>0.7 -- BARREL
  // 0.986580507233 +/- 0.000885817076123    0.121796210609 +/- 0.00144946431957    0.000155933842421 +/- 2.281313639e-05
  Wdr2Bpar1    = 0.986580507233;     Wdr2Bpar2    = 0.121796210609;    Wdr2Bpar3    = 0.000155933842421;
  Wdr2BparErr1 = 0.000885817076123;  Wdr2BparErr2 = 0.00144946431957;  Wdr2BparErr3 = 2.281313639e-05;
  // 0.94305644114 +/- 0.00139583354793    0.120113380094 +/- 0.00342875695156    5.93387769282e-05 +/- 4.00721031189e-05
  LQdr2Bpar1    = 0.94305644114;     LQdr2Bpar2    = 0.120113380094;    LQdr2Bpar3    = 5.93387769282e-05;
  LQdr2BparErr1 = 0.00139583354793;  LQdr2BparErr2 = 0.00342875695156;  LQdr2BparErr3 = 4.00721031189e-05;
  // 0.986924142308 +/- 0.00218858925587   0.125596084483 +/- 0.00353641532975   0.000115073980455 +/- 0.556701886971e-04
  DATAdr2Bpar1    = 0.986924142308;    DATAdr2Bpar2    = 0.125596084483;    DATAdr2Bpar3    = 0.000115073980455;
  DATAdr2BparErr1 = 0.00218858925587;  DATAdr2BparErr2 = 0.00353641532975;  DATAdr2BparErr3 = 0.556701886971e-04;
  //
  //0.7>dR>0.5 -- BARREL
  // 0.969974094692 +/- 0.00247033249185    0.0786157656957 +/- 0.00212025894379    0.000300500921649 +/- 3.47113927135e-05
  Wdr1Bpar1    = 0.969974094692;    Wdr1Bpar2    = 0.0786157656957;   Wdr1Bpar3    = 0.000300500921649;
  Wdr1BparErr1 = 0.00247033249185;  Wdr1BparErr2 = 0.00212025894379;  Wdr1BparErr3 = 3.47113927135e-05;
  // 0.922695470797 +/- 0.00263157564797    0.0765608196989 +/- 0.00116272006956    0
  LQdr1Bpar1    = 0.922695470797;    LQdr1Bpar2    = 0.0765608196989;   LQdr1Bpar3    = 0;
  LQdr1BparErr1 = 0.00263157564797;  LQdr1BparErr2 = 0.00116272006956;  LQdr1BparErr3 = 0;
  // 0.991467644403 +/- 0.00507297028039  0.0859972645421 +/- 0.0022818941508  4.6824333923e-05 +/- 8.48876460385e-06
  DATAdr1Bpar1    = 0.991467644403;    DATAdr1Bpar2    = 0.0859972645421;  DATAdr1Bpar3    = 4.6824333923e-05;
  DATAdr1BparErr1 = 0.00507297028039;  DATAdr1BparErr2 = 0.0022818941508;  DATAdr1BparErr3 = 8.48876460385e-06;
  // 
  //dR>1.0 -- ENDCAP
  // 0.993166780388 +/- 0.00022698067734    0.165025610265 +/- 0.000464273595962  0
  Wdr3Epar1    = 0.993166780388;    Wdr3Epar2    = 0.165025610265;     Wdr3Epar3    = 0;
  Wdr3EparErr1 = 0.00022698067734;  Wdr3EparErr2 = 0.000464273595962;  Wdr3EparErr3 = 0;
  // 0.973439893819 +/- 0.00211194412279    0.171398183843 +/- 0.00503405965635  0
  LQdr3Epar1    = 0.973439893819;    LQdr3Epar2    = 0.171398183843;    LQdr3Epar3    = 0;
  LQdr3EparErr1 = 0.00211194412279;  LQdr3EparErr2 = 0.00503405965635;  LQdr3EparErr3 = 0;
  // 0.995047966458 +/- 0.000695377981881   0.179023277964 +/- 0.00170713966175   0
  DATAdr3Epar1    = 0.995047966458;     DATAdr3Epar2    = 0.179023277964;    DATAdr3Epar3    = 0;
  DATAdr3EparErr1 = 0.000695377981881;  DATAdr3EparErr2 = 0.00170713966175;  DATAdr3EparErr3 = 0;
  //
  //1.0>dR>0.7 -- ENDCAP
  // 0.992030908401 +/- 0.00105045616213    0.146989460444 +/- 0.00125053225107 0
  Wdr2Epar1    = 0.992030908401;    Wdr2Epar2    = 0.146989460444;    Wdr2Epar3    = 0;
  Wdr2EparErr1 = 0.00105045616213;  Wdr2EparErr2 = 0.00125053225107;  Wdr2EparErr3 = 0;
  // 0.956574713723 +/- 0.00449105972872    0.161317907934 +/- 0.00844274840356 0 
  LQdr2Epar1    = 0.956574713723;    LQdr2Epar2    = 0.161317907934;    LQdr2Epar3    = 0;
  LQdr2EparErr1 = 0.00449105972872;  LQdr2EparErr2 = 0.00844274840356;  LQdr2EparErr3 = 0;
  // 0.983588670603 +/- 0.00335180041675   0.172603611092 +/- 0.00565677230066
  DATAdr2Epar1    = 0.983588670603;    DATAdr2Epar2    = 0.172603611092;    DATAdr2Epar3    = 0;
  DATAdr2EparErr1 = 0.00335180041675;  DATAdr2EparErr2 = 0.00565677230066;  DATAdr2EparErr3 = 0;
  //
  //0.7>dR>0.5 -- ENDCAP
  // 0.994577247675 +/- 0.00325981417142    0.107184868828 +/- 0.00169227099275 0
  Wdr1Epar1    = 0.994577247675;    Wdr1Epar2    = 0.107184868828;    Wdr1Epar3    = 0;
  Wdr1EparErr1 = 0.00325981417142;  Wdr1EparErr2 = 0.00169227099275;  Wdr1EparErr3 = 0;
  // 0.935837152119 +/- 0.0113840436514    0.103903740798 +/- 0.00705436225208
  LQdr1Epar1    = 0.935837152119;   LQdr1Epar2    = 0.103903740798;    LQdr1Epar3    = 0;
  LQdr1EparErr1 = 0.0113840436514;  LQdr1EparErr2 = 0.00705436225208;  LQdr1EparErr3 = 0;
  // 0.986871038545 +/- 0.00711163099758   0.116099921349 +/- 0.00517940797901  0
  DATAdr1Epar1    = 0.986871038545;    DATAdr1Epar2    = 0.116099921349;    DATAdr1Epar3    = 0;
  DATAdr1EparErr1 = 0.00711163099758;  DATAdr1EparErr2 = 0.00517940797901;  DATAdr1EparErr3 = 0;
  // ------------------------------------------------------------------------------------------------------------------------
}
// // //
double analysisClass::muPR( unsigned int iMuR ){
  double muJetDeltaRmin_=muJetDeltaRmin(iMuR);
  double muPt=muPtcorr(iMuR);
  bool   isBarrel_=false;
  if( fabs(MuonEta->at(iMuR))<1.5 ) isBarrel_=true;
  double aveRate=0;
  //
  //BARREL
  if( muJetDeltaRmin_>=1.00 && isBarrel_ ){   // (W rate + LQ3 rate)*1/2
    //return AvePromptRate( 0.992363589082, 0.141057516954, 7.01212733182e-05, muPt, 0.952919953136, 0.139894951743, 5.91193760613e-14 );
    aveRate=AvePromptRate( Wdr3Bpar1, Wdr3Bpar2, Wdr3Bpar3, muPt, LQdr3Bpar1, LQdr3Bpar2, LQdr3Bpar3 );
    return aveRate*muPRdR3B_SF;
  }
  if( muJetDeltaRmin_<1.00 && muJetDeltaRmin_>=0.70 && isBarrel_ ){
    //return AvePromptRate( 0.986502396183, 0.12147330776, 0.000163736563901, muPt, 0.94312847352, 0.120065370783, 6.01804097773e-05 );
    aveRate=AvePromptRate( Wdr2Bpar1, Wdr2Bpar2, Wdr2Bpar3, muPt, LQdr2Bpar1, LQdr2Bpar2, LQdr2Bpar3 );
    return aveRate*muPRdR2B_SF;
  }
  if(  muJetDeltaRmin_<0.70 && muJetDeltaRmin_>=0.50 && isBarrel_ ){
    //return AvePromptRate( 0.96938878404, 0.0785795765961, 0.000304128907785, muPt, 0.922695470797, 0.0765608196989, 5.2569060216e-14 );
    aveRate=AvePromptRate( Wdr1Bpar1, Wdr1Bpar2, Wdr1Bpar3, muPt, LQdr1Bpar1, LQdr1Bpar2, LQdr1Bpar3 );
    return aveRate*muPRdR1B_SF;
  }
  //
  // ENDCAP
  if( muJetDeltaRmin_>=1.00 && !isBarrel_ ){
    //return AvePromptRate( 0.993514263739, 0.164810790281, 1.37112543541e-14, muPt, 0.972293945493, 0.171564717602, 1.69586567011e-13 );
    aveRate=AvePromptRate( Wdr3Epar1, Wdr3Epar2, Wdr3Epar3, muPt, LQdr3Epar1, LQdr3Epar2, LQdr3Epar3 );
    return aveRate*muPRdR3E_SF;
  }
  if( muJetDeltaRmin_<1.00 && muJetDeltaRmin_>=0.70 && !isBarrel_ ){
    //return AvePromptRate( 0.98823767823, 0.139072254739, 0.000149671874559, muPt, 0.953355043527, 0.153325169726, 0.000134950408949 );
    aveRate=AvePromptRate( Wdr2Epar1, Wdr2Epar2, Wdr2Epar3, muPt, LQdr2Epar1, LQdr2Epar2, LQdr2Epar3 );
    return aveRate*muPRdR2E_SF;
  }
  if( muJetDeltaRmin_<0.70 && muJetDeltaRmin_>=0.50 && !isBarrel_ ){
    //return AvePromptRate( 0.97518267147, 0.0883441346788, 0.000362354384714, muPt, 0.931919993275, 0.103973960404, 1.21391785513e-11 );
    aveRate=AvePromptRate( Wdr1Epar1, Wdr1Epar2, Wdr1Epar3, muPt, LQdr1Epar1, LQdr1Epar2, LQdr1Epar3 );
    return aveRate*muPRdR1E_SF;
  }
  //
  return 0;
}
// // //
double analysisClass::muPRE_STcorrection( double eta, double dr, double pt, double st ){
  //
  return 3;
  //
  unsigned int drbin=0;
  if( dr>0.5 && dr<0.7 ) drbin=1;
  if( dr>0.7 && dr<1.0 ) drbin=2;
  if( dr>1.0           ) drbin=3;
  //
  // barrel
  if( fabs(eta)<1.5 ){
    if( st>100 && st<400 ){
      if( pt>25 && pt<30  && drbin==1 ) return 1.1;
      if( pt>25 && pt<30  && drbin==2 ) return 1.0;
      if( pt>25 && pt<30  && drbin==3 ) return 1.0;
      //
      if( pt>30 && pt<50  && drbin==1 ) return 1.3;
      if( pt>30 && pt<50  && drbin==2 ) return 1.1;
      if( pt>30 && pt<50  && drbin==3 ) return 1.0;
      //
      if( pt>50 && pt<100 && drbin==1 ) return 1.1;
      if( pt>50 && pt<100 && drbin==2 ) return 1.1;
      if( pt>50 && pt<100 && drbin==3 ) return 1.0;
      //
      if( pt>100          && drbin==1 ) return 1.5;
      if( pt>100          && drbin==2 ) return 1.2;
      if( pt>100          && drbin==3 ) return 1.1;
    }
    if( st>400 && st<800 ){
      if( pt>25 && pt<30  && drbin==1 ) return 1.4;
      if( pt>25 && pt<30  && drbin==2 ) return 1.0;
      if( pt>25 && pt<30  && drbin==3 ) return 1.5;
      //
      if( pt>30 && pt<50  && drbin==1 ) return 1.0;
      if( pt>30 && pt<50  && drbin==2 ) return 1.0;
      if( pt>30 && pt<50  && drbin==3 ) return 1.0;
      //
      if( pt>50 && pt<100 && drbin==1 ) return 1.0;
      if( pt>50 && pt<100 && drbin==2 ) return 1.0;
      if( pt>50 && pt<100 && drbin==3 ) return 1.0;
      //
      if( pt>100          && drbin==1 ) return 1.1;
      if( pt>100          && drbin==2 ) return 1.0;
      if( pt>100          && drbin==3 ) return 1.0;
    }
    if( st>800           ){
      if( pt>25 && pt<30  && drbin==1 ) return 2.0;
      if( pt>25 && pt<30  && drbin==2 ) return 1.0;
      if( pt>25 && pt<30  && drbin==3 ) return 2.3;
      //
      if( pt>30 && pt<50  && drbin==1 ) return 1.7;
      if( pt>30 && pt<50  && drbin==2 ) return 1.2;
      if( pt>30 && pt<50  && drbin==3 ) return 1.1;
      //
      if( pt>50 && pt<100 && drbin==1 ) return 1.2;
      if( pt>50 && pt<100 && drbin==2 ) return 1.0;
      if( pt>50 && pt<100 && drbin==3 ) return 1.0;
      //
      if( pt>100          && drbin==1 ) return 1.4;
      if( pt>100          && drbin==2 ) return 1.0;
      if( pt>100          && drbin==3 ) return 1.0;
    }
  }
  //
  // endcap
  if( fabs(eta)>=1.5 ){
    if( st>100 && st<400 ){
      if( pt>25 && pt<30  && drbin==1 ) return 1.0;
      if( pt>25 && pt<30  && drbin==2 ) return 3.7;
      if( pt>25 && pt<30  && drbin==3 ) return 1.4;
      //
      if( pt>30 && pt<50  && drbin==1 ) return 1.6;
      if( pt>30 && pt<50  && drbin==2 ) return 1.8;
      if( pt>30 && pt<50  && drbin==3 ) return 1.5;
      //
      if( pt>50 && pt<100 && drbin==1 ) return 1.4;
      if( pt>50 && pt<100 && drbin==2 ) return 1.2;
      if( pt>50 && pt<100 && drbin==3 ) return 1.1;
      //
      if( pt>100          && drbin==1 ) return 1.0;
      if( pt>100          && drbin==2 ) return 1.0;
      if( pt>100          && drbin==3 ) return 1.1;
    }
    if( st>400 && st<800 ){
      if( pt>25 && pt<30  && drbin==1 ) return 2.1;
      if( pt>25 && pt<30  && drbin==2 ) return 6.4;
      if( pt>25 && pt<30  && drbin==3 ) return 3.1;
      //
      if( pt>30 && pt<50  && drbin==1 ) return 3.0;
      if( pt>30 && pt<50  && drbin==2 ) return 2.8;
      if( pt>30 && pt<50  && drbin==3 ) return 2.1;
      //
      if( pt>50 && pt<100 && drbin==1 ) return 1.0;
      if( pt>50 && pt<100 && drbin==2 ) return 1.0;
      if( pt>50 && pt<100 && drbin==3 ) return 1.1;
      //
      if( pt>100          && drbin==1 ) return 1.0;
      if( pt>100          && drbin==2 ) return 1.0;
      if( pt>100          && drbin==3 ) return 1.0;
    }
    if( st>800           ){
      if( pt>25 && pt<30  && drbin==1 ) return 1.0;
      if( pt>25 && pt<30  && drbin==2 ) return 1.0;
      if( pt>25 && pt<30  && drbin==3 ) return 1.0;
      //
      if( pt>30 && pt<50  && drbin==1 ) return 3.2;
      if( pt>30 && pt<50  && drbin==2 ) return 4.9;
      if( pt>30 && pt<50  && drbin==3 ) return 4.1;
      //
      if( pt>50 && pt<100 && drbin==1 ) return 1.2;
      if( pt>50 && pt<100 && drbin==2 ) return 1.2;
      if( pt>50 && pt<100 && drbin==3 ) return 1.0;
      //
      if( pt>100          && drbin==1 ) return 1.0;
      if( pt>100          && drbin==2 ) return 1.1;
      if( pt>100          && drbin==3 ) return 1.4;
    }
  }
  //
}
// // //
double analysisClass::muPRE( unsigned int iMuR ){
  double muJetDeltaRmin_=muJetDeltaRmin(iMuR);
  double muPt=muPtcorr(iMuR);
  bool   isBarrel_=false;
  if( fabs(MuonEta->at(iMuR))<1.5 ) isBarrel_=true;
  double RateErr=0;
  double aveRate=0;
  double ST_=ST();
  //
  //BARREL
  if( muJetDeltaRmin_>=1.00 && isBarrel_ ){   // W rate - 1/2*(W rate + LQ3 rate), oplus Error on Correction Factor
    aveRate=AvePromptRate( Wdr3Bpar1, Wdr3Bpar2, Wdr3Bpar3, muPt, LQdr3Bpar1, LQdr3Bpar2, LQdr3Bpar3 );
    RateErr=ErrPromptRate( Wdr3Bpar1, Wdr3Bpar2, Wdr3Bpar3, muPt, LQdr3Bpar1, LQdr3Bpar2, LQdr3Bpar3 );
    cout<<"Before muPRE STcorr: "<<RateErr<<endl;
    RateErr=RateErr*muPRE_STcorrection( MuonEta->at(iMuR), muJetDeltaRmin_, muPt, ST_ );   
    cout<<"After muPRE STcorr: "<<RateErr<<"  "<<muPRE_STcorrection( MuonEta->at(iMuR), muJetDeltaRmin_, muPt, ST_ )<<endl;
    return QuadSum(RateErr/aveRate,muPRdR3B_SFerr/muPRdR3B_SF)*aveRate*muPRdR3B_SF;
  }
  if( muJetDeltaRmin_<1.00 && muJetDeltaRmin_>=0.70 && isBarrel_ ){
    aveRate=AvePromptRate( Wdr2Bpar1, Wdr2Bpar2, Wdr2Bpar3, muPt, LQdr2Bpar1, LQdr2Bpar2, LQdr2Bpar3 );
    RateErr=ErrPromptRate( Wdr2Bpar1, Wdr2Bpar2, Wdr2Bpar3, muPt, LQdr2Bpar1, LQdr2Bpar2, LQdr2Bpar3 );
    cout<<"Before muPRE STcorr: "<<RateErr<<endl;
    RateErr=RateErr*muPRE_STcorrection( MuonEta->at(iMuR), muJetDeltaRmin_, muPt, ST_ );
    cout<<"After muPRE STcorr: "<<RateErr<<"  "<<muPRE_STcorrection( MuonEta->at(iMuR), muJetDeltaRmin_, muPt, ST_ )<<endl;
    return QuadSum(RateErr/aveRate,muPRdR2B_SFerr/muPRdR2B_SF)*aveRate*muPRdR2B_SF;
  }
  if( muJetDeltaRmin_<0.70 && muJetDeltaRmin_>=0.50 && isBarrel_ ){
    aveRate=AvePromptRate( Wdr1Bpar1, Wdr1Bpar2, Wdr1Bpar3, muPt, LQdr1Bpar1, LQdr1Bpar2, LQdr1Bpar3 );
    RateErr=ErrPromptRate( Wdr1Bpar1, Wdr1Bpar2, Wdr1Bpar3, muPt, LQdr1Bpar1, LQdr1Bpar2, LQdr1Bpar3 );
    cout<<"Before muPRE STcorr: "<<RateErr<<endl;
    RateErr=RateErr*muPRE_STcorrection( MuonEta->at(iMuR), muJetDeltaRmin_, muPt, ST_ );
    cout<<"After muPRE STcorr: "<<RateErr<<"  "<<muPRE_STcorrection( MuonEta->at(iMuR), muJetDeltaRmin_, muPt, ST_ )<<endl;
    return QuadSum(RateErr/aveRate,muPRdR1B_SFerr/muPRdR1B_SF)*aveRate*muPRdR1B_SF;
  }
  //
  // ENDCAP
  if( muJetDeltaRmin_>=1.00 && !isBarrel_ ){
    aveRate=AvePromptRate( Wdr3Epar1, Wdr3Epar2, Wdr3Epar3, muPt, LQdr3Epar1, LQdr3Epar2, LQdr3Epar3 );
    RateErr=ErrPromptRate( Wdr3Epar1, Wdr3Epar2, Wdr3Epar3, muPt, LQdr3Epar1, LQdr3Epar2, LQdr3Epar3 );
    cout<<"Before muPRE STcorr: "<<RateErr<<endl;
    RateErr=RateErr*muPRE_STcorrection( MuonEta->at(iMuR), muJetDeltaRmin_, muPt, ST_ );
    cout<<"After muPRE STcorr: "<<RateErr<<"  "<<muPRE_STcorrection( MuonEta->at(iMuR), muJetDeltaRmin_, muPt, ST_ )<<endl;
    return QuadSum(RateErr/aveRate,muPRdR3E_SFerr/muPRdR3E_SF)*aveRate*muPRdR3E_SF;
  }
  if( muJetDeltaRmin_<1.00 && muJetDeltaRmin_>=0.70 && !isBarrel_ ){
    aveRate=AvePromptRate( Wdr2Epar1, Wdr2Epar2, Wdr2Epar3, muPt, LQdr2Epar1, LQdr2Epar2, LQdr2Epar3 );
    RateErr=ErrPromptRate( Wdr2Epar1, Wdr2Epar2, Wdr2Epar3, muPt, LQdr2Epar1, LQdr2Epar2, LQdr2Epar3 );
    cout<<"Before muPRE STcorr: "<<RateErr<<endl;
    RateErr=RateErr*muPRE_STcorrection( MuonEta->at(iMuR), muJetDeltaRmin_, muPt, ST_ );
    cout<<"After muPRE STcorr: "<<RateErr<<"  "<<muPRE_STcorrection( MuonEta->at(iMuR), muJetDeltaRmin_, muPt, ST_ )<<endl;
    return QuadSum(RateErr/aveRate,muPRdR2E_SFerr/muPRdR2E_SF)*aveRate*muPRdR2E_SF;
  }
  if( muJetDeltaRmin_<0.70 && muJetDeltaRmin_>=0.50 && !isBarrel_ ){
    aveRate=AvePromptRate( Wdr1Epar1, Wdr1Epar2, Wdr1Epar3, muPt, LQdr1Epar1, LQdr1Epar2, LQdr1Epar3 );
    RateErr=ErrPromptRate( Wdr1Epar1, Wdr1Epar2, Wdr1Epar3, muPt, LQdr1Epar1, LQdr1Epar2, LQdr1Epar3 );
    cout<<"Before muPRE STcorr: "<<RateErr<<endl;
    RateErr=RateErr*muPRE_STcorrection( MuonEta->at(iMuR), muJetDeltaRmin_, muPt, ST_ );
    cout<<"After muPRE STcorr: "<<RateErr<<"  "<<muPRE_STcorrection( MuonEta->at(iMuR), muJetDeltaRmin_, muPt, ST_ )<<endl;
    return QuadSum(RateErr/aveRate,muPRdR1E_SFerr/muPRdR1E_SF)*aveRate*muPRdR1E_SF;
  }
  //
  return 0;
}
// // //
double analysisClass::DoubleFakeWeight( unsigned int iMuR, unsigned int iTauR ){
  //
  bool isMuTight_  = false;
  bool isTauTight_ = false;
  isMuTight_  = muRTightCheck(iMuR);
  isTauTight_ = tauRTightCheck(iTauR);
  //
  double Jec = 1.0/((muPR_-muFR_)*(tauPR_-tauFR_));
  //
  if( !isMuTight_ && !isTauTight_ ){//LL
    return  Jec*muPR_*tauPR_;
  }
  if( !isMuTight_ &&  isTauTight_ ){//LT
    return -Jec*muPR_*(1-tauPR_);
  }
  if(  isMuTight_ && !isTauTight_ ){//TL
    return -Jec*(1-muPR_)*tauPR_;
  }
  if(  isMuTight_ &&  isTauTight_ ){//TT
    return  Jec*(1-muPR_)*(1-tauPR_);
  }
  //
}
// // //
double analysisClass::SingleFakeMuWeight( unsigned int iMuR, unsigned int iTauR ){
  //
  bool isMuTight_  = false;
  bool isTauTight_ = false;
  isMuTight_  = muRTightCheck(iMuR);
  isTauTight_ = tauRTightCheck(iTauR);
  //
  double Jec = 1.0/((muPR_-muFR_)*(tauPR_-tauFR_));
  //
  if( !isMuTight_ && !isTauTight_ ){//LL
    return -Jec*muPR_*tauFR_;
  }
  if( !isMuTight_ &&  isTauTight_ ){//LT
    return  Jec*muPR_*(1-tauFR_);
  }
  if(  isMuTight_ && !isTauTight_ ){//TL
    return  Jec*(1-muPR_)*tauFR_;
  }
  if(  isMuTight_ &&  isTauTight_ ){//TT
    return -Jec*(1-muPR_)*(1-tauFR_);
  }
  //
}
// // //
double analysisClass::SingleFakeTauWeight( unsigned int iMuR, unsigned int iTauR ){
  //
  bool isMuTight_  = false;
  bool isTauTight_ = false;
  isMuTight_  = muRTightCheck(iMuR);
  isTauTight_ = tauRTightCheck(iTauR);
  //
  double Jec = 1.0/((muPR_-muFR_)*(tauPR_-tauFR_));
  //
  if( !isMuTight_ && !isTauTight_ ){//LL
    return -Jec*muFR_*tauPR_;
  }
  if( !isMuTight_ &&  isTauTight_ ){//LT
    return  Jec*muFR_*(1-tauPR_);
  }
  if(  isMuTight_ && !isTauTight_ ){//TL
    return  Jec*(1-muFR_)*tauPR_;
  }
  if(  isMuTight_ &&  isTauTight_ ){//TT
    return -Jec*(1-muFR_)*(1-tauPR_);
  }
  //
}
// // //
double analysisClass::LTEM_ScaleFactor( unsigned int LTEMmu, bool isLTEMmuPrompt, unsigned int LTEMtau, bool isLTEMtauPrompt ){
  //
  double ScaleFactorMu_  = 0;
  double ScaleFactorTau_ = 0;
  double muJetDeltaRmin_  = muJetDeltaRmin(LTEMmu);
  double tauJetDeltaRmin_ = tauJetDeltaRmin(LTEMtau);
  bool   isTauBarrel_ = false;
  bool   isMuBarrel_  = false;
  if( fabs(HPSTauEta->at(LTEMtau))<1.5 ){ isTauBarrel_ = true; }
  if( fabs(MuonEta->at(LTEMmu))<1.5    ){ isMuBarrel_  = true; }
  //
  if( isLTEMmuPrompt ){//prompt
    if( isMuBarrel_ ){//barrel
      if( muJetDeltaRmin_>=1.0                        ) ScaleFactorMu_ = muPRdR3B_SF;
      if( muJetDeltaRmin_>=0.7 && muJetDeltaRmin_<1.0 ) ScaleFactorMu_ = muPRdR2B_SF;
      if( muJetDeltaRmin_>=0.5 && muJetDeltaRmin_<0.7 ) ScaleFactorMu_ = muPRdR1B_SF;
    }
    if( !isMuBarrel_ ){//endcap
      if( muJetDeltaRmin_>=1.0                        ) ScaleFactorMu_ = muPRdR3E_SF;
      if( muJetDeltaRmin_>=0.7 && muJetDeltaRmin_<1.0 ) ScaleFactorMu_ = muPRdR2E_SF;
      if( muJetDeltaRmin_>=0.5 && muJetDeltaRmin_<0.7 ) ScaleFactorMu_ = muPRdR1E_SF;
    }
  }
  if( !isLTEMmuPrompt ){//fake
    ScaleFactorMu_=1;
  }
  //
  if( isLTEMtauPrompt ){//prompt
    if( isTauBarrel_ ){//barrel
      if( tauJetDeltaRmin_>=1.0                         ) ScaleFactorTau_ = tauPRdR3B_SF;
      if( tauJetDeltaRmin_>=0.8 && tauJetDeltaRmin_<1.0 ) ScaleFactorTau_ = tauPRdR2B_SF;
      if( tauJetDeltaRmin_>=0.7 && tauJetDeltaRmin_<0.8 ) ScaleFactorTau_ = tauPRdR1B_SF;
    }
    if( !isTauBarrel_ ){//endcap
      if( tauJetDeltaRmin_>=1.0                         ) ScaleFactorTau_ = tauPRdR3E_SF;
      if( tauJetDeltaRmin_>=0.8 && tauJetDeltaRmin_<1.0 ) ScaleFactorTau_ = tauPRdR2E_SF;
      if( tauJetDeltaRmin_>=0.7 && tauJetDeltaRmin_<0.8 ) ScaleFactorTau_ = tauPRdR1E_SF;
    }
  }
  if( !isLTEMtauPrompt ){//fake
    if( isTauBarrel_ ){//barrel
      if( tauJetDeltaRmin_>=1.0                         ) ScaleFactorTau_ = tauFRdR3B_SF;
      if( tauJetDeltaRmin_>=0.8 && tauJetDeltaRmin_<1.0 ) ScaleFactorTau_ = tauFRdR2B_SF;
      if( tauJetDeltaRmin_>=0.7 && tauJetDeltaRmin_<0.8 ) ScaleFactorTau_ = tauFRdR1B_SF;
    }
    if( !isTauBarrel_ ){//endcap
      if( tauJetDeltaRmin_>=1.0                         ) ScaleFactorTau_ = tauFRdR3E_SF;
      if( tauJetDeltaRmin_>=0.8 && tauJetDeltaRmin_<1.0 ) ScaleFactorTau_ = tauFRdR2E_SF;
      if( tauJetDeltaRmin_>=0.7 && tauJetDeltaRmin_<0.8 ) ScaleFactorTau_ = tauFRdR1E_SF;
    }
  }
  return ScaleFactorMu_*ScaleFactorTau_;
}
// // //
double analysisClass::EtaToTheta( double Eta ){
  double AbsEta=fabs(Eta);
  return (double)(2*TMath::ATan(TMath::Exp(-AbsEta)));
}
// // //
double analysisClass::ThetaToEta( double Theta ){
  double AbsTheta=fabs(Theta);
  return (double)(-TMath::Log(TMath::Tan(AbsTheta/2)));
}
// // //
double analysisClass::GetAveAbsEta( ){
  //
  double SumAbsTheta_ = 0;
  int    totalN     = 0;
  //
  for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
    if( !jetRisoCheck(iJetR)   )continue;
    totalN++;
    SumAbsTheta_+=EtaToTheta(fabs(PFJetEta->at(iJetR)));
  }
  for(unsigned int iElR=0; iElR<ElectronPt->size(); iElR++){
    if( !elRisoCheck(iElR)        )continue;
    totalN++;
    SumAbsTheta_+=EtaToTheta(fabs(ElectronEta->at(iElR)));
  }
  for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
    if( !muRisoCheck(iMuR)        )continue;
    totalN++;
    SumAbsTheta_+=EtaToTheta(fabs(MuonEta->at(iMuR)));
  }
  for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
    if( !tauRisoCheck(iTauR) )continue;
    totalN++;
    SumAbsTheta_+=EtaToTheta(fabs(HPSTauEta->at(iTauR)));
  }
  //
  if( totalN>0 ){
    return ThetaToEta( (SumAbsTheta_)/((double)(totalN)) );
  }
  else 
    return -1;
}
// // //
double analysisClass::GetAveAbsEtaV2( ){
  //
  double MinAbsEta_ = 999999;
  double MaxAbsEta_ = 0;
  double SumAbsTheta_ = 0;
  int    totalN     = 0;
  int    jetN       = 0;
  int    lepN       = 0;
  //
  for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
    if( !jetRisoCheck(iJetR)   )continue;
    totalN++;
    jetN++;
    if( fabs(PFJetEta->at(iJetR))<MinAbsEta_ ) MinAbsEta_ = fabs(PFJetEta->at(iJetR));
    if( fabs(PFJetEta->at(iJetR))>MaxAbsEta_ ) MaxAbsEta_ = fabs(PFJetEta->at(iJetR));
    SumAbsTheta_+=EtaToTheta(fabs(PFJetEta->at(iJetR)));
  }
  for(unsigned int iElR=0; iElR<ElectronPt->size(); iElR++){
    if( !elRisoCheck(iElR)        )continue;
    totalN++;
    lepN++;
    if( fabs(ElectronEta->at(iElR))<MinAbsEta_ ) MinAbsEta_ = fabs(ElectronEta->at(iElR));
    if( fabs(ElectronEta->at(iElR))>MaxAbsEta_ ) MaxAbsEta_ = fabs(ElectronEta->at(iElR));
    SumAbsTheta_+=EtaToTheta(fabs(ElectronEta->at(iElR)));
  }
  for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
    if( !muRisoCheck(iMuR)        )continue;
    totalN++;
    lepN++;
    if( fabs(MuonEta->at(iMuR))<MinAbsEta_ ) MinAbsEta_ = fabs(MuonEta->at(iMuR));
    if( fabs(MuonEta->at(iMuR))>MaxAbsEta_ ) MaxAbsEta_ = fabs(MuonEta->at(iMuR));
    SumAbsTheta_+=EtaToTheta(fabs(MuonEta->at(iMuR)));
  }
  for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
    if( !tauRisoCheck(iTauR) )continue;
    totalN++;
    lepN++;
    if( fabs(HPSTauEta->at(iTauR))<MinAbsEta_ ) MinAbsEta_ = fabs(HPSTauEta->at(iTauR));
    if( fabs(HPSTauEta->at(iTauR))>MaxAbsEta_ ) MaxAbsEta_ = fabs(HPSTauEta->at(iTauR));
    SumAbsTheta_+=EtaToTheta(fabs(HPSTauEta->at(iTauR)));
  }
  //
  if( totalN>2 ){
    return ThetaToEta( (SumAbsTheta_-EtaToTheta(MinAbsEta_)-EtaToTheta(MaxAbsEta_))/((double)(totalN)-2.0) );
  }
  else 
    return -1;
}
// // //
double analysisClass::GetAveAbsEtaV3( ){
  //
  double SumAbsTheta_ = 0;
  int    totalN     = 0;
  //
  for(unsigned int iElR=0; iElR<ElectronPt->size(); iElR++){
    if( !elRisoCheck(iElR)        )continue;
    totalN++;
    SumAbsTheta_+=EtaToTheta(fabs(ElectronEta->at(iElR)));
  }
  for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
    if( !muRisoCheck(iMuR)        )continue;
    totalN++;
    SumAbsTheta_+=EtaToTheta(fabs(MuonEta->at(iMuR)));
  }
  for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
    if( !tauRisoCheck(iTauR) )continue;
    totalN++;
    SumAbsTheta_+=EtaToTheta(fabs(HPSTauEta->at(iTauR)));
  }
  //
  if( totalN>0 ){
    return ThetaToEta( (SumAbsTheta_)/((double)(totalN)) );
  }
  else
    return -1;
}
// // //
