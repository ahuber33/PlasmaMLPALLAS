void readAndAccessTreesInformations(const char *filename, std::vector<float>& x, std::vector<float>& y, std::vector<float>& energy, std::vector<float>& parentID)
{
    // Open the ROOT file
    TFile *file = TFile::Open(filename);
    if (!file || file->IsZombie())
    {
        printf("Error opening file %s\n", filename);
        return ;
    }

    // Get the two trees
    TTree *tree = (TTree *)file->Get("YAG");

    // Variables to hold data
    std::vector<float> *value1 = 0;
    std::vector<float> *value2 = 0;
    std::vector<float> *value3 = 0;
    std::vector<float> *value4 = 0;

    // Set branch addresses
    tree->SetBranchAddress("x_exit", &value1);
    tree->SetBranchAddress("z_exit", &value2);
    tree->SetBranchAddress("energy", &value3);
    tree->SetBranchAddress("parentID", &value4);

    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; i++)
    {
        value1->clear();
	value2->clear();
	value3->clear();
	value4->clear();
        tree->GetEntry(i);

	for (int i=0; i< value1->size(); i++)
	  {
            x.push_back(value1->at(i));
	    y.push_back(value2->at(i));
	    energy.push_back(value3->at(i));
	    parentID.push_back(value4->at(i));

	  }
    }

    // Clean up
    file->Close();

    return;
}


TH2F* Histo_2D(const char* name, int binX, float xmin, float xmax, int binY, float ymin, float ymax, std::vector<float> a, std::vector<float> b)
{
  TH2F* Plot_2D = new TH2F(name, name, binX, xmin, xmax, binY, ymin, ymax);
  
  for (int i=0; i<a.size(); i++)
    {
      Plot_2D->Fill(a.at(i), b.at(i));
    }

  return Plot_2D;
}


TH1F* Histo_1D(const char* name, int bin, float min, float max, std::vector<float> a)
{
  TH1F* Plot_1D = new TH1F(name, name, bin, min, max);
  
  for (int i=0; i<a.size(); i++)
    {
      Plot_1D->Fill(a.at(i));
    }

  return Plot_1D;
}


TH1F* Histo_Energy(const char* name, std::vector<float> energy, std::vector<float> parentID, int condition)
{
  TH1F* Plot_1D = new TH1F(name, name, 500, 0, 500);
  
  for (int i=0; i<energy.size(); i++)
    {
      if(condition ==0) // ALL
	{
	  Plot_1D->Fill(energy.at(i));
	}

      else if (condition ==1) //only primary electrons
	{
	  if(parentID.at(i)==0) Plot_1D->Fill(energy.at(i));
	}

      else if (condition ==2) // onlu noise (no primary electrons
	{
	  if (parentID.at(i)!=0) Plot_1D->Fill(energy.at(i));
	}
		  
    }

  return Plot_1D;
}



void Draw_Results(string filename, TH2F* xy, TH1F* energy_all, TH1F* energy_e, TH1F* energy_noise, TH2F* xenergy, TH2F* yenergy)
{
  TCanvas *c1 = new TCanvas("RESULTS", "RESULTS", 0, 0, 2500, 1900);
  TPad *pad1 = new TPad("pad1", "", 0, 0, 0.45, 0.45);
  TPad *pad2 = new TPad("pad2", "", 0.55, 0, 1, 0.45);
  TPad *pad3 = new TPad("pad3", "", 0, 0.55, 0.45, 1);
  TPad *pad4 = new TPad("pad4", "", 0.55, 0.55, 1, 1);
  TPad *pad5 = new TPad("pad5", "", 0.1, 0.46, 0.9, 0.54);


  pad1->Draw();
  pad1->SetGridx();
  pad1->SetGridy();
  pad2->Draw();
  pad2->SetGridx();
  pad2->SetGridy();
  pad2->SetLogz();
  pad3->Draw();
  pad3->SetGridx();
  pad3->SetGridy();
  pad3->SetLogz();
  pad4->Draw();
  pad4->SetGridx();
  pad4->SetGridy();
  pad5->Draw();

  pad1->cd();
  xy->Draw("colz");
  xy->SetTitle("");
  xy->GetXaxis()->SetTitle("X [mm]");
  xy->GetYaxis()->SetTitle("Y [mm]");
  pad1->SetLogz();
  TF2* fit2D = new TF2("fit2D", "xygaus", -5, 5, -5, 5);
  xy->Fit(fit2D, "RQN");
  fit2D->Draw("same");
  fit2D->SetContour(10);
  string   s = Form("#sigma_{x} = #bf{%.2f mm}", fit2D->GetParameter(2));
  TLatex *t0 = new TLatex(10, 14, s.c_str());
  t0->SetTextColor(kRed);
  t0->SetTextFont(43);
  t0->SetTextSize(25);
  t0->Draw("same");
  s = Form("#sigma_{y} = #bf{%.2f mm}", fit2D->GetParameter(4));
  TLatex *t0b = new TLatex(10, 12, s.c_str());
  t0b->SetTextColor(kRed);
  t0b->SetTextFont(43);
  t0b->SetTextSize(25);
  t0b->Draw("same");
  

  pad2->cd();
  energy_all->Draw();
  energy_all->SetTitle("");
  energy_all->GetXaxis()->SetTitle("Energy [MeV]");
  energy_all->GetYaxis()->SetTitle("N [MeV^{-1}]");
  energy_all->SetLineColor(kBlack);
  energy_all->SetLineWidth(2);
  TF1* fit = new TF1("fit", "gaus", 235, 260);
  energy_all->Fit(fit, "RQN");
  fit->SetLineColor(kBlue);
  fit->Draw("same");
  energy_e->Draw("same");
  energy_e->SetLineColor(kRed);
  energy_e->SetLineWidth(2);
  energy_noise->Draw("same");
  energy_noise->SetLineColor(kGreen);
  energy_noise->SetLineWidth(2);
  pad2->SetLogy();

  s = Form("All particles : #bf{%.1f %%}", float(energy_all->GetEntries()/7690745)*100);
  TLatex *t1 = new TLatex(290, 100000, s.c_str());
  t1->SetTextColor(kBlack);
  t1->SetTextFont(43);
  t1->SetTextSize(25);
  t1->Draw("same");

  s = Form("Mean = #bf{%.1f MeV}", fit->GetParameter(1));
  TLatex *tf1 = new TLatex(320, 60000, s.c_str());
  tf1->SetTextColor(kBlue);
  tf1->SetTextFont(43);
  tf1->SetTextSize(25);
  tf1->Draw("same");

  s = Form("#sigma = #bf{%.1f MeV}", fit->GetParameter(2));
  TLatex *tf2 = new TLatex(320, 40000, s.c_str());
  tf2->SetTextColor(kBlue);
  tf2->SetTextFont(43);
  tf2->SetTextSize(25);
  tf2->Draw("same");

  s = Form("Primary electrons : #bf{%.1f %%}", float(energy_e->GetEntries()/7690745)*100);
  TLatex *t2 = new TLatex(290, 20000, s.c_str());
  t2->SetTextColor(kRed);
  t2->SetTextFont(43);
  t2->SetTextSize(25);
  t2->Draw("same");

  s = Form("Noise : #bf{%.1f %%}", float(energy_noise->GetEntries()/7690745)*100);
  TLatex *t3 = new TLatex(290, 10000, s.c_str());
  t3->SetTextColor(kGreen);
  t3->SetTextFont(43);
  t3->SetTextSize(25);
  t3->Draw("same");

  s = Form("Noise generated / Primary e- : #bf{%.1f %%}", 100*(float(energy_noise->GetEntries()) / float(energy_e->GetEntries())));
  TLatex *t4 = new TLatex(290, 4000, s.c_str());
  t4->SetTextColor(kCyan);
  t4->SetTextFont(43);
  t4->SetTextSize(25);
  t4->Draw("same");

  pad3->cd();
  xenergy->Draw("colz");
  xenergy->SetTitle("");
  xenergy->GetXaxis()->SetTitle("X [mm]");
  xenergy->GetYaxis()->SetTitle("Energy [MeV]");
  pad3->SetLogz();

  pad4->cd();
  yenergy->Draw("colz");
  yenergy->SetTitle("");
  yenergy->GetXaxis()->SetTitle("Y [mm]");
  yenergy->GetYaxis()->SetTitle("Energy [MeV]");
  pad4->SetLogz();


  pad5->cd();
  TLatex *t5 = new TLatex(0, 0.5, filename.c_str());
  t5->SetTextColor(kBlack);
  t5->SetTextFont(43);
  t5->SetTextSize(40);
  t5->Draw();

  string fileout = "Images_Plots/" + filename + ".png";
  c1->SaveAs(fileout.c_str(), "RECREATE");
}


void Plot_2VD(string filename)
{
  gStyle->SetOptStat(0);
  std::vector<float> x;
  std::vector<float> y;
  std::vector<float> energy;
  std::vector<float> parentID;
  readAndAccessTreesInformations(filename.c_str(), x, y, energy, parentID);

  TH1F* Energy_all = Histo_Energy("Energy_ALL", energy, parentID, 0);
  TH1F* Energy_e = Histo_Energy("Energy_e", energy, parentID, 1);
  TH1F* Energy_noise = Histo_Energy("Energy_noise", energy, parentID, 2);
  TH2F* XY = Histo_2D("XY", 200, -15, 15, 200, -15, 15, x, y);
  TH2F* XEnergy = Histo_2D("XEnergy", 200, -15, 15, 500, 0, 500, x, energy);
  TH2F* YEnergy = Histo_2D("YEnergy", 200, -15, 15, 500, 0, 500, y, energy);

  Draw_Results(filename, XY, Energy_all, Energy_e, Energy_noise, XEnergy, YEnergy);

  
  
}
