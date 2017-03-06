// -*- C++ -*-
//
// Package:    BasicMuounsInvariantMass/TestAnalyzer
// Class:      TestAnalyzer
// 
/**\class TestAnalyzer TestAnalyzer.cc BasicMuounsInvariantMass/TestAnalyzer/plugins/TestAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Sergey Guts
//         Created:  Wed, 22 Feb 2017 17:10:57 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


#include <vector>
#include <algorithm>
#include <math.h>


#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"


#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TH2.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class TestAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit TestAnalyzer(const edm::ParameterSet&);
      ~TestAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<std::vector<reco::CompositeCandidate>> muon_pairs_token;
      edm::Handle <std::vector<reco::CompositeCandidate>> muon_pairs;
      std::vector<reco::CompositeCandidate> sorted_muon_pairs;
      
      edm::EDGetTokenT <edm::OwnVector<reco::Candidate> > jet_token;
      edm::Handle <edm::OwnVector<reco::Candidate> > jets;
      edm::OwnVector<reco::Candidate> sorted_jets;
      
      
      
      
      int total_events;
      
      double inv_mass;
      
      TH2D *mass_histogram;
      double min_x_histo;
      double max_x_histo;
      int bin_number_x;
      double min_y_histo;
      double max_y_histo;
      int bin_number_y;
      
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TestAnalyzer::TestAnalyzer(const edm::ParameterSet& iConfig) : 
inv_mass(0),
min_x_histo(86), max_x_histo (96), bin_number_x(20),
min_y_histo(2),  max_y_histo (4),  bin_number_y(20)
{
   //now do what ever initialization is needed
   //Tokens consume the particles vectors you need that were defined in ConfigFile_cfg.py
   muon_pairs_token = consumes< std::vector<reco::CompositeCandidate> > (iConfig.getParameter<edm::InputTag>("ZCandidates"));
   jet_token = consumes<edm::OwnVector<reco::Candidate> > (iConfig.getParameter<edm::InputTag>("bestJets"));
   
   //File service initialization
   usesResource("TFileService");
   edm::Service<TFileService> file_service;
   
   //histogram initialization
   mass_histogram = file_service-> make <TH2D> ("inv_mass_histo", "Muon pair invariant mass vs jet invariant mass", 
						bin_number_x, min_x_histo, max_x_histo, bin_number_y, min_y_histo, max_y_histo);
   mass_histogram->GetXaxis()->SetTitle("M_{\mu+ \mu-} [GeV]");
   mass_histogram->GetXaxis()->SetTitle("M_{J/\psi} [GeV]");
   
   
}



TestAnalyzer::~TestAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
TestAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace std;
   using namespace edm;
  
  iEvent.getByToken(muon_pairs_token, muon_pairs); 
  iEvent.getByToken(jet_token, jets);
  
  
  //Block that sorts nessesary vectors
  /*
  if (muons->size() > 1) {
    sorted_muons = *muons;
    sorted_jets = *jets;
    sort(sorted_muons.begin(), sorted_muons.end(), [](const pat::Muon mu_a, const pat::Muon mu_b) {
        return mu_a.energy() > mu_b.energy();   
    });
  }*/
  
  
  //Block that fills out the histogram
   
   sorted_muon_pairs = *muon_pairs;
   sorted_jets = *jets;
   mass_histogram->Fill(sorted_muon_pairs[0].mass(),sorted_jets[0].mass());
    
    //Block that finds histogram limits, run before to find out limits, then input into global variables
    /*
   if (min_histo == 0 || min_histo > inv_mass ) {
     min_histo = inv_mass;
   }
   if ( max_histo < inv_mass) max_histo = inv_mass;*/
    
    
  
  //LogInfo("Demo") << "INFO "<< VARIABLE;
  
  

}


// ------------ method called once each job just before starting event loop  ------------
void 
TestAnalyzer::beginJob()
{
  
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TestAnalyzer::endJob() 
{	
    using namespace edm;
    //LogInfo("Demo") << "processed_events = " << processed_events << " total_events = " << total_events;
    
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TestAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  
}

//define this as a plug-in
DEFINE_FWK_MODULE(TestAnalyzer);
