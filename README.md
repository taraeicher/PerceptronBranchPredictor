This is a perceptron predictor based on "Dynamic Branch Prediction with Perceptrons" by
Jimenez and Lin. It is constrained to use no more than 64 KB memory, and it uses a 59-bit
global history register and a table of 1K perceptrons with 60 weights each. The perceptrons
are indexed by the last 10 bits of the global history register XORed with the last 10 bits
of the PC address.

This branch predictor is meant to be run with the simulator available at
https://drive.google.com/open?id=0B5TPQrtyq9_kRmxmak1ESFF4cXc
To install the simulator, run the following commands:

	tar -xzvf bpc6421AU15.tar.gz
	cd bpc6421AU15
	cd sim
	make ./predictor ../traces/SHORT-INT-1.cbp4.gz
	
After installing the simulator, overwrite the existing predictor.cc and predictor.h files
with the CC and H files in this repository. Run using either runall.pl or doit.sh in the 
scripts directory. You can view your results using the getdata.pl script.

This predictor achieves the following results with respect to misprediction rate:

LONG-SPEC2K6-00		2.893
LONG-SPEC2K6-01		7.129
LONG-SPEC2K6-02		2.578
LONG-SPEC2K6-03		3.739
LONG-SPEC2K6-04		9.296
LONG-SPEC2K6-05		4.973
LONG-SPEC2K6-06		1.157
LONG-SPEC2K6-07		11.713
LONG-SPEC2K6-08		1.115
LONG-SPEC2K6-09		4.643
SHORT-FP-1			1.651
SHORT-FP-2			0.913
SHORT-FP-3			0.437
SHORT-INT-1			1.487
SHORT-INT-2			6.433
SHORT-INT-3			6.601
SHORT-MM-1			7.453
SHORT-MM-2			9.290
SHORT-MM-3			0.799
SHORT-SERV-1		2.011
SHORT-SERV-2		1.963
SHORT-SERV-3		4.971
AMEAN				4.238
