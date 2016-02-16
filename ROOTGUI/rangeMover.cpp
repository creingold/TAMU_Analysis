/*
An example from root.cern.ch transcribed by Craig.
Looking to see if learning this is super useful or not

*/

#include<TApplication.h>
#include<TGClient.h>
#include<TCanvas.h>
#include<TF1.h>
#include<TRandom.h>
#include<TGButton.h>
#include<TGFrame.h>
#include<TRootEmbeddedCanvas.h>
#include<RQ_OBJECT.h>
#include<TLine.h>
#include<TROOT.h>

class MyMainFrame{
	RQ_OBJECT("MyMainFrame")
private:
	TGMainFrame	*fMain;
	TRootEmbeddedCanvas *fEcanvas;
public:
	MyMainFrame( const TGWindow *p , UInt_t w , UInt_t h );
	virtual ~MyMainFrame();
	void DoDraw();

	int rangeStart;
	int rangeStop;

	void Reset();

	void StartLeft();
	void StartRight();
	void StopLeft();
	void StopRight();
};

MyMainFrame::MyMainFrame(const TGWindow *p , UInt_t w , UInt_t h ){
// Create a frame
	fMain = new TGMainFrame(p,w,h);

//Create canvas widget
	fEcanvas = new TRootEmbeddedCanvas("Ecanvas" , fMain, 800 , 600 );
	fMain -> AddFrame( fEcanvas , new TGLayoutHints ( kLHintsExpandX | kLHintsExpandY, 10 , 10 , 10 , 1 ));

//Create a horizontal frame widget with buttons
	TGHorizontalFrame *hframe = new TGHorizontalFrame( fMain , 800 , 50 );
	TGTextButton *reset = new TGTextButton( hframe , "&Reset Variables" );
	reset->Connect( "Clicked()" , "MyMainFrame" , this , "Reset()" );
	hframe->AddFrame( reset , new TGLayoutHints( kLHintsCenterX , 5 , 5 , 3, 4 ) );

	TGTextButton *startLeft = new TGTextButton( hframe , "Start &Left" );
	startLeft->Connect( "Clicked()" , "MyMainFrame" , this , "StartLeft()" );
	hframe->AddFrame( startLeft , new TGLayoutHints( kLHintsCenterX , 5 , 5 , 3 , 4 ) );

	TGTextButton *startRight = new TGTextButton( hframe , "Start &Right" );
	startRight->Connect( "Clicked()" , "MyMainFrame" , this , "StartRight()" );
	hframe->AddFrame( startRight , new TGLayoutHints( kLHintsCenterX , 5 , 5 , 3 , 4 ) );

	TGTextButton *stopLeft = new TGTextButton( hframe , "&Stop Left" );
	stopLeft->Connect( "Clicked()" , "MyMainFrame" , this , "StopLeft()" );
	hframe->AddFrame( stopLeft , new TGLayoutHints( kLHintsCenterX , 5 , 5 , 3 , 4 ) );

	TGTextButton *stopRight = new TGTextButton( hframe , "S&top Right" );
	stopRight->Connect( "Clicked()" , "MyMainFrame" , this , "StopRight()" );
	hframe->AddFrame( stopRight , new TGLayoutHints( kLHintsCenterX , 5 , 5 , 3 , 4 ) );

	TGTextButton *exit = new TGTextButton( hframe , "&Exit" , "gApplication->Terminate(0)");
	hframe->AddFrame( exit , new TGLayoutHints( kLHintsCenterX , 5, 5 , 3 , 4 ) );
	fMain->AddFrame(hframe , new TGLayoutHints( kLHintsCenterX , 2 , 2, 2, 2 ) );

//Set a name to the main frame
	fMain->SetWindowName("Simple Example");

//Map all subwindows of main frame
	fMain->MapSubwindows();

//Initialize the layout algorithm
	fMain->Resize( fMain->GetDefaultSize() );

//Map main frame
	fMain->MapWindow();
}

void MyMainFrame::DoDraw(){
//Draws function graphics in randomly choosen interval

	TF1 *f1 = new TF1("f1" , "sin(x)/x" , 0 , 15);
	f1 -> SetFillColor(19);
	f1 -> SetFillStyle(1);
	f1 -> SetLineWidth(3);
	f1 -> Draw();

	TLine *startLine = new TLine( rangeStart , 0 , rangeStart , 1 );
	TLine *stopLine = new TLine( rangeStop , 0 , rangeStop , 1 );

	startLine->SetLineStyle(9);
	stopLine->SetLineStyle(9);

	startLine->SetLineColor(8);
	stopLine->SetLineColor(46);

	startLine->Draw("same");
	stopLine->Draw("same");

	TCanvas *fCanvas = fEcanvas -> GetCanvas();
	fCanvas -> cd();
	fCanvas -> Update();
}

void MyMainFrame::StartLeft(){
//Hopefully moves the start line over
	rangeStart += -1;
	DoDraw();	
}

void MyMainFrame::StartRight(){
//Hopefully moves the start line over
	rangeStart += 1;
	DoDraw();	
}

void MyMainFrame::StopLeft(){
//Hopefully moves the start line over
	rangeStop += -1;
	DoDraw();	
}


void MyMainFrame::StopRight(){
//Hopefully moves the start line over
	rangeStop += 1;
	DoDraw();	
}


void MyMainFrame::Reset(){
	rangeStart = 5;
	rangeStop = 12;
	DoDraw();
}

MyMainFrame::~MyMainFrame(){
//Cleans up used widgets: frames, buttons, layouthints
	fMain -> Cleanup();
	delete fMain;
}

void rangeMover(){
//Popup the GUI...
	new MyMainFrame(gClient->GetRoot(), 800 , 600 );
}
