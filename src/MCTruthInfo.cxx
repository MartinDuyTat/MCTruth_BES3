// Martin Duy Tat 12th February 2021

// MCTruthInfo
#include "MCTruthInfo/MCTruthInfo.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/PropertyMgr.h"
#include "GaudiKernel/StatusCode.h"
// STL
#include<string>

DECLARE_COMPONENT(MCTruthInfo)

MCTruthInfo::MCTruthInfo(const std::string& name, ISvcLocator* pSvcLocator): Algorithm(name, pSvcLocator) {
  declareProperty("MCTruthInfoTuple", m_recTruthTuple = true);
}

MCTruthInfo::~MCTruthInfo() {
}

StatusCode MCTruthInfo::initialize() {
  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "Creating MCTruthInfo Double Tag Algorithm" << endreq;
  StatusCode sc;
  if(m_recTruthTuple) {
    sc = createSubAlgorithm("TruthTuple", "TruthTuple", m_TruthTuple);
    if(sc.isFailure()) {
      log << MSG::ERROR << "Error while creating TruthTuple" << endreq;
      return StatusCode::FAILURE;
    }
  }
  return StatusCode::SUCCESS;
}

StatusCode MCTruthInfo::execute() {
  for(std::vector<Algorithm*>::const_iterator it = subAlgorithms()->begin(); it != subAlgorithms()->end(); it++) {
    StatusCode sc = (*it)->execute();
    if(sc.isFailure()) {
      return StatusCode::FAILURE;
    }
  }
  return StatusCode::SUCCESS;
}

StatusCode MCTruthInfo::finalize() {
  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "MCTruthInfo Algorithm finalized" << endmsg;
  return StatusCode::SUCCESS;
}
