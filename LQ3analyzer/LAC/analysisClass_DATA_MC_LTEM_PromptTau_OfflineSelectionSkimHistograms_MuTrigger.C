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
   //MuonPtCut = 20;
   //MuonEtaCut= 2.4;
   MuonPtCut = 25;// <<< changes needed, store more Muon iso information!
   MuonEtaCut= 2.1;
   TauPtCut  = 20;
   ApplyAbsMuonIsolation=false;
   //removeOverlaps=true;
   removeOverlaps=false;
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
   TH1D* AppliedTrigEffWeightshisto  = new TH1D("AppliedTrigEffWeightshisto","AppliedTrigEffWeightshisto",10000,0,100);//distribution of the applied TrigEff-weights
   TH1D* Triggerhisto                = new TH1D("Triggerhisto",              "Triggerhisto",              10,-3.5,6.5);//distribution of triggers in the events
   TH1D* AppliedTotalWeightshisto    = new TH1D("AppliedTotalWeightshisto",  "AppliedTotalWeightshisto",  10000,0,100);//distribution of the applied Total-weights
   //
   TH1D* LeadMuTauDeltaRhisto = new TH1D("LeadMuTauDeltaRhisto","LeadMuTauDeltaRhisto",5000,0,50);
   TH1D* SThisto              = new TH1D("SThisto", "SThisto", 5000,0,5000);
   TH1D* STwFakeshisto        = new TH1D("STwFakeshisto", "STwFakeshisto", 5000,0,5000);//fake
   TH1D* STwRealshisto        = new TH1D("STwRealshisto", "STwRealshisto", 5000,0,5000);//fake
   TH1D* STotherhisto         = new TH1D("STotherhisto", "STotherhisto", 5000,0,5000);//fake
   TH1D* FakesDistrhisto      = new TH1D("FakesDistrhisto", "FakesDistrhisto",7,0.25,3.75);//fake
   TH1D* SJThisto             = new TH1D("SJThisto","SJThisto",5000,0,5000);
   TH1D* METhisto             = new TH1D("METhisto","METhisto",2000,0,2000);
   //
   TH1D* MaxDiLepMasshisto     = new TH1D("MaxDiLepMasshisto",    "MaxDiLepMasshisto",    2000,0,2000);
   TH1D* AllMuMuInvMasshisto   = new TH1D("AllMuMuInvMasshisto",  "AllMuMuInvMasshisto",  2000,0,2000);
   TH1D* AllMuTauInvMasshisto  = new TH1D("AllMuTauInvMasshisto", "AllMuTauInvMasshisto", 2000,0,2000);
   TH1D* MaxMuTauInvMasshisto  = new TH1D("MaxMuTauInvMasshisto","MaxMuTauInvMasshisto",  2000,0,2000);
   TH1D* AllTauTauInvMasshisto = new TH1D("AllTauTauInvMasshisto","AllTauTauInvMasshisto",2000,0,2000);
   //
   TH1D* METmuTransMasshisto  = new TH1D("METmuTransMasshisto", "METmuTransMasshisto",500,0,1000);
   TH1D* METtauTransMasshisto = new TH1D("METtauTransMasshisto","METtauTransMasshisto",500,0,1000);
   //
   TH1D* METtauDeltaPhihisto = new TH1D("METtauDeltaPhihisto","METtauDeltaPhihisto",800,-4,4);
   TH1D* METmuDeltaPhihisto  = new TH1D("METmuDeltaPhihisto", "METmuDeltaPhihisto", 800,-4,4);
   TH1D* METelDeltaPhihisto  = new TH1D("METelDeltaPhihisto", "METelDeltaPhihisto", 800,-4,4);
   //
   TH1D* LeadMuPthisto    = new TH1D("LeadMuPthisto",   "LeadMuPthisto",   1000,0,1000);
   TH1D* LeadTauPthisto   = new TH1D("LeadTauPthisto",  "LeadTauPthisto",  1000,0,1000);
   TH1D* LeadElPthisto    = new TH1D("LeadElPthisto",   "LeadElPthisto",   1000,0,1000);
   TH1D* LeadJetPthisto   = new TH1D("LeadJetPthisto",  "LeadJetPthisto",  1000,0,1000);
   TH1D* LeadBJetPthisto  = new TH1D("LeadBJetPthisto", "LeadBJetPthisto", 1000,0,1000);
   //
   TH1D* LeadMuEtahisto   = new TH1D("LeadMuEtahisto",  "LeadMuEtahisto",  500,-2.5,2.5);
   TH1D* LeadTauEtahisto  = new TH1D("LeadTauEtahisto", "LeadTauEtahisto", 500,-2.5,2.5);
   TH1D* LeadElEtahisto   = new TH1D("LeadElEtahisto",  "LeadElEtahisto",  500,-2.5,2.5);
   TH1D* LeadJetEtahisto  = new TH1D("LeadJetEtahisto", "LeadJetEtahisto", 700,-3.5,3.5);
   TH1D* LeadBJetEtahisto = new TH1D("LeadBJetEtahisto","LeadBJetEtahisto",700,-3.5,3.5);
   //
   TH1D* LeadMuPhihisto   = new TH1D("LeadMuPhihisto",  "LeadMuPhihisto",  800,-4,4);
   TH1D* LeadTauPhihisto  = new TH1D("LeadTauPhihisto", "LeadTauPhihisto", 800,-4,4);
   TH1D* LeadElPhihisto   = new TH1D("LeadElPhihisto",  "LeadElPhihisto",  800,-4,4);
   TH1D* LeadJetPhihisto  = new TH1D("LeadJetPhihisto", "LeadJetPhihisto", 800,-4,4);
   TH1D* LeadBJetPhihisto = new TH1D("LeadBJetPhihisto","LeadBJetPhihisto",800,-4,4);
   //
   TH1D* MuPthisto    = new TH1D("MuPthisto",   "MuPthisto",   200, 0,1000);
   TH1D* TauPthisto   = new TH1D("TauPthisto",  "TauPthisto",  200, 0,1000);
   TH1D* ElPthisto    = new TH1D("ElPthisto",   "ElPthisto",   200, 0,1000);
   TH1D* JetPthisto   = new TH1D("JetPthisto",  "JetPthisto",  200, 0,1000);
   TH1D* BJetPthisto  = new TH1D("BJetPthisto", "BJetPthisto", 200, 0,1000);
   //
   TH1D* MuEtahisto   = new TH1D("MuEtahisto",  "MuEtahisto",  500,-2.5,2.5);
   TH1D* TauEtahisto  = new TH1D("TauEtahisto", "TauEtahisto", 500,-2.5,2.5);
   TH1D* ElEtahisto   = new TH1D("ElEtahisto",  "ElEtahisto",  500,-2.5,2.5);
   TH1D* JetEtahisto  = new TH1D("JetEtahisto", "JetEtahisto", 700,-3.5,3.5);
   TH1D* BJetEtahisto = new TH1D("BJetEtahisto","BJetEtahisto",700,-3.5,3.5);
   //
   TH1D* MuPhihisto   = new TH1D("MuPhihisto",  "MuPhihisto",  800,-4,4);
   TH1D* TauPhihisto  = new TH1D("TauPhihisto", "TauPhihisto", 800,-4,4);
   TH1D* ElPhihisto   = new TH1D("ElPhihisto",  "ElPhihisto",  800,-4,4);
   TH1D* JetPhihisto  = new TH1D("JetPhihisto", "JetPhihisto", 800,-4,4);
   TH1D* BJetPhihisto = new TH1D("BJetPhihisto","BJetPhihisto",800,-4,4);
   //
   // from here on, it used to be TH1I
   TH1D* MuNhisto       = new TH1D("MuNhisto",      "MuNhisto",      16,-0.5,15.5);
   TH1D* TauNhisto      = new TH1D("TauNhisto",     "TauNhisto",     16,-0.5,15.5);
   TH1D* ElNhisto       = new TH1D("ElNhisto",      "ElNhisto",      16,-0.5,15.5);
   TH1D* JetNhisto      = new TH1D("JetNhisto",     "JetNhisto",     26,-0.5,25.5);
   TH1D* BJetNhisto     = new TH1D("BJetNhisto",    "BJetNhisto",    16,-0.5,15.5);
   TH1D* TotalNhisto    = new TH1D("TotalNhisto",   "TotalNhisto",   41,-0.5,40.5);
   TH1D* VertexNhisto   = new TH1D("VertexNhisto",  "VertexNhisto",  61,-0.5,60.5);
   TH1D* VertexNV2histo = new TH1D("VertexNV2histo","VertexNV2histo",61,-0.5,60.5);
   //
   TH1D* RecoSignalTypehisto         = new TH1D("RecoSignalTypehisto",        "RecoSignalTypehisto",        5,-2.5,2.5);
   TH1D* MuBasedRecoSignalTypehisto  = new TH1D("MuBasedRecoSignalTypehisto", "MuBasedRecoSignalTypehisto", 9,-4.5,4.5);
   TH1D* TauBasedRecoSignalTypehisto = new TH1D("TauBasedRecoSignalTypehisto","TauBasedRecoSignalTypehisto",9,-4.5,4.5);
   //
   TH2D* LooseToTightMatrixRAW2Dhisto = new TH2D("LooseToTightMatrixRAW2Dhisto","LooseToTightMatrixRAW2Dhisto", 2,0.5,2.5, 2,0.5,2.5);
   TH2D* LooseToTightMatrix2Dhisto    = new TH2D("LooseToTightMatrix2Dhisto",   "LooseToTightMatrix2Dhisto",    2,0.5,2.5, 2,0.5,2.5);
   //
   TH1D* LooseTauPthisto  = new TH1D("LooseTauPthisto", "LooseTauPthisto", 2000,0,2000);
   TH1D* TightTauPthisto  = new TH1D("TightTauPthisto", "TightTauPthisto", 2000,0,2000);
   TH1D* LooseTauEtahisto = new TH1D("LooseTauEtahisto","LooseTauEtahisto",500,-2.5,2.5);
   TH1D* TightTauEtahisto = new TH1D("TightTauEtahisto","TightTauEtahisto",500,-2.5,2.5);
   TH1D* LooseTauPhihisto = new TH1D("LooseTauPhihisto","LooseTauPhihisto",700,-3.5,3.5);
   TH1D* TightTauPhihisto = new TH1D("TightTauPhihisto","TightTauPhihisto",700,-3.5,3.5);
   TGraphAsymmErrors* LooseTightRatioPtTauhisto  = new TGraphAsymmErrors(200);
   TGraphAsymmErrors* LooseTightRatioEtaTauhisto = new TGraphAsymmErrors(50);
   TGraphAsymmErrors* LooseTightRatioPhiTauhisto = new TGraphAsymmErrors(70);
   //
   TH1D* LooseFakeTauPthisto  = new TH1D("LooseFakeTauPthisto", "LooseFakeTauPthisto", 2000,0,2000);
   TH1D* TightFakeTauPthisto  = new TH1D("TightFakeTauPthisto", "TightFakeTauPthisto", 2000,0,2000);
   TH1D* LooseFakeTauEtahisto = new TH1D("LooseFakeTauEtahisto","LooseFakeTauEtahisto",500,-2.5,2.5);
   TH1D* TightFakeTauEtahisto = new TH1D("TightFakeTauEtahisto","TightFakeTauEtahisto",500,-2.5,2.5);
   TH1D* LooseFakeTauPhihisto = new TH1D("LooseFakeTauPhihisto","LooseFakeTauPhihisto",700,-3.5,3.5);
   TH1D* TightFakeTauPhihisto = new TH1D("TightFakeTauPhihisto","TightFakeTauPhihisto",700,-3.5,3.5);
   //
   TH1D* LoosePromptTauPthisto  = new TH1D("LoosePromptTauPthisto", "LoosePromptTauPthisto", 2000,0,2000);
   TH1D* TightPromptTauPthisto  = new TH1D("TightPromptTauPthisto", "TightPromptTauPthisto", 2000,0,2000);
   TH1D* LoosePromptTauEtahisto = new TH1D("LoosePromptTauEtahisto","LoosePromptTauEtahisto",500,-2.5,2.5);
   TH1D* TightPromptTauEtahisto = new TH1D("TightPromptTauEtahisto","TightPromptTauEtahisto",500,-2.5,2.5);
   TH1D* LoosePromptTauPhihisto = new TH1D("LoosePromptTauPhihisto","LoosePromptTauPhihisto",700,-3.5,3.5);
   TH1D* TightPromptTauPhihisto = new TH1D("TightPromptTauPhihisto","TightPromptTauPhihisto",700,-3.5,3.5);

   /////////initialize variables
   TLorentzVector allmu;
   TLorentzVector alltau;
   TLorentzVector allmu2;
   TLorentzVector alltau2;
   //
   int MuN;
   int TauN;
   int ElN;
   int JetN;
   int BJetN;
   int VertexN;
   double SJT;
   TLorentzVector Mu;
   TLorentzVector Tau;
   TLorentzVector El;
   TLorentzVector Jet;
   TLorentzVector BJet;
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
     ltemMuMu.clear();
     ltemMuTau.clear();

     unsigned int probeTau=99;
     unsigned int trigMuon=99;
     std::vector<unsigned int> probeTauArray;
     std::vector<unsigned int> probeTauMatchedJetArray;
     std::vector<double>       probeTauMuDeltaRArray;
     std::vector<double>       probeTauIsFakeArray;
     std::vector<double>       probeTauDeltaRminTauArray;
     std::vector<double>       probeTauDeltaRminMuArray;
     std::vector<double>       probeTauDeltaRminElArray;

     // -- WEIGHTS - see below

     //  -- JSON SKIM
     int passJSON_=0;
     if( isData  ) passJSON_ = passJSON(run,ls,isData);
     if( !isData ) passJSON_ = 1;
     fillVariableWithValue("PassJSON", passJSON_ );//..............// returns 0, 1                                                                                              
     //  -- TRIGGER SKIM
     int passTrigger_=0;
     passTrigger_ = SingleMu_passTrigger();
     fillVariableWithValue("PassTrig", passTrigger_ );//...// returns -2, -1, 0, trig                                                                                   
     //  -- EVENT FILTER SKIM
     int passEventFilter_=0;
     if( passEventFilter() ) passEventFilter_ = 1;
     fillVariableWithValue("PassEventFilter", passEventFilter_ );//.// returns 0, 1 

     //  -- LEPTON SKIM - Exactly 1 LI Muon & 1 LI Tau, no charge requirement, dR>0.9
     int isOfflineLepton_=0;
     int NMuons=0;
     int LooseTaus=0;
     bool isMuTight_=false;
     //
     TLorentzVector Muon;
     TLorentzVector Tau;
     for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
       if( !muRLooseCheck(iMuR) ) continue;
       Muon.SetPtEtaPhiM( MuonPt->at(iMuR),  MuonEta->at(iMuR),  MuonPhi->at(iMuR), 0 );
       NMuons++;
       trigMuon=iMuR;
       //if( muRTightCheck(iMuR) && MuonPt->at(iMuR)>25 && fabs(MuonEta->at(iMuR))<2.1 ) isMuTight_=true;
       if( MuonPt->at(iMuR)>25 && fabs(MuonEta->at(iMuR))<2.1 ) isMuTight_=true;//disable TightIso requirement
     }
     for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
       if( !tauRLooseCheck(iTauR) || NMuons!=1 ) continue;
       Tau.SetPtEtaPhiM( HPSTauPt->at(iTauR),  HPSTauEta->at(iTauR),  HPSTauPhi->at(iTauR), 0 );
       if( fabs(Muon.DeltaR(Tau))<0.5 ) continue;
       probeTauMuDeltaRArray.push_back( fabs(Muon.DeltaR(Tau)) );
       probeTauArray.push_back( iTauR );
       if( isData ){
	 probeTauDeltaRminTauArray.push_back(9999);
	 probeTauDeltaRminMuArray.push_back(9999);
	 probeTauDeltaRminElArray.push_back(9999);
	 probeTauIsFakeArray.push_back(-99);
       }
       if( probeTau==99 ) probeTau=iTauR;
       LooseTaus++;
       //
       // Tau-Jet Matching
       TLorentzVector taucand, jetcand;
       double tauJetDeltaRmin=9999.0;
       unsigned int iTauJetR=999;
       taucand.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
       //
       for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
         jetcand.SetPtEtaPhiM( PFJetPt->at(iJetR),  PFJetEta->at(iJetR),  PFJetPhi->at(iJetR),  0 );
         if( taucand.DeltaR(jetcand)<tauJetDeltaRmin ){ tauJetDeltaRmin=taucand.DeltaR(jetcand); iTauJetR=iJetR; }//picks the smallest deltaR and JET
       }
       if( tauJetDeltaRmin>=0.2 ) iTauJetR=999;//checks if nearest JET is near enough!
       probeTauMatchedJetArray.push_back(iTauJetR);
     }


     if( debug_ && NMuons==1 &&  LooseTaus>0 ){
       cout<<"("<<jentry<<")   TrigMu: "<<MuonPt->at(trigMuon) <<" "<<MuonEta->at(trigMuon) <<" "<<MuonPhi->at(trigMuon)<<endl;
       cout<<"("<<jentry<<") ProbeTau: "<<HPSTauPt->at(probeTau)<<" "<<HPSTauEta->at(probeTau)<<" "<<HPSTauPhi->at(probeTau)<<endl;
     }

     if( NMuons==1 && isMuTight_ && LooseTaus>0 ) isOfflineLepton_=1;
     fillVariableWithValue("PassOfflineLepton", isOfflineLepton_ );// Returns 0, 1


     // -- HLT MATCHING SKIM
     int passHLTmatching_=0;
     if( trigMuon!=99 && RecoHLTdeltaRmin_SingleMuTrigger( trigMuon )<0.15 ) passHLTmatching_=1;
     fillVariableWithValue("PassHLTmatching",passHLTmatching_);

     double LooseJetHT=0;
     for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
       if( !jetRisoCheck(iJetR)   )continue;
       LooseJetHT+=PFJetPt->at(iJetR);
     }

     // -- OFFLINE SKIM
     int OfflineCuts_=0;
     if( ElCounter()==0  
	 && JetCounter()>=1 
	 //&& LooseJetHT>250
	 //&& PFMETType1Cor->at(0)<50
	 ) OfflineCuts_=1;
     fillVariableWithValue("PassOfflineCuts", OfflineCuts_ );// returns 0, 1 

     evaluateCuts();
     //---------------------------------------------------------------------------------
     if( passedCut("PassJSON") && passedCut("PassTrig") && passedCut("PassEventFilter") && passedCut("PassOfflineLepton") &&
	 passedCut("PassHLTmatching") && passedCut("PassOfflineCuts") ){ 


       // Reduced Skim variables  -  probe quantities
       double tagPt;     double tagEta;     double tagPhi; double tagQ;        
       double tagR04ChargedHadron;       double tagR04NeutralHadron;       double tagR04Photon;       double tagR04PU;       double tagPFIso;
       double probe1Pt;  double probe1Eta;  double probe1Phi; double probe1Fake; double probe1Iso; double probe1Q; double probe1DM; double probe1MuDeltaR;
       double probe2Pt;  double probe2Eta;  double probe2Phi; double probe2Fake; double probe2Iso; double probe2Q; double probe2DM; double probe2MuDeltaR;
       double probe3Pt;  double probe3Eta;  double probe3Phi; double probe3Fake; double probe3Iso; double probe3Q; double probe3DM; double probe3MuDeltaR;
       double probe4Pt;  double probe4Eta;  double probe4Phi; double probe4Fake; double probe4Iso; double probe4Q; double probe4DM; double probe4MuDeltaR;
       double probe1GenDeltaRTau;  double probe1GenDeltaRMu;  double probe1GenDeltaREl; double probe1ElRej;
       double probe2GenDeltaRTau;  double probe2GenDeltaRMu;  double probe2GenDeltaREl; double probe2ElRej;
       double probe3GenDeltaRTau;  double probe3GenDeltaRMu;  double probe3GenDeltaREl; double probe3ElRej;
       double probe4GenDeltaRTau;  double probe4GenDeltaRMu;  double probe4GenDeltaREl; double probe4ElRej;
       double eventJet1Pt;  double eventJet1Eta;  double eventJet1Phi;
       double eventJet2Pt;  double eventJet2Eta;  double eventJet2Phi;
       double eventData;
       double eventST;
       double eventIsoST;
       double eventSJT;
       double eventMET;
       double eventMETphi;
       double eventJetN;
       double eventIsoJetN;
       double eventTauN;
       double eventPU;
       double eventPUWeight;
       //
       double  probe1DR1p0STpt20; double probe1DR1p0STpt20W; double probe1DR1p0STpt20WC;
       double  probe1DR1p0STpt30; double probe1DR1p0STpt30W; double probe1DR1p0STpt30WC;
       double  probe1DR1p0STpt40; double probe1DR1p0STpt40W; double probe1DR1p0STpt40WC;
       double  probe1DR0p7STpt20; double probe1DR0p7STpt20W; double probe1DR0p7STpt20WC;
       double  probe1DR0p7STpt30; double probe1DR0p7STpt30W; double probe1DR0p7STpt30WC;
       double  probe1DR0p7STpt40; double probe1DR0p7STpt40W; double probe1DR0p7STpt40WC;
       double  probe1DR0p5STpt20; double probe1DR0p5STpt20W; double probe1DR0p5STpt20WC;
       double  probe1DR0p5STpt30; double probe1DR0p5STpt30W; double probe1DR0p5STpt30WC;
       double  probe1DR0p5STpt40; double probe1DR0p5STpt40W; double probe1DR0p5STpt40WC;


       // WEIGHTS
       // ---- pileup
       double safePileupWeights_=0;
       safePileupWeights_=safePileupWeights();
       AppliedPileUpWeightshisto->Fill( safePileupWeights_ );
       // ---- trigger
       int usedTrigger_=-5;
       double TriggerEfficiencyWeights_=1;
       usedTrigger_ = SingleMu_passTrigger();
       Triggerhisto->Fill( usedTrigger_ );
       if( !isData ) TriggerEfficiencyWeights_=IsoMu24e2p1_Eff( MuonPt->at(trigMuon), MuonEta->at(trigMuon) );
       AppliedTrigEffWeightshisto->Fill( TriggerEfficiencyWeights_ );
       // ---- total = pileup x trigger
       double w = 0;
       w=safePileupWeights_*TriggerEfficiencyWeights_;
       AppliedTotalWeightshisto->Fill( w );
       //
       
       // Probe Tau
       LooseTauPthisto->Fill(  HPSTauPt ->at(probeTau), w );
       LooseTauEtahisto->Fill( HPSTauEta->at(probeTau), w );
       LooseTauPhihisto->Fill( HPSTauPhi->at(probeTau), w );
       if( tauRTightCheck(probeTau) ){
	 TightTauPthisto->Fill(  HPSTauPt ->at(probeTau), w );
	 TightTauEtahisto->Fill( HPSTauEta->at(probeTau), w );
	 TightTauPhihisto->Fill( HPSTauPhi->at(probeTau), w );
       }

       // is probeTau Fake ?
       if( !isData ){
	 for( unsigned int iTauR=0; iTauR<probeTauArray.size(); iTauR++){
	   bool isProbeTauFake_=true;
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
	     if( fabs(DeltaRminTau)<0.15 ) isProbeTauFake_=false;
	   }
	   for( unsigned int iTauT=0; iTauT<GenZTauPt->size(); iTauT++){
	     if( GenZTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
	     taugen.SetPtEtaPhiM( GenZTauPt->at(iTauT),     GenZTauEta->at(iTauT),   GenZTauPhi->at(iTauT), 0 );
	     if( GenZTauPdgId->at(iTauT)>0 ) charge=-1;
	     if( GenZTauPdgId->at(iTauT)<0 ) charge=+1;
	     if( fabs(taureco.DeltaR(taugen))<fabs(DeltaRminTau) ) DeltaRminTau=fabs(taureco.DeltaR(taugen))*charge;
	     if( fabs(DeltaRminTau)<0.15 ) isProbeTauFake_=false;
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
	   if(  isProbeTauFake_ ) probeTauIsFakeArray.push_back(1);
	   if( !isProbeTauFake_ ) probeTauIsFakeArray.push_back(0);
	   //--------------------------------------------------------------
	   // Fake Tau
	   if( isProbeTauFake_ ){
	     LooseFakeTauPthisto->Fill(  HPSTauPt ->at(probeTau), w );
	     LooseFakeTauEtahisto->Fill( HPSTauEta->at(probeTau), w );
	     LooseFakeTauPhihisto->Fill( HPSTauPhi->at(probeTau), w );
	     if( tauRTightCheck(probeTau) ){
	       TightFakeTauPthisto->Fill(  HPSTauPt ->at(probeTau), w );
	       TightFakeTauEtahisto->Fill( HPSTauEta->at(probeTau), w );
	       TightFakeTauPhihisto->Fill( HPSTauPhi->at(probeTau), w );
	     }
	   }
	   // Prompt Tau
	   if( !isProbeTauFake_ ){
	     LoosePromptTauPthisto->Fill(  HPSTauPt ->at(probeTau), w );
	     LoosePromptTauEtahisto->Fill( HPSTauEta->at(probeTau), w );
	     LoosePromptTauPhihisto->Fill( HPSTauPhi->at(probeTau), w );
	     if( tauRTightCheck(probeTau) ){
	       TightPromptTauPthisto->Fill(  HPSTauPt ->at(probeTau), w );
	       TightPromptTauEtahisto->Fill( HPSTauEta->at(probeTau), w );
	       TightPromptTauPhihisto->Fill( HPSTauPhi->at(probeTau), w );
	     }
	   }
	 }
       }
       //--------------------------------------------------------------
       //

       if(  isData ) eventData=1; 
       else eventData=0;
       //
       tagQ   = MuonCharge->at(trigMuon);
       tagPt  = MuonPt ->at(trigMuon);
       tagEta = MuonEta->at(trigMuon);
       tagPhi = MuonPhi->at(trigMuon);
       tagR04ChargedHadron = MuonPFIsoR04ChargedHadron->at(trigMuon);        
       tagR04NeutralHadron = MuonPFIsoR04NeutralHadron->at(trigMuon);
       tagR04Photon        = MuonPFIsoR04Photon->at(trigMuon);
       tagR04PU            = MuonPFIsoR04PU->at(trigMuon);
       tagPFIso            = tagR04ChargedHadron+TMath::Max( 0.0 ,tagR04NeutralHadron+tagR04Photon-0.5*tagR04PU);

       eventTauN=LooseTaus;
       //
       eventJet1Pt=-99; eventJet1Eta=-99; eventJet1Phi=-99;
       eventJet2Pt=-99; eventJet2Eta=-99; eventJet2Phi=-99;
       probe1Pt=-99; probe1Eta=-99; probe1Phi=-99; probe1Fake=-99; probe1Iso=-99; probe1Q=-99; probe1DM=-99; probe1MuDeltaR=-99; probe1ElRej=-99;
       probe2Pt=-99; probe2Eta=-99; probe2Phi=-99; probe2Fake=-99; probe2Iso=-99; probe2Q=-99; probe2DM=-99; probe2MuDeltaR=-99; probe2ElRej=-99;
       probe3Pt=-99; probe3Eta=-99; probe3Phi=-99; probe3Fake=-99; probe3Iso=-99; probe3Q=-99; probe3DM=-99; probe3MuDeltaR=-99; probe3ElRej=-99;
       probe4Pt=-99; probe4Eta=-99; probe4Phi=-99; probe4Fake=-99; probe4Iso=-99; probe4Q=-99; probe4DM=-99; probe4MuDeltaR=-99; probe4ElRej=-99;
       probe1GenDeltaRTau=-99;  probe1GenDeltaRMu=-99;  probe1GenDeltaREl=-99;
       probe2GenDeltaRTau=-99;  probe2GenDeltaRMu=-99;  probe2GenDeltaREl=-99;
       probe3GenDeltaRTau=-99;  probe3GenDeltaRMu=-99;  probe3GenDeltaREl=-99;
       probe4GenDeltaRTau=-99;  probe4GenDeltaRMu=-99;  probe4GenDeltaREl=-99;
       //
       probe1DR1p0STpt20W=0;  probe1DR1p0STpt20WC=0; probe1DR1p0STpt20=0;
       probe1DR1p0STpt30W=0;  probe1DR1p0STpt30WC=0; probe1DR1p0STpt30=0;
       probe1DR1p0STpt40W=0;  probe1DR1p0STpt40WC=0; probe1DR1p0STpt40=0;
       probe1DR0p7STpt20W=0;  probe1DR0p7STpt20WC=0; probe1DR0p7STpt20=0;
       probe1DR0p7STpt30W=0;  probe1DR0p7STpt30WC=0; probe1DR0p7STpt30=0;
       probe1DR0p7STpt40W=0;  probe1DR0p7STpt40WC=0; probe1DR0p7STpt40=0;
       probe1DR0p5STpt20W=0;  probe1DR0p5STpt20WC=0; probe1DR0p5STpt20=0;
       probe1DR0p5STpt30W=0;  probe1DR0p5STpt30WC=0; probe1DR0p5STpt30=0;
       probe1DR0p5STpt40W=0;  probe1DR0p5STpt40WC=0; probe1DR0p5STpt40=0;
       //
       //
       for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
         if( !jetRLooseCheck(iJetR)   )continue;
	 if( eventJet1Pt==-99 ){ eventJet1Pt=PFJetPt->at(iJetR); eventJet1Eta=PFJetEta->at(iJetR); eventJet1Phi=PFJetPhi->at(iJetR); continue; }
	 if( eventJet2Pt==-99 ){ eventJet2Pt=PFJetPt->at(iJetR); eventJet2Eta=PFJetEta->at(iJetR); eventJet2Phi=PFJetPhi->at(iJetR); break;    }
       }
       //
       if( probeTauArray.size()>=1 ){
	 for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
	   if( fabs(PFJetEta->at(iJetR))>3       ) continue;
	   if( iJetR==probeTauMatchedJetArray[0] ) continue; //this vetoes the tau-jet for our probe tau: probeTauArray[0]
	   //
           TLorentzVector probetau, pfjet, pftaujet;
           probetau.SetPtEtaPhiM( HPSTauPt->at(probeTauArray[0]), HPSTauEta->at(probeTauArray[0]), HPSTauPhi->at(probeTauArray[0]), 0 );
           pfjet.SetPtEtaPhiM( PFJetPt->at(iJetR), PFJetEta->at(iJetR), PFJetPhi->at(iJetR), 0 );
           double deltaRtaujet=fabs(probetau.DeltaR(pfjet));
	   //
           // For 0.5 jet,  0.5 tau:
           double taujetoverlapweight=0;
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
             if( deltaRtaujet<0.2 ){ CorrJetPt=fabs((pfjet-probetau).Pt()); }//if tau is within the jet
             if( deltaRtaujet<1.0 ) probe1DR1p0STpt20  +=PFJetPt->at(iJetR);
             if( deltaRtaujet<1.0 ) probe1DR1p0STpt20W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
             if( deltaRtaujet<1.0 ) probe1DR1p0STpt20WC+=(CorrJetPt*taujetoverlapweight);
             if( deltaRtaujet<0.7 ) probe1DR0p7STpt20  +=PFJetPt->at(iJetR);
             if( deltaRtaujet<0.7 ) probe1DR0p7STpt20W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
             if( deltaRtaujet<0.7 ) probe1DR0p7STpt20WC+=(CorrJetPt*taujetoverlapweight);
             if( deltaRtaujet<0.5 ) probe1DR0p5STpt20  +=PFJetPt->at(iJetR);
             if( deltaRtaujet<0.5 ) probe1DR0p5STpt20W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
             if( deltaRtaujet<0.5 ) probe1DR0p5STpt20WC+=(CorrJetPt*taujetoverlapweight);
           }
	   //
           if( PFJetPt->at(iJetR)>30 ){
             double CorrJetPt=PFJetPt->at(iJetR);
             if( deltaRtaujet<0.2 ){ CorrJetPt=fabs((pfjet-probetau).Pt()); }//if tau is within the jet
             if( deltaRtaujet<1.0 ) probe1DR1p0STpt30  +=PFJetPt->at(iJetR);
             if( deltaRtaujet<1.0 ) probe1DR1p0STpt30W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
             if( deltaRtaujet<1.0 ) probe1DR1p0STpt30WC+=(CorrJetPt*taujetoverlapweight);
             if( deltaRtaujet<0.7 ) probe1DR0p7STpt30  +=PFJetPt->at(iJetR);
             if( deltaRtaujet<0.7 ) probe1DR0p7STpt30W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
             if( deltaRtaujet<0.7 ) probe1DR0p7STpt30WC+=(CorrJetPt*taujetoverlapweight);
             if( deltaRtaujet<0.5 ) probe1DR0p5STpt30  +=PFJetPt->at(iJetR);
             if( deltaRtaujet<0.5 ) probe1DR0p5STpt30W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
             if( deltaRtaujet<0.5 ) probe1DR0p5STpt30WC+=(CorrJetPt*taujetoverlapweight);
           }
	   //
           if( PFJetPt->at(iJetR)>40 ){
             double CorrJetPt=PFJetPt->at(iJetR);
             if( deltaRtaujet<0.2 ){ CorrJetPt=fabs((pfjet-probetau).Pt()); }//if tau is within the jet
             if( deltaRtaujet<1.0 ) probe1DR1p0STpt40  +=PFJetPt->at(iJetR);
             if( deltaRtaujet<1.0 ) probe1DR1p0STpt40W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
             if( deltaRtaujet<1.0 ) probe1DR1p0STpt40WC+=(CorrJetPt*taujetoverlapweight);
             if( deltaRtaujet<0.7 ) probe1DR0p7STpt40  +=PFJetPt->at(iJetR);
             if( deltaRtaujet<0.7 ) probe1DR0p7STpt40W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
             if( deltaRtaujet<0.7 ) probe1DR0p7STpt40WC+=(CorrJetPt*taujetoverlapweight);
             if( deltaRtaujet<0.5 ) probe1DR0p5STpt40  +=PFJetPt->at(iJetR);
             if( deltaRtaujet<0.5 ) probe1DR0p5STpt40W +=(PFJetPt->at(iJetR)*taujetoverlapweight);
             if( deltaRtaujet<0.5 ) probe1DR0p5STpt40WC+=(CorrJetPt*taujetoverlapweight);
           }
	 }
	 //
	 probe1Pt  = HPSTauPt ->at(probeTauArray[0]); probe1Eta = HPSTauEta->at(probeTauArray[0]); probe1Phi = HPSTauPhi->at(probeTauArray[0]);
	 //if(  tauRTightCheck(probeTauArray[0]) ) probe1Tight=1;
	 // else probe1Tight=0;
	 if( HPSTauVLooseCombinedIsolationDeltaBetaCorrDiscr->at(probeTauArray[0])==1      ) probe1Iso=1;
	 if( HPSTauLooseCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[0])==1  ) probe1Iso=2;
	 if( HPSTauMediumCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[0])==1 ) probe1Iso=3;
	 if( HPSTauTightCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[0])==1  ) probe1Iso=4;
         if( HPSTauAgainstElectronLooseMVA3Discr->at(probeTauArray[0])==1  )  probe1ElRej=1;
         if( HPSTauAgainstElectronMediumMVA3Discr->at(probeTauArray[0])==1 )  probe1ElRej=2;
         if( HPSTauAgainstElectronTightMVA3Discr->at(probeTauArray[0])==1  )  probe1ElRej=3;
         if( HPSTauAgainstElectronVTightMVA3Discr->at(probeTauArray[0])==1 )  probe1ElRej=4;
	 probe1Q=HPSTauCharge->at(probeTauArray[0]);
	 probe1DM=HPSTauDecayMode->at(probeTauArray[0]);
	 probe1Fake=probeTauIsFakeArray[0];
	 probe1GenDeltaRTau = probeTauDeltaRminTauArray[0];
	 probe1GenDeltaRMu  = probeTauDeltaRminMuArray[0];
	 probe1GenDeltaREl  = probeTauDeltaRminElArray[0];
	 probe1MuDeltaR     = probeTauMuDeltaRArray[0];
       }
       if( probeTauArray.size()>=2 ){
	 probe2Pt  = HPSTauPt ->at(probeTauArray[1]); probe2Eta = HPSTauEta->at(probeTauArray[1]); probe2Phi = HPSTauPhi->at(probeTauArray[1]);
	 //if(  tauRTightCheck(probeTauArray[1]) ) probe2Tight=1;
         //else probe2Tight=0;
	 if( HPSTauVLooseCombinedIsolationDeltaBetaCorrDiscr->at(probeTauArray[1])==1      ) probe2Iso=1;
	 if( HPSTauLooseCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[1])==1  ) probe2Iso=2;
	 if( HPSTauMediumCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[1])==1 ) probe2Iso=3;
	 if( HPSTauTightCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[1])==1  ) probe2Iso=4;
         if( HPSTauAgainstElectronLooseMVA3Discr->at(probeTauArray[1])==1  )  probe2ElRej=1;
         if( HPSTauAgainstElectronMediumMVA3Discr->at(probeTauArray[1])==1 )  probe2ElRej=2;
         if( HPSTauAgainstElectronTightMVA3Discr->at(probeTauArray[1])==1  )  probe2ElRej=3;
         if( HPSTauAgainstElectronVTightMVA3Discr->at(probeTauArray[1])==1 )  probe2ElRej=4;
	 probe2Q=HPSTauCharge->at(probeTauArray[1]);
	 probe2DM=HPSTauDecayMode->at(probeTauArray[1]);
	 probe2Fake=probeTauIsFakeArray[1];
	 probe2GenDeltaRTau = probeTauDeltaRminTauArray[1];
	 probe2GenDeltaRMu  = probeTauDeltaRminMuArray[1];
	 probe2GenDeltaREl  = probeTauDeltaRminElArray[1];
	 probe2MuDeltaR     = probeTauMuDeltaRArray[1];
       }
       if( probeTauArray.size()>=3 ){
	 probe3Pt  = HPSTauPt ->at(probeTauArray[2]); probe3Eta = HPSTauEta->at(probeTauArray[2]); probe3Phi = HPSTauPhi->at(probeTauArray[2]);
	 //if(  tauRTightCheck(probeTauArray[2]) ) probe3Tight=1;
         //else probe3Tight=0;
	 if( HPSTauVLooseCombinedIsolationDeltaBetaCorrDiscr->at(probeTauArray[2])==1      ) probe3Iso=1;
	 if( HPSTauLooseCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[2])==1  ) probe3Iso=2;
	 if( HPSTauMediumCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[2])==1 ) probe3Iso=3;
	 if( HPSTauTightCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[2])==1  ) probe3Iso=4;
         if( HPSTauAgainstElectronLooseMVA3Discr->at(probeTauArray[2])==1  )  probe3ElRej=1;
         if( HPSTauAgainstElectronMediumMVA3Discr->at(probeTauArray[2])==1 )  probe3ElRej=2;
         if( HPSTauAgainstElectronTightMVA3Discr->at(probeTauArray[2])==1  )  probe3ElRej=3;
         if( HPSTauAgainstElectronVTightMVA3Discr->at(probeTauArray[2])==1 )  probe3ElRej=4;
	 probe3Q=HPSTauCharge->at(probeTauArray[2]);
	 probe3DM=HPSTauDecayMode->at(probeTauArray[2]);
	 probe3Fake=probeTauIsFakeArray[2];
	 probe3GenDeltaRTau = probeTauDeltaRminTauArray[2];
	 probe3GenDeltaRMu  = probeTauDeltaRminMuArray[2];
	 probe3GenDeltaREl  = probeTauDeltaRminElArray[2];
	 probe3MuDeltaR     = probeTauMuDeltaRArray[2];
       }
       if( probeTauArray.size()>=4 ){
	 probe4Pt  = HPSTauPt ->at(probeTauArray[3]); probe4Eta = HPSTauEta->at(probeTauArray[3]); probe4Phi = HPSTauPhi->at(probeTauArray[3]);
	 //if(  tauRTightCheck(probeTauArray[3]) ) probe4Tight=1;
         //else probe4Tight=0;
	 if( HPSTauVLooseCombinedIsolationDeltaBetaCorrDiscr->at(probeTauArray[3])==1      ) probe4Iso=1;
	 if( HPSTauLooseCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[3])==1  ) probe4Iso=2;
	 if( HPSTauMediumCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[3])==1 ) probe4Iso=3;
	 if( HPSTauTightCombinedIsolationDeltaBetaCorr3HitsDiscr->at(probeTauArray[3])==1  ) probe4Iso=4;
         if( HPSTauAgainstElectronLooseMVA3Discr->at(probeTauArray[3])==1  )  probe4ElRej=1;
         if( HPSTauAgainstElectronMediumMVA3Discr->at(probeTauArray[3])==1 )  probe4ElRej=2;
         if( HPSTauAgainstElectronTightMVA3Discr->at(probeTauArray[3])==1  )  probe4ElRej=3;
         if( HPSTauAgainstElectronVTightMVA3Discr->at(probeTauArray[3])==1 )  probe4ElRej=4;
	 probe4Q=HPSTauCharge->at(probeTauArray[3]);
	 probe4DM=HPSTauDecayMode->at(probeTauArray[3]);
	 probe4Fake=probeTauIsFakeArray[3];
	 probe4GenDeltaRTau = probeTauDeltaRminTauArray[3];
	 probe4GenDeltaRMu  = probeTauDeltaRminMuArray[3];
	 probe4GenDeltaREl  = probeTauDeltaRminElArray[3];
	 probe4MuDeltaR     = probeTauMuDeltaRArray[3];
       }
       //
       eventSJT=LooseJetHT;
       eventMET=PFMETType01XYCor->at(0);
       eventMETphi=PFMETPhiType01XYCor->at(0);
       eventST=ST();
       eventJetN=JetCounter();
       //overlap
       removeOverlaps=true;
       eventIsoST=ST();
       eventIsoJetN=JetCounter();
       removeOverlaps=false;
       //
       eventPU=(double)VertexX->size();
       eventPUWeight=w;
       //
       //
       
       //
       //


       // >>>>>>>>>>>>>>>>>>>>>> Histograms - multiple times per event. 
       allmu.SetPtEtaPhiM( 0, 0, 0, 0 );
       alltau.SetPtEtaPhiM( 0, 0, 0, 0 );
       allmu2.SetPtEtaPhiM( 0, 0, 0, 0 );
       alltau2.SetPtEtaPhiM( 0, 0, 0, 0 );
       //
       for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
	 if(!muRisoCheck(iMuR))continue;
	 //
	 METmuTransMasshisto->Fill( METlepMT("Mu",iMuR),       w );
	 METmuDeltaPhihisto ->Fill( METlepDeltaPhi("Mu",iMuR), w );
	 //
	 allmu.SetPtEtaPhiM( MuonPt->at(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0);
	 for(unsigned int iMuR2=(iMuR+1); iMuR2<MuonPt->size(); iMuR2++){
	   if(!muRisoCheck(iMuR2))continue;
	   allmu2.SetPtEtaPhiM( MuonPt->at(iMuR2), MuonEta->at(iMuR2), MuonPhi->at(iMuR2), 0);
	   AllMuMuInvMasshisto->Fill( (double)((allmu+allmu2).M()), w );
	 }
       }
       //
       for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
	 if(!muRisoCheck(iMuR))continue;
	 allmu.SetPtEtaPhiM( MuonPt->at(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0);
	 for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
	   if(!tauRisoCheck(iTauR))continue;
	   alltau.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR) , HPSTauPhi->at(iTauR), 0);
	   AllMuTauInvMasshisto->Fill( (double)((allmu+alltau).M()), w );
	 }
       }
       //
       for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
	 if(!tauRisoCheck(iTauR))continue;
	 //
	 METtauTransMasshisto->Fill( METlepMT("Tau",iTauR),       w );
	 METtauDeltaPhihisto ->Fill( METlepDeltaPhi("Tau",iTauR), w );
	 //
	 alltau.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR) , HPSTauPhi->at(iTauR), 0);
	 for(unsigned int iTauR2=(iTauR+1); iTauR2<HPSTauPt->size(); iTauR2++){
	   if(!tauRisoCheck(iTauR2))continue;
	   alltau2.SetPtEtaPhiM( HPSTauPt->at(iTauR2), HPSTauEta->at(iTauR2) , HPSTauPhi->at(iTauR2), 0);
	   AllTauTauInvMasshisto->Fill( (double)((alltau+alltau2).M()), w );
	 }
       }
       //

       // >>>>>>>>>>>>>>>>>>>>>> Histograms - once per event. 
       Mu.SetPtEtaPhiM(0,0,0,0);
       Tau.SetPtEtaPhiM(0,0,0,0);
       El.SetPtEtaPhiM(0,0,0,0);
       Jet.SetPtEtaPhiM(0,0,0,0);
       BJet.SetPtEtaPhiM(0,0,0,0);
       MuN=0;
       TauN=0;
       ElN=0;
       JetN=0;
       BJetN=0;
       SJT=0;
       VertexN=0;
       //
       //
       for(unsigned int iVertexR=0; iVertexR<VertexX->size(); iVertexR++){
	 if( vertexRCheck(iVertexR) ) VertexN++;
       }
       VertexNhisto->Fill((double)VertexN, w);
       VertexNV2histo->Fill((double)VertexX->size(), w);
       //
       for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
	 if( !tauRisoCheck(iTauR) )continue;  
	 TauN++;
	 TauPthisto->Fill(HPSTauPt->at(iTauR), w );
	 TauEtahisto->Fill(HPSTauEta->at(iTauR), w );
	 TauPhihisto->Fill(HPSTauPhi->at(iTauR), w );
	 if( HPSTauPt->at(iTauR)>Tau.Pt() ){ 
	   Tau.SetPtEtaPhiM(HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0);
	 }
       }
       for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
	 if( !muRisoCheck(iMuR)        )continue;  
	 MuN++;
	 MuPthisto->Fill(MuonPt->at(iMuR), w );
	 MuEtahisto->Fill(MuonEta->at(iMuR), w );
	 MuPhihisto->Fill(MuonPhi->at(iMuR), w );
	 if( MuonPt->at(iMuR)>Mu.Pt() ){ 
	   Mu.SetPtEtaPhiM(MuonPt->at(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0);
	 }
       }
       for(unsigned int iElR=0; iElR<ElectronPt->size(); iElR++){
	 if( !elRisoCheck(iElR)        )continue;  
	 ElN++;
	 ElPthisto->Fill(ElectronPt->at(iElR), w );
	 ElEtahisto->Fill(ElectronEta->at(iElR), w );
	 ElPhihisto->Fill(ElectronPhi->at(iElR), w );
	 if( ElectronPt->at(iElR)>El.Pt() ){
	   El.SetPtEtaPhiM(ElectronPt->at(iElR), ElectronEta->at(iElR), ElectronPhi->at(iElR), 0);
	 }
       }
       for(unsigned int iJetR=0; iJetR<PFJetPt->size(); iJetR++){
	 if( !jetRisoCheck(iJetR)   )continue;  
	 JetN++;
	 SJT+=PFJetPt->at(iJetR);
	 JetPthisto->Fill(PFJetPt->at(iJetR), w );
	 JetEtahisto->Fill(PFJetEta->at(iJetR), w );
	 JetPhihisto->Fill(PFJetPhi->at(iJetR), w );
	 if(PFJetPt->at(iJetR)>Jet.Pt() ){
	   Jet.SetPtEtaPhiM(PFJetPt->at(iJetR),PFJetEta->at(iJetR),PFJetPhi->at(iJetR),0);
	 }
	 //BJet
	 if(  btag_jetRCheck(iJetR)  ){
	   BJetN++;
	   BJetPthisto->Fill(PFJetPt->at(iJetR), w );
	   BJetEtahisto->Fill(PFJetEta->at(iJetR), w );
	   BJetPhihisto->Fill(PFJetPhi->at(iJetR), w );
	   if(PFJetPt->at(iJetR)>BJet.Pt() ){
	     BJet.SetPtEtaPhiM(PFJetPt->at(iJetR),PFJetEta->at(iJetR),PFJetPhi->at(iJetR),0);
	   }
	 }
	 //
       }
       //
       //
       //
       // ------ Filling histograms
       //
       //////  recoSignalType_ = (1000*TotalLepCtr) + (100*IsoTauCtr) + (10*IsoMuCtr) + (IsoElCtr) //////
       int RecoSignalType_=RecoSignalType();
       if( RecoSignalType_>2000      ){RecoSignalTypehisto->Fill((double)1, w );   } // 2,3,4 leptons, including ssdl at given ObjDef (++|--|++-|+--|++--)
       if( abs(RecoSignalType_)<2000 ){RecoSignalTypehisto->Fill((double)0, w );   } // 1 or 0 leptons at given ObjDef
       if( RecoSignalType_<-2000     ){RecoSignalTypehisto->Fill((double)(-1), w );} // 2 leptons opp. sign, sstrilep, etc. at given ObjDef 
       //(+-|+++|---|+++-|+---|++++|----)
       //
       //..............................................................................................................................
       //(++|--|++-|+--|++--) 
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%100-RecoSignalType_%10)==40 ){MuBasedRecoSignalTypehisto->Fill((double)(+4), w ); }//4mu
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%100-RecoSignalType_%10)==30 ){MuBasedRecoSignalTypehisto->Fill((double)(+3), w ); }//3mu
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%100-RecoSignalType_%10)==20 ){MuBasedRecoSignalTypehisto->Fill((double)(+2), w ); }//2mu
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%100-RecoSignalType_%10)==10 ){MuBasedRecoSignalTypehisto->Fill((double)(+1), w ); }//1mu
       //..............................................................................................................................
       //(+-|+++|---|+++-|+---|++++|----) 
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%100-RecoSignalType_%10)==10 ){MuBasedRecoSignalTypehisto->Fill((double)(-1), w ); }//1mu       
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%100-RecoSignalType_%10)==20 ){MuBasedRecoSignalTypehisto->Fill((double)(-2), w ); }//2mu
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%100-RecoSignalType_%10)==30 ){MuBasedRecoSignalTypehisto->Fill((double)(-3), w ); }//3mu 
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%100-RecoSignalType_%10)==40 ){MuBasedRecoSignalTypehisto->Fill((double)(-4), w ); }//4mu 
       if( (RecoSignalType_%100-RecoSignalType_%10)==0   ){MuBasedRecoSignalTypehisto->Fill((double)(0),  w ); }//0mu
       //
       //..............................................................................................................................
       //(++|--|++-|+--|++--) 
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==400 ){TauBasedRecoSignalTypehisto->Fill((double)(+4), w ); }//4tau
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==300 ){TauBasedRecoSignalTypehisto->Fill((double)(+3), w ); }//3tau
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==200 ){TauBasedRecoSignalTypehisto->Fill((double)(+2), w ); }//2tau
       if( RecoSignalType_>+2000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==100 ){TauBasedRecoSignalTypehisto->Fill((double)(+1), w ); }//1tau
       //..............................................................................................................................
       //(+-|+++|---|+++-|+---|++++|----) 
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==100 ){TauBasedRecoSignalTypehisto->Fill((double)(-1), w ); }//1tau       
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==200 ){TauBasedRecoSignalTypehisto->Fill((double)(-2), w ); }//2tau
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==300 ){TauBasedRecoSignalTypehisto->Fill((double)(-3), w ); }//3tau 
       if( RecoSignalType_<-1000 && abs(RecoSignalType_%1000-RecoSignalType_%100)==400 ){TauBasedRecoSignalTypehisto->Fill((double)(-4), w ); }//4tau 
       if( (RecoSignalType_%1000-RecoSignalType_%100)==0   ){TauBasedRecoSignalTypehisto->Fill((double)(0),  w ); }//0tau
       //
       METhisto->Fill(PFMETType1Cor->at(0), w );
       //
       if( MuN>0   )LeadMuPthisto->Fill(Mu.Pt(), w );
       if( TauN>0  )LeadTauPthisto->Fill(Tau.Pt(), w );
       if( ElN>0   )LeadElPthisto->Fill(El.Pt(), w );
       if( JetN>0  )LeadJetPthisto->Fill(Jet.Pt(), w );
       if( BJetN>0 )LeadBJetPthisto->Fill(BJet.Pt(), w );
       //
       if( MuN>0   )LeadMuEtahisto->Fill(Mu.Eta(), w );
       if( TauN>0  )LeadTauEtahisto->Fill(Tau.Eta(), w );
       if( ElN>0   )LeadElEtahisto->Fill(El.Eta(), w );
       if( JetN>0  )LeadJetEtahisto->Fill(Jet.Eta(), w );
       if( BJetN>0 )LeadBJetEtahisto->Fill(BJet.Eta(), w );
       //
       if( MuN>0   )LeadMuPhihisto->Fill(Mu.Phi(), w );
       if( TauN>0  )LeadTauPhihisto->Fill(Tau.Phi(), w );
       if( ElN>0   )LeadElPhihisto->Fill(El.Phi(), w );
       if( JetN>0  )LeadJetPhihisto->Fill(Jet.Phi(), w );
       if( BJetN>0 )LeadBJetPhihisto->Fill(BJet.Phi(), w );
       //
       //
       MaxMuTauInvMasshisto->Fill( (double)(MaxMuTauInvMass()) );
       MaxDiLepMasshisto->Fill(MaxDiLepInvMass(), w );
       SThisto->Fill(  ST(), w );
       //-----------------
       /*if( isFakeMu_TOP() ||  isFakeTau_TOP() ){ 
	 if(  isFakeMu_TOP() && !isFakeTau_TOP()  ){FakesDistrhisto->Fill( (double)1, w);}//fake mu
	 if(  isFakeMu_TOP() &&  isFakeTau_TOP()  ){FakesDistrhisto->Fill( (double)2, w);}//fake both
	 if( !isFakeMu_TOP() &&  isFakeTau_TOP()  ){FakesDistrhisto->Fill( (double)3, w);}//fake tau
	 }*/
       /*if( isFakeMu_LQ() ||  isFakeTau_LQ() ){ 
	 if(  isFakeMu_LQ() && !isFakeTau_LQ()  ){FakesDistrhisto->Fill( (double)1, w);}//fake mu
	 if(  isFakeMu_LQ() &&  isFakeTau_LQ()  ){FakesDistrhisto->Fill( (double)2, w);}//fake both
	 if( !isFakeMu_LQ() &&  isFakeTau_LQ()  ){FakesDistrhisto->Fill( (double)3, w);}//fake tau
	 }*/
       //if(  isFakeMu_TOP() ||  isFakeTau_TOP() ){ STwFakeshisto->Fill(  ST(), w ); } if( !isFakeMu_TOP() && !isFakeTau_TOP() ){ STwRealshisto->Fill( ST(), w ); }//fake
       //if(  isFakeMu_LQ()  ||  isFakeTau_LQ()  ){ STwFakeshisto->Fill(  ST(), w ); } if( !isFakeMu_LQ()  && !isFakeTau_LQ()  ){ STwRealshisto->Fill( ST(), w ); }//fake
       STotherhisto->Fill( ST(), w );//fake
       //-----------------
       SJThisto->Fill( SJT, w  );
       if( MuN>0 && TauN>0 )LeadMuTauDeltaRhisto->Fill(Mu.DeltaR(Tau), w );
       //
       MuNhisto->Fill((double)MuN, w );
       TauNhisto->Fill((double)TauN, w );
       ElNhisto->Fill((double)ElN, w );
       JetNhisto->Fill((double)JetN, w );
       BJetNhisto->Fill((double)BJetN, w );
       TotalNhisto->Fill((double)(MuN+TauN+ElN+JetN), w );
       //

       fillVariableWithValue("tagQ", tagQ);
       fillVariableWithValue("tagPt", tagPt);
       fillVariableWithValue("tagEta",tagEta);
       fillVariableWithValue("tagPhi",tagPhi);
       fillVariableWithValue("tagR04ChargedHadron",tagR04ChargedHadron);
       fillVariableWithValue("tagR04NeutralHadron",tagR04NeutralHadron);
       fillVariableWithValue("tagR04Photon",tagR04Photon);
       fillVariableWithValue("tagR04PU",tagR04PU);
       fillVariableWithValue("tagPFIso",tagPFIso);
       fillVariableWithValue("probe1Pt",   probe1Pt);
       fillVariableWithValue("probe1Eta",  probe1Eta);
       fillVariableWithValue("probe1Phi",  probe1Phi);
       fillVariableWithValue("probe1DecayMode",   probe1DM);
       fillVariableWithValue("probe1MuDeltaR",  probe1MuDeltaR);
       fillVariableWithValue("probe1Fake", probe1Fake);
       fillVariableWithValue("probe1Q",    probe1Q);
       fillVariableWithValue("probe1GenDeltaRTau",probe1GenDeltaRTau);
       fillVariableWithValue("probe1GenDeltaRMu", probe1GenDeltaRMu );
       fillVariableWithValue("probe1GenDeltaREl", probe1GenDeltaREl );
       fillVariableWithValue("probe1Iso",  probe1Iso);
       fillVariableWithValue("probe1ElRej",  probe1ElRej);
       fillVariableWithValue("probe2Pt",   probe2Pt);
       fillVariableWithValue("probe2Eta",  probe2Eta);
       fillVariableWithValue("probe2Phi",  probe2Phi);
       fillVariableWithValue("probe2DecayMode",   probe2DM);
       fillVariableWithValue("probe2MuDeltaR",  probe2MuDeltaR);
       fillVariableWithValue("probe2Fake", probe2Fake);
       fillVariableWithValue("probe2Q",    probe2Q);
       fillVariableWithValue("probe2GenDeltaRTau",probe2GenDeltaRTau);
       fillVariableWithValue("probe2GenDeltaRMu", probe2GenDeltaRMu );
       fillVariableWithValue("probe2GenDeltaREl", probe2GenDeltaREl );
       fillVariableWithValue("probe2Iso",  probe2Iso);
       fillVariableWithValue("probe2ElRej",  probe2ElRej);
       fillVariableWithValue("probe3Pt",   probe3Pt);
       fillVariableWithValue("probe3Eta",  probe3Eta);
       fillVariableWithValue("probe3Phi",  probe3Phi);
       fillVariableWithValue("probe3DecayMode",   probe3DM);
       fillVariableWithValue("probe3MuDeltaR",  probe3MuDeltaR);
       fillVariableWithValue("probe3Fake", probe3Fake);
       fillVariableWithValue("probe3Q",    probe3Q);
       fillVariableWithValue("probe3GenDeltaRTau",probe3GenDeltaRTau);
       fillVariableWithValue("probe3GenDeltaRMu", probe3GenDeltaRMu );
       fillVariableWithValue("probe3GenDeltaREl", probe3GenDeltaREl );
       fillVariableWithValue("probe3Iso",  probe3Iso);
       fillVariableWithValue("probe3ElRej",  probe3ElRej);
       fillVariableWithValue("probe4Pt",   probe4Pt);
       fillVariableWithValue("probe4Eta",  probe4Eta);
       fillVariableWithValue("probe4Phi",  probe4Phi);
       fillVariableWithValue("probe4DecayMode",   probe4DM);
       fillVariableWithValue("probe4MuDeltaR",  probe4MuDeltaR);
       fillVariableWithValue("probe4Fake", probe4Fake);
       fillVariableWithValue("probe4Q",    probe4Q);
       fillVariableWithValue("probe4GenDeltaRTau",probe4GenDeltaRTau);
       fillVariableWithValue("probe4GenDeltaRMu", probe4GenDeltaRMu );
       fillVariableWithValue("probe4GenDeltaREl", probe4GenDeltaREl );
       fillVariableWithValue("probe4Iso",  probe4Iso);
       fillVariableWithValue("probe4ElRej",  probe4ElRej);
       //
       fillVariableWithValue("eventData",eventData);
       fillVariableWithValue("eventST",eventST);
       fillVariableWithValue("eventIsoST",eventIsoST);
       fillVariableWithValue("eventSJT",eventSJT);
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
       fillVariableWithValue("eventPU",eventPU);
       fillVariableWithValue("eventPUWeight",eventPUWeight);
       //
       fillVariableWithValue("probe1DR1p0STpt40",   probe1DR1p0STpt40);
       fillVariableWithValue("probe1DR1p0STpt40W",  probe1DR1p0STpt40W);
       fillVariableWithValue("probe1DR1p0STpt40WC", probe1DR1p0STpt40WC);
       fillVariableWithValue("probe1DR1p0STpt30",   probe1DR1p0STpt30);
       fillVariableWithValue("probe1DR1p0STpt30W",  probe1DR1p0STpt30W);
       fillVariableWithValue("probe1DR1p0STpt30WC", probe1DR1p0STpt30WC);
       fillVariableWithValue("probe1DR1p0STpt20",   probe1DR1p0STpt20);
       fillVariableWithValue("probe1DR1p0STpt20W",  probe1DR1p0STpt20W);
       fillVariableWithValue("probe1DR1p0STpt20WC", probe1DR1p0STpt20WC);
       //
       fillVariableWithValue("probe1DR0p7STpt40",   probe1DR0p7STpt40);
       fillVariableWithValue("probe1DR0p7STpt40W",  probe1DR0p7STpt40W);
       fillVariableWithValue("probe1DR0p7STpt40WC", probe1DR0p7STpt40WC);
       fillVariableWithValue("probe1DR0p7STpt30",   probe1DR0p7STpt30);
       fillVariableWithValue("probe1DR0p7STpt30W",  probe1DR0p7STpt30W);
       fillVariableWithValue("probe1DR0p7STpt30WC", probe1DR0p7STpt30WC);
       fillVariableWithValue("probe1DR0p7STpt20",   probe1DR0p7STpt20);
       fillVariableWithValue("probe1DR0p7STpt20W",  probe1DR0p7STpt20W);
       fillVariableWithValue("probe1DR0p7STpt20WC", probe1DR0p7STpt20WC);
       //
       fillVariableWithValue("probe1DR0p5STpt40",   probe1DR0p5STpt40);
       fillVariableWithValue("probe1DR0p5STpt40W",  probe1DR0p5STpt40W);
       fillVariableWithValue("probe1DR0p5STpt40WC", probe1DR0p5STpt40WC);
       fillVariableWithValue("probe1DR0p5STpt30",   probe1DR0p5STpt30);
       fillVariableWithValue("probe1DR0p5STpt30W",  probe1DR0p5STpt30W);
       fillVariableWithValue("probe1DR0p5STpt30WC", probe1DR0p5STpt30WC);
       fillVariableWithValue("probe1DR0p5STpt20",   probe1DR0p5STpt20);
       fillVariableWithValue("probe1DR0p5STpt20W",  probe1DR0p5STpt20W);
       fillVariableWithValue("probe1DR0p5STpt20WC", probe1DR0p5STpt20WC);
       //
       fillVariableWithValue("eventrhoJets",rhoJets);
       fillVariableWithValue("eventrhoForHEEP",rhoForHEEP);
       fillVariableWithValue("eventrhoJetsCCPU",rhoJetsCCPU);
       fillVariableWithValue("eventrhoJetsCN",rhoJetsCN);
       fillVariableWithValue("eventrhoJetsCNT",rhoJetsCNT);

       fillReducedSkimTree();
     }
     //---------------------------------------------------------------------------------

     ////////////////////// User's code ends here ///////////////////////
   } // End loop over events

   //////////write histos 
   //
   ProvidedPileUpWeightshisto->Write();
   AppliedPileUpWeightshisto->Write();
   AppliedTrigEffWeightshisto->Write();
   Triggerhisto->Write();
   AppliedTotalWeightshisto->Write();
   //
   RecoSignalTypehisto->Write();
   MuBasedRecoSignalTypehisto->Write();
   TauBasedRecoSignalTypehisto->Write();
   //
   METhisto->Write();
   MaxMuTauInvMasshisto->Write();
   MaxDiLepMasshisto->Write();
   AllMuMuInvMasshisto->Write();
   AllMuTauInvMasshisto->Write();
   AllTauTauInvMasshisto->Write();
   METmuTransMasshisto->Write();
   METtauTransMasshisto->Write();
   //
   SThisto->Write();
   STwFakeshisto->Write();//fake
   STwRealshisto->Write();//fake
   STotherhisto->Write(); //fake
   FakesDistrhisto->Write();//fake
   SJThisto->Write();
   LeadMuTauDeltaRhisto->Write();
   //
   METtauDeltaPhihisto->Write();
   METmuDeltaPhihisto ->Write();
   METelDeltaPhihisto ->Write();
   //
   LeadMuPthisto->Write();
   LeadTauPthisto->Write();
   LeadElPthisto->Write();
   LeadJetPthisto->Write();
   LeadBJetPthisto->Write();
   LeadMuEtahisto->Write();
   LeadTauEtahisto->Write();
   LeadElEtahisto->Write();
   LeadJetEtahisto->Write();
   LeadBJetEtahisto->Write();
   LeadMuPhihisto->Write();
   LeadTauPhihisto->Write();
   LeadElPhihisto->Write();
   LeadJetPhihisto->Write();
   LeadBJetPhihisto->Write();
   //
   MuPthisto->Write();
   TauPthisto->Write();
   ElPthisto->Write();
   JetPthisto->Write();
   BJetPthisto->Write();
   MuEtahisto->Write();
   TauEtahisto->Write();
   ElEtahisto->Write();
   JetEtahisto->Write();
   BJetEtahisto->Write();
   MuPhihisto->Write();
   TauPhihisto->Write();
   ElPhihisto->Write();
   JetPhihisto->Write();
   BJetPhihisto->Write();
   //
   MuNhisto->Write();
   TauNhisto->Write();
   ElNhisto->Write();
   JetNhisto->Write();
   BJetNhisto->Write();
   TotalNhisto->Write();
   VertexNhisto->Write();
   VertexNV2histo->Write();
   //
   //-------------------------------------------
   LooseToTightMatrixRAW2Dhisto->Write();
   LooseToTightMatrix2Dhisto->Write();
   //
   //-------------------------------------------
   LooseFakeTauPthisto ->Write();
   LooseFakeTauEtahisto->Write();
   LooseFakeTauPhihisto->Write();
   TightFakeTauPthisto ->Write();
   TightFakeTauEtahisto->Write();
   TightFakeTauPhihisto->Write();
   //
   LoosePromptTauPthisto ->Write();
   LoosePromptTauEtahisto->Write();
   LoosePromptTauPhihisto->Write();
   TightPromptTauPthisto ->Write();
   TightPromptTauEtahisto->Write();
   TightPromptTauPhihisto->Write();
   //
   LooseTauPthisto ->Write();
   LooseTauEtahisto->Write();
   LooseTauPhihisto->Write();
   TightTauPthisto ->Write();
   TightTauEtahisto->Write();
   TightTauPhihisto->Write();
   //
   LooseTightRatioPtTauhisto ->TGraphAsymmErrors::Divide( TightTauPthisto->Rebin(10),  LooseTauPthisto->Rebin(10),  "vcp" );
   LooseTightRatioEtaTauhisto->TGraphAsymmErrors::Divide( TightTauEtahisto->Rebin(10), LooseTauEtahisto->Rebin(10), "vcp" );
   LooseTightRatioPhiTauhisto->TGraphAsymmErrors::Divide( TightTauPhihisto->Rebin(10), LooseTauPhihisto->Rebin(10), "vcp" );
   LooseTightRatioPtTauhisto ->Write();
   LooseTightRatioEtaTauhisto->Write();
   LooseTightRatioPhiTauhisto->Write();

   //////////
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
// // //
