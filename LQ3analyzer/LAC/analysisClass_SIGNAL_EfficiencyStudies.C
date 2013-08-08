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

   TH1::SetDefaultSumw2();

   ////////// Set analysis Object ID variables here
   ObjDef  = "LI";
   BTagDef = "CSVM";
   idDef   = "CUT";
   MuonPtCut = 25;
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
   //
   TH1D* GenLeptonHisto =  new TH1D("GenLeptonHisto","GenLeptonHisto",5,-0.5,4.5);
   TH1D* MatchedGenLeptonHisto =  new TH1D("MatchedGenLeptonHisto","MatchedGenLeptonHisto",5,-0.5,4.5);
   TH1D* DeltaRMinGenMuMuHisto = new TH1D("DeltaRMinGenMuMuHisto","DeltaRMinGenMuMuHisto",100,0,5);
   TH1D* DeltaRMinGenTauTauHisto = new TH1D("DeltaRMinGenTauTauHisto","DeltaRMinGenTauTauHisto",100,0,5);
   TH1D* DeltaRMinGenMuTauHisto = new TH1D("DeltaRMinGenMuTauHisto","DeltaRMinGenMuTauHisto",100,0,5);
   //
   TH1D* TotalGenLepPtHisto   =  new TH1D("TotalGenLepPtHisto","TotalGenLepPtHisto",100,0,500);
   TH1D* MatchedGenLepPtHisto =  new TH1D("MatchedGenLepPtHisto","MatchedGenLepPtHisto",100,0,500);
   TH1D* EffGenLepPtHisto     =  new TH1D("EffGenLepPtHisto","EffGenLepPtHisto",100,0,500);


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


     if( !passEventFilter() ) continue;

     int NGenMuon=0;
     int NMatchedGenMuon=0;
     int NGenTau=0;
     int NMatchedGenTau=0;

     TLorentzVector GenTau,  GenTau2, GenMuon, GenMuon2;
     TLorentzVector RecoTau, RecoMuon;
     //
     for( unsigned int iMuT=0; iMuT<GenLQTauMuonPt->size(); iMuT++){
       if( GenLQTauMuonPt->at(iMuT)<25         ) continue;
       if( fabs(GenLQTauMuonEta->at(iMuT))>2.1 ) continue;
       NGenMuon++;
       // Is Matched to a Reco Muon?
       GenMuon.SetPtEtaPhiM( GenLQTauMuonPt->at(iMuT),     GenLQTauMuonEta->at(iMuT),   GenLQTauMuonPhi->at(iMuT), 0 );
       double DeltaRminMu=999.0;
       for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
	 if( !muRCheck(iMuR) ) continue;
	 RecoMuon.SetPtEtaPhiM( MuonPt->at(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0 );
	 if( fabs(RecoMuon.DeltaR(GenMuon))<DeltaRminMu ) DeltaRminMu=fabs(RecoMuon.DeltaR(GenMuon));
       }
       //
       if( DeltaRminMu<0.15 ) NMatchedGenMuon++; 
     }
     for( unsigned int iMuT=0; iMuT<GenWMuPt->size(); iMuT++){
       if( GenWMuPt->at(iMuT)<25         ) continue;
       if( fabs(GenWMuEta->at(iMuT))>2.1 ) continue;
       NGenMuon++;
       // Is Matched to a Reco Muon?
       GenMuon.SetPtEtaPhiM( GenWMuPt->at(iMuT),     GenWMuEta->at(iMuT),   GenWMuPhi->at(iMuT), 0 );
       double DeltaRminMu=999.0;
       for(unsigned int iMuR=0; iMuR<MuonPt->size(); iMuR++){
	 if( !muRCheck(iMuR) ) continue;
	 RecoMuon.SetPtEtaPhiM( MuonPt->at(iMuR), MuonEta->at(iMuR), MuonPhi->at(iMuR), 0 );
	 if( fabs(RecoMuon.DeltaR(GenMuon))<DeltaRminMu ) DeltaRminMu=fabs(RecoMuon.DeltaR(GenMuon));
       }
       //
       if( DeltaRminMu<0.15 ) NMatchedGenMuon++; 
     }

     //HPSTauAgainstElectronLooseMVA3Discr->at(iTauR)!=1

     for( unsigned int iTauT=0; iTauT<GenLQTauTauPt->size(); iTauT++){
       if( GenLQTauTauTauVisiblePt->at(iTauT)<20          ) continue;
       if( fabs(GenLQTauTauTauVisibleEta->at(iTauT))>2.1  ) continue;
       if( GenLQTauTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
       NGenTau++;
       // Is Matched to a Reco Tau?
       GenTau.SetPtEtaPhiM( GenLQTauTauPt->at(iTauT), GenLQTauTauEta->at(iTauT), GenLQTauTauPhi->at(iTauT), 0 );
       double DeltaRminTau=999.0;
       for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
	 if( !tauRCheck(iTauR) ) continue;
	 RecoTau.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
	 if( fabs(RecoTau.DeltaR(GenTau))<DeltaRminTau ) DeltaRminTau=fabs(RecoTau.DeltaR(GenTau));
       }
       //
       if( DeltaRminTau<0.15 ) NMatchedGenTau++;
     }

     for( unsigned int iTauT=0; iTauT<GenWTauPt->size(); iTauT++){
       if( GenWTauTauVisiblePt->at(iTauT)<20          ) continue;
       if( fabs(GenWTauTauVisibleEta->at(iTauT))>2.1  ) continue;
       if( GenWTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
       NGenTau++;
       // Is Matched to a Reco Tau?
       GenTau.SetPtEtaPhiM( GenWTauPt->at(iTauT), GenWTauEta->at(iTauT), GenWTauPhi->at(iTauT), 0 );
       double DeltaRminTau=999.0;
       for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
	 if( !tauRCheck(iTauR) ) continue;
	 RecoTau.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
	 if( fabs(RecoTau.DeltaR(GenTau))<DeltaRminTau ) DeltaRminTau=fabs(RecoTau.DeltaR(GenTau));
       }
       //
       if( DeltaRminTau<0.15 ) NMatchedGenTau++;
     }


     // Gen Mu-Mu DeltaR
     for( unsigned int iMuT=0; iMuT<GenLQTauMuonPt->size(); iMuT++){
       if( GenLQTauMuonPt->at(iMuT)<25         ) continue;
       if( fabs(GenLQTauMuonEta->at(iMuT))>2.1 ) continue;
       for( unsigned int iMuT2=(iMuT+1); iMuT2<GenLQTauMuonPt->size(); iMuT2++){
	 if( GenLQTauMuonPt->at(iMuT2)<25         ) continue;
	 if( fabs(GenLQTauMuonEta->at(iMuT2))>2.1 ) continue;
	 GenMuon.SetPtEtaPhiM( GenLQTauMuonPt->at(iMuT),     GenLQTauMuonEta->at(iMuT),   GenLQTauMuonPhi->at(iMuT), 0 );
	 GenMuon2.SetPtEtaPhiM( GenLQTauMuonPt->at(iMuT2),     GenLQTauMuonEta->at(iMuT2),   GenLQTauMuonPhi->at(iMuT2), 0 );
	 DeltaRMinGenMuMuHisto->Fill(GenMuon.DeltaR(GenMuon2));
       }
     }
     for( unsigned int iMuT=0; iMuT<GenWMuPt->size(); iMuT++){
       if( GenWMuPt->at(iMuT)<25         ) continue;
       if( fabs(GenWMuEta->at(iMuT))>2.1 ) continue;
       for( unsigned int iMuT2=(iMuT+1); iMuT2<GenWMuPt->size(); iMuT2++){
	 if( GenWMuPt->at(iMuT2)<25         ) continue;
	 if( fabs(GenWMuEta->at(iMuT2))>2.1 ) continue;
	 GenMuon.SetPtEtaPhiM( GenWMuPt->at(iMuT),    GenWMuEta->at(iMuT),   GenWMuPhi->at(iMuT), 0 );
	 GenMuon2.SetPtEtaPhiM( GenWMuPt->at(iMuT2),    GenWMuEta->at(iMuT2),   GenWMuPhi->at(iMuT2), 0 );
	 DeltaRMinGenMuMuHisto->Fill(GenMuon.DeltaR(GenMuon2));
       }
     }


     //Gen Tau-Tau DeltaR
     for( unsigned int iTauT=0; iTauT<GenLQTauTauPt->size(); iTauT++){
       if( GenLQTauTauTauVisiblePt->at(iTauT)<20          ) continue;
       if( fabs(GenLQTauTauTauVisibleEta->at(iTauT))>2.1  ) continue;
       if( GenLQTauTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
       for( unsigned int iTauT2=(iTauT+1); iTauT2<GenLQTauTauPt->size(); iTauT2++){
         if( GenLQTauTauTauVisiblePt->at(iTauT2)<20          ) continue;
         if( fabs(GenLQTauTauTauVisibleEta->at(iTauT2))>2.1  ) continue;
         if( GenLQTauTauTauDecayMode->at(iTauT2)<3           ) continue;//mode 1: electron, 2: muon (semileptonic)
         GenTau.SetPtEtaPhiM( GenLQTauTauPt->at(iTauT), GenLQTauTauEta->at(iTauT), GenLQTauTauPhi->at(iTauT), 0);
	 GenTau2.SetPtEtaPhiM( GenLQTauTauPt->at(iTauT2), GenLQTauTauEta->at(iTauT2), GenLQTauTauPhi->at(iTauT2), 0);
	 DeltaRMinGenTauTauHisto->Fill(GenTau.DeltaR(GenTau2));
       }
     }
     for( unsigned int iTauT=0; iTauT<GenWTauPt->size(); iTauT++){
       if( GenWTauTauVisiblePt->at(iTauT)<20          ) continue;
       if( fabs(GenWTauTauVisibleEta->at(iTauT))>2.1  ) continue;
       if( GenWTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
       for( unsigned int iTauT2=(iTauT+1); iTauT2<GenWTauPt->size(); iTauT2++){
         if( GenWTauTauVisiblePt->at(iTauT2)<20          ) continue;
         if( fabs(GenWTauTauVisibleEta->at(iTauT2))>2.1  ) continue;
         if( GenWTauTauDecayMode->at(iTauT2)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
         GenTau.SetPtEtaPhiM( GenWTauPt->at(iTauT), GenWTauEta->at(iTauT), GenWTauPhi->at(iTauT), 0);
         GenTau2.SetPtEtaPhiM( GenWTauPt->at(iTauT2), GenWTauEta->at(iTauT2), GenWTauPhi->at(iTauT2), 0);
	 DeltaRMinGenTauTauHisto->Fill(GenTau.DeltaR(GenTau2));
       }
     }


     // Gen Mu-Tau DeltaR
     for( unsigned int iMuT=0; iMuT<GenLQTauMuonPt->size(); iMuT++){
       if( GenLQTauMuonPt->at(iMuT)<25         ) continue;
       if( fabs(GenLQTauMuonEta->at(iMuT))>2.1 ) continue;
       GenMuon.SetPtEtaPhiM( GenLQTauMuonPt->at(iMuT),     GenLQTauMuonEta->at(iMuT),   GenLQTauMuonPhi->at(iMuT), 0 );
       for( unsigned int iTauT=0; iTauT<GenLQTauTauPt->size(); iTauT++){
	 if( GenLQTauTauTauVisiblePt->at(iTauT)<20          ) continue;
	 if( fabs(GenLQTauTauTauVisibleEta->at(iTauT))>2.1  ) continue;
	 if( GenLQTauTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
	 GenTau.SetPtEtaPhiM( GenLQTauTauPt->at(iTauT), GenLQTauTauEta->at(iTauT), GenLQTauTauPhi->at(iTauT), 0);
	 //
	 DeltaRMinGenMuTauHisto->Fill(GenMuon.DeltaR(GenTau));
       }
     }
     for( unsigned int iMuT=0; iMuT<GenWMuPt->size(); iMuT++){
       if( GenWMuPt->at(iMuT)<25         ) continue;
       if( fabs(GenWMuEta->at(iMuT))>2.1 ) continue;
       GenMuon.SetPtEtaPhiM( GenWMuPt->at(iMuT),    GenWMuEta->at(iMuT),   GenWMuPhi->at(iMuT), 0 );
       for( unsigned int iTauT=0; iTauT<GenLQTauTauPt->size(); iTauT++){
         if( GenLQTauTauTauVisiblePt->at(iTauT)<20          ) continue;
         if( fabs(GenLQTauTauTauVisibleEta->at(iTauT))>2.1  ) continue;
         if( GenLQTauTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
         GenTau.SetPtEtaPhiM( GenLQTauTauPt->at(iTauT), GenLQTauTauEta->at(iTauT), GenLQTauTauPhi->at(iTauT), 0);
         //
         DeltaRMinGenMuTauHisto->Fill(GenMuon.DeltaR(GenTau));
       }
     }
     for( unsigned int iMuT=0; iMuT<GenLQTauMuonPt->size(); iMuT++){
       if( GenLQTauMuonPt->at(iMuT)<25         ) continue;
       if( fabs(GenLQTauMuonEta->at(iMuT))>2.1 ) continue;
       GenMuon.SetPtEtaPhiM( GenLQTauMuonPt->at(iMuT),     GenLQTauMuonEta->at(iMuT),   GenLQTauMuonPhi->at(iMuT), 0 );
       for( unsigned int iTauT=0; iTauT<GenWTauPt->size(); iTauT++){
	 if( GenWTauTauVisiblePt->at(iTauT)<20          ) continue;
	 if( fabs(GenWTauTauVisibleEta->at(iTauT))>2.1  ) continue;
	 if( GenWTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
	 GenTau.SetPtEtaPhiM( GenWTauPt->at(iTauT), GenWTauEta->at(iTauT), GenWTauPhi->at(iTauT), 0);
	 //
	 DeltaRMinGenMuTauHisto->Fill(GenMuon.DeltaR(GenTau));
       }
     }
     for( unsigned int iMuT=0; iMuT<GenWMuPt->size(); iMuT++){
       if( GenWMuPt->at(iMuT)<25         ) continue;
       if( fabs(GenWMuEta->at(iMuT))>2.1 ) continue;
       GenMuon.SetPtEtaPhiM( GenWMuPt->at(iMuT),    GenWMuEta->at(iMuT),   GenWMuPhi->at(iMuT), 0 );
       for( unsigned int iTauT=0; iTauT<GenWTauPt->size(); iTauT++){
         if( GenWTauTauVisiblePt->at(iTauT)<20          ) continue;
         if( fabs(GenWTauTauVisibleEta->at(iTauT))>2.1  ) continue;
         if( GenWTauTauDecayMode->at(iTauT)<3 ) continue;//mode 1: electron, 2: muon (semileptonic)
         GenTau.SetPtEtaPhiM( GenWTauPt->at(iTauT), GenWTauEta->at(iTauT), GenWTauPhi->at(iTauT), 0);
         //
         DeltaRMinGenMuTauHisto->Fill(GenMuon.DeltaR(GenTau));
       }
     }



     /*
     cout<<"       GenMuon: "<<NGenMuon<<endl;
     cout<<"MatchedGenMuon: "<<NMatchedGenMuon<<endl;
     cout<<"        GenTau: "<<NGenTau<<endl;
     cout<<" MatchedGenTau: "<<NMatchedGenTau<<endl;
     */

     GenLeptonHisto->Fill( 1, NGenMuon);
     GenLeptonHisto->Fill( 2, NMatchedGenMuon);
     GenLeptonHisto->Fill( 3, NGenTau);
     GenLeptonHisto->Fill( 4, NMatchedGenTau);
     //MatchedGenLeptonHisto->Fill( 1, NMatchedGenMuon);
     //MatchedGenLeptonHisto->Fill( 3, NMatchedGenTau);



     TLorentzVector taugen, taureco;
     for( unsigned int iTauT=0; iTauT<GenLQTauTauPt->size(); iTauT++){
       if( GenLQTauTauTauVisiblePt->at(iTauT)<20          ) continue;
       if( fabs(GenLQTauTauTauVisibleEta->at(iTauT))>2.1  ) continue;
       if( GenLQTauTauTauDecayMode->at(iTauT)<3           ) continue;//mode 1: electron, 2: muon (semileptonic)
       taugen.SetPtEtaPhiM( GenLQTauTauPt->at(iTauT),     GenLQTauTauEta->at(iTauT),   GenLQTauTauPhi->at(iTauT), 0 );
       TotalGenLepPtHisto->Fill(taugen.Pt());
       double deltaRmin=9999.0;
       for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
         if( !tauRCheck(iTauR) ) continue;
	 taureco.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
	 if( taugen.DeltaR(taureco)<deltaRmin ) deltaRmin=taugen.DeltaR(taureco);
       }
       if( deltaRmin<0.15 ) MatchedGenLepPtHisto->Fill(taugen.Pt());
     }
     for( unsigned int iTauT=0; iTauT<GenWTauPt->size(); iTauT++){
       if( GenWTauTauVisiblePt->at(iTauT)<20          ) continue;
       if( fabs(GenWTauTauVisibleEta->at(iTauT))>2.1  ) continue;
       if( GenWTauTauDecayMode->at(iTauT)<3           ) continue;//mode 1: electron, 2: muon (semileptonic)
       taugen.SetPtEtaPhiM( GenWTauPt->at(iTauT),     GenWTauEta->at(iTauT),   GenWTauPhi->at(iTauT), 0 );
       TotalGenLepPtHisto->Fill(taugen.Pt());
       double deltaRmin=9999.0;
       for(unsigned int iTauR=0; iTauR<HPSTauPt->size(); iTauR++){
         if( !tauRCheck(iTauR) ) continue;
         taureco.SetPtEtaPhiM( HPSTauPt->at(iTauR), HPSTauEta->at(iTauR), HPSTauPhi->at(iTauR), 0 );
         if( taugen.DeltaR(taureco)<deltaRmin ) deltaRmin=taugen.DeltaR(taureco);
       }
       if( deltaRmin<0.15 ) MatchedGenLepPtHisto->Fill(taugen.Pt());
     }



     //---------------------------------------------------------------------------------
     ////////////////////// User's code ends here ///////////////////////
   } // End loop over events

   //////////write histos 
   //
   TotalGenLepPtHisto->Write();
   MatchedGenLepPtHisto->Write();
   EffGenLepPtHisto->Divide(MatchedGenLepPtHisto,TotalGenLepPtHisto,1,1,"B");
   EffGenLepPtHisto->Write();
   //
   ProvidedPileUpWeightshisto->Write();
   AppliedPileUpWeightshisto->Write();
   GenLeptonHisto->Write();
   MatchedGenLeptonHisto->Write();
   DeltaRMinGenMuTauHisto->Write();
   DeltaRMinGenMuMuHisto->Write();
   DeltaRMinGenTauTauHisto->Write();

   GenLeptonHisto->Print();
   MatchedGenLeptonHisto->Print();

   cout<<endl;
   cout<<"Muons: "<< GenLeptonHisto->GetBinContent(3)<<" / "<< GenLeptonHisto->GetBinContent(2)<<"  :  "
       <<GenLeptonHisto->GetBinContent(3)/GenLeptonHisto->GetBinContent(2)<<endl;
   cout<<" Taus: "<< GenLeptonHisto->GetBinContent(5)<<" / "<< GenLeptonHisto->GetBinContent(4)<<"  :  "
       <<GenLeptonHisto->GetBinContent(5)/GenLeptonHisto->GetBinContent(4)<<endl;
   cout<<endl;
   //

   //////////
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}
// // //
