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




#we have at least 1 jet and 2 muons


process.bestMuons = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("slimmedMuons"),
    cut = cms.string("pt > 2")
)

process.bestMuonsFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("bestMuons"),
    minNumber = cms.uint32(2),
)


process.bestJets = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("slimmedJets"),
    cut = cms.string("mass < 4 & mass > 2")
  )

process.bestJetsFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("bestJets"),
    minNumber = cms.uint32(1),
)


#enough jets and muons with appropriate mass

process.ZCandidates = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("bestMuons@+ bestMuons@-"),
    cut = cms.string("86.0 < mass < 96.0")
)

process.ZCandidatesNonempty = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("ZCandidates"),
    minNumber = cms.uint32(1),
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
   SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
	'file:/home/guts/Root/CMSSW_8_0_11/src/BasicMuounsInvariantMass/Files/24141132-5863-E511-8FD6-0025905B85AE.root'
        #'root://cms-xrd-global.cern.ch//store/data/Run2015B/DoubleMuon/MINIAOD/23Sep2015-v1/60000/24141132-5863-E511-8FD6-0025905B85AE.root', 'root://cms-xrd-global.cern.ch//store/data/Run2015B/DoubleMuon/MINIAOD/23Sep2015-v1/60000/6209D134-5863-E511-B2BC-0025905B8610.root'
    )
)

"""
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("file:///home/guts/Root/CMSSW_8_0_11/src/BasicMuounsInvariantMass/Files/TestOutput.root")
)
"""

process.demo = cms.EDAnalyzer('TestAnalyzer',
  ZCandidates=cms.InputTag("ZCandidates"),
  bestJets=cms.InputTag("bestJets"),
)

process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string('file:/home/guts/Root/CMSSW_8_0_11/src/BasicMuounsInvariantMass/Files/DoubleMassHistogram.root')
                                   )



process.p = cms.Path( process.bestMuons * process.bestMuonsFilter * process.bestJets * process.bestJetsFilter * process.ZCandidates * process.ZCandidatesNonempty * process.demo )
#process.outpath = cms.EndPath(process.out)
#process.p = cms.Path(process.muonFilter * process.jetFilter * process.bestMuons * process.bestJets * process.bestJetsNonempty * process.ZCandidates * process.ZCandidatesNonempty * process.demo)
