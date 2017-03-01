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
      
      edm::EDGetTokenT <std::vector<pat::Jet> > jet_token;
      edm::Handle <std::vector<pat::Jet> > jets;
      
      std::vector<pat::Muon> sorted_muon_pairs;
      std::vector<pat::Jet> sorted_jets;
      
      int total_events;
      int muon_number;
      int processed_events;
                
      
      double inv_mass;
      double mu_mass_sq;
      
      TH1D *mass_histogram;
      double min_histo;
      double max_histo;
      int bin_number;
      
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
total_events(0), muon_number(0), processed_events(0), mu_mass_sq(0), 
min_histo(40), max_histo (150), bin_number(50)

{
   //now do what ever initialization is needed
   //Tokens consume the particles vectors you need that were defined in ConfigFile_cfg.py
   muon_pairs_token = consumes< std::vector< pat::Muon > > (iConfig.getParameter<edm::InputTag>("ZCandidates"));
   jet_token = consumes< std::vector< pat::Jet > > (iConfig.getParameter<edm::InputTag>("bestJets"));
   
   //File service initialization
   usesResource("TFileService");
   edm::Service<TFileService> file_service;
   
   //histogram initialization
   mass_histogram = file_service-> make <TH1D> ("inv_mass_histo", "Muon pair invariant mass", bin_number, min_histo, max_histo);
   mass_histogram->GetXaxis()->SetTitle("m [GeV]");
   
   
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
  
  iEvent.getByToken(muon_token, muons); 
  iEvent.getByToken(jet_token, jets);
  total_events++;
  muon_number += muons->size();
  
  if (muons->size() > 1) {
    sorted_muons = *muons;
    sorted_jets = *jets;
    sort(sorted_muons.begin(), sorted_muons.end(), [](const pat::Muon mu_a, const pat::Muon mu_b) {
        return mu_a.energy() > mu_b.energy();   
    });
    
    /* //Block that fills out the histogram
    inv_mass = (sorted_muons[0].p4() + sorted_muons[1].p4()).mass();       
    if (inv_mass >= min_histo && inv_mass <= max_histo ) {
      mass_histogram->Fill(inv_mass);
      processed_events++;
    }
    */
    
    //Block that finds histogram limits, run before to find out limits, then input into global variables
    /*if (min_histo == 0 || min_histo > inv_mass ) {
      min_histo = inv_mass;
    }
    if ( max_histo < inv_mass) max_histo = inv_mass;*/
    
    
  }
  
  //LogInfo("Demo") << "sizer of muons "<< muons->size();
  
  


#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
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
    LogInfo("Demo") << "processed_events = " << processed_events << " total_events = " << total_events;
    
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TestAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  
}

//define this as a plug-in
DEFINE_FWK_MODULE(TestAnalyzer);