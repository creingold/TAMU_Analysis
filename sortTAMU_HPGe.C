// Initializing structures of detectors

struct leafStruct{
	float energy;
	float time;
	float mult; //Short for multiplicity
};

struct BGOStruct{
	float energy;
	float time;
	float mult;
};


struct cloverStruct{
	float energy;
	float time;
	leafStruct leaf[4];
	float mult;
};



int sortTAMU_HPGe(){
	cout << "File to be sorted? (do not include .root extension)" << endl;

// Reading in the file to be sorted, and the calParam.dat containing the leaf calibaration parameters
// Also generating a tree to store the data into within the ROOT file.

	char filename[256];
	cin >> filename;
	
	char fileLocation[256] , sortLocation[256];
	sprintf(fileLocation, "/home/craig/Documents/TAMU/%s.root" , filename);
	sprintf(sortLocation , "/home/craig/Documents/TAMU/%s_sorted.root" , filename);

	TFile *unsorted = new TFile(fileLocation , "READ");
	if ( !unsorted ){
		cout << "Cannot find this file.  Shutting down" << endl;
		exit(1);
	}

	TFile *sorted = new TFile(sortLocation, "RECREATE");
	if ( !sorted ){
		cout << "Cannot open write file.  Shutting down" << endl;
		exit(1);
	}

	ifstream calibration("/home/craig/Documents/TAMU/calParam.dat");
	if ( !calibration ){
		cout << "Cannot find calParam.dat.  Shutting down" << endl;
		exit(1);
	}

// Initializing the TTree and proper branches using struct definitions above.

	TTree *t0 = new TTree("t0" , "Detector Tree");

	leafStruct leaf[48] = {0};
	BGOStruct BGO[12] = {0};
	cloverStruct clover[12] = {0};

	t0->Branch("leaves" , leaf , "leaf[48]/F:energy/F:time/F:mult/F" );
	t0->Branch("BGO" , BGO , "BGO[12]/F:energy/F:time/F:mult/F" );
	t0->Branch("clovers" , clover , "clover[12]/F:energy/F:time/F:leaf[4]/L:mult/F" );

//	TH1F *h = new TH1F("h" , "" , 4096 , 0 , 4096 );

// Loop for reading data into structures and filling branches.

	unsorted->cd();

	float interceptCal[48] , slopeCal[48];
	
	for ( int b =  0 ; b < 48 ; b++ ){
		calibration >> interceptCal[b];
		calibration >> slopeCal[b];
	}

	calibration.close();

	// For debugging
	TH1F *h = new TH1F("h" , "h" , 4000 , 0 , 4000 );

	int *readIn;

	t->SetBranchAddress("ADC" , ADC );
	t->SetBranchAddress("TDC" , TDC );
	int entries = t->GetEntries();

	for ( int i = 0 ; i < entries ; i ++ ){
	
	}

	h->Draw();

	sorted->Write();

	return 0;
}
