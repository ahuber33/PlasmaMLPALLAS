#include <iostream>
#include <fstream>
#include <sstream>
#include <TFile.h>
#include <TNtuple.h>
#include <TCanvas.h>

Double_t Test(Double_t *x, Double_t *par) {
  double x0 = par[1];
  double amp_e= par[2];
  double x0_e = par[3];
  double sigma_e = par[4];
  double amp_g= par[5];
  double x0_g = par[6];
  double sigma_g = par[7];

  double stepF=0;
  double gausF=0;

  if(x[0] <x0)
    {
      stepF=0;
      gausF = amp_g * TMath::Exp(-0.5 * TMath::Power((x[0] - x0_g)/sigma_g, 2));
    }

  else
    {
      stepF = amp_e*(TMath::Erf((2*x0_e - x[0]) / sigma_e));
      gausF=0;
    }

  double combined = stepF + gausF;

  return combined;
}



Double_t Fit(Double_t *x, Double_t *par) {
  double x0 = par[0];
  double amp_e= par[1];
  double x0_e = par[2];
  double sigma_e = par[3];
  double amp_g= par[4];
  double x0_g = par[5];
  double sigma_g = par[6];

  double stepF=0;
  double gausF=0;

  if(x[0] <x0)
    {
      stepF=amp_e*(TMath::Erf((x[0]-x0_e) / sigma_e));
      gausF = 0;
    }

  else
    {
      stepF = 0;
      gausF=amp_g * TMath::Exp(-0.5 * TMath::Power((x[0] - x0_g)/sigma_g, 2));
    }

  double combined = stepF + gausF;

  return combined;
}



double symmetrizedFunction(double *x, double *par) {
    double x0 =0; // Point de symétrie
    double f_x=0;
    double f_symmetric=0;
    
    if (x[0] < x0)
      {
	f_x = 0;
	f_symmetric = Fit(new double[1]{2 * x0 - x[0]}, par);
      }

    else
      {
	f_x = Fit(x, par);
	f_symmetric =0;
      }

    return (f_x + f_symmetric);
}


double symmetrizedFunctionS(double *x, double *par) {
    double x0 =3.4495; // Point de symétrie
    double f_x=0;
    double f_symmetric=0;
    
    if (x[0] < x0)
      {
	f_x = 0;
	f_symmetric = Fit(new double[1]{2 * x0 - x[0]}, par);
      }

    else
      {
	f_x = Fit(x, par);
	f_symmetric =0;
      }

    return (f_x + f_symmetric);
}

int Plot_BField() {
    // Nom du fichier à lire
    std::string filename = "XYZB_Dipole_FIELD_vf.txt";

    // Ouverture du fichier
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Erreur d'ouverture du fichier " << filename << std::endl;
        return 1;
    }

    // Création d'un fichier ROOT pour stocker les données
    TFile *rootFile = new TFile("output.root", "RECREATE");

    // Création du TNtuple
    TNtuple *ntuple = new TNtuple("ntuple", "XYZB Data", "x:y:z:b");
    TGraph2D* graph = new TGraph2D();
    TGraph* graph1D = new TGraph();

    // Variables pour stocker les valeurs lues
    float x, y, z, b;

    // Lecture du fichier ligne par ligne
    std::string line;
    int point=0;
    int point1D=0;
    while (std::getline(infile, line)) {
        // Utilisation d'un stringstream pour extraire les valeurs
        std::stringstream ss(line);
        ss >> x >> y >> z >> b;
	if(y>-1.15 && y<-1.14)
	  {
	    graph1D->SetPoint(point1D, z, b);
	    point1D++;
	  }
	graph->SetPoint(point, y, z, b);
	point++;
        
        // Remplissage du TNtuple
        ntuple->Fill(x, y, z, b);
    }

    //     ntuple->Draw("b:z>>", "y>-1.14 && y<-1.13", "");
    //graph1D->Draw();

    //graph1D->Fit("fitFunc", "R");

    //fitFunc->Draw("same");
    //fitFunc->SetLineColor(kCyan);
    

    // Écriture et fermeture du fichier ROOT
    ntuple->Write();
    rootFile->Close();

    // Fermeture du fichier texte
    infile.close();

    std::cout << "Lecture du fichier et stockage des données dans un TNtuple terminées avec succès." << std::endl;

    // Affichage des données avec ROOT
    TCanvas *canvas = new TCanvas("canvas", "XYZB Data", 800, 600);
    graph->Draw("colz");

    

    TF1 *fitFuncY = new TF1("fitFuncY", symmetrizedFunction, -0.15, 0.15, 7);
    fitFuncY->FixParameter(0, -1.05579 +1.15);
    fitFuncY->FixParameter(1, -0.985);
    fitFuncY->FixParameter(2, -1.03649+1.15);
    fitFuncY->FixParameter(3, 0.0307999);
    fitFuncY->FixParameter(4, 721.501);
    fitFuncY->FixParameter(5, -1.58778+1.15);
    fitFuncY->FixParameter(6, 0.141887);

    TF1 *fitFuncS = new TF1("fitFuncS", symmetrizedFunctionS, 3.0995, 3.7995, 7);
    fitFuncS->FixParameter(0, 0.169992 +3.4495);
    fitFuncS->FixParameter(1, -0.806796);
    fitFuncS->FixParameter(2, 0.193481 + 3.4495);
    fitFuncS->FixParameter(3, 0.0405178);
    fitFuncS->FixParameter(4, 1.9817);
    fitFuncS->FixParameter(5, 0.0119007 + 3.4495);
    fitFuncS->FixParameter(6, 0.0946281);

    new TCanvas;
    fitFuncS->Draw();
    //    fitFuncY->Draw("same");

    cout << "2D = " << graph->Interpolate(-1.15, 0.2) << endl;
    cout << "Fit Y = " << fitFuncY->Eval(0) << endl;
    cout << "Fit S = " << fitFuncS->Eval(-0.3) << endl;
    cout << "Value Fit = " << fitFuncY->Eval(0) * fitFuncS->Eval(-0.2) << endl;


    return 0;
}
