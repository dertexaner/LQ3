#define analysisClass_cxx
#include "analysisClass.h"
#include <TH1.h>
#include <TH2.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TVector3.h>
#include "TGraphAsymmErrors.h"
//
//LAC
//#include "LAC/ADDON1_LAC.C" //this is included in LAC/ADDON2_LAC.C
#include "LAC/ADDON2_LAC.C"
#include "LAC/ADDON3_LAC.C"
//#include "LAC/ADDON4_LAC.C"///  LQgen functions //fake
//#include "LAC/ADDON5_LAC.C"/// TOPgen functions //fake
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

   ////////// Set analysis Object ID variables here
   ObjDef  = "LI";
   BTagDef = "CSVM";
   idDef   = "CUT";
   MuonPtCut = 25;
   MuonEtaCut= 2.1;
   TauPtCut  = 20;
   ApplyAbsMuonIsolation=false;
   removeOverlaps=true;
   //removeOverlaps=false;
   //
   LumiA=891; 
   LumiB=4428;
   LumiC=7018;
   LumiD=7248;
   //
   bool debug_=false;

   //////////book histos here
   TH1D* ProvidedPileUpWeightshisto  = new TH1D("ProvidedPileUpWeightshisto","ProvidedPileUpWeightshisto",10000,0,100);//distribution of the provided PileUp-weights
   TH1D* AppliedPileUpWeightshisto   = new TH1D("AppliedPileUpWeightshisto", "AppliedPileUpWeightshisto", 10000,0,100);//distribution of the applied PileUp-weights
   //

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

     std::vector<unsigned int> probeTauArray;
     std::vector<unsigned int> probeTauMatchedJetArray;
     std::vector<int>          probeTauJetFlavorArray;
     std::vector<double>       probeTauJetDeltaRminArray;
     std::vector<double>       probeTauDeltaRminTauArray;
     std::vector<double>       probeTauDeltaRminMuArray;
     std::vector<double>       probeTauDeltaRminElArray;

     std::vector<unsigned int> probeMuArray;
     std::vector<unsigned int> probeMuMatchedJetArray;
     std::vector<int>          probeMuJetFlavorArray;
     std::vector<double>       probeMuJetDeltaRminArray;
     std::vector<double>       probeMuDeltaRminTauArray;
     std::vector<double>       probeMuDeltaRminMuArray;
     std::vector<double>       probeMuDeltaRminElArray;


     //  -- WEIGHTS - see below
     //  -- JSON SKIM
     //  -- TRIGGER SKIM

     //  -- EVENT FILTER SKIM
     int passEventFilter_=0;
     if( passEventFilter() ) passEventFilter_ = 1;
     fillVariableWithValue("PassEventFilter", passEventFilter_ );//.// returns 0, 1 

     //  -- LEPTON SKIM - At Least 1 LI Muon or 1 LI Tau
     int isOfflineLepton_=0;
     //
     for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
       if( !muRisoCheck(iMuR) ) continue;
       probeMuArray.push_back(iMuR);
       //
       //cout<<endl;
       // Mu-Jet Matching
       TLorentzVector mucand, jetcand;
       double muJetDeltaRmin_=9999.0;
       unsigned int iMuJetR=999;
       mucand.SetPtEtaPhiM( MuonPt->at(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0 );
       for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
         jetcand.SetPtEtaPhiM( PFJetPt->at(iJetR),  PFJetEta->at(iJetR),  PFJetPhi->at(iJetR),  0 );
         if( mucand.DeltaR(jetcand)<muJetDeltaRmin_ ){ muJetDeltaRmin_=mucand.DeltaR(jetcand); iMuJetR=iJetR; }//picks the smallest deltaR and JET
	 //if( mucand.DeltaR(jetcand)<0.3 ) cout<< "Mu - Jet: "<<mucand.DeltaR(jetcand)<<endl;
       }
       //cout<< "Mu - Jet: "<<muJetDeltaRmin_<<"     JetEnergy/MuEnergy="<<setprecision(2)<<PFJetEnergy->at(iMuJetR)/MuonEnergy->at(iMuR)<<endl;
       if( muJetDeltaRmin_>=0.2 ) iMuJetR=999;//checks if nearest JET is near enough!
       probeMuMatchedJetArray.push_back(iMuJetR);
       if( iMuJetR==999 ) probeMuJetFlavorArray.push_back( (int)(999) );
       if( iMuJetR!=999 ) probeMuJetFlavorArray.push_back( (int)(PFJetPartonFlavour->at(iMuJetR)) );
       //
       //
       /*
       muJetDeltaRmin_=9999.0;
       for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
	 //if( PFJetPt->at(iJetR)<20       ) continue;
         //if( fabs(PFJetEta->at(iJetR))>3 ) continue;
         jetcand.SetPtEtaPhiM( PFJetPt->at(iJetR),  PFJetEta->at(iJetR),  PFJetPhi->at(iJetR),  0 );
         //if( PFJetMuonEnergyFraction->at(iJetR)>0 && mucand.DeltaR(jetcand)<0.55 )  continue;
         //if( PFJetMuonEnergyFraction->at(iJetR)>0.7 && mucand.DeltaR(jetcand)<0.55 )  continue;
         //if( PFJetMuonEnergyFraction->at(iJetR)>0 && fabs(1-((MuonEnergy->at(iMuR)/PFJetEnergy->at(iJetR))/PFJetMuonEnergyFraction->at(iJetR)))<0.3 ) continue;
	 //
         //if( PFJetMuonEnergyFraction->at(iJetR)>0.85 && mucand.DeltaR(jetcand)<0.5 )  continue;
         //if( PFJetMuonEnergyFraction->at(iJetR)>0.60 && mucand.DeltaR(jetcand)<0.5 )  continue;
         if( mucand.DeltaR(jetcand)<0.05 || (PFJetMuonEnergyFraction->at(iJetR)>0.70 && mucand.DeltaR(jetcand)<0.5) )  continue;
         if( mucand.DeltaR(jetcand)<0.5 ){
           jetcand=(jetcand-mucand);
         }
         if( jetcand.Pt()<20       ) continue;
         if( fabs(jetcand.Eta())>4 ) continue;
	 //
         if( mucand.DeltaR(jetcand)<muJetDeltaRmin_ ){ muJetDeltaRmin_=mucand.DeltaR(jetcand);}
       }
       */
       //probeMuJetDeltaRminArray.push_back(muJetDeltaRmin_);
       probeMuJetDeltaRminArray.push_back(muJetDeltaRmin(iMuR));
       //
     }
     //
     for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
       if( !tauRisoCheck(iTauR) ) continue;
       probeTauArray.push_back(iTauR);
       //
       // Tau-Jet Matching
       TLorentzVector taucand, jetcand;
       double tauJetDeltaRmin_=9999.0;
       unsigned int iTauJetR=999;
       taucand.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
       //
       for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
	 jetcand.SetPtEtaPhiM( PFJetPt->at(iJetR),  PFJetEta->at(iJetR),  PFJetPhi->at(iJetR),  0 );
	 if( taucand.DeltaR(jetcand)<tauJetDeltaRmin_ ){ tauJetDeltaRmin_=taucand.DeltaR(jetcand); iTauJetR=iJetR; }//picks the smallest deltaR and JET
       }
       //cout<< "Tau - Jet: "<<tauJetDeltaRmin_<<"   JetPt/TauPt="<<PFJetPt->at(iTauJetR)/HPSTauPt->at(iTauR)<<endl;
       if( tauJetDeltaRmin_>=0.2 ) iTauJetR=999;//checks if nearest JET is near enough!
       probeTauMatchedJetArray.push_back(iTauJetR);
       if( iTauJetR==999 ) probeTauJetFlavorArray.push_back( (int)(999) );
       if( iTauJetR!=999 ) probeTauJetFlavorArray.push_back( (int)(PFJetPartonFlavour->at(iTauJetR)) );
       //
       //
       /*
       tauJetDeltaRmin_=9999.0;
       for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
	 if( PFJetPt->at(iJetR)<20       ) continue;
	 if( fabs(PFJetEta->at(iJetR))>4 ) continue;
         jetcand.SetPtEtaPhiM( PFJetPt->at(iJetR),  PFJetEta->at(iJetR),  PFJetPhi->at(iJetR),  0 );
         if( taucand.DeltaR(jetcand)<tauJetDeltaRmin_ && taucand.DeltaR(jetcand)>0.2 ){ tauJetDeltaRmin_=taucand.DeltaR(jetcand);}
       }
       */
       //probeTauJetDeltaRminArray.push_back(tauJetDeltaRmin_);
       probeTauJetDeltaRminArray.push_back(tauJetDeltaRmin(iTauR));
       //
     }

     //TLorentzVector jetcand1, jetcand2;
     //for(unsigned int iJetR1=0; iJetR1<PFJetPt->size(); iJetR1++){
     //if( PFJetPt->at(iJetR1)<50 ) continue;
     //jetcand1.SetPtEtaPhiM( PFJetPt->at(iJetR1),  PFJetEta->at(iJetR1),  PFJetPhi->at(iJetR1),  0 );
     //for(unsigned int iJetR2=(iJetR1+1); iJetR2<PFJetPt->size(); iJetR2++){
     //if( PFJetPt->at(iJetR2)<50 ) continue;
     // jetcand2.SetPtEtaPhiM( PFJetPt->at(iJetR2),  PFJetEta->at(iJetR2),  PFJetPhi->at(iJetR2),  0 );
     //
     // if( jetcand1.DeltaR(jetcand2)<0.5 ) cout<< "DeltaR(J1,J2): "<<jetcand1.DeltaR(jetcand2)<<endl;
     //
     //  }
     //}
     //if( PFJetPt->at(PFJetPt->size()-1)>20 ) cout<<"lastjet: "<<PFJetPt->at(PFJetPt->size()-1)<<endl;


     if( probeMuArray.size()>0 || probeTauArray.size()>0 ) isOfflineLepton_=1;
     //if( ElCounter()>0 ) isOfflineLepton_=0;//veto on electrons
     fillVariableWithValue("PassOfflineLepton", isOfflineLepton_ );// Returns 0, 1

     // -- HLT MATCHING SKIM
     // -- OFFLINE SKIM

     evaluateCuts();
     //---------------------------------------------------------------------------------
     if( passedCut("PassEventFilter") && passedCut("PassOfflineLepton") ){


       // Reduced Skim variables  -  probe quantities

       double LooseJetHT=0;
       for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
	 if( !jetRLooseCheck(iJetR)   )continue;
	 LooseJetHT+=PFJetPt->at(iJetR);
       }

       double probeMu1Pt;   double probeMu1Eta;   double probeMu1Phi;  double probeMu1Q; double probeMu1HLT; double probeMu1PFIso;
       double probeMu2Pt;   double probeMu2Eta;   double probeMu2Phi;  double probeMu2Q; double probeMu2HLT; double probeMu2PFIso;
       double probeMu1GenDeltaRTau;   double probeMu1GenDeltaRMu;   double probeMu1GenDeltaREl;
       double probeMu2GenDeltaRTau;   double probeMu2GenDeltaRMu;   double probeMu2GenDeltaREl;
       double probeMu1JetDeltaRmin;  double probeMu2JetDeltaRmin;
       double probeMu1JetFlavor;     double probeMu2JetFlavor;

       double  probeMu1DR0p9STpt20; double probeMu1DR0p9STpt20W; double probeMu1DR0p9STpt20WC;
       double  probeMu1DR0p9STpt30; double probeMu1DR0p9STpt30W; double probeMu1DR0p9STpt30WC;
       double  probeMu1DR0p9STpt40; double probeMu1DR0p9STpt40W; double probeMu1DR0p9STpt40WC;
       double  probeMu1DR0p7STpt20; double probeMu1DR0p7STpt20W; double probeMu1DR0p7STpt20WC;
       double  probeMu1DR0p7STpt30; double probeMu1DR0p7STpt30W; double probeMu1DR0p7STpt30WC;
       double  probeMu1DR0p7STpt40; double probeMu1DR0p7STpt40W; double probeMu1DR0p7STpt40WC;
       double  probeMu1DR0p5STpt20; double probeMu1DR0p5STpt20W; double probeMu1DR0p5STpt20WC;
       double  probeMu1DR0p5STpt30; double probeMu1DR0p5STpt30W; double probeMu1DR0p5STpt30WC;
       double  probeMu1DR0p5STpt40; double probeMu1DR0p5STpt40W; double probeMu1DR0p5STpt40WC;

       double  probeTau1DR1p0STpt20; double probeTau1DR1p0STpt20W; double probeTau1DR1p0STpt20WC;
       double  probeTau1DR1p0STpt30; double probeTau1DR1p0STpt30W; double probeTau1DR1p0STpt30WC;
       double  probeTau1DR1p0STpt40; double probeTau1DR1p0STpt40W; double probeTau1DR1p0STpt40WC;
       double  probeTau1DR0p7STpt20; double probeTau1DR0p7STpt20W; double probeTau1DR0p7STpt20WC;
       double  probeTau1DR0p7STpt30; double probeTau1DR0p7STpt30W; double probeTau1DR0p7STpt30WC;
       double  probeTau1DR0p7STpt40; double probeTau1DR0p7STpt40W; double probeTau1DR0p7STpt40WC;
       double  probeTau1DR0p5STpt20; double probeTau1DR0p5STpt20W; double probeTau1DR0p5STpt20WC;
       double  probeTau1DR0p5STpt30; double probeTau1DR0p5STpt30W; double probeTau1DR0p5STpt30WC;
       double  probeTau1DR0p5STpt40; double probeTau1DR0p5STpt40W; double probeTau1DR0p5STpt40WC;

       double probeTau1Pt;  double probeTau1Eta;  double probeTau1Phi;  double probeTau1Iso; double probeTau1Q; double probeTau1DM; double probeTau1ElRej;
       double probeTau2Pt;  double probeTau2Eta;  double probeTau2Phi;  double probeTau2Iso; double probeTau2Q; double probeTau2DM; double probeTau2ElRej;
       double probeTau1GenDeltaRTau;  double probeTau1GenDeltaRMu;  double probeTau1GenDeltaREl;
       double probeTau2GenDeltaRTau;  double probeTau2GenDeltaRMu;  double probeTau2GenDeltaREl;
       double probeTau1JetDeltaRmin;  double probeTau2JetDeltaRmin;
       double probeTau1JetFlavor;     double probeTau2JetFlavor;

       double eventJet1Pt;  double eventJet1Eta;  double eventJet1Phi;
       double eventJet2Pt;  double eventJet2Eta;  double eventJet2Phi;

       double eventData;
       double eventST;
       double eventIsoST;
       double eventSJT;
       double eventrhoJets;
       double eventrhoForHEEP;
       double eventrhoJetsCCPU;
       double eventrhoJetsCN;
       double eventrhoJetsCNT;
       double eventMET;
       double eventMETphi;
       double eventJetN;
       double eventIsoJetN;
       double eventTauN;
       double eventMuN;
       double eventPU;
       double eventPUWeight;


       // WEIGHTS
       // ---- pileup
       double safePileupWeights_=0;
       safePileupWeights_=safePileupWeights();
       AppliedPileUpWeightshisto->Fill( safePileupWeights_ );
       // ---- trigger
       // ---- total = pileup x trigger
       //

       
       // is probeMu Fake ?
       for( unsigned int iMuR=0; iMuR<probeMuArray.size(); iMuR++){
	 TLorentzVector mureco;
	 TLorentzVector taugen;
	 TLorentzVector mugen;
	 TLorentzVector elgen;
	 double DeltaRminTau = 99999;
	 double DeltaRminMu  = 99999;
	 double DeltaRminEl  = 99999;
	 double charge=0;
	 mureco.SetPtEtaPhiM( MuonPt->at(probeMuArray[iMuR]), MuonEta->at(probeMuArray[iMuR]), MuonPhi->at(probeMuArray[iMuR]), 0 );
	 //
	 for( unsigned int iTauT=0; iTauT<GenWTauPt->size(); iTauT++){
	   if( GenWTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
	   taugen.SetPtEtaPhiM( GenWTauPt->at(iTauT),     GenWTauEta->at(iTauT),   GenWTauPhi->at(iTauT), 0 );
	   if( GenWTauPdgId->at(iTauT)>0 ) charge=-1;
	   if( GenWTauPdgId->at(iTauT)<0 ) charge=+1;
	   if( fabs(mureco.DeltaR(taugen))<fabs(DeltaRminTau) ) DeltaRminTau=fabs(mureco.DeltaR(taugen))*charge;
	 }
	 for( unsigned int iTauT=0; iTauT<GenZTauPt->size(); iTauT++){
	   if( GenZTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
	   taugen.SetPtEtaPhiM( GenZTauPt->at(iTauT),     GenZTauEta->at(iTauT),   GenZTauPhi->at(iTauT), 0 );
	   if( GenZTauPdgId->at(iTauT)>0 ) charge=-1;
	   if( GenZTauPdgId->at(iTauT)<0 ) charge=+1;
	   if( fabs(mureco.DeltaR(taugen))<fabs(DeltaRminTau) ) DeltaRminTau=fabs(mureco.DeltaR(taugen))*charge;
	 }
	 //
	 for( unsigned int iMuT=0; iMuT<GenWMuPt->size(); iMuT++){
	   mugen.SetPtEtaPhiM( GenWMuPt->at(iMuT),     GenWMuEta->at(iMuT),   GenWMuPhi->at(iMuT), 0 );
	   if( GenWMuPdgId->at(iMuT)>0 ) charge=-1;
	   if( GenWMuPdgId->at(iMuT)<0 ) charge=+1;
	   if( fabs(mureco.DeltaR(mugen))<fabs(DeltaRminMu) ) DeltaRminMu=fabs(mureco.DeltaR(mugen))*charge;
	 }
	 for( unsigned int iMuT=0; iMuT<GenZMuPt->size(); iMuT++){
	   mugen.SetPtEtaPhiM( GenZMuPt->at(iMuT),     GenZMuEta->at(iMuT),   GenZMuPhi->at(iMuT), 0 );
	   if( GenZMuPdgId->at(iMuT)>0 ) charge=-1;
	   if( GenZMuPdgId->at(iMuT)<0 ) charge=+1;
	   if( fabs(mureco.DeltaR(mugen))<fabs(DeltaRminMu) ) DeltaRminMu=fabs(mureco.DeltaR(mugen))*charge;
	 }
	 //
	 for( unsigned int iElT=0; iElT<GenWElectronPt->size(); iElT++){
	   elgen.SetPtEtaPhiM( GenWElectronPt->at(iElT),     GenWElectronEta->at(iElT),   GenWElectronPhi->at(iElT), 0 );
	   if( GenWElectronPdgId->at(iElT)>0 ) charge=-1;
	   if( GenWElectronPdgId->at(iElT)<0 ) charge=+1;
	   if( fabs(mureco.DeltaR(elgen))<fabs(DeltaRminEl) ) DeltaRminEl=fabs(mureco.DeltaR(elgen))*charge;
	 }
	 for( unsigned int iElT=0; iElT<GenZElectronPt->size(); iElT++){
	   elgen.SetPtEtaPhiM( GenZElectronPt->at(iElT),     GenZElectronEta->at(iElT),   GenZElectronPhi->at(iElT), 0 );
	   if( GenZElectronPdgId->at(iElT)>0 ) charge=-1;
	   if( GenZElectronPdgId->at(iElT)<0 ) charge=+1;
	   if( fabs(mureco.DeltaR(elgen))<fabs(DeltaRminEl) ) DeltaRminEl=fabs(mureco.DeltaR(elgen))*charge;
	 }
	 //
	 probeMuDeltaRminTauArray.push_back(DeltaRminTau);
	 probeMuDeltaRminMuArray.push_back(DeltaRminMu);
	 probeMuDeltaRminElArray.push_back(DeltaRminEl);
       }



       // is probeTau Fake ?
       for( unsigned int iTauR=0; iTauR<probeTauArray.size(); iTauR++){
	 TLorentzVector taureco;
	 TLorentzVector taugen;
	 TLorentzVector mugen;
	 TLorentzVector elgen;
	 double DeltaRminTau = 99999;
	 double DeltaRminMu  = 99999;
	 double DeltaRminEl  = 99999;
	 double charge=0;
	 taureco.SetPtEtaPhiM( HPSTauPt->at(probeTauArray[iTauR]), HPSTauEta->at(probeTauArray[iTauR]), HPSTauPhi->at(probeTauArray[iTauR]), 0 );
	 //
	 for( unsigned int iTauT=0; iTauT<GenWTauPt->size(); iTauT++){
	   if( GenWTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
	   taugen.SetPtEtaPhiM( GenWTauPt->at(iTauT),     GenWTauEta->at(iTauT),   GenWTauPhi->at(iTauT), 0 );
	   if( GenWTauPdgId->at(iTauT)>0 ) charge=-1;
	   if( GenWTauPdgId->at(iTauT)<0 ) charge=+1;
	   if( fabs(taureco.DeltaR(taugen))<fabs(DeltaRminTau) ) DeltaRminTau=fabs(taureco.DeltaR(taugen))*charge;
	 }
	 for( unsigned int iTauT=0; iTauT<GenZTauPt->size(); iTauT++){
	   if( GenZTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
	   taugen.SetPtEtaPhiM( GenZTauPt->at(iTauT),     GenZTauEta->at(iTauT),   GenZTauPhi->at(iTauT), 0 );
	   if( GenZTauPdgId->at(iTauT)>0 ) charge=-1;
	   if( GenZTauPdgId->at(iTauT)<0 ) charge=+1;
	   if( fabs(taureco.DeltaR(taugen))<fabs(DeltaRminTau) ) DeltaRminTau=fabs(taureco.DeltaR(taugen))*charge;
	 }
	 //
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
	 for( unsigned int iElT=0; iElT<GenZElectronPt->size(); iElT++){
	   elgen.SetPtEtaPhiM( GenZElectronPt->at(iElT),     GenZElectronEta->at(iElT),   GenZElectronPhi->at(iElT), 0 );
	   if( GenZElectronPdgId->at(iElT)>0 ) charge=-1;
	   if( GenZElectronPdgId->at(iElT)<0 ) charge=+1;
	   if( fabs(taureco.DeltaR(elgen))<fabs(DeltaRminEl) ) DeltaRminEl=fabs(taureco.DeltaR(elgen))*charge;
	 }
	 //
	 probeTauDeltaRminTauArray.push_back(DeltaRminTau);
	 probeTauDeltaRminMuArray.push_back(DeltaRminMu);
	 probeTauDeltaRminElArray.push_back(DeltaRminEl);
       }
       //--------------------------------------------------------------
       //


       //
       // Jet investigation
       //if( probeMuDeltaRminMuArray.size()>0 && probeMuMatchedJetArray[0]!=999 ){
       //if( probeMuDeltaRminMuArray[0]<0.15 ) cout<<" Real Muon --  nJetConst= "<<PFJetNConstituents->at(probeMuMatchedJetArray[0])<<endl;
       //if( probeMuDeltaRminMuArray[0]>0.15 ) cout<<" Fake Muon --  nJetConst= "<<PFJetNConstituents->at(probeMuMatchedJetArray[0])<<endl;
       //}
       //
       //


       if(  isData ) eventData=1; 
       else eventData=0;
       //

       eventTauN=probeTauArray.size();
       eventMuN=probeMuArray.size();

       //
       eventJet1Pt=-99; eventJet1Eta=-99; eventJet1Phi=-99;
       eventJet2Pt=-99; eventJet2Eta=-99; eventJet2Phi=-99;
       //
       probeMu1Pt=-99; probeMu1Eta=-99; probeMu1Phi=-99; probeMu1Q=-99; probeMu1HLT=-99;  probeMu1PFIso=-99;
       probeMu2Pt=-99; probeMu2Eta=-99; probeMu2Phi=-99; probeMu2Q=-99; probeMu2HLT=-99;  probeMu2PFIso=-99;
       probeMu1GenDeltaRTau=-99;  probeMu1GenDeltaRMu=-99;  probeMu1GenDeltaREl=-99;
       probeMu2GenDeltaRTau=-99;  probeMu2GenDeltaRMu=-99;  probeMu2GenDeltaREl=-99;
       probeMu1JetDeltaRmin=-99;  probeMu2JetDeltaRmin=-99;
       probeMu1JetFlavor=-99;     probeMu2JetFlavor=-99;
       //
       probeTau1Pt=-99; probeTau1Eta=-99; probeTau1Phi=-99; probeTau1Iso=-99; probeTau1Q=-99; probeTau1DM=-99;  probeTau1ElRej=-99;
       probeTau2Pt=-99; probeTau2Eta=-99; probeTau2Phi=-99; probeTau2Iso=-99; probeTau2Q=-99; probeTau2DM=-99;  probeTau2ElRej=-99;
       probeTau1GenDeltaRTau=-99;  probeTau1GenDeltaRMu=-99;  probeTau1GenDeltaREl=-99;
       probeTau2GenDeltaRTau=-99;  probeTau2GenDeltaRMu=-99;  probeTau2GenDeltaREl=-99;
       probeTau1JetDeltaRmin=-99;  probeTau2JetDeltaRmin=-99;
       probeTau1JetFlavor=-99;     probeTau2JetFlavor=-99;
       //
       probeMu1DR0p9STpt20W=0;  probeMu1DR0p9STpt20WC=0; probeMu1DR0p9STpt20=0;
       probeMu1DR0p9STpt30W=0;  probeMu1DR0p9STpt30WC=0; probeMu1DR0p9STpt30=0;
       probeMu1DR0p9STpt40W=0;  probeMu1DR0p9STpt40WC=0; probeMu1DR0p9STpt40=0;
       probeMu1DR0p7STpt20W=0;  probeMu1DR0p7STpt20WC=0; probeMu1DR0p7STpt20=0;
       probeMu1DR0p7STpt30W=0;  probeMu1DR0p7STpt30WC=0; probeMu1DR0p7STpt30=0;
       probeMu1DR0p7STpt40W=0;  probeMu1DR0p7STpt40WC=0; probeMu1DR0p7STpt40=0;
       probeMu1DR0p5STpt20W=0;  probeMu1DR0p5STpt20WC=0; probeMu1DR0p5STpt20=0;
       probeMu1DR0p5STpt30W=0;  probeMu1DR0p5STpt30WC=0; probeMu1DR0p5STpt30=0;
       probeMu1DR0p5STpt40W=0;  probeMu1DR0p5STpt40WC=0; probeMu1DR0p5STpt40=0;
       //
       probeTau1DR1p0STpt20W=0;  probeTau1DR1p0STpt20WC=0; probeTau1DR1p0STpt20=0;
       probeTau1DR1p0STpt30W=0;  probeTau1DR1p0STpt30WC=0; probeTau1DR1p0STpt30=0;
       probeTau1DR1p0STpt40W=0;  probeTau1DR1p0STpt40WC=0; probeTau1DR1p0STpt40=0;
       probeTau1DR0p7STpt20W=0;  probeTau1DR0p7STpt20WC=0; probeTau1DR0p7STpt20=0;
       probeTau1DR0p7STpt30W=0;  probeTau1DR0p7STpt30WC=0; probeTau1DR0p7STpt30=0;
       probeTau1DR0p7STpt40W=0;  probeTau1DR0p7STpt40WC=0; probeTau1DR0p7STpt40=0;
       probeTau1DR0p5STpt20W=0;  probeTau1DR0p5STpt20WC=0; probeTau1DR0p5STpt20=0;
       probeTau1DR0p5STpt30W=0;  probeTau1DR0p5STpt30WC=0; probeTau1DR0p5STpt30=0;
       probeTau1DR0p5STpt40W=0;  probeTau1DR0p5STpt40WC=0; probeTau1DR0p5STpt40=0;


       if( probeMuArray.size()>=1 ){
	 for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
	   //if( PFJetNeutralHadronEnergyFraction->at(iJetR)>=0.99 ) continue;
	   //if( PFJetNeutralEmEnergyFraction->at(iJetR)>=0.99     ) continue;
	   //if( PFJetNConstituents->at(iJetR)<2                   ) continue;
	   //if( fabs(PFJetEta->at(iJetR))<2.4 ){
	   //if( PFJetChargedHadronEnergyFraction->at(iJetR)<=0  ) continue;
	   //if( PFJetChargedMultiplicity->at(iJetR)<=0          ) continue;
	   //if( PFJetChargedEmEnergyFraction->at(iJetR)>=0.99   ) continue;
	   //}
	   if( fabs(PFJetEta->at(iJetR))>3 ) continue;
	   //
	   // to veto muon-jets (prompt muons mis-ID'ed as a jet)
	   //if( PFJetMuonEnergyFraction->at(iJetR)>0.6 ) continue;
	   //
	   if( iJetR==probeMuMatchedJetArray[0] ) continue; //this vetoes the mu-jet for our probe mu: probeMuArray[0].
	   //
	   TLorentzVector probemuon, pfjet, pfmujet;
	   probemuon.SetPtEtaPhiM( MuonPt->at(probeMuArray[0]), MuonEta->at(probeMuArray[0]), MuonPhi->at(probeMuArray[0]), 0 );
	   pfjet.SetPtEtaPhiM( PFJetPt->at(iJetR), PFJetEta->at(iJetR), PFJetPhi->at(iJetR), 0 );
	   double deltaRmujet=fabs(probemuon.DeltaR(pfjet));
	   //For both radii=0.5:
	   //double mujetoverlapweight=(2.0*(0.25*TMath::ACos(deltaRmujet)-0.5*deltaRmujet*TMath::Sqrt(0.25-0.25*deltaRmujet*deltaRmujet)))/(TMath::Pi()*0.25);
	   //
	   // For 0.5 jet,  0.4 muon:
	   double mujetoverlapweight=0;
	   if( deltaRmujet>=0.2 && deltaRmujet<=0.9 ){
	     double dj=(deltaRmujet*deltaRmujet+0.09)/(2.0*deltaRmujet);//0.09 = (0.5^2)-(0.4^2)
	     double dm=(deltaRmujet*deltaRmujet-0.09)/(2.0*deltaRmujet);
	     double Aj=0.5*0.5*TMath::ACos(dj/0.5)-dj*TMath::Sqrt(0.5*0.5-dj*dj);
	     double Am=0.4*0.4*TMath::ACos(dm/0.4)-dm*TMath::Sqrt(0.4*0.4-dm*dm);
	     mujetoverlapweight=(Aj+Am)/(TMath::Pi()*0.4*0.4);
	   }
	   //if( deltaRmujet<0.1 ) mujetoverlapweight=(TMath::Pi()*0.4*0.4)/(TMath::Pi()*0.5*0.5);
	   if( deltaRmujet<0.2 ) mujetoverlapweight=0;
	   //
	   if( PFJetPt->at(iJetR)>20 ){	 
	     double CorrJetPt=PFJetPt->at(iJetR);
             //if( probeMuMatchedJetArray[0]!=999 ){
	     //pfmujet.SetPtEtaPhiM(PFJetPt->at(probeMuMatchedJetArray[0]), PFJetEta->at(probeMuMatchedJetArray[0]), PFJetPhi->at(probeMuMatchedJetArray[0]),0);
             //}
             //if( probeMuMatchedJetArray[0]==999 ){
	     //pfmujet=probemuon;
             //}
             if( deltaRmujet<0.2 ){ CorrJetPt=fabs((pfjet-probemuon).Pt()); }
	     if( deltaRmujet<0.9 ) probeMu1DR0p9STpt20  +=PFJetPt->at(iJetR);
	     if( deltaRmujet<0.9 ) probeMu1DR0p9STpt20W +=(PFJetPt->at(iJetR)*mujetoverlapweight);
	     if( deltaRmujet<0.9 ) probeMu1DR0p9STpt20WC+=(CorrJetPt*mujetoverlapweight);
	     if( deltaRmujet<0.7 ) probeMu1DR0p7STpt20  +=PFJetPt->at(iJetR);
	     if( deltaRmujet<0.7 ) probeMu1DR0p7STpt20W +=(PFJetPt->at(iJetR)*mujetoverlapweight);
	     if( deltaRmujet<0.7 ) probeMu1DR0p7STpt20WC+=(CorrJetPt*mujetoverlapweight);
	     if( deltaRmujet<0.5 ) probeMu1DR0p5STpt20  +=PFJetPt->at(iJetR);
	     if( deltaRmujet<0.5 ) probeMu1DR0p5STpt20W +=(PFJetPt->at(iJetR)*mujetoverlapweight);
	     if( deltaRmujet<0.5 ) probeMu1DR0p5STpt20WC+=(CorrJetPt*mujetoverlapweight);
	   }
	   if( PFJetPt->at(iJetR)>30 ){	 
	     double CorrJetPt=PFJetPt->at(iJetR);
             //if( probeMuMatchedJetArray[0]!=999 ){
	     //pfmujet.SetPtEtaPhiM(PFJetPt->at(probeMuMatchedJetArray[0]), PFJetEta->at(probeMuMatchedJetArray[0]), PFJetPhi->at(probeMuMatchedJetArray[0]),0);
             //}
             //if( probeMuMatchedJetArray[0]==999 ){
	     //pfmujet=probemuon;
             //}
             if( deltaRmujet<0.2 ){ CorrJetPt=fabs((pfjet-probemuon).Pt()); }
	     if( deltaRmujet<0.9 ) probeMu1DR0p9STpt30  +=PFJetPt->at(iJetR);
	     if( deltaRmujet<0.9 ) probeMu1DR0p9STpt30W +=(PFJetPt->at(iJetR)*mujetoverlapweight);
	     if( deltaRmujet<0.9 ) probeMu1DR0p9STpt30WC+=(CorrJetPt*mujetoverlapweight);
	     if( deltaRmujet<0.7 ) probeMu1DR0p7STpt30  +=PFJetPt->at(iJetR);
	     if( deltaRmujet<0.7 ) probeMu1DR0p7STpt30W +=(PFJetPt->at(iJetR)*mujetoverlapweight);
	     if( deltaRmujet<0.7 ) probeMu1DR0p7STpt30WC+=(CorrJetPt*mujetoverlapweight);
	     if( deltaRmujet<0.5 ) probeMu1DR0p5STpt30  +=PFJetPt->at(iJetR);
	     if( deltaRmujet<0.5 ) probeMu1DR0p5STpt30W +=(PFJetPt->at(iJetR)*mujetoverlapweight);
	     if( deltaRmujet<0.5 ) probeMu1DR0p5STpt30WC+=(CorrJetPt*mujetoverlapweight);
	   }
	   if( PFJetPt->at(iJetR)>40 ){	 
	     double CorrJetPt=PFJetPt->at(iJetR);
             //if( probeMuMatchedJetArray[0]!=999 ){
	     //pfmujet.SetPtEtaPhiM(PFJetPt->at(probeMuMatchedJetArray[0]), PFJetEta->at(probeMuMatchedJetArray[0]), PFJetPhi->at(probeMuMatchedJetArray[0]),0);
             //}
             //if( probeMuMatchedJetArray[0]==999 ){
	     //pfmujet=probemuon;
             //}
             if( deltaRmujet<0.2 ){ CorrJetPt=fabs((pfjet-probemuon).Pt()); }
	     if( deltaRmujet<0.9 ) probeMu1DR0p9STpt40  +=PFJetPt->at(iJetR);
	     if( deltaRmujet<0.9 ) probeMu1DR0p9STpt40W +=(PFJetPt->at(iJetR)*mujetoverlapweight);
	     if( deltaRmujet<0.9 ) probeMu1DR0p9STpt40WC+=(CorrJetPt*mujetoverlapweight);
	     if( deltaRmujet<0.7 ) probeMu1DR0p7STpt40  +=PFJetPt->at(iJetR);
	     if( deltaRmujet<0.7 ) probeMu1DR0p7STpt40W +=(PFJetPt->at(iJetR)*mujetoverlapweight);
	     if( deltaRmujet<0.7 ) probeMu1DR0p7STpt40WC+=(CorrJetPt*mujetoverlapweight);
	     if( deltaRmujet<0.5 ) probeMu1DR0p5STpt40  +=PFJetPt->at(iJetR);
	     if( deltaRmujet<0.5 ) probeMu1DR0p5STpt40W +=(PFJetPt->at(iJetR)*mujetoverlapweight);
	     if( deltaRmujet<0.5 ) probeMu1DR0p5STpt40WC+=(CorrJetPt*mujetoverlapweight);
	   }
	   //
	 }
       }
       //


       if( probeTauArray.size()>=1 ){
	 for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
	   //if( PFJetNeutralHadronEnergyFraction->at(iJetR)>=0.99 ) continue;
	   //if( PFJetNeutralEmEnergyFraction->at(iJetR)>=0.99     ) continue;
	   //if( PFJetNConstituents->at(iJetR)<2                   ) continue;
	   //if( fabs(PFJetEta->at(iJetR))<2.4 ){
	   //if( PFJetChargedHadronEnergyFraction->at(iJetR)<=0  ) continue;
	   //if( PFJetChargedMultiplicity->at(iJetR)<=0          ) continue;
	   //if( PFJetChargedEmEnergyFraction->at(iJetR)>=0.99   ) continue;
	   //}
	   if( fabs(PFJetEta->at(iJetR))>3 ) continue;
	   //
	   if( iJetR==probeTauMatchedJetArray[0] ) continue; //this vetoes the tau-jet for our probe tau: probeTauArray[0]. 
	   //
	   //
	   TLorentzVector probetau, pfjet, pftaujet;
	   probetau.SetPtEtaPhiM( HPSTauPt->at(probeTauArray[0]), HPSTauEta->at(probeTauArray[0]), HPSTauPhi->at(probeTauArray[0]), 0 );
	   pfjet.SetPtEtaPhiM( PFJetPt->at(iJetR), PFJetEta->at(iJetR), PFJetPhi->at(iJetR), 0 );
	   double deltaRtaujet=fabs(probetau.DeltaR(pfjet));
	   //For both radii=0.5:
	   //double mujetoverlapweight=(2.0*(0.25*TMath::ACos(deltaRmujet)-0.5*deltaRmujet*TMath::Sqrt(0.25-0.25*deltaRmujet*deltaRmujet)))/(TMath::Pi()*0.25);
	   //
	   // For 0.5 jet,  0.5 tau:
	   double taujetoverlapweight=0;
	   //if( deltaRtaujet>=0 && deltaRtaujet<=1.0 ){
	   if( deltaRtaujet>=0.2 && deltaRtaujet<=1.0 ){
	     double dj=(deltaRtaujet*deltaRtaujet+0.0)/(2.0*deltaRtaujet);//0.0=(0.5^2)-(0.5^2)   For muons:0.09 = (0.5^2)-(0.4^2)
	     double dt=(deltaRtaujet*deltaRtaujet-0.0)/(2.0*deltaRtaujet);
	     double Aj=0.5*0.5*TMath::ACos(dj/0.5)-dj*TMath::Sqrt(0.5*0.5-dj*dj);
	     double At=0.5*0.5*TMath::ACos(dt/0.5)-dt*TMath::Sqrt(0.5*0.5-dt*dt);
	     taujetoverlapweight=(Aj+At)/(TMath::Pi()*0.5*0.5);
	   }
	   if( deltaRtaujet<0.2 ){
	     taujetoverlapweight=0;
	   }
	   //
	   if( PFJetPt->at(iJetR)>20 ){	 
	     double CorrJetPt=PFJetPt->at(iJetR);
	     //if( probeTauMatchedJetArray[0]!=999 ){
	     //pftaujet.SetPtEtaPhiM(PFJetPt->at(probeTauMatchedJetArray[0]), PFJetEta->at(probeTauMatchedJetArray[0]), PFJetPhi->at(probeTauMatchedJetArray[0]),0);
	     //}
	     //if( probeTauMatchedJetArray[0]==999 ){
	     //pftaujet=probetau;
	     //}
             if( deltaRtaujet<0.2 ){ CorrJetPt=fabs((pfjet-probetau).Pt()); }//if tau is within the jet
	     if( deltaRtaujet<1.0 ) probeTau1DR1p0STpt20  +=PFJetPt->at(iJetR);
	     if( deltaRtaujet<1.0 ) probeTau1DR1p0STpt20W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
	     if( deltaRtaujet<1.0 ) probeTau1DR1p0STpt20WC+=(CorrJetPt*taujetoverlapweight);
	     if( deltaRtaujet<0.7 ) probeTau1DR0p7STpt20  +=PFJetPt->at(iJetR);
	     if( deltaRtaujet<0.7 ) probeTau1DR0p7STpt20W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
	     if( deltaRtaujet<0.7 ) probeTau1DR0p7STpt20WC+=(CorrJetPt*taujetoverlapweight);
	     if( deltaRtaujet<0.5 ) probeTau1DR0p5STpt20  +=PFJetPt->at(iJetR);
	     if( deltaRtaujet<0.5 ) probeTau1DR0p5STpt20W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
	     if( deltaRtaujet<0.5 ) probeTau1DR0p5STpt20WC+=(CorrJetPt*taujetoverlapweight);
	   }
	   if( PFJetPt->at(iJetR)>30 ){	 
	     double CorrJetPt=PFJetPt->at(iJetR);
	     //if( probeTauMatchedJetArray[0]!=999 ){
	     //pftaujet.SetPtEtaPhiM(PFJetPt->at(probeTauMatchedJetArray[0]), PFJetEta->at(probeTauMatchedJetArray[0]), PFJetPhi->at(probeTauMatchedJetArray[0]),0);
	     //}
	     //if( probeTauMatchedJetArray[0]==999 ){
	     //pftaujet=probetau;
	     //}
             if( deltaRtaujet<0.2 ){ CorrJetPt=fabs((pfjet-probetau).Pt()); }//if tau is within the jet
	     if( deltaRtaujet<1.0 ) probeTau1DR1p0STpt30  +=PFJetPt->at(iJetR);
	     if( deltaRtaujet<1.0 ) probeTau1DR1p0STpt30W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
	     if( deltaRtaujet<1.0 ) probeTau1DR1p0STpt30WC+=(CorrJetPt*taujetoverlapweight);
	     if( deltaRtaujet<0.7 ) probeTau1DR0p7STpt30  +=PFJetPt->at(iJetR);
	     if( deltaRtaujet<0.7 ) probeTau1DR0p7STpt30W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
	     if( deltaRtaujet<0.7 ) probeTau1DR0p7STpt30WC+=(CorrJetPt*taujetoverlapweight);
	     if( deltaRtaujet<0.5 ) probeTau1DR0p5STpt30  +=PFJetPt->at(iJetR);
	     if( deltaRtaujet<0.5 ) probeTau1DR0p5STpt30W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
	     if( deltaRtaujet<0.5 ) probeTau1DR0p5STpt30WC+=(CorrJetPt*taujetoverlapweight);
	   }
	   if( PFJetPt->at(iJetR)>40 ){	 
	     double CorrJetPt=PFJetPt->at(iJetR);
	     //if( probeTauMatchedJetArray[0]!=999 ){
	     //pftaujet.SetPtEtaPhiM(PFJetPt->at(probeTauMatchedJetArray[0]), PFJetEta->at(probeTauMatchedJetArray[0]), PFJetPhi->at(probeTauMatchedJetArray[0]),0);
	     //}
	     //if( probeTauMatchedJetArray[0]==999 ){
	     //pftaujet=probetau;
	     //}
             if( deltaRtaujet<0.2 ){ CorrJetPt=fabs((pfjet-probetau).Pt()); }//if tau is within the jet
	     if( deltaRtaujet<1.0 ) probeTau1DR1p0STpt40  +=PFJetPt->at(iJetR);
	     if( deltaRtaujet<1.0 ) probeTau1DR1p0STpt40W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
	     if( deltaRtaujet<1.0 ) probeTau1DR1p0STpt40WC+=(CorrJetPt*taujetoverlapweight);
	     if( deltaRtaujet<0.7 ) probeTau1DR0p7STpt40  +=PFJetPt->at(iJetR);
	     if( deltaRtaujet<0.7 ) probeTau1DR0p7STpt40W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
	     if( deltaRtaujet<0.7 ) probeTau1DR0p7STpt40WC+=(CorrJetPt*taujetoverlapweight);
	     if( deltaRtaujet<0.5 ) probeTau1DR0p5STpt40  +=PFJetPt->at(iJetR);
	     if( deltaRtaujet<0.5 ) probeTau1DR0p5STpt40W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
	     if( deltaRtaujet<0.5 ) probeTau1DR0p5STpt40WC+=(CorrJetPt*taujetoverlapweight);
	   }
	   //
	 }
       }
       //


       //
       for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
         if( !jetRisoCheck(iJetR)   )continue;
	 if( eventJet1Pt==-99 ){ eventJet1Pt=PFJetPt->at(iJetR); eventJet1Eta=PFJetEta->at(iJetR); eventJet1Phi=PFJetPhi->at(iJetR); continue; }
	 if( eventJet2Pt==-99 ){ eventJet2Pt=PFJetPt->at(iJetR); eventJet2Eta=PFJetEta->at(iJetR); eventJet2Phi=PFJetPhi->at(iJetR); break;    }
       }
       //
       //
       if( probeTauArray.size()>=1 ){
	 probeTau1Pt  = HPSTauPt ->at(probeTauArray[0]); probeTau1Eta = HPSTauEta->at(probeTauArray[0]); probeTau1Phi = HPSTauPhi->at(probeTauArray[0]);
	 if( HPSTauVLooseCombinedIsolationDeltaBetaCorrDiscr->at(probeTauArray[0])==1      ) probeTau1Iso=1;
	 if( HPSTauLooseCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[0])==1  ) probeTau1Iso=2;
	 if( HPSTauMediumCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[0])==1 ) probeTau1Iso=3;
	 if( HPSTauTightCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[0])==1  ) probeTau1Iso=4;
	 if( HPSTauAgainstElectronLooseMVA3Discr->at(probeTauArray[0])==1  )  probeTau1ElRej=1;
	 if( HPSTauAgainstElectronMediumMVA3Discr->at(probeTauArray[0])==1 )  probeTau1ElRej=2;
	 if( HPSTauAgainstElectronTightMVA3Discr->at(probeTauArray[0])==1  )  probeTau1ElRej=3;
	 if( HPSTauAgainstElectronVTightMVA3Discr->at(probeTauArray[0])==1 )  probeTau1ElRej=4;
	 probeTau1Q=HPSTauCharge->at(probeTauArray[0]);
	 probeTau1DM=HPSTauDecayMode->at(probeTauArray[0]);
	 probeTau1GenDeltaRTau = probeTauDeltaRminTauArray[0];
	 probeTau1GenDeltaRMu  = probeTauDeltaRminMuArray[0];
	 probeTau1GenDeltaREl  = probeTauDeltaRminElArray[0];
	 probeTau1JetDeltaRmin = probeTauJetDeltaRminArray[0];
	 probeTau1JetFlavor    = probeTauJetFlavorArray[0];
       }
       if( probeTauArray.size()>=2 ){
	 probeTau2Pt  = HPSTauPt ->at(probeTauArray[1]); probeTau2Eta = HPSTauEta->at(probeTauArray[1]); probeTau2Phi = HPSTauPhi->at(probeTauArray[1]);
	 if( HPSTauVLooseCombinedIsolationDeltaBetaCorrDiscr->at(probeTauArray[1])==1      ) probeTau2Iso=1;
	 if( HPSTauLooseCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[1])==1  ) probeTau2Iso=2;
	 if( HPSTauMediumCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[1])==1 ) probeTau2Iso=3;
	 if( HPSTauTightCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[1])==1  ) probeTau2Iso=4;
	 if( HPSTauAgainstElectronLooseMVA3Discr->at(probeTauArray[1])==1  )  probeTau2ElRej=1;
	 if( HPSTauAgainstElectronMediumMVA3Discr->at(probeTauArray[1])==1 )  probeTau2ElRej=2;
	 if( HPSTauAgainstElectronTightMVA3Discr->at(probeTauArray[1])==1  )  probeTau2ElRej=3;
	 if( HPSTauAgainstElectronVTightMVA3Discr->at(probeTauArray[1])==1 )  probeTau2ElRej=4;
	 probeTau2Q=HPSTauCharge->at(probeTauArray[1]);
	 probeTau2DM=HPSTauDecayMode->at(probeTauArray[1]);
	 probeTau2GenDeltaRTau = probeTauDeltaRminTauArray[1];
	 probeTau2GenDeltaRMu  = probeTauDeltaRminMuArray[1];
	 probeTau2GenDeltaREl  = probeTauDeltaRminElArray[1];
	 probeTau2JetDeltaRmin = probeTauJetDeltaRminArray[1];
	 probeTau2JetFlavor    = probeTauJetFlavorArray[1];
       }
       //
       //
       if( probeMuArray.size()>=1 ){
	 probeMu1Pt  = MuonPt ->at(probeMuArray[0]); probeMu1Eta = MuonEta->at(probeMuArray[0]); probeMu1Phi = MuonPhi->at(probeMuArray[0]);
	 probeMu1Q=MuonCharge->at(probeMuArray[0]);
	 probeMu1GenDeltaRTau = probeMuDeltaRminTauArray[0];
	 probeMu1GenDeltaRMu  = probeMuDeltaRminMuArray[0];
	 probeMu1GenDeltaREl  = probeMuDeltaRminElArray[0];
	 if( RecoHLTdeltaRmin_SingleMuTrigger(probeMuArray[0])<0.15 ) probeMu1HLT=1;
	 else probeMu1HLT  =  0;
	 probeMu1PFIso     = MuonPFIsoR04ChargedHadron->at(probeMuArray[0]) +
	   TMath::Max( 0.0 , (MuonPFIsoR04NeutralHadron->at(probeMuArray[0])+MuonPFIsoR04Photon->at(probeMuArray[0])-0.5*MuonPFIsoR04PU->at(probeMuArray[0])) );
	 //
	 probeMu1JetDeltaRmin=probeMuJetDeltaRminArray[0];
	 probeMu1JetFlavor=probeMuJetFlavorArray[0];
       }
       if( probeMuArray.size()>=2 ){
	 probeMu2Pt  = MuonPt ->at(probeMuArray[1]); probeMu2Eta = MuonEta->at(probeMuArray[1]); probeMu2Phi = MuonPhi->at(probeMuArray[1]);
	 probeMu2Q=MuonCharge->at(probeMuArray[1]);
	 probeMu2GenDeltaRTau = probeMuDeltaRminTauArray[1];
	 probeMu2GenDeltaRMu  = probeMuDeltaRminMuArray[1];
	 probeMu2GenDeltaREl  = probeMuDeltaRminElArray[1];
	 if( RecoHLTdeltaRmin_SingleMuTrigger(probeMuArray[1])<0.15 ) probeMu2HLT=1;
	 else probeMu2HLT  =  0;
	 probeMu2PFIso     = MuonPFIsoR04ChargedHadron->at(probeMuArray[1]) +
	   TMath::Max( 0.0 , (MuonPFIsoR04NeutralHadron->at(probeMuArray[1])+MuonPFIsoR04Photon->at(probeMuArray[1])-0.5*MuonPFIsoR04PU->at(probeMuArray[1])) );
	 //
	 probeMu2JetDeltaRmin=probeMuJetDeltaRminArray[1];
	 probeMu2JetFlavor=probeMuJetFlavorArray[1];
       }
       //
       //
       eventST       = ST();
       eventSJT      = LooseJetHT;
       eventMET      = PFMETType01XYCor->at(0);
       eventMETphi   = PFMETPhiType01XYCor->at(0);
       eventJetN     = JetCounter();
       eventrhoJets     = rhoJets;
       eventrhoForHEEP  = rhoForHEEP;
       eventrhoJetsCCPU = rhoJetsCCPU;
       eventrhoJetsCN   = rhoJetsCN;
       eventrhoJetsCNT  = rhoJetsCNT;
       //
       //removeOverlaps=true;
       eventIsoJetN  = JetCounter();
       eventIsoST    = ST();
       //removeOverlaps=false;
       //
       eventPU       = (double)(VertexX->size());
       eventPUWeight = safePileupWeights_;

       fillVariableWithValue("probeMu1Pt",   probeMu1Pt);
       fillVariableWithValue("probeMu1Eta",  probeMu1Eta);
       fillVariableWithValue("probeMu1Phi",  probeMu1Phi);
       fillVariableWithValue("probeMu1Q",    probeMu1Q);
       fillVariableWithValue("probeMu1HLT",  probeMu1HLT);
       fillVariableWithValue("probeMu1PFIso",    probeMu1PFIso);
       fillVariableWithValue("probeMu1JetDeltaRmin",probeMu1JetDeltaRmin);
       fillVariableWithValue("probeMu1JetFlavor",probeMu1JetFlavor);
       //
       fillVariableWithValue("probeMu1DR0p9STpt40",   probeMu1DR0p9STpt40);
       fillVariableWithValue("probeMu1DR0p9STpt40W",  probeMu1DR0p9STpt40W);
       fillVariableWithValue("probeMu1DR0p9STpt40WC", probeMu1DR0p9STpt40WC);
       fillVariableWithValue("probeMu1DR0p9STpt30",   probeMu1DR0p9STpt30);
       fillVariableWithValue("probeMu1DR0p9STpt30W",  probeMu1DR0p9STpt30W);
       fillVariableWithValue("probeMu1DR0p9STpt30WC", probeMu1DR0p9STpt30WC);
       fillVariableWithValue("probeMu1DR0p9STpt20",   probeMu1DR0p9STpt20);
       fillVariableWithValue("probeMu1DR0p9STpt20W",  probeMu1DR0p9STpt20W);
       fillVariableWithValue("probeMu1DR0p9STpt20WC", probeMu1DR0p9STpt20WC);
       //
       fillVariableWithValue("probeMu1DR0p7STpt40",   probeMu1DR0p7STpt40);
       fillVariableWithValue("probeMu1DR0p7STpt40W",  probeMu1DR0p7STpt40W);
       fillVariableWithValue("probeMu1DR0p7STpt40WC", probeMu1DR0p7STpt40WC);
       fillVariableWithValue("probeMu1DR0p7STpt30",   probeMu1DR0p7STpt30);
       fillVariableWithValue("probeMu1DR0p7STpt30W",  probeMu1DR0p7STpt30W);
       fillVariableWithValue("probeMu1DR0p7STpt30WC", probeMu1DR0p7STpt30WC);
       fillVariableWithValue("probeMu1DR0p7STpt20",   probeMu1DR0p7STpt20);
       fillVariableWithValue("probeMu1DR0p7STpt20W",  probeMu1DR0p7STpt20W);
       fillVariableWithValue("probeMu1DR0p7STpt20WC", probeMu1DR0p7STpt20WC);
       //
       fillVariableWithValue("probeMu1DR0p5STpt40",   probeMu1DR0p5STpt40);
       fillVariableWithValue("probeMu1DR0p5STpt40W",  probeMu1DR0p5STpt40W);
       fillVariableWithValue("probeMu1DR0p5STpt40WC", probeMu1DR0p5STpt40WC);
       fillVariableWithValue("probeMu1DR0p5STpt30",   probeMu1DR0p5STpt30);
       fillVariableWithValue("probeMu1DR0p5STpt30W",  probeMu1DR0p5STpt30W);
       fillVariableWithValue("probeMu1DR0p5STpt30WC", probeMu1DR0p5STpt30WC);
       fillVariableWithValue("probeMu1DR0p5STpt20",   probeMu1DR0p5STpt20);
       fillVariableWithValue("probeMu1DR0p5STpt20W",  probeMu1DR0p5STpt20W);
       fillVariableWithValue("probeMu1DR0p5STpt20WC", probeMu1DR0p5STpt20WC);
       //
       //
       fillVariableWithValue("probeTau1DR1p0STpt40",   probeTau1DR1p0STpt40);
       fillVariableWithValue("probeTau1DR1p0STpt40W",  probeTau1DR1p0STpt40W);
       fillVariableWithValue("probeTau1DR1p0STpt40WC", probeTau1DR1p0STpt40WC);
       fillVariableWithValue("probeTau1DR1p0STpt30",   probeTau1DR1p0STpt30);
       fillVariableWithValue("probeTau1DR1p0STpt30W",  probeTau1DR1p0STpt30W);
       fillVariableWithValue("probeTau1DR1p0STpt30WC", probeTau1DR1p0STpt30WC);
       fillVariableWithValue("probeTau1DR1p0STpt20",   probeTau1DR1p0STpt20);
       fillVariableWithValue("probeTau1DR1p0STpt20W",  probeTau1DR1p0STpt20W);
       fillVariableWithValue("probeTau1DR1p0STpt20WC", probeTau1DR1p0STpt20WC);
       //
       fillVariableWithValue("probeTau1DR0p7STpt40",   probeTau1DR0p7STpt40);
       fillVariableWithValue("probeTau1DR0p7STpt40W",  probeTau1DR0p7STpt40W);
       fillVariableWithValue("probeTau1DR0p7STpt40WC", probeTau1DR0p7STpt40WC);
       fillVariableWithValue("probeTau1DR0p7STpt30",   probeTau1DR0p7STpt30);
       fillVariableWithValue("probeTau1DR0p7STpt30W",  probeTau1DR0p7STpt30W);
       fillVariableWithValue("probeTau1DR0p7STpt30WC", probeTau1DR0p7STpt30WC);
       fillVariableWithValue("probeTau1DR0p7STpt20",   probeTau1DR0p7STpt20);
       fillVariableWithValue("probeTau1DR0p7STpt20W",  probeTau1DR0p7STpt20W);
       fillVariableWithValue("probeTau1DR0p7STpt20WC", probeTau1DR0p7STpt20WC);
       //
       fillVariableWithValue("probeTau1DR0p5STpt40",   probeTau1DR0p5STpt40);
       fillVariableWithValue("probeTau1DR0p5STpt40W",  probeTau1DR0p5STpt40W);
       fillVariableWithValue("probeTau1DR0p5STpt40WC", probeTau1DR0p5STpt40WC);
       fillVariableWithValue("probeTau1DR0p5STpt30",   probeTau1DR0p5STpt30);
       fillVariableWithValue("probeTau1DR0p5STpt30W",  probeTau1DR0p5STpt30W);
       fillVariableWithValue("probeTau1DR0p5STpt30WC", probeTau1DR0p5STpt30WC);
       fillVariableWithValue("probeTau1DR0p5STpt20",   probeTau1DR0p5STpt20);
       fillVariableWithValue("probeTau1DR0p5STpt20W",  probeTau1DR0p5STpt20W);
       fillVariableWithValue("probeTau1DR0p5STpt20WC", probeTau1DR0p5STpt20WC);
       //
       //
       fillVariableWithValue("probeMu1GenDeltaRTau",probeMu1GenDeltaRTau);
       fillVariableWithValue("probeMu1GenDeltaRMu", probeMu1GenDeltaRMu );
       fillVariableWithValue("probeMu1GenDeltaREl", probeMu1GenDeltaREl );
       fillVariableWithValue("probeMu2Pt",   probeMu2Pt);
       fillVariableWithValue("probeMu2Eta",  probeMu2Eta);
       fillVariableWithValue("probeMu2Phi",  probeMu2Phi);
       fillVariableWithValue("probeMu2Q",    probeMu2Q);
       fillVariableWithValue("probeMu2HLT",  probeMu2HLT);
       fillVariableWithValue("probeMu2PFIso",    probeMu2PFIso);
       fillVariableWithValue("probeMu2GenDeltaRTau",probeMu2GenDeltaRTau);
       fillVariableWithValue("probeMu2GenDeltaRMu", probeMu2GenDeltaRMu );
       fillVariableWithValue("probeMu2GenDeltaREl", probeMu2GenDeltaREl );
       fillVariableWithValue("probeMu2JetDeltaRmin",probeMu2JetDeltaRmin);
       fillVariableWithValue("probeMu2JetFlavor",probeMu2JetFlavor);
       //
       //
       fillVariableWithValue("probeTau1Pt",   probeTau1Pt);
       fillVariableWithValue("probeTau1Eta",  probeTau1Eta);
       fillVariableWithValue("probeTau1Phi",  probeTau1Phi);
       fillVariableWithValue("probeTau1DecayMode",   probeTau1DM);
       fillVariableWithValue("probeTau1Q",    probeTau1Q);
       fillVariableWithValue("probeTau1GenDeltaRTau",probeTau1GenDeltaRTau);
       fillVariableWithValue("probeTau1GenDeltaRMu", probeTau1GenDeltaRMu );
       fillVariableWithValue("probeTau1GenDeltaREl", probeTau1GenDeltaREl );
       fillVariableWithValue("probeTau1JetDeltaRmin",probeTau1JetDeltaRmin);
       fillVariableWithValue("probeTau1JetFlavor",probeTau1JetFlavor);
       fillVariableWithValue("probeTau1Iso",  probeTau1Iso);
       fillVariableWithValue("probeTau1ElRej",probeTau1ElRej);
       fillVariableWithValue("probeTau2Pt",   probeTau2Pt);
       fillVariableWithValue("probeTau2Eta",  probeTau2Eta);
       fillVariableWithValue("probeTau2Phi",  probeTau2Phi);
       fillVariableWithValue("probeTau2DecayMode",   probeTau2DM);
       fillVariableWithValue("probeTau2Q",    probeTau2Q);
       fillVariableWithValue("probeTau2GenDeltaRTau",probeTau2GenDeltaRTau);
       fillVariableWithValue("probeTau2GenDeltaRMu", probeTau2GenDeltaRMu );
       fillVariableWithValue("probeTau2GenDeltaREl", probeTau2GenDeltaREl );
       fillVariableWithValue("probeTau2JetDeltaRmin",probeTau2JetDeltaRmin);
       fillVariableWithValue("probeTau2JetFlavor",probeTau2JetFlavor);
       fillVariableWithValue("probeTau2Iso",  probeTau2Iso);
       fillVariableWithValue("probeTau2ElRej",probeTau2ElRej);
       //
       //
       fillVariableWithValue("eventData",eventData);
       fillVariableWithValue("eventST",eventST);
       fillVariableWithValue("eventIsoST",eventIsoST);
       fillVariableWithValue("eventSJT",eventSJT);
       fillVariableWithValue("eventrhoJets",eventrhoJets);
       fillVariableWithValue("eventrhoForHEEP",eventrhoForHEEP);
       fillVariableWithValue("eventrhoJetsCCPU",eventrhoJetsCCPU);
       fillVariableWithValue("eventrhoJetsCN",eventrhoJetsCN);
       fillVariableWithValue("eventrhoJetsCNT",eventrhoJetsCNT);
       fillVariableWithValue("eventMET",eventMET);
       fillVariableWithValue("eventMETphi",eventMETphi);
       fillVariableWithValue("eventJet1Pt", eventJet1Pt);
       fillVariableWithValue("eventJet1Eta",eventJet1Eta);
       fillVariableWithValue("eventJet1Phi",eventJet1Phi);
       fillVariableWithValue("eventJet2Pt", eventJet2Pt);
       fillVariableWithValue("eventJet2Eta",eventJet2Eta);
       fillVariableWithValue("eventJet2Phi",eventJet2Phi);
       fillVariableWithValue("eventJetN",eventJetN);
       fillVariableWithValue("eventIsoJetN",eventIsoJetN);
       fillVariableWithValue("eventTauN",eventTauN);
       fillVariableWithValue("eventMuN", eventMuN);
       fillVariableWithValue("eventPU",eventPU);
       fillVariableWithValue("eventPUWeight",eventPUWeight);

       fillReducedSkimTree();
     }
     //---------------------------------------------------------------------------------

     ////////////////////// User's code ends here ///////////////////////
   } // End loop over events

   //////////write histos 
   //
   ProvidedPileUpWeightshisto->Write();
   AppliedPileUpWeightshisto->Write();

   //

   //////////
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
// // //
