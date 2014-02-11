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
//#include "LAC/ADDON4_LAC.C"///  LQgen functions //fake
#include "LAC/ADDON5_LAC.C" //gen-reco matching 
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
   //--// InitTauFunctionParameters(); //This is hard-coded!  (no such function)
   InitMuonFunctionParameters();
   //

   //JER_1sigmaUP   = false; //1 --> ST
   //JER_1sigmaDOWN = false; //2 --> ST
   //JEC_1sigmaUP   = false; //3 --> ST
   //JEC_1sigmaDOWN = false; //4 --> ST
   //Tau_1sigmaUP   = false; //5 --> event selection, ST
   //Tau_1sigmaDOWN = false; //6 --> event selection, ST
   //El_1sigmaUP    = false; //7 --> ST
   //El_1sigmaDOWN  = false; //8 --> ST
   //Mu_1sigmaUP    = false; //9 --> event selection, ST (small)
   //Mu_1sigmaDOWN  = false; //10--> event selection, ST (small)
   //////////book histos here
   TH1D* SystematicsTThisto     = new TH1D("SystematicsTThisto",     "SystematicsTThisto",     12,-0.5,11.5 );
   TH1D* SystematicsTTW1histo   = new TH1D("SystematicsTTW1histo",   "SystematicsTTW1histo",   12,-0.5,11.5 );
   TH1D* SystematicsTTPPhisto   = new TH1D("SystematicsTTPPhisto",   "SystematicsTTPPhisto",   12,-0.5,11.5 );
   TH1D* SystematicsTTPPW1histo = new TH1D("SystematicsTTPPW1histo", "SystematicsTTPPW1histo", 12,-0.5,11.5 );
   TH1D* SystematicsTTPPQFhisto   = new TH1D("SystematicsTTPPQFhisto",   "SystematicsTTPPQFhisto",   12,-0.5,11.5 );
   TH1D* SystematicsTTPPQFW1histo = new TH1D("SystematicsTTPPQFW1histo", "SystematicsTTPPQFW1histo", 12,-0.5,11.5 );
   TH1D* SystematicsTTPPQChisto   = new TH1D("SystematicsTTPPQChisto",   "SystematicsTTPPQChisto",   12,-0.5,11.5 );
   TH1D* SystematicsTTPPQCW1histo = new TH1D("SystematicsTTPPQCW1histo", "SystematicsTTPPQCW1histo", 12,-0.5,11.5 );
   //
   TH1D* JEC1sigmaUP_METcorrOverMEThisto       = new TH1D("JEC1sigmaUP_METcorrOverMEThisto",      "JEC1sigmaUP_METcorrOverMEThisto"      ,1000,0,10);
   TH1D* JEC1sigmaUP_jetPtcorrOverjetPthisto   = new TH1D("JEC1sigmaUP_jetPtcorrOverjetPthisto",  "JEC1sigmaUP_jetPtcorrOverjetPthisto"  ,1000,0,10);
   TH1D* JEC1sigmaDOWN_METcorrOverMEThisto     = new TH1D("JEC1sigmaDOWN_METcorrOverMEThisto",    "JEC1sigmaDOWN_METcorrOverMEThisto"    ,1000,0,10);
   TH1D* JEC1sigmaDOWN_jetPtcorrOverjetPthisto = new TH1D("JEC1sigmaDOWN_jetPtcorrOverjetPthisto","JEC1sigmaDOWN_jetPtcorrOverjetPthisto",1000,0,10);
   //
   TH1D* LQ3RecoMasshisto     = new TH1D("LQ3RecoMasshisto",    "LQ3RecoMasshisto",     500,0,5000 );
   TH1D* LQ3RecoVisMasshisto  = new TH1D("LQ3RecoVisMasshisto", "LQ3RecoVisMasshisto",  500,0,5000 );
   TH1D* TopPthisto           = new TH1D("TopPthisto",          "TopPthisto",           500,0,5000 );
   TH1D* TopMomentumhisto     = new TH1D("TopMomentumhisto",    "TopMomentumhisto",     500,0,5000 );
   TH1D* TopTauVisSumPthisto  = new TH1D("TopTauVisSumPthisto", "TopTauVisSumPthisto",  500,0,5000 );
   TH1D* TopTauSumPthisto     = new TH1D("TopTauSumPthisto",    "TopTauSumPthisto",     500,0,5000 );
   TH2D* TopTauVisPt2Dhisto   = new TH2D("TopTauVisPt2Dhisto",  "TopTauVisPt2Dhisto",   200,0,1000,120,0,600 );
   TH2D* TopTauPt2Dhisto      = new TH2D("TopTauPt2Dhisto",     "TopTauPt2Dhisto",      200,0,1000,200,0,1000 );



   /////////initialize variables

   // Get the provided PileUp Weights for MC.
   if( !isData ){
     std::cout<<"-------- Verifying Pileup Weights --------"<<std::endl;
     for(int pu=0; pu<60; pu++){//[0-60)
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

     // Set the evaluation of the cuts to false and clear the variable values and filled status
     resetCuts();
     //

     // WEIGHTS
     // ---- pileup
     double safePileupWeights_=0;
     safePileupWeights_=safePileupWeights();

     //  -- JSON SKIM -- unaffected by energy systematics
     int passJSON_=0;
     if( isData  ) passJSON_ = passJSON(run,ls,isData);
     if( !isData ) passJSON_ = 1;
     fillVariableWithValue("PassJSON", passJSON_ );//..............// returns 0, 1                             

     //  -- EVENT FILTER SKIM -- unaffected by energy systematics
     int passEventFilter_=0;
     if( passEventFilter() ) passEventFilter_ = 1;
     fillVariableWithValue("PassEventFilter", passEventFilter_ );//.// returns 0, 1 


     // Disabled..
     fillVariableWithValue("PassTrig",             1 );
     fillVariableWithValue("PassOfflineDilepton",  1 );
     fillVariableWithValue("PassAllMuHLTmatching", 1 );
     fillVariableWithValue("PassOfflineCuts",      1 );
     evaluateCuts();
     //---------------------------------------------------------------------------------
     if( !passedCut("PassJSON") || !passedCut("PassTrig") || !passedCut("PassEventFilter") || !passedCut("PassOfflineDilepton") ||
	 !passedCut("PassAllMuHLTmatching") || !passedCut("PassOfflineCuts") ) continue;


     for(unsigned int iSys=0; iSys<12; iSys++){ //loop over all systematics per event that passes the pre-skim as given above
       //if( iSys!=0 && iSys!=3 && iSys!=4 ) continue;
       if( iSys>0 ) continue;
       ApplyLepJetMetSystematics(iSys);
       
       ltemMuMu.clear();
       ltemMuTau.clear();
       // -- RUN LTEM 
       which_MuTau(ltemMuTau);
       
       // WEIGHTS
       // ---- trigger 
       unsigned int triggerMuon=99;
       if( ltemMuTau.size()==2 ) triggerMuon=ltemMuTau[0];
       if( ltemMuTau.size()!=2 ) continue;
       //
       double TriggerEfficiencyWeights_;
       if( muPtcorr(triggerMuon)> 45                                ){
	 TriggerEfficiencyWeights_ = Mu40e2p1_ScaleFactor( muPtcorr(triggerMuon), MuonEta->at(triggerMuon) ); }
       if( muPtcorr(triggerMuon)<=45 &&  muRTightCheck(triggerMuon) ){
	 TriggerEfficiencyWeights_ = IsoMu24e2p1_Eff(      muPtcorr(triggerMuon), MuonEta->at(triggerMuon) ); }
       if( muPtcorr(triggerMuon)<=45 && !muRTightCheck(triggerMuon) ){
	 TriggerEfficiencyWeights_ = Mu24e2p1_ScaleFactor( muPtcorr(triggerMuon), MuonEta->at(triggerMuon) ); }
       //
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
       
       //  -- TRIGGER SKIM -- APPLIED HERE NOW
       int passTrigger_=0;
       if( muPtcorr(triggerMuon)> 45                                ) passTrigger_ = SingleMu40_passTrigger();//<<<<<<<<< this is Mu40 trigger
       if( muPtcorr(triggerMuon)<=45 &&  muRTightCheck(triggerMuon) ) passTrigger_ = SingleMu_passTrigger();  //<<<<<<<<< this is IsoMu24 trigger
       if( muPtcorr(triggerMuon)<=45 && !muRTightCheck(triggerMuon) ) passTrigger_ = HLT_MuPT_eta2p1_passTrigger(); //<<<<<<<<< this is Mu24 trigger
       
       //  -- DILEPTON SKIM - MuTau -- APPLIED HERE NOW
       int isOfflineDilepton_=0;
       if( isMuTauDR0p30() ) isOfflineDilepton_ = 1;
       
       // -- HLT MATCHING SKIM -- APPLIED HERE NOW
       int passAllMuHLTmatching_=0;
       if(muPtcorr(triggerMuon)> 45                                && RecoHLTdeltaRmin_SingleMu40Trigger(triggerMuon)<0.15) passAllMuHLTmatching_=1;
       if(muPtcorr(triggerMuon)<=45 &&  muRTightCheck(triggerMuon) && RecoHLTdeltaRmin_SingleMuTrigger(triggerMuon)<0.15  ) passAllMuHLTmatching_=1;
       if(muPtcorr(triggerMuon)<=45 && !muRTightCheck(triggerMuon) && RecoHLTdeltaRmin_SingleMu24Trigger(triggerMuon)<0.15) passAllMuHLTmatching_=1;
     
       // -- OFFLINE SKIM -- APPLIED HERE NOW
       int OfflineCuts_=0;
       if( ltemMuTau.size()==2 && //muPtcorr(ltemMuTau[0])>45 &&
	   //ST()>400  && JetCounter()>=2 &&
	   JetCounter()>=2 &&
	   tauPtcorr(ltemMuTau[1])>35 
	   // 
	   // Optimized Cuts
	   && ST()>410 && tauPtcorr(ltemMuTau[1])>35 && GetAveAbsEtaV3()<0.9 //LQM250
	   //&& ST()>470 && tauPtcorr(ltemMuTau[1])>50 && GetAveAbsEtaV3()<0.9 //LQM300
	   //&& ST()>490 && tauPtcorr(ltemMuTau[1])>50 && GetAveAbsEtaV3()<0.9 //LQM350
	   //&& ST()>680 && tauPtcorr(ltemMuTau[1])>65 && GetAveAbsEtaV3()<0.9 //LQM400
	   //&& ST()>700 && tauPtcorr(ltemMuTau[1])>70 && GetAveAbsEtaV3()<0.9 //LQM450
	   //&& ST()>770 && tauPtcorr(ltemMuTau[1])>70 && GetAveAbsEtaV3()<0.9 //LQM500
	   //&& ST()>800 && tauPtcorr(ltemMuTau[1])>65 && GetAveAbsEtaV3()<0.9 //LQM550
	   //&& ST()>850 && tauPtcorr(ltemMuTau[1])>65 && GetAveAbsEtaV3()<0.9 //LQM600, 650
	   //&& ST()>850 && tauPtcorr(ltemMuTau[1])>85 && GetAveAbsEtaV3()<0.9 //LQM700, 750
	   //
	   ) OfflineCuts_=1;
       //
       
       // standard cuts - now affected by energy systematics
       if( passTrigger_==0          ) continue;  
       if( isOfflineDilepton_==0    ) continue;  
       if( passAllMuHLTmatching_==0 ) continue;  
       if( OfflineCuts_==0          ) continue;  
       if( !muRTightCheck(ltemMuTau[0]) || !tauRTightCheck(ltemMuTau[1]) ) continue; //require TT  (signal box)
       //
       cout<<"Event: "<<jentry<<"   iSys: "<<iSys<<endl;
       //for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
       //if( !jetRisoCheck(iJetR) ) continue;
       //cout<<iJetR<<"::"<<jetPtcorr(iJetR)<<" btag: "<<btag_jetRCheck(iJetR)<<endl;
       //}
       double Mtop=173;
       double MW=80;
       double MtopDeltaReco=9999;
       TLorentzVector topVector;
       for(unsigned int iJetR1=0; iJetR1<PFJetPt->size(); iJetR1++){
	 if( !jetRisoCheck(iJetR1) ) continue;
	 for(unsigned int iJetR2=(iJetR1+1); iJetR2<PFJetPt->size(); iJetR2++){
	   if( !jetRisoCheck(iJetR2) ) continue;
	   TLorentzVector jet1, jet2;
	   jet1.SetPtEtaPhiM( jetPtcorr(iJetR1), PFJetEta->at(iJetR1), PFJetPhi->at(iJetR1) , 0 );
	   jet2.SetPtEtaPhiM( jetPtcorr(iJetR2), PFJetEta->at(iJetR2), PFJetPhi->at(iJetR2) , 0 );
	   if( fabs((jet1+jet2).M()-Mtop)<30 ){
	     MtopDeltaReco=fabs((jet1+jet2).M()-Mtop);
	     //cout<<iJetR1<<" - "<<iJetR2<<"::"<<(jet1+jet2).M()<<endl;
	     topVector+=jet1;
	     topVector+=jet2;
	   }
	   for(unsigned int iJetR3=(iJetR2+1); iJetR3<PFJetPt->size(); iJetR3++){
	     if( !jetRisoCheck(iJetR3) ) continue;
	     TLorentzVector jet3;
	     jet3.SetPtEtaPhiM( jetPtcorr(iJetR3), PFJetEta->at(iJetR3), PFJetPhi->at(iJetR3) , 0 );
	     if( fabs((jet1+jet2+jet3).M()-Mtop)<30 && MtopDeltaReco>fabs((jet1+jet2+jet3).M()-Mtop) ){
	       //cout<<iJetR1<<" - "<<iJetR2<<" - "<<iJetR3<<"::"<<(jet1+jet2+jet3).M()<<endl;
	       topVector+=jet1;
	       topVector+=jet2;
	       topVector+=jet3;
	     }
	   }
	   //
	 }
       }
       //
       TLorentzVector LTEMTauVector;
       LTEMTauVector.SetPtEtaPhiM( tauPtcorr(ltemMuTau[1]), HPSTauEta->at(ltemMuTau[1]),  HPSTauPhi->at(ltemMuTau[1]), 0 );
       TLorentzVector LTEMMuVector;
       LTEMMuVector.SetPtEtaPhiM( muPtcorr(ltemMuTau[0]), MuonEta->at(ltemMuTau[0]),  MuonPhi->at(ltemMuTau[0]), 0 );
       //
       TLorentzVector LQTauVector;
       TLorentzVector LQTauVisVector;
       TLorentzVector LQTauMETVector;
       TVector2 metVector2D;
       TVector2 LQTauVisVector2D;
       //
       //if( LTEMTauVector.Pt()>=LTEMMuVector.Pt() ) cout<<"LTEMTauPT > LTEMMuPT"<<endl;
       //if( LTEMTauVector.Pt()< LTEMMuVector.Pt() ) cout<<"LTEMTauPT < LTEMMuPT !!!<<<<<< !!!"<<endl;
       //if( LTEMTauVector.Pt()>=LTEMMuVector.Pt() ) LQTauVisVector=LTEMTauVector;
       //if( LTEMTauVector.Pt()< LTEMMuVector.Pt() ) LQTauVisVector=LTEMMuVector;
       LQTauVisVector=LTEMTauVector;
       //
       if( fabs((topVector).M()-Mtop)<30 ){
	 //
	 topVector.SetPtEtaPhiM( topVector.Pt(),  topVector.Eta(),  topVector.Phi(), 173.0 );//SET TOP MASS
	 //
	 //cout<<" TOP     RecoMass: "<<(topVector).M()<<endl;
	 cout<<" LQ3Vis  RecoMass: "<<(LQTauVisVector+topVector).M()<<endl;
	 //cout<<" LQ3wTau RecoMass: "<<(LTEMTauVector+topVector).M()<<endl;
	 //cout<<" LQ3wMu  RecoMass: "<<(LTEMMuVector+topVector).M()<<endl;
	 LQ3RecoVisMasshisto->Fill( (LQTauVisVector+topVector).M(), w);
	 TopPthisto->Fill( topVector.Pt(), w );
	 TopMomentumhisto->Fill( topVector.P(), w );
	 TopTauVisSumPthisto->Fill( topVector.Pt()+LQTauVisVector.Pt(), w );
	 TopTauVisPt2Dhisto->Fill( topVector.Pt(), LQTauVisVector.Pt(), w );
	 //if( topVector.Pt()>LQTauVisVector.Pt() ){
	 //LQTauMETVector.SetPtEtaPhiM( (topVector.Pt()-LQTauVisVector.Pt()), LQTauVisVector.Eta(), LQTauVisVector.Phi(), 0 );
	 //cout<<" LQ3     RecoMass: "<<(LQTauVisVector+topVector+LQTauMETVector).M()<<endl;
	 //LQ3RecoMasshisto->Fill( (LQTauVisVector+LQTauMETVector+topVector).M(), w);       
	 //}
	 //
	 metVector2D.SetMagPhi( METcorr("Pt"), METcorr("Phi") );
	 LQTauVisVector2D.SetMagPhi( 1, LQTauVisVector.Phi() );//unit LQTauVis vector
	 if( fabs(LQTauVisVector2D.DeltaPhi(metVector2D))<(TMath::Pi()/2) ){
	   //cout<<"          MET: "<<METcorr("Pt")<<endl;
	   //cout<<"          MET: "<<metVector2D.Mod()<<endl;
	   //cout<<"Projected MET: "<<(metVector2D.Proj(LQTauVisVector2D)).Mod()<<endl;
	   //LQTauVisVector2D.SetMagPhi( 32, LQTauVisVector.Phi() );//unit LQTauVis vector
	   //cout<<"Projected2MET: "<<(metVector2D.Proj(LQTauVisVector2D)).Mod()<<endl;
	   double newLQTauPt=(metVector2D.Proj(LQTauVisVector2D)).Mod()+LQTauVisVector.Pt();
	   LQTauVector.SetPtEtaPhiM( newLQTauPt, LQTauVisVector.Eta(), LQTauVisVector.Phi(), 0 );
	   LQ3RecoMasshisto->Fill( (LQTauVector+topVector).M(), w);
	   TopTauSumPthisto->Fill( topVector.Pt()+LQTauVector.Pt(), w );
	   TopTauPt2Dhisto->Fill( topVector.Pt(), LQTauVector.Pt(), w );
	   cout<<" LQ3  RecoMass: "<<(LQTauVector+topVector).M()<<endl;
	 }
       }
       cout<<endl;



     }//loop over systematics
     //---------------------------------------------------------------------------------

     ////////////////////// User's code ends here ///////////////////////
   } // End loop over events

   //////////write histos 
   SystematicsTThisto->Write();
   SystematicsTTW1histo->Write();
   SystematicsTTPPhisto->Write();
   SystematicsTTPPW1histo->Write();
   SystematicsTTPPQFhisto->Write();
   SystematicsTTPPQFW1histo->Write();
   SystematicsTTPPQChisto->Write();
   SystematicsTTPPQCW1histo->Write();
   //
   JEC1sigmaUP_METcorrOverMEThisto->Write();
   JEC1sigmaUP_jetPtcorrOverjetPthisto->Write();
   JEC1sigmaDOWN_METcorrOverMEThisto->Write();
   JEC1sigmaDOWN_jetPtcorrOverjetPthisto->Write();
   //
   LQ3RecoMasshisto->Write();
   LQ3RecoVisMasshisto->Write();
   TopPthisto->Write();
   TopMomentumhisto->Write();
   TopTauVisSumPthisto->Write();
   TopTauSumPthisto->Write();
   //
   TopTauVisPt2Dhisto->Write();
   TopTauPt2Dhisto->Write();

   //////////
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
// // //
