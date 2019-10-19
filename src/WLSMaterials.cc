//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: WLSMaterials.cc 82854 2014-07-14 09:08:25Z gcosmo $
//
/// \file optical/wls/src/WLSMaterials.cc
/// \brief Implementation of the WLSMaterials class
//
//
#include "WLSMaterials.hh"

#include "G4SystemOfUnits.hh"

#include "parameter.hh"

WLSMaterials* WLSMaterials::fInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLSMaterials::WLSMaterials()
{
  fNistMan = G4NistManager::Instance();

  fNistMan->SetVerbose(2);

  CreateMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLSMaterials::~WLSMaterials()
{
  delete    fPMMA;
  delete    fPethylene;
  delete    fFPethylene;
  delete    fPolystyrene;
  delete    fSilicone;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLSMaterials* WLSMaterials::GetInstance()
{
  if (fInstance == 0)
    {
      fInstance = new WLSMaterials();
    }
  return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* WLSMaterials::GetMaterial(const G4String material)
{
  G4Material* mat =  fNistMan->FindOrBuildMaterial(material);

  if (!mat) mat = G4Material::GetMaterial(material);
  if (!mat) {
     std::ostringstream o;
     o << "Material " << material << " not found!";
     G4Exception("WLSMaterials::GetMaterial","",
                 FatalException,o.str().c_str());
  }

  return mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLSMaterials::CreateMaterials()
{
  G4double density;
  G4int ncomponents;
  G4double fractionmass;
  std::vector<G4int> natoms;
  std::vector<G4double> fractionMass;
  std::vector<G4String> elements;

  // Materials Definitions
  // =====================

  //--------------------------------------------------
  // Vacuum
  //--------------------------------------------------

  fNistMan->FindOrBuildMaterial("G4_Galactic");

  //--------------------------------------------------
  // Air
  //--------------------------------------------------

  fAir = fNistMan->FindOrBuildMaterial("G4_AIR");

  //--------------------------------------------------
  // WLSfiber PMMA
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(5);
  elements.push_back("H");     natoms.push_back(8);
  elements.push_back("O");     natoms.push_back(2);

  density = 1.190*g/cm3;

  fPMMA = fNistMan-> ConstructNewMaterial("PMMA", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Cladding (polyethylene)
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(2);
  elements.push_back("H");     natoms.push_back(4);

  density = 1.050*g/cm3;

  fPethylene = fNistMan->
          ConstructNewMaterial("Pethylene", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Double Cladding (fluorinated polyethylene)
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(2);
  elements.push_back("H");     natoms.push_back(4);

  density = 1.430*g/cm3;

  fFPethylene = fNistMan->
          ConstructNewMaterial("FPethylene", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Polystyrene
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(8);
  elements.push_back("H");     natoms.push_back(8);

  density = 1.050*g/cm3;

  fPolystyrene = fNistMan->ConstructNewMaterial("Polystyrene", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Silicone (Template for Optical Grease)
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(2);
  elements.push_back("H");     natoms.push_back(6);

  density = 1.060*g/cm3;

  fSilicone = fNistMan->
          ConstructNewMaterial("Silicone", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Aluminium
  //--------------------------------------------------

  fNistMan->FindOrBuildMaterial("G4_Al");

  //--------------------------------------------------
  // TiO2
  //--------------------------------------------------

  elements.push_back("Ti");     natoms.push_back(1);
  elements.push_back("O");      natoms.push_back(2);

  density     = 4.26*g/cm3;

  G4Material* TiO2 = fNistMan->
          ConstructNewMaterial("TiO2", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Scintillator Coating - 15% TiO2 and 85% polystyrene by weight.
  //--------------------------------------------------

  density = 1.52*g/cm3;

  fCoating = new G4Material("Coating", density, ncomponents=2);
  fCoating->AddMaterial(TiO2,         fractionmass = 15*perCent);
  fCoating->AddMaterial(fPolystyrene, fractionmass = 85*perCent);

  //
  // ------------ Generate & Add Material Properties Table ------------
  //
// [nm]
// 620      610.837     601.942     593.301  584.906     576.744     568.807  561.086     553.571     546.256
// 539.13   532.189     525.424     518.828  512.397     506.122     500      494.024     488.189     482.49
// 476.923  471.483     466.165     460.967  455.882     450.909     446.043  441.281     436.62      432.056
// 427.586  423.208     418.919     414.716  410.596     406.557     402.597  398.714     394.904     391.167
// 387.5    383.901     380.368     376.9    373.494     370.149     366.864  363.636     360.465     357.349
   G4double photonEnergy[] = {
   2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
   2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
   2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
   2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
   3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};
  	const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);

  	// ----- Air
  	G4double refractiveIndex[] =
  	{ 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};
  	assert(sizeof(refractiveIndex) == sizeof(photonEnergy));

  	G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
  	mpt->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);
  	fAir->SetMaterialPropertiesTable(mpt);

	// ----- Pethylene: WLS core information

	// references
   // http://kuraraypsf.jp/pdf/all.pdf
   // https://indico.cern.ch/event/143675/contributions/164201/
	// https://doi.org/10.1016/j.nima.2010.09.027

   G4double refractiveIndexWLSfiber[NSpectrum];
   for(int i=0; i<NSpectrum; i++)
   {
       refractiveIndexWLSfiber[i] = parameter::refractiveIndexWLSfiber[i];
   }
   assert(sizeof(refractiveIndexWLSfiber) == sizeof(photonEnergy));
/* //  default
   G4double absWLSfiber[] = {
   5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,
   5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,
   5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,1.10*m,
   1.10*m,1.10*m,1.10*m,1.10*m,1.10*m,1.10*m,1.0*mm,1.0*mm,1.0*mm,1.0*mm,
   1.0*mm,1.0*mm,1.0*mm,1.0*mm,1.0*mm,1.0*mm,1.0*mm,1.0*mm,1.0*mm,1.0*mm};
*/
   G4double absWLSfiber[NSpectrum];
   for(int i=0; i<NSpectrum; i++)
   {
       absWLSfiber[i] = parameter::absWLSfiber[i];
   }

   G4double emissionFib[NSpectrum];
   for(int i=0; i<NSpectrum; i++)
   {
       emissionFib[i] = parameter::emissionFib[i];
   }
   assert(sizeof(emissionFib) == sizeof(photonEnergy));

   // Add entries into properties table
   G4MaterialPropertiesTable* mptWLSfiber = new G4MaterialPropertiesTable();
   mptWLSfiber->AddProperty("RINDEX",photonEnergy,refractiveIndexWLSfiber,nEntries);
   //mptWLSfiber->AddProperty("ABSLENGTH",photonEnergy,absWLSfiber,nEntries);
   mptWLSfiber->AddProperty("WLSABSLENGTH",photonEnergy,absWLSfiber,nEntries);
   mptWLSfiber->AddProperty("WLSCOMPONENT",photonEnergy,emissionFib,nEntries);
   mptWLSfiber->AddConstProperty("WLSTIMECONSTANT", parameter::WLSTime);

   // ----- PMMA (inner claddig)
	//http://kuraraypsf.jp/pdf/all.pdf
   G4double refractiveIndexClad1[NSpectrum];
   G4double absClad[NSpectrum];
   for(int i=0; i<NSpectrum; i++)
   {
       refractiveIndexClad1[i] = parameter::refractiveIndexClad1[i];
       absClad[i] = parameter::absClad[i];
   }
   assert(sizeof(refractiveIndexClad1) == sizeof(photonEnergy));
   assert(sizeof(absClad) == sizeof(photonEnergy));

   // Add entries into properties table
   G4MaterialPropertiesTable* mptClad1 = new G4MaterialPropertiesTable();
   mptClad1->AddProperty("RINDEX",photonEnergy,refractiveIndexClad1,nEntries);
   mptClad1->AddProperty("ABSLENGTH",photonEnergy,absClad,nEntries);


   // ----- Fluorinated Polyethylene (outer cladding(FP))
	// http://kuraraypsf.jp/pdf/all.pdf
   G4double refractiveIndexClad2[NSpectrum];
   for(int i=0; i<NSpectrum; i++)
   {
       refractiveIndexClad2[i] = parameter::refractiveIndexClad2[i];
   }
   assert(sizeof(refractiveIndexClad2) == sizeof(photonEnergy));

   // Add entries into properties table
   G4MaterialPropertiesTable* mptClad2 = new G4MaterialPropertiesTable();
   mptClad2->AddProperty("RINDEX",photonEnergy,refractiveIndexClad2,nEntries);
   mptClad2->AddProperty("ABSLENGTH",photonEnergy,absClad,nEntries);

   // ----- set table
   #if 0 // original
   fPMMA      ->SetMaterialPropertiesTable(mptWLSfiber);
   fPethylene ->SetMaterialPropertiesTable(mptClad1);
   fFPethylene->SetMaterialPropertiesTable(mptClad2);
   #else // CAUTION: this is coreect, check used materials
   fPethylene ->SetMaterialPropertiesTable(mptWLSfiber); // fPethylene is "core"
   fPMMA      ->SetMaterialPropertiesTable(mptClad1);    // fPMMA inner cladding
   fFPethylene->SetMaterialPropertiesTable(mptClad2);    // fFPethylene outer cladding
   #endif



  //--------------------------------------------------
  // Silicone
  //--------------------------------------------------
   G4double refractiveIndexSilicone[] =
   { 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46};
   assert(sizeof(refractiveIndexSilicone) == sizeof(photonEnergy));

  // Add entries into properties table
  G4MaterialPropertiesTable* mptSilicone = new G4MaterialPropertiesTable();
  mptSilicone->AddProperty("RINDEX",photonEnergy,refractiveIndexSilicone,nEntries);
  mptSilicone->AddProperty("ABSLENGTH",photonEnergy,absClad,nEntries);

  fSilicone->SetMaterialPropertiesTable(mptSilicone);

   // ----- Polystyrene / scintillator
// 620      610.837     601.942     593.301  584.906     576.744     568.807  561.086     553.571     546.256
// 539.13   532.189     525.424     518.828  512.397     506.122     500      494.024     488.189     482.49
// 476.923  471.483     466.165     460.967  455.882     450.909     446.043  441.281     436.62      432.056
// 427.586  423.208     418.919     414.716  410.596     406.557     402.597  398.714     394.904     391.167
// 387.5    383.901     380.368     376.9    373.494     370.149     366.864  363.636     360.465     357.349
  	G4double refractiveIndexPS[] =
  	{1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50};
  	assert(sizeof(refractiveIndexPS) == sizeof(photonEnergy));
 	G4double absPS[NSpectrum];
    for(int i=0;i<NSpectrum; i++)
    {
        absPS[i] = parameter::absPS[i];
    }
  	assert(sizeof(absPS) == sizeof(photonEnergy));
	G4double scintilFast[NSpectrum];
    for(int i=0; i<NSpectrum; i++)
    {
        scintilFast[i] = parameter::scintilFast[i];
    }
   assert(sizeof(scintilFast) == sizeof(photonEnergy));

  	// Add entries into properties table
#if 0 // default
  	G4MaterialPropertiesTable* mptPolystyrene = new G4MaterialPropertiesTable();
  	mptPolystyrene->AddProperty("RINDEX",photonEnergy,refractiveIndexPS,nEntries);
  	mptPolystyrene->AddProperty("ABSLENGTH",photonEnergy,absPS,nEntries);
  	mptPolystyrene->AddProperty("FASTCOMPONENT",photonEnergy, scintilFast,nEntries);
  	mptPolystyrene->AddConstProperty("SCINTILLATIONYIELD",50./keV);
  	mptPolystyrene->AddConstProperty("RESOLUTIONSCALE",1.0);
  	mptPolystyrene->AddConstProperty("FASTTIMECONSTANT",10.*ns);
#endif
#if 1
   G4MaterialPropertiesTable* mptPolystyrene = new G4MaterialPropertiesTable();
   mptPolystyrene->AddProperty("RINDEX",photonEnergy,refractiveIndexPS,nEntries);
   mptPolystyrene->AddProperty("ABSLENGTH",photonEnergy,absPS,nEntries);
   mptPolystyrene->AddProperty("FASTCOMPONENT",photonEnergy, scintilFast,nEntries);

   mptPolystyrene->AddConstProperty("EFFICIENCY",1);
   mptPolystyrene->AddConstProperty("SCINTILLATIONYIELD", parameter::scintiLY); // typical pla-scinti ~ 10,000/MeV
   //mptPolystyrene->AddConstProperty("SCINTILLATIONYIELD",50./keV);
   mptPolystyrene->AddConstProperty("RESOLUTIONSCALE",1.0);
   mptPolystyrene->AddConstProperty("FASTTIMECONSTANT", parameter::scintiTime);
   //mptPolystyrene->AddConstProperty("SLOWTIMECONSTANT",5.*ns);
#endif
   fPolystyrene->SetMaterialPropertiesTable(mptPolystyrene);

  	// Set the Birks Constant for the Polystyrene scintillator
  	fPolystyrene->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
}
