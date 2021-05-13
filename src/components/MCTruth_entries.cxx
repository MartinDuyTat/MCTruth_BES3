#include "GaudiKernel/DeclareFactoryEntries.h"
#include "MCTruth/MCTruth.h"
#include "MCTruth/TruthTuple.h"

DECLARE_ALGORITHM_FACTORY(MCTruth)
DECLARE_ALGORITHM_FACTORY(TruthTuple)

DECLARE_FACTORY_ENTRIES(MCTruth) {
  DECLARE_ALGORITHM(MCTruth)
}


