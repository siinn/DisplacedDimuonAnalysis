#Wed Jan 31 13:39:05 2018"""Automatically generated. DO NOT EDIT please"""
from GaudiKernel.GaudiHandles import *
from GaudiKernel.Proxy.Configurable import *

class CosmicBackground( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCounter' : 0, # int
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'THistSvc' : ServiceHandle('THistSvc/THistSvc'), # GaudiHandle
    'RootStreamName' : '/', # str
    'RootDirName' : '', # str
    'HistNamePrefix' : '', # str
    'HistNamePostfix' : '', # str
    'HistTitlePrefix' : '', # str
    'HistTitlePostfix' : '', # str
    'DiLepDVCuts' : PublicToolHandle('DDL::DiLepDVCuts/DiLepDVCuts'), # GaudiHandle
    'DVUtils' : PublicToolHandle('DVUtils'), # GaudiHandle
    'LeptonSelectionTool' : PublicToolHandle('LeptonSelectionTools'), # GaudiHandle
    'CosmicTool' : PublicToolHandle('CosmicTools'), # GaudiHandle
    'DiLepEventCuts' : PublicToolHandle('DDL::EventCuts/DiLepEventCuts'), # GaudiHandle
    'DiLepBaseCuts' : PublicToolHandle('DDL::DVCuts/DiLepBaseCuts'), # GaudiHandle
    'GRLTool' : PublicToolHandle('GoodRunsListSelectionTool/GRLTool'), # GaudiHandle
    'TrigDecisionTool' : PublicToolHandle('Trig::TrigDecisionTool/TrigDecisionTool'), # GaudiHandle
    'TrigMatchingTool' : PublicToolHandle('Trig::MatchingTool/TrigMatchingTool'), # GaudiHandle
    'TrigMatch' : PublicToolHandle('DDL::TrigMatch/TrigMatch'), # GaudiHandle
    'DiLepCosmics' : PublicToolHandle('DDL::DiLepCosmics/DiLepCosmics'), # GaudiHandle
    'OverlapRemoval' : PublicToolHandle('DDL::OverlapRemoval/OverlapRemoval'), # GaudiHandle
  }
  _propertyDocDct = { 
    'HistTitlePrefix' : """ The prefix for the histogram THx title """,
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'RootStreamName' : """ Name of the output ROOT stream (file) that the THistSvc uses """,
    'HistNamePostfix' : """ The postfix for the histogram THx name """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'HistTitlePostfix' : """ The postfix for the histogram THx title """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
    'THistSvc' : """ Handle to a THistSvc instance: it will be used to write ROOT objects to ROOT files """,
    'HistNamePrefix' : """ The prefix for the histogram THx name """,
    'RootDirName' : """ Name of the ROOT directory inside the ROOT file where the histograms will go """,
    'GRLTool' : """ The private GoodRunsListSelectionTool """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(CosmicBackground, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'DisplacedDimuonAnalysis'
  def getType( self ):
      return 'CosmicBackground'
  pass # class CosmicBackground

class CosmicTools( ConfigurableAlgTool ) :
  __slots__ = { 
    'MonitorService' : 'MonitorSvc', # str
    'OutputLevel' : 7, # int
    'AuditTools' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'AuditFinalize' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
  }
  _propertyDocDct = { 
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(CosmicTools, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'DisplacedDimuonAnalysis'
  def getType( self ):
      return 'CosmicTools'
  pass # class CosmicTools

class DVEfficiency( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCounter' : 0, # int
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'THistSvc' : ServiceHandle('THistSvc/THistSvc'), # GaudiHandle
    'RootStreamName' : '/', # str
    'RootDirName' : '', # str
    'HistNamePrefix' : '', # str
    'HistNamePostfix' : '', # str
    'HistTitlePrefix' : '', # str
    'HistTitlePostfix' : '', # str
    'DiLepDVCuts' : PublicToolHandle('DDL::DiLepDVCuts/DiLepDVCuts'), # GaudiHandle
    'DVUtils' : PublicToolHandle('DVUtils'), # GaudiHandle
    'LeptonSelectionTool' : PublicToolHandle('LeptonSelectionTools'), # GaudiHandle
    'CosmicTool' : PublicToolHandle('CosmicTools'), # GaudiHandle
    'DiLepEventCuts' : PublicToolHandle('DDL::EventCuts/DiLepEventCuts'), # GaudiHandle
    'DiLepBaseCuts' : PublicToolHandle('DDL::DVCuts/DiLepBaseCuts'), # GaudiHandle
    'GRLTool' : PublicToolHandle('GoodRunsListSelectionTool/GRLTool'), # GaudiHandle
    'TrigDecisionTool' : PublicToolHandle('Trig::TrigDecisionTool/TrigDecisionTool'), # GaudiHandle
    'DiLepCosmics' : PublicToolHandle('DDL::DiLepCosmics/DiLepCosmics'), # GaudiHandle
    'OverlapRemoval' : PublicToolHandle('DDL::OverlapRemoval/OverlapRemoval'), # GaudiHandle
    'PileupReweightingTool' : PublicToolHandle('CP::PileupReweightingTool/PileupReweightingTool'), # GaudiHandle
  }
  _propertyDocDct = { 
    'HistTitlePrefix' : """ The prefix for the histogram THx title """,
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'RootStreamName' : """ Name of the output ROOT stream (file) that the THistSvc uses """,
    'HistNamePostfix' : """ The postfix for the histogram THx name """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'HistTitlePostfix' : """ The postfix for the histogram THx title """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
    'THistSvc' : """ Handle to a THistSvc instance: it will be used to write ROOT objects to ROOT files """,
    'HistNamePrefix' : """ The prefix for the histogram THx name """,
    'RootDirName' : """ Name of the ROOT directory inside the ROOT file where the histograms will go """,
    'GRLTool' : """ The private GoodRunsListSelectionTool """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(DVEfficiency, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'DisplacedDimuonAnalysis'
  def getType( self ):
      return 'DVEfficiency'
  pass # class DVEfficiency

class DVUtils( ConfigurableAlgTool ) :
  __slots__ = { 
    'MonitorService' : 'MonitorSvc', # str
    'OutputLevel' : 7, # int
    'AuditTools' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'AuditFinalize' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'MuonCut' : PublicToolHandle('DDL::MuonCuts/DiLepMuonCuts'), # GaudiHandle
    'DiLepDVCuts' : PublicToolHandle('DDL::DiLepDVCuts/DiLepDVCuts'), # GaudiHandle
    'TrigMatch' : PublicToolHandle('DDL::TrigMatch/TrigMatch'), # GaudiHandle
    'TrigDecisionTool' : PublicToolHandle('Trig::TrigDecisionTool/TrigDecisionTool'), # GaudiHandle
    'TrigMatchingTool' : PublicToolHandle('Trig::MatchingTool/TrigMatchingTool'), # GaudiHandle
  }
  _propertyDocDct = { 
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(DVUtils, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'DisplacedDimuonAnalysis'
  def getType( self ):
      return 'DVUtils'
  pass # class DVUtils

class DisplacedDimuonAnalysisAlg( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCounter' : 0, # int
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'THistSvc' : ServiceHandle('THistSvc/THistSvc'), # GaudiHandle
    'RootStreamName' : '/', # str
    'RootDirName' : '', # str
    'HistNamePrefix' : '', # str
    'HistNamePostfix' : '', # str
    'HistTitlePrefix' : '', # str
    'HistTitlePostfix' : '', # str
    'DVUtils' : PublicToolHandle('DVUtils'), # GaudiHandle
    'LeptonSelectionTool' : PublicToolHandle('LeptonSelectionTools'), # GaudiHandle
    'CosmicTool' : PublicToolHandle('CosmicTools'), # GaudiHandle
    'GRLTool' : PublicToolHandle('GoodRunsListSelectionTool/GRLTool'), # GaudiHandle
    'TrigDecisionTool' : PublicToolHandle('Trig::TrigDecisionTool/TrigDecisionTool'), # GaudiHandle
    'TrigMatchingTool' : PublicToolHandle('Trig::MatchingTool/TrigMatchingTool'), # GaudiHandle
    'OverlapRemoval' : PublicToolHandle('DDL::OverlapRemoval/OverlapRemoval'), # GaudiHandle
    'DiLepCosmics' : PublicToolHandle('DDL::DiLepCosmics/DiLepCosmics'), # GaudiHandle
    'DiLepEventCuts' : PublicToolHandle('DDL::EventCuts/DiLepEventCuts'), # GaudiHandle
    'DiLepBaseCuts' : PublicToolHandle('DDL::DVCuts/DiLepBaseCuts'), # GaudiHandle
    'DiLepDVCuts' : PublicToolHandle('DDL::DiLepDVCuts/DiLepDVCuts'), # GaudiHandle
    'PhotonMatch' : PublicToolHandle('DDL::PhotonMatch/PhotonMatch'), # GaudiHandle
  }
  _propertyDocDct = { 
    'HistTitlePrefix' : """ The prefix for the histogram THx title """,
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'RootStreamName' : """ Name of the output ROOT stream (file) that the THistSvc uses """,
    'HistNamePostfix' : """ The postfix for the histogram THx name """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'HistTitlePostfix' : """ The postfix for the histogram THx title """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
    'THistSvc' : """ Handle to a THistSvc instance: it will be used to write ROOT objects to ROOT files """,
    'HistNamePrefix' : """ The prefix for the histogram THx name """,
    'RootDirName' : """ Name of the ROOT directory inside the ROOT file where the histograms will go """,
    'GRLTool' : """ The private GoodRunsListSelectionTool """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(DisplacedDimuonAnalysisAlg, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'DisplacedDimuonAnalysis'
  def getType( self ):
      return 'DisplacedDimuonAnalysisAlg'
  pass # class DisplacedDimuonAnalysisAlg

class LRTValidation( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCounter' : 0, # int
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'THistSvc' : ServiceHandle('THistSvc/THistSvc'), # GaudiHandle
    'RootStreamName' : '/', # str
    'RootDirName' : '', # str
    'HistNamePrefix' : '', # str
    'HistNamePostfix' : '', # str
    'HistTitlePrefix' : '', # str
    'HistTitlePostfix' : '', # str
    'DVUtils' : PublicToolHandle('DVUtils'), # GaudiHandle
  }
  _propertyDocDct = { 
    'HistTitlePrefix' : """ The prefix for the histogram THx title """,
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'RootStreamName' : """ Name of the output ROOT stream (file) that the THistSvc uses """,
    'HistNamePostfix' : """ The postfix for the histogram THx name """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'HistTitlePostfix' : """ The postfix for the histogram THx title """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
    'THistSvc' : """ Handle to a THistSvc instance: it will be used to write ROOT objects to ROOT files """,
    'HistNamePrefix' : """ The prefix for the histogram THx name """,
    'RootDirName' : """ Name of the ROOT directory inside the ROOT file where the histograms will go """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(LRTValidation, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'DisplacedDimuonAnalysis'
  def getType( self ):
      return 'LRTValidation'
  pass # class LRTValidation

class LeptonEfficiency( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCounter' : 0, # int
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'THistSvc' : ServiceHandle('THistSvc/THistSvc'), # GaudiHandle
    'RootStreamName' : '/', # str
    'RootDirName' : '', # str
    'HistNamePrefix' : '', # str
    'HistNamePostfix' : '', # str
    'HistTitlePrefix' : '', # str
    'HistTitlePostfix' : '', # str
    'DVUtils' : PublicToolHandle('DVUtils'), # GaudiHandle
    'TrigDecisionTool' : PublicToolHandle('Trig::TrigDecisionTool/TrigDecisionTool'), # GaudiHandle
  }
  _propertyDocDct = { 
    'HistTitlePrefix' : """ The prefix for the histogram THx title """,
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'RootStreamName' : """ Name of the output ROOT stream (file) that the THistSvc uses """,
    'HistNamePostfix' : """ The postfix for the histogram THx name """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'HistTitlePostfix' : """ The postfix for the histogram THx title """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
    'THistSvc' : """ Handle to a THistSvc instance: it will be used to write ROOT objects to ROOT files """,
    'HistNamePrefix' : """ The prefix for the histogram THx name """,
    'RootDirName' : """ Name of the ROOT directory inside the ROOT file where the histograms will go """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(LeptonEfficiency, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'DisplacedDimuonAnalysis'
  def getType( self ):
      return 'LeptonEfficiency'
  pass # class LeptonEfficiency

class LeptonSelectionTools( ConfigurableAlgTool ) :
  __slots__ = { 
    'MonitorService' : 'MonitorSvc', # str
    'OutputLevel' : 7, # int
    'AuditTools' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'AuditFinalize' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'ElectronLikelihoodTool' : PublicToolHandle('AsgElectronLikelihoodTool/ElectronLikelihoodTool'), # GaudiHandle
    'MuonCorrectionTool' : PublicToolHandle('CP::MuonCalibrationAndSmearingTool/MuonCorrectionTool'), # GaudiHandle
    'MuonSelectionTool' : PublicToolHandle('CP::MuonSelectionTool/MuonSelectionTool'), # GaudiHandle
  }
  _propertyDocDct = { 
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(LeptonSelectionTools, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'DisplacedDimuonAnalysis'
  def getType( self ):
      return 'LeptonSelectionTools'
  pass # class LeptonSelectionTools

class LowMass( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCounter' : 0, # int
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'THistSvc' : ServiceHandle('THistSvc/THistSvc'), # GaudiHandle
    'RootStreamName' : '/', # str
    'RootDirName' : '', # str
    'HistNamePrefix' : '', # str
    'HistNamePostfix' : '', # str
    'HistTitlePrefix' : '', # str
    'HistTitlePostfix' : '', # str
    'DiLepDVCuts' : PublicToolHandle('DDL::DiLepDVCuts/DiLepDVCuts'), # GaudiHandle
    'DVUtils' : PublicToolHandle('DVUtils'), # GaudiHandle
    'LeptonSelectionTool' : PublicToolHandle('LeptonSelectionTools'), # GaudiHandle
    'CosmicTool' : PublicToolHandle('CosmicTools'), # GaudiHandle
    'DiLepEventCuts' : PublicToolHandle('DDL::EventCuts/DiLepEventCuts'), # GaudiHandle
    'DiLepBaseCuts' : PublicToolHandle('DDL::DVCuts/DiLepBaseCuts'), # GaudiHandle
    'GRLTool' : PublicToolHandle('GoodRunsListSelectionTool/GRLTool'), # GaudiHandle
    'TrigDecisionTool' : PublicToolHandle('Trig::TrigDecisionTool/TrigDecisionTool'), # GaudiHandle
    'TrigMatchingTool' : PublicToolHandle('Trig::MatchingTool/TrigMatchingTool'), # GaudiHandle
    'DiLepCosmics' : PublicToolHandle('DDL::DiLepCosmics/DiLepCosmics'), # GaudiHandle
    'OverlapRemoval' : PublicToolHandle('DDL::OverlapRemoval/OverlapRemoval'), # GaudiHandle
  }
  _propertyDocDct = { 
    'HistTitlePrefix' : """ The prefix for the histogram THx title """,
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'RootStreamName' : """ Name of the output ROOT stream (file) that the THistSvc uses """,
    'HistNamePostfix' : """ The postfix for the histogram THx name """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'HistTitlePostfix' : """ The postfix for the histogram THx title """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
    'THistSvc' : """ Handle to a THistSvc instance: it will be used to write ROOT objects to ROOT files """,
    'HistNamePrefix' : """ The prefix for the histogram THx name """,
    'RootDirName' : """ Name of the ROOT directory inside the ROOT file where the histograms will go """,
    'GRLTool' : """ The private GoodRunsListSelectionTool """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(LowMass, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'DisplacedDimuonAnalysis'
  def getType( self ):
      return 'LowMass'
  pass # class LowMass

class MuonPlots( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCounter' : 0, # int
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'THistSvc' : ServiceHandle('THistSvc/THistSvc'), # GaudiHandle
    'RootStreamName' : '/', # str
    'RootDirName' : '', # str
    'HistNamePrefix' : '', # str
    'HistNamePostfix' : '', # str
    'HistTitlePrefix' : '', # str
    'HistTitlePostfix' : '', # str
    'TriggerMatchingTool' : PublicToolHandle('Trig::MatchingTool/MyMatchingTool'), # GaudiHandle
  }
  _propertyDocDct = { 
    'HistTitlePrefix' : """ The prefix for the histogram THx title """,
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'RootStreamName' : """ Name of the output ROOT stream (file) that the THistSvc uses """,
    'HistNamePostfix' : """ The postfix for the histogram THx name """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'HistTitlePostfix' : """ The postfix for the histogram THx title """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
    'THistSvc' : """ Handle to a THistSvc instance: it will be used to write ROOT objects to ROOT files """,
    'HistNamePrefix' : """ The prefix for the histogram THx name """,
    'RootDirName' : """ Name of the ROOT directory inside the ROOT file where the histograms will go """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(MuonPlots, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'DisplacedDimuonAnalysis'
  def getType( self ):
      return 'MuonPlots'
  pass # class MuonPlots

class SUSYSelection( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCounter' : 0, # int
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'THistSvc' : ServiceHandle('THistSvc/THistSvc'), # GaudiHandle
    'RootStreamName' : '/', # str
    'RootDirName' : '', # str
    'HistNamePrefix' : '', # str
    'HistNamePostfix' : '', # str
    'HistTitlePrefix' : '', # str
    'HistTitlePostfix' : '', # str
    'DiLepDVCuts' : PublicToolHandle('DDL::DiLepDVCuts/DiLepDVCuts'), # GaudiHandle
    'DVUtils' : PublicToolHandle('DVUtils'), # GaudiHandle
    'LeptonSelectionTool' : PublicToolHandle('LeptonSelectionTools'), # GaudiHandle
    'CosmicTool' : PublicToolHandle('CosmicTools'), # GaudiHandle
    'DiLepEventCuts' : PublicToolHandle('DDL::EventCuts/DiLepEventCuts'), # GaudiHandle
    'DiLepBaseCuts' : PublicToolHandle('DDL::DVCuts/DiLepBaseCuts'), # GaudiHandle
    'GRLTool' : PublicToolHandle('GoodRunsListSelectionTool/GRLTool'), # GaudiHandle
    'TrigDecisionTool' : PublicToolHandle('Trig::TrigDecisionTool/TrigDecisionTool'), # GaudiHandle
    'DiLepCosmics' : PublicToolHandle('DDL::DiLepCosmics/DiLepCosmics'), # GaudiHandle
    'OverlapRemoval' : PublicToolHandle('DDL::OverlapRemoval/OverlapRemoval'), # GaudiHandle
    'PileupReweightingTool' : PublicToolHandle('CP::PileupReweightingTool/PileupReweightingTool'), # GaudiHandle
  }
  _propertyDocDct = { 
    'HistTitlePrefix' : """ The prefix for the histogram THx title """,
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'RootStreamName' : """ Name of the output ROOT stream (file) that the THistSvc uses """,
    'HistNamePostfix' : """ The postfix for the histogram THx name """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'HistTitlePostfix' : """ The postfix for the histogram THx title """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
    'THistSvc' : """ Handle to a THistSvc instance: it will be used to write ROOT objects to ROOT files """,
    'HistNamePrefix' : """ The prefix for the histogram THx name """,
    'RootDirName' : """ Name of the ROOT directory inside the ROOT file where the histograms will go """,
    'GRLTool' : """ The private GoodRunsListSelectionTool """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(SUSYSelection, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'DisplacedDimuonAnalysis'
  def getType( self ):
      return 'SUSYSelection'
  pass # class SUSYSelection

class TrackingSystematics( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCounter' : 0, # int
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'THistSvc' : ServiceHandle('THistSvc/THistSvc'), # GaudiHandle
    'RootStreamName' : '/', # str
    'RootDirName' : '', # str
    'HistNamePrefix' : '', # str
    'HistNamePostfix' : '', # str
    'HistTitlePrefix' : '', # str
    'HistTitlePostfix' : '', # str
    'DiLepDVCuts' : PublicToolHandle('DDL::DiLepDVCuts/DiLepDVCuts'), # GaudiHandle
    'DVUtils' : PublicToolHandle('DVUtils'), # GaudiHandle
    'LeptonSelectionTool' : PublicToolHandle('LeptonSelectionTools'), # GaudiHandle
    'CosmicTool' : PublicToolHandle('CosmicTools'), # GaudiHandle
    'DiLepEventCuts' : PublicToolHandle('DDL::EventCuts/DiLepEventCuts'), # GaudiHandle
    'DiLepBaseCuts' : PublicToolHandle('DDL::DVCuts/DiLepBaseCuts'), # GaudiHandle
    'GRLTool' : PublicToolHandle('GoodRunsListSelectionTool/GRLTool'), # GaudiHandle
    'TrigDecisionTool' : PublicToolHandle('Trig::TrigDecisionTool/TrigDecisionTool'), # GaudiHandle
    'TrigMatchingTool' : PublicToolHandle('Trig::MatchingTool/TrigMatchingTool'), # GaudiHandle
    'DiLepCosmics' : PublicToolHandle('DDL::DiLepCosmics/DiLepCosmics'), # GaudiHandle
    'OverlapRemoval' : PublicToolHandle('DDL::OverlapRemoval/OverlapRemoval'), # GaudiHandle
  }
  _propertyDocDct = { 
    'HistTitlePrefix' : """ The prefix for the histogram THx title """,
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'RootStreamName' : """ Name of the output ROOT stream (file) that the THistSvc uses """,
    'HistNamePostfix' : """ The postfix for the histogram THx name """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'HistTitlePostfix' : """ The postfix for the histogram THx title """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
    'THistSvc' : """ Handle to a THistSvc instance: it will be used to write ROOT objects to ROOT files """,
    'HistNamePrefix' : """ The prefix for the histogram THx name """,
    'RootDirName' : """ Name of the ROOT directory inside the ROOT file where the histograms will go """,
    'GRLTool' : """ The private GoodRunsListSelectionTool """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(TrackingSystematics, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'DisplacedDimuonAnalysis'
  def getType( self ):
      return 'TrackingSystematics'
  pass # class TrackingSystematics

class TruthPlots( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCounter' : 0, # int
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'THistSvc' : ServiceHandle('THistSvc/THistSvc'), # GaudiHandle
    'RootStreamName' : '/', # str
    'RootDirName' : '', # str
    'HistNamePrefix' : '', # str
    'HistNamePostfix' : '', # str
    'HistTitlePrefix' : '', # str
    'HistTitlePostfix' : '', # str
    'DVUtils' : PublicToolHandle('DVUtils'), # GaudiHandle
    'PileupReweightingTool' : PublicToolHandle('CP::PileupReweightingTool/PileupReweightingTool'), # GaudiHandle
  }
  _propertyDocDct = { 
    'HistTitlePrefix' : """ The prefix for the histogram THx title """,
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'RootStreamName' : """ Name of the output ROOT stream (file) that the THistSvc uses """,
    'HistNamePostfix' : """ The postfix for the histogram THx name """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'HistTitlePostfix' : """ The postfix for the histogram THx title """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
    'THistSvc' : """ Handle to a THistSvc instance: it will be used to write ROOT objects to ROOT files """,
    'HistNamePrefix' : """ The prefix for the histogram THx name """,
    'RootDirName' : """ Name of the ROOT directory inside the ROOT file where the histograms will go """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(TruthPlots, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'DisplacedDimuonAnalysis'
  def getType( self ):
      return 'TruthPlots'
  pass # class TruthPlots

class VertexRes( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCounter' : 0, # int
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'THistSvc' : ServiceHandle('THistSvc/THistSvc'), # GaudiHandle
    'RootStreamName' : '/', # str
    'RootDirName' : '', # str
    'HistNamePrefix' : '', # str
    'HistNamePostfix' : '', # str
    'HistTitlePrefix' : '', # str
    'HistTitlePostfix' : '', # str
    'DiLepDVCuts' : PublicToolHandle('DDL::DiLepDVCuts/DiLepDVCuts'), # GaudiHandle
    'DVUtils' : PublicToolHandle('DVUtils'), # GaudiHandle
    'DiLepEventCuts' : PublicToolHandle('DDL::EventCuts/DiLepEventCuts'), # GaudiHandle
  }
  _propertyDocDct = { 
    'HistTitlePrefix' : """ The prefix for the histogram THx title """,
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'RootStreamName' : """ Name of the output ROOT stream (file) that the THistSvc uses """,
    'HistNamePostfix' : """ The postfix for the histogram THx name """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'HistTitlePostfix' : """ The postfix for the histogram THx title """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
    'THistSvc' : """ Handle to a THistSvc instance: it will be used to write ROOT objects to ROOT files """,
    'HistNamePrefix' : """ The prefix for the histogram THx name """,
    'RootDirName' : """ Name of the ROOT directory inside the ROOT file where the histograms will go """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(VertexRes, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'DisplacedDimuonAnalysis'
  def getType( self ):
      return 'VertexRes'
  pass # class VertexRes
