#include "predictor.h"
#include <bitset>
#include <stdint.h>


#define TABLE_SIZE 1024 // Determined by threshold and history length
#define THRESHOLD 127 // Floor(1.93 * HIST_LEN + 14) as specified in paper
#define HIST_LEN   59 // Optimal value as determined by paper

/////////////// STORAGE BUDGET JUSTIFICATION ////////////////
// Total storage budget: 64KB = 524288 bits
// Total bits in history register:  59 bits
// Total number of weights per perceptron:  59 + 1 = 60
// Total bits to store weight: 8
// Total number of perceptrons: 1024 
// Total perceptron table size = 1024 * 60 * 8 = 524160 bits
// Total perceptron steps table size = 1024 * 8
// Total Size = perceptron table size + history register size
//       + perceptron steps table = 523776 bits
/////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

PREDICTOR::PREDICTOR(void){

  // Given the history length and table size, construct the ghr
  // and perceptron table.

  perceptronSteps = 0;
  ghr = bitset<HIST_LEN>();

  // Initialize each entry in the perceptron table to a value of
  // zero. Initialize number of steps executed for each perceptron to zero.

  for(UINT32 i=0; i < TABLE_SIZE; i++){
    for(UINT32 j=0; j < HIST_LEN; j++){
	  perceptronTable[i][j] = 0;
    }
  }
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

bool   PREDICTOR::GetPrediction(UINT32 PC){

  UINT32 perceptronIndex = HashPC(PC);
  INT32 prediction = 0;

  // Calculate prediction based on selected perceptron and global history.
  // First add the bias, then all other weights.

  prediction += perceptronTable[perceptronIndex][0]; 
  for(UINT32 i=1; i < HIST_LEN + 1; i++){
	  
    // If history bit is taken, add the weight to the prediction.
    // Else, subtract the weight.

    if(ghr[i - 1] == 1){
	  prediction += perceptronTable[perceptronIndex][i];   
    }
    else{
	  prediction -= perceptronTable[perceptronIndex][i]; 
    }
  }

  // Update perceptron steps to absolute value of the prediction.

  perceptronSteps = abs(prediction);

  // If the prediction is negative, predict not taken. If it is positive,
  // predict taken. Assume zero is positive.
 
 if(prediction >= 0){
    return TAKEN; 
  }
  else{
    return NOT_TAKEN; 
  }  
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void  PREDICTOR::UpdatePredictor(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget){

  UINT32 perceptronIndex = HashPC(PC);

  // Update the perceptron table entry only if the threshold has not been
  // reached or the predicted and true outcomes disagree. Update the bias first, then the weights.
  // Correct for overflow.

  if(resolveDir != predDir || perceptronSteps <= THRESHOLD){
    
    // If the branch was taken, increment the bias value. Else, decrement it.

    if(resolveDir == TAKEN){
	  int32_t updateVal = perceptronTable[perceptronIndex][0] + 1;
	  if (updateVal > 128){
		  perceptronTable[perceptronIndex][0] = 128;
	  }
	  else{
		 perceptronTable[perceptronIndex][0]++; 
	  }
	}
    else{
	  int32_t updateVal = perceptronTable[perceptronIndex][0] - 1;
	  if (updateVal < -128){
		  perceptronTable[perceptronIndex][0] = -128;
	  }
	  else{
		  perceptronTable[perceptronIndex][0]--;
	  } 
	}
	   
	// Update the weights.

	for(UINT32 i = 1; i < HIST_LEN + 1; i++){

       // If the branch outcome matches the history bit, increment the weight value.
	   // Else, decrement the weight value.

       if((resolveDir == TAKEN && ghr[i - 1] == 1) || (resolveDir == NOT_TAKEN && ghr[i - 1] == 0)){
		 int32_t updateVal = perceptronTable[perceptronIndex][i] + 1;
	     if (updateVal > THRESHOLD){
		   perceptronTable[perceptronIndex][i] = THRESHOLD;
	     }
	      else{
			  perceptronTable[perceptronIndex][i]++;
		  }
       }
	   else{
	     int32_t updateVal = perceptronTable[perceptronIndex][i] - 1;
	     if (updateVal < THRESHOLD * (-1)){
		   perceptronTable[perceptronIndex][i] = THRESHOLD * (-1);
	     }
		 else{
			 perceptronTable[perceptronIndex][i]--;
		 }
	   }
	}	   
  }

  // update the GHR by shifting left and setting new bit.
  ghr = (ghr << 1);
  if(resolveDir == TAKEN){
    ghr.set(0, 1); 
  }
  else{
	ghr.set(0, 0);
  }
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void    PREDICTOR::TrackOtherInst(UINT32 PC, OpType opType, UINT32 branchTarget){

  // This function is called for instructions which are not
  // conditional branches, just in case someone decides to design
  // a predictor that uses information from such instructions.
  // We expect most contestants to leave this function untouched.

  return;
}

/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

UINT32 PREDICTOR::HashPC(UINT32 PC){
	
	// Hash the PC so that it can be used as an index for the perceptron table.
	UINT32 PCend = PC % TABLE_SIZE;
	UINT32 ghrend = ((UINT32)ghr.to_ulong()) % TABLE_SIZE;
	return PCend ^ ghrend;
}
