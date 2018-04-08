#ifndef SKFlatMaker_H
#define SKFlatMaker_H

////////////////////////////////
// -- system include files -- //
////////////////////////////////
#include <memory>
#include <iostream>

//////////////////////
// -- FrameWorks -- //
//////////////////////
#include "FWCore/Framework/interface/Frameworkfwd.h" // -- Forward declarations of types in the EDM(Event Data Model).
#include "FWCore/Framework/interface/EDAnalyzer.h" // -- EDAnalyzer is the base class for all analyzer "modules".
#include "FWCore/Framework/interface/Event.h" // -- This is the primary interface for accessing EDProducts from a single collision and inserting new derived products.
#include "FWCore/Framework/interface/MakerMacros.h" // -- including temporary until a better solution can be found (leads to more physical coupling than is probably necessary)
#include "FWCore/ParameterSet/interface/ParameterSet.h" // -- Declaration for ParameterSet(parameter set) and related types 
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h" // -- Foward declarations of variables for ParameterSet
#include "FWCore/Framework/interface/ESHandle.h" // -- No description
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RegexMatch.h"


/////////////////////
// -- For Muons -- //
/////////////////////
#include "DataFormats/MuonReco/interface/MuonFwd.h" // -- Forward declarations of muon variables
#include "DataFormats/MuonReco/interface/Muon.h" // -- A reconstructed Muon. (tracker alone, muon detector alone, combined muon plus tracker)
#include "DataFormats/PatCandidates/interface/Muon.h" // -- Analysis-level muon class, pat::Muon implements the analysis-level muon class within the 'pat' namespace. PAT(Physics Analysis Toolkit)
#include "DataFormats/MuonReco/interface/MuonTimeExtra.h"
#include "DataFormats/MuonReco/interface/MuonTimeExtraMap.h"
#include "DataFormats/MuonReco/interface/MuonCocktails.h"
#include "DataFormats/MuonReco/interface/MuonCosmicCompatibility.h" // -- Variables shows how muon is cosmic-like


/////////////////////////
// -- For Electrons -- //
/////////////////////////
#include "DataFormats/PatCandidates/interface/Photon.h" // -- Analysis-level Photon class, pat::Photon implements the analysis-level photon class within the 'pat' namespace
#include "DataFormats/PatCandidates/interface/Electron.h" // -- Analysis-level electron class,  pat::Electron implements the analysis-level electron class within the 'pat' namespace
#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h" // -- It seems as a function refer effective areas of ECAL
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"


///////////////////
// -- For MET -- //
///////////////////
#include "DataFormats/PatCandidates/interface/MET.h" // -- Analysis-level MET class, pat::MET implements an analysis-level missing energy class as a 4-vector within the 'pat' namespace
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/PFMET.h" // --  MET made from Particle Flow candidates, with various fraction functions ex)photonEtFraction()
#include "DataFormats/METReco/interface/PFMETCollection.h"

////////////////////
// -- For Jets -- //
////////////////////
#include "DataFormats/PatCandidates/interface/Jet.h" // -- Analysis-level calorimeter jet class, Jet implements the analysis-level calorimeter jet class within the 'pat' namespace.

////////////////////////////
// -- For GenParticles -- //
////////////////////////////
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h" // -- LHE info like PDF
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h" // -- ??
#include "DataFormats/HepMCCandidate/interface/GenParticle.h" // -- 
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h" // -- ???
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" // -- contains information related to the details of the pileup simulation for a given event, ex) Nvtx, z of PU, sum(pt) of vtxs


//#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

//////////////////////////
// -- Track & Vertex -- //
//////////////////////////
#include "DataFormats/TrackReco/interface/TrackFwd.h" // -- Forward definitions for tracker variables
#include "DataFormats/TrackReco/interface/Track.h" // -- reconstructed tracks that are stored in the AOD and RECO. also contains a reference to more detailed information(RECO)
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h" // -- A composite Candidate  with error matrix and other vertex fix information. 
#include "DataFormats/Candidate/interface/VertexCompositeCandidateFwd.h" // -- Foward declaration of VertexCompositeCandidate class's variables
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h" // -- Least-squares vertex fitter implemented in the Kalman Filter formalism
#include "PhysicsTools/RecoUtils/interface/CandCommonVertexFitter.h" // -- 
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h" // -- Helper class to build TransientTrack from the persistent Track.
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "DataFormats/PatCandidates/interface/Vertexing.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexTools/interface/InvariantMassFromVertex.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

////////////////////
// -- Triggers -- //
////////////////////
#include "FWCore/Common/interface/TriggerNames.h" // -- Used to access the names and indices of the triggers corresponding to a particular TriggerResults object
#include "FWCore/Common/interface/TriggerResultsByName.h" // --  Class which provides methods to access trigger results
#include "DataFormats/Common/interface/TriggerResults.h" // -- The trigger path results are maintained here as a sequence of entries, one per trigger path
#include "DataFormats/HLTReco/interface/TriggerEvent.h" // -- The single EDProduct to be saved for each event (AOD case) describing the (HLT) trigger table
#include "DataFormats/HLTReco/interface/TriggerObject.h" // --  A single trigger object (e.g., an isolated muon, or MET) described by its 4-momentum and physics type
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h" // -- Analysis-level trigger object class (stand-alone). (within the 'pat' namespace.)
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h" // -- This class provides access routines to get hold of the HLT Configuration


////////////////
// -- Else -- //
////////////////
#include "DataFormats/Math/interface/LorentzVector.h" // -- (pt, eta, phi, E) OR (pt, eta, phi, M)
#include "DataFormats/GeometrySurface/interface/GloballyPositioned.h" // -- Base class for surfaces and volumes positioned in global 3D space.
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h" // -- The Muon DetUnit geometry.
#include "DataFormats/Common/interface/RefToBaseVector.h" // -- 
#include "DataFormats/Candidate/interface/NamedCompositeCandidateFwd.h" // -- Foward definition of NamedCompositeCandidate class
#include "DataFormats/Candidate/interface/NamedCompositeCandidate.h" // -- A Candidate composed of daughters. The daughters are owned by the composite candidate.
#include "DataFormats/Candidate/interface/CompositeCandidateFwd.h" // -- Foward definition of CompositeCandidate class
#include "DataFormats/Candidate/interface/CompositeCandidate.h" // A Candidate composed of daughters. The daughters are owned by the composite candidate.
#include "DataFormats/Common/interface/View.h" // -- Provide access to the collected elements contained by any WrapperBase that is a sequence.
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h" // -- 
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h" // -- Class to provide lumi weighting for analyzers to weight "flat-to-N" MC samples to data
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" // -- contains information related to the details of the pileup simulation for a given event
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositFwd.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositVetos.h"
#include "PhysicsTools/IsolationAlgos/interface/IsoDepositVetoFactory.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositDirection.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "MagneticField/Engine/interface/MagneticField.h"


////////////////
// -- ROOT -- //
////////////////
#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TFrame.h>
#include <TMath.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TF1.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <boost/foreach.hpp>

using namespace std;
using namespace pat;
using namespace edm;

namespace reco { class CandCommonVertexFitterBase; class VertexCompositeCandidate; class CandCommonVertexFitter; }

class SKFlatMaker : public edm::EDAnalyzer
{
 public:
  explicit SKFlatMaker(const edm::ParameterSet&);
  ~SKFlatMaker();
  
 private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void beginRun(const edm::Run &, const edm::EventSetup & );
  virtual void endRun(const edm::Run &, const edm::EventSetup & );
  
  virtual void fillPrimaryVertex(const edm::Event &iEvent);  // fill primary vertex information
  virtual void fillMET(const edm::Event &iEvent);            // fill MET information
  virtual void fillPhotons(const edm::Event &iEvent);
  virtual void fillMuons(const edm::Event &iEvent, const edm::EventSetup& iSetup);
  virtual void fillElectrons(const edm::Event &iEvent, const edm::EventSetup& iSetup);
  virtual void fillJet(const edm::Event &iEvent);            // fill jet and b-tagging information
  virtual void hltReport(const edm::Event &iEvent);          // fill list of triggers fired in an event
  virtual void fillLHEInfo(const edm::Event &iEvent);
  virtual void fillGENInfo(const edm::Event &iEvent);            // fill MET information
  virtual void fillTT(const edm::Event&);
  
  bool reorder(double &a, double &b)
  {
    return a > b;
  }
  
  int theDebugLevel;                   // 0 no prints, 1 some, 2 lots
  std::string processName;
  std::string theElectronID;
  
  HLTConfigProvider hltConfig_;
  
  // -- Tokens (for 76X) -- //
  edm::EDGetTokenT< std::vector<pat::Muon> >             MuonToken;
  edm::EDGetTokenT< edm::View<pat::Electron> >                         ElectronToken;
  edm::EDGetTokenT< edm::View<pat::Electron> >                                          UnCorrElectronToken;
  edm::EDGetTokenT< edm::View<pat::Photon> >                   PhotonToken;
  edm::EDGetTokenT< edm::View<pat::Photon> >                                            UnCorrPhotonToken;
  edm::EDGetTokenT< std::vector<pat::Jet> >             JetToken;
  edm::EDGetTokenT< std::vector<pat::MET> >             MetToken;
  //edm::EDGetTokenT<pat::METCollection>                                           MetToken;

  edm::EDGetTokenT< LHEEventProduct >               LHEEventProductToken;
  edm::EDGetTokenT< LHERunInfoProduct >              LHERunInfoProductToken;
  edm::EDGetTokenT< reco::GenParticleCollection>                                        mcLabel_;
  
  edm::EDGetTokenT< edm::TriggerResults >                                               METFilterResultsToken_PAT;
  edm::EDGetTokenT< edm::TriggerResults >                                               METFilterResultsToken_RECO;
  
  edm::EDGetTokenT< double >                      RhoToken;
  edm::EDGetTokenT< edm::ValueMap<float> >                                              mvaIsoValuesMapToken;
  edm::EDGetTokenT< edm::ValueMap<float> >                                              mvaNoIsoValuesMapToken;
  edm::EDGetTokenT< std::vector<reco::Conversion> > 		           		ConversionsToken;
  edm::EDGetTokenT< std::vector< reco::GsfTrack > > 			         	GsfTrackToken;
  
  edm::EDGetTokenT< edm::TriggerResults > 						TriggerToken;
  edm::EDGetTokenT< edm::TriggerResults > 						TriggerTokenPAT;
  edm::EDGetTokenT< std::vector<pat::TriggerObjectStandAlone> > 	                TriggerObjectToken;
  
  edm::EDGetTokenT< GenEventInfoProduct > 						GenEventInfoToken;
  edm::EDGetTokenT< reco::BeamSpot > 						       	BeamSpotToken;
  edm::EDGetTokenT< reco::VertexCollection > 						PrimaryVertexToken;
  edm::EDGetTokenT< edm::View<reco::Track> > 						TrackToken;
  edm::EDGetTokenT< std::vector< PileupSummaryInfo > > 	                   		PileUpInfoToken;
  
  
  //edm::Handle<bool> ifilterbadChCand;
  //edm::Handle<bool> ifilterbadPFMuon;
  edm::Handle<edm::TriggerResults> METFilterResults;
  
  // // -- Photon information -- //
  // edm::InputTag thePhotonLabel;
  // edm::InputTag full5x5SigmaIEtaIEtaMapLabel; 
  // edm::InputTag phoChargedIsolationLabel; 
  // edm::InputTag phoNeutralHadronIsolationLabel; 
  // edm::InputTag phoPhotonIsolationLabel;     
  edm::FileInPath effAreaChHadronsFile;
  edm::FileInPath effAreaNeuHadronsFile;
  edm::FileInPath effAreaPhotonsFile;  
  
    
  // -- Store flags -- // 
  bool theStorePriVtxFlag;                // Yes or No to store primary vertex
  bool theStoreJetFlag;                // Yes or No to store Jet
  bool theStoreMETFlag;                // Yes or No to store MET 
  bool theStoreHLTReportFlag;             // Yes or No to store HLT reuslts (list of triggers fired)
  bool theStoreMuonFlag;
  bool theStoreElectronFlag;
  bool theStoreLHEFlag;
  bool theStoreGENFlag;
  bool theStorePhotonFlag;
  bool theStoreTTFlag;
  bool theKeepAllGen;
  bool isMC;  //turn gen on and off
  
  // double theCrossSection;
  // double theFilterEfficiency;
  // double theTotalNevents;
  double theBDiscriminant;
  // double theIntegratedLumi;
  double theBDiscriminant_alg1;
  double theBDiscriminant_alg2;
  double theBDiscriminant_alg3;
  int theNHighQualLeptons;
  
  edm::ESHandle<TransientTrackBuilder> theTTBuilder;

  std::vector<std::string > MuonHLT;
  std::vector<int > MuonHLTPS;
  std::vector<std::string > ListHLT;
  std::vector<int > ListHLTPS;
  std::vector<std::string > trigModuleNames;
  std::vector<std::string > trigModuleNames_preFil;
  
  bool Flag_goodVertices;
  bool Flag_globalTightHalo2016Filter;
  bool Flag_HBHENoiseFilter;
  bool Flag_HBHENoiseIsoFilter;
  bool Flag_EcalDeadCellTriggerPrimitiveFilter;
  bool Flag_BadPFMuonFilter;
  bool Flag_BadChargedCandidateFilter;
  bool Flag_eeBadScFilter;
  bool Flag_ecalBadCalibFilter;
  
  // Pile-up Reweight
  // edm::LumiReWeighting LumiWeights_;
  // reweight::PoissonMeanShifter PShiftUp_;
  // reweight::PoissonMeanShifter PShiftDown_;
  // edm::LumiReWeighting LumiWeightsMuonPhys_;
  // reweight::PoissonMeanShifter PShiftUpMuonPhys_;
  // reweight::PoissonMeanShifter PShiftDownMuonPhys_;
  
  // std::vector<double> PileUpRD_;
  // std::vector<double> PileUpRDMuonPhys_;
  // std::vector<double> PileUpMC_;
  
  unsigned int nPileUp;
  double pileUpReweightIn;
  double pileUpReweight;
  double pileUpReweightPlus;
  double pileUpReweightMinus;
  
  double pileUpReweightInMuonPhys;
  double pileUpReweightMuonPhys;
  double pileUpReweightPlusMuonPhys;
  double pileUpReweightMinusMuonPhys;
  
  TTree *DYTree;
  
  int nEvt;
  
  //FILL a tree
  static const int MPSIZE = 2000;
  
  // Invariant Mass distribution of SS(OS) di-muon events GG (2 global)
  // GlbTree
  int runNum;
  unsigned long long evtNum;
  int lumiBlock;
  double PUweight;
  double sumEt;
  double photonEt;
  double chargedHadronEt;
  double neutralHadronEt;
  
  // double MET_sumEt;
  // double MET_pt;
  // double MET_px;
  // double MET_py;
  // double MET_phi;
  // double pfMET_sumEt;
  // double pfMET_pt;
  // double pfMET_px;
  // double pfMET_py;
  // double pfMET_phi;
  int Njets;
  int Nelectrons;
  int Nbtagged;
  int NbtaggedCloseMuon;
  
  // -- PDf weights -- //
  std::vector< double > PDFWeights;
  
  // -- Flags in re-miniAOD -- //
  bool Flag_duplicateMuons;
  bool Flag_badMuons;
  bool Flag_noBadMuons;
  
  // PV
  int nVertices;
  int PVtrackSize;
  double PVchi2;
  double PVndof;
  double PVnormalizedChi2;
  double PVx;
  double PVy;
  double PVz;
  double PVprob;
  
  // trigger object
  int _HLT_ntrig;
  int _HLT_trigType[MPSIZE];
  int _HLT_trigFired[MPSIZE];
  std::vector<std::string> _HLT_trigName;
  std::vector<int> _HLT_trigPS;
  double _HLT_trigPt[MPSIZE];
  double _HLT_trigEta[MPSIZE];
  double _HLT_trigPhi[MPSIZE];
  
  //==== Jet

  vector<double> jet_pt;
  vector<double> jet_eta;
  vector<double> jet_phi;
  vector<double> jet_charge;
  vector<double> jet_area;
  vector<double> jet_rho;
  vector<int> jet_partonFlavour;
  vector<int> jet_hadronFlavour;
  vector<double> jet_bTag;
  vector<double> jet_chargedHadronEnergyFraction;
  vector<double> jet_neutralHadronEnergyFraction;
  vector<double> jet_neutralEmEnergyFraction;
  vector<double> jet_chargedEmEnergyFraction;
  vector<int> jet_chargedMultiplicity;
  vector<int> jet_neutralMultiplicity;
  vector<bool> jet_looseJetID;
  vector<bool> jet_tightJetID;
  vector<bool> jet_tightLepVetoJetID;
  vector<int> jet_partonPdgId;
  vector<int> jet_vtxNtracks;
  vector<double> jet_m;
  vector<double> jet_energy;

  //==== Electron

  vector<double> electron_MVAIso;
  vector<double> electron_MVANoIso;
  vector<double> electron_et;
  vector<double> electron_caloEnergy;
  vector<double> electron_Energy;
  vector<double> electron_pt;
  vector<double> electron_Px;
  vector<double> electron_Py;
  vector<double> electron_Pz;
  vector<double> electron_eta;
  vector<double> electron_phi;
  vector<int> electron_charge;
  vector<double> electron_gsfpt;
  vector<double> electron_gsfPx;
  vector<double> electron_gsfPy;
  vector<double> electron_gsfPz;
  vector<double> electron_gsfEta;
  vector<double> electron_gsfPhi;
  vector<int> electron_gsfCharge;
  vector<double> electron_scEta;
  vector<double> electron_scPhi;
  vector<double> electron_etaWidth;
  vector<double> electron_phiWidth;
  vector<double> electron_dEtaIn;
  vector<double> electron_dEtaInSeed;
  vector<double> electron_dPhiIn;
  vector<double> electron_sigmaIEtaIEta;
  vector<double> electron_Full5x5_SigmaIEtaIEta;
  vector<double> electron_HoverE;
  vector<double> electron_fbrem;
  vector<double> electron_eOverP;
  vector<double> electron_energyEC;
  vector<double> electron_Pnorm;
  vector<double> electron_InvEminusInvP;
  vector<double> electron_dxyVTX;
  vector<double> electron_dzVTX;
  vector<double> electron_dxy;
  vector<double> electron_sigdxy;
  vector<double> electron_dz;
  vector<double> electron_ip3D;
  vector<double> electron_sigip3D;
  vector<double> electron_dxyBS;
  vector<double> electron_dzBS;
  vector<double> electron_AEff03;
  vector<double> electron_chIso03;
  vector<double> electron_nhIso03;
  vector<double> electron_phIso03;
  vector<double> electron_pcIso03;
  vector<double> electron_puChIso03;
  vector<double> electron_chIso04;
  vector<double> electron_nhIso04;
  vector<double> electron_phIso04;
  vector<double> electron_pcIso04;
  vector<double> electron_puChIso04;
  vector<double> electron_relIsoCom03;
  vector<double> electron_relIsoCom04;
  vector<double> electron_relIsoBeta03;
  vector<double> electron_relIsoBeta04;
  vector<double> electron_relIsoRho03;
  vector<bool> electron_passConversionVeto;
  vector<bool> electron_isGsfCtfScPixChargeConsistent;
  vector<int> electron_mHits;
  vector<int> electron_crack;
  vector<int> electron_ecalDriven;
  vector<double> electron_isoEMHADDepth1;
  vector<double> electron_25over55;
  vector<double> electron_15over55;
  vector<double> electron_isoHADDepth2;
  vector<double> electron_isoptTrks;
  vector<double> electron_modIsoEMHADDepth1;
  vector<double> electron_modIsoptTrks;
  vector<double> electron_modIsoEMHADDepth1Orig;
  vector<double> electron_modIsoptTrksOrig;
  vector<double> electron_ambGsf0pt;
  vector<double> electron_ambGsf0Eta;
  vector<double> electron_ambGsf0Phi;
  vector<double> electron_ambGsf0Charge;
  vector<double> electron_ambGsf1pt;
  vector<double> electron_ambGsf1Eta;
  vector<double> electron_ambGsf1Phi;
  vector<double> electron_ambGsf1Charge;
  vector<double> electron_ambGsf2pt;
  vector<double> electron_ambGsf2Eta;
  vector<double> electron_ambGsf2Phi;
  vector<double> electron_ambGsf2Charge;
  vector<double> electron_ambGsf3pt;
  vector<double> electron_ambGsf3Eta;
  vector<double> electron_ambGsf3Phi;
  vector<double> electron_ambGsf3Charge;
  vector<double> electron_r9;
  vector<double> electron_scEnergy;
  vector<double> electron_scPreEnergy;
  vector<double> electron_scRawEnergy;
  vector<double> electron_scEt;
  vector<double> electron_E15;
  vector<double> electron_E25;
  vector<double> electron_E55;
  vector<double> electron_RelPFIso_dBeta;
  vector<double> electron_RelPFIso_Rho;
  vector<bool> electron_passVetoID;
  vector<bool> electron_passLooseID;
  vector<bool> electron_passMediumID;
  vector<bool> electron_passTightID;
  vector<bool> electron_passMVAID_noIso_WP80;
  vector<bool> electron_passMVAID_noIso_WP90;
  vector<bool> electron_passMVAID_iso_WP80;
  vector<bool> electron_passMVAID_iso_WP90;
  vector<bool> electron_passMVAID_WP80;
  vector<bool> electron_passMVAID_WP90;
  vector<bool> electron_passHEEPID;
  vector<double> electron_ptUnCorr;
  vector<double> electron_etaUnCorr;
  vector<double> electron_phiUnCorr;
  vector<double> electron_PxUnCorr;
  vector<double> electron_PyUnCorr;
  vector<double> electron_PzUnCorr;
  vector<double> electron_EnergyUnCorr;
  vector<double> electron_scEnergyUnCorr;
  vector<double> electron_scEtaUnCorr;
  vector<double> electron_scPhiUnCorr;
  vector<double> electron_scEtUnCorr;
  vector<double> electron_mva;
  vector<double> electron_zzmva;
  vector<int> electron_missinghits;
  vector<string> electron_trigmatch;

  //==== Muon

  vector<double> muon_PfChargedHadronIsoR05;
  vector<double> muon_PfNeutralHadronIsoR05;
  vector<double> muon_PfGammaIsoR05;
  vector<double> muon_PfChargedHadronIsoR04;
  vector<double> muon_PfNeutralHadronIsoR04;
  vector<double> muon_PfGammaIsoR04;
  vector<double> muon_PFSumPUIsoR04;
  vector<double> muon_PfChargedHadronIsoR03;
  vector<double> muon_PfNeutralHadronIsoR03;
  vector<double> muon_PfGammaIsoR03;
  vector<double> muon_PFSumPUIsoR03;
  vector<int> muon_isPF;
  vector<int> muon_isGlobal;
  vector<int> muon_isTracker;
  vector<int> muon_isStandAlone;
  vector<int> muon_filterName;
  vector<string> muon_trigmatch;
  vector<double> muon_dB;
  vector<double> muon_phi;
  vector<double> muon_eta;
  vector<double> muon_pt;
  vector<double> muon_cktpt;
  vector<double> muon_cktPx;
  vector<double> muon_cktPy;
  vector<double> muon_cktPz;
  vector<double> muon_cktptError;
  vector<double> muon_Px;
  vector<double> muon_Py;
  vector<double> muon_Pz;
  vector<double> muon_sumtrkpt;
  vector<double> muon_trkiso;
  vector<double> muon_hcaliso;
  vector<double> muon_ecaliso;
  vector<double> muon_trkisoR05;
  vector<double> muon_hcalisoR05;
  vector<double> muon_ecalisoR05;
  vector<int> muon_charge;
  vector<int> muon_nChambers;
  vector<int> muon_matchedstations;
  vector<int> muon_stationMask;
  vector<int> muon_nSegments;
  vector<double> muon_chi2dof;
  vector<int> muon_validhits;
  vector<int> muon_trackerHits;
  vector<int> muon_pixelHits;
  vector<int> muon_validmuonhits;
  vector<int> muon_trackerLayers;
  vector<int> muon_trackerHitsGLB;
  vector<int> muon_trackerLayersGLB;
  vector<int> muon_pixelHitsGLB;
  vector<double> muon_qoverp;
  vector<double> muon_theta;
  vector<double> muon_lambda;
  vector<double> muon_dxy;
  vector<double> muon_d0;
  vector<double> muon_dsz;
  vector<double> muon_dz;
  vector<double> muon_dxyBS;
  vector<double> muon_dzBS;
  vector<double> muon_dszBS;
  vector<double> muon_dxyVTX;
  vector<double> muon_dzVTX;
  vector<double> muon_dszVTX;
  vector<double> muon_dxycktVTX;
  vector<double> muon_dzcktVTX;
  vector<double> muon_dszcktVTX;
  vector<double> muon_vx;
  vector<double> muon_vy;
  vector<double> muon_vz;
  vector<double> muon_Best_pt;
  vector<double> muon_Best_ptError;
  vector<double> muon_Best_Px;
  vector<double> muon_Best_Py;
  vector<double> muon_Best_Pz;
  vector<double> muon_Best_eta;
  vector<double> muon_Best_phi;
  vector<double> muon_Inner_pt;
  vector<double> muon_Inner_ptError;
  vector<double> muon_Inner_Px;
  vector<double> muon_Inner_Py;
  vector<double> muon_Inner_Pz;
  vector<double> muon_Inner_eta;
  vector<double> muon_Inner_phi;
  vector<double> muon_Outer_pt;
  vector<double> muon_Outer_ptError;
  vector<double> muon_Outer_Px;
  vector<double> muon_Outer_Py;
  vector<double> muon_Outer_Pz;
  vector<double> muon_Outer_eta;
  vector<double> muon_Outer_phi;
  vector<double> muon_GLB_pt;
  vector<double> muon_GLB_ptError;
  vector<double> muon_GLB_Px;
  vector<double> muon_GLB_Py;
  vector<double> muon_GLB_Pz;
  vector<double> muon_GLB_eta;
  vector<double> muon_GLB_phi;
  vector<double> muon_TuneP_pt;
  vector<double> muon_TuneP_ptError;
  vector<double> muon_TuneP_Px;
  vector<double> muon_TuneP_Py;
  vector<double> muon_TuneP_Pz;
  vector<double> muon_TuneP_eta;
  vector<double> muon_TuneP_phi;

  // LHE
  int nLHEParticle;
  double LHELepton_Px[MPSIZE];
  double LHELepton_Py[MPSIZE];
  double LHELepton_Pz[MPSIZE];
  double LHELepton_E[MPSIZE];
  int LHELepton_ID[MPSIZE];
  int LHELepton_status[MPSIZE];
  
  // GEN
  int GENnPair;
  double GENLepton_phi[MPSIZE];
  double GENLepton_eta[MPSIZE];
  double GENLepton_pT[MPSIZE];
  double GENLepton_Px[MPSIZE];
  double GENLepton_Py[MPSIZE];
  double GENLepton_Pz[MPSIZE];
  double GENLepton_E[MPSIZE];
  int GENLepton_mother[MPSIZE];
  double GENLepton_mother_pT[MPSIZE];
  int GENLepton_mother_index[MPSIZE];
  int GENLepton_charge[MPSIZE];
  int GENLepton_status[MPSIZE];
  int GENLepton_ID[MPSIZE];
  int GENLepton_isPrompt[MPSIZE];
  int GENLepton_isPromptFinalState[MPSIZE];
  int GENLepton_isTauDecayProduct[MPSIZE];
  int GENLepton_isPromptTauDecayProduct[MPSIZE];
  int GENLepton_isDirectPromptTauDecayProductFinalState[MPSIZE];
  int GENLepton_isHardProcess[MPSIZE];
  int GENLepton_isLastCopy[MPSIZE];
  int GENLepton_isLastCopyBeforeFSR[MPSIZE];
  int GENLepton_isPromptDecayed[MPSIZE];
  int GENLepton_isDecayedLeptonHadron[MPSIZE];
  int GENLepton_fromHardProcessBeforeFSR[MPSIZE];
  int GENLepton_fromHardProcessDecayed[MPSIZE];
  int GENLepton_fromHardProcessFinalState[MPSIZE];
  int GENLepton_isMostlyLikePythia6Status3[MPSIZE];
  double GENEvt_weight;
  double GENEvt_QScale;
  double GENEvt_x1;
  double GENEvt_x2;
  int GENEvt_id1;
  int GENEvt_id2;
  double GENEvt_alphaQCD;
  double GENEvt_alphaQED;
  
  
  // -- Photon information -- //
  int nPhotons;
  double Photon_pT[MPSIZE];
  double Photon_eta[MPSIZE];
  double Photon_phi[MPSIZE];
  double Photon_scEta[MPSIZE];
  double Photon_scPhi[MPSIZE];
  double Photon_HoverE[MPSIZE];
  int Photon_hasPixelSeed[MPSIZE];
  double Photon_Full5x5_SigmaIEtaIEta[MPSIZE];
  double Photon_ChIso[MPSIZE];
  double Photon_NhIso[MPSIZE];
  double Photon_PhIso[MPSIZE];
  double Photon_ChIsoWithEA[MPSIZE];
  double Photon_NhIsoWithEA[MPSIZE];
  double Photon_PhIsoWithEA[MPSIZE];
  bool Photon_passMVAID_WP80[MPSIZE];
  bool Photon_passMVAID_WP90[MPSIZE];
  bool Photon_passLooseID[MPSIZE];
  bool Photon_passMediumID[MPSIZE];
  bool Photon_passTightID[MPSIZE];

  int nUnCorrPhoton;
  double Photon_pTUnCorr[MPSIZE];
  double Photon_etaUnCorr[MPSIZE];
  double Photon_phiUnCorr[MPSIZE];
  
  // Effective area constants for all isolation types
  // EffectiveAreas effAreaChHadrons_;
  // EffectiveAreas effAreaNeuHadrons_;
  // EffectiveAreas effAreaPhotons_;
  
  int NTT;
  double TTrack_dxy[MPSIZE];
  double TTrack_dxyErr[MPSIZE];
  double TTrack_d0[MPSIZE];
  double TTrack_d0Err[MPSIZE];
  double TTrack_dsz[MPSIZE];
  double TTrack_dszErr[MPSIZE];
  double TTrack_dz[MPSIZE];
  double TTrack_dzErr[MPSIZE];
  double TTrack_dxyBS[MPSIZE];
  double TTrack_dszBS[MPSIZE];
  double TTrack_dzBS[MPSIZE];
  double TTrack_pT[MPSIZE];
  double TTrack_Px[MPSIZE];
  double TTrack_Py[MPSIZE];
  double TTrack_Pz[MPSIZE];
  double TTrack_eta[MPSIZE];
  double TTrack_phi[MPSIZE];
  double TTrack_charge[MPSIZE];
  
  // -- MET -- //
  double pfMET_pT;
  double pfMET_phi; 
  double pfMET_Px; 
  double pfMET_Py; 
  double pfMET_SumEt;
  
  double pfMET_Type1_pT;
  double pfMET_Type1_phi; 
  double pfMET_Type1_Px; 
  double pfMET_Type1_Py; 
  double pfMET_Type1_SumEt;
  
  double pfMET_Type1_PhiCor_pT;
  double pfMET_Type1_PhiCor_phi; 
  double pfMET_Type1_PhiCor_Px; 
  double pfMET_Type1_PhiCor_Py; 
  double pfMET_Type1_PhiCor_SumEt;
};
#endif
