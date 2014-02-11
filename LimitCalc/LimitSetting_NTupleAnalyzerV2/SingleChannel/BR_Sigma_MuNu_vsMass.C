#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TF1.h"
#include "TLegend.h"
#include "TPolyLine.h"
#include "TPad.h"
#include "TLatex.h"

using namespace std;
void myStyle();
void setTDRStyle();

void makePlotsBH()
{
 // **********************************************
 // *            Input parameters                *
 // **********************************************
 // switch to include/exclude sytematics uncertainties
 bool systematics = true; // does nothing at the moment

 // total integrated luminosity (in pb-1)
 Double_t L_int = 4980;
 // relative uncertainty on the integrated luminosity (0.1 = 10% uncertainty)
 Double_t Sigma_L_int = 0.11;

 // array of signal efficiencies
 Double_t S_eff[4] = {.3353,.4574,.5276,.5763};
 // array of relative uncertainties on the signal efficiencies
 Double_t Sigma_S_eff[4] = {0.21, 0.21, 0.21,.21};

 // array of N_background for L_int
 Double_t N_bkg[4] = {.2724,.1786,.1098,.0753};
 // array of relative uncertainties on N_background (0.1 = 10%)
 Double_t Sigma_N_bkg[4] = {0.88, 0.88, 0.88, 0.88};

 // array of N_observed for L_int
 Double_t N_obs[4] = {0, 0, 0, 0};

 // array of LQ masses for calculation of upXS
Double_t mData[10] = {250, 350, 400, 450, 500, 550, 600, 650, 750, 850};
 // arrays of LQ masses for theoretical cross section
 //Double_t mTh[9] = {100, 150, 200, 250, 300, 350, 400,450,500};
 Double_t mTh[15] = { 150, 200, 250, 300, 350, 400,450,500,550,600,650,700,750,800,850};
 // array of theoretical cross-sections for different leptoquark masses
 //Double_t xsTh[9] = {386/2.0, 53.3/2.0, 11.9/2.0, 3.47/2.0, 1.21/2.0, 0.477/2.0, .205/2.0,.0949/2.0,.0463/2.0};
 Double_t xsTh[15] = { 53.3/2.0, 11.9/2.0, 3.47/2.0, 1.21/2.0, 0.477/2.0, .205/2.0,.0949/2.0,.0463/2.0,.0236/2.0,.0124/2.0,.00676/2.0,.00377/2.0,.00215/2.0,.00124/2.0,.000732/2.0};
  
 // filename for the final plot (NB: changing the name extension changes the file format)
 string fileName2 = "BR_Sigma_MuNu.pdf";
 string fileName3 = "BR_Sigma_MuNu.png";
 string fileName1 = "BR_Sigma_MuNu.eps";
  
 // axes labels for the final plot
 string title = ";M_{LQ} (GeV);2#beta(1-#beta)#times#sigma (pb)";

 // integrated luminosity
 string lint = "5.0 fb^{-1}";

 // region excluded by Tevatron limits
 Double_t x_shaded[5] = {250,552,552,250,250};// CHANGED FOR LQ2
 Double_t y_shaded[5] = {0.0001,0.0001,50,50,0.0001};// CHANGED FOR LQ2

 Double_t x_shaded3[5] = {552,615,615,552,552};// CHANGED FOR LQ2
 Double_t y_shaded3[5] = {0.0001,0.0001,50,50,0.0001};// CHANGED FOR LQ2


 //Double_t x_pdf[28] = {	150		,	200		,	250	,	300		,	350		,	400	,  450 	 ,  500, 600,	650,	700,	750,	800,	850,	850,	800,	750,	700,	650,	600  , 500  , 450 	,	400	,	350		,	300		,	250	, 200		, 150		};
 //Double_t y_pdf[28] = {	61.4	/2.0,	13.7	/2.0,	4.1	/2.0,	1.43	/2.0, 0.57	/2.0,	.25	/2.0, .1167	 /2.0,	.058/2.0,0.01561/2.0,	0.00866/2.0,	0.00491/2.0,	0.00284/2.0,	0.001677/2.0,	0.001008/2.0,	0.000456/2.0,	0.000803/2.0,	0.00144/2.0,	0.00263/2.0,	0.00486/2.0,	0.00919 /2.0, .034	/2.0,	.072	/2.0, .16	/2.0,	0.38	/2.0, 0.98	/2.0, 2.9	/2.0, 10.0	/2.0, 45.2	/2.0};
 
Double_t x_shademasses[20]={250, 350, 400, 450, 500, 550, 600, 650, 750, 850, 850, 750, 650, 600, 550, 500, 450, 400, 350, 250};

  Double_t x_pdf[20] = 	{250,350,400,450,500,550,600,650,750,850,850,750,650,600,550,500,450,400,350,250}; 																	
Double_t y_pdf[20] = 	{2.00913 , 0.276183 , 0.119105 , 0.0557898 , 0.0274096 , 0.014101 , 0.0075082 , 0.00413374 , 0.00135355 , 0.000480558 , 0.000251442 , 0.00078645 , 0.00262626 , 0.0048918 , 0.009499 , 0.0188904 , 0.0390102 , 0.085895 , 0.200817 , 1.46087}; 			

//////////// ASYMPTOTIC /////////////////

//Double_t xsUp_expected[10] = {0.141923 , 0.03384315 , 0.02200675 , 0.014068925 , 0.01037583 , 0.00729122 , 0.00574554 , 0.005273814 , 0.00359996 , 0.003205611 }; 


//Double_t xsUp_observed[10] = {0.18391 , 0.0349641 , 0.02171975 , 0.01339988 , 0.00853772 , 0.00717794 , 0.00455576 , 0.006753916 , 0.0045080125 , 0.004213758 }; 


//Double_t y_1sigma[20]={0.102365 , 0.0244224 , 0.01587725 , 0.0101543 , 0.007489025 , 0.00526162 , 0.00414594 , 0.00380588 , 0.0025979525 , 0.0023134128 , 0.0044526096 , 0.0050003625 , 0.007325136 , 0.00798002 , 0.01012794 , 0.01441319 , 0.019544655 , 0.0305655 , 0.0470322 , 0.197096 }; 


//Double_t y_2sigma[20]={0.077034 , 0.0183645 , 0.01194125 , 0.007634705 , 0.00563008 , 0.00395536 , 0.00311736 , 0.00286117 , 0.0019531675 , 0.0017391954 , 0.0059158776 , 0.0066436075 , 0.009732372 , 0.01060262 , 0.01345554 , 0.01914968 , 0.02596464 , 0.0406105 , 0.062487 , 0.2618115 }; 


Double_t xsUp_expected[10] = {0.1313395 , 0.0312912 , 0.020664 , 0.01348529 , 0.0100934 , 0.00715906 , 0.00567052 , 0.005269758 , 0.003567925 , 0.0031765506 }; 


Double_t xsUp_observed[10] = {0.174541 , 0.032436 , 0.02038725 , 0.01285895 , 0.00818584 , 0.00704106 , 0.00448136 , 0.00675155 , 0.0045041425 , 0.004189785 }; 


Double_t y_1sigma[20]={0.094731 , 0.02258595 , 0.01491375 , 0.009731995 , 0.007285305 , 0.00516604 , 0.004092 , 0.003802838 , 0.0025749475 , 0.002292441 , 0.0044122032 , 0.004955965 , 0.007319728 , 0.00787586 , 0.00994386 , 0.01401964 , 0.01873326 , 0.02871025 , 0.04347855 , 0.1823485 }; 


Double_t y_2sigma[20]={0.0713085 , 0.0169812 , 0.0112135 , 0.00731679 , 0.00547729 , 0.00388456 , 0.00307644 , 0.002859142 , 0.0019357525 , 0.0017234208 , 0.005862222 , 0.00658459 , 0.009724936 , 0.01046436 , 0.01321246 , 0.01862649 , 0.02489227 , 0.03814025 , 0.0577647 , 0.2423795 }; 





////////////  FULL CLS  /////////////////

  

//Double_t xsUp_expected[10] = {0.3165334 , 0.059536755 , 0.03237975 , 0.019297915 , 0.0128829287 , 0.0080891714 , 0.006322698 , 0.0053530412 , 0.00442430225 , 0.0037808532 }; 


//Double_t xsUp_observed[10] = {0.3323566 , 0.059536755 , 0.027971225 , 0.01767545715 , 0.01061355735 , 0.0079504624 , 0.0058610336 , 0.0064787164 , 0.0056629495 , 0.0053919852 }; 


//Double_t y_1sigma[20]={0.24801825 , 0.04442778 , 0.024162325 , 0.015120417 , 0.0100352935 , 0.0059720744 , 0.0046267128 , 0.0040133106 , 0.00334538925 , 0.00283745892 , 0.0052764756 , 0.00622165925 , 0.0075395632 , 0.008942384 , 0.0116223274 , 0.0177623005 , 0.0258607245 , 0.045562275 , 0.07978302 , 0.40399475 }; 


//Double_t y_2sigma[20]={0.1522636 , 0.036550125 , 0.01987885 , 0.011847316 , 0.0071747406 , 0.0042791166 , 0.0035134904 , 0.00320833994 , 0.002698637 , 0.00248859138 , 0.0070422426 , 0.0085735335 , 0.0103770056 , 0.012493434 , 0.015822856 , 0.023259268 , 0.03475489485 , 0.058149275 , 0.101827575 , 0.5413894 }; 




 //// **********************************************
 // *  Don't change anything below this point!   *
 // **********************************************




  // turn on/off batch mode
 gROOT->SetBatch(kTRUE);

 Int_t size = sizeof(S_eff)/sizeof(*S_eff);


////// MEDIAN



 // set ROOT style
//  myStyle();
 setTDRStyle();
 gStyle->SetPadLeftMargin(0.14);
 gROOT->ForceStyle();
 
 TCanvas *c = new TCanvas("c","",800,800);
 c->cd();
 
 TH2F *bg = new TH2F("bg",title.c_str(), 500, 250., 850., 500., 0.0001, 50.);
 bg->SetStats(kFALSE);
 bg->SetTitleOffset(1.,"X");
 bg->SetTitleOffset(1.15,"Y");
 
 bg->Draw();

 //TPolyLine *pl = new TPolyLine(5,x_shaded,y_shaded,"F");
////  pl->SetFillStyle(3001);
 //pl->SetLineColor(0);
 //pl->SetFillColor(kGray);
 //pl->SetLineColor(kGray);   // CHANGED FOR LQ2
 //pl->Draw();
 
 
 
  //TPolyLine *p2 = new TPolyLine(5,x_shaded2,y_shaded2,"");
////  pl->SetFillStyle(3001);
 //p2->SetFillColor(8);
 //p2->SetFillStyle(3345);
 //p2->SetLineColor(8);   // CHANGED FOR LQ2
 //p2->Draw();
 //p2->Draw("F");


 TPolyLine *pl = new TPolyLine(5,x_shaded,y_shaded,"");
//  pl->SetFillStyle(3001);
 pl->SetLineColor(14);
 pl->SetFillColor(14);
 pl->SetFillStyle(3344);
 pl->Draw();
 pl->Draw("F");
 
 

 TPolyLine *p3 = new TPolyLine(5,x_shaded3,y_shaded3,"");
 p3->SetLineColor(46);
 p3->SetFillColor(46);
 p3->SetFillStyle(3354);
 p3->Draw();
 p3->Draw("F");

 TGraph *exshade1 = new TGraph(20,x_shademasses,y_1sigma);
 exshade1->SetFillColor(kGreen);
 TGraph *exshade2 = new TGraph(20,x_shademasses,y_2sigma);
 exshade2->SetFillColor(kYellow);

 exshade2->Draw("f");
 exshade1->Draw("f");

 gPad->RedrawAxis();


 TGraph *grshade = new TGraph(20,x_pdf,y_pdf);
 grshade->SetFillColor(kCyan-6);
 grshade->SetFillStyle(3001);
 grshade->Draw("f");



 // set ROOT style
//  myStyle();
 setTDRStyle();
 gStyle->SetPadLeftMargin(0.14);
 gROOT->ForceStyle();

 TGraph *xsTh_vs_m = new TGraph(15, mTh, xsTh);
 xsTh_vs_m->SetLineWidth(2);
 xsTh_vs_m->SetLineColor(kBlue);
 xsTh_vs_m->SetFillColor(kCyan-6);
 xsTh_vs_m->SetMarkerSize(0.00001);
 xsTh_vs_m->SetMarkerStyle(22);
 grshade->SetFillStyle(1001); 
 xsTh_vs_m->SetMarkerColor(kBlue);
 xsTh_vs_m->Draw("L");

 TGraph *xsData_vs_m_expected = new TGraph(10, mData, xsUp_expected);
 xsData_vs_m_expected->SetMarkerStyle(0);
 xsData_vs_m_expected->SetMarkerColor(kBlack);
 xsData_vs_m_expected->SetLineColor(kBlack);
 xsData_vs_m_expected->SetLineWidth(2);
 xsData_vs_m_expected->SetLineStyle(7);
 xsData_vs_m_expected->SetMarkerSize(0.001);
 xsData_vs_m_expected->Draw("LP");

 TGraph *xsData_vs_m_observed = new TGraph(10, mData, xsUp_observed);
 xsData_vs_m_observed->SetMarkerStyle(21);
 xsData_vs_m_observed->SetMarkerColor(kBlack);
 xsData_vs_m_observed->SetLineColor(kBlack);
 xsData_vs_m_observed->SetLineWidth(2);
 xsData_vs_m_observed->SetLineStyle(1);
 xsData_vs_m_observed->SetMarkerSize(1);
 xsData_vs_m_observed->Draw("LP");
 
 Double_t xsUp_observed_logY[10], xsUp_expected_logY[10], xsTh_logY[15];
 for (int ii = 0; ii<10; ++ii) xsUp_observed_logY[ii] = log10(xsUp_observed[ii]);
 for (int ii = 0; ii<10; ++ii) xsUp_expected_logY[ii] = log10(xsUp_expected[ii]);
 for (int ii = 0; ii<15; ++ii) xsTh_logY[ii] = log10(xsTh[ii]);
 TGraph *xsTh_vs_m_log = new TGraph(15, mTh, xsTh_logY);
 TGraph *xsData_vs_m_expected_log = new TGraph(10, mData, xsUp_expected_logY);
 TGraph *xsData_vs_m_observed_log = new TGraph(10, mData, xsUp_observed_logY);
 
 for (Double_t mtest=250.10; mtest<849.90; mtest = mtest+0.10){
	 if(( pow(10.0,xsData_vs_m_expected_log->Eval(mtest))/pow(10.0,xsTh_vs_m_log->Eval(mtest)) ) < 1.0 && ( pow(10.0,xsData_vs_m_expected_log->Eval(mtest+0.1))/pow(10.0,xsTh_vs_m_log->Eval(mtest+0.10)) ) > 1.0) std::cout<<"## LVJJ expected limit: "<<mtest<<" GeV"<<std::endl;
	 if(( pow(10.0,xsData_vs_m_observed_log->Eval(mtest))/pow(10.0,xsTh_vs_m_log->Eval(mtest)) ) < 1.0 && ( pow(10.0,xsData_vs_m_observed_log->Eval(mtest+0.1))/pow(10.0,xsTh_vs_m_log->Eval(mtest+0.10)) ) > 1.0) std::cout<<"## LVJJ observed limit: "<<mtest<<" GeV"<<std::endl;
	}
	
 TLegend *legend = new TLegend(.37,.66,.94,.92);
 
  legend->SetBorderSize(1);
 legend->SetFillColor(0);
 //legend->SetFillStyle(0);
 legend->SetTextFont(132);
 legend->SetMargin(0.15);
 legend->SetHeader("LQ #bar{LQ} #rightarrow #muq#nuq");
 legend->AddEntry(pl,"ATLAS exclusion (1.03 fb^{-1})","f");
 legend->AddEntry(p3,"CMS exclusion (5.0 fb^{-1})","f"); 
 legend->AddEntry(xsTh_vs_m,"2#beta(1-#beta)#times #sigma_{theory} with unc., (#beta=1/2)","lf");
 legend->AddEntry(xsData_vs_m_expected, "Expected 95% C.L. upper limit","lp");
 legend->AddEntry(xsData_vs_m_observed, "Observed 95% C.L. upper limit","lp");
 legend->Draw();

 TLatex l1;
  l1.SetTextAlign(12);
 l1.SetTextFont(132);
 l1.SetNDC();
 l1.SetTextSize(0.04);
 l1.DrawLatex(0.7,0.53,"CMS 2011");
 l1.DrawLatex(0.7,0.48,"#sqrt{s} = 7 TeV");

 //l1.DrawLatex(0.76,0.58,"CMS 2011");
 //l1.DrawLatex(0.76,0.53,"Preliminary");
 //l1.DrawLatex(0.76,0.48,"#sqrt{s} = 7 TeV");

//  TLatex l2;
//  l2.SetTextAlign(12);
//  l2.SetTextSize(0.037);
//  l2.SetTextFont(42);
//  l2.SetNDC();
//  l2.DrawLatex(0.4,0.485,"EXO-10-005 scaled to #sqrt{s} = 7 TeV");

 c->SetGridx();
 c->SetGridy();


 c->SetLogy();
 c->SaveAs((fileName1).c_str());
 c->SaveAs((fileName2).c_str());
 c->SaveAs((fileName3).c_str());

 delete pl;
 delete xsTh_vs_m;
 delete bg;
 delete c;
}


void myStyle()
{
 gStyle->Reset("Default");
 gStyle->SetCanvasColor(0);
 gStyle->SetPadColor(0);
 gStyle->SetTitleFillColor(10);
 gStyle->SetCanvasBorderMode(0);
 gStyle->SetStatColor(0);
 gStyle->SetPadBorderMode(0);
 gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
 gStyle->SetPadTickY(1);
 gStyle->SetFrameBorderMode(0);
 gStyle->SetPalette(1);

   //gStyle->SetOptStat(kFALSE);
 gStyle->SetOptStat(111110);
 gStyle->SetOptFit(0);
 gStyle->SetStatFont(42);
 gStyle->SetPadLeftMargin(0.13);
 gStyle->SetPadRightMargin(0.07);
//    gStyle->SetTitleFont(42);
//    gStyle->SetTitleFont(42, "XYZ");
//    gStyle->SetLabelFont(42, "XYZ");
 gStyle->SetStatY(.9);
}

void setTDRStyle() {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

  // For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

  // For the histo:
  tdrStyle->SetHistFillColor(63);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

//  tdrStyle->SetEndErrorSize(0);
//   tdrStyle->SetErrorX(0.);
//  tdrStyle->SetErrorMarker(20);

  tdrStyle->SetMarkerStyle(20);

  //For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

  //For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

  // For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

  // Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.13);
  tdrStyle->SetPadRightMargin(0.05);

  // For the Global title:

  //  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

  // For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(132, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.05);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(132, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

  // For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  // Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  // Postscript options:
  // tdrStyle->SetPaperSize(15.,15.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);
  
  
  

  tdrStyle->cd();
}

