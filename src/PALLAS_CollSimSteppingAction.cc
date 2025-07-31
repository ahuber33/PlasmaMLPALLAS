/// PALLAS_CollSimSteppingAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimSteppingAction.hh"
#include "G4GenericMessenger.hh"

using namespace CLHEP;

const G4String PALLAS_CollSimSteppingAction::path = "../simulation_input_files/";

PALLAS_CollSimSteppingAction::PALLAS_CollSimSteppingAction()
{
    sMessenger = new G4GenericMessenger(this, "/step/", "Control commands for my application");

    sMessenger->DeclareProperty("SetTrackingStatus", TrackingStatus)
    .SetGuidance("Set the boolean parameter.")
    .SetParameterName("TrackingStatus", false)
    .SetDefaultValue("true");
}

PALLAS_CollSimSteppingAction::~PALLAS_CollSimSteppingAction()
{
    delete sMessenger;
}

bool PALLAS_CollSimSteppingAction::SetFlagGammaEnergyDeposition()
{
    flag_Gamma = (particleID == 22 || creatorProcessName == "phot" || creatorProcessName == "compt" || creatorProcessName == "conv");

    return flag_Gamma;
}

void PALLAS_CollSimSteppingAction::ResetFlagColl(PALLAS_CollSimEventAction *evtac)
{
    evtac->ResetFlagHorizontalColl();
    evtac->ResetFlagVerticalColl();
}


void PALLAS_CollSimSteppingAction::GetInputInformations(PALLAS_CollSimEventAction *evtac)
{
    //evtac->SetEstartCollimator(energy);
    evtac->SetXStart(x);
    evtac->SetXOffsetStart(-0.152);
    evtac->SetXpStart(px);
    evtac->SetYStart(z);
    evtac->SetYOffsetStart(0.08);
    evtac->SetYpStart(pz);
    evtac->SetSStart(y);
    evtac->SetSOffsetStart(3114.5);
    evtac->SetPStart(py);
    evtac->SetEnergyStart(energy);
    evtac->SetDeltaStart(energy-247);
    evtac->AddNeventStart();
    // G4cout << "x = " << x << G4endl;
    // G4cout << "y = " << y << G4endl;
    // G4cout << "z = " << z << G4endl;
    // G4cout << "px = " << px << G4endl;
    // G4cout << "py = " << py << G4endl;
    // G4cout << "pz = " << pz << G4endl;
    // G4cout << "energy = " << energy << G4endl;
    // G4cout << "Nevent = " << evtac->GetNeventStart() << G4endl;
    if (TrackingStatus ==false) theTrack->SetTrackStatus(fStopAndKill);
}


void PALLAS_CollSimSteppingAction::UpdateHorizontalCollInformations(PALLAS_CollSimEventAction *evtac)
{
    evtac->ActiveFlagHorizontalColl();
    evtac->AddParentIDHorizontalColl(parentID);
    evtac->AddParticleIDHorizontalColl(particleID);
    evtac->AddEnergyHorizontalColl(energy);

//  G4cout << "ParentID = " << parentID << G4endl;
//  G4cout << "Particle ID = " << particleID << G4endl;
//  G4cout << "energy = " << energy << G4endl;
}


void PALLAS_CollSimSteppingAction::UpdateVerticalCollInformations(PALLAS_CollSimEventAction *evtac)
{
    evtac->ActiveFlagVerticalColl();
    evtac->AddParentIDVerticalColl(parentID);
    evtac->AddParticleIDVerticalColl(particleID);
    evtac->AddEnergyVerticalColl(energy);

//  G4cout << "ParentID = " << parentID << G4endl;
//  G4cout << "Particle ID = " << particleID << G4endl;
}

void PALLAS_CollSimSteppingAction::UpdateBSYAGInformations(PALLAS_CollSimEventAction *evtac)
{
    if(evtac->ReturnFlagBSYAG() == false)
    {
        evtac->AddXExitBSYAG(xpost);
        evtac->AddYExitBSYAG(ypost);
        evtac->AddZExitBSYAG(zpost);
        evtac->AddParentIDBSYAG(parentID);
        evtac->AddParticleIDBSYAG(particleID);
        evtac->AddEnergyBSYAG(energy);
        evtac->ActiveFlagBSYAG();

        //SG4cout << "ParentID = " << parentID << G4endl;
        //G4cout << "TrackID = " << trackID << G4endl;
        //G4cout << "x = " << x << G4endl;
        //G4cout << "y = " << y << G4endl;
        //G4cout << "z = " << z << G4endl;
        //G4cout << "energy = " << energy << G4endl;
    }

    evtac->AddDepositedEnergyBSYAG(energyDeposited);

    //G4cout << "energy dep = " << energyDeposited << G4endl;
    //G4cout << "energy dep TOT = " << evtac->GetDepositedEnergyBSYAG() << G4endl;


    if (volumeNamePostStep == "Holder" || (energy-energyDeposited) ==0)
    {
        evtac->AddTotalDepositedEnergyBSYAG(evtac->GetDepositedEnergyBSYAG());
        //G4cout << "Chgmt de track => Edep particule précédente = " << evtac->GetDepositedEnergyBSYAG() << G4endl;
        evtac->ResetDepositedEnergyBSYAG();
        evtac->ResetFlagBSYAG();
    }

    if (TrackingStatus ==false) theTrack->SetTrackStatus(fStopAndKill);
}

void PALLAS_CollSimSteppingAction::UpdateBSPECYAGInformations(PALLAS_CollSimEventAction *evtac)
{
    if(evtac->ReturnFlagBSPECYAG() == false)
    {
        evtac->AddXExitBSPECYAG(xpost);
        evtac->AddYExitBSPECYAG(ypost);
        evtac->AddZExitBSPECYAG(zpost);
        evtac->AddParentIDBSPECYAG(parentID);
        evtac->AddParticleIDBSPECYAG(particleID);
        evtac->AddEnergyBSPECYAG(energy);
        evtac->ActiveFlagBSPECYAG();

        //SG4cout << "ParentID = " << parentID << G4endl;
        //G4cout << "TrackID = " << trackID << G4endl;
        // G4cout << "x = " << x << G4endl;
        // G4cout << "y = " << y << G4endl;
        // G4cout << "z = " << z << G4endl;
        //G4cout << "energy = " << energy << G4endl;
    }

    evtac->AddDepositedEnergyBSPECYAG(energyDeposited);

    //G4cout << "energy dep = " << energyDeposited << G4endl;
    //G4cout << "energy dep TOT = " << evtac->GetDepositedEnergyBSPECYAG() << G4endl;


    if (volumeNamePostStep == "Holder" || (energy-energyDeposited) ==0)
    {
        evtac->AddTotalDepositedEnergyBSPECYAG(evtac->GetDepositedEnergyBSPECYAG());
        //G4cout << "Chgmt de track => Edep particule précédente = " << evtac->GetDepositedEnergyBSPECYAG() << G4endl;
        evtac->ResetDepositedEnergyBSPECYAG();
        evtac->ResetFlagBSPECYAG();
    }


    if (TrackingStatus ==false) theTrack->SetTrackStatus(fStopAndKill);
}


void PALLAS_CollSimSteppingAction::UserSteppingAction(const G4Step *aStep)
{

    // ###################################
    //  Déclaration of functions/variables
    // ###################################
    PALLAS_CollSimRunAction *runac = (PALLAS_CollSimRunAction *)(G4RunManager::GetRunManager()->GetUserRunAction());
    G4EventManager *evtman = G4EventManager::GetEventManager();
    PALLAS_CollSimEventAction *evtac = (PALLAS_CollSimEventAction *)evtman->GetUserEventAction();

    theTrack = aStep->GetTrack();
    particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
    particleID = aStep->GetTrack()->GetDefinition()->GetPDGEncoding(); // PART ID => https://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf
    endproc = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    auto creatorProcess = theTrack->GetCreatorProcess();
    parentID = aStep->GetTrack()->GetParentID();
    trackID = aStep->GetTrack()->GetTrackID();
    stepNo = aStep->GetTrack()->GetCurrentStepNumber();
    energy = aStep->GetPreStepPoint()->GetKineticEnergy() / MeV;
    volumeNamePreStep = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    volumeNamePostStep = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();

    energyDeposited = aStep->GetTotalEnergyDeposit() / keV;
    x = aStep->GetPreStepPoint()->GetPosition().x()/mm;
    y = aStep->GetPreStepPoint()->GetPosition().y()/mm;
    z = aStep->GetPreStepPoint()->GetPosition().z()/mm;
    xpost = aStep->GetPostStepPoint()->GetPosition().x()/mm;
    ypost = aStep->GetPostStepPoint()->GetPosition().y()/mm;
    zpost = aStep->GetPostStepPoint()->GetPosition().z()/mm;
    px = aStep->GetPreStepPoint()->GetMomentumDirection().x();
    py = aStep->GetPreStepPoint()->GetMomentumDirection().y();
    pz = aStep->GetPreStepPoint()->GetMomentumDirection().z();


    // // #######################################################################
    // // #######################################################################
    // // ###########################START EM INFOS PART#########################
    // // #######################################################################
    // // #######################################################################

    if (parentID == 0 && stepNo == 1) GetInputInformations(evtac);

    if (stepNo ==1) ResetFlagColl(evtac);



    if (creatorProcess != NULL)
    {
        creatorProcessName = creatorProcess->GetProcessName();
        SetFlagGammaEnergyDeposition();
        //G4cout << "Creator process = " << creatorProcessName << G4endl;
    }


    if(volumeNamePreStep == "HorizontalCollimator")
    {
        evtac->AddEdepHorizontalColl(energyDeposited);
        //G4cout << "energy dep = " << energyDeposited << G4endl;
        //G4cout << "energy dep tot = " << evtac->GetEdepHorizontalColl()<< G4endl;

        if(volumeNamePostStep != "HorizontalCollimator" && evtac->ReturnFlagHorizontalColl() == false && evtac->ReturnFlagVerticalColl() == false)
        {
            //UpdateHorizontalCollInformations(evtac);
        }
    }



    if(volumeNamePreStep == "VerticalCollimator")
    {
        evtac->AddEdepVerticalColl(energyDeposited);

        if(volumeNamePostStep != "VerticalCollimator" && evtac->ReturnFlagVerticalColl() == false && evtac->ReturnFlagHorizontalColl() == false)
        {
            //UpdateVerticalCollInformations(evtac);
        }
    }



    if (volumeNamePostStep == "World")
        theTrack->SetTrackStatus(fStopAndKill);

    if (volumeNamePreStep == "BS1_YAG")
    {
        UpdateBSYAGInformations(evtac);
        //theTrack->SetTrackStatus(fStopAndKill);
    }

    if (volumeNamePreStep == "BSPEC1_YAG")
    {
        UpdateBSPECYAGInformations(evtac);
        //theTrack->SetTrackStatus(fStopAndKill);
    }


    // G4cout << "x = " << x << G4endl;
    // G4cout << "y = " << y << G4endl;
    // G4cout << "z = " << z << G4endl;


    //G4cout << " y =" << y << G4endl;

    // if (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "Collimator")
    //   theTrack->SetTrackStatus(fStopAndKill);

    //if (parentID>0) theTrack->SetTrackStatus(fStopAndKill);
}