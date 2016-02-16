/*
Craig Reingold
02/02/16
InterPol

This software will run in ROOT, and allow easier analysis of polarization data.
After execution, this code will open a GUI with two canvases, one with a horizonatlly polarized histogram and the other with a vertically polarize one.  The polarization histograms will need to be made prior to this code's execution.
The code will loop over the peak array delclared in the beginning of this code.
The code will do the following for each peak in the array:
	- Open the correct ROOT file
	- Plot both histograms over an appropriate range
	- Fit Gaussians with linear backgrounds to both peaks
	- Allow the user to move the start and stop range for the integral
	- Integrate both peaks, within the ranges specified
	- Calculate the ratio of horizontal to vertical polarized gammas, and the associated error
*/

#include<iostream>
#include<iomanip>
#include<fstream>

#include<TROOT.h>
#include<TChain.h>
#include<TFile.h>
#include<TTree.h>
#include<TH1.h>
#include<TCanvas.h>
#include<TF1.h>
#include<TLine.h>

#include<TApplication.h>
#include<TGClient.h>
#include<TGButton.h>
#include<TGFrame.h>
#include<TGSplitter.h>
#include<TRootEmbeddedCanvas.h>
#include<RQ_OBJECT.h>

using namespace std;

const float gPi = 3.141592;
const int numberOfPeaks = 3;

const float peakArray[numberOfPeaks] = { 244.7 , 344.28 , 411.12 };

class MyMainFrame{
	RQ_OBJECT("MyMainFrame")
private:
	TGMainFrame		*fMain;
	TRootEmbeddedCanvas 	*fHcanvas;
	TRootEmbeddedCanvas	*fVcanvas;
public:
	MyMainFrame( const TGWindow *p , UInt_t w , UInt_t h );
	virtual ~MyMainFrame();
	void DoDrawH();
	void DoDrawV();

	int placeHolder;// Keeps track of which peak in the peak array the user is currently working on

	float ratio[numberOfPeaks];// Arrays to put the output into, to be written to file later
	float dRatio[numberOfPeaks];
	float ene[numberOfPeaks];
	float dEne[numberOfPeaks];

	float hamp[numberOfPeaks];
	float hmean[numberOfPeaks];
	float hstd[numberOfPeaks];
	float hintercept[numberOfPeaks];
	float hslope[numberOfPeaks];

	float vamp[numberOfPeaks];
	float vmean[numberOfPeaks];
	float vstd[numberOfPeaks];
	float vintercept[numberOfPeaks];
	float vslope[numberOfPeaks];

	float hStart[numberOfPeaks];// To keep track of the starts and stops for each graph
	float hStop[numberOfPeaks];
	float vStart[numberOfPeaks];
	float vStop[numberOfPeaks];

	void Initialize();//The primary buttons
	void Prev();
	void Next();
	void Write();
	void WriteAll();
	void Exit();

	void hStartL();//The secondary buttons
	void hStartR();
	void hStopL();
	void hStopR();

	void vStartL();
	void vStartR();
	void vStopL();
	void vStopR();
};

MyMainFrame::MyMainFrame(const TGWindow *p , UInt_t w , UInt_t h ){
// Create a frame
	fMain = new TGMainFrame(p,w,h);
// Creating the display and corresponding buttons	
// Main canvas frame
	TGHorizontalFrame *canframe = new TGHorizontalFrame( fMain , 1600 , 600 );
// Creating the subframes with the canvases and buttons
	TGVerticalFrame *canHframe = new TGVerticalFrame( canframe , 800 , 600 , kFixedWidth );
	TGVerticalFrame *canVframe = new TGVerticalFrame( canframe , 800 , 600 );
	TGCompositeFrame *canLeft = new TGCompositeFrame( canHframe , 10 , 10 , kSunkenFrame );
	TGCompositeFrame *canRight = new TGCompositeFrame( canVframe , 10 , 10 , kSunkenFrame );

// For the secondary buttons
	TGHorizontalFrame *lframe = new TGHorizontalFrame( canLeft , 800 , 20 );
	TGHorizontalFrame *rframe = new TGHorizontalFrame( canRight , 800 , 20 );

// The Labels
	TGLabel *lcanLabel = new TGLabel( canLeft , "Horizontal Polarization" );
	TGLabel *rcanLabel = new TGLabel( canRight , "Vertical Polarization" );

//Making the canvases
	fHcanvas = new TRootEmbeddedCanvas("Hcanvas", canLeft , 800 , 600);
	fVcanvas = new TRootEmbeddedCanvas("Vcanvas", canRight , 800 , 600);

// Making the left subframe with buttons
	TGTextButton *hStartL = new TGTextButton( lframe , "Start Left" );
	hStartL->Connect( "Clicked()" , "MyMainFrame" , this , "hStartL()" );
	lframe -> AddFrame( hStartL , new TGLayoutHints( kLHintsCenterX , 5 , 5 , 3 , 2 ) );

	TGTextButton *hStartR = new TGTextButton( lframe , "Start Right" );
	hStartR->Connect("Clicked()" , "MyMainFrame" , this , "hStartR()" );
	lframe -> AddFrame( hStartR , new TGLayoutHints( kLHintsCenterX , 5 , 5 , 2 , 5 ) );

	TGTextButton *hStopL = new TGTextButton( lframe , "Stop Left" );
	hStopL->Connect("Clicked()" , "MyMainFrame" , this , "hStopL()" );
	lframe -> AddFrame( hStopL , new TGLayoutHints( kLHintsCenterX , 5 , 5 , 5 , 2 ) );

	TGTextButton *hStopR = new TGTextButton( lframe , "Stop Right" );
	hStopR->Connect("Clicked()" , "MyMainFrame" , this , "hStopR()" );
	lframe -> AddFrame( hStopR , new TGLayoutHints( kLHintsCenterX , 5 , 5 , 2 , 3 ) ); 

// Making the right subframe with buttons
	TGTextButton *rreset = new TGTextButton( rframe , "Re&set" , "gApplication->Terminate(0)");
	rframe->AddFrame( rreset , new TGLayoutHints( kLHintsCenterX , 5 , 5 , 3 , 4) );

// Adding canvases and Labels
	canLeft -> AddFrame( lcanLabel , new TGLayoutHints( kLHintsCenterX | kLHintsTop , 3 , 3 , 3 , 3 ) );
	canRight-> AddFrame( rcanLabel , new TGLayoutHints( kLHintsCenterX | kLHintsTop , 3 , 3 , 3 , 3 ) );
	canLeft->AddFrame( fHcanvas , new TGLayoutHints( kLHintsExpandX | kLHintsExpandY , 10 , 10 , 3 , 3 ) );
	canRight->AddFrame(fVcanvas , new TGLayoutHints( kLHintsExpandX | kLHintsExpandY , 10 , 10 , 3 , 3 ) );
	canLeft -> AddFrame( lframe , new TGLayoutHints( kLHintsExpandX | kLHintsBottom , 2 , 2 , 2 , 2 ) );
	canRight ->AddFrame( rframe , new TGLayoutHints( kLHintsExpandX | kLHintsBottom , 2 , 2 , 2 , 2 ) );

// Adding the composite, sunk frame to the vertical frames
	canHframe->AddFrame(canLeft , new TGLayoutHints( kLHintsExpandX | kLHintsExpandY , 2 , 2 , 3 , 3 ) );
	canVframe->AddFrame(canRight , new TGLayoutHints( kLHintsExpandX | kLHintsExpandY , 2 , 2 , 3 , 3 ) );

// Adding the vertical frames to the main canvas frame
	canframe -> AddFrame( canHframe , new TGLayoutHints( kLHintsLeft | kLHintsExpandY ) );
// This adds the splitter functionality
	TGVSplitter *splitter = new TGVSplitter( canframe , 2 , 2);
	splitter -> SetFrame(canHframe , kTRUE );
	canframe -> AddFrame(splitter , new TGLayoutHints( kLHintsLeft | kLHintsExpandY ) );

	canframe -> AddFrame( canVframe, new TGLayoutHints( kLHintsRight | kLHintsExpandX | kLHintsExpandY) );

// Adding the main canvas frame to the main frame
	fMain -> AddFrame( canframe , new TGLayoutHints( kLHintsCenterX | kLHintsExpandX | kLHintsExpandY ) );

// Create main horizontal frame with the primary widgets
	TGHorizontalFrame *hframe = new TGHorizontalFrame( fMain , 1600 , 30 );

	TGTextButton *init = new TGTextButton( hframe , "Initialize" );
	init->Connect("Clicked()" , "MyMainFrame" , this , "Initialize()");
	hframe->AddFrame( init , new TGLayoutHints( kLHintsLeft , 5 , 5 , 3 , 4 ) );

	TGTextButton *hexit = new TGTextButton( hframe , "&Exit" , "gApplication->Terminate(0)");
	hframe->AddFrame( hexit , new TGLayoutHints( kLHintsRight , 5 , 5 , 3 , 4) );
	fMain->AddFrame( hframe , new TGLayoutHints( kLHintsCenterX | kLHintsBottom | kLHintsExpandX, 2 , 2, 2, 2) );


// Set a name to the main frame
	fMain -> SetWindowName("InterPol");
// Map all subwindows of main frame
	fMain -> MapSubwindows();
// Initialize the layout algorithm
	fMain -> Resize( fMain -> GetDefaultSize() );
// Map main frame
	fMain -> MapWindow();

}

MyMainFrame::~MyMainFrame(){
// Cleans up used widgets: frames, buttons, layouthints
	fMain -> Cleanup();
	delete fMain;
}

void MyMainFrame::Initialize(){
// This function will initialize or reset all variables, depending on when it is called
	for ( int d = 0 ; d < 2 ; d++ ){
		for ( int c = 0 ; c < numberOfPeaks ; c++ ){
		// Defining, fitting, and drawing the Gaussian with Linear Background
			TF1 *fit = new TF1("fit" , "gaus(0) + pol1(3)" , peakArray[c] - 15 , peakArray[c] + 15 );
			Double_t param[5];
			param[0] = 200;
			param[1] = peakArray[c];
			param[2] = 1.5;
			param[3] = 45;
			param[4] = -0.05;
			fit->SetParameters(param);
			fit->SetParLimits( 2 , 0.5 , 4 );
			fit->SetParLimits( 1 , param[1] - 5 , param[1] + 5 );
			fit->SetParLimits( 0 , 1 , 1000 );
	
			if ( d == 0 ){
				h1->Fit(fit, "GERMQN0");

				hamp[c] = fit->GetParameter(0);
				hmean[c] = fit->GetParameter(1);
				hstd[c] = fit->GetParameter(2);
				hintercept[c] = fit->GetParameter(3);
				hslope[c] = fit->GetParameter(4);

//				hStart[c] = h1->FindBin( hmean[c] - 3*abs( hstd[c] ) );
//				hStop[c] = h1-> FindBin( hmean[c] + 3*abs( hstd[c] ) );

				hStart[c] = hmean[c] - 5*abs( hstd[c] );
				hStop[c] =  hmean[c] + 5*abs( hstd[c] );
			}

			else(){
				h2->Fit(fit , "GERMQN0");	
				
				vamp[c] = fit->GetParameter(0);
				vmean[c] = fit->GetParameter(1);
				vstd[c] = fit->GetParameter(2);
				vintercept[c] = fit->GetParameter(3);
				vslope[c] = fit->GetParameter(4);

				vStart[c] = vmean[c] - 5*abs( vstd[c] )  ;
				vStop[c] =  vmean[c] + 5*abs( vstd[c] )  ;
			}	
		}
	}

	placeHolder = 0;

	DoDrawH();
	DoDrawV();
}

void MyMainFrame::DoDrawH(){
// This function will redraw the horizontal canvas
	TCanvas *fCanvas = fHcanvas->GetCanvas();
	fCanvas->cd();

// Temporary variables to ease my pain
	float thisPeak = peakArray[placeHolder];
	float thisMin = peakArray[placeHolder] - 15;
	float thisMax = peakArray[placeHolder] + 15;
// Making fit functions to draw over the graph
	TF1 *fit = new TF1( "fit" , "gaus(0) + pol1(3)" , thisMin , thisMax );
	TF1 *bkg = new TF1( "bkg" , "pol1(0)" , thisMin , thisMax );
	TF1 *fpk = new TF1( "fpk" , "gaus(0)" , thisMin , thisMax ); 

	fit->SetLineColor(12);
	fit->SetLineStyle(2);

	bkg->SetLineColor(38);
	bkg->SetLineStyle(2);

	fpk->SetLineColor(39);
	fpk->SetLineStyle(2);

	fit->SetParameters( hamp[placeHolder] , hmean[placeHolder] , hstd[placeHolder] , hintercept[placeHolder] , hslope[placeHolder] );
	bkg->SetParameters( hintercept[placeHolder] , hslope[placeHolder] );
	fpk->SetParameters( hamp[placeHolder] , hmean[placeHolder] , hstd[placeHolder] );

// Making vertical lines for better visualization
	TLine *startLine = new TLine( hStart[placeHolder] , 0 , hStart[placeHolder] , hamp[placeHolder] + 20 );
	TLine *stopLine = new TLine( hStop[placeHolder] , 0 , hStop[placeHolder] , hamp[placeHolder] + 20 );
	startLine->SetLineStyle(9);
	stopLine->SetLineStyle(9);
	startLine->SetLineColor(8);
	stopLine->SetLineColor(46);

	TLine *minLine = new TLine( thisMin, 0 , thisMin , hamp[placeHolder] + 20 );
	TLine *maxLine = new TLine( thisMax, 0 , thisMax , hamp[placeHolder] + 20 );
	minLine->SetLineStyle(9);
	maxLine->SetLineStyle(9);

	TLine *zeroLine = new TLine( thisPeak - 65 , 0 , thisPeak + 65 , 0);
	zeroLine->SetLineStyle(2);
	zeroLine->SetLineColor(13);

// Making a background subtracted histogram
	TH1 *h4 = (TH1*) h1->Clone();
	h4->SetName("h4");
	h4->SetLineColor(30);
	h4->Add(bkg, -1);

// Draw everything
	h1->Draw();
	h1->GetXaxis()->SetRangeUser( thisPeak - 65 , thisPeak + 65 );
	if ( thisPeak < 180 ){
		h1->GetXaxis()->SetRangeUser( 101 , thisPeak + 80 );
	}
	h1->GetYaxis()->SetRangeUser( -hamp[placeHolder]*0.10 , hamp[placeHolder] + 50 );

	h4->Draw("same");
	fit->Draw("same");
	bkg->Draw("same");
	fpk->Draw("same");
	minLine->Draw("same");
	maxLine->Draw("same");
	startLine->Draw("same");
	stopLine->Draw("same");
	zeroLine->Draw("same");	

	h1->Draw("same");

// Update the canvas
	fCanvas->Update();
}
/*
// for debugging
void MyMainFrame::DoDrawH(){
	TCanvas *fCanvas = fHcanvas->GetCanvas();
	fCanvas->cd();

	TF1 *f1 = new TF1("f1" , "sin(x) / x" , 0 , 20 );
	f1->Draw();

	TCanvas *fCanvas = fHcanvas->GetCanvas();
	fCanvas->cd();
	fCanvas->Update();
}

void MyMainFrame::DoDrawV(){
	TF1 *f1 = new TF1("f1" , "sin(x) / x" , 0 , 20 );
	f1->Draw();
	f1->SetLineColor(4);

	TCanvas *c0 = fVcanvas->GetCanvas();
	c0->cd();
	c0->Update();
}
*/
void MyMainFrame::DoDrawV(){
// This function will redraw the vertical canvas
	TCanvas *fCanvas = fVcanvas->GetCanvas();
	fCanvas->cd();

// Temporary variables to ease my pain
	float thisPeak = peakArray[placeHolder];
	float thisMin = peakArray[placeHolder] - 15;
	float thisMax = peakArray[placeHolder] + 15;
// Making fit functions to draw over the graph
	TF1 *fit = new TF1( "fit" , "gaus(0) + pol1(3)" , thisMin , thisMax );
	TF1 *bkg = new TF1( "bkg" , "pol1(0)" , thisMin , thisMax );
	TF1 *fpk = new TF1( "fpk" , "gaus(0)" , thisMin , thisMax ); 

	fit->SetLineColor(12);
	fit->SetLineStyle(2);

	bkg->SetLineColor(38);
	bkg->SetLineStyle(2);

	fpk->SetLineColor(39);
	fpk->SetLineStyle(2);

	fit->SetParameters( vamp[placeHolder] , vmean[placeHolder] , vstd[placeHolder] , vintercept[placeHolder] , vslope[placeHolder] );
	bkg->SetParameters( vintercept[placeHolder] , vslope[placeHolder] );
	fpk->SetParameters( vamp[placeHolder] , vmean[placeHolder] , vstd[placeHolder] );

// Making vertical lines for better visualization
	TLine *startLine = new TLine( vStart[placeHolder] , 0 , vStart[placeHolder] , vamp[placeHolder] + 20 );
	TLine *stopLine = new TLine( vStop[placeHolder] , 0 , vStop[placeHolder] , vamp[placeHolder] + 20 );
	startLine->SetLineStyle(9);
	stopLine->SetLineStyle(9);
	startLine->SetLineColor(8);
	stopLine->SetLineColor(46);

	TLine *minLine = new TLine( thisMin, 0 , thisMin , vamp[placeHolder] + 20 );
	TLine *maxLine = new TLine( thisMax, 0 , thisMax , vamp[placeHolder] + 20 );
	minLine->SetLineStyle(9);
	maxLine->SetLineStyle(9);

	TLine *zeroLine = new TLine( thisPeak - 65 , 0 , thisPeak + 65 , 0);
	zeroLine->SetLineStyle(2);
	zeroLine->SetLineColor(13);

// Making a background subtracted histogram
	TH1 *h4 = (TH1*) h2->Clone();
	h4->SetName("h4");
	h4->SetLineColor(30);
	h4->Add(bkg, -1);

// Draw everything
	h2->Draw();
	h2->GetXaxis()->SetRangeUser( thisPeak - 65 , thisPeak + 65 );
	if ( thisPeak < 180 ){
		h2->GetXaxis()->SetRangeUser( 101 , thisPeak + 65 );
	}
	h2->GetYaxis()->SetRangeUser( -hamp[placeHolder]*0.1 , hamp[placeHolder] + 50 );

	h4->Draw("same");
	fit->Draw("same");
	bkg->Draw("same");
	fpk->Draw("same");
	minLine->Draw("same");
	maxLine->Draw("same");
	startLine->Draw("same");
	stopLine->Draw("same");
	zeroLine->Draw("same");	

	h2->Draw("same");

// Updating the canvas
	fCanvas->Update();
}

void MyMainFrame::hStartL(){
	hStart[placeHolder] += -1;
	DoDrawH();
}

void MyMainFrame::hStartR(){
	hStart[placeHolder] += 1;
	DoDrawH();
}

void MyMainFrame::hStopL(){
	hStop[placeHolder] += -1;
	DoDrawH();
}

void MyMainFrame::hStopR(){
	hStop[placeHolder] += 1;
	DoDrawH();
}

void interpol(){
	TFile *myfile = new TFile("clover0Energy.root");
// Popup the GUI
	new MyMainFrame( gClient -> GetRoot() , 1600 , 600 );
}
