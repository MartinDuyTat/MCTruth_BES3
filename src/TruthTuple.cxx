// Martin Duy Tat 13th May 2021

// MCTruthInfo
#include "MCTruthInfo/TruthTuple.h"
#include "MCTruthInfo/FindMCInfo.h"
// Gaudi
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/NTuple.h"
#include "GaudiKernel/SmartDataPtr.h"
// Event information
#include "EventModel/EventHeader.h"
// Boss
#include "McDecayModeSvc/McDecayModeSvc.h"
#include "McTruth/McParticle.h"
// STL
#include<vector>
#include<string>

TruthTuple::TruthTuple(const std::string &name, ISvcLocator *pSvcLocator): Algorithm(name, pSvcLocator) {
  declareProperty("dummy", m_dummy = 0);
}

TruthTuple::~TruthTuple() {
}

StatusCode TruthTuple::initialize() {
  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "Initializing MC truth tuple" << endreq;
  StatusCode status;
  NTuplePtr ntp(ntupleSvc(), "MCTRUTHINFO/TruthTuple");
  if(ntp) {
    m_tuple = ntp;
  } else {
    m_tuple = ntupleSvc()->book("MCTRUTHINFO/TruthTuple", CLID_ColumnWiseTuple, "Tuple with generator level information");
    if(m_tuple) {
      status = m_tuple->addItem("Run", m_RunNumber);
      status = m_tuple->addItem("Event", m_EventNumber);
      status = m_tuple->addItem("NumberOfParticles", m_NumberParticles, 0, 100);
      status = m_tuple->addIndexedItem("ParticleIDs", m_NumberParticles, m_pdgID);
      status = m_tuple->addIndexedItem("MotherIndex", m_NumberParticles, m_MotherIndex);
      status = m_tuple->addItem("NumberOfParticlesStripped", m_NumberParticlesStripped, 0, 100);
      status = m_tuple->addIndexedItem("ParticleIDsStripped", m_NumberParticlesStripped, m_pdgIDStripped);
      status = m_tuple->addIndexedItem("MotherIndexStripped", m_NumberParticlesStripped, m_MotherIndexStripped);
      status = m_tuple->addItem("MCmode", m_MCmode);
      status = m_tuple->addIndexedItem("True_Px", m_NumberParticles, m_TruePx);
      status = m_tuple->addIndexedItem("True_Py", m_NumberParticles, m_TruePy);
      status = m_tuple->addIndexedItem("True_Pz", m_NumberParticles, m_TruePz);
      status = m_tuple->addIndexedItem("True_Energy", m_NumberParticles, m_TrueEnergy);
    } else {
      log << MSG::ERROR << "Cannot book NTuple for MC generator level information" << endmsg;
      return StatusCode::FAILURE;
    }
    return StatusCode::SUCCESS;
  }
}

StatusCode TruthTuple::execute() {
  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "Executing TruthTuple Algorithm" << endreq;
  SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(), "/Event/EventHeader");
  m_RunNumber = eventHeader->runNumber();
  m_EventNumber = eventHeader->eventNumber();
  StatusCode FillTupleStatus = FillTuple();
  if(FillTupleStatus != StatusCode::SUCCESS) {
    log << MSG::FATAL << "Assigning tuple info failed" << endreq;
    return StatusCode::FAILURE;
  }
  m_tuple->write();
  return StatusCode::SUCCESS;
}

StatusCode TruthTuple::finalize() {
  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "Finalizing Truth Tuple" << endreq;
  return StatusCode::SUCCESS;
}

StatusCode TruthTuple::FillTuple() {
  if(m_RunNumber < 0) {
    SmartDataPtr<Event::McParticleCol> MCParticleCol(eventSvc(), "/Event/MC/McParticleCol");
    if(!MCParticleCol) {
      return StatusCode::FAILURE;
    }
    IMcDecayModeSvc *IMcDecayModeService;
    StatusCode McDecayModeSVC_Status = service("McDecayModeSvc", IMcDecayModeService);
    if(McDecayModeSVC_Status.isFailure()) {
      return StatusCode::FAILURE;
    }
    FindMCInfo findMCInfo;
    StatusCode MCStatus = findMCInfo.CalculateMCInfo(MCParticleCol, IMcDecayModeService);
    if(MCStatus != StatusCode::SUCCESS) {
      return MCStatus;
    }
    m_NumberParticles = findMCInfo.GetNumberParticles();
    m_MCmode = findMCInfo.GetMCmode();
    for(int i = 0; i < m_NumberParticles; i++) {
      m_pdgID[i] = findMCInfo.GetpdgID(i);
      m_MotherIndex[i] = findMCInfo.GetMotherIndex(i);
      m_TruePx[i] = findMCInfo.GetTruePx(i);
      m_TruePy[i] = findMCInfo.GetTruePy(i);
      m_TruePz[i] = findMCInfo.GetTruePz(i);
      m_TrueEnergy[i] = findMCInfo.GetTrueEnergy(i);
    }
    m_NumberParticlesStripped = findMCInfo.GetNumberParticlesStripped();
    for(int i = 0; i < m_NumberParticlesStripped; i++) {
      m_pdgIDStripped[i] = findMCInfo.GetpdgIDStripped(i);
      m_MotherIndexStripped[i] = findMCInfo.GetMotherIndexStripped(i);
    }
  }
  return StatusCode::SUCCESS;
}
