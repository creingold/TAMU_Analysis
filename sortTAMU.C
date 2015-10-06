{
	cout << "File to be sorted? (do not include .root extension)" << endl;

	char filename[256];
	cin >> filename;
	
	char fileLocation[256] , sortLocation[256];
	sprintf(fileLocation, "/home/craig/Documents/TAMU/%s.root" , filename);
	sprintf(sortLocation , "/home/craig/Documents/TAMU/%s_sorted.root" , filename);

	TFile *unsorted = new TFile(fileLocation);
	if ( !run00533 ){
		cout << "Cannot find this file.  Shutting down" << endl;
		exit(1);
	}

	TFile *sorted = new TFile(sortLocation);

	



}
