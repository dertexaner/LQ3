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
#include "TMath.h"
#include "stdio.h"
using namespace std;
void myStyle();
void setTDRStyle();


void makePlotsBO()
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
Double_t mData[11] = {250, 350, 400, 450, 500, 550, 600, 650, 750, 850,900};
 // arrays of LQ masses for theoretical cross section
 //Double_t mTh[9] = {100, 150, 200, 250, 300, 350, 400,450,500};
 Double_t mTh[16] = { 150, 200, 250, 300, 350, 400,450,500,550,600,650,700,750,800,850,900};
 // array of theoretical cross-sections for different leptoquark masses
 //Double_t xsTh[9] = {386, 53.3, 11.9, 3.47, 1.21, 0.477, .205,.0949,.0463};
 Double_t xsTh[16] = { 53.3, 11.9, 3.47, 1.21, 0.477, .205,.0949,.0463,.0236,.0124,.00676,.00377,.00215,.00124,.000732,.000436};
  
 // filename for the final plot (NB: changing the name extension changes the file format)
 string fileName2 = "BR_Sigma_MuMu.pdf";
 string fileName3 = "BR_Sigma_MuMu.png";
 string fileName1 = "BR_Sigma_MuMu.eps";

  
 // axes labels for the final plot
 string title = ";M_{LQ} (GeV);#beta^{2}#times#sigma (pb)";

 // integrated luminosity
 string lint = "5.0 fb^{-1}";

 // region excluded by Tevatron limits
 Double_t x_shaded[5] = {394,675,675,394,394};// CHANGED FOR LQ2
 Double_t y_shaded[5] = {0.0001,0.0001,50,50,0.0001};// CHANGED FOR LQ2

 Double_t x_shaded2[5] = {250,394,394,250,250};// CHANGED FOR LQ2
 Double_t y_shaded2[5] = {0.0001,0.0001,50,50,0.0001};// CHANGED FOR LQ2


 Double_t x_shaded3[5] = {675,842,842,675,675};// CHANGED FOR LQ2
 Double_t y_shaded3[5] = {0.0001,0.0001,50,50,0.0001};// CHANGED FOR LQ2

 // PDF uncertainty band
 //Double_t x_pdf[12] = {100	 ,	150	 ,	200 	,	250	, 300	,	350	,	350	, 300	, 250, 200, 150, 100};
 //Double_t y_pdf[12] = {445.5 ,	61.4 ,	13.7 	,	4.1	, 1.43,	0.57,	0.38, 0.98, 2.9, 10.0, 45.2, 330.3};
 //Double_t x_pdf[18] = {100		,	150		,	200		,	250	,	300		,	350		,	400	,  450 	 ,  500  , 500  , 450 	,	400	,	350		,	300		,	250	, 200		, 150		, 100		};
 //Double_t y_pdf[18] = {445.5	,	61.4	,	13.7	,	4.1	,	1.43	, 0.57	,	.25	, .1167	 ,	.058 , .034	,	.072	, .16	,	0.38	, 0.98	, 2.9	, 10.0	, 45.2	, 330.3	};
 //Double_t x_pdf[28] = {	150		,	200		,	250	,	300		,	350		,	400	,  450 	 ,  500, 600,	650,	700,	750,	800,	850,	850,	800,	750,	700,	650,	600  , 500  , 450 	,	400	,	350		,	300		,	250	, 200		, 150		};
 //Double_t y_pdf[28] = {	61.4	,	13.7	,	4.1	,	1.43	, 0.57	,	.25	, .1167	 ,	.058,0.01561,	0.00866,	0.00491,	0.00284,	0.001677,	0.001008,	0.000456,	0.000803,	0.00144,	0.00263,	0.00486,	0.00919 , .034	,	.072	, .16	,	0.38	, 0.98	, 2.9	, 10.0	, 45.28	};
 
 
 Double_t x_pdf[22] = 	{250,350,400,450,500,550,600,650,750,850,900,900,850,750,650,600,550,500,450,400,350,250}; 															
Double_t y_pdf[22] = 	{4.0599 , 0.552843 , 0.238415 , 0.1115796 , 0.0548655 , 0.0282492 , 0.0149792 , 0.00826072 , 0.00270068 , 0.000945744 , 0.00058206 , 0.00028994 , 0.000518256 , 0.00157932 , 0.00525928 , 0.0098208 , 0.0189508 , 0.0377345 , 0.0780204 , 0.171585 , 0.401157 , 2.8801};
 
Double_t x_shademasses[22]={250, 350, 400, 450, 500, 550, 600, 650, 750, 850,900,900, 850, 750, 650, 600, 550, 500, 450, 400, 350, 250};




Double_t xsUp_expected[11] = {0.051009 , 0.0134037 , 0.0074005 , 0.00496327 , 0.0033336 , 0.00267624 , 0.00216132 , 0.001942148 , 0.001674205 , 0.0012040668 , 0.0011598036 }; 


Double_t xsUp_observed[11] = {0.047886 , 0.0129744 , 0.006273 , 0.00330252 , 0.00302339 , 0.00248508 , 0.00213776 , 0.0020449 , 0.00126893 , 0.0007630368 , 0.0007516204 }; 


Double_t y_1sigma[22]={0.036782 , 0.0096831 , 0.00533 , 0.00358722 , 0.0024076 , 0.00193284 , 0.00155992 , 0.001401348 , 0.0012083 , 0.0008689572 , 0.0008369892 , 0.0016109764 , 0.0016724736 , 0.002325655 , 0.002697916 , 0.00300204 , 0.003717 , 0.00463 , 0.00689923 , 0.0102705 , 0.018603 , 0.070788 }; 


Double_t y_2sigma[22]={0.02776 , 0.0072504 , 0.004018 , 0.00269516 , 0.00181033 , 0.0014514 , 0.00117304 , 0.001053884 , 0.000908375 , 0.00065331 , 0.0006292352 , 0.0021404112 , 0.0022221324 , 0.003089765 , 0.003584152 , 0.00398784 , 0.00493948 , 0.00615327 , 0.00916734 , 0.013653 , 0.0247563 , 0.09369 }; 




 // **********************************************
 // *  Don't change anything below this point!   *
 // **********************************************




  // turn on/off batch mode
 gROOT->SetBatch(kTRUE);

 Int_t size = sizeof(S_eff)/sizeof(*S_eff);

 // Upper Limits can be entered manually here when the call to CLA.C is commented below
 // Array of 95% CL upper limits on the cross section
//  Double_t xsUp[3] = {0.23722, 0.166074, 0.10131};




 // set ROOT style
//  myStyle();
 setTDRStyle();
 gStyle->SetPadLeftMargin(0.14);
 gROOT->ForceStyle();
 
 TCanvas *c = new TCanvas("c","",800,800);
 c->cd();
 
 TH2F *bg = new TH2F("bg",title.c_str(), 500, 250., 900., 500., 0.0001, 50.);
 bg->SetStats(kFALSE);
 bg->SetTitleOffset(1.,"X");
 bg->SetTitleOffset(1.15,"Y");
 
 bg->Draw();

 TPolyLine *p2 = new TPolyLine(5,x_shaded2,y_shaded2,"");
//  pl->SetFillStyle(3001);
 p2->SetFillColor(8);
 p2->SetFillStyle(3345);
 p2->SetLineColor(8);   // CHANGED FOR LQ2
 p2->Draw();
 p2->Draw("F");


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


 TGraph *exshade1 = new TGraph(22,x_shademasses,y_1sigma);
 exshade1->SetFillColor(kGreen);
 TGraph *exshade2 = new TGraph(22,x_shademasses,y_2sigma);
 exshade2->SetFillColor(kYellow);

 exshade2->Draw("f");
 exshade1->Draw("f");

 gPad->RedrawAxis();


 TGraph *grshade = new TGraph(22,x_pdf,y_pdf);
 grshade->SetFillColor(kCyan-6);
 grshade->SetFillStyle(3001);
 grshade->Draw("f");



 // set ROOT style
//  myStyle();
 setTDRStyle();
 gStyle->SetPadLeftMargin(0.14);
 gROOT->ForceStyle();

 TGraph *xsTh_vs_m = new TGraph(16, mTh, xsTh);
 xsTh_vs_m->SetLineWidth(2);
 xsTh_vs_m->SetLineColor(kBlue);
 xsTh_vs_m->SetFillColor(kCyan-6);
 xsTh_vs_m->SetMarkerSize(0.00001);
 xsTh_vs_m->SetMarkerStyle(22);
 grshade->SetFillStyle(1001); 
 xsTh_vs_m->SetMarkerColor(kBlue);
 xsTh_vs_m->Draw("L");

 TGraph *xsData_vs_m_expected = new TGraph(11, mData, xsUp_expected);
 xsData_vs_m_expected->SetMarkerStyle(0);
 xsData_vs_m_expected->SetMarkerColor(kBlack);
 xsData_vs_m_expected->SetLineColor(kBlack);
 xsData_vs_m_expected->SetLineWidth(2);
 xsData_vs_m_expected->SetLineStyle(7);
 xsData_vs_m_expected->SetMarkerSize(0.001);
 xsData_vs_m_expected->Draw("LP");

 TGraph *xsData_vs_m_observed = new TGraph(11, mData, xsUp_observed);
 xsData_vs_m_observed->SetMarkerStyle(21);
 xsData_vs_m_observed->SetMarkerColor(kBlack);
 xsData_vs_m_observed->SetLineColor(kBlack);
 xsData_vs_m_observed->SetLineWidth(2);
 xsData_vs_m_observed->SetLineStyle(1);
 xsData_vs_m_observed->SetMarkerSize(1);
 xsData_vs_m_observed->Draw("LP");
 
 Double_t xsUp_observed_logY[11], xsUp_expected_logY[11], xsTh_logY[15];
 for (int ii = 0; ii<10; ++ii) xsUp_observed_logY[ii] = log10(xsUp_observed[ii]);
 for (int ii = 0; ii<10; ++ii) xsUp_expected_logY[ii] = log10(xsUp_expected[ii]);
 for (int ii = 0; ii<15; ++ii) xsTh_logY[ii] = log10(xsTh[ii]);
 TGraph *xsTh_vs_m_log = new TGraph(15, mTh, xsTh_logY);
 TGraph *xsData_vs_m_expected_log = new TGraph(11, mData, xsUp_expected_logY);
 TGraph *xsData_vs_m_observed_log = new TGraph(11, mData, xsUp_observed_logY);
 
 for (Double_t mtest=250.10; mtest<849.90; mtest = mtest+0.10){
	 if(( pow(10.0,xsData_vs_m_expected_log->Eval(mtest))/pow(10.0,xsTh_vs_m_log->Eval(mtest)) ) < 1.0 && ( pow(10.0,xsData_vs_m_expected_log->Eval(mtest+0.1))/pow(10.0,xsTh_vs_m_log->Eval(mtest+0.10)) ) > 1.0) std::cout<<"## LLJJ expected limit: "<<mtest<<" GeV"<<std::endl;
	 if(( pow(10.0,xsData_vs_m_observed_log->Eval(mtest))/pow(10.0,xsTh_vs_m_log->Eval(mtest)) ) < 1.0 && ( pow(10.0,xsData_vs_m_observed_log->Eval(mtest+0.1))/pow(10.0,xsTh_vs_m_log->Eval(mtest+0.10)) ) > 1.0) std::cout<<"## LLJJ observed limit: "<<mtest<<" GeV"<<std::endl;
	}
 
 
 TLegend *legend = new TLegend(.37,.62,.94,.92);
 legend->SetBorderSize(1);
 legend->SetFillColor(0);
 //legend->SetFillStyle(0);
 legend->SetTextFont(132);
 legend->SetMargin(0.15);
 legend->SetHeader("LQ #bar{LQ} #rightarrow #muq#muq");
 legend->AddEntry(p2,"CMS exclusion (34 pb^{-1})","f");
 legend->AddEntry(pl,"ATLAS exclusion (1.03 fb^{-1})","f");
 legend->AddEntry(p3,"CMS exclusion (5.0 fb^{-1})","f");

 legend->AddEntry(xsTh_vs_m,"#beta^{2}#times #sigma_{theory} with unc., (#beta=1)","lf");
 legend->AddEntry(xsData_vs_m_expected, "Expected 95% C.L. upper limit","lp");
 legend->AddEntry(xsData_vs_m_observed, "Observed 95% C.L. upper limit","lp");
 legend->Draw();

 TLatex l1;
 l1.SetTextAlign(12);
 l1.SetTextFont(132);
 l1.SetNDC();
 l1.SetTextSize(0.04);
  l1.DrawLatex(0.70,0.53,"CMS 2011");
 l1.DrawLatex(0.70,0.48,"#sqrt{s} = 7 TeV");

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
