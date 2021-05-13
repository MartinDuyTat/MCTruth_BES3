#include "GaudiKernel/DeclareFactoryEntries.h"
#include "MCTruthInfo/MCTruthInfo.h"
#include "MCTruthInfo/TruthTuple.h"

DECLARE_ALGORITHM_FACTORY(MCTruthInfo)
DECLARE_ALGORITHM_FACTORY(TruthTuple)

DECLARE_FACTORY_ENTRIES(MCTruthInfo) {
  DECLARE_ALGORITHM(MCTruthInfo)
}


