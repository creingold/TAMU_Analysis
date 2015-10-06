{
	int ADC[180], TDC[180];

	TFile *unsorted = new TFile("run00522.root", "READ");
	if ( !unsorted ){
		cout << "Cannot find this file.  Shutting down" << endl;
		exit(1);
	}

	t->SetBranchAddress("ADC" , ADC);
	t->SetBranchAddress("TDC" , TDC);

	int entries = t->GetEntries();

	for ( int i = 0 ; i < entries ; i++ ){
		t->GetEntry(i);
		
		cout << ADC[64] << endl;

	}

}
