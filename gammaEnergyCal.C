{
	TCanvas *c0 = new TCanvas("c0" , "E1");

	c0->cd();
	
	double par[2];
	char cString[80];
	char writeString[80];
	char outFileString[80];
	ofstream outFile("calParam.dat",ofstream::app);
	if (!outFile){
		cout << "Error: cannot open outfile";
		exit(1);
	}

	for ( int segment = 108 ; segment < 112 ; segment += 1 ){

		sprintf(writeString , "GammaCal%d.dat" , segment);
		ifstream datFile2(writeString);
		if ( !datFile2 ){
			cout << "Error: Cannot open read file" << endl;
			exit(1);
		}

		
		double channel[2]; 
		double channelSort[2];
		int indexArray[2];
		for ( int b = 0 ; b < 2 ; b++ ){
			datFile2 >> channel[b];
		}
		
		double EnergykeV[2];
		EnergykeV[0] = 1173;
		EnergykeV[1] = 1332;
		
		int iTwo = 2;

		TMath::Sort( iTwo , channel, indexArray , kFALSE );

		for ( int d = 0 ; d < iTwo ; d++ ){
			channelSort[d] = channel[indexArray[d]];
		}
		
		TGraph *graph = new TGraph( 2, channelSort , EnergykeV );
		

		graph->Draw("A*");

		TF1 *total = new TF1("fit","pol 1", 1800 , 3500);
		fit->SetParameters(par);

		graph->Fit(fit,"R+");

		for ( int k = 0 ; k < 2 ; k++ ){
			double toWrite = fit->GetParameter(k);
			//double errorWrite = fit->GetParError(k);
			
			outFile << toWrite << '\t' ;
		}
		outFile << endl;

		graph->Draw();		

		datFile2.close();
	}

	outFile.close();

}
