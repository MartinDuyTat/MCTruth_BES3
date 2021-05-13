// Martin Duy Tat 12th February 2021
/**
 * MCTruth is a BOSS algorithm
 * It saves all the generator information from MC DST files
 */

#ifndef MCTRUTH
#define MCTRUTH

// Gaudi
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/NTuple.h"
// STL
#include<string>

class MCTruth: public Algorithm {
  public:
    /**
     * Default constructor for an algorithm where all necessary properties are declared
     */
    MCTruth(const std::string& name, ISvcLocator* pSvcLocator);
    /**
     * Trivial destructor
     */
    ~MCTruth();
    /**
     * This function runs when algorithm is initialized
     */
    StatusCode initialize();
    /**
     * Execution of the algorithm
     */
    StatusCode execute();
    /**
     * This function runs when algorithm is finalized
     */
    StatusCode finalize();
  private:
    /**
     * Truth tuple algorithm
     */
    Algorithm *m_TruthTuple;
    /**
     * Turn on Truth tuple algorithm
     */
    bool m_recTruthTuple;
};

#endif
