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
   BTagDef = "CSVL";
   idDef   = "CUT";
   MuonPtCut = 20;
   MuonEtaCut= 2.4;
   TauPtCut  = 20;
   ApplyAbsMuonIsolation=false;
   removeOverlaps=false;

   //////////book histos here
   TH1D* ProvidedPileUpWeightshisto  = new TH1D("ProvidedPileUpWeightshisto","ProvidedPileUpWeightshisto",10000,0,100);//distribution of the provided PileUp-weights
   TH1D* Triggerhisto                = new TH1D("Triggerhisto",              "Triggerhisto",              17,-5.5,11.5);//distribution of triggers in the events
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
     //ltemMuMu.clear();
     //ltemMuTau.clear();


     //  -- JSON SKIM
     int passJSON_=0;
     if( isData  ) passJSON_ = passJSON(run,ls,isData);
     if( !isData ) passJSON_ = 1;
     fillVariableWithValue("PassJSON", passJSON_ );//..............// returns 0, 1                                                                                              

     //  -- TRIGGER SKIM
     int passTrigger_=0;
     //passTrigger_ = Jet_passTrigger();
     passTrigger_=1;
     Triggerhisto->Fill( passTrigger_ );
     fillVariableWithValue("PassTrig", passTrigger_ );//...// returns -2, -1, 0, trig                                                                                   


     //  -- EVENT FILTER SKIM
     int passEventFilter_=0;
     if( passEventFilter() ) passEventFilter_ = 1;
     fillVariableWithValue("PassEventFilter", passEventFilter_ );//.// returns 0, 1 


     //  -- LEPTON+JET SKIM - One or more LI Muons or LI Taus
     int isOfflineLepton_=0;
     int CountLooseLeptons=0;
     for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
       if( muRLooseCheck(iMuR) ) CountLooseLeptons+=1;
     }
     //for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
     //if( tauRLooseCheck(iTauR) ) CountLooseLeptons+=1;
     //}
     double LooseJetHT=0;
     double AllJetHT=0;
     for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
       //if( jetRLooseCheck(iJetR) && PFJetPt->at(iJetR)>80 ) CountLooseJets+=1;
       if( jetRLooseCheck(iJetR) ) LooseJetHT+=PFJetPt->at(iJetR);
       //AllJetHT+=PFJetPt->at(iJetR);
     }
     //if( CountLooseLeptons>0 && CountLooseJets>3 ) isOfflineLepton_=1;
     if( CountLooseLeptons>0 && LooseJetHT>650 ) isOfflineLepton_=1;
     fillVariableWithValue("PassOfflineLepton", isOfflineLepton_ );// Returns 0, 1  


     evaluateCuts();
     //---------------------------------------------------------------------------------
     if( passedCut("PassJSON") && passedCut("PassTrig") && passedCut("PassEventFilter") && passedCut("PassOfflineLepton") ){
       fillSkimTree();
     }
     //---------------------------------------------------------------------------------


     ////////////////////// User's code ends here ///////////////////////
   } // End loop over events


   //////////write histos 
   //
   ProvidedPileUpWeightshisto->Write();
   Triggerhisto->Write();
   //

   //////////
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
// // //
