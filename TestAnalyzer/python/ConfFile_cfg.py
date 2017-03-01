# -*- coding: utf-8 -*-
import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('Demo')
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    limit = cms.untracked.int32(-1)
)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.muonFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("slimmedMuons"),
    minNumber = cms.uint32(2),
)
process.jetFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("slimmedJets"),
    minNumber = cms.uint32(1),
)
#we have at least 1 jet and 2 muons

process.bestMuons = cms.EDProducer("EtaPtMinCandViewSelector",
    src = cms.InputTag("slimmedMuons"),
    ptMin   = cms.double(10),
    etaMin = cms.double(-10),
    etaMax = cms.double(10)
  )
#We limit the lowest pt of muons, but not really eta 


process.bestJets = cms.EDProducer("CandSelector",
    src = cms.InputTag("slimmedJets"),
    cut = cms.string("mass < 10 & mass > 1")
  )

#jets with appropriate mass

process.ZCandidates = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("bestMuons@+ bestMuons@-"),
    cut = cms.string("86.0 < mass < 96.0")
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
#    SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'root://cms-xrd-global.cern.ch//store/data/Run2015B/DoubleMuon/MINIAOD/23Sep2015-v1/60000/24141132-5863-E511-8FD6-0025905B85AE.root', 'root://cms-xrd-global.cern.ch//store/data/Run2015B/DoubleMuon/MINIAOD/23Sep2015-v1/60000/6209D134-5863-E511-B2BC-0025905B8610.root'
    )
)

process.demo = cms.EDAnalyzer('TestAnalyzer',
  srcMuons = cms.InputTag("bestMuons")
)

process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string('file:/home/guts/Root/CMSSW_8_0_11/src/BasicMuounsInvariantMass/Files/MuonInvariantMassHistogram.root')
                                   )




process.p = cms.Path(process.jetFilter * process.muonFilter * process.ZCandidates * process.bestJets * process.demo)
