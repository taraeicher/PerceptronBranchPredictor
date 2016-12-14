#ifndef _PREDICTOR_H_
#define _PREDICTOR_H_

#include "utils.h"
#include "tracer.h"
#include <bitset>
#include <stdint.h>

#define TABLE_SIZE 1024 // 2^10
#define HIST_LEN   59 // Optimal value as determined by paper

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

class PREDICTOR{

  // The state is defined for perceptron predictor.

 private:

  bitset<HIST_LEN> ghr;           // global history register
  UINT32 perceptronSteps;
  UINT32 perceptronTable[TABLE_SIZE][HIST_LEN + 1]; //table of perceptrons
  
 public:

  // The interface to the four functions below CAN NOT be changed

  PREDICTOR(void);
  bool    GetPrediction(UINT32 PC);  
  void    UpdatePredictor(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget);
  void    TrackOtherInst(UINT32 PC, OpType opType, UINT32 branchTarget);

  // Contestants can define their own functions below

  UINT32 HashPC(UINT32 PC);
};



/***********************************************************/
#endif

