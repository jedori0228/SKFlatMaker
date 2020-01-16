import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')
options.register('sampletype', "DATA", VarParsing.multiplicity.singleton, VarParsing.varType.string, "sampletype: DATA/MC/PrivateMC")
options.register('ScaleIDRange', "-999,-999", VarParsing.multiplicity.singleton, VarParsing.varType.string, "PDF Scale ID range: 1,9")
options.register('PDFErrorIDRange', "-999,-999", VarParsing.multiplicity.singleton, VarParsing.varType.string, "PDF Error ID range: 1001,1100")
options.register('PDFAlphaSIDRange', "-999,-999", VarParsing.multiplicity.singleton, VarParsing.varType.string, "PDF AlphaS ID range: 1101,1102")
options.register('PDFAlphaSScaleValue', "-999,-999", VarParsing.multiplicity.singleton, VarParsing.varType.string, "PDF AlphaS Scale values: 1.5,1.5")
options.register('year',-1, VarParsing.multiplicity.singleton, VarParsing.varType.int, "year: Which year")
options.parseArguments()

import sys

Is2016 = False
Is2017 = False
Is2018 = False
if options.year==2016:
  Is2016 = True
elif options.year==2017:
  Is2017 = True
elif options.year==2018:
  Is2018 = True
else:
  ErrorMgs = "year is not correct; "+str(options.year)
  sys.exit(ErrorMgs)

isMC = True
if "data" in options.sampletype.lower():
  isMC = False
if "mc" in options.sampletype.lower():
  isMC = True
isPrivateSample = False
if "private" in options.sampletype.lower():
  isPrivateSample = True

ScaleIDRange = [int(options.ScaleIDRange.split(',')[0]), int(options.ScaleIDRange.split(',')[1])]
PDFErrorIDRange = [int(options.PDFErrorIDRange.split(',')[0]), int(options.PDFErrorIDRange.split(',')[1])]
PDFAlphaSIDRange = [int(options.PDFAlphaSIDRange.split(',')[0]), int(options.PDFAlphaSIDRange.split(',')[1])]
PDFAlphaSScaleValue = [float(options.PDFAlphaSScaleValue.split(',')[0]), float(options.PDFAlphaSScaleValue.split(',')[1])]

print 'isMC = '+str(isMC)
print 'isPrivateSample = '+str(isPrivateSample)
print 'ScaleIDRange = ',
print ScaleIDRange
print 'PDFErrorIDRange = ',
print PDFErrorIDRange
print 'PDFAlphaSIDRange = ',
print PDFAlphaSIDRange
print 'PDFAlphaSScaleValue = ',
print PDFAlphaSScaleValue
print 'year = '+str(options.year)


#### Global Tag
#### https://twiki.cern.ch/twiki/bin/viewauth/CMS/PdmVAnalysisSummaryTable

GT_MC = ''
GT_DATA = ''

if Is2016:
  GT_MC = '102X_mcRun2_asymptotic_v7'
  GT_DATA = '102X_dataRun2_v12'
elif Is2017:
  GT_MC = '102X_mc2017_realistic_v7'
  GT_DATA = '102X_dataRun2_v12'
elif Is2018:
  GT_MC = '102X_upgrade2018_realistic_v20'
  GT_DATA = '102X_dataRun2_v12'
  if (not isMC) and '2018Prompt' in options.sampletype:
    GT_DATA = '102X_dataRun2_Prompt_v15'

print 'GT_MC = '+GT_MC
print 'GT_DATA = '+GT_DATA

####################################################################################################################

process = cms.Process("NTUPLE")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

## Options and Output Report
process.options   = cms.untracked.PSet( 
  SkipEvent = cms.untracked.vstring('ProductNotFound'),
  wantSummary = cms.untracked.bool(True) 
)
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring( options.inputFiles ),
  #skipEvents=cms.untracked.uint32(5),
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

# -- Geometry and Detector Conditions (needed for a few patTuple production steps) -- #
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
#process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

# -- Global Tags -- #
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")

if isMC == True:
  process.GlobalTag.globaltag = cms.string(GT_MC)
else:
  process.GlobalTag.globaltag = cms.string(GT_DATA) #prompt-reco global tag


process.TFileService = cms.Service("TFileService",
  fileName = cms.string( options.outputFile )
)

#################
# -- DY Tree -- #
#################
from SKFlatMaker.SKFlatMaker.SKFlatMaker_cfi import *

process.recoTree = SKFlatMaker.clone()
process.recoTree.DataYear = cms.untracked.int32(options.year)
process.recoTree.DebugLevel = cms.untracked.int32(0)
process.recoTree.StoreHLTObjectFlag = False ##FIXME

# -- Objects without Corrections -- # 
process.recoTree.Muon = cms.untracked.InputTag("slimmedMuons") # -- miniAOD -- #
process.recoTree.Electron = cms.untracked.InputTag("slimmedElectrons") # -- miniAOD -- #
process.recoTree.Photon = cms.untracked.InputTag("slimmedPhotons") # -- miniAOD -- #
process.recoTree.Jet = cms.untracked.InputTag("slimmedJets") # -- miniAOD -- #
process.recoTree.FatJet = cms.untracked.InputTag("slimmedJetsAK8")
process.recoTree.MET = cms.InputTag("slimmedMETs")
process.recoTree.GenParticle = cms.untracked.InputTag("prunedGenParticles") # -- miniAOD -- #

process.recoTree.ScaleIDRange = cms.untracked.vint32(ScaleIDRange)
process.recoTree.PDFErrorIDRange = cms.untracked.vint32(PDFErrorIDRange)
process.recoTree.PDFAlphaSIDRange = cms.untracked.vint32(PDFAlphaSIDRange)
process.recoTree.PDFAlphaSScaleValue = cms.untracked.vdouble(PDFAlphaSScaleValue)

if isPrivateSample:
  process.recoTree.LHEEventProduct = cms.untracked.InputTag("source")
  process.recoTree.LHERunInfoProduct = cms.untracked.InputTag("source")

process.recoTree.rho = cms.untracked.InputTag("fixedGridRhoFastjetAll")
process.recoTree.conversionsInputTag = cms.untracked.InputTag("reducedEgamma:reducedConversions") # -- miniAOD -- #

# -- for Track & Vertex -- #
process.recoTree.PrimaryVertex = cms.untracked.InputTag("offlineSlimmedPrimaryVertices") # -- miniAOD -- #

# -- Else -- #
process.recoTree.PileUpInfo = cms.untracked.InputTag("slimmedAddPileupInfo")

# -- Filters -- #
process.recoTree.ApplyFilter = False

# -- Store Flags -- #
process.recoTree.StoreMuonFlag = True
process.recoTree.StoreElectronFlag = True
process.recoTree.StorePhotonFlag = True # -- photon part should be updated! later when it is necessary -- #
process.recoTree.StoreJetFlag = True
process.recoTree.StoreMETFlag = True
process.recoTree.StoreGENFlag = isMC
process.recoTree.KeepAllGen = isMC
process.recoTree.StoreLHEFlag = isMC

#### EGamma ####

myEleID =  [
'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff',
'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V2_cff',
'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_noIso_V2_cff',
'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V2_cff',
]
myPhoID =  [
'RecoEgamma.PhotonIdentification.Identification.mvaPhotonID_Fall17_94X_V2_cff',
'RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V2_cff',
]

if Is2016:

  print "################"
  print "Running 2016"
  print "################"

  ###########################
  #### Rerun EGammaPostReco
  ###########################

  from EgammaUser.EgammaPostRecoTools.EgammaPostRecoTools import setupEgammaPostRecoSeq
  setupEgammaPostRecoSeq(process,
                         runVID=True,
                         era='2016-Legacy',
                         eleIDModules=myEleID,
                         phoIDModules=myPhoID,
                         #runEnergyCorrections=False ## when False, VID not reran.. I dunno why..
  )

  #################
  ### Reapply JEC
  ### https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#CorrPatJets
  #################

  from CondCore.CondDB.CondDB_cfi import CondDB
  CondDBJECFile = CondDB.clone(connect = cms.string( 'sqlite_file:/data6/Users/jskim/SKFlatMaker/Run2Legacy_v4__CMSSW_10_2_18/src/SKFlatMaker/SKFlatMaker/script/TAMSA/Summer16_FastsimV1_MC.db' ) )
  process.jec = cms.ESSource('PoolDBESSource',
      CondDBJECFile,
      toGet = cms.VPSet(
          cms.PSet(
              record = cms.string('JetCorrectionsRecord'),
              tag    = cms.string('JetCorrectorParametersCollection_Summer16_FastsimV1_MC_AK4PFchs'),
              label  = cms.untracked.string('AK4PFchs')
          ),
          cms.PSet(
              record = cms.string('JetCorrectionsRecord'),
              tag    = cms.string('JetCorrectorParametersCollection_Summer16_FastsimV1_MC_AK8PFPuppi'),
              label  = cms.untracked.string('AK8PFPuppi')
          ),
          # ...and so on for all jet types you need
      )
  )

  # Add an ESPrefer to override JEC that might be available from the global tag
  process.es_prefer_jec = cms.ESPrefer('PoolDBESSource', 'jec')

  from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection

  #### AK4

  updateJetCollection(
     process,
     jetSource = cms.InputTag('slimmedJets'),
     labelName = 'UpdatedJECslimmedJets',
     jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')  # Update: Safe to always add 'L2L3Residual' as MC contains dummy L2L3Residual corrections (always set to 1)
  )
  process.recoTree.Jet = cms.untracked.InputTag("updatedPatJetsUpdatedJECslimmedJets")

  #### AK8

  updateJetCollection(
     process,
     jetSource = cms.InputTag('slimmedJetsAK8'),
     labelName = 'UpdatedJECslimmedJetsAK8',
     jetCorrections = ('AK8PFPuppi', cms.vstring(['L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')  # Update: Safe to always add 'L2L3Residual' as MC contains dummy L2L3Residual corrections (always set to 1)
  )
  process.recoTree.FatJet = cms.untracked.InputTag("updatedPatJetsUpdatedJECslimmedJetsAK8")

  #### JEC Sequence

  process.jecSequence = cms.Sequence(
    process.patJetCorrFactorsUpdatedJECslimmedJets *
    process.updatedPatJetsUpdatedJECslimmedJets *
    process.patJetCorrFactorsUpdatedJECslimmedJetsAK8 *
    process.updatedPatJetsUpdatedJECslimmedJetsAK8
  )

  #################
  #### Update MET
  #### https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETUncertaintyPrescription
  #### This is independent of jecSequence, but it rather reapply JEC/JER using GT withing this MET corrector module
  #################

  from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
  runMetCorAndUncFromMiniAOD(process,
                           isData=(not isMC),
                           )


  #########################
  #### L1Prefire reweight
  #########################

  from PhysicsTools.PatUtils.l1ECALPrefiringWeightProducer_cfi import l1ECALPrefiringWeightProducer
  process.prefiringweight = l1ECALPrefiringWeightProducer.clone(
    DataEra = cms.string("2016BtoH"), #Use 2016BtoH for 2016
    ThePhotons = cms.InputTag("slimmedPhotons"),
    TheJets = cms.InputTag("slimmedJets"),
    UseJetEMPt = cms.bool(True),
    PrefiringRateSystematicUncty = cms.double(0.2),
    SkipWarnings = False)

  ###########################
  #### Rochester correction
  ###########################
  process.recoTree.roccorPath = cms.string('SKFlatMaker/SKFlatMaker/data/roccor.Run2.v3/RoccoR2016.txt')

  ###########
  #### Path
  ###########

  process.p = cms.Path(
    process.egammaPostRecoSeq *
    process.jecSequence *
    process.fullPatMetSequence
  )
  if isMC:
    process.recoTree.StoreL1PrefireFlag = cms.untracked.bool(True)
    process.p *= process.prefiringweight

  process.p *= process.recoTree

elif Is2017:

  print "################"
  print "Running 2017"
  print "################"

  ###########################
  #### Rerun EGammaPostReco
  ###########################

  from EgammaUser.EgammaPostRecoTools.EgammaPostRecoTools import setupEgammaPostRecoSeq
  setupEgammaPostRecoSeq(process,
                         runVID=True, #saves CPU time by not needlessly re-running VID
                         era='2017-Nov17ReReco',
                         eleIDModules=myEleID,
                         phoIDModules=myPhoID,
  )
  #a sequence egammaPostRecoSeq has now been created and should be added to your path, eg process.p=cms.Path(process.egammaPostRecoSeq)

  ###########################
  #### MET EE Noise
  ###########################

  from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD

  runMetCorAndUncFromMiniAOD (
          process,
          isData = (not isMC), # false for MC
          fixEE2017 = True,
          fixEE2017Params = {'userawPt': True, 'ptThreshold':50.0, 'minEtaThreshold':2.65, 'maxEtaThreshold': 3.139} ,
          postfix = "ModifiedMET"
  )
  process.recoTree.MET = cms.InputTag("slimmedMETsModifiedMET")

  #################
  ### Reapply JEC
  ### https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#CorrPatJets
  #################

  from CondCore.CondDB.CondDB_cfi import CondDB
  CondDBJECFile = CondDB.clone(connect = cms.string( 'sqlite_file:/data6/Users/jskim/SKFlatMaker/Run2Legacy_v4__CMSSW_10_2_18/src/SKFlatMaker/SKFlatMaker/data/JECDatabase/SQLiteFiles/Fall17_FastSimV1_MC.db' ) )
  process.jec = cms.ESSource('PoolDBESSource',
      CondDBJECFile,
      toGet = cms.VPSet(
          cms.PSet(
              record = cms.string('JetCorrectionsRecord'),
              tag    = cms.string('JetCorrectorParametersCollection_Fall17_FastSimV1_MC_AK4PFchs'),
              label  = cms.untracked.string('AK4PFchs')
          ),
          cms.PSet(
              record = cms.string('JetCorrectionsRecord'),
              tag    = cms.string('JetCorrectorParametersCollection_Fall17_FastSimV1_MC_AK8PFPuppi'),
              label  = cms.untracked.string('AK8PFPuppi')
          ),
          # ...and so on for all jet types you need
      )
  )

  # Add an ESPrefer to override JEC that might be available from the global tag
  process.es_prefer_jec = cms.ESPrefer('PoolDBESSource', 'jec')

  from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection

  #### AK4

  updateJetCollection(
     process,
     jetSource = cms.InputTag('slimmedJets'),
     labelName = 'UpdatedJECslimmedJets',
     jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')  # Update: Safe to always add 'L2L3Residual' as MC contains dummy L2L3Residual corrections (always set to 1)
  )
  process.recoTree.Jet = cms.untracked.InputTag("updatedPatJetsUpdatedJECslimmedJets")

  #### AK8

  updateJetCollection(
     process,
     jetSource = cms.InputTag('slimmedJetsAK8'),
     labelName = 'UpdatedJECslimmedJetsAK8',
     jetCorrections = ('AK8PFPuppi', cms.vstring(['L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')  # Update: Safe to always add 'L2L3Residual' as MC contains dummy L2L3Residual corrections (always set to 1)
  )
  process.recoTree.FatJet = cms.untracked.InputTag("updatedPatJetsUpdatedJECslimmedJetsAK8")

  #### JEC Sequence

  process.jecSequence = cms.Sequence(
    process.patJetCorrFactorsUpdatedJECslimmedJets *
    process.updatedPatJetsUpdatedJECslimmedJets *
    process.patJetCorrFactorsUpdatedJECslimmedJetsAK8 *
    process.updatedPatJetsUpdatedJECslimmedJetsAK8
  )

  ##########
  #### JER
  ##########

  process.recoTree.AK4Jet_JER_PtRes_filepath = cms.string('SKFlatMaker/SKFlatMaker/data/JRDatabase/textFiles/Fall17_V3_MC/Fall17_V3_MC_PtResolution_AK4PFchs.txt')
  process.recoTree.AK4Jet_JER_SF_filepath    = cms.string('SKFlatMaker/SKFlatMaker/data/JRDatabase/textFiles/Fall17_V3_MC/Fall17_V3_MC_SF_AK4PFchs.txt')
  process.recoTree.AK8Jet_JER_PtRes_filepath = cms.string('SKFlatMaker/SKFlatMaker/data/JRDatabase/textFiles/Fall17_V3_MC/Fall17_V3_MC_PtResolution_AK8PFPuppi.txt')
  process.recoTree.AK8Jet_JER_SF_filepath    = cms.string('SKFlatMaker/SKFlatMaker/data/JRDatabase/textFiles/Fall17_V3_MC/Fall17_V3_MC_SF_AK8PFPuppi.txt')

  #######################################
  #### ecalBadCalibReducedMINIAODFilter
  #### https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2#How_to_run_ecal_BadCalibReducedM
  ########################################

  process.load('RecoMET.METFilters.ecalBadCalibFilter_cfi')

  baddetEcallist = cms.vuint32(
    [872439604,872422825,872420274,872423218,
     872423215,872416066,872435036,872439336,
     872420273,872436907,872420147,872439731,
     872436657,872420397,872439732,872439339,
     872439603,872422436,872439861,872437051,
     872437052,872420649,872421950,872437185,
     872422564,872421566,872421695,872421955,
     872421567,872437184,872421951,872421694,
     872437056,872437057,872437313,872438182,
     872438951,872439990,872439864,872439609,
     872437181,872437182,872437053,872436794,
     872436667,872436536,872421541,872421413,
     872421414,872421031,872423083,872421439]
  )

  process.ecalBadCalibReducedMINIAODFilter = cms.EDFilter(
    "EcalBadCalibFilter",
    EcalRecHitSource = cms.InputTag("reducedEgamma:reducedEERecHits"),
    ecalMinEt        = cms.double(50.),
    baddetEcal    = baddetEcallist, 
    taggingMode = cms.bool(True),
    debug = cms.bool(False)
  )

  #########################
  #### L1Prefire reweight
  #########################

  from PhysicsTools.PatUtils.l1ECALPrefiringWeightProducer_cfi import l1ECALPrefiringWeightProducer
  process.prefiringweight = l1ECALPrefiringWeightProducer.clone(
    DataEra = cms.string("2017BtoF"), #Use 2016BtoH for 2016
    ThePhotons = cms.InputTag("slimmedPhotons"),
    TheJets = cms.InputTag("slimmedJets"),
    UseJetEMPt = cms.bool(True),
    PrefiringRateSystematicUncty = cms.double(0.2),
    SkipWarnings = False)

  ###########################
  #### Rochester correction
  ###########################
  process.recoTree.roccorPath = cms.string('SKFlatMaker/SKFlatMaker/data/roccor.Run2.v3/RoccoR2017.txt')

  ###########
  #### Path
  ###########

  process.p = cms.Path(
    process.egammaPostRecoSeq *
    process.jecSequence *
    process.fullPatMetSequenceModifiedMET *
    process.ecalBadCalibReducedMINIAODFilter
  )
  if isMC:
    process.recoTree.StoreL1PrefireFlag = cms.untracked.bool(True)
    process.p *= process.prefiringweight

  process.p *= process.recoTree

elif Is2018:

  print "################"
  print "Running 2018"
  print "################"

  ###########################
  #### Rerun EGammaPostReco
  ###########################

  from EgammaUser.EgammaPostRecoTools.EgammaPostRecoTools import setupEgammaPostRecoSeq
  setupEgammaPostRecoSeq(process,
                         era='2018-Prompt')  
  #a sequence egammaPostRecoSeq has now been created and should be added to your path, eg process.p=cms.Path(process.egammaPostRecoSeq)

  #################
  ### Reapply JEC
  ### https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#CorrPatJets
  #################

  from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection

  #### AK4

  updateJetCollection(
     process,
     jetSource = cms.InputTag('slimmedJets'),
     labelName = 'UpdatedJECslimmedJets',
     jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')  # Update: Safe to always add 'L2L3Residual' as MC contains dummy L2L3Residual corrections (always set to 1)
  )
  process.recoTree.Jet = cms.untracked.InputTag("updatedPatJetsUpdatedJECslimmedJets")

  #### AK8

  updateJetCollection(
     process,
     jetSource = cms.InputTag('slimmedJetsAK8'),
     labelName = 'UpdatedJECslimmedJetsAK8',
     jetCorrections = ('AK8PFPuppi', cms.vstring(['L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')  # Update: Safe to always add 'L2L3Residual' as MC contains dummy L2L3Residual corrections (always set to 1)
  )
  process.recoTree.FatJet = cms.untracked.InputTag("updatedPatJetsUpdatedJECslimmedJetsAK8")

  #### JEC Sequence

  process.jecSequence = cms.Sequence(
    process.patJetCorrFactorsUpdatedJECslimmedJets *
    process.updatedPatJetsUpdatedJECslimmedJets *
    process.patJetCorrFactorsUpdatedJECslimmedJetsAK8 *
    process.updatedPatJetsUpdatedJECslimmedJetsAK8
  )

  ##########
  #### JER
  ##########

  process.recoTree.AK4Jet_JER_PtRes_filepath = cms.string('SKFlatMaker/SKFlatMaker/data/JRDatabase/textFiles/Autumn18_V7b_MC/Autumn18_V7b_MC_PtResolution_AK4PFchs.txt')
  process.recoTree.AK4Jet_JER_SF_filepath    = cms.string('SKFlatMaker/SKFlatMaker/data/JRDatabase/textFiles/Autumn18_V7b_MC/Autumn18_V7b_MC_SF_AK4PFchs.txt')
  process.recoTree.AK8Jet_JER_PtRes_filepath = cms.string('SKFlatMaker/SKFlatMaker/data/JRDatabase/textFiles/Autumn18_V7b_MC/Autumn18_V7b_MC_PtResolution_AK8PFPuppi.txt')
  process.recoTree.AK8Jet_JER_SF_filepath    = cms.string('SKFlatMaker/SKFlatMaker/data/JRDatabase/textFiles/Autumn18_V7b_MC/Autumn18_V7b_MC_SF_AK8PFPuppi.txt')

  #################
  #### Update MET
  #### https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETUncertaintyPrescription
  #### This is independent of jecSequence, but it rather reapply JEC/JER using GT withing this MET corrector module
  #################

  from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
  runMetCorAndUncFromMiniAOD(process,
                           isData=(not isMC),
                           )

  #######################################
  #### ecalBadCalibReducedMINIAODFilter
  #### https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2#How_to_run_ecal_BadCalibReducedM
  ########################################

  process.load('RecoMET.METFilters.ecalBadCalibFilter_cfi')

  baddetEcallist = cms.vuint32(
    [872439604,872422825,872420274,872423218,
     872423215,872416066,872435036,872439336,
     872420273,872436907,872420147,872439731,
     872436657,872420397,872439732,872439339,
     872439603,872422436,872439861,872437051,
     872437052,872420649,872421950,872437185,
     872422564,872421566,872421695,872421955,
     872421567,872437184,872421951,872421694,
     872437056,872437057,872437313,872438182,
     872438951,872439990,872439864,872439609,
     872437181,872437182,872437053,872436794,
     872436667,872436536,872421541,872421413,
     872421414,872421031,872423083,872421439]
  )

  process.ecalBadCalibReducedMINIAODFilter = cms.EDFilter(
    "EcalBadCalibFilter",
    EcalRecHitSource = cms.InputTag("reducedEgamma:reducedEERecHits"),
    ecalMinEt        = cms.double(50.),
    baddetEcal    = baddetEcallist,
    taggingMode = cms.bool(True),
    debug = cms.bool(False)
  )

  ###########################
  #### Rochester correction
  ###########################
  process.recoTree.roccorPath = cms.string('SKFlatMaker/SKFlatMaker/data/roccor.Run2.v3/RoccoR2018.txt')

  ###########
  #### Path
  ###########

  process.p = cms.Path(
    process.egammaPostRecoSeq *
    process.jecSequence *
    process.fullPatMetSequence *
    process.ecalBadCalibReducedMINIAODFilter *
    process.recoTree
  )
