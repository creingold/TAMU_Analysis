{	
	float detectorRaw[4];
	float detectorCal[4];

	TFile *myFile = new TFile("rootMaker.root","RECREATE");


	TTree *myTree = new TTree("detTree" , "Detector Tree");


	myTree->Branch("detectorRaw", detectorRaw , "detectorRaw[4]/F" );
	myTree->Branch("detectorCal" , detectorCal , "detectorCal[4]/F" );


	TCanvas c0("c0" , "Detectors Uncalibrated");
	TCanvas c1("c1" , "Detectors Calibrated");

	TH1F *h0 = new TH1F("h0" , "Detector 0 Uncalibrated" , 100 , 0 , 1000);
	TH1F *h1 = new TH1F("h1" , "Detector 1 Uncalibrated" , 100 , 0 , 1000);
	TH1F *h2 = new TH1F("h2" , "Detector 2 Uncalibrated" , 100 , 0 , 1000);
	TH1F *h3 = new TH1F("h3" , "Detector 3 Uncalibrated" , 100 , 0 , 1000);

	TH1F *h0cal = new TH1F("h0cal" , "Detector 0 Calibrated" , 100 , 0 , 1000);
	TH1F *h1cal = new TH1F("h1cal" , "Detector 1 Calibrated" , 100 , 0 , 1000);
	TH1F *h2cal = new TH1F("h2cal" , "Detector 2 Calibrated" , 100 , 0 , 1000);
	TH1F *h3cal = new TH1F("h3cal" , "Detector 3 Calibrated" , 100 , 0 , 1000);

	ifstream data("hist.dat");

	if ( !data ){
		cout << "Could not open data.  Check input." << endl;
		exit(1);
	}

	// Reading the data into a pointer, and then placing it into the histogram array

	float slopeCal[4] = { 1.05 , 1.05 , 0.95 , 0.95 };
	float OffsetCal[4] = {-20 , 20, -20 , 20};

	while ( !data.eof() ){
		data >> detectorRaw[0] >> detectorRaw[1] >> detectorRaw[2] >> detectorRaw[3];

		// This is where I will calibrate;

		for ( int c = 0 ; c < 4 ; c++ ){
			detectorCal[c] = detectorRaw[c] * slopeCal[c] + OffsetCal[c];
		}

		myTree->Fill();
	}

	data.close();
//Making Histograms


	c0.cd();

	h0->SetLineColor(4);
	myTree->Draw( "detectorRaw[0] >> h0" );

	h1->SetLineColor(1);
	myTree->Draw( "detectorRaw[1] >> h1" );

	h2->SetLineColor(2);
	myTree->Draw( "detectorRaw[2] >> h2" );
	
	h3->SetLineColor(3);
	myTree->Draw( "detectorRaw[3] >> h3" );

	h3->GetYaxis()->SetRangeUser(0,6500);
	
	h0->Draw("same");
	h1->Draw("same");
	h2->Draw("same");
	h3->Draw("same");


	c1.cd();

	h0cal->SetLineColor(4);
	myTree->Draw( "detectorCal[0] >> h0cal" );

	h1cal->SetLineColor(1);
	myTree->Draw( "detectorCal[1] >> h1cal" );

	myTree->Draw( "detectorCal[2] >> h2cal" );
	h2cal->SetLineColor(2);
	
	myTree->Draw( "detectorCal[3] >> h3cal" );
	h3cal->SetLineColor(3);

	h0cal->Draw();
	h1cal->Draw("same");
	h2cal->Draw("same");
	h3cal->Draw("same");

	myFile->Write();

}

