{
//	TFile *myFile = new TFile("~/Documents/run00068.root");
	
	TH1F *diffHist = new TH1F("diffHist" , "Difference Histogram" , 3000 , 100, 3101 );
	TH1F *h1 = new TH1F("h1" , "Horizontal Polarization" , 3000, 100, 3101);	
	TH1F *h2 = new TH1F("h2" , "Vertical Polarization" , 3000, 100, 3101);	

	TCanvas c1("c1" , "Clover 0 Energy w/ PID == 3");
	TCanvas c2("c2" , "Clover 0 Energy Diff w/ PID == 3");

	c1.cd();
	t->Draw("clover.energy[0] >> h1" , "telescope.pid==3 &&  ((leaf.energy[0] > 10 && leaf.energy[1] > 10) || (leaf.energy[2] > 10 && leaf.energy[3] > 10)  )");
	t->Draw("clover.energy[0] >> h2" , "telescope.pid==3 &&  ((leaf.energy[0] > 10 && leaf.energy[3] > 10) || (leaf.energy[2] > 10 && leaf.energy[1] > 10)  )");

	h1->Draw();
	h2->Draw("same");

	h2->SetLineColor(3);

	diffHist.Add(h1,1);
	diffHist.Add(h2,-1);

	c2.cd();
	diffHist->Draw();

}
