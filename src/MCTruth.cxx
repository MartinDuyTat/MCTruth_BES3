// Martin Duy Tat 12th February 2021

// MCTruth
#include "MCTruth/MCTruth.h"
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

MCTruth::MCTruth(const std::string& name, ISvcLocator* pSvcLocator): Algorithm(name, pSvcLocator) {
  declareProperty("MCTruthTuple", m_recTruthTuple = true);
}

MCTruth::~MCTruth() {
}

StatusCode MCTruth::initialize() {
  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "Creating MCTruth Double Tag Algorithm" << endreq;
  StatusCode sc;
  if(m_recTruthTuple) {
    sc = createSubAlgorithm("TruthTuple", "TruthTuple", m_TruthTuple);
    if(sc.isFailure()) {
      log << MSG::ERROR << "Error while creating TruthTuple" << endreq;
      return StatusCode::FAILURE;
    }
  }
}

StatusCode MCTruth::execute() {
  for(std::vector<Algorithm*>::const_iterator it = subAlgorithms()->begin(); it != subAlgorithms()->end(); it++) {
    StatusCode sc = (*it)->execute();
    if(sc.isFailure()) {
      return StatusCode::FAILURE;
    }
  }
}

StatusCode MCTruth::finalize() {
  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "MCTruth Algorithm finalized" << endmsg;
  return StatusCode::SUCCESS;
}
