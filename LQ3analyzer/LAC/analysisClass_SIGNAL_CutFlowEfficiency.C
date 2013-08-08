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
   TH1D* signalTypeHisto = new TH1D("signalTypeHisto","signalTypeHisto",4,0.5,4.5);
   signalTypeHisto->GetXaxis()->SetBinLabel(1,"All");
   signalTypeHisto->GetXaxis()->SetBinLabel(2,"Tau");
   signalTypeHisto->GetXaxis()->SetBinLabel(3,"Mu+Tau");
   signalTypeHisto->GetXaxis()->SetBinLabel(4,"Mu+Tau (SS)");
   TH1D* signalTypeRatioHisto = new TH1D("signalTypeRatioHisto","signalTypeRatioHisto",4,0.5,4.5);
   signalTypeRatioHisto->GetXaxis()->SetBinLabel(1,"All");
   signalTypeRatioHisto->GetXaxis()->SetBinLabel(2,"Tau");
   signalTypeRatioHisto->GetXaxis()->SetBinLabel(3,"Mu+Tau");
   signalTypeRatioHisto->GetXaxis()->SetBinLabel(4,"Mu+Tau (SS)");
   //
   TH1D* cutFlowHisto = new TH1D("cutFlowHisto","cutFlowHisto",8,0.5,8.5);
   cutFlowHisto->GetXaxis()->SetBinLabel(1,"All");
   cutFlowHisto->GetXaxis()->SetBinLabel(2,"Event Filter");
   cutFlowHisto->GetXaxis()->SetBinLabel(3,"IsoMu Trig");
   cutFlowHisto->GetXaxis()->SetBinLabel(4,"Mu+Tau Loose (OS+SS)");
   cutFlowHisto->GetXaxis()->SetBinLabel(5,"ST>300");
   cutFlowHisto->GetXaxis()->SetBinLabel(6,"Mu+Tau Loose (SS)");
   cutFlowHisto->GetXaxis()->SetBinLabel(7,"Mu Trig Match");
   cutFlowHisto->GetXaxis()->SetBinLabel(8,"Mu+Tau Tight (SS)");
   TH1D* cutFlowRatioHisto = new TH1D("cutFlowRatioHisto","cutFlowRatioHisto",8,0.5,8.5);
   cutFlowRatioHisto->GetXaxis()->SetBinLabel(1,"All");
   cutFlowRatioHisto->GetXaxis()->SetBinLabel(2,"Event Filter");
   cutFlowRatioHisto->GetXaxis()->SetBinLabel(3,"IsoMu Trig");
   cutFlowRatioHisto->GetXaxis()->SetBinLabel(4,"Mu+Tau Loose (OS+SS)");
   cutFlowRatioHisto->GetXaxis()->SetBinLabel(5,"ST>300");
   cutFlowRatioHisto->GetXaxis()->SetBinLabel(6,"Mu+Tau Loose (SS)");
   cutFlowRatioHisto->GetXaxis()->SetBinLabel(7,"Mu Trig Match");
   cutFlowRatioHisto->GetXaxis()->SetBinLabel(8,"Mu+Tau Tight (SS)");
   //

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

     ltemMuTau.clear();
     double w=safePileupWeights();

     // -------- -------- -------- -------- -------- -------- -------- -------- --------
     // GEN CHECK
     int NGenTau=0;
     int NGenMu=0;
     std::vector<int> GenTauCharge;
     std::vector<int> GenMuCharge;
     for( unsigned int iTauT=0; iTauT<GenLQTopTauPt->size(); iTauT++){
       if( GenLQTopTauTauDecayMode->at(iTauT)<3 ) continue;//skip leptonic taus
       NGenTau++;
       if( GenLQTopTauPdgId->at(iTauT)== 15 ) GenTauCharge.push_back(-1);
       if( GenLQTopTauPdgId->at(iTauT)==-15 ) GenTauCharge.push_back( 1);
     }
     for( unsigned int iTauT=0; iTauT<GenLQTauTauPt->size(); iTauT++){
       if( GenLQTauTauTauDecayMode->at(iTauT)<3 ) continue;//skip leptonic tau
       NGenTau++;
       if( GenLQTauTauPdgId->at(iTauT)== 15 ) GenTauCharge.push_back(-1);
       if( GenLQTauTauPdgId->at(iTauT)==-15 ) GenTauCharge.push_back( 1);
     }
     for( unsigned int iMuT=0; iMuT<GenLQTauMuonPt->size(); iMuT++){
       if( GenLQTauMuonPdgId->at(iMuT)== 13 ) GenMuCharge.push_back(-1);
       if( GenLQTauMuonPdgId->at(iMuT)==-13 ) GenMuCharge.push_back( 1);
     }
     for( unsigned int iMuT=0; iMuT<GenLQTopMuonPt->size(); iMuT++){
       if( GenLQTopMuonPdgId->at(iMuT)== 13 ) GenMuCharge.push_back(-1);
       if( GenLQTopMuonPdgId->at(iMuT)==-13 ) GenMuCharge.push_back( 1);
     }
     NGenMu+=GenLQTauMuonPt->size();
     NGenMu+=GenLQTopMuonPt->size();
     //
     bool isSS_=false;
     for( unsigned int imu=0; imu<GenMuCharge.size(); imu++ ){
       for( unsigned int itau=0; itau<GenTauCharge.size(); itau++ ){
	 if( GenMuCharge[imu]==GenTauCharge[itau] ) isSS_=true;
       } 
     }

     //
     // ALL
     signalTypeHisto->Fill(1,w);
     //
     // HADRONIC TAU
     if( NGenTau>0 ) signalTypeHisto->Fill(2,w);
     //
     // MUON
     if( NGenTau>0 && NGenMu>0 ) signalTypeHisto->Fill(3,w);
     //
     // SameSign MuTau
     if( NGenTau>0 && NGenMu>0 && isSS_ ) signalTypeHisto->Fill(4,w);
     //
     //
     // -------- -------- -------- -------- -------- -------- -------- -------- --------

     if( !isSS_ ) continue;
     //if(  isSS_ ) continue;

     // ALL EVENTS -- Continuing from above!
     cutFlowHisto->Fill(1,w);
     //
     // EVENT FILTER
     if( passEventFilter() ) cutFlowHisto->Fill(2,w);
     else continue;
     //
     // ISO MU TRIGGER
     if( SingleMu_passTrigger()>0 ) cutFlowHisto->Fill(3,w);
     else continue;
     //
     // MU-TAU PAIR
     if( isMuTauDR0p30() ) cutFlowHisto->Fill(4,w);
     else continue;
     //
     // ST CUT
     if( ST()>300 ) cutFlowHisto->Fill(5,w);
     else continue;
     //
     // SS MU-TAU PAIR for LTEM - LOOSE
     which_MuTau(ltemMuTau);
     if( ltemMuTau.size()==2 ) cutFlowHisto->Fill(6,w);
     else continue;
     //
     // MU-HLT MATCHING
     if( RecoHLTdeltaRmin_SingleMuTrigger(ltemMuTau[0])<0.15 ) cutFlowHisto->Fill(7,w);
     else continue;
     //
     // SS MU-TAU PAIR for LTEM - TIGHT
     if( muRTightCheck(ltemMuTau[0]) && tauRTightCheck(ltemMuTau[1]) ) cutFlowHisto->Fill(8,w);
     else continue;
     //
     //
     cout<<endl;
     for(unsigned int iMuR=0;  iMuR<MuonPt->size();     iMuR++){
       if( !muRisoCheck(iMuR) ) continue;
       cout<<"Muon ("<<muRTightCheck(iMuR)<<"): "<<MuonCharge->at(iMuR)<<" "<<MuonPt->at(iMuR)<<" "<<MuonEta->at(iMuR)<<" "<<MuonPhi->at(iMuR)<<endl;
     }
     for(unsigned int iTauR=0;  iTauR<HPSTauPt->size();     iTauR++){
       if( !tauRisoCheck(iTauR) ) continue;
       cout<<" Tau ("<<HPSTauVLooseCombinedIsolationDeltaBetaCorrDiscr->at(iTauR)<<"/"<<tauRTightCheck(iTauR)<<"): "<<HPSTauCharge->at(iTauR)<<" "<<HPSTauPt->at(iTauR)<<" "<<HPSTauEta->at(iTauR)<<" "<<HPSTauPhi->at(iTauR)<<endl;
     }
     cout<< "LTEM Muon ("<<muRTightCheck(ltemMuTau[0])<<"): "<<MuonCharge->at(ltemMuTau[0])<<" "<<MuonPt->at(ltemMuTau[0])<<" "<<MuonEta->at(ltemMuTau[0])<<" "<<MuonPhi->at(ltemMuTau[0])<<endl;
     cout<< "LTEM  Tau ("<<tauRTightCheck(ltemMuTau[1])<<"): "<<HPSTauCharge->at(ltemMuTau[1])<<" "<<HPSTauPt->at(ltemMuTau[1])<<" "<<HPSTauEta->at(ltemMuTau[1])<<" "<<HPSTauPhi->at(ltemMuTau[1])<<endl;


     //---------------------------------------------------------------------------------
     ////////////////////// User's code ends here ///////////////////////
   } // End loop over events

   cout<<endl;
   cout<<endl;
   for( int ibin=1; ibin<5; ibin++){
     std::cout<<signalTypeHisto->GetXaxis()->GetBinLabel(ibin)<<" : "<<std::setprecision(4)<<signalTypeHisto->GetBinContent(ibin)/signalTypeHisto->GetBinContent(1)*100.<<std::endl;
     signalTypeRatioHisto->SetBinContent(ibin,(signalTypeHisto->GetBinContent(ibin)/signalTypeHisto->GetBinContent(1)));
   }
   cout<<endl;
   cout<<endl;
   //
   cout<<endl;
   cout<<endl;
   for( int ibin=1; ibin<9; ibin++){
     std::cout<<cutFlowHisto->GetXaxis()->GetBinLabel(ibin)<<" : "<<std::setprecision(4)<<cutFlowHisto->GetBinContent(ibin)/cutFlowHisto->GetBinContent(1)*100.<<std::endl;
     cutFlowRatioHisto->SetBinContent(ibin,(cutFlowHisto->GetBinContent(ibin)/cutFlowHisto->GetBinContent(1)));
   }
   cout<<endl;
   cout<<endl;

   //////////write histos 
   //
   cutFlowHisto->Write();
   cutFlowRatioHisto->Write();
   signalTypeHisto->Write();
   signalTypeRatioHisto->Write();
   //

   //////////
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
// // //
