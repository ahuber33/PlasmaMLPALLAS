void readAndAccessMacroCharge(const char *filename, std::vector<float>& macroCharge)
{
    std::ifstream infile(filename);
    if (!infile)
    {
        cerr << "Could not open file " << filename << endl;
        return;
    }

    double macrocharge;

    std::string line;

    while (std::getline(infile, line))
    {
        if (line[0] == '#')
            continue; // Skip comments
        std::istringstream iss(line);
        iss >> macrocharge;
        macroCharge.push_back(static_cast<float>(macrocharge));
    }

    infile.close();

return;
}

void readAndAccessTreesInformations(const char *filename, std::vector<float>& dataSim)
{
    // Open the ROOT file
    TFile *file = TFile::Open(filename);
    if (!file || file->IsZombie())
    {
        printf("Error opening file %s\n", filename);
        return ;
    }

    // Get the two trees
    TTree *treeFront = (TTree *)file->Get("FrontCollimator");

    // Variables to hold data
    std::vector<float> *value2 = 0;

    // Set branch addresses
    treeFront->SetBranchAddress("E_exit", &value2);

    Long64_t nEntries = treeFront->GetEntries();
    for (Long64_t i = 0; i < nEntries; i++)
    {
        value2->clear();
        treeFront->GetEntry(i);
        if (value2->size() == 0)
        {
            dataSim.push_back(0.0);
            //cout << " i = " << i << endl;
            // cout << "N vector    " << value2->size() << endl;
        }
        else
        {
            dataSim.push_back(value2->at(0));
            // cout << " i = " << i << endl;
            // cout << "N vector    " << value2->size() << endl;
            // cout << "value = " << value2->at(0) << endl;
        }
    }

    // Clean up
    file->Close();

    return;
}


double DetermineMacroChargeConfiguration(std::vector<float>& macro_charge, std::vector<float>& data_sim)
{
    double TotalMacroCharge=0;

    for(int i=0; i<macro_charge.size(); i++)
    {
        if(data_sim.at(i) != 0.0) TotalMacroCharge +=macro_charge.at(i);
    }

    return TotalMacroCharge;
}

double DetermineMacroChargeMax(std::vector<float>& macro_charge)
{
    double MacroCharge=0;

    for(int i=0; i<macro_charge.size(); i++)
    {
        MacroCharge +=macro_charge.at(i);
    }

    return MacroCharge;
}

std::string floatToStringWithPrecision(float value, int precision) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

void Plot_efficacite_epaisseur_radius()
{
    std::vector<float> macroCharge;
    std::vector<float> dataSim;

    readAndAccessMacroCharge("../bin/PALLAS_qm1411_preCollimateur.txt", macroCharge);
    double MacroChargeMax= DetermineMacroChargeMax(macroCharge);;
    double TotalMacroCharge;
    double fraction;
    double x=1;
    double y=1;

    std::string filename;
    TH2D* Plot = new TH2D("Plot", "Plot", 30, 5, 305, 30, -0.05, 3.05);
    //TH2D* Plot = new TH2D("Plot", "Plot", 25, 0.1, 5.1, 15, 5, 155);

    for (int i =10; i<=300; i+=10)
    {
        y=1;
        for (float j=0.1; j<=3; j+=0.1)
        {
            filename = "Study_VerticalConfiguration_epaisseur_ecartement/VerticalConfiguration_epaisseur_" + std::to_string(i) + "_ecartement_" + floatToStringWithPrecision(j, 1) + ".root";
            cout << "\nfilename = " << filename << endl;

            readAndAccessTreesInformations(filename.c_str(), dataSim);
            TotalMacroCharge = DetermineMacroChargeConfiguration(macroCharge, dataSim);
            fraction = TotalMacroCharge/MacroChargeMax;

            cout << "MacroChargeTotal = " << TotalMacroCharge << endl;
            cout << "MacroChargeMax = " << MacroChargeMax << endl;
            cout << fraction*100 << " % of max charge" << endl;
            cout << "x = " << x << endl;
            cout << "y = " << y << endl;

            Plot->SetBinContent(x, y, fraction);
            //Plot->SetBinContent(y, x, fraction);
            y++;
            dataSim.clear();
        }
        x++;
    }

    Plot->Draw("colz");
    Plot->SaveAs("Plot_efficacite_epaisseur_radius.root");
}
