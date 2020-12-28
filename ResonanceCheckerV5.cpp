//This code is not the same as v1. Remaking the resonence code to be able to act alone so pos growth models and non pos growth may be looked at.

//V2: Attempting to make code output resonence data. Unfinished. Need to work out way to output ResonenceRatios to correct file with or without cut on data. Also want overall average ratio?
///Note: So far its finished?
///Note: Add collection of near resonances to smolec? Also eventually want bulk data output?
///Note: Must add conditions for things only working when FU period is positive as well. Also need condition that does not output 1 to file if no pos growth rate models were found for a mode

//V3: Added conditions for pos FU period and when no pos growth rates were found. This results in a lot less data! Make collection of close resoneces?	

//V4: Adding Check with all other modes 
///Note: Need to run through modes and get all periods first. Then run through loop that calculates the resonances.
///All FU_mode varibles now changed, now need to add each modes resonance calc and variables/arrays
///All done?!? - Running to check no problems with copy paste and replace. Also note that all varibles have the form: Name_ModeBeingCheckedWith_ModeBeingChecked 

//V5: Fixing some things with output - Looks like thirdO was done two times to many. Need to delete these and carry that down for all below. Also changed spacing from 20 to 30.
///Note: Still outputing -1 due to illegal inf when dividing by 0. This can be fixed by making if/else statments first and asigning a varible to be outputted - lots of work though
///EightO Check must be broken, no output when there should be Pos growth models - Fixed? Typo in assignment of growth_EighthO. Running and checking...
///Program output seems good now. Add standard deviation calc for each mode combination?? -V6?
///2O output is wrong, there is a bug somewhere

# include <iostream>
# include <fstream>
# include <math.h>
# include <iomanip>
# include <cmath>
# include <stdlib.h>
# include <cstdlib>
//# include <fstream.h>
# include <string.h>
# include <string>
//# include <dos.h> //For Sleep() 

using namespace std;
int main(){
	
	//Controls//

	int NumOfSets = 4; //Number of sets, usually 4 for A, B, C, D. Max is 4 sets currently
	int NumOfModels = 20412; //Total possible number of models. 20412 is total TypeII models. 16281 is Classical Cep. 15876 is RRL.
	int NumOfModes = 10; //Number of modes investigated. Max is 10. Must have the positive growth files named accordingly with prefix and suffix below!
	
	char outputFileName[50] = "ResonanceData"; //output file with resonance data in it. Will have prefix for mode defined below as well as the set suffix.
	
	char LINAFileName[30] = "LINA_period_growth.data"; //Normally should be "LINA_period_growth.data"
	char log_directory_prefix_SetA[100] = "LINA_reruns/SetA/LOGS/logs_"; //Prefix to log_directory, suffix is model number. This is where LINA file should be
	char log_directory_prefix_SetB[100] = "LINA_reruns/SetB/LOGS/logs_"; //Prefix to log_directory, suffix is model number. This is where LINA file should be
	char log_directory_prefix_SetC[100] = "LINA_reruns/SetC/LOGS/logs_"; //Prefix to log_directory, suffix is model number. This is where LINA file should be
	char log_directory_prefix_SetD[100] = "LINA_reruns/SetD/LOGS/logs_"; //Prefix to log_directory, suffix is model number. This is where LINA file should be
	
	char FU_FilePrefix[50] = "FU_"; //Prefix to input/output files that contain data for FU mode
	char FO_FilePrefix[50] = "FO_"; //Prefix to input/output files that contain data for FO mode
	char SO_FilePrefix[50] = "2O_"; //Prefix to input/output files that contain data for 2O mode
	char ThirdO_FilePrefix[50] = "ThirdO_"; //Prefix to input/output files that contain data for ThirdO mode
	char ForthO_FilePrefix[50] = "ForthO_"; //Prefix to input/output files that contain data for ForthO mode
	char FifthO_FilePrefix[50] = "FifthO_"; //Prefix to input/output files that contain data for FifthO mode
	char SixthO_FilePrefix[50] = "SixthO_"; //Prefix to input/output files that contain data for SixthO mode
	char SeventhO_FilePrefix[50] = "SeventhO_"; //Prefix to input/output files that contain data for SeventhO mode
	char EighthO_FilePrefix[50] = "EighthO_"; //Prefix to input/output files that contain data for EighthO mode
	char NinthO_FilePrefix[50] = "NinthO_"; //Prefix to input/output files that contain data for NinthO mode
	
	char SetA_Suffix[10] = "_SetA.dat"; //Suffix to input/output file for SetA
	char SetB_Suffix[10] = "_SetB.dat"; //Suffix to input/output file for SetB
	char SetC_Suffix[10] = "_SetC.dat"; //Suffix to input/output file for SetC
	char SetD_Suffix[10] = "_SetD.dat"; //Suffix to input/output file for SetD
	
	bool DoPosGrowthCut = false; //If true, will only get positive growth models (Still mode being checked with will only be positive)
	
	///////////////////////////////////////
	
	//Variables//
	
	const int nArray = NumOfModels + 1;
	
	char FU_FileName[100];
	char FO_FileName[100];
	char SO_FileName[100];
	char ThirdO_FileName[100];
	char ForthO_FileName[100];
	char FifthO_FileName[100];
	char SixthO_FileName[100];
	char SeventhO_FileName[100];
	char EighthO_FileName[100];
	char NinthO_FileName[100];
	
	char FU_FullOutputFileName[100];
	char FO_FullOutputFileName[100];
	char SO_FullOutputFileName[100];
	char ThirdO_FullOutputFileName[100];
	char ForthO_FullOutputFileName[100];
	char FifthO_FullOutputFileName[100];
	char SixthO_FullOutputFileName[100];
	char SeventhO_FullOutputFileName[100];
	char EighthO_FullOutputFileName[100];
	char NinthO_FullOutputFileName[100];
	
	///////////////////////////////////////////
	
	double ResonanceRatio_FU_FU_SetA[nArray];
	double ResonanceRatio_FU_FO_SetA[nArray];
	double ResonanceRatio_FU_2O_SetA[nArray];
	double ResonanceRatio_FU_ThirdO_SetA[nArray];
	double ResonanceRatio_FU_ForthO_SetA[nArray];
	double ResonanceRatio_FU_FifthO_SetA[nArray];
	double ResonanceRatio_FU_SixthO_SetA[nArray];
	double ResonanceRatio_FU_SeventhO_SetA[nArray];
	double ResonanceRatio_FU_EighthO_SetA[nArray];
	double ResonanceRatio_FU_NinthO_SetA[nArray];
	
	double ResonanceRatio_FU_FU_SetB[nArray];
	double ResonanceRatio_FU_FO_SetB[nArray];
	double ResonanceRatio_FU_2O_SetB[nArray];
	double ResonanceRatio_FU_ThirdO_SetB[nArray];
	double ResonanceRatio_FU_ForthO_SetB[nArray];
	double ResonanceRatio_FU_FifthO_SetB[nArray];
	double ResonanceRatio_FU_SixthO_SetB[nArray];
	double ResonanceRatio_FU_SeventhO_SetB[nArray];
	double ResonanceRatio_FU_EighthO_SetB[nArray];
	double ResonanceRatio_FU_NinthO_SetB[nArray];
	
	double ResonanceRatio_FU_FU_SetC[nArray];
	double ResonanceRatio_FU_FO_SetC[nArray];
	double ResonanceRatio_FU_2O_SetC[nArray];
	double ResonanceRatio_FU_ThirdO_SetC[nArray];
	double ResonanceRatio_FU_ForthO_SetC[nArray];
	double ResonanceRatio_FU_FifthO_SetC[nArray];
	double ResonanceRatio_FU_SixthO_SetC[nArray];
	double ResonanceRatio_FU_SeventhO_SetC[nArray];
	double ResonanceRatio_FU_EighthO_SetC[nArray];
	double ResonanceRatio_FU_NinthO_SetC[nArray];
	
	double ResonanceRatio_FU_FU_SetD[nArray];
	double ResonanceRatio_FU_FO_SetD[nArray];
	double ResonanceRatio_FU_2O_SetD[nArray];
	double ResonanceRatio_FU_ThirdO_SetD[nArray];
	double ResonanceRatio_FU_ForthO_SetD[nArray];
	double ResonanceRatio_FU_FifthO_SetD[nArray];
	double ResonanceRatio_FU_SixthO_SetD[nArray];
	double ResonanceRatio_FU_SeventhO_SetD[nArray];
	double ResonanceRatio_FU_EighthO_SetD[nArray];
	double ResonanceRatio_FU_NinthO_SetD[nArray];
	
	double OverallResonance_FU_FU_SetA = 0;
	double AverageResonance_FU_FU_SetA;
	double OverallResonance_FU_FU_SetB = 0;
	double AverageResonance_FU_FU_SetB;
	double OverallResonance_FU_FU_SetC = 0;
	double AverageResonance_FU_FU_SetC;
	double OverallResonance_FU_FU_SetD = 0;
	double AverageResonance_FU_FU_SetD;
	
	double OverallResonance_FU_FO_SetA = 0;
	double AverageResonance_FU_FO_SetA;
	double OverallResonance_FU_FO_SetB = 0;
	double AverageResonance_FU_FO_SetB;
	double OverallResonance_FU_FO_SetC = 0;
	double AverageResonance_FU_FO_SetC;
	double OverallResonance_FU_FO_SetD = 0;
	double AverageResonance_FU_FO_SetD;
	
	double OverallResonance_FU_2O_SetA = 0;
	double AverageResonance_FU_2O_SetA;
	double OverallResonance_FU_2O_SetB = 0;
	double AverageResonance_FU_2O_SetB;
	double OverallResonance_FU_2O_SetC = 0;
	double AverageResonance_FU_2O_SetC;
	double OverallResonance_FU_2O_SetD = 0;
	double AverageResonance_FU_2O_SetD;
	
	double OverallResonance_FU_ThirdO_SetA = 0;
	double AverageResonance_FU_ThirdO_SetA;
	double OverallResonance_FU_ThirdO_SetB = 0;
	double AverageResonance_FU_ThirdO_SetB;
	double OverallResonance_FU_ThirdO_SetC = 0;
	double AverageResonance_FU_ThirdO_SetC;
	double OverallResonance_FU_ThirdO_SetD = 0;
	double AverageResonance_FU_ThirdO_SetD;
	
	double OverallResonance_FU_ForthO_SetA = 0;
	double AverageResonance_FU_ForthO_SetA;
	double OverallResonance_FU_ForthO_SetB = 0;
	double AverageResonance_FU_ForthO_SetB;
	double OverallResonance_FU_ForthO_SetC = 0;
	double AverageResonance_FU_ForthO_SetC;
	double OverallResonance_FU_ForthO_SetD = 0;
	double AverageResonance_FU_ForthO_SetD;
	
	double OverallResonance_FU_FifthO_SetA = 0;
	double AverageResonance_FU_FifthO_SetA;
	double OverallResonance_FU_FifthO_SetB = 0;
	double AverageResonance_FU_FifthO_SetB;
	double OverallResonance_FU_FifthO_SetC = 0;
	double AverageResonance_FU_FifthO_SetC;
	double OverallResonance_FU_FifthO_SetD = 0;
	double AverageResonance_FU_FifthO_SetD;
	
	double OverallResonance_FU_SixthO_SetA = 0;
	double AverageResonance_FU_SixthO_SetA;
	double OverallResonance_FU_SixthO_SetB = 0;
	double AverageResonance_FU_SixthO_SetB;
	double OverallResonance_FU_SixthO_SetC = 0;
	double AverageResonance_FU_SixthO_SetC;
	double OverallResonance_FU_SixthO_SetD = 0;
	double AverageResonance_FU_SixthO_SetD;
	
	double OverallResonance_FU_SeventhO_SetA = 0;
	double AverageResonance_FU_SeventhO_SetA;
	double OverallResonance_FU_SeventhO_SetB = 0;
	double AverageResonance_FU_SeventhO_SetB;
	double OverallResonance_FU_SeventhO_SetC = 0;
	double AverageResonance_FU_SeventhO_SetC;
	double OverallResonance_FU_SeventhO_SetD = 0;
	double AverageResonance_FU_SeventhO_SetD;
	
	double OverallResonance_FU_EighthO_SetA = 0;
	double AverageResonance_FU_EighthO_SetA;
	double OverallResonance_FU_EighthO_SetB = 0;
	double AverageResonance_FU_EighthO_SetB;
	double OverallResonance_FU_EighthO_SetC = 0;
	double AverageResonance_FU_EighthO_SetC;
	double OverallResonance_FU_EighthO_SetD = 0;
	double AverageResonance_FU_EighthO_SetD;
	
	double OverallResonance_FU_NinthO_SetA = 0;
	double AverageResonance_FU_NinthO_SetA;
	double OverallResonance_FU_NinthO_SetB = 0;
	double AverageResonance_FU_NinthO_SetB;
	double OverallResonance_FU_NinthO_SetC = 0;
	double AverageResonance_FU_NinthO_SetC;
	double OverallResonance_FU_NinthO_SetD = 0;
	double AverageResonance_FU_NinthO_SetD;
	
	int NumOfFU_FU_SetA = 0;
	int NumOfFU_FU_SetB = 0;
	int NumOfFU_FU_SetC = 0;
	int NumOfFU_FU_SetD = 0;
	
	int NumOfFU_FO_SetA = 0;
	int NumOfFU_FO_SetB = 0;
	int NumOfFU_FO_SetC = 0;
	int NumOfFU_FO_SetD = 0;
	
	int NumOfFU_2O_SetA = 0;
	int NumOfFU_2O_SetB = 0;
	int NumOfFU_2O_SetC = 0;
	int NumOfFU_2O_SetD = 0;
	
	int NumOfFU_ThirdO_SetA = 0;
	int NumOfFU_ThirdO_SetB = 0;
	int NumOfFU_ThirdO_SetC = 0;
	int NumOfFU_ThirdO_SetD = 0;
	
	int NumOfFU_ForthO_SetA = 0;
	int NumOfFU_ForthO_SetB = 0;
	int NumOfFU_ForthO_SetC = 0;
	int NumOfFU_ForthO_SetD = 0;
	
	int NumOfFU_FifthO_SetA = 0;
	int NumOfFU_FifthO_SetB = 0;
	int NumOfFU_FifthO_SetC = 0;
	int NumOfFU_FifthO_SetD = 0;
	
	int NumOfFU_SixthO_SetA = 0;
	int NumOfFU_SixthO_SetB = 0;
	int NumOfFU_SixthO_SetC = 0;
	int NumOfFU_SixthO_SetD = 0;
	
	int NumOfFU_SeventhO_SetA = 0;
	int NumOfFU_SeventhO_SetB = 0;
	int NumOfFU_SeventhO_SetC = 0;
	int NumOfFU_SeventhO_SetD = 0;
	
	int NumOfFU_EighthO_SetA = 0;
	int NumOfFU_EighthO_SetB = 0;
	int NumOfFU_EighthO_SetC = 0;
	int NumOfFU_EighthO_SetD = 0;
	
	int NumOfFU_NinthO_SetA = 0;
	int NumOfFU_NinthO_SetB = 0;
	int NumOfFU_NinthO_SetC = 0;
	int NumOfFU_NinthO_SetD = 0;
	
	
	/////////////////////////////////////////
	
	double ResonanceRatio_FO_FU_SetA[nArray];
	double ResonanceRatio_FO_FO_SetA[nArray];
	double ResonanceRatio_FO_2O_SetA[nArray];
	double ResonanceRatio_FO_ThirdO_SetA[nArray];
	double ResonanceRatio_FO_ForthO_SetA[nArray];
	double ResonanceRatio_FO_FifthO_SetA[nArray];
	double ResonanceRatio_FO_SixthO_SetA[nArray];
	double ResonanceRatio_FO_SeventhO_SetA[nArray];
	double ResonanceRatio_FO_EighthO_SetA[nArray];
	double ResonanceRatio_FO_NinthO_SetA[nArray];
	
	double ResonanceRatio_FO_FU_SetB[nArray];
	double ResonanceRatio_FO_FO_SetB[nArray];
	double ResonanceRatio_FO_2O_SetB[nArray];
	double ResonanceRatio_FO_ThirdO_SetB[nArray];
	double ResonanceRatio_FO_ForthO_SetB[nArray];
	double ResonanceRatio_FO_FifthO_SetB[nArray];
	double ResonanceRatio_FO_SixthO_SetB[nArray];
	double ResonanceRatio_FO_SeventhO_SetB[nArray];
	double ResonanceRatio_FO_EighthO_SetB[nArray];
	double ResonanceRatio_FO_NinthO_SetB[nArray];
	
	double ResonanceRatio_FO_FU_SetC[nArray];
	double ResonanceRatio_FO_FO_SetC[nArray];
	double ResonanceRatio_FO_2O_SetC[nArray];
	double ResonanceRatio_FO_ThirdO_SetC[nArray];
	double ResonanceRatio_FO_ForthO_SetC[nArray];
	double ResonanceRatio_FO_FifthO_SetC[nArray];
	double ResonanceRatio_FO_SixthO_SetC[nArray];
	double ResonanceRatio_FO_SeventhO_SetC[nArray];
	double ResonanceRatio_FO_EighthO_SetC[nArray];
	double ResonanceRatio_FO_NinthO_SetC[nArray];
	
	double ResonanceRatio_FO_FU_SetD[nArray];
	double ResonanceRatio_FO_FO_SetD[nArray];
	double ResonanceRatio_FO_2O_SetD[nArray];
	double ResonanceRatio_FO_ThirdO_SetD[nArray];
	double ResonanceRatio_FO_ForthO_SetD[nArray];
	double ResonanceRatio_FO_FifthO_SetD[nArray];
	double ResonanceRatio_FO_SixthO_SetD[nArray];
	double ResonanceRatio_FO_SeventhO_SetD[nArray];
	double ResonanceRatio_FO_EighthO_SetD[nArray];
	double ResonanceRatio_FO_NinthO_SetD[nArray];
	
	double OverallResonance_FO_FU_SetA = 0;
	double AverageResonance_FO_FU_SetA;
	double OverallResonance_FO_FU_SetB = 0;
	double AverageResonance_FO_FU_SetB;
	double OverallResonance_FO_FU_SetC = 0;
	double AverageResonance_FO_FU_SetC;
	double OverallResonance_FO_FU_SetD = 0;
	double AverageResonance_FO_FU_SetD;
	
	double OverallResonance_FO_FO_SetA = 0;
	double AverageResonance_FO_FO_SetA;
	double OverallResonance_FO_FO_SetB = 0;
	double AverageResonance_FO_FO_SetB;
	double OverallResonance_FO_FO_SetC = 0;
	double AverageResonance_FO_FO_SetC;
	double OverallResonance_FO_FO_SetD = 0;
	double AverageResonance_FO_FO_SetD;
	
	double OverallResonance_FO_2O_SetA = 0;
	double AverageResonance_FO_2O_SetA;
	double OverallResonance_FO_2O_SetB = 0;
	double AverageResonance_FO_2O_SetB;
	double OverallResonance_FO_2O_SetC = 0;
	double AverageResonance_FO_2O_SetC;
	double OverallResonance_FO_2O_SetD = 0;
	double AverageResonance_FO_2O_SetD;
	
	double OverallResonance_FO_ThirdO_SetA = 0;
	double AverageResonance_FO_ThirdO_SetA;
	double OverallResonance_FO_ThirdO_SetB = 0;
	double AverageResonance_FO_ThirdO_SetB;
	double OverallResonance_FO_ThirdO_SetC = 0;
	double AverageResonance_FO_ThirdO_SetC;
	double OverallResonance_FO_ThirdO_SetD = 0;
	double AverageResonance_FO_ThirdO_SetD;
	
	double OverallResonance_FO_ForthO_SetA = 0;
	double AverageResonance_FO_ForthO_SetA;
	double OverallResonance_FO_ForthO_SetB = 0;
	double AverageResonance_FO_ForthO_SetB;
	double OverallResonance_FO_ForthO_SetC = 0;
	double AverageResonance_FO_ForthO_SetC;
	double OverallResonance_FO_ForthO_SetD = 0;
	double AverageResonance_FO_ForthO_SetD;
	
	double OverallResonance_FO_FifthO_SetA = 0;
	double AverageResonance_FO_FifthO_SetA;
	double OverallResonance_FO_FifthO_SetB = 0;
	double AverageResonance_FO_FifthO_SetB;
	double OverallResonance_FO_FifthO_SetC = 0;
	double AverageResonance_FO_FifthO_SetC;
	double OverallResonance_FO_FifthO_SetD = 0;
	double AverageResonance_FO_FifthO_SetD;
	
	double OverallResonance_FO_SixthO_SetA = 0;
	double AverageResonance_FO_SixthO_SetA;
	double OverallResonance_FO_SixthO_SetB = 0;
	double AverageResonance_FO_SixthO_SetB;
	double OverallResonance_FO_SixthO_SetC = 0;
	double AverageResonance_FO_SixthO_SetC;
	double OverallResonance_FO_SixthO_SetD = 0;
	double AverageResonance_FO_SixthO_SetD;
	
	double OverallResonance_FO_SeventhO_SetA = 0;
	double AverageResonance_FO_SeventhO_SetA;
	double OverallResonance_FO_SeventhO_SetB = 0;
	double AverageResonance_FO_SeventhO_SetB;
	double OverallResonance_FO_SeventhO_SetC = 0;
	double AverageResonance_FO_SeventhO_SetC;
	double OverallResonance_FO_SeventhO_SetD = 0;
	double AverageResonance_FO_SeventhO_SetD;
	
	double OverallResonance_FO_EighthO_SetA = 0;
	double AverageResonance_FO_EighthO_SetA;
	double OverallResonance_FO_EighthO_SetB = 0;
	double AverageResonance_FO_EighthO_SetB;
	double OverallResonance_FO_EighthO_SetC = 0;
	double AverageResonance_FO_EighthO_SetC;
	double OverallResonance_FO_EighthO_SetD = 0;
	double AverageResonance_FO_EighthO_SetD;
	
	double OverallResonance_FO_NinthO_SetA = 0;
	double AverageResonance_FO_NinthO_SetA;
	double OverallResonance_FO_NinthO_SetB = 0;
	double AverageResonance_FO_NinthO_SetB;
	double OverallResonance_FO_NinthO_SetC = 0;
	double AverageResonance_FO_NinthO_SetC;
	double OverallResonance_FO_NinthO_SetD = 0;
	double AverageResonance_FO_NinthO_SetD;
	
	int NumOfFO_FU_SetA = 0;
	int NumOfFO_FU_SetB = 0;
	int NumOfFO_FU_SetC = 0;
	int NumOfFO_FU_SetD = 0;
	
	int NumOfFO_FO_SetA = 0;
	int NumOfFO_FO_SetB = 0;
	int NumOfFO_FO_SetC = 0;
	int NumOfFO_FO_SetD = 0;
	
	int NumOfFO_2O_SetA = 0;
	int NumOfFO_2O_SetB = 0;
	int NumOfFO_2O_SetC = 0;
	int NumOfFO_2O_SetD = 0;
	
	int NumOfFO_ThirdO_SetA = 0;
	int NumOfFO_ThirdO_SetB = 0;
	int NumOfFO_ThirdO_SetC = 0;
	int NumOfFO_ThirdO_SetD = 0;
	
	int NumOfFO_ForthO_SetA = 0;
	int NumOfFO_ForthO_SetB = 0;
	int NumOfFO_ForthO_SetC = 0;
	int NumOfFO_ForthO_SetD = 0;
	
	int NumOfFO_FifthO_SetA = 0;
	int NumOfFO_FifthO_SetB = 0;
	int NumOfFO_FifthO_SetC = 0;
	int NumOfFO_FifthO_SetD = 0;
	
	int NumOfFO_SixthO_SetA = 0;
	int NumOfFO_SixthO_SetB = 0;
	int NumOfFO_SixthO_SetC = 0;
	int NumOfFO_SixthO_SetD = 0;
	
	int NumOfFO_SeventhO_SetA = 0;
	int NumOfFO_SeventhO_SetB = 0;
	int NumOfFO_SeventhO_SetC = 0;
	int NumOfFO_SeventhO_SetD = 0;
	
	int NumOfFO_EighthO_SetA = 0;
	int NumOfFO_EighthO_SetB = 0;
	int NumOfFO_EighthO_SetC = 0;
	int NumOfFO_EighthO_SetD = 0;
	
	int NumOfFO_NinthO_SetA = 0;
	int NumOfFO_NinthO_SetB = 0;
	int NumOfFO_NinthO_SetC = 0;
	int NumOfFO_NinthO_SetD = 0;
	
	////////////////////////////////////////////
	
	double ResonanceRatio_2O_FU_SetA[nArray];
	double ResonanceRatio_2O_FO_SetA[nArray];
	double ResonanceRatio_2O_2O_SetA[nArray];
	double ResonanceRatio_2O_ThirdO_SetA[nArray];
	double ResonanceRatio_2O_ForthO_SetA[nArray];
	double ResonanceRatio_2O_FifthO_SetA[nArray];
	double ResonanceRatio_2O_SixthO_SetA[nArray];
	double ResonanceRatio_2O_SeventhO_SetA[nArray];
	double ResonanceRatio_2O_EighthO_SetA[nArray];
	double ResonanceRatio_2O_NinthO_SetA[nArray];
	
	double ResonanceRatio_2O_FU_SetB[nArray];
	double ResonanceRatio_2O_FO_SetB[nArray];
	double ResonanceRatio_2O_2O_SetB[nArray];
	double ResonanceRatio_2O_ThirdO_SetB[nArray];
	double ResonanceRatio_2O_ForthO_SetB[nArray];
	double ResonanceRatio_2O_FifthO_SetB[nArray];
	double ResonanceRatio_2O_SixthO_SetB[nArray];
	double ResonanceRatio_2O_SeventhO_SetB[nArray];
	double ResonanceRatio_2O_EighthO_SetB[nArray];
	double ResonanceRatio_2O_NinthO_SetB[nArray];
	
	double ResonanceRatio_2O_FU_SetC[nArray];
	double ResonanceRatio_2O_FO_SetC[nArray];
	double ResonanceRatio_2O_2O_SetC[nArray];
	double ResonanceRatio_2O_ThirdO_SetC[nArray];
	double ResonanceRatio_2O_ForthO_SetC[nArray];
	double ResonanceRatio_2O_FifthO_SetC[nArray];
	double ResonanceRatio_2O_SixthO_SetC[nArray];
	double ResonanceRatio_2O_SeventhO_SetC[nArray];
	double ResonanceRatio_2O_EighthO_SetC[nArray];
	double ResonanceRatio_2O_NinthO_SetC[nArray];
	
	double ResonanceRatio_2O_FU_SetD[nArray];
	double ResonanceRatio_2O_FO_SetD[nArray];
	double ResonanceRatio_2O_2O_SetD[nArray];
	double ResonanceRatio_2O_ThirdO_SetD[nArray];
	double ResonanceRatio_2O_ForthO_SetD[nArray];
	double ResonanceRatio_2O_FifthO_SetD[nArray];
	double ResonanceRatio_2O_SixthO_SetD[nArray];
	double ResonanceRatio_2O_SeventhO_SetD[nArray];
	double ResonanceRatio_2O_EighthO_SetD[nArray];
	double ResonanceRatio_2O_NinthO_SetD[nArray];
	
	double OverallResonance_2O_FU_SetA = 0;
	double AverageResonance_2O_FU_SetA;
	double OverallResonance_2O_FU_SetB = 0;
	double AverageResonance_2O_FU_SetB;
	double OverallResonance_2O_FU_SetC = 0;
	double AverageResonance_2O_FU_SetC;
	double OverallResonance_2O_FU_SetD = 0;
	double AverageResonance_2O_FU_SetD;
	
	double OverallResonance_2O_FO_SetA = 0;
	double AverageResonance_2O_FO_SetA;
	double OverallResonance_2O_FO_SetB = 0;
	double AverageResonance_2O_FO_SetB;
	double OverallResonance_2O_FO_SetC = 0;
	double AverageResonance_2O_FO_SetC;
	double OverallResonance_2O_FO_SetD = 0;
	double AverageResonance_2O_FO_SetD;
	
	double OverallResonance_2O_2O_SetA = 0;
	double AverageResonance_2O_2O_SetA;
	double OverallResonance_2O_2O_SetB = 0;
	double AverageResonance_2O_2O_SetB;
	double OverallResonance_2O_2O_SetC = 0;
	double AverageResonance_2O_2O_SetC;
	double OverallResonance_2O_2O_SetD = 0;
	double AverageResonance_2O_2O_SetD;
	
	double OverallResonance_2O_ThirdO_SetA = 0;
	double AverageResonance_2O_ThirdO_SetA;
	double OverallResonance_2O_ThirdO_SetB = 0;
	double AverageResonance_2O_ThirdO_SetB;
	double OverallResonance_2O_ThirdO_SetC = 0;
	double AverageResonance_2O_ThirdO_SetC;
	double OverallResonance_2O_ThirdO_SetD = 0;
	double AverageResonance_2O_ThirdO_SetD;
	
	double OverallResonance_2O_ForthO_SetA = 0;
	double AverageResonance_2O_ForthO_SetA;
	double OverallResonance_2O_ForthO_SetB = 0;
	double AverageResonance_2O_ForthO_SetB;
	double OverallResonance_2O_ForthO_SetC = 0;
	double AverageResonance_2O_ForthO_SetC;
	double OverallResonance_2O_ForthO_SetD = 0;
	double AverageResonance_2O_ForthO_SetD;
	
	double OverallResonance_2O_FifthO_SetA = 0;
	double AverageResonance_2O_FifthO_SetA;
	double OverallResonance_2O_FifthO_SetB = 0;
	double AverageResonance_2O_FifthO_SetB;
	double OverallResonance_2O_FifthO_SetC = 0;
	double AverageResonance_2O_FifthO_SetC;
	double OverallResonance_2O_FifthO_SetD = 0;
	double AverageResonance_2O_FifthO_SetD;
	
	double OverallResonance_2O_SixthO_SetA = 0;
	double AverageResonance_2O_SixthO_SetA;
	double OverallResonance_2O_SixthO_SetB = 0;
	double AverageResonance_2O_SixthO_SetB;
	double OverallResonance_2O_SixthO_SetC = 0;
	double AverageResonance_2O_SixthO_SetC;
	double OverallResonance_2O_SixthO_SetD = 0;
	double AverageResonance_2O_SixthO_SetD;
	
	double OverallResonance_2O_SeventhO_SetA = 0;
	double AverageResonance_2O_SeventhO_SetA;
	double OverallResonance_2O_SeventhO_SetB = 0;
	double AverageResonance_2O_SeventhO_SetB;
	double OverallResonance_2O_SeventhO_SetC = 0;
	double AverageResonance_2O_SeventhO_SetC;
	double OverallResonance_2O_SeventhO_SetD = 0;
	double AverageResonance_2O_SeventhO_SetD;
	
	double OverallResonance_2O_EighthO_SetA = 0;
	double AverageResonance_2O_EighthO_SetA;
	double OverallResonance_2O_EighthO_SetB = 0;
	double AverageResonance_2O_EighthO_SetB;
	double OverallResonance_2O_EighthO_SetC = 0;
	double AverageResonance_2O_EighthO_SetC;
	double OverallResonance_2O_EighthO_SetD = 0;
	double AverageResonance_2O_EighthO_SetD;
	
	double OverallResonance_2O_NinthO_SetA = 0;
	double AverageResonance_2O_NinthO_SetA;
	double OverallResonance_2O_NinthO_SetB = 0;
	double AverageResonance_2O_NinthO_SetB;
	double OverallResonance_2O_NinthO_SetC = 0;
	double AverageResonance_2O_NinthO_SetC;
	double OverallResonance_2O_NinthO_SetD = 0;
	double AverageResonance_2O_NinthO_SetD;
	
	int NumOf2O_FU_SetA = 0;
	int NumOf2O_FU_SetB = 0;
	int NumOf2O_FU_SetC = 0;
	int NumOf2O_FU_SetD = 0;
	
	int NumOf2O_FO_SetA = 0;
	int NumOf2O_FO_SetB = 0;
	int NumOf2O_FO_SetC = 0;
	int NumOf2O_FO_SetD = 0;
	
	int NumOf2O_2O_SetA = 0;
	int NumOf2O_2O_SetB = 0;
	int NumOf2O_2O_SetC = 0;
	int NumOf2O_2O_SetD = 0;
	
	int NumOf2O_ThirdO_SetA = 0;
	int NumOf2O_ThirdO_SetB = 0;
	int NumOf2O_ThirdO_SetC = 0;
	int NumOf2O_ThirdO_SetD = 0;
	
	int NumOf2O_ForthO_SetA = 0;
	int NumOf2O_ForthO_SetB = 0;
	int NumOf2O_ForthO_SetC = 0;
	int NumOf2O_ForthO_SetD = 0;
	
	int NumOf2O_FifthO_SetA = 0;
	int NumOf2O_FifthO_SetB = 0;
	int NumOf2O_FifthO_SetC = 0;
	int NumOf2O_FifthO_SetD = 0;
	
	int NumOf2O_SixthO_SetA = 0;
	int NumOf2O_SixthO_SetB = 0;
	int NumOf2O_SixthO_SetC = 0;
	int NumOf2O_SixthO_SetD = 0;
	
	int NumOf2O_SeventhO_SetA = 0;
	int NumOf2O_SeventhO_SetB = 0;
	int NumOf2O_SeventhO_SetC = 0;
	int NumOf2O_SeventhO_SetD = 0;
	
	int NumOf2O_EighthO_SetA = 0;
	int NumOf2O_EighthO_SetB = 0;
	int NumOf2O_EighthO_SetC = 0;
	int NumOf2O_EighthO_SetD = 0;
	
	int NumOf2O_NinthO_SetA = 0;
	int NumOf2O_NinthO_SetB = 0;
	int NumOf2O_NinthO_SetC = 0;
	int NumOf2O_NinthO_SetD = 0;
	
	////////////////////////////////////////////
	
	double ResonanceRatio_ThirdO_FU_SetA[nArray];
	double ResonanceRatio_ThirdO_FO_SetA[nArray];
	double ResonanceRatio_ThirdO_2O_SetA[nArray];
	double ResonanceRatio_ThirdO_ThirdO_SetA[nArray];
	double ResonanceRatio_ThirdO_ForthO_SetA[nArray];
	double ResonanceRatio_ThirdO_FifthO_SetA[nArray];
	double ResonanceRatio_ThirdO_SixthO_SetA[nArray];
	double ResonanceRatio_ThirdO_SeventhO_SetA[nArray];
	double ResonanceRatio_ThirdO_EighthO_SetA[nArray];
	double ResonanceRatio_ThirdO_NinthO_SetA[nArray];
	
	double ResonanceRatio_ThirdO_FU_SetB[nArray];
	double ResonanceRatio_ThirdO_FO_SetB[nArray];
	double ResonanceRatio_ThirdO_2O_SetB[nArray];
	double ResonanceRatio_ThirdO_ThirdO_SetB[nArray];
	double ResonanceRatio_ThirdO_ForthO_SetB[nArray];
	double ResonanceRatio_ThirdO_FifthO_SetB[nArray];
	double ResonanceRatio_ThirdO_SixthO_SetB[nArray];
	double ResonanceRatio_ThirdO_SeventhO_SetB[nArray];
	double ResonanceRatio_ThirdO_EighthO_SetB[nArray];
	double ResonanceRatio_ThirdO_NinthO_SetB[nArray];
	
	double ResonanceRatio_ThirdO_FU_SetC[nArray];
	double ResonanceRatio_ThirdO_FO_SetC[nArray];
	double ResonanceRatio_ThirdO_2O_SetC[nArray];
	double ResonanceRatio_ThirdO_ThirdO_SetC[nArray];
	double ResonanceRatio_ThirdO_ForthO_SetC[nArray];
	double ResonanceRatio_ThirdO_FifthO_SetC[nArray];
	double ResonanceRatio_ThirdO_SixthO_SetC[nArray];
	double ResonanceRatio_ThirdO_SeventhO_SetC[nArray];
	double ResonanceRatio_ThirdO_EighthO_SetC[nArray];
	double ResonanceRatio_ThirdO_NinthO_SetC[nArray];
	
	double ResonanceRatio_ThirdO_FU_SetD[nArray];
	double ResonanceRatio_ThirdO_FO_SetD[nArray];
	double ResonanceRatio_ThirdO_2O_SetD[nArray];
	double ResonanceRatio_ThirdO_ThirdO_SetD[nArray];
	double ResonanceRatio_ThirdO_ForthO_SetD[nArray];
	double ResonanceRatio_ThirdO_FifthO_SetD[nArray];
	double ResonanceRatio_ThirdO_SixthO_SetD[nArray];
	double ResonanceRatio_ThirdO_SeventhO_SetD[nArray];
	double ResonanceRatio_ThirdO_EighthO_SetD[nArray];
	double ResonanceRatio_ThirdO_NinthO_SetD[nArray];
	
	double OverallResonance_ThirdO_FU_SetA = 0;
	double AverageResonance_ThirdO_FU_SetA;
	double OverallResonance_ThirdO_FU_SetB = 0;
	double AverageResonance_ThirdO_FU_SetB;
	double OverallResonance_ThirdO_FU_SetC = 0;
	double AverageResonance_ThirdO_FU_SetC;
	double OverallResonance_ThirdO_FU_SetD = 0;
	double AverageResonance_ThirdO_FU_SetD;
	
	double OverallResonance_ThirdO_FO_SetA = 0;
	double AverageResonance_ThirdO_FO_SetA;
	double OverallResonance_ThirdO_FO_SetB = 0;
	double AverageResonance_ThirdO_FO_SetB;
	double OverallResonance_ThirdO_FO_SetC = 0;
	double AverageResonance_ThirdO_FO_SetC;
	double OverallResonance_ThirdO_FO_SetD = 0;
	double AverageResonance_ThirdO_FO_SetD;
	
	double OverallResonance_ThirdO_2O_SetA = 0;
	double AverageResonance_ThirdO_2O_SetA;
	double OverallResonance_ThirdO_2O_SetB = 0;
	double AverageResonance_ThirdO_2O_SetB;
	double OverallResonance_ThirdO_2O_SetC = 0;
	double AverageResonance_ThirdO_2O_SetC;
	double OverallResonance_ThirdO_2O_SetD = 0;
	double AverageResonance_ThirdO_2O_SetD;
	
	double OverallResonance_ThirdO_ThirdO_SetA = 0;
	double AverageResonance_ThirdO_ThirdO_SetA;
	double OverallResonance_ThirdO_ThirdO_SetB = 0;
	double AverageResonance_ThirdO_ThirdO_SetB;
	double OverallResonance_ThirdO_ThirdO_SetC = 0;
	double AverageResonance_ThirdO_ThirdO_SetC;
	double OverallResonance_ThirdO_ThirdO_SetD = 0;
	double AverageResonance_ThirdO_ThirdO_SetD;
	
	double OverallResonance_ThirdO_ForthO_SetA = 0;
	double AverageResonance_ThirdO_ForthO_SetA;
	double OverallResonance_ThirdO_ForthO_SetB = 0;
	double AverageResonance_ThirdO_ForthO_SetB;
	double OverallResonance_ThirdO_ForthO_SetC = 0;
	double AverageResonance_ThirdO_ForthO_SetC;
	double OverallResonance_ThirdO_ForthO_SetD = 0;
	double AverageResonance_ThirdO_ForthO_SetD;
	
	double OverallResonance_ThirdO_FifthO_SetA = 0;
	double AverageResonance_ThirdO_FifthO_SetA;
	double OverallResonance_ThirdO_FifthO_SetB = 0;
	double AverageResonance_ThirdO_FifthO_SetB;
	double OverallResonance_ThirdO_FifthO_SetC = 0;
	double AverageResonance_ThirdO_FifthO_SetC;
	double OverallResonance_ThirdO_FifthO_SetD = 0;
	double AverageResonance_ThirdO_FifthO_SetD;
	
	double OverallResonance_ThirdO_SixthO_SetA = 0;
	double AverageResonance_ThirdO_SixthO_SetA;
	double OverallResonance_ThirdO_SixthO_SetB = 0;
	double AverageResonance_ThirdO_SixthO_SetB;
	double OverallResonance_ThirdO_SixthO_SetC = 0;
	double AverageResonance_ThirdO_SixthO_SetC;
	double OverallResonance_ThirdO_SixthO_SetD = 0;
	double AverageResonance_ThirdO_SixthO_SetD;
	
	double OverallResonance_ThirdO_SeventhO_SetA = 0;
	double AverageResonance_ThirdO_SeventhO_SetA;
	double OverallResonance_ThirdO_SeventhO_SetB = 0;
	double AverageResonance_ThirdO_SeventhO_SetB;
	double OverallResonance_ThirdO_SeventhO_SetC = 0;
	double AverageResonance_ThirdO_SeventhO_SetC;
	double OverallResonance_ThirdO_SeventhO_SetD = 0;
	double AverageResonance_ThirdO_SeventhO_SetD;
	
	double OverallResonance_ThirdO_EighthO_SetA = 0;
	double AverageResonance_ThirdO_EighthO_SetA;
	double OverallResonance_ThirdO_EighthO_SetB = 0;
	double AverageResonance_ThirdO_EighthO_SetB;
	double OverallResonance_ThirdO_EighthO_SetC = 0;
	double AverageResonance_ThirdO_EighthO_SetC;
	double OverallResonance_ThirdO_EighthO_SetD = 0;
	double AverageResonance_ThirdO_EighthO_SetD;
	
	double OverallResonance_ThirdO_NinthO_SetA = 0;
	double AverageResonance_ThirdO_NinthO_SetA;
	double OverallResonance_ThirdO_NinthO_SetB = 0;
	double AverageResonance_ThirdO_NinthO_SetB;
	double OverallResonance_ThirdO_NinthO_SetC = 0;
	double AverageResonance_ThirdO_NinthO_SetC;
	double OverallResonance_ThirdO_NinthO_SetD = 0;
	double AverageResonance_ThirdO_NinthO_SetD;
	
	int NumOfThirdO_FU_SetA = 0;
	int NumOfThirdO_FU_SetB = 0;
	int NumOfThirdO_FU_SetC = 0;
	int NumOfThirdO_FU_SetD = 0;
	
	int NumOfThirdO_FO_SetA = 0;
	int NumOfThirdO_FO_SetB = 0;
	int NumOfThirdO_FO_SetC = 0;
	int NumOfThirdO_FO_SetD = 0;
	
	int NumOfThirdO_2O_SetA = 0;
	int NumOfThirdO_2O_SetB = 0;
	int NumOfThirdO_2O_SetC = 0;
	int NumOfThirdO_2O_SetD = 0;
	
	int NumOfThirdO_ThirdO_SetA = 0;
	int NumOfThirdO_ThirdO_SetB = 0;
	int NumOfThirdO_ThirdO_SetC = 0;
	int NumOfThirdO_ThirdO_SetD = 0;
	
	int NumOfThirdO_ForthO_SetA = 0;
	int NumOfThirdO_ForthO_SetB = 0;
	int NumOfThirdO_ForthO_SetC = 0;
	int NumOfThirdO_ForthO_SetD = 0;
	
	int NumOfThirdO_FifthO_SetA = 0;
	int NumOfThirdO_FifthO_SetB = 0;
	int NumOfThirdO_FifthO_SetC = 0;
	int NumOfThirdO_FifthO_SetD = 0;
	
	int NumOfThirdO_SixthO_SetA = 0;
	int NumOfThirdO_SixthO_SetB = 0;
	int NumOfThirdO_SixthO_SetC = 0;
	int NumOfThirdO_SixthO_SetD = 0;
	
	int NumOfThirdO_SeventhO_SetA = 0;
	int NumOfThirdO_SeventhO_SetB = 0;
	int NumOfThirdO_SeventhO_SetC = 0;
	int NumOfThirdO_SeventhO_SetD = 0;
	
	int NumOfThirdO_EighthO_SetA = 0;
	int NumOfThirdO_EighthO_SetB = 0;
	int NumOfThirdO_EighthO_SetC = 0;
	int NumOfThirdO_EighthO_SetD = 0;
	
	int NumOfThirdO_NinthO_SetA = 0;
	int NumOfThirdO_NinthO_SetB = 0;
	int NumOfThirdO_NinthO_SetC = 0;
	int NumOfThirdO_NinthO_SetD = 0;
	
	///////////////////////////////////////////////////////
	
	double ResonanceRatio_ForthO_FU_SetA[nArray];
	double ResonanceRatio_ForthO_FO_SetA[nArray];
	double ResonanceRatio_ForthO_2O_SetA[nArray];
	double ResonanceRatio_ForthO_ThirdO_SetA[nArray];
	double ResonanceRatio_ForthO_ForthO_SetA[nArray];
	double ResonanceRatio_ForthO_FifthO_SetA[nArray];
	double ResonanceRatio_ForthO_SixthO_SetA[nArray];
	double ResonanceRatio_ForthO_SeventhO_SetA[nArray];
	double ResonanceRatio_ForthO_EighthO_SetA[nArray];
	double ResonanceRatio_ForthO_NinthO_SetA[nArray];
	
	double ResonanceRatio_ForthO_FU_SetB[nArray];
	double ResonanceRatio_ForthO_FO_SetB[nArray];
	double ResonanceRatio_ForthO_2O_SetB[nArray];
	double ResonanceRatio_ForthO_ThirdO_SetB[nArray];
	double ResonanceRatio_ForthO_ForthO_SetB[nArray];
	double ResonanceRatio_ForthO_FifthO_SetB[nArray];
	double ResonanceRatio_ForthO_SixthO_SetB[nArray];
	double ResonanceRatio_ForthO_SeventhO_SetB[nArray];
	double ResonanceRatio_ForthO_EighthO_SetB[nArray];
	double ResonanceRatio_ForthO_NinthO_SetB[nArray];
	
	double ResonanceRatio_ForthO_FU_SetC[nArray];
	double ResonanceRatio_ForthO_FO_SetC[nArray];
	double ResonanceRatio_ForthO_2O_SetC[nArray];
	double ResonanceRatio_ForthO_ThirdO_SetC[nArray];
	double ResonanceRatio_ForthO_ForthO_SetC[nArray];
	double ResonanceRatio_ForthO_FifthO_SetC[nArray];
	double ResonanceRatio_ForthO_SixthO_SetC[nArray];
	double ResonanceRatio_ForthO_SeventhO_SetC[nArray];
	double ResonanceRatio_ForthO_EighthO_SetC[nArray];
	double ResonanceRatio_ForthO_NinthO_SetC[nArray];
	
	double ResonanceRatio_ForthO_FU_SetD[nArray];
	double ResonanceRatio_ForthO_FO_SetD[nArray];
	double ResonanceRatio_ForthO_2O_SetD[nArray];
	double ResonanceRatio_ForthO_ThirdO_SetD[nArray];
	double ResonanceRatio_ForthO_ForthO_SetD[nArray];
	double ResonanceRatio_ForthO_FifthO_SetD[nArray];
	double ResonanceRatio_ForthO_SixthO_SetD[nArray];
	double ResonanceRatio_ForthO_SeventhO_SetD[nArray];
	double ResonanceRatio_ForthO_EighthO_SetD[nArray];
	double ResonanceRatio_ForthO_NinthO_SetD[nArray];
	
	double OverallResonance_ForthO_FU_SetA = 0;
	double AverageResonance_ForthO_FU_SetA;
	double OverallResonance_ForthO_FU_SetB = 0;
	double AverageResonance_ForthO_FU_SetB;
	double OverallResonance_ForthO_FU_SetC = 0;
	double AverageResonance_ForthO_FU_SetC;
	double OverallResonance_ForthO_FU_SetD = 0;
	double AverageResonance_ForthO_FU_SetD;
	
	double OverallResonance_ForthO_FO_SetA = 0;
	double AverageResonance_ForthO_FO_SetA;
	double OverallResonance_ForthO_FO_SetB = 0;
	double AverageResonance_ForthO_FO_SetB;
	double OverallResonance_ForthO_FO_SetC = 0;
	double AverageResonance_ForthO_FO_SetC;
	double OverallResonance_ForthO_FO_SetD = 0;
	double AverageResonance_ForthO_FO_SetD;
	
	double OverallResonance_ForthO_2O_SetA = 0;
	double AverageResonance_ForthO_2O_SetA;
	double OverallResonance_ForthO_2O_SetB = 0;
	double AverageResonance_ForthO_2O_SetB;
	double OverallResonance_ForthO_2O_SetC = 0;
	double AverageResonance_ForthO_2O_SetC;
	double OverallResonance_ForthO_2O_SetD = 0;
	double AverageResonance_ForthO_2O_SetD;
	
	double OverallResonance_ForthO_ThirdO_SetA = 0;
	double AverageResonance_ForthO_ThirdO_SetA;
	double OverallResonance_ForthO_ThirdO_SetB = 0;
	double AverageResonance_ForthO_ThirdO_SetB;
	double OverallResonance_ForthO_ThirdO_SetC = 0;
	double AverageResonance_ForthO_ThirdO_SetC;
	double OverallResonance_ForthO_ThirdO_SetD = 0;
	double AverageResonance_ForthO_ThirdO_SetD;
	
	double OverallResonance_ForthO_ForthO_SetA = 0;
	double AverageResonance_ForthO_ForthO_SetA;
	double OverallResonance_ForthO_ForthO_SetB = 0;
	double AverageResonance_ForthO_ForthO_SetB;
	double OverallResonance_ForthO_ForthO_SetC = 0;
	double AverageResonance_ForthO_ForthO_SetC;
	double OverallResonance_ForthO_ForthO_SetD = 0;
	double AverageResonance_ForthO_ForthO_SetD;
	
	double OverallResonance_ForthO_FifthO_SetA = 0;
	double AverageResonance_ForthO_FifthO_SetA;
	double OverallResonance_ForthO_FifthO_SetB = 0;
	double AverageResonance_ForthO_FifthO_SetB;
	double OverallResonance_ForthO_FifthO_SetC = 0;
	double AverageResonance_ForthO_FifthO_SetC;
	double OverallResonance_ForthO_FifthO_SetD = 0;
	double AverageResonance_ForthO_FifthO_SetD;
	
	double OverallResonance_ForthO_SixthO_SetA = 0;
	double AverageResonance_ForthO_SixthO_SetA;
	double OverallResonance_ForthO_SixthO_SetB = 0;
	double AverageResonance_ForthO_SixthO_SetB;
	double OverallResonance_ForthO_SixthO_SetC = 0;
	double AverageResonance_ForthO_SixthO_SetC;
	double OverallResonance_ForthO_SixthO_SetD = 0;
	double AverageResonance_ForthO_SixthO_SetD;
	
	double OverallResonance_ForthO_SeventhO_SetA = 0;
	double AverageResonance_ForthO_SeventhO_SetA;
	double OverallResonance_ForthO_SeventhO_SetB = 0;
	double AverageResonance_ForthO_SeventhO_SetB;
	double OverallResonance_ForthO_SeventhO_SetC = 0;
	double AverageResonance_ForthO_SeventhO_SetC;
	double OverallResonance_ForthO_SeventhO_SetD = 0;
	double AverageResonance_ForthO_SeventhO_SetD;
	
	double OverallResonance_ForthO_EighthO_SetA = 0;
	double AverageResonance_ForthO_EighthO_SetA;
	double OverallResonance_ForthO_EighthO_SetB = 0;
	double AverageResonance_ForthO_EighthO_SetB;
	double OverallResonance_ForthO_EighthO_SetC = 0;
	double AverageResonance_ForthO_EighthO_SetC;
	double OverallResonance_ForthO_EighthO_SetD = 0;
	double AverageResonance_ForthO_EighthO_SetD;
	
	double OverallResonance_ForthO_NinthO_SetA = 0;
	double AverageResonance_ForthO_NinthO_SetA;
	double OverallResonance_ForthO_NinthO_SetB = 0;
	double AverageResonance_ForthO_NinthO_SetB;
	double OverallResonance_ForthO_NinthO_SetC = 0;
	double AverageResonance_ForthO_NinthO_SetC;
	double OverallResonance_ForthO_NinthO_SetD = 0;
	double AverageResonance_ForthO_NinthO_SetD;
	
	int NumOfForthO_FU_SetA = 0;
	int NumOfForthO_FU_SetB = 0;
	int NumOfForthO_FU_SetC = 0;
	int NumOfForthO_FU_SetD = 0;
	
	int NumOfForthO_FO_SetA = 0;
	int NumOfForthO_FO_SetB = 0;
	int NumOfForthO_FO_SetC = 0;
	int NumOfForthO_FO_SetD = 0;
	
	int NumOfForthO_2O_SetA = 0;
	int NumOfForthO_2O_SetB = 0;
	int NumOfForthO_2O_SetC = 0;
	int NumOfForthO_2O_SetD = 0;
	
	int NumOfForthO_ThirdO_SetA = 0;
	int NumOfForthO_ThirdO_SetB = 0;
	int NumOfForthO_ThirdO_SetC = 0;
	int NumOfForthO_ThirdO_SetD = 0;
	
	int NumOfForthO_ForthO_SetA = 0;
	int NumOfForthO_ForthO_SetB = 0;
	int NumOfForthO_ForthO_SetC = 0;
	int NumOfForthO_ForthO_SetD = 0;
	
	int NumOfForthO_FifthO_SetA = 0;
	int NumOfForthO_FifthO_SetB = 0;
	int NumOfForthO_FifthO_SetC = 0;
	int NumOfForthO_FifthO_SetD = 0;
	
	int NumOfForthO_SixthO_SetA = 0;
	int NumOfForthO_SixthO_SetB = 0;
	int NumOfForthO_SixthO_SetC = 0;
	int NumOfForthO_SixthO_SetD = 0;
	
	int NumOfForthO_SeventhO_SetA = 0;
	int NumOfForthO_SeventhO_SetB = 0;
	int NumOfForthO_SeventhO_SetC = 0;
	int NumOfForthO_SeventhO_SetD = 0;
	
	int NumOfForthO_EighthO_SetA = 0;
	int NumOfForthO_EighthO_SetB = 0;
	int NumOfForthO_EighthO_SetC = 0;
	int NumOfForthO_EighthO_SetD = 0;
	
	int NumOfForthO_NinthO_SetA = 0;
	int NumOfForthO_NinthO_SetB = 0;
	int NumOfForthO_NinthO_SetC = 0;
	int NumOfForthO_NinthO_SetD = 0;
	
	//////////////////////////////////////////////////
	
	double ResonanceRatio_FifthO_FU_SetA[nArray];
	double ResonanceRatio_FifthO_FO_SetA[nArray];
	double ResonanceRatio_FifthO_2O_SetA[nArray];
	double ResonanceRatio_FifthO_ThirdO_SetA[nArray];
	double ResonanceRatio_FifthO_ForthO_SetA[nArray];
	double ResonanceRatio_FifthO_FifthO_SetA[nArray];
	double ResonanceRatio_FifthO_SixthO_SetA[nArray];
	double ResonanceRatio_FifthO_SeventhO_SetA[nArray];
	double ResonanceRatio_FifthO_EighthO_SetA[nArray];
	double ResonanceRatio_FifthO_NinthO_SetA[nArray];
	
	double ResonanceRatio_FifthO_FU_SetB[nArray];
	double ResonanceRatio_FifthO_FO_SetB[nArray];
	double ResonanceRatio_FifthO_2O_SetB[nArray];
	double ResonanceRatio_FifthO_ThirdO_SetB[nArray];
	double ResonanceRatio_FifthO_ForthO_SetB[nArray];
	double ResonanceRatio_FifthO_FifthO_SetB[nArray];
	double ResonanceRatio_FifthO_SixthO_SetB[nArray];
	double ResonanceRatio_FifthO_SeventhO_SetB[nArray];
	double ResonanceRatio_FifthO_EighthO_SetB[nArray];
	double ResonanceRatio_FifthO_NinthO_SetB[nArray];
	
	double ResonanceRatio_FifthO_FU_SetC[nArray];
	double ResonanceRatio_FifthO_FO_SetC[nArray];
	double ResonanceRatio_FifthO_2O_SetC[nArray];
	double ResonanceRatio_FifthO_ThirdO_SetC[nArray];
	double ResonanceRatio_FifthO_ForthO_SetC[nArray];
	double ResonanceRatio_FifthO_FifthO_SetC[nArray];
	double ResonanceRatio_FifthO_SixthO_SetC[nArray];
	double ResonanceRatio_FifthO_SeventhO_SetC[nArray];
	double ResonanceRatio_FifthO_EighthO_SetC[nArray];
	double ResonanceRatio_FifthO_NinthO_SetC[nArray];
	
	double ResonanceRatio_FifthO_FU_SetD[nArray];
	double ResonanceRatio_FifthO_FO_SetD[nArray];
	double ResonanceRatio_FifthO_2O_SetD[nArray];
	double ResonanceRatio_FifthO_ThirdO_SetD[nArray];
	double ResonanceRatio_FifthO_ForthO_SetD[nArray];
	double ResonanceRatio_FifthO_FifthO_SetD[nArray];
	double ResonanceRatio_FifthO_SixthO_SetD[nArray];
	double ResonanceRatio_FifthO_SeventhO_SetD[nArray];
	double ResonanceRatio_FifthO_EighthO_SetD[nArray];
	double ResonanceRatio_FifthO_NinthO_SetD[nArray];
	
	double OverallResonance_FifthO_FU_SetA = 0;
	double AverageResonance_FifthO_FU_SetA;
	double OverallResonance_FifthO_FU_SetB = 0;
	double AverageResonance_FifthO_FU_SetB;
	double OverallResonance_FifthO_FU_SetC = 0;
	double AverageResonance_FifthO_FU_SetC;
	double OverallResonance_FifthO_FU_SetD = 0;
	double AverageResonance_FifthO_FU_SetD;
	
	double OverallResonance_FifthO_FO_SetA = 0;
	double AverageResonance_FifthO_FO_SetA;
	double OverallResonance_FifthO_FO_SetB = 0;
	double AverageResonance_FifthO_FO_SetB;
	double OverallResonance_FifthO_FO_SetC = 0;
	double AverageResonance_FifthO_FO_SetC;
	double OverallResonance_FifthO_FO_SetD = 0;
	double AverageResonance_FifthO_FO_SetD;
	
	double OverallResonance_FifthO_2O_SetA = 0;
	double AverageResonance_FifthO_2O_SetA;
	double OverallResonance_FifthO_2O_SetB = 0;
	double AverageResonance_FifthO_2O_SetB;
	double OverallResonance_FifthO_2O_SetC = 0;
	double AverageResonance_FifthO_2O_SetC;
	double OverallResonance_FifthO_2O_SetD = 0;
	double AverageResonance_FifthO_2O_SetD;
	
	double OverallResonance_FifthO_ThirdO_SetA = 0;
	double AverageResonance_FifthO_ThirdO_SetA;
	double OverallResonance_FifthO_ThirdO_SetB = 0;
	double AverageResonance_FifthO_ThirdO_SetB;
	double OverallResonance_FifthO_ThirdO_SetC = 0;
	double AverageResonance_FifthO_ThirdO_SetC;
	double OverallResonance_FifthO_ThirdO_SetD = 0;
	double AverageResonance_FifthO_ThirdO_SetD;
	
	double OverallResonance_FifthO_ForthO_SetA = 0;
	double AverageResonance_FifthO_ForthO_SetA;
	double OverallResonance_FifthO_ForthO_SetB = 0;
	double AverageResonance_FifthO_ForthO_SetB;
	double OverallResonance_FifthO_ForthO_SetC = 0;
	double AverageResonance_FifthO_ForthO_SetC;
	double OverallResonance_FifthO_ForthO_SetD = 0;
	double AverageResonance_FifthO_ForthO_SetD;
	
	double OverallResonance_FifthO_FifthO_SetA = 0;
	double AverageResonance_FifthO_FifthO_SetA;
	double OverallResonance_FifthO_FifthO_SetB = 0;
	double AverageResonance_FifthO_FifthO_SetB;
	double OverallResonance_FifthO_FifthO_SetC = 0;
	double AverageResonance_FifthO_FifthO_SetC;
	double OverallResonance_FifthO_FifthO_SetD = 0;
	double AverageResonance_FifthO_FifthO_SetD;
	
	double OverallResonance_FifthO_SixthO_SetA = 0;
	double AverageResonance_FifthO_SixthO_SetA;
	double OverallResonance_FifthO_SixthO_SetB = 0;
	double AverageResonance_FifthO_SixthO_SetB;
	double OverallResonance_FifthO_SixthO_SetC = 0;
	double AverageResonance_FifthO_SixthO_SetC;
	double OverallResonance_FifthO_SixthO_SetD = 0;
	double AverageResonance_FifthO_SixthO_SetD;
	
	double OverallResonance_FifthO_SeventhO_SetA = 0;
	double AverageResonance_FifthO_SeventhO_SetA;
	double OverallResonance_FifthO_SeventhO_SetB = 0;
	double AverageResonance_FifthO_SeventhO_SetB;
	double OverallResonance_FifthO_SeventhO_SetC = 0;
	double AverageResonance_FifthO_SeventhO_SetC;
	double OverallResonance_FifthO_SeventhO_SetD = 0;
	double AverageResonance_FifthO_SeventhO_SetD;
	
	double OverallResonance_FifthO_EighthO_SetA = 0;
	double AverageResonance_FifthO_EighthO_SetA;
	double OverallResonance_FifthO_EighthO_SetB = 0;
	double AverageResonance_FifthO_EighthO_SetB;
	double OverallResonance_FifthO_EighthO_SetC = 0;
	double AverageResonance_FifthO_EighthO_SetC;
	double OverallResonance_FifthO_EighthO_SetD = 0;
	double AverageResonance_FifthO_EighthO_SetD;
	
	double OverallResonance_FifthO_NinthO_SetA = 0;
	double AverageResonance_FifthO_NinthO_SetA;
	double OverallResonance_FifthO_NinthO_SetB = 0;
	double AverageResonance_FifthO_NinthO_SetB;
	double OverallResonance_FifthO_NinthO_SetC = 0;
	double AverageResonance_FifthO_NinthO_SetC;
	double OverallResonance_FifthO_NinthO_SetD = 0;
	double AverageResonance_FifthO_NinthO_SetD;
	
	int NumOfFifthO_FU_SetA = 0;
	int NumOfFifthO_FU_SetB = 0;
	int NumOfFifthO_FU_SetC = 0;
	int NumOfFifthO_FU_SetD = 0;
	
	int NumOfFifthO_FO_SetA = 0;
	int NumOfFifthO_FO_SetB = 0;
	int NumOfFifthO_FO_SetC = 0;
	int NumOfFifthO_FO_SetD = 0;
	
	int NumOfFifthO_2O_SetA = 0;
	int NumOfFifthO_2O_SetB = 0;
	int NumOfFifthO_2O_SetC = 0;
	int NumOfFifthO_2O_SetD = 0;
	
	int NumOfFifthO_ThirdO_SetA = 0;
	int NumOfFifthO_ThirdO_SetB = 0;
	int NumOfFifthO_ThirdO_SetC = 0;
	int NumOfFifthO_ThirdO_SetD = 0;
	
	int NumOfFifthO_ForthO_SetA = 0;
	int NumOfFifthO_ForthO_SetB = 0;
	int NumOfFifthO_ForthO_SetC = 0;
	int NumOfFifthO_ForthO_SetD = 0;
	
	int NumOfFifthO_FifthO_SetA = 0;
	int NumOfFifthO_FifthO_SetB = 0;
	int NumOfFifthO_FifthO_SetC = 0;
	int NumOfFifthO_FifthO_SetD = 0;
	
	int NumOfFifthO_SixthO_SetA = 0;
	int NumOfFifthO_SixthO_SetB = 0;
	int NumOfFifthO_SixthO_SetC = 0;
	int NumOfFifthO_SixthO_SetD = 0;
	
	int NumOfFifthO_SeventhO_SetA = 0;
	int NumOfFifthO_SeventhO_SetB = 0;
	int NumOfFifthO_SeventhO_SetC = 0;
	int NumOfFifthO_SeventhO_SetD = 0;
	
	int NumOfFifthO_EighthO_SetA = 0;
	int NumOfFifthO_EighthO_SetB = 0;
	int NumOfFifthO_EighthO_SetC = 0;
	int NumOfFifthO_EighthO_SetD = 0;
	
	int NumOfFifthO_NinthO_SetA = 0;
	int NumOfFifthO_NinthO_SetB = 0;
	int NumOfFifthO_NinthO_SetC = 0;
	int NumOfFifthO_NinthO_SetD = 0;
	
	///////////////////////////////////////////////////
	
	double ResonanceRatio_SixthO_FU_SetA[nArray];
	double ResonanceRatio_SixthO_FO_SetA[nArray];
	double ResonanceRatio_SixthO_2O_SetA[nArray];
	double ResonanceRatio_SixthO_ThirdO_SetA[nArray];
	double ResonanceRatio_SixthO_ForthO_SetA[nArray];
	double ResonanceRatio_SixthO_FifthO_SetA[nArray];
	double ResonanceRatio_SixthO_SixthO_SetA[nArray];
	double ResonanceRatio_SixthO_SeventhO_SetA[nArray];
	double ResonanceRatio_SixthO_EighthO_SetA[nArray];
	double ResonanceRatio_SixthO_NinthO_SetA[nArray];
	
	double ResonanceRatio_SixthO_FU_SetB[nArray];
	double ResonanceRatio_SixthO_FO_SetB[nArray];
	double ResonanceRatio_SixthO_2O_SetB[nArray];
	double ResonanceRatio_SixthO_ThirdO_SetB[nArray];
	double ResonanceRatio_SixthO_ForthO_SetB[nArray];
	double ResonanceRatio_SixthO_FifthO_SetB[nArray];
	double ResonanceRatio_SixthO_SixthO_SetB[nArray];
	double ResonanceRatio_SixthO_SeventhO_SetB[nArray];
	double ResonanceRatio_SixthO_EighthO_SetB[nArray];
	double ResonanceRatio_SixthO_NinthO_SetB[nArray];
	
	double ResonanceRatio_SixthO_FU_SetC[nArray];
	double ResonanceRatio_SixthO_FO_SetC[nArray];
	double ResonanceRatio_SixthO_2O_SetC[nArray];
	double ResonanceRatio_SixthO_ThirdO_SetC[nArray];
	double ResonanceRatio_SixthO_ForthO_SetC[nArray];
	double ResonanceRatio_SixthO_FifthO_SetC[nArray];
	double ResonanceRatio_SixthO_SixthO_SetC[nArray];
	double ResonanceRatio_SixthO_SeventhO_SetC[nArray];
	double ResonanceRatio_SixthO_EighthO_SetC[nArray];
	double ResonanceRatio_SixthO_NinthO_SetC[nArray];
	
	double ResonanceRatio_SixthO_FU_SetD[nArray];
	double ResonanceRatio_SixthO_FO_SetD[nArray];
	double ResonanceRatio_SixthO_2O_SetD[nArray];
	double ResonanceRatio_SixthO_ThirdO_SetD[nArray];
	double ResonanceRatio_SixthO_ForthO_SetD[nArray];
	double ResonanceRatio_SixthO_FifthO_SetD[nArray];
	double ResonanceRatio_SixthO_SixthO_SetD[nArray];
	double ResonanceRatio_SixthO_SeventhO_SetD[nArray];
	double ResonanceRatio_SixthO_EighthO_SetD[nArray];
	double ResonanceRatio_SixthO_NinthO_SetD[nArray];
	
	double OverallResonance_SixthO_FU_SetA = 0;
	double AverageResonance_SixthO_FU_SetA;
	double OverallResonance_SixthO_FU_SetB = 0;
	double AverageResonance_SixthO_FU_SetB;
	double OverallResonance_SixthO_FU_SetC = 0;
	double AverageResonance_SixthO_FU_SetC;
	double OverallResonance_SixthO_FU_SetD = 0;
	double AverageResonance_SixthO_FU_SetD;
	
	double OverallResonance_SixthO_FO_SetA = 0;
	double AverageResonance_SixthO_FO_SetA;
	double OverallResonance_SixthO_FO_SetB = 0;
	double AverageResonance_SixthO_FO_SetB;
	double OverallResonance_SixthO_FO_SetC = 0;
	double AverageResonance_SixthO_FO_SetC;
	double OverallResonance_SixthO_FO_SetD = 0;
	double AverageResonance_SixthO_FO_SetD;
	
	double OverallResonance_SixthO_2O_SetA = 0;
	double AverageResonance_SixthO_2O_SetA;
	double OverallResonance_SixthO_2O_SetB = 0;
	double AverageResonance_SixthO_2O_SetB;
	double OverallResonance_SixthO_2O_SetC = 0;
	double AverageResonance_SixthO_2O_SetC;
	double OverallResonance_SixthO_2O_SetD = 0;
	double AverageResonance_SixthO_2O_SetD;
	
	double OverallResonance_SixthO_ThirdO_SetA = 0;
	double AverageResonance_SixthO_ThirdO_SetA;
	double OverallResonance_SixthO_ThirdO_SetB = 0;
	double AverageResonance_SixthO_ThirdO_SetB;
	double OverallResonance_SixthO_ThirdO_SetC = 0;
	double AverageResonance_SixthO_ThirdO_SetC;
	double OverallResonance_SixthO_ThirdO_SetD = 0;
	double AverageResonance_SixthO_ThirdO_SetD;
	
	double OverallResonance_SixthO_ForthO_SetA = 0;
	double AverageResonance_SixthO_ForthO_SetA;
	double OverallResonance_SixthO_ForthO_SetB = 0;
	double AverageResonance_SixthO_ForthO_SetB;
	double OverallResonance_SixthO_ForthO_SetC = 0;
	double AverageResonance_SixthO_ForthO_SetC;
	double OverallResonance_SixthO_ForthO_SetD = 0;
	double AverageResonance_SixthO_ForthO_SetD;
	
	double OverallResonance_SixthO_FifthO_SetA = 0;
	double AverageResonance_SixthO_FifthO_SetA;
	double OverallResonance_SixthO_FifthO_SetB = 0;
	double AverageResonance_SixthO_FifthO_SetB;
	double OverallResonance_SixthO_FifthO_SetC = 0;
	double AverageResonance_SixthO_FifthO_SetC;
	double OverallResonance_SixthO_FifthO_SetD = 0;
	double AverageResonance_SixthO_FifthO_SetD;
	
	double OverallResonance_SixthO_SixthO_SetA = 0;
	double AverageResonance_SixthO_SixthO_SetA;
	double OverallResonance_SixthO_SixthO_SetB = 0;
	double AverageResonance_SixthO_SixthO_SetB;
	double OverallResonance_SixthO_SixthO_SetC = 0;
	double AverageResonance_SixthO_SixthO_SetC;
	double OverallResonance_SixthO_SixthO_SetD = 0;
	double AverageResonance_SixthO_SixthO_SetD;
	
	double OverallResonance_SixthO_SeventhO_SetA = 0;
	double AverageResonance_SixthO_SeventhO_SetA;
	double OverallResonance_SixthO_SeventhO_SetB = 0;
	double AverageResonance_SixthO_SeventhO_SetB;
	double OverallResonance_SixthO_SeventhO_SetC = 0;
	double AverageResonance_SixthO_SeventhO_SetC;
	double OverallResonance_SixthO_SeventhO_SetD = 0;
	double AverageResonance_SixthO_SeventhO_SetD;
	
	double OverallResonance_SixthO_EighthO_SetA = 0;
	double AverageResonance_SixthO_EighthO_SetA;
	double OverallResonance_SixthO_EighthO_SetB = 0;
	double AverageResonance_SixthO_EighthO_SetB;
	double OverallResonance_SixthO_EighthO_SetC = 0;
	double AverageResonance_SixthO_EighthO_SetC;
	double OverallResonance_SixthO_EighthO_SetD = 0;
	double AverageResonance_SixthO_EighthO_SetD;
	
	double OverallResonance_SixthO_NinthO_SetA = 0;
	double AverageResonance_SixthO_NinthO_SetA;
	double OverallResonance_SixthO_NinthO_SetB = 0;
	double AverageResonance_SixthO_NinthO_SetB;
	double OverallResonance_SixthO_NinthO_SetC = 0;
	double AverageResonance_SixthO_NinthO_SetC;
	double OverallResonance_SixthO_NinthO_SetD = 0;
	double AverageResonance_SixthO_NinthO_SetD;
	
	int NumOfSixthO_FU_SetA = 0;
	int NumOfSixthO_FU_SetB = 0;
	int NumOfSixthO_FU_SetC = 0;
	int NumOfSixthO_FU_SetD = 0;
	
	int NumOfSixthO_FO_SetA = 0;
	int NumOfSixthO_FO_SetB = 0;
	int NumOfSixthO_FO_SetC = 0;
	int NumOfSixthO_FO_SetD = 0;
	
	int NumOfSixthO_2O_SetA = 0;
	int NumOfSixthO_2O_SetB = 0;
	int NumOfSixthO_2O_SetC = 0;
	int NumOfSixthO_2O_SetD = 0;
	
	int NumOfSixthO_ThirdO_SetA = 0;
	int NumOfSixthO_ThirdO_SetB = 0;
	int NumOfSixthO_ThirdO_SetC = 0;
	int NumOfSixthO_ThirdO_SetD = 0;
	
	int NumOfSixthO_ForthO_SetA = 0;
	int NumOfSixthO_ForthO_SetB = 0;
	int NumOfSixthO_ForthO_SetC = 0;
	int NumOfSixthO_ForthO_SetD = 0;
	
	int NumOfSixthO_FifthO_SetA = 0;
	int NumOfSixthO_FifthO_SetB = 0;
	int NumOfSixthO_FifthO_SetC = 0;
	int NumOfSixthO_FifthO_SetD = 0;
	
	int NumOfSixthO_SixthO_SetA = 0;
	int NumOfSixthO_SixthO_SetB = 0;
	int NumOfSixthO_SixthO_SetC = 0;
	int NumOfSixthO_SixthO_SetD = 0;
	
	int NumOfSixthO_SeventhO_SetA = 0;
	int NumOfSixthO_SeventhO_SetB = 0;
	int NumOfSixthO_SeventhO_SetC = 0;
	int NumOfSixthO_SeventhO_SetD = 0;
	
	int NumOfSixthO_EighthO_SetA = 0;
	int NumOfSixthO_EighthO_SetB = 0;
	int NumOfSixthO_EighthO_SetC = 0;
	int NumOfSixthO_EighthO_SetD = 0;
	
	int NumOfSixthO_NinthO_SetA = 0;
	int NumOfSixthO_NinthO_SetB = 0;
	int NumOfSixthO_NinthO_SetC = 0;
	int NumOfSixthO_NinthO_SetD = 0;
	
	//////////////////////////////////////////////////
	
	double ResonanceRatio_SeventhO_FU_SetA[nArray];
	double ResonanceRatio_SeventhO_FO_SetA[nArray];
	double ResonanceRatio_SeventhO_2O_SetA[nArray];
	double ResonanceRatio_SeventhO_ThirdO_SetA[nArray];
	double ResonanceRatio_SeventhO_ForthO_SetA[nArray];
	double ResonanceRatio_SeventhO_FifthO_SetA[nArray];
	double ResonanceRatio_SeventhO_SixthO_SetA[nArray];
	double ResonanceRatio_SeventhO_SeventhO_SetA[nArray];
	double ResonanceRatio_SeventhO_EighthO_SetA[nArray];
	double ResonanceRatio_SeventhO_NinthO_SetA[nArray];
	
	double ResonanceRatio_SeventhO_FU_SetB[nArray];
	double ResonanceRatio_SeventhO_FO_SetB[nArray];
	double ResonanceRatio_SeventhO_2O_SetB[nArray];
	double ResonanceRatio_SeventhO_ThirdO_SetB[nArray];
	double ResonanceRatio_SeventhO_ForthO_SetB[nArray];
	double ResonanceRatio_SeventhO_FifthO_SetB[nArray];
	double ResonanceRatio_SeventhO_SixthO_SetB[nArray];
	double ResonanceRatio_SeventhO_SeventhO_SetB[nArray];
	double ResonanceRatio_SeventhO_EighthO_SetB[nArray];
	double ResonanceRatio_SeventhO_NinthO_SetB[nArray];
	
	double ResonanceRatio_SeventhO_FU_SetC[nArray];
	double ResonanceRatio_SeventhO_FO_SetC[nArray];
	double ResonanceRatio_SeventhO_2O_SetC[nArray];
	double ResonanceRatio_SeventhO_ThirdO_SetC[nArray];
	double ResonanceRatio_SeventhO_ForthO_SetC[nArray];
	double ResonanceRatio_SeventhO_FifthO_SetC[nArray];
	double ResonanceRatio_SeventhO_SixthO_SetC[nArray];
	double ResonanceRatio_SeventhO_SeventhO_SetC[nArray];
	double ResonanceRatio_SeventhO_EighthO_SetC[nArray];
	double ResonanceRatio_SeventhO_NinthO_SetC[nArray];
	
	double ResonanceRatio_SeventhO_FU_SetD[nArray];
	double ResonanceRatio_SeventhO_FO_SetD[nArray];
	double ResonanceRatio_SeventhO_2O_SetD[nArray];
	double ResonanceRatio_SeventhO_ThirdO_SetD[nArray];
	double ResonanceRatio_SeventhO_ForthO_SetD[nArray];
	double ResonanceRatio_SeventhO_FifthO_SetD[nArray];
	double ResonanceRatio_SeventhO_SixthO_SetD[nArray];
	double ResonanceRatio_SeventhO_SeventhO_SetD[nArray];
	double ResonanceRatio_SeventhO_EighthO_SetD[nArray];
	double ResonanceRatio_SeventhO_NinthO_SetD[nArray];
	
	double OverallResonance_SeventhO_FU_SetA = 0;
	double AverageResonance_SeventhO_FU_SetA;
	double OverallResonance_SeventhO_FU_SetB = 0;
	double AverageResonance_SeventhO_FU_SetB;
	double OverallResonance_SeventhO_FU_SetC = 0;
	double AverageResonance_SeventhO_FU_SetC;
	double OverallResonance_SeventhO_FU_SetD = 0;
	double AverageResonance_SeventhO_FU_SetD;
	
	double OverallResonance_SeventhO_FO_SetA = 0;
	double AverageResonance_SeventhO_FO_SetA;
	double OverallResonance_SeventhO_FO_SetB = 0;
	double AverageResonance_SeventhO_FO_SetB;
	double OverallResonance_SeventhO_FO_SetC = 0;
	double AverageResonance_SeventhO_FO_SetC;
	double OverallResonance_SeventhO_FO_SetD = 0;
	double AverageResonance_SeventhO_FO_SetD;
	
	double OverallResonance_SeventhO_2O_SetA = 0;
	double AverageResonance_SeventhO_2O_SetA;
	double OverallResonance_SeventhO_2O_SetB = 0;
	double AverageResonance_SeventhO_2O_SetB;
	double OverallResonance_SeventhO_2O_SetC = 0;
	double AverageResonance_SeventhO_2O_SetC;
	double OverallResonance_SeventhO_2O_SetD = 0;
	double AverageResonance_SeventhO_2O_SetD;
	
	double OverallResonance_SeventhO_ThirdO_SetA = 0;
	double AverageResonance_SeventhO_ThirdO_SetA;
	double OverallResonance_SeventhO_ThirdO_SetB = 0;
	double AverageResonance_SeventhO_ThirdO_SetB;
	double OverallResonance_SeventhO_ThirdO_SetC = 0;
	double AverageResonance_SeventhO_ThirdO_SetC;
	double OverallResonance_SeventhO_ThirdO_SetD = 0;
	double AverageResonance_SeventhO_ThirdO_SetD;
	
	double OverallResonance_SeventhO_ForthO_SetA = 0;
	double AverageResonance_SeventhO_ForthO_SetA;
	double OverallResonance_SeventhO_ForthO_SetB = 0;
	double AverageResonance_SeventhO_ForthO_SetB;
	double OverallResonance_SeventhO_ForthO_SetC = 0;
	double AverageResonance_SeventhO_ForthO_SetC;
	double OverallResonance_SeventhO_ForthO_SetD = 0;
	double AverageResonance_SeventhO_ForthO_SetD;
	
	double OverallResonance_SeventhO_FifthO_SetA = 0;
	double AverageResonance_SeventhO_FifthO_SetA;
	double OverallResonance_SeventhO_FifthO_SetB = 0;
	double AverageResonance_SeventhO_FifthO_SetB;
	double OverallResonance_SeventhO_FifthO_SetC = 0;
	double AverageResonance_SeventhO_FifthO_SetC;
	double OverallResonance_SeventhO_FifthO_SetD = 0;
	double AverageResonance_SeventhO_FifthO_SetD;
	
	double OverallResonance_SeventhO_SixthO_SetA = 0;
	double AverageResonance_SeventhO_SixthO_SetA;
	double OverallResonance_SeventhO_SixthO_SetB = 0;
	double AverageResonance_SeventhO_SixthO_SetB;
	double OverallResonance_SeventhO_SixthO_SetC = 0;
	double AverageResonance_SeventhO_SixthO_SetC;
	double OverallResonance_SeventhO_SixthO_SetD = 0;
	double AverageResonance_SeventhO_SixthO_SetD;
	
	double OverallResonance_SeventhO_SeventhO_SetA = 0;
	double AverageResonance_SeventhO_SeventhO_SetA;
	double OverallResonance_SeventhO_SeventhO_SetB = 0;
	double AverageResonance_SeventhO_SeventhO_SetB;
	double OverallResonance_SeventhO_SeventhO_SetC = 0;
	double AverageResonance_SeventhO_SeventhO_SetC;
	double OverallResonance_SeventhO_SeventhO_SetD = 0;
	double AverageResonance_SeventhO_SeventhO_SetD;
	
	double OverallResonance_SeventhO_EighthO_SetA = 0;
	double AverageResonance_SeventhO_EighthO_SetA;
	double OverallResonance_SeventhO_EighthO_SetB = 0;
	double AverageResonance_SeventhO_EighthO_SetB;
	double OverallResonance_SeventhO_EighthO_SetC = 0;
	double AverageResonance_SeventhO_EighthO_SetC;
	double OverallResonance_SeventhO_EighthO_SetD = 0;
	double AverageResonance_SeventhO_EighthO_SetD;
	
	double OverallResonance_SeventhO_NinthO_SetA = 0;
	double AverageResonance_SeventhO_NinthO_SetA;
	double OverallResonance_SeventhO_NinthO_SetB = 0;
	double AverageResonance_SeventhO_NinthO_SetB;
	double OverallResonance_SeventhO_NinthO_SetC = 0;
	double AverageResonance_SeventhO_NinthO_SetC;
	double OverallResonance_SeventhO_NinthO_SetD = 0;
	double AverageResonance_SeventhO_NinthO_SetD;
	
	int NumOfSeventhO_FU_SetA = 0;
	int NumOfSeventhO_FU_SetB = 0;
	int NumOfSeventhO_FU_SetC = 0;
	int NumOfSeventhO_FU_SetD = 0;
	
	int NumOfSeventhO_FO_SetA = 0;
	int NumOfSeventhO_FO_SetB = 0;
	int NumOfSeventhO_FO_SetC = 0;
	int NumOfSeventhO_FO_SetD = 0;
	
	int NumOfSeventhO_2O_SetA = 0;
	int NumOfSeventhO_2O_SetB = 0;
	int NumOfSeventhO_2O_SetC = 0;
	int NumOfSeventhO_2O_SetD = 0;
	
	int NumOfSeventhO_ThirdO_SetA = 0;
	int NumOfSeventhO_ThirdO_SetB = 0;
	int NumOfSeventhO_ThirdO_SetC = 0;
	int NumOfSeventhO_ThirdO_SetD = 0;
	
	int NumOfSeventhO_ForthO_SetA = 0;
	int NumOfSeventhO_ForthO_SetB = 0;
	int NumOfSeventhO_ForthO_SetC = 0;
	int NumOfSeventhO_ForthO_SetD = 0;
	
	int NumOfSeventhO_FifthO_SetA = 0;
	int NumOfSeventhO_FifthO_SetB = 0;
	int NumOfSeventhO_FifthO_SetC = 0;
	int NumOfSeventhO_FifthO_SetD = 0;
	
	int NumOfSeventhO_SixthO_SetA = 0;
	int NumOfSeventhO_SixthO_SetB = 0;
	int NumOfSeventhO_SixthO_SetC = 0;
	int NumOfSeventhO_SixthO_SetD = 0;
	
	int NumOfSeventhO_SeventhO_SetA = 0;
	int NumOfSeventhO_SeventhO_SetB = 0;
	int NumOfSeventhO_SeventhO_SetC = 0;
	int NumOfSeventhO_SeventhO_SetD = 0;
	
	int NumOfSeventhO_EighthO_SetA = 0;
	int NumOfSeventhO_EighthO_SetB = 0;
	int NumOfSeventhO_EighthO_SetC = 0;
	int NumOfSeventhO_EighthO_SetD = 0;
	
	int NumOfSeventhO_NinthO_SetA = 0;
	int NumOfSeventhO_NinthO_SetB = 0;
	int NumOfSeventhO_NinthO_SetC = 0;
	int NumOfSeventhO_NinthO_SetD = 0;
	
	///////////////////////////////////////////////////////
	
	double ResonanceRatio_EighthO_FU_SetA[nArray];
	double ResonanceRatio_EighthO_FO_SetA[nArray];
	double ResonanceRatio_EighthO_2O_SetA[nArray];
	double ResonanceRatio_EighthO_ThirdO_SetA[nArray];
	double ResonanceRatio_EighthO_ForthO_SetA[nArray];
	double ResonanceRatio_EighthO_FifthO_SetA[nArray];
	double ResonanceRatio_EighthO_SixthO_SetA[nArray];
	double ResonanceRatio_EighthO_SeventhO_SetA[nArray];
	double ResonanceRatio_EighthO_EighthO_SetA[nArray];
	double ResonanceRatio_EighthO_NinthO_SetA[nArray];
	
	double ResonanceRatio_EighthO_FU_SetB[nArray];
	double ResonanceRatio_EighthO_FO_SetB[nArray];
	double ResonanceRatio_EighthO_2O_SetB[nArray];
	double ResonanceRatio_EighthO_ThirdO_SetB[nArray];
	double ResonanceRatio_EighthO_ForthO_SetB[nArray];
	double ResonanceRatio_EighthO_FifthO_SetB[nArray];
	double ResonanceRatio_EighthO_SixthO_SetB[nArray];
	double ResonanceRatio_EighthO_SeventhO_SetB[nArray];
	double ResonanceRatio_EighthO_EighthO_SetB[nArray];
	double ResonanceRatio_EighthO_NinthO_SetB[nArray];
	
	double ResonanceRatio_EighthO_FU_SetC[nArray];
	double ResonanceRatio_EighthO_FO_SetC[nArray];
	double ResonanceRatio_EighthO_2O_SetC[nArray];
	double ResonanceRatio_EighthO_ThirdO_SetC[nArray];
	double ResonanceRatio_EighthO_ForthO_SetC[nArray];
	double ResonanceRatio_EighthO_FifthO_SetC[nArray];
	double ResonanceRatio_EighthO_SixthO_SetC[nArray];
	double ResonanceRatio_EighthO_SeventhO_SetC[nArray];
	double ResonanceRatio_EighthO_EighthO_SetC[nArray];
	double ResonanceRatio_EighthO_NinthO_SetC[nArray];
	
	double ResonanceRatio_EighthO_FU_SetD[nArray];
	double ResonanceRatio_EighthO_FO_SetD[nArray];
	double ResonanceRatio_EighthO_2O_SetD[nArray];
	double ResonanceRatio_EighthO_ThirdO_SetD[nArray];
	double ResonanceRatio_EighthO_ForthO_SetD[nArray];
	double ResonanceRatio_EighthO_FifthO_SetD[nArray];
	double ResonanceRatio_EighthO_SixthO_SetD[nArray];
	double ResonanceRatio_EighthO_SeventhO_SetD[nArray];
	double ResonanceRatio_EighthO_EighthO_SetD[nArray];
	double ResonanceRatio_EighthO_NinthO_SetD[nArray];
	
	double OverallResonance_EighthO_FU_SetA = 0;
	double AverageResonance_EighthO_FU_SetA;
	double OverallResonance_EighthO_FU_SetB = 0;
	double AverageResonance_EighthO_FU_SetB;
	double OverallResonance_EighthO_FU_SetC = 0;
	double AverageResonance_EighthO_FU_SetC;
	double OverallResonance_EighthO_FU_SetD = 0;
	double AverageResonance_EighthO_FU_SetD;
	
	double OverallResonance_EighthO_FO_SetA = 0;
	double AverageResonance_EighthO_FO_SetA;
	double OverallResonance_EighthO_FO_SetB = 0;
	double AverageResonance_EighthO_FO_SetB;
	double OverallResonance_EighthO_FO_SetC = 0;
	double AverageResonance_EighthO_FO_SetC;
	double OverallResonance_EighthO_FO_SetD = 0;
	double AverageResonance_EighthO_FO_SetD;
	
	double OverallResonance_EighthO_2O_SetA = 0;
	double AverageResonance_EighthO_2O_SetA;
	double OverallResonance_EighthO_2O_SetB = 0;
	double AverageResonance_EighthO_2O_SetB;
	double OverallResonance_EighthO_2O_SetC = 0;
	double AverageResonance_EighthO_2O_SetC;
	double OverallResonance_EighthO_2O_SetD = 0;
	double AverageResonance_EighthO_2O_SetD;
	
	double OverallResonance_EighthO_ThirdO_SetA = 0;
	double AverageResonance_EighthO_ThirdO_SetA;
	double OverallResonance_EighthO_ThirdO_SetB = 0;
	double AverageResonance_EighthO_ThirdO_SetB;
	double OverallResonance_EighthO_ThirdO_SetC = 0;
	double AverageResonance_EighthO_ThirdO_SetC;
	double OverallResonance_EighthO_ThirdO_SetD = 0;
	double AverageResonance_EighthO_ThirdO_SetD;
	
	double OverallResonance_EighthO_ForthO_SetA = 0;
	double AverageResonance_EighthO_ForthO_SetA;
	double OverallResonance_EighthO_ForthO_SetB = 0;
	double AverageResonance_EighthO_ForthO_SetB;
	double OverallResonance_EighthO_ForthO_SetC = 0;
	double AverageResonance_EighthO_ForthO_SetC;
	double OverallResonance_EighthO_ForthO_SetD = 0;
	double AverageResonance_EighthO_ForthO_SetD;
	
	double OverallResonance_EighthO_FifthO_SetA = 0;
	double AverageResonance_EighthO_FifthO_SetA;
	double OverallResonance_EighthO_FifthO_SetB = 0;
	double AverageResonance_EighthO_FifthO_SetB;
	double OverallResonance_EighthO_FifthO_SetC = 0;
	double AverageResonance_EighthO_FifthO_SetC;
	double OverallResonance_EighthO_FifthO_SetD = 0;
	double AverageResonance_EighthO_FifthO_SetD;
	
	double OverallResonance_EighthO_SixthO_SetA = 0;
	double AverageResonance_EighthO_SixthO_SetA;
	double OverallResonance_EighthO_SixthO_SetB = 0;
	double AverageResonance_EighthO_SixthO_SetB;
	double OverallResonance_EighthO_SixthO_SetC = 0;
	double AverageResonance_EighthO_SixthO_SetC;
	double OverallResonance_EighthO_SixthO_SetD = 0;
	double AverageResonance_EighthO_SixthO_SetD;
	
	double OverallResonance_EighthO_SeventhO_SetA = 0;
	double AverageResonance_EighthO_SeventhO_SetA;
	double OverallResonance_EighthO_SeventhO_SetB = 0;
	double AverageResonance_EighthO_SeventhO_SetB;
	double OverallResonance_EighthO_SeventhO_SetC = 0;
	double AverageResonance_EighthO_SeventhO_SetC;
	double OverallResonance_EighthO_SeventhO_SetD = 0;
	double AverageResonance_EighthO_SeventhO_SetD;
	
	double OverallResonance_EighthO_EighthO_SetA = 0;
	double AverageResonance_EighthO_EighthO_SetA;
	double OverallResonance_EighthO_EighthO_SetB = 0;
	double AverageResonance_EighthO_EighthO_SetB;
	double OverallResonance_EighthO_EighthO_SetC = 0;
	double AverageResonance_EighthO_EighthO_SetC;
	double OverallResonance_EighthO_EighthO_SetD = 0;
	double AverageResonance_EighthO_EighthO_SetD;
	
	double OverallResonance_EighthO_NinthO_SetA = 0;
	double AverageResonance_EighthO_NinthO_SetA;
	double OverallResonance_EighthO_NinthO_SetB = 0;
	double AverageResonance_EighthO_NinthO_SetB;
	double OverallResonance_EighthO_NinthO_SetC = 0;
	double AverageResonance_EighthO_NinthO_SetC;
	double OverallResonance_EighthO_NinthO_SetD = 0;
	double AverageResonance_EighthO_NinthO_SetD;
	
	int NumOfEighthO_FU_SetA = 0;
	int NumOfEighthO_FU_SetB = 0;
	int NumOfEighthO_FU_SetC = 0;
	int NumOfEighthO_FU_SetD = 0;
	
	int NumOfEighthO_FO_SetA = 0;
	int NumOfEighthO_FO_SetB = 0;
	int NumOfEighthO_FO_SetC = 0;
	int NumOfEighthO_FO_SetD = 0;
	
	int NumOfEighthO_2O_SetA = 0;
	int NumOfEighthO_2O_SetB = 0;
	int NumOfEighthO_2O_SetC = 0;
	int NumOfEighthO_2O_SetD = 0;
	
	int NumOfEighthO_ThirdO_SetA = 0;
	int NumOfEighthO_ThirdO_SetB = 0;
	int NumOfEighthO_ThirdO_SetC = 0;
	int NumOfEighthO_ThirdO_SetD = 0;
	
	int NumOfEighthO_ForthO_SetA = 0;
	int NumOfEighthO_ForthO_SetB = 0;
	int NumOfEighthO_ForthO_SetC = 0;
	int NumOfEighthO_ForthO_SetD = 0;
	
	int NumOfEighthO_FifthO_SetA = 0;
	int NumOfEighthO_FifthO_SetB = 0;
	int NumOfEighthO_FifthO_SetC = 0;
	int NumOfEighthO_FifthO_SetD = 0;
	
	int NumOfEighthO_SixthO_SetA = 0;
	int NumOfEighthO_SixthO_SetB = 0;
	int NumOfEighthO_SixthO_SetC = 0;
	int NumOfEighthO_SixthO_SetD = 0;
	
	int NumOfEighthO_SeventhO_SetA = 0;
	int NumOfEighthO_SeventhO_SetB = 0;
	int NumOfEighthO_SeventhO_SetC = 0;
	int NumOfEighthO_SeventhO_SetD = 0;
	
	int NumOfEighthO_EighthO_SetA = 0;
	int NumOfEighthO_EighthO_SetB = 0;
	int NumOfEighthO_EighthO_SetC = 0;
	int NumOfEighthO_EighthO_SetD = 0;
	
	int NumOfEighthO_NinthO_SetA = 0;
	int NumOfEighthO_NinthO_SetB = 0;
	int NumOfEighthO_NinthO_SetC = 0;
	int NumOfEighthO_NinthO_SetD = 0;
	
	//////////////////////////////////////////////////////
	
	double ResonanceRatio_NinthO_FU_SetA[nArray];
	double ResonanceRatio_NinthO_FO_SetA[nArray];
	double ResonanceRatio_NinthO_2O_SetA[nArray];
	double ResonanceRatio_NinthO_ThirdO_SetA[nArray];
	double ResonanceRatio_NinthO_ForthO_SetA[nArray];
	double ResonanceRatio_NinthO_FifthO_SetA[nArray];
	double ResonanceRatio_NinthO_SixthO_SetA[nArray];
	double ResonanceRatio_NinthO_SeventhO_SetA[nArray];
	double ResonanceRatio_NinthO_EighthO_SetA[nArray];
	double ResonanceRatio_NinthO_NinthO_SetA[nArray];
	
	double ResonanceRatio_NinthO_FU_SetB[nArray];
	double ResonanceRatio_NinthO_FO_SetB[nArray];
	double ResonanceRatio_NinthO_2O_SetB[nArray];
	double ResonanceRatio_NinthO_ThirdO_SetB[nArray];
	double ResonanceRatio_NinthO_ForthO_SetB[nArray];
	double ResonanceRatio_NinthO_FifthO_SetB[nArray];
	double ResonanceRatio_NinthO_SixthO_SetB[nArray];
	double ResonanceRatio_NinthO_SeventhO_SetB[nArray];
	double ResonanceRatio_NinthO_EighthO_SetB[nArray];
	double ResonanceRatio_NinthO_NinthO_SetB[nArray];
	
	double ResonanceRatio_NinthO_FU_SetC[nArray];
	double ResonanceRatio_NinthO_FO_SetC[nArray];
	double ResonanceRatio_NinthO_2O_SetC[nArray];
	double ResonanceRatio_NinthO_ThirdO_SetC[nArray];
	double ResonanceRatio_NinthO_ForthO_SetC[nArray];
	double ResonanceRatio_NinthO_FifthO_SetC[nArray];
	double ResonanceRatio_NinthO_SixthO_SetC[nArray];
	double ResonanceRatio_NinthO_SeventhO_SetC[nArray];
	double ResonanceRatio_NinthO_EighthO_SetC[nArray];
	double ResonanceRatio_NinthO_NinthO_SetC[nArray];
	
	double ResonanceRatio_NinthO_FU_SetD[nArray];
	double ResonanceRatio_NinthO_FO_SetD[nArray];
	double ResonanceRatio_NinthO_2O_SetD[nArray];
	double ResonanceRatio_NinthO_ThirdO_SetD[nArray];
	double ResonanceRatio_NinthO_ForthO_SetD[nArray];
	double ResonanceRatio_NinthO_FifthO_SetD[nArray];
	double ResonanceRatio_NinthO_SixthO_SetD[nArray];
	double ResonanceRatio_NinthO_SeventhO_SetD[nArray];
	double ResonanceRatio_NinthO_EighthO_SetD[nArray];
	double ResonanceRatio_NinthO_NinthO_SetD[nArray];
	
	double OverallResonance_NinthO_FU_SetA = 0;
	double AverageResonance_NinthO_FU_SetA;
	double OverallResonance_NinthO_FU_SetB = 0;
	double AverageResonance_NinthO_FU_SetB;
	double OverallResonance_NinthO_FU_SetC = 0;
	double AverageResonance_NinthO_FU_SetC;
	double OverallResonance_NinthO_FU_SetD = 0;
	double AverageResonance_NinthO_FU_SetD;
	
	double OverallResonance_NinthO_FO_SetA = 0;
	double AverageResonance_NinthO_FO_SetA;
	double OverallResonance_NinthO_FO_SetB = 0;
	double AverageResonance_NinthO_FO_SetB;
	double OverallResonance_NinthO_FO_SetC = 0;
	double AverageResonance_NinthO_FO_SetC;
	double OverallResonance_NinthO_FO_SetD = 0;
	double AverageResonance_NinthO_FO_SetD;
	
	double OverallResonance_NinthO_2O_SetA = 0;
	double AverageResonance_NinthO_2O_SetA;
	double OverallResonance_NinthO_2O_SetB = 0;
	double AverageResonance_NinthO_2O_SetB;
	double OverallResonance_NinthO_2O_SetC = 0;
	double AverageResonance_NinthO_2O_SetC;
	double OverallResonance_NinthO_2O_SetD = 0;
	double AverageResonance_NinthO_2O_SetD;
	
	double OverallResonance_NinthO_ThirdO_SetA = 0;
	double AverageResonance_NinthO_ThirdO_SetA;
	double OverallResonance_NinthO_ThirdO_SetB = 0;
	double AverageResonance_NinthO_ThirdO_SetB;
	double OverallResonance_NinthO_ThirdO_SetC = 0;
	double AverageResonance_NinthO_ThirdO_SetC;
	double OverallResonance_NinthO_ThirdO_SetD = 0;
	double AverageResonance_NinthO_ThirdO_SetD;
	
	double OverallResonance_NinthO_ForthO_SetA = 0;
	double AverageResonance_NinthO_ForthO_SetA;
	double OverallResonance_NinthO_ForthO_SetB = 0;
	double AverageResonance_NinthO_ForthO_SetB;
	double OverallResonance_NinthO_ForthO_SetC = 0;
	double AverageResonance_NinthO_ForthO_SetC;
	double OverallResonance_NinthO_ForthO_SetD = 0;
	double AverageResonance_NinthO_ForthO_SetD;
	
	double OverallResonance_NinthO_FifthO_SetA = 0;
	double AverageResonance_NinthO_FifthO_SetA;
	double OverallResonance_NinthO_FifthO_SetB = 0;
	double AverageResonance_NinthO_FifthO_SetB;
	double OverallResonance_NinthO_FifthO_SetC = 0;
	double AverageResonance_NinthO_FifthO_SetC;
	double OverallResonance_NinthO_FifthO_SetD = 0;
	double AverageResonance_NinthO_FifthO_SetD;
	
	double OverallResonance_NinthO_SixthO_SetA = 0;
	double AverageResonance_NinthO_SixthO_SetA;
	double OverallResonance_NinthO_SixthO_SetB = 0;
	double AverageResonance_NinthO_SixthO_SetB;
	double OverallResonance_NinthO_SixthO_SetC = 0;
	double AverageResonance_NinthO_SixthO_SetC;
	double OverallResonance_NinthO_SixthO_SetD = 0;
	double AverageResonance_NinthO_SixthO_SetD;
	
	double OverallResonance_NinthO_SeventhO_SetA = 0;
	double AverageResonance_NinthO_SeventhO_SetA;
	double OverallResonance_NinthO_SeventhO_SetB = 0;
	double AverageResonance_NinthO_SeventhO_SetB;
	double OverallResonance_NinthO_SeventhO_SetC = 0;
	double AverageResonance_NinthO_SeventhO_SetC;
	double OverallResonance_NinthO_SeventhO_SetD = 0;
	double AverageResonance_NinthO_SeventhO_SetD;
	
	double OverallResonance_NinthO_EighthO_SetA = 0;
	double AverageResonance_NinthO_EighthO_SetA;
	double OverallResonance_NinthO_EighthO_SetB = 0;
	double AverageResonance_NinthO_EighthO_SetB;
	double OverallResonance_NinthO_EighthO_SetC = 0;
	double AverageResonance_NinthO_EighthO_SetC;
	double OverallResonance_NinthO_EighthO_SetD = 0;
	double AverageResonance_NinthO_EighthO_SetD;
	
	double OverallResonance_NinthO_NinthO_SetA = 0;
	double AverageResonance_NinthO_NinthO_SetA;
	double OverallResonance_NinthO_NinthO_SetB = 0;
	double AverageResonance_NinthO_NinthO_SetB;
	double OverallResonance_NinthO_NinthO_SetC = 0;
	double AverageResonance_NinthO_NinthO_SetC;
	double OverallResonance_NinthO_NinthO_SetD = 0;
	double AverageResonance_NinthO_NinthO_SetD;
	
	int NumOfNinthO_FU_SetA = 0;
	int NumOfNinthO_FU_SetB = 0;
	int NumOfNinthO_FU_SetC = 0;
	int NumOfNinthO_FU_SetD = 0;
	
	int NumOfNinthO_FO_SetA = 0;
	int NumOfNinthO_FO_SetB = 0;
	int NumOfNinthO_FO_SetC = 0;
	int NumOfNinthO_FO_SetD = 0;
	
	int NumOfNinthO_2O_SetA = 0;
	int NumOfNinthO_2O_SetB = 0;
	int NumOfNinthO_2O_SetC = 0;
	int NumOfNinthO_2O_SetD = 0;
	
	int NumOfNinthO_ThirdO_SetA = 0;
	int NumOfNinthO_ThirdO_SetB = 0;
	int NumOfNinthO_ThirdO_SetC = 0;
	int NumOfNinthO_ThirdO_SetD = 0;
	
	int NumOfNinthO_ForthO_SetA = 0;
	int NumOfNinthO_ForthO_SetB = 0;
	int NumOfNinthO_ForthO_SetC = 0;
	int NumOfNinthO_ForthO_SetD = 0;
	
	int NumOfNinthO_FifthO_SetA = 0;
	int NumOfNinthO_FifthO_SetB = 0;
	int NumOfNinthO_FifthO_SetC = 0;
	int NumOfNinthO_FifthO_SetD = 0;
	
	int NumOfNinthO_SixthO_SetA = 0;
	int NumOfNinthO_SixthO_SetB = 0;
	int NumOfNinthO_SixthO_SetC = 0;
	int NumOfNinthO_SixthO_SetD = 0;
	
	int NumOfNinthO_SeventhO_SetA = 0;
	int NumOfNinthO_SeventhO_SetB = 0;
	int NumOfNinthO_SeventhO_SetC = 0;
	int NumOfNinthO_SeventhO_SetD = 0;
	
	int NumOfNinthO_EighthO_SetA = 0;
	int NumOfNinthO_EighthO_SetB = 0;
	int NumOfNinthO_EighthO_SetC = 0;
	int NumOfNinthO_EighthO_SetD = 0;
	
	int NumOfNinthO_NinthO_SetA = 0;
	int NumOfNinthO_NinthO_SetB = 0;
	int NumOfNinthO_NinthO_SetC = 0;
	int NumOfNinthO_NinthO_SetD = 0;
	
	/////////////////////////////////////////////////
	
	///New Varibles///
	const int nArray = NumOfModels + 1;
	
	// double mode;
	// double period;
	// double growth;
	
	double period_FU;
	double growth_FU;
	
	double period_FO;
	double growth_FO;
	
	double period_2O;
	double growth_2O;
	
	double period_ThirdO;
	double growth_ThirdO;
	
	double period_ForthO;
	double growth_ForthO;
	
	double period_FifthO;
	double growth_FifthO;
	
	double period_SixthO;
	double growth_SixthO;
	
	double period_SeventhO;
	double growth_SeventhO;
	
	double period_EighthO;
	double growth_EighthO;
	
	double period_NinthO;
	double growth_NinthO;
	
	
	//////////////////////////////////////
	
	for(int j = 0; j < NumOfSets; j++){
		
		ofstream outFile_AverageResonance;
		
		char AverageResonanceFileName[100];
		strcpy(AverageResonanceFileName, outputFileName);
		strcat(AverageResonanceFileName, "_AverageResonances");
		
		if(j == 0){
			strcat(AverageResonanceFileName, SetA_Suffix);
		}
		if(j == 1){
			strcat(AverageResonanceFileName, SetB_Suffix);
		}
		if(j == 2){
			strcat(AverageResonanceFileName, SetC_Suffix);
		}
		if(j == 3){
			strcat(AverageResonanceFileName, SetD_Suffix);
		}
		
		cout<<"AverageResonanceFileName (output): "<<AverageResonanceFileName<<endl;
		//outFile_AverageResonance.open(AverageResonanceFileName, ios::out);
		
		for(int i = 0; i < NumOfModels; i++){
			
			//////Reading LINA file////////
			
			//Getting model number into string
			stringstream stream;
				stream<<(i+1);
				string Model = stream.str();
				//cout<<"Current Model in string form: "<<Model<<endl;
			
			string LINA_header; //Varible for header of LINA file
			string readout; //String thats viewing each line
			char LINAFilePath[100];
			ifstream inFile;
			
			if(j == 0){
				strcpy(LINAFilePath, log_directory_prefix_SetA);
			}
			if(j == 1){
				strcpy(LINAFilePath, log_directory_prefix_SetB);
			}
			if(j == 2){
				strcpy(LINAFilePath, log_directory_prefix_SetC);
			}
			if(j == 3){
				strcpy(LINAFilePath, log_directory_prefix_SetD);
			}
			strcat(LINAFilePath, Model.c_str());
			strcat(LINAFilePath, "/");
			strcat(LINAFilePath, LINAFileName);
			cout<<"Path to LINA file: "<<LINAFilePath<<endl;
			
			inFile.open(LINAFilePath,ios::in);
		
			// Warning if file cant be opened
			if(!inFile.is_open()){ 
				cout << "Error opening LINA file. Model did not run. \n";
			}
			if(inFile.is_open()){
				cout<<"LINA File was opened successfully"<<endl;			
			}
			if(inFile.good()){
				//reads LINA file into arrays
				getline(inFile,LINA_header);
				//cout<<LINA_header<<endl;
				
				//Loop for number of modes on LINA data. Each iteration looks at a different mode and fills periods to be used
				for(int m = 0; m < NumOfModes; m++){
					
					double mode;
					double period;
					double growth;
					
					inFile>>mode>>period>>growth;
					cout<<mode<<" "<<period<<" "<<growth<<endl;
					
					if(m == 0){
						period_FU = period;
						growth_FU = growth;
					}
					if(m == 1){
						period_FO = period;
						growth_FO = growth;
					}
					if(m == 2){
						period_2O = period;
						growth_2O = growth;
					}
					if(m == 3){
						period_ThirdO = period;
						growth_ThirdO = growth;
					}
					if(m == 4){
						period_ForthO = period;
						growth_ForthO = growth;
					}
					if(m == 5){
						period_FifthO = period;
						growth_FifthO = growth;
					}
					if(m == 6){
						period_SixthO = period;
						growth_SixthO = growth;
					}
					if(m == 7){
						period_SeventhO = period;
						growth_SeventhO = growth;
					}
					if(m == 8){
						period_EighthO = period;
						growth_EighthO = growth;
					}
					if(m == 9){
						period_NinthO = period;
						growth_NinthO = growth;
					}
				}
				inFile.close();
				inFile.open(LINAFilePath,ios::in);
				getline(inFile,LINA_header);
				
				for(int m = 0; m < NumOfModes; m++){
					
					double mode;
					double period;
					double growth;
					
					inFile>>mode>>period>>growth;
					cout<<mode<<" "<<period<<" "<<growth<<endl;
					
					//FU mode//
					if((growth > 0.0 || !DoPosGrowthCut) && (growth_FU > 0)){
						cout<<"Resonance Ratios With FU..."<<endl;
						//SetA
						if(j == 0){
							if(m == 0){
								ResonanceRatio_FU_FU_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_FU_SetA[i]<<endl;
								
								OverallResonance_FU_FU_SetA = OverallResonance_FU_FU_SetA + ResonanceRatio_FU_FU_SetA[i];
								NumOfFU_FU_SetA++;
							}
							if(m == 1){
								ResonanceRatio_FU_FO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_FO_SetA[i]<<endl;
								
								OverallResonance_FU_FO_SetA = OverallResonance_FU_FO_SetA + ResonanceRatio_FU_FO_SetA[i];
								NumOfFU_FO_SetA++;
							}
							if(m == 2){
								ResonanceRatio_FU_2O_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_2O_SetA[i]<<endl;
								
								OverallResonance_FU_2O_SetA = OverallResonance_FU_2O_SetA + ResonanceRatio_FU_2O_SetA[i];
								NumOfFU_2O_SetA++;
							}
							if(m == 3){
								ResonanceRatio_FU_ThirdO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_ThirdO_SetA[i]<<endl;
								
								OverallResonance_FU_ThirdO_SetA = OverallResonance_FU_ThirdO_SetA + ResonanceRatio_FU_ThirdO_SetA[i];
								NumOfFU_ThirdO_SetA++;
							}
							if(m == 4){
								ResonanceRatio_FU_ForthO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_ForthO_SetA[i]<<endl;
								
								OverallResonance_FU_ForthO_SetA = OverallResonance_FU_ForthO_SetA + ResonanceRatio_FU_ForthO_SetA[i];
								NumOfFU_ForthO_SetA++;
							}
							if(m == 5){
								ResonanceRatio_FU_FifthO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_FifthO_SetA[i]<<endl;
								
								OverallResonance_FU_FifthO_SetA = OverallResonance_FU_FifthO_SetA + ResonanceRatio_FU_FifthO_SetA[i];
								NumOfFU_FifthO_SetA++;
							}
							if(m == 6){
								ResonanceRatio_FU_SixthO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_SixthO_SetA[i]<<endl;
								
								OverallResonance_FU_SixthO_SetA = OverallResonance_FU_SixthO_SetA + ResonanceRatio_FU_SixthO_SetA[i];
								NumOfFU_SixthO_SetA++;
							}
							if(m == 7){
								ResonanceRatio_FU_SeventhO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_SeventhO_SetA[i]<<endl;
								
								OverallResonance_FU_SeventhO_SetA = OverallResonance_FU_SeventhO_SetA + ResonanceRatio_FU_SeventhO_SetA[i];
								NumOfFU_SeventhO_SetA++;
							}
							if(m == 8){
								ResonanceRatio_FU_EighthO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_EighthO_SetA[i]<<endl;
								
								OverallResonance_FU_EighthO_SetA = OverallResonance_FU_EighthO_SetA + ResonanceRatio_FU_EighthO_SetA[i];
								NumOfFU_EighthO_SetA++;
							}
							if(m == 9){
								ResonanceRatio_FU_NinthO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_NinthO_SetA[i]<<endl;
								
								OverallResonance_FU_NinthO_SetA = OverallResonance_FU_NinthO_SetA + ResonanceRatio_FU_NinthO_SetA[i];
								NumOfFU_NinthO_SetA++;
							}
						}
						//SetB
						if(j == 1){
							if(m == 0){
								ResonanceRatio_FU_FU_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_FU_SetB[i]<<endl;
								
								OverallResonance_FU_FU_SetB = OverallResonance_FU_FU_SetB + ResonanceRatio_FU_FU_SetB[i];
								NumOfFU_FU_SetB++;
							}
							if(m == 1){
								ResonanceRatio_FU_FO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_FO_SetB[i]<<endl;
								
								OverallResonance_FU_FO_SetB = OverallResonance_FU_FO_SetB + ResonanceRatio_FU_FO_SetB[i];
								NumOfFU_FO_SetB++;
							}
							if(m == 2){
								ResonanceRatio_FU_2O_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_2O_SetB[i]<<endl;
								
								OverallResonance_FU_2O_SetB = OverallResonance_FU_2O_SetB + ResonanceRatio_FU_2O_SetB[i];
								NumOfFU_2O_SetB++;
							}
							if(m == 3){
								ResonanceRatio_FU_ThirdO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_ThirdO_SetB[i]<<endl;
								
								OverallResonance_FU_ThirdO_SetB = OverallResonance_FU_ThirdO_SetB + ResonanceRatio_FU_ThirdO_SetB[i];
								NumOfFU_ThirdO_SetB++;
							}
							if(m == 4){
								ResonanceRatio_FU_ForthO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_ForthO_SetB[i]<<endl;
								
								OverallResonance_FU_ForthO_SetB = OverallResonance_FU_ForthO_SetB + ResonanceRatio_FU_ForthO_SetB[i];
								NumOfFU_ForthO_SetB++;
							}
							if(m == 5){
								ResonanceRatio_FU_FifthO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_FifthO_SetB[i]<<endl;
								
								OverallResonance_FU_FifthO_SetB = OverallResonance_FU_FifthO_SetB + ResonanceRatio_FU_FifthO_SetB[i];
								NumOfFU_FifthO_SetB++;
							}
							if(m == 6){
								ResonanceRatio_FU_SixthO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_SixthO_SetB[i]<<endl;
								
								OverallResonance_FU_SixthO_SetB = OverallResonance_FU_SixthO_SetB + ResonanceRatio_FU_SixthO_SetB[i];
								NumOfFU_SixthO_SetB++;
							}
							if(m == 7){
								ResonanceRatio_FU_SeventhO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_SeventhO_SetB[i]<<endl;
								
								OverallResonance_FU_SeventhO_SetB = OverallResonance_FU_SeventhO_SetB + ResonanceRatio_FU_SeventhO_SetB[i];
								NumOfFU_SeventhO_SetB++;
							}
							if(m == 8){
								ResonanceRatio_FU_EighthO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_EighthO_SetB[i]<<endl;
								
								OverallResonance_FU_EighthO_SetB = OverallResonance_FU_EighthO_SetB + ResonanceRatio_FU_EighthO_SetB[i];
								NumOfFU_EighthO_SetB++;
							}
							if(m == 9){
								ResonanceRatio_FU_NinthO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_NinthO_SetB[i]<<endl;
								
								OverallResonance_FU_NinthO_SetB = OverallResonance_FU_NinthO_SetB + ResonanceRatio_FU_NinthO_SetB[i];
								NumOfFU_NinthO_SetB++;
							}
						}
						//SetC
						if(j == 2){
							if(m == 0){
								ResonanceRatio_FU_FU_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_FU_SetC[i]<<endl;
								
								OverallResonance_FU_FU_SetC = OverallResonance_FU_FU_SetC + ResonanceRatio_FU_FU_SetC[i];
								NumOfFU_FU_SetC++;
							}
							if(m == 1){
								ResonanceRatio_FU_FO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_FO_SetC[i]<<endl;
								
								OverallResonance_FU_FO_SetC = OverallResonance_FU_FO_SetC + ResonanceRatio_FU_FO_SetC[i];
								NumOfFU_FO_SetC++;
							}
							if(m == 2){
								ResonanceRatio_FU_2O_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_2O_SetC[i]<<endl;
								
								OverallResonance_FU_2O_SetC = OverallResonance_FU_2O_SetC + ResonanceRatio_FU_2O_SetC[i];
								NumOfFU_2O_SetC++;
							}
							if(m == 3){
								ResonanceRatio_FU_ThirdO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_ThirdO_SetC[i]<<endl;
								
								OverallResonance_FU_ThirdO_SetC = OverallResonance_FU_ThirdO_SetC + ResonanceRatio_FU_ThirdO_SetC[i];
								NumOfFU_ThirdO_SetC++;
							}
							if(m == 4){
								ResonanceRatio_FU_ForthO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_ForthO_SetC[i]<<endl;
								
								OverallResonance_FU_ForthO_SetC = OverallResonance_FU_ForthO_SetC + ResonanceRatio_FU_ForthO_SetC[i];
								NumOfFU_ForthO_SetC++;
							}
							if(m == 5){
								ResonanceRatio_FU_FifthO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_FifthO_SetC[i]<<endl;
								
								OverallResonance_FU_FifthO_SetC = OverallResonance_FU_FifthO_SetC + ResonanceRatio_FU_FifthO_SetC[i];
								NumOfFU_FifthO_SetC++;
							}
							if(m == 6){
								ResonanceRatio_FU_SixthO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_SixthO_SetC[i]<<endl;
								
								OverallResonance_FU_SixthO_SetC = OverallResonance_FU_SixthO_SetC + ResonanceRatio_FU_SixthO_SetC[i];
								NumOfFU_SixthO_SetC++;
							}
							if(m == 7){
								ResonanceRatio_FU_SeventhO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_SeventhO_SetC[i]<<endl;
								
								OverallResonance_FU_SeventhO_SetC = OverallResonance_FU_SeventhO_SetC + ResonanceRatio_FU_SeventhO_SetC[i];
								NumOfFU_SeventhO_SetC++;
							}
							if(m == 8){
								ResonanceRatio_FU_EighthO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_EighthO_SetC[i]<<endl;
								
								OverallResonance_FU_EighthO_SetC = OverallResonance_FU_EighthO_SetC + ResonanceRatio_FU_EighthO_SetC[i];
								NumOfFU_EighthO_SetC++;
							}
							if(m == 9){
								ResonanceRatio_FU_NinthO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_NinthO_SetC[i]<<endl;
								
								OverallResonance_FU_NinthO_SetC = OverallResonance_FU_NinthO_SetC + ResonanceRatio_FU_NinthO_SetC[i];
								NumOfFU_NinthO_SetC++;
							}
						}
						//SetD
						if(j == 3){
							if(m == 0){
								ResonanceRatio_FU_FU_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_FU_SetD[i]<<endl;
								
								OverallResonance_FU_FU_SetD = OverallResonance_FU_FU_SetD + ResonanceRatio_FU_FU_SetD[i];
								NumOfFU_FU_SetD++;
							}
							if(m == 1){
								ResonanceRatio_FU_FO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_FO_SetD[i]<<endl;
								
								OverallResonance_FU_FO_SetD = OverallResonance_FU_FO_SetD + ResonanceRatio_FU_FO_SetD[i];
								NumOfFU_FO_SetD++;
							}
							if(m == 2){
								ResonanceRatio_FU_2O_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_2O_SetD[i]<<endl;
								
								OverallResonance_FU_2O_SetD = OverallResonance_FU_2O_SetD + ResonanceRatio_FU_2O_SetD[i];
								NumOfFU_2O_SetD++;
							}
							if(m == 3){
								ResonanceRatio_FU_ThirdO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_ThirdO_SetD[i]<<endl;
								
								OverallResonance_FU_ThirdO_SetD = OverallResonance_FU_ThirdO_SetD + ResonanceRatio_FU_ThirdO_SetD[i];
								NumOfFU_ThirdO_SetD++;
							}
							if(m == 4){
								ResonanceRatio_FU_ForthO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_ForthO_SetD[i]<<endl;
								
								OverallResonance_FU_ForthO_SetD = OverallResonance_FU_ForthO_SetD + ResonanceRatio_FU_ForthO_SetD[i];
								NumOfFU_ForthO_SetD++;
							}
							if(m == 5){
								ResonanceRatio_FU_FifthO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_FifthO_SetD[i]<<endl;
								
								OverallResonance_FU_FifthO_SetD = OverallResonance_FU_FifthO_SetD + ResonanceRatio_FU_FifthO_SetD[i];
								NumOfFU_FifthO_SetD++;
							}
							if(m == 6){
								ResonanceRatio_FU_SixthO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_SixthO_SetD[i]<<endl;
								
								OverallResonance_FU_SixthO_SetD = OverallResonance_FU_SixthO_SetD + ResonanceRatio_FU_SixthO_SetD[i];
								NumOfFU_SixthO_SetD++;
							}
							if(m == 7){
								ResonanceRatio_FU_SeventhO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_SeventhO_SetD[i]<<endl;
								
								OverallResonance_FU_SeventhO_SetD = OverallResonance_FU_SeventhO_SetD + ResonanceRatio_FU_SeventhO_SetD[i];
								NumOfFU_SeventhO_SetD++;
							}
							if(m == 8){
								ResonanceRatio_FU_EighthO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_EighthO_SetD[i]<<endl;
								
								OverallResonance_FU_EighthO_SetD = OverallResonance_FU_EighthO_SetD + ResonanceRatio_FU_EighthO_SetD[i];
								NumOfFU_EighthO_SetD++;
							}
							if(m == 9){
								ResonanceRatio_FU_NinthO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_NinthO_SetD[i]<<endl;
								
								OverallResonance_FU_NinthO_SetD = OverallResonance_FU_NinthO_SetD + ResonanceRatio_FU_NinthO_SetD[i];
								NumOfFU_NinthO_SetD++;
							}
						}
					}
					//FO mode//
					if((growth > 0.0 || !DoPosGrowthCut) && (growth_FO > 0)){
						cout<<"Resonance Ratios With FO..."<<endl;
						//SetA
						if(j == 0){
							if(m == 0){
								ResonanceRatio_FO_FU_SetA[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_FU_SetA[i]<<endl;
								
								OverallResonance_FO_FU_SetA = OverallResonance_FO_FU_SetA + ResonanceRatio_FO_FU_SetA[i];
								NumOfFO_FU_SetA++;
							}
							if(m == 1){
								ResonanceRatio_FO_FO_SetA[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_FO_SetA[i]<<endl;
								
								OverallResonance_FO_FO_SetA = OverallResonance_FO_FO_SetA + ResonanceRatio_FO_FO_SetA[i];
								NumOfFO_FO_SetA++;
							}
							if(m == 2){
								ResonanceRatio_FO_2O_SetA[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_2O_SetA[i]<<endl;
								
								OverallResonance_FO_2O_SetA = OverallResonance_FO_2O_SetA + ResonanceRatio_FO_2O_SetA[i];
								NumOfFO_2O_SetA++;
							}
							if(m == 3){
								ResonanceRatio_FO_ThirdO_SetA[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_ThirdO_SetA[i]<<endl;
								
								OverallResonance_FO_ThirdO_SetA = OverallResonance_FO_ThirdO_SetA + ResonanceRatio_FO_ThirdO_SetA[i];
								NumOfFO_ThirdO_SetA++;
							}
							if(m == 4){
								ResonanceRatio_FO_ForthO_SetA[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_ForthO_SetA[i]<<endl;
								
								OverallResonance_FO_ForthO_SetA = OverallResonance_FO_ForthO_SetA + ResonanceRatio_FO_ForthO_SetA[i];
								NumOfFO_ForthO_SetA++;
							}
							if(m == 5){
								ResonanceRatio_FO_FifthO_SetA[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_FifthO_SetA[i]<<endl;
								
								OverallResonance_FO_FifthO_SetA = OverallResonance_FO_FifthO_SetA + ResonanceRatio_FO_FifthO_SetA[i];
								NumOfFO_FifthO_SetA++;
							}
							if(m == 6){
								ResonanceRatio_FO_SixthO_SetA[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_SixthO_SetA[i]<<endl;
								
								OverallResonance_FO_SixthO_SetA = OverallResonance_FO_SixthO_SetA + ResonanceRatio_FO_SixthO_SetA[i];
								NumOfFO_SixthO_SetA++;
							}
							if(m == 7){
								ResonanceRatio_FO_SeventhO_SetA[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_SeventhO_SetA[i]<<endl;
								
								OverallResonance_FO_SeventhO_SetA = OverallResonance_FO_SeventhO_SetA + ResonanceRatio_FO_SeventhO_SetA[i];
								NumOfFO_SeventhO_SetA++;
							}
							if(m == 8){
								ResonanceRatio_FO_EighthO_SetA[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_EighthO_SetA[i]<<endl;
								
								OverallResonance_FO_EighthO_SetA = OverallResonance_FO_EighthO_SetA + ResonanceRatio_FO_EighthO_SetA[i];
								NumOfFO_EighthO_SetA++;
							}
							if(m == 9){
								ResonanceRatio_FO_NinthO_SetA[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_NinthO_SetA[i]<<endl;
								
								OverallResonance_FO_NinthO_SetA = OverallResonance_FO_NinthO_SetA + ResonanceRatio_FO_NinthO_SetA[i];
								NumOfFO_NinthO_SetA++;
							}
						}
						//SetB
						if(j == 1){
							if(m == 0){
								ResonanceRatio_FO_FU_SetB[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_FU_SetB[i]<<endl;
								
								OverallResonance_FO_FU_SetB = OverallResonance_FO_FU_SetB + ResonanceRatio_FO_FU_SetB[i];
								NumOfFO_FU_SetB++;
							}
							if(m == 1){
								ResonanceRatio_FO_FO_SetB[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_FO_SetB[i]<<endl;
								
								OverallResonance_FO_FO_SetB = OverallResonance_FO_FO_SetB + ResonanceRatio_FO_FO_SetB[i];
								NumOfFO_FO_SetB++;
							}
							if(m == 2){
								ResonanceRatio_FO_2O_SetB[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_2O_SetB[i]<<endl;
								
								OverallResonance_FO_2O_SetB = OverallResonance_FO_2O_SetB + ResonanceRatio_FO_2O_SetB[i];
								NumOfFO_2O_SetB++;
							}
							if(m == 3){
								ResonanceRatio_FO_ThirdO_SetB[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_ThirdO_SetB[i]<<endl;
								
								OverallResonance_FO_ThirdO_SetB = OverallResonance_FO_ThirdO_SetB + ResonanceRatio_FO_ThirdO_SetB[i];
								NumOfFO_ThirdO_SetB++;
							}
							if(m == 4){
								ResonanceRatio_FO_ForthO_SetB[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_ForthO_SetB[i]<<endl;
								
								OverallResonance_FO_ForthO_SetB = OverallResonance_FO_ForthO_SetB + ResonanceRatio_FO_ForthO_SetB[i];
								NumOfFO_ForthO_SetB++;
							}
							if(m == 5){
								ResonanceRatio_FO_FifthO_SetB[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_FifthO_SetB[i]<<endl;
								
								OverallResonance_FO_FifthO_SetB = OverallResonance_FO_FifthO_SetB + ResonanceRatio_FO_FifthO_SetB[i];
								NumOfFO_FifthO_SetB++;
							}
							if(m == 6){
								ResonanceRatio_FO_SixthO_SetB[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_SixthO_SetB[i]<<endl;
								
								OverallResonance_FO_SixthO_SetB = OverallResonance_FO_SixthO_SetB + ResonanceRatio_FO_SixthO_SetB[i];
								NumOfFO_SixthO_SetB++;
							}
							if(m == 7){
								ResonanceRatio_FO_SeventhO_SetB[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_SeventhO_SetB[i]<<endl;
								
								OverallResonance_FO_SeventhO_SetB = OverallResonance_FO_SeventhO_SetB + ResonanceRatio_FO_SeventhO_SetB[i];
								NumOfFO_SeventhO_SetB++;
							}
							if(m == 8){
								ResonanceRatio_FO_EighthO_SetB[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_EighthO_SetB[i]<<endl;
								
								OverallResonance_FO_EighthO_SetB = OverallResonance_FO_EighthO_SetB + ResonanceRatio_FO_EighthO_SetB[i];
								NumOfFO_EighthO_SetB++;
							}
							if(m == 9){
								ResonanceRatio_FO_NinthO_SetB[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_NinthO_SetB[i]<<endl;
								
								OverallResonance_FO_NinthO_SetB = OverallResonance_FO_NinthO_SetB + ResonanceRatio_FO_NinthO_SetB[i];
								NumOfFO_NinthO_SetB++;
							}
						}
						//SetC
						if(j == 2){
							if(m == 0){
								ResonanceRatio_FO_FU_SetC[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_FU_SetC[i]<<endl;
								
								OverallResonance_FO_FU_SetC = OverallResonance_FO_FU_SetC + ResonanceRatio_FO_FU_SetC[i];
								NumOfFO_FU_SetC++;
							}
							if(m == 1){
								ResonanceRatio_FO_FO_SetC[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_FO_SetC[i]<<endl;
								
								OverallResonance_FO_FO_SetC = OverallResonance_FO_FO_SetC + ResonanceRatio_FO_FO_SetC[i];
								NumOfFO_FO_SetC++;
							}
							if(m == 2){
								ResonanceRatio_FO_2O_SetC[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_2O_SetC[i]<<endl;
								
								OverallResonance_FO_2O_SetC = OverallResonance_FO_2O_SetC + ResonanceRatio_FO_2O_SetC[i];
								NumOfFO_2O_SetC++;
							}
							if(m == 3){
								ResonanceRatio_FO_ThirdO_SetC[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_ThirdO_SetC[i]<<endl;
								
								OverallResonance_FO_ThirdO_SetC = OverallResonance_FO_ThirdO_SetC + ResonanceRatio_FO_ThirdO_SetC[i];
								NumOfFO_ThirdO_SetC++;
							}
							if(m == 4){
								ResonanceRatio_FO_ForthO_SetC[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_ForthO_SetC[i]<<endl;
								
								OverallResonance_FO_ForthO_SetC = OverallResonance_FO_ForthO_SetC + ResonanceRatio_FO_ForthO_SetC[i];
								NumOfFO_ForthO_SetC++;
							}
							if(m == 5){
								ResonanceRatio_FO_FifthO_SetC[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_FifthO_SetC[i]<<endl;
								
								OverallResonance_FO_FifthO_SetC = OverallResonance_FO_FifthO_SetC + ResonanceRatio_FO_FifthO_SetC[i];
								NumOfFO_FifthO_SetC++;
							}
							if(m == 6){
								ResonanceRatio_FO_SixthO_SetC[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_SixthO_SetC[i]<<endl;
								
								OverallResonance_FO_SixthO_SetC = OverallResonance_FO_SixthO_SetC + ResonanceRatio_FO_SixthO_SetC[i];
								NumOfFO_SixthO_SetC++;
							}
							if(m == 7){
								ResonanceRatio_FO_SeventhO_SetC[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_SeventhO_SetC[i]<<endl;
								
								OverallResonance_FO_SeventhO_SetC = OverallResonance_FO_SeventhO_SetC + ResonanceRatio_FO_SeventhO_SetC[i];
								NumOfFO_SeventhO_SetC++;
							}
							if(m == 8){
								ResonanceRatio_FO_EighthO_SetC[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_EighthO_SetC[i]<<endl;
								
								OverallResonance_FO_EighthO_SetC = OverallResonance_FO_EighthO_SetC + ResonanceRatio_FO_EighthO_SetC[i];
								NumOfFO_EighthO_SetC++;
							}
							if(m == 9){
								ResonanceRatio_FO_NinthO_SetC[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_NinthO_SetC[i]<<endl;
								
								OverallResonance_FO_NinthO_SetC = OverallResonance_FO_NinthO_SetC + ResonanceRatio_FO_NinthO_SetC[i];
								NumOfFO_NinthO_SetC++;
							}
						}
						//SetD
						if(j == 3){
							if(m == 0){
								ResonanceRatio_FO_FU_SetD[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_FU_SetD[i]<<endl;
								
								OverallResonance_FO_FU_SetD = OverallResonance_FO_FU_SetD + ResonanceRatio_FO_FU_SetD[i];
								NumOfFO_FU_SetD++;
							}
							if(m == 1){
								ResonanceRatio_FO_FO_SetD[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_FO_SetD[i]<<endl;
								
								OverallResonance_FO_FO_SetD = OverallResonance_FO_FO_SetD + ResonanceRatio_FO_FO_SetD[i];
								NumOfFO_FO_SetD++;
							}
							if(m == 2){
								ResonanceRatio_FO_2O_SetD[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_2O_SetD[i]<<endl;
								
								OverallResonance_FO_2O_SetD = OverallResonance_FO_2O_SetD + ResonanceRatio_FO_2O_SetD[i];
								NumOfFO_2O_SetD++;
							}
							if(m == 3){
								ResonanceRatio_FO_ThirdO_SetD[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_ThirdO_SetD[i]<<endl;
								
								OverallResonance_FO_ThirdO_SetD = OverallResonance_FO_ThirdO_SetD + ResonanceRatio_FO_ThirdO_SetD[i];
								NumOfFO_ThirdO_SetD++;
							}
							if(m == 4){
								ResonanceRatio_FO_ForthO_SetD[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_ForthO_SetD[i]<<endl;
								
								OverallResonance_FO_ForthO_SetD = OverallResonance_FO_ForthO_SetD + ResonanceRatio_FO_ForthO_SetD[i];
								NumOfFO_ForthO_SetD++;
							}
							if(m == 5){
								ResonanceRatio_FO_FifthO_SetD[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_FifthO_SetD[i]<<endl;
								
								OverallResonance_FO_FifthO_SetD = OverallResonance_FO_FifthO_SetD + ResonanceRatio_FO_FifthO_SetD[i];
								NumOfFO_FifthO_SetD++;
							}
							if(m == 6){
								ResonanceRatio_FO_SixthO_SetD[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_SixthO_SetD[i]<<endl;
								
								OverallResonance_FO_SixthO_SetD = OverallResonance_FO_SixthO_SetD + ResonanceRatio_FO_SixthO_SetD[i];
								NumOfFO_SixthO_SetD++;
							}
							if(m == 7){
								ResonanceRatio_FO_SeventhO_SetD[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_SeventhO_SetD[i]<<endl;
								
								OverallResonance_FO_SeventhO_SetD = OverallResonance_FO_SeventhO_SetD + ResonanceRatio_FO_SeventhO_SetD[i];
								NumOfFO_SeventhO_SetD++;
							}
							if(m == 8){
								ResonanceRatio_FO_EighthO_SetD[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_EighthO_SetD[i]<<endl;
								
								OverallResonance_FO_EighthO_SetD = OverallResonance_FO_EighthO_SetD + ResonanceRatio_FO_EighthO_SetD[i];
								NumOfFO_EighthO_SetD++;
							}
							if(m == 9){
								ResonanceRatio_FO_NinthO_SetD[i] = period_FO/period;
								cout<<m<<": "<<ResonanceRatio_FO_NinthO_SetD[i]<<endl;
								
								OverallResonance_FO_NinthO_SetD = OverallResonance_FO_NinthO_SetD + ResonanceRatio_FO_NinthO_SetD[i];
								NumOfFO_NinthO_SetD++;
							}
						}
					}
					//2O mode//
					if((growth > 0.0 || !DoPosGrowthCut) && (growth_2O > 0)){
						cout<<"Resonance Ratios With 2O..."<<endl;
						//SetA
						if(j == 0){
							if(m == 0){
								ResonanceRatio_2O_FU_SetA[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_FU_SetA[i]<<endl;
								
								OverallResonance_2O_FU_SetA = OverallResonance_2O_FU_SetA + ResonanceRatio_2O_FU_SetA[i];
								NumOf2O_FU_SetA++;
							}
							if(m == 1){
								ResonanceRatio_2O_FO_SetA[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_FO_SetA[i]<<endl;
								
								OverallResonance_2O_FO_SetA = OverallResonance_2O_FO_SetA + ResonanceRatio_2O_FO_SetA[i];
								NumOf2O_FO_SetA++;
							}
							if(m == 2){
								ResonanceRatio_2O_2O_SetA[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_2O_SetA[i]<<endl;
								
								OverallResonance_2O_2O_SetA = OverallResonance_2O_2O_SetA + ResonanceRatio_2O_2O_SetA[i];
								NumOf2O_2O_SetA++;
							}
							if(m == 3){
								ResonanceRatio_2O_ThirdO_SetA[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_ThirdO_SetA[i]<<endl;
								
								OverallResonance_2O_ThirdO_SetA = OverallResonance_2O_ThirdO_SetA + ResonanceRatio_2O_ThirdO_SetA[i];
								NumOf2O_ThirdO_SetA++;
							}
							if(m == 4){
								ResonanceRatio_2O_ForthO_SetA[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_ForthO_SetA[i]<<endl;
								
								OverallResonance_2O_ForthO_SetA = OverallResonance_2O_ForthO_SetA + ResonanceRatio_2O_ForthO_SetA[i];
								NumOf2O_ForthO_SetA++;
							}
							if(m == 5){
								ResonanceRatio_2O_FifthO_SetA[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_FifthO_SetA[i]<<endl;
								
								OverallResonance_2O_FifthO_SetA = OverallResonance_2O_FifthO_SetA + ResonanceRatio_2O_FifthO_SetA[i];
								NumOf2O_FifthO_SetA++;
							}
							if(m == 6){
								ResonanceRatio_2O_SixthO_SetA[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_SixthO_SetA[i]<<endl;
								
								OverallResonance_2O_SixthO_SetA = OverallResonance_2O_SixthO_SetA + ResonanceRatio_2O_SixthO_SetA[i];
								NumOf2O_SixthO_SetA++;
							}
							if(m == 7){
								ResonanceRatio_2O_SeventhO_SetA[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_SeventhO_SetA[i]<<endl;
								
								OverallResonance_2O_SeventhO_SetA = OverallResonance_2O_SeventhO_SetA + ResonanceRatio_2O_SeventhO_SetA[i];
								NumOf2O_SeventhO_SetA++;
							}
							if(m == 8){
								ResonanceRatio_2O_EighthO_SetA[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_EighthO_SetA[i]<<endl;
								
								OverallResonance_2O_EighthO_SetA = OverallResonance_2O_EighthO_SetA + ResonanceRatio_2O_EighthO_SetA[i];
								NumOf2O_EighthO_SetA++;
							}
							if(m == 9){
								ResonanceRatio_2O_NinthO_SetA[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_NinthO_SetA[i]<<endl;
								
								OverallResonance_2O_NinthO_SetA = OverallResonance_2O_NinthO_SetA + ResonanceRatio_2O_NinthO_SetA[i];
								NumOf2O_NinthO_SetA++;
							}
						}
						//SetB
						if(j == 1){
							if(m == 0){
								ResonanceRatio_2O_FU_SetB[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_FU_SetB[i]<<endl;
								
								OverallResonance_2O_FU_SetB = OverallResonance_2O_FU_SetB + ResonanceRatio_2O_FU_SetB[i];
								NumOf2O_2O_SetB++;
							}
							if(m == 1){
								ResonanceRatio_2O_FO_SetB[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_FO_SetB[i]<<endl;
								
								OverallResonance_2O_FO_SetB = OverallResonance_2O_FO_SetB + ResonanceRatio_2O_FO_SetB[i];
								NumOf2O_FO_SetB++;
							}
							if(m == 2){
								ResonanceRatio_2O_2O_SetB[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_2O_SetB[i]<<endl;
								
								OverallResonance_2O_2O_SetB = OverallResonance_2O_2O_SetB + ResonanceRatio_2O_2O_SetB[i];
								NumOf2O_2O_SetB++;
							}
							if(m == 3){
								ResonanceRatio_2O_ThirdO_SetB[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_ThirdO_SetB[i]<<endl;
								
								OverallResonance_2O_ThirdO_SetB = OverallResonance_2O_ThirdO_SetB + ResonanceRatio_2O_ThirdO_SetB[i];
								NumOf2O_ThirdO_SetB++;
							}
							if(m == 4){
								ResonanceRatio_2O_ForthO_SetB[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_ForthO_SetB[i]<<endl;
								
								OverallResonance_2O_ForthO_SetB = OverallResonance_2O_ForthO_SetB + ResonanceRatio_2O_ForthO_SetB[i];
								NumOf2O_ForthO_SetB++;
							}
							if(m == 5){
								ResonanceRatio_2O_FifthO_SetB[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_FifthO_SetB[i]<<endl;
								
								OverallResonance_2O_FifthO_SetB = OverallResonance_2O_FifthO_SetB + ResonanceRatio_2O_FifthO_SetB[i];
								NumOf2O_FifthO_SetB++;
							}
							if(m == 6){
								ResonanceRatio_2O_SixthO_SetB[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_SixthO_SetB[i]<<endl;
								
								OverallResonance_2O_SixthO_SetB = OverallResonance_2O_SixthO_SetB + ResonanceRatio_2O_SixthO_SetB[i];
								NumOf2O_SixthO_SetB++;
							}
							if(m == 7){
								ResonanceRatio_2O_SeventhO_SetB[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_SeventhO_SetB[i]<<endl;
								
								OverallResonance_2O_SeventhO_SetB = OverallResonance_2O_SeventhO_SetB + ResonanceRatio_2O_SeventhO_SetB[i];
								NumOf2O_SeventhO_SetB++;
							}
							if(m == 8){
								ResonanceRatio_2O_EighthO_SetB[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_EighthO_SetB[i]<<endl;
								
								OverallResonance_2O_EighthO_SetB = OverallResonance_2O_EighthO_SetB + ResonanceRatio_2O_EighthO_SetB[i];
								NumOf2O_EighthO_SetB++;
							}
							if(m == 9){
								ResonanceRatio_2O_NinthO_SetB[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_NinthO_SetB[i]<<endl;
								
								OverallResonance_2O_NinthO_SetB = OverallResonance_2O_NinthO_SetB + ResonanceRatio_2O_NinthO_SetB[i];
								NumOf2O_NinthO_SetB++;
							}
						}
						//SetC
						if(j == 2){
							if(m == 0){
								ResonanceRatio_2O_FU_SetC[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_FU_SetC[i]<<endl;
								
								OverallResonance_2O_FU_SetC = OverallResonance_2O_FU_SetC + ResonanceRatio_2O_FU_SetC[i];
								NumOf2O_FU_SetC++;
							}
							if(m == 1){
								ResonanceRatio_2O_FO_SetC[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_FO_SetC[i]<<endl;
								
								OverallResonance_2O_FO_SetC = OverallResonance_2O_FO_SetC + ResonanceRatio_2O_FO_SetC[i];
								NumOf2O_FO_SetC++;
							}
							if(m == 2){
								ResonanceRatio_2O_2O_SetC[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_2O_SetC[i]<<endl;
								
								OverallResonance_2O_2O_SetC = OverallResonance_2O_2O_SetC + ResonanceRatio_2O_2O_SetC[i];
								NumOf2O_2O_SetC++;
							}
							if(m == 3){
								ResonanceRatio_2O_ThirdO_SetC[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_ThirdO_SetC[i]<<endl;
								
								OverallResonance_2O_ThirdO_SetC = OverallResonance_2O_ThirdO_SetC + ResonanceRatio_2O_ThirdO_SetC[i];
								NumOf2O_ThirdO_SetC++;
							}
							if(m == 4){
								ResonanceRatio_2O_ForthO_SetC[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_ForthO_SetC[i]<<endl;
								
								OverallResonance_2O_ForthO_SetC = OverallResonance_2O_ForthO_SetC + ResonanceRatio_2O_ForthO_SetC[i];
								NumOf2O_ForthO_SetC++;
							}
							if(m == 5){
								ResonanceRatio_2O_FifthO_SetC[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_FifthO_SetC[i]<<endl;
								
								OverallResonance_2O_FifthO_SetC = OverallResonance_2O_FifthO_SetC + ResonanceRatio_2O_FifthO_SetC[i];
								NumOf2O_FifthO_SetC++;
							}
							if(m == 6){
								ResonanceRatio_2O_SixthO_SetC[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_SixthO_SetC[i]<<endl;
								
								OverallResonance_2O_SixthO_SetC = OverallResonance_2O_SixthO_SetC + ResonanceRatio_2O_SixthO_SetC[i];
								NumOf2O_SixthO_SetC++;
							}
							if(m == 7){
								ResonanceRatio_2O_SeventhO_SetC[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_SeventhO_SetC[i]<<endl;
								
								OverallResonance_2O_SeventhO_SetC = OverallResonance_2O_SeventhO_SetC + ResonanceRatio_2O_SeventhO_SetC[i];
								NumOf2O_SeventhO_SetC++;
							}
							if(m == 8){
								ResonanceRatio_2O_EighthO_SetC[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_EighthO_SetC[i]<<endl;
								
								OverallResonance_2O_EighthO_SetC = OverallResonance_2O_EighthO_SetC + ResonanceRatio_2O_EighthO_SetC[i];
								NumOf2O_EighthO_SetC++;
							}
							if(m == 9){
								ResonanceRatio_2O_NinthO_SetC[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_NinthO_SetC[i]<<endl;
								
								OverallResonance_2O_NinthO_SetC = OverallResonance_2O_NinthO_SetC + ResonanceRatio_2O_NinthO_SetC[i];
								NumOf2O_NinthO_SetC++;
							}
						}
						//SetD
						if(j == 3){
							if(m == 0){
								ResonanceRatio_2O_FU_SetD[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_FU_2O_SetD[i]<<endl;
								
								OverallResonance_2O_FU_SetD = OverallResonance_2O_FU_SetD + ResonanceRatio_2O_FU_SetD[i];
								NumOf2O_FU_SetD++;
							}
							if(m == 1){
								ResonanceRatio_2O_FO_SetD[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_FO_SetD[i]<<endl;
								
								OverallResonance_2O_FO_SetD = OverallResonance_2O_FO_SetD + ResonanceRatio_2O_FO_SetD[i];
								NumOf2O_FO_SetD++;
							}
							if(m == 2){
								ResonanceRatio_2O_2O_SetD[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_2O_SetD[i]<<endl;
								
								OverallResonance_2O_2O_SetD = OverallResonance_2O_2O_SetD + ResonanceRatio_2O_2O_SetD[i];
								NumOf2O_2O_SetD++;
							}
							if(m == 3){
								ResonanceRatio_2O_ThirdO_SetD[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_ThirdO_SetD[i]<<endl;
								
								OverallResonance_2O_ThirdO_SetD = OverallResonance_2O_ThirdO_SetD + ResonanceRatio_2O_ThirdO_SetD[i];
								NumOf2O_ThirdO_SetD++;
							}
							if(m == 4){
								ResonanceRatio_2O_ForthO_SetD[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_ForthO_SetD[i]<<endl;
								
								OverallResonance_2O_ForthO_SetD = OverallResonance_2O_ForthO_SetD + ResonanceRatio_2O_ForthO_SetD[i];
								NumOf2O_ForthO_SetD++;
							}
							if(m == 5){
								ResonanceRatio_2O_FifthO_SetD[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_FifthO_SetD[i]<<endl;
								
								OverallResonance_2O_FifthO_SetD = OverallResonance_2O_FifthO_SetD + ResonanceRatio_2O_FifthO_SetD[i];
								NumOf2O_FifthO_SetD++;
							}
							if(m == 6){
								ResonanceRatio_2O_SixthO_SetD[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_SixthO_SetD[i]<<endl;
								
								OverallResonance_2O_SixthO_SetD = OverallResonance_2O_SixthO_SetD + ResonanceRatio_2O_SixthO_SetD[i];
								NumOf2O_SixthO_SetD++;
							}
							if(m == 7){
								ResonanceRatio_2O_SeventhO_SetD[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_SeventhO_SetD[i]<<endl;
								
								OverallResonance_2O_SeventhO_SetD = OverallResonance_2O_SeventhO_SetD + ResonanceRatio_2O_SeventhO_SetD[i];
								NumOf2O_SeventhO_SetD++;
							}
							if(m == 8){
								ResonanceRatio_2O_EighthO_SetD[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_EighthO_SetD[i]<<endl;
								
								OverallResonance_2O_EighthO_SetD = OverallResonance_2O_EighthO_SetD + ResonanceRatio_2O_EighthO_SetD[i];
								NumOf2O_EighthO_SetD++;
							}
							if(m == 9){
								ResonanceRatio_2O_NinthO_SetD[i] = period_2O/period;
								cout<<m<<": "<<ResonanceRatio_2O_NinthO_SetD[i]<<endl;
								
								OverallResonance_2O_NinthO_SetD = OverallResonance_2O_NinthO_SetD + ResonanceRatio_2O_NinthO_SetD[i];
								NumOf2O_NinthO_SetD++;
							}
						}
					}
					//ThirdO mode//
					if((growth > 0.0 || !DoPosGrowthCut) && (growth_ThirdO > 0)){
						cout<<"Resonance Ratios With ThirdO..."<<endl;
						//SetA
						if(j == 0){
							if(m == 0){
								ResonanceRatio_ThirdO_FU_SetA[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_FU_SetA[i]<<endl;
								
								OverallResonance_ThirdO_FU_SetA = OverallResonance_ThirdO_FU_SetA + ResonanceRatio_ThirdO_FU_SetA[i];
								NumOfThirdO_FU_SetA++;
							}
							if(m == 1){
								ResonanceRatio_ThirdO_FO_SetA[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_FO_SetA[i]<<endl;
								
								OverallResonance_ThirdO_FO_SetA = OverallResonance_ThirdO_FO_SetA + ResonanceRatio_ThirdO_FO_SetA[i];
								NumOfThirdO_FO_SetA++;
							}
							if(m == 2){
								ResonanceRatio_ThirdO_2O_SetA[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_2O_SetA[i]<<endl;
								
								OverallResonance_ThirdO_2O_SetA = OverallResonance_ThirdO_2O_SetA + ResonanceRatio_ThirdO_2O_SetA[i];
								NumOfThirdO_2O_SetA++;
							}
							if(m == 3){
								ResonanceRatio_ThirdO_ThirdO_SetA[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_ThirdO_SetA[i]<<endl;
								
								OverallResonance_ThirdO_ThirdO_SetA = OverallResonance_ThirdO_ThirdO_SetA + ResonanceRatio_ThirdO_ThirdO_SetA[i];
								NumOfThirdO_ThirdO_SetA++;
							}
							if(m == 4){
								ResonanceRatio_ThirdO_ForthO_SetA[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_ForthO_SetA[i]<<endl;
								
								OverallResonance_ThirdO_ForthO_SetA = OverallResonance_ThirdO_ForthO_SetA + ResonanceRatio_ThirdO_ForthO_SetA[i];
								NumOfThirdO_ForthO_SetA++;
							}
							if(m == 5){
								ResonanceRatio_ThirdO_FifthO_SetA[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_FifthO_SetA[i]<<endl;
								
								OverallResonance_ThirdO_FifthO_SetA = OverallResonance_ThirdO_FifthO_SetA + ResonanceRatio_ThirdO_FifthO_SetA[i];
								NumOfThirdO_FifthO_SetA++;
							}
							if(m == 6){
								ResonanceRatio_ThirdO_SixthO_SetA[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_SixthO_SetA[i]<<endl;
								
								OverallResonance_ThirdO_SixthO_SetA = OverallResonance_ThirdO_SixthO_SetA + ResonanceRatio_ThirdO_SixthO_SetA[i];
								NumOfThirdO_SixthO_SetA++;
							}
							if(m == 7){
								ResonanceRatio_ThirdO_SeventhO_SetA[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_SeventhO_SetA[i]<<endl;
								
								OverallResonance_ThirdO_SeventhO_SetA = OverallResonance_ThirdO_SeventhO_SetA + ResonanceRatio_ThirdO_SeventhO_SetA[i];
								NumOfThirdO_SeventhO_SetA++;
							}
							if(m == 8){
								ResonanceRatio_ThirdO_EighthO_SetA[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_EighthO_SetA[i]<<endl;
								
								OverallResonance_ThirdO_EighthO_SetA = OverallResonance_ThirdO_EighthO_SetA + ResonanceRatio_ThirdO_EighthO_SetA[i];
								NumOfThirdO_EighthO_SetA++;
							}
							if(m == 9){
								ResonanceRatio_ThirdO_NinthO_SetA[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_NinthO_SetA[i]<<endl;
								
								OverallResonance_ThirdO_NinthO_SetA = OverallResonance_ThirdO_NinthO_SetA + ResonanceRatio_ThirdO_NinthO_SetA[i];
								NumOfThirdO_NinthO_SetA++;
							}
						}
						//SetB
						if(j == 1){
							if(m == 0){
								ResonanceRatio_ThirdO_FU_SetB[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_FU_SetB[i]<<endl;
								
								OverallResonance_ThirdO_FU_SetB = OverallResonance_ThirdO_FU_SetB + ResonanceRatio_ThirdO_FU_SetB[i];
								NumOfThirdO_FU_SetB++;
							}
							if(m == 1){
								ResonanceRatio_ThirdO_FO_SetB[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_FO_SetB[i]<<endl;
								
								OverallResonance_ThirdO_FO_SetB = OverallResonance_ThirdO_FO_SetB + ResonanceRatio_ThirdO_FO_SetB[i];
								NumOfThirdO_FO_SetB++;
							}
							if(m == 2){
								ResonanceRatio_ThirdO_2O_SetB[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_2O_SetB[i]<<endl;
								
								OverallResonance_ThirdO_2O_SetB = OverallResonance_ThirdO_2O_SetB + ResonanceRatio_ThirdO_2O_SetB[i];
								NumOfThirdO_2O_SetB++;
							}
							if(m == 3){
								ResonanceRatio_ThirdO_ThirdO_SetB[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_ThirdO_SetB[i]<<endl;
								
								OverallResonance_ThirdO_ThirdO_SetB = OverallResonance_ThirdO_ThirdO_SetB + ResonanceRatio_ThirdO_ThirdO_SetB[i];
								NumOfThirdO_ThirdO_SetB++;
							}
							if(m == 4){
								ResonanceRatio_ThirdO_ForthO_SetB[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_ForthO_SetB[i]<<endl;
								
								OverallResonance_ThirdO_ForthO_SetB = OverallResonance_ThirdO_ForthO_SetB + ResonanceRatio_ThirdO_ForthO_SetB[i];
								NumOfThirdO_ForthO_SetB++;
							}
							if(m == 5){
								ResonanceRatio_ThirdO_FifthO_SetB[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_FifthO_SetB[i]<<endl;
								
								OverallResonance_ThirdO_FifthO_SetB = OverallResonance_ThirdO_FifthO_SetB + ResonanceRatio_ThirdO_FifthO_SetB[i];
								NumOfThirdO_FifthO_SetB++;
							}
							if(m == 6){
								ResonanceRatio_ThirdO_SixthO_SetB[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_SixthO_SetB[i]<<endl;
								
								OverallResonance_ThirdO_SixthO_SetB = OverallResonance_ThirdO_SixthO_SetB + ResonanceRatio_ThirdO_SixthO_SetB[i];
								NumOfThirdO_SixthO_SetB++;
							}
							if(m == 7){
								ResonanceRatio_ThirdO_SeventhO_SetB[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_SeventhO_SetB[i]<<endl;
								
								OverallResonance_ThirdO_SeventhO_SetB = OverallResonance_ThirdO_SeventhO_SetB + ResonanceRatio_ThirdO_SeventhO_SetB[i];
								NumOfThirdO_SeventhO_SetB++;
							}
							if(m == 8){
								ResonanceRatio_ThirdO_EighthO_SetB[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_EighthO_SetB[i]<<endl;
								
								OverallResonance_ThirdO_EighthO_SetB = OverallResonance_ThirdO_EighthO_SetB + ResonanceRatio_ThirdO_EighthO_SetB[i];
								NumOfThirdO_EighthO_SetB++;
							}
							if(m == 9){
								ResonanceRatio_ThirdO_NinthO_SetB[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_NinthO_SetB[i]<<endl;
								
								OverallResonance_ThirdO_NinthO_SetB = OverallResonance_ThirdO_NinthO_SetB + ResonanceRatio_ThirdO_NinthO_SetB[i];
								NumOfThirdO_NinthO_SetB++;
							}
						}
						//SetC
						if(j == 2){
							if(m == 0){
								ResonanceRatio_ThirdO_FU_SetC[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_FU_SetC[i]<<endl;
								
								OverallResonance_ThirdO_FU_SetC = OverallResonance_ThirdO_FU_SetC + ResonanceRatio_ThirdO_FU_SetC[i];
								NumOfThirdO_FU_SetC++;
							}
							if(m == 1){
								ResonanceRatio_ThirdO_FO_SetC[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_FO_SetC[i]<<endl;
								
								OverallResonance_ThirdO_FO_SetC = OverallResonance_ThirdO_FO_SetC + ResonanceRatio_ThirdO_FO_SetC[i];
								NumOfThirdO_FO_SetC++;
							}
							if(m == 2){
								ResonanceRatio_ThirdO_2O_SetC[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_2O_SetC[i]<<endl;
								
								OverallResonance_ThirdO_2O_SetC = OverallResonance_ThirdO_2O_SetC + ResonanceRatio_ThirdO_2O_SetC[i];
								NumOfThirdO_2O_SetC++;
							}
							if(m == 3){
								ResonanceRatio_ThirdO_ThirdO_SetC[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_ThirdO_SetC[i]<<endl;
								
								OverallResonance_ThirdO_ThirdO_SetC = OverallResonance_ThirdO_ThirdO_SetC + ResonanceRatio_ThirdO_ThirdO_SetC[i];
								NumOfThirdO_ThirdO_SetC++;
							}
							if(m == 4){
								ResonanceRatio_ThirdO_ForthO_SetC[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_ForthO_SetC[i]<<endl;
								
								OverallResonance_ThirdO_ForthO_SetC = OverallResonance_ThirdO_ForthO_SetC + ResonanceRatio_ThirdO_ForthO_SetC[i];
								NumOfThirdO_ForthO_SetC++;
							}
							if(m == 5){
								ResonanceRatio_ThirdO_FifthO_SetC[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_FifthO_SetC[i]<<endl;
								
								OverallResonance_ThirdO_FifthO_SetC = OverallResonance_ThirdO_FifthO_SetC + ResonanceRatio_ThirdO_FifthO_SetC[i];
								NumOfThirdO_FifthO_SetC++;
							}
							if(m == 6){
								ResonanceRatio_ThirdO_SixthO_SetC[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_SixthO_SetC[i]<<endl;
								
								OverallResonance_ThirdO_SixthO_SetC = OverallResonance_ThirdO_SixthO_SetC + ResonanceRatio_ThirdO_SixthO_SetC[i];
								NumOfThirdO_SixthO_SetC++;
							}
							if(m == 7){
								ResonanceRatio_ThirdO_SeventhO_SetC[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_SeventhO_SetC[i]<<endl;
								
								OverallResonance_ThirdO_SeventhO_SetC = OverallResonance_ThirdO_SeventhO_SetC + ResonanceRatio_ThirdO_SeventhO_SetC[i];
								NumOfThirdO_SeventhO_SetC++;
							}
							if(m == 8){
								ResonanceRatio_ThirdO_EighthO_SetC[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_EighthO_SetC[i]<<endl;
								
								OverallResonance_ThirdO_EighthO_SetC = OverallResonance_ThirdO_EighthO_SetC + ResonanceRatio_ThirdO_EighthO_SetC[i];
								NumOfThirdO_EighthO_SetC++;
							}
							if(m == 9){
								ResonanceRatio_ThirdO_NinthO_SetC[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_NinthO_SetC[i]<<endl;
								
								OverallResonance_ThirdO_NinthO_SetC = OverallResonance_ThirdO_NinthO_SetC + ResonanceRatio_ThirdO_NinthO_SetC[i];
								NumOfThirdO_NinthO_SetC++;
							}
						}
						//SetD
						if(j == 3){
							if(m == 0){
								ResonanceRatio_ThirdO_FU_SetD[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_FU_SetD[i]<<endl;
								
								OverallResonance_ThirdO_FU_SetD = OverallResonance_ThirdO_FU_SetD + ResonanceRatio_ThirdO_FU_SetD[i];
								NumOfThirdO_FU_SetD++;
							}
							if(m == 1){
								ResonanceRatio_ThirdO_FO_SetD[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_FO_SetD[i]<<endl;
								
								OverallResonance_ThirdO_FO_SetD = OverallResonance_ThirdO_FO_SetD + ResonanceRatio_ThirdO_FO_SetD[i];
								NumOfThirdO_FO_SetD++;
							}
							if(m == 2){
								ResonanceRatio_ThirdO_2O_SetD[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_2O_SetD[i]<<endl;
								
								OverallResonance_ThirdO_2O_SetD = OverallResonance_ThirdO_2O_SetD + ResonanceRatio_ThirdO_2O_SetD[i];
								NumOfThirdO_2O_SetD++;
							}
							if(m == 3){
								ResonanceRatio_ThirdO_ThirdO_SetD[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_ThirdO_SetD[i]<<endl;
								
								OverallResonance_ThirdO_ThirdO_SetD = OverallResonance_ThirdO_ThirdO_SetD + ResonanceRatio_ThirdO_ThirdO_SetD[i];
								NumOfThirdO_ThirdO_SetD++;
							}
							if(m == 4){
								ResonanceRatio_ThirdO_ForthO_SetD[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_ForthO_SetD[i]<<endl;
								
								OverallResonance_ThirdO_ForthO_SetD = OverallResonance_ThirdO_ForthO_SetD + ResonanceRatio_ThirdO_ForthO_SetD[i];
								NumOfThirdO_ForthO_SetD++;
							}
							if(m == 5){
								ResonanceRatio_ThirdO_FifthO_SetD[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_FifthO_SetD[i]<<endl;
								
								OverallResonance_ThirdO_FifthO_SetD = OverallResonance_ThirdO_FifthO_SetD + ResonanceRatio_ThirdO_FifthO_SetD[i];
								NumOfThirdO_FifthO_SetD++;
							}
							if(m == 6){
								ResonanceRatio_ThirdO_SixthO_SetD[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_SixthO_SetD[i]<<endl;
								
								OverallResonance_ThirdO_SixthO_SetD = OverallResonance_ThirdO_SixthO_SetD + ResonanceRatio_ThirdO_SixthO_SetD[i];
								NumOfThirdO_SixthO_SetD++;
							}
							if(m == 7){
								ResonanceRatio_ThirdO_SeventhO_SetD[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_SeventhO_SetD[i]<<endl;
								
								OverallResonance_ThirdO_SeventhO_SetD = OverallResonance_ThirdO_SeventhO_SetD + ResonanceRatio_ThirdO_SeventhO_SetD[i];
								NumOfThirdO_SeventhO_SetD++;
							}
							if(m == 8){
								ResonanceRatio_ThirdO_EighthO_SetD[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_EighthO_SetD[i]<<endl;
								
								OverallResonance_ThirdO_EighthO_SetD = OverallResonance_ThirdO_EighthO_SetD + ResonanceRatio_ThirdO_EighthO_SetD[i];
								NumOfThirdO_EighthO_SetD++;
							}
							if(m == 9){
								ResonanceRatio_ThirdO_NinthO_SetD[i] = period_ThirdO/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_NinthO_SetD[i]<<endl;
								
								OverallResonance_ThirdO_NinthO_SetD = OverallResonance_ThirdO_NinthO_SetD + ResonanceRatio_ThirdO_NinthO_SetD[i];
								NumOfThirdO_NinthO_SetD++;
							}
						}
					}
					//ForthO mode//
					if((growth > 0.0 || !DoPosGrowthCut) && (growth_ForthO > 0)){
						cout<<"Resonance Ratios With ForthO..."<<endl;
						//SetA
						if(j == 0){
							if(m == 0){
								ResonanceRatio_ForthO_FU_SetA[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_FU_SetA[i]<<endl;
								
								OverallResonance_ForthO_FU_SetA = OverallResonance_ForthO_FU_SetA + ResonanceRatio_ForthO_FU_SetA[i];
								NumOfForthO_FU_SetA++;
							}
							if(m == 1){
								ResonanceRatio_ForthO_FO_SetA[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_FO_SetA[i]<<endl;
								
								OverallResonance_ForthO_FO_SetA = OverallResonance_ForthO_FO_SetA + ResonanceRatio_ForthO_FO_SetA[i];
								NumOfForthO_FO_SetA++;
							}
							if(m == 2){
								ResonanceRatio_ForthO_2O_SetA[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_2O_SetA[i]<<endl;
								
								OverallResonance_ForthO_2O_SetA = OverallResonance_ForthO_2O_SetA + ResonanceRatio_ForthO_2O_SetA[i];
								NumOfForthO_2O_SetA++;
							}
							if(m == 3){
								ResonanceRatio_ForthO_ThirdO_SetA[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_ThirdO_SetA[i]<<endl;
								
								OverallResonance_ForthO_ThirdO_SetA = OverallResonance_ForthO_ThirdO_SetA + ResonanceRatio_ForthO_ThirdO_SetA[i];
								NumOfForthO_ThirdO_SetA++;
							}
							if(m == 4){
								ResonanceRatio_ForthO_ForthO_SetA[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_ForthO_SetA[i]<<endl;
								
								OverallResonance_ForthO_ForthO_SetA = OverallResonance_ForthO_ForthO_SetA + ResonanceRatio_ForthO_ForthO_SetA[i];
								NumOfForthO_ForthO_SetA++;
							}
							if(m == 5){
								ResonanceRatio_ForthO_FifthO_SetA[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_FifthO_SetA[i]<<endl;
								
								OverallResonance_ForthO_FifthO_SetA = OverallResonance_ForthO_FifthO_SetA + ResonanceRatio_ForthO_FifthO_SetA[i];
								NumOfForthO_FifthO_SetA++;
							}
							if(m == 6){
								ResonanceRatio_ForthO_SixthO_SetA[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_SixthO_SetA[i]<<endl;
								
								OverallResonance_ForthO_SixthO_SetA = OverallResonance_ForthO_SixthO_SetA + ResonanceRatio_ForthO_SixthO_SetA[i];
								NumOfForthO_SixthO_SetA++;
							}
							if(m == 7){
								ResonanceRatio_ForthO_SeventhO_SetA[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_SeventhO_SetA[i]<<endl;
								
								OverallResonance_ForthO_SeventhO_SetA = OverallResonance_ForthO_SeventhO_SetA + ResonanceRatio_ForthO_SeventhO_SetA[i];
								NumOfForthO_SeventhO_SetA++;
							}
							if(m == 8){
								ResonanceRatio_ForthO_EighthO_SetA[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_EighthO_SetA[i]<<endl;
								
								OverallResonance_ForthO_EighthO_SetA = OverallResonance_ForthO_EighthO_SetA + ResonanceRatio_ForthO_EighthO_SetA[i];
								NumOfForthO_EighthO_SetA++;
							}
							if(m == 9){
								ResonanceRatio_ForthO_NinthO_SetA[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_NinthO_SetA[i]<<endl;
								
								OverallResonance_ForthO_NinthO_SetA = OverallResonance_ForthO_NinthO_SetA + ResonanceRatio_ForthO_NinthO_SetA[i];
								NumOfForthO_NinthO_SetA++;
							}
						}
						//SetB
						if(j == 1){
							if(m == 0){
								ResonanceRatio_ForthO_FU_SetB[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_FU_SetB[i]<<endl;
								
								OverallResonance_ForthO_FU_SetB = OverallResonance_ForthO_FU_SetB + ResonanceRatio_ForthO_FU_SetB[i];
								NumOfForthO_FU_SetB++;
							}
							if(m == 1){
								ResonanceRatio_ForthO_FO_SetB[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_FO_SetB[i]<<endl;
								
								OverallResonance_ForthO_FO_SetB = OverallResonance_ForthO_FO_SetB + ResonanceRatio_ForthO_FO_SetB[i];
								NumOfForthO_FO_SetB++;
							}
							if(m == 2){
								ResonanceRatio_ForthO_2O_SetB[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_2O_SetB[i]<<endl;
								
								OverallResonance_ForthO_2O_SetB = OverallResonance_ForthO_2O_SetB + ResonanceRatio_ForthO_2O_SetB[i];
								NumOfForthO_2O_SetB++;
							}
							if(m == 3){
								ResonanceRatio_ForthO_ThirdO_SetB[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_ThirdO_SetB[i]<<endl;
								
								OverallResonance_ForthO_ThirdO_SetB = OverallResonance_ForthO_ThirdO_SetB + ResonanceRatio_ForthO_ThirdO_SetB[i];
								NumOfForthO_ThirdO_SetB++;
							}
							if(m == 4){
								ResonanceRatio_ForthO_ForthO_SetB[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_ForthO_SetB[i]<<endl;
								
								OverallResonance_ForthO_ForthO_SetB = OverallResonance_ForthO_ForthO_SetB + ResonanceRatio_ForthO_ForthO_SetB[i];
								NumOfForthO_ForthO_SetB++;
							}
							if(m == 5){
								ResonanceRatio_ForthO_FifthO_SetB[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_FifthO_SetB[i]<<endl;
								
								OverallResonance_ForthO_FifthO_SetB = OverallResonance_ForthO_FifthO_SetB + ResonanceRatio_ForthO_FifthO_SetB[i];
								NumOfForthO_FifthO_SetB++;
							}
							if(m == 6){
								ResonanceRatio_ForthO_SixthO_SetB[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_SixthO_SetB[i]<<endl;
								
								OverallResonance_ForthO_SixthO_SetB = OverallResonance_ForthO_SixthO_SetB + ResonanceRatio_ForthO_SixthO_SetB[i];
								NumOfForthO_SixthO_SetB++;
							}
							if(m == 7){
								ResonanceRatio_ForthO_SeventhO_SetB[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_SeventhO_SetB[i]<<endl;
								
								OverallResonance_ForthO_SeventhO_SetB = OverallResonance_ForthO_SeventhO_SetB + ResonanceRatio_ForthO_SeventhO_SetB[i];
								NumOfForthO_SeventhO_SetB++;
							}
							if(m == 8){
								ResonanceRatio_ForthO_EighthO_SetB[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_EighthO_SetB[i]<<endl;
								
								OverallResonance_ForthO_EighthO_SetB = OverallResonance_ForthO_EighthO_SetB + ResonanceRatio_ForthO_EighthO_SetB[i];
								NumOfForthO_EighthO_SetB++;
							}
							if(m == 9){
								ResonanceRatio_ForthO_NinthO_SetB[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_NinthO_SetB[i]<<endl;
								
								OverallResonance_ForthO_NinthO_SetB = OverallResonance_ForthO_NinthO_SetB + ResonanceRatio_ForthO_NinthO_SetB[i];
								NumOfForthO_NinthO_SetB++;
							}
						}
						//SetC
						if(j == 2){
							if(m == 0){
								ResonanceRatio_ForthO_FU_SetC[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_FU_SetC[i]<<endl;
								
								OverallResonance_ForthO_FU_SetC = OverallResonance_ForthO_FU_SetC + ResonanceRatio_ForthO_FU_SetC[i];
								NumOfForthO_FU_SetC++;
							}
							if(m == 1){
								ResonanceRatio_ForthO_FO_SetC[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_FO_SetC[i]<<endl;
								
								OverallResonance_ForthO_FO_SetC = OverallResonance_ForthO_FO_SetC + ResonanceRatio_ForthO_FO_SetC[i];
								NumOfForthO_FO_SetC++;
							}
							if(m == 2){
								ResonanceRatio_ForthO_2O_SetC[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_2O_SetC[i]<<endl;
								
								OverallResonance_ForthO_2O_SetC = OverallResonance_ForthO_2O_SetC + ResonanceRatio_ForthO_2O_SetC[i];
								NumOfForthO_2O_SetC++;
							}
							if(m == 3){
								ResonanceRatio_ForthO_ThirdO_SetC[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_ThirdO_SetC[i]<<endl;
								
								OverallResonance_ForthO_ThirdO_SetC = OverallResonance_ForthO_ThirdO_SetC + ResonanceRatio_ForthO_ThirdO_SetC[i];
								NumOfForthO_ThirdO_SetC++;
							}
							if(m == 4){
								ResonanceRatio_ForthO_ForthO_SetC[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_ForthO_SetC[i]<<endl;
								
								OverallResonance_ForthO_ForthO_SetC = OverallResonance_ForthO_ForthO_SetC + ResonanceRatio_ForthO_ForthO_SetC[i];
								NumOfForthO_ForthO_SetC++;
							}
							if(m == 5){
								ResonanceRatio_ForthO_FifthO_SetC[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_FifthO_SetC[i]<<endl;
								
								OverallResonance_ForthO_FifthO_SetC = OverallResonance_ForthO_FifthO_SetC + ResonanceRatio_ForthO_FifthO_SetC[i];
								NumOfForthO_FifthO_SetC++;
							}
							if(m == 6){
								ResonanceRatio_ForthO_SixthO_SetC[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_SixthO_SetC[i]<<endl;
								
								OverallResonance_ForthO_SixthO_SetC = OverallResonance_ForthO_SixthO_SetC + ResonanceRatio_ForthO_SixthO_SetC[i];
								NumOfForthO_SixthO_SetC++;
							}
							if(m == 7){
								ResonanceRatio_ForthO_SeventhO_SetC[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_SeventhO_SetC[i]<<endl;
								
								OverallResonance_ForthO_SeventhO_SetC = OverallResonance_ForthO_SeventhO_SetC + ResonanceRatio_ForthO_SeventhO_SetC[i];
								NumOfForthO_SeventhO_SetC++;
							}
							if(m == 8){
								ResonanceRatio_ForthO_EighthO_SetC[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_EighthO_SetC[i]<<endl;
								
								OverallResonance_ForthO_EighthO_SetC = OverallResonance_ForthO_EighthO_SetC + ResonanceRatio_ForthO_EighthO_SetC[i];
								NumOfForthO_EighthO_SetC++;
							}
							if(m == 9){
								ResonanceRatio_ForthO_NinthO_SetC[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_NinthO_SetC[i]<<endl;
								
								OverallResonance_ForthO_NinthO_SetC = OverallResonance_ForthO_NinthO_SetC + ResonanceRatio_ForthO_NinthO_SetC[i];
								NumOfForthO_NinthO_SetC++;
							}
						}
						//SetD
						if(j == 3){
							if(m == 0){
								ResonanceRatio_ForthO_FU_SetD[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_FU_SetD[i]<<endl;
								
								OverallResonance_ForthO_FU_SetD = OverallResonance_ForthO_FU_SetD + ResonanceRatio_ForthO_FU_SetD[i];
								NumOfForthO_FU_SetD++;
							}
							if(m == 1){
								ResonanceRatio_ForthO_FO_SetD[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_FO_SetD[i]<<endl;
								
								OverallResonance_ForthO_FO_SetD = OverallResonance_ForthO_FO_SetD + ResonanceRatio_ForthO_FO_SetD[i];
								NumOfForthO_FO_SetD++;
							}
							if(m == 2){
								ResonanceRatio_ForthO_2O_SetD[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_2O_SetD[i]<<endl;
								
								OverallResonance_ForthO_2O_SetD = OverallResonance_ForthO_2O_SetD + ResonanceRatio_ForthO_2O_SetD[i];
								NumOfForthO_2O_SetD++;
							}
							if(m == 3){
								ResonanceRatio_ForthO_ThirdO_SetD[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_ThirdO_SetD[i]<<endl;
								
								OverallResonance_ForthO_ThirdO_SetD = OverallResonance_ForthO_ThirdO_SetD + ResonanceRatio_ForthO_ThirdO_SetD[i];
								NumOfForthO_ThirdO_SetD++;
							}
							if(m == 4){
								ResonanceRatio_ForthO_ForthO_SetD[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_ForthO_SetD[i]<<endl;
								
								OverallResonance_ForthO_ForthO_SetD = OverallResonance_ForthO_ForthO_SetD + ResonanceRatio_ForthO_ForthO_SetD[i];
								NumOfForthO_ForthO_SetD++;
							}
							if(m == 5){
								ResonanceRatio_ForthO_FifthO_SetD[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_FifthO_SetD[i]<<endl;
								
								OverallResonance_ForthO_FifthO_SetD = OverallResonance_ForthO_FifthO_SetD + ResonanceRatio_ForthO_FifthO_SetD[i];
								NumOfForthO_FifthO_SetD++;
							}
							if(m == 6){
								ResonanceRatio_ForthO_SixthO_SetD[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_SixthO_SetD[i]<<endl;
								
								OverallResonance_ForthO_SixthO_SetD = OverallResonance_ForthO_SixthO_SetD + ResonanceRatio_ForthO_SixthO_SetD[i];
								NumOfForthO_SixthO_SetD++;
							}
							if(m == 7){
								ResonanceRatio_ForthO_SeventhO_SetD[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_SeventhO_SetD[i]<<endl;
								
								OverallResonance_ForthO_SeventhO_SetD = OverallResonance_ForthO_SeventhO_SetD + ResonanceRatio_ForthO_SeventhO_SetD[i];
								NumOfForthO_SeventhO_SetD++;
							}
							if(m == 8){
								ResonanceRatio_ForthO_EighthO_SetD[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_EighthO_SetD[i]<<endl;
								
								OverallResonance_ForthO_EighthO_SetD = OverallResonance_ForthO_EighthO_SetD + ResonanceRatio_ForthO_EighthO_SetD[i];
								NumOfForthO_EighthO_SetD++;
							}
							if(m == 9){
								ResonanceRatio_ForthO_NinthO_SetD[i] = period_ForthO/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_NinthO_SetD[i]<<endl;
								
								OverallResonance_ForthO_NinthO_SetD = OverallResonance_ForthO_NinthO_SetD + ResonanceRatio_ForthO_NinthO_SetD[i];
								NumOfForthO_NinthO_SetD++;
							}
						}
					}
					//FifthO mode//
					if((growth > 0.0 || !DoPosGrowthCut) && (growth_FifthO > 0)){
						cout<<"Resonance Ratios With FifthO..."<<endl;
						//SetA
						if(j == 0){
							if(m == 0){
								ResonanceRatio_FifthO_FU_SetA[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_FU_SetA[i]<<endl;
								
								OverallResonance_FifthO_FU_SetA = OverallResonance_FifthO_FU_SetA + ResonanceRatio_FifthO_FU_SetA[i];
								NumOfFifthO_FU_SetA++;
							}
							if(m == 1){
								ResonanceRatio_FifthO_FO_SetA[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_FO_SetA[i]<<endl;
								
								OverallResonance_FifthO_FO_SetA = OverallResonance_FifthO_FO_SetA + ResonanceRatio_FifthO_FO_SetA[i];
								NumOfFifthO_FO_SetA++;
							}
							if(m == 2){
								ResonanceRatio_FifthO_2O_SetA[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_2O_SetA[i]<<endl;
								
								OverallResonance_FifthO_2O_SetA = OverallResonance_FifthO_2O_SetA + ResonanceRatio_FifthO_2O_SetA[i];
								NumOfFifthO_2O_SetA++;
							}
							if(m == 3){
								ResonanceRatio_FifthO_ThirdO_SetA[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_ThirdO_SetA[i]<<endl;
								
								OverallResonance_FifthO_ThirdO_SetA = OverallResonance_FifthO_ThirdO_SetA + ResonanceRatio_FifthO_ThirdO_SetA[i];
								NumOfFifthO_ThirdO_SetA++;
							}
							if(m == 4){
								ResonanceRatio_FifthO_ForthO_SetA[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_ForthO_SetA[i]<<endl;
								
								OverallResonance_FifthO_ForthO_SetA = OverallResonance_FifthO_ForthO_SetA + ResonanceRatio_FifthO_ForthO_SetA[i];
								NumOfFifthO_ForthO_SetA++;
							}
							if(m == 5){
								ResonanceRatio_FifthO_FifthO_SetA[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_FifthO_SetA[i]<<endl;
								
								OverallResonance_FifthO_FifthO_SetA = OverallResonance_FifthO_FifthO_SetA + ResonanceRatio_FifthO_FifthO_SetA[i];
								NumOfFifthO_FifthO_SetA++;
							}
							if(m == 6){
								ResonanceRatio_FifthO_SixthO_SetA[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_SixthO_SetA[i]<<endl;
								
								OverallResonance_FifthO_SixthO_SetA = OverallResonance_FifthO_SixthO_SetA + ResonanceRatio_FifthO_SixthO_SetA[i];
								NumOfFifthO_SixthO_SetA++;
							}
							if(m == 7){
								ResonanceRatio_FifthO_SeventhO_SetA[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_SeventhO_SetA[i]<<endl;
								
								OverallResonance_FifthO_SeventhO_SetA = OverallResonance_FifthO_SeventhO_SetA + ResonanceRatio_FifthO_SeventhO_SetA[i];
								NumOfFifthO_SeventhO_SetA++;
							}
							if(m == 8){
								ResonanceRatio_FifthO_EighthO_SetA[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_EighthO_SetA[i]<<endl;
								
								OverallResonance_FifthO_EighthO_SetA = OverallResonance_FifthO_EighthO_SetA + ResonanceRatio_FifthO_EighthO_SetA[i];
								NumOfFifthO_EighthO_SetA++;
							}
							if(m == 9){
								ResonanceRatio_FifthO_NinthO_SetA[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_NinthO_SetA[i]<<endl;
								
								OverallResonance_FifthO_NinthO_SetA = OverallResonance_FifthO_NinthO_SetA + ResonanceRatio_FifthO_NinthO_SetA[i];
								NumOfFifthO_NinthO_SetA++;
							}
						}
						//SetB
						if(j == 1){
							if(m == 0){
								ResonanceRatio_FifthO_FU_SetB[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_FU_SetB[i]<<endl;
								
								OverallResonance_FifthO_FU_SetB = OverallResonance_FifthO_FU_SetB + ResonanceRatio_FifthO_FU_SetB[i];
								NumOfFifthO_FU_SetB++;
							}
							if(m == 1){
								ResonanceRatio_FifthO_FO_SetB[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_FO_SetB[i]<<endl;
								
								OverallResonance_FifthO_FO_SetB = OverallResonance_FifthO_FO_SetB + ResonanceRatio_FifthO_FO_SetB[i];
								NumOfFifthO_FO_SetB++;
							}
							if(m == 2){
								ResonanceRatio_FifthO_2O_SetB[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_2O_SetB[i]<<endl;
								
								OverallResonance_FifthO_2O_SetB = OverallResonance_FifthO_2O_SetB + ResonanceRatio_FifthO_2O_SetB[i];
								NumOfFifthO_2O_SetB++;
							}
							if(m == 3){
								ResonanceRatio_FifthO_ThirdO_SetB[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_ThirdO_SetB[i]<<endl;
								
								OverallResonance_FifthO_ThirdO_SetB = OverallResonance_FifthO_ThirdO_SetB + ResonanceRatio_FifthO_ThirdO_SetB[i];
								NumOfFifthO_ThirdO_SetB++;
							}
							if(m == 4){
								ResonanceRatio_FifthO_ForthO_SetB[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_ForthO_SetB[i]<<endl;
								
								OverallResonance_FifthO_ForthO_SetB = OverallResonance_FifthO_ForthO_SetB + ResonanceRatio_FifthO_ForthO_SetB[i];
								NumOfFifthO_ForthO_SetB++;
							}
							if(m == 5){
								ResonanceRatio_FifthO_FifthO_SetB[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_FifthO_SetB[i]<<endl;
								
								OverallResonance_FifthO_FifthO_SetB = OverallResonance_FifthO_FifthO_SetB + ResonanceRatio_FifthO_FifthO_SetB[i];
								NumOfFifthO_FifthO_SetB++;
							}
							if(m == 6){
								ResonanceRatio_FifthO_SixthO_SetB[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_SixthO_SetB[i]<<endl;
								
								OverallResonance_FifthO_SixthO_SetB = OverallResonance_FifthO_SixthO_SetB + ResonanceRatio_FifthO_SixthO_SetB[i];
								NumOfFifthO_SixthO_SetB++;
							}
							if(m == 7){
								ResonanceRatio_FifthO_SeventhO_SetB[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_SeventhO_SetB[i]<<endl;
								
								OverallResonance_FifthO_SeventhO_SetB = OverallResonance_FifthO_SeventhO_SetB + ResonanceRatio_FifthO_SeventhO_SetB[i];
								NumOfFifthO_SeventhO_SetB++;
							}
							if(m == 8){
								ResonanceRatio_FifthO_EighthO_SetB[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_EighthO_SetB[i]<<endl;
								
								OverallResonance_FifthO_EighthO_SetB = OverallResonance_FifthO_EighthO_SetB + ResonanceRatio_FifthO_EighthO_SetB[i];
								NumOfFifthO_EighthO_SetB++;
							}
							if(m == 9){
								ResonanceRatio_FifthO_NinthO_SetB[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_NinthO_SetB[i]<<endl;
								
								OverallResonance_FifthO_NinthO_SetB = OverallResonance_FifthO_NinthO_SetB + ResonanceRatio_FifthO_NinthO_SetB[i];
								NumOfFifthO_NinthO_SetB++;
							}
						}
						//SetC
						if(j == 2){
							if(m == 0){
								ResonanceRatio_FifthO_FU_SetC[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_FU_SetC[i]<<endl;
								
								OverallResonance_FifthO_FU_SetC = OverallResonance_FifthO_FU_SetC + ResonanceRatio_FifthO_FU_SetC[i];
								NumOfFifthO_FU_SetC++;
							}
							if(m == 1){
								ResonanceRatio_FifthO_FO_SetC[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_FO_SetC[i]<<endl;
								
								OverallResonance_FifthO_FO_SetC = OverallResonance_FifthO_FO_SetC + ResonanceRatio_FifthO_FO_SetC[i];
								NumOfFifthO_FO_SetC++;
							}
							if(m == 2){
								ResonanceRatio_FifthO_2O_SetC[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_2O_SetC[i]<<endl;
								
								OverallResonance_FifthO_2O_SetC = OverallResonance_FifthO_2O_SetC + ResonanceRatio_FifthO_2O_SetC[i];
								NumOfFifthO_2O_SetC++;
							}
							if(m == 3){
								ResonanceRatio_FifthO_ThirdO_SetC[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_ThirdO_SetC[i]<<endl;
								
								OverallResonance_FifthO_ThirdO_SetC = OverallResonance_FifthO_ThirdO_SetC + ResonanceRatio_FifthO_ThirdO_SetC[i];
								NumOfFifthO_ThirdO_SetC++;
							}
							if(m == 4){
								ResonanceRatio_FifthO_ForthO_SetC[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_ForthO_SetC[i]<<endl;
								
								OverallResonance_FifthO_ForthO_SetC = OverallResonance_FifthO_ForthO_SetC + ResonanceRatio_FifthO_ForthO_SetC[i];
								NumOfFifthO_ForthO_SetC++;
							}
							if(m == 5){
								ResonanceRatio_FifthO_FifthO_SetC[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_FifthO_SetC[i]<<endl;
								
								OverallResonance_FifthO_FifthO_SetC = OverallResonance_FifthO_FifthO_SetC + ResonanceRatio_FifthO_FifthO_SetC[i];
								NumOfFifthO_FifthO_SetC++;
							}
							if(m == 6){
								ResonanceRatio_FifthO_SixthO_SetC[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_SixthO_SetC[i]<<endl;
								
								OverallResonance_FifthO_SixthO_SetC = OverallResonance_FifthO_SixthO_SetC + ResonanceRatio_FifthO_SixthO_SetC[i];
								NumOfFifthO_SixthO_SetC++;
							}
							if(m == 7){
								ResonanceRatio_FifthO_SeventhO_SetC[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_SeventhO_SetC[i]<<endl;
								
								OverallResonance_FifthO_SeventhO_SetC = OverallResonance_FifthO_SeventhO_SetC + ResonanceRatio_FifthO_SeventhO_SetC[i];
								NumOfFifthO_SeventhO_SetC++;
							}
							if(m == 8){
								ResonanceRatio_FifthO_EighthO_SetC[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_EighthO_SetC[i]<<endl;
								
								OverallResonance_FifthO_EighthO_SetC = OverallResonance_FifthO_EighthO_SetC + ResonanceRatio_FifthO_EighthO_SetC[i];
								NumOfFifthO_EighthO_SetC++;
							}
							if(m == 9){
								ResonanceRatio_FifthO_NinthO_SetC[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_NinthO_SetC[i]<<endl;
								
								OverallResonance_FifthO_NinthO_SetC = OverallResonance_FifthO_NinthO_SetC + ResonanceRatio_FifthO_NinthO_SetC[i];
								NumOfFifthO_NinthO_SetC++;
							}
						}
						//SetD
						if(j == 3){
							if(m == 0){
								ResonanceRatio_FifthO_FU_SetD[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_FU_SetD[i]<<endl;
								
								OverallResonance_FifthO_FU_SetD = OverallResonance_FifthO_FU_SetD + ResonanceRatio_FifthO_FU_SetD[i];
								NumOfFifthO_FU_SetD++;
							}
							if(m == 1){
								ResonanceRatio_FifthO_FO_SetD[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_FO_SetD[i]<<endl;
								
								OverallResonance_FifthO_FO_SetD = OverallResonance_FifthO_FO_SetD + ResonanceRatio_FifthO_FO_SetD[i];
								NumOfFifthO_FO_SetD++;
							}
							if(m == 2){
								ResonanceRatio_FifthO_2O_SetD[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_2O_SetD[i]<<endl;
								
								OverallResonance_FifthO_2O_SetD = OverallResonance_FifthO_2O_SetD + ResonanceRatio_FifthO_2O_SetD[i];
								NumOfFifthO_2O_SetD++;
							}
							if(m == 3){
								ResonanceRatio_FifthO_ThirdO_SetD[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_ThirdO_SetD[i]<<endl;
								
								OverallResonance_FifthO_ThirdO_SetD = OverallResonance_FifthO_ThirdO_SetD + ResonanceRatio_FifthO_ThirdO_SetD[i];
								NumOfFifthO_ThirdO_SetD++;
							}
							if(m == 4){
								ResonanceRatio_FifthO_ForthO_SetD[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_ForthO_SetD[i]<<endl;
								
								OverallResonance_FifthO_ForthO_SetD = OverallResonance_FifthO_ForthO_SetD + ResonanceRatio_FifthO_ForthO_SetD[i];
								NumOfFifthO_ForthO_SetD++;
							}
							if(m == 5){
								ResonanceRatio_FifthO_FifthO_SetD[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_FifthO_SetD[i]<<endl;
								
								OverallResonance_FifthO_FifthO_SetD = OverallResonance_FifthO_FifthO_SetD + ResonanceRatio_FifthO_FifthO_SetD[i];
								NumOfFifthO_FifthO_SetD++;
							}
							if(m == 6){
								ResonanceRatio_FifthO_SixthO_SetD[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_SixthO_SetD[i]<<endl;
								
								OverallResonance_FifthO_SixthO_SetD = OverallResonance_FifthO_SixthO_SetD + ResonanceRatio_FifthO_SixthO_SetD[i];
								NumOfFifthO_SixthO_SetD++;
							}
							if(m == 7){
								ResonanceRatio_FifthO_SeventhO_SetD[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_SeventhO_SetD[i]<<endl;
								
								OverallResonance_FifthO_SeventhO_SetD = OverallResonance_FifthO_SeventhO_SetD + ResonanceRatio_FifthO_SeventhO_SetD[i];
								NumOfFifthO_SeventhO_SetD++;
							}
							if(m == 8){
								ResonanceRatio_FifthO_EighthO_SetD[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_EighthO_SetD[i]<<endl;
								
								OverallResonance_FifthO_EighthO_SetD = OverallResonance_FifthO_EighthO_SetD + ResonanceRatio_FifthO_EighthO_SetD[i];
								NumOfFifthO_EighthO_SetD++;
							}
							if(m == 9){
								ResonanceRatio_FifthO_NinthO_SetD[i] = period_FifthO/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_NinthO_SetD[i]<<endl;
								
								OverallResonance_FifthO_NinthO_SetD = OverallResonance_FifthO_NinthO_SetD + ResonanceRatio_FifthO_NinthO_SetD[i];
								NumOfFifthO_NinthO_SetD++;
							}
						}
					}
					//SixthO mode//
					if((growth > 0.0 || !DoPosGrowthCut) && (growth_SixthO > 0)){
						cout<<"Resonance Ratios With SixthO..."<<endl;
						//SetA
						if(j == 0){
							if(m == 0){
								ResonanceRatio_SixthO_FU_SetA[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_FU_SetA[i]<<endl;
								
								OverallResonance_SixthO_FU_SetA = OverallResonance_SixthO_FU_SetA + ResonanceRatio_SixthO_FU_SetA[i];
								NumOfSixthO_FU_SetA++;
							}
							if(m == 1){
								ResonanceRatio_SixthO_FO_SetA[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_FO_SetA[i]<<endl;
								
								OverallResonance_SixthO_FO_SetA = OverallResonance_SixthO_FO_SetA + ResonanceRatio_SixthO_FO_SetA[i];
								NumOfSixthO_FO_SetA++;
							}
							if(m == 2){
								ResonanceRatio_SixthO_2O_SetA[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_2O_SetA[i]<<endl;
								
								OverallResonance_SixthO_2O_SetA = OverallResonance_SixthO_2O_SetA + ResonanceRatio_SixthO_2O_SetA[i];
								NumOfSixthO_2O_SetA++;
							}
							if(m == 3){
								ResonanceRatio_SixthO_ThirdO_SetA[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_ThirdO_SetA[i]<<endl;
								
								OverallResonance_SixthO_ThirdO_SetA = OverallResonance_SixthO_ThirdO_SetA + ResonanceRatio_SixthO_ThirdO_SetA[i];
								NumOfSixthO_ThirdO_SetA++;
							}
							if(m == 4){
								ResonanceRatio_SixthO_ForthO_SetA[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_ForthO_SetA[i]<<endl;
								
								OverallResonance_SixthO_ForthO_SetA = OverallResonance_SixthO_ForthO_SetA + ResonanceRatio_SixthO_ForthO_SetA[i];
								NumOfSixthO_ForthO_SetA++;
							}
							if(m == 5){
								ResonanceRatio_SixthO_FifthO_SetA[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_FifthO_SetA[i]<<endl;
								
								OverallResonance_SixthO_FifthO_SetA = OverallResonance_SixthO_FifthO_SetA + ResonanceRatio_SixthO_FifthO_SetA[i];
								NumOfSixthO_FifthO_SetA++;
							}
							if(m == 6){
								ResonanceRatio_SixthO_SixthO_SetA[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_SixthO_SetA[i]<<endl;
								
								OverallResonance_SixthO_SixthO_SetA = OverallResonance_SixthO_SixthO_SetA + ResonanceRatio_SixthO_SixthO_SetA[i];
								NumOfSixthO_SixthO_SetA++;
							}
							if(m == 7){
								ResonanceRatio_SixthO_SeventhO_SetA[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_SeventhO_SetA[i]<<endl;
								
								OverallResonance_SixthO_SeventhO_SetA = OverallResonance_SixthO_SeventhO_SetA + ResonanceRatio_SixthO_SeventhO_SetA[i];
								NumOfSixthO_SeventhO_SetA++;
							}
							if(m == 8){
								ResonanceRatio_SixthO_EighthO_SetA[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_EighthO_SetA[i]<<endl;
								
								OverallResonance_SixthO_EighthO_SetA = OverallResonance_SixthO_EighthO_SetA + ResonanceRatio_SixthO_EighthO_SetA[i];
								NumOfSixthO_EighthO_SetA++;
							}
							if(m == 9){
								ResonanceRatio_SixthO_NinthO_SetA[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_NinthO_SetA[i]<<endl;
								
								OverallResonance_SixthO_NinthO_SetA = OverallResonance_SixthO_NinthO_SetA + ResonanceRatio_SixthO_NinthO_SetA[i];
								NumOfSixthO_NinthO_SetA++;
							}
						}
						//SetB
						if(j == 1){
							if(m == 0){
								ResonanceRatio_SixthO_FU_SetB[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_FU_SetB[i]<<endl;
								
								OverallResonance_SixthO_FU_SetB = OverallResonance_SixthO_FU_SetB + ResonanceRatio_SixthO_FU_SetB[i];
								NumOfSixthO_FU_SetB++;
							}
							if(m == 1){
								ResonanceRatio_SixthO_FO_SetB[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_FO_SetB[i]<<endl;
								
								OverallResonance_SixthO_FO_SetB = OverallResonance_SixthO_FO_SetB + ResonanceRatio_SixthO_FO_SetB[i];
								NumOfSixthO_FO_SetB++;
							}
							if(m == 2){
								ResonanceRatio_SixthO_2O_SetB[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_2O_SetB[i]<<endl;
								
								OverallResonance_SixthO_2O_SetB = OverallResonance_SixthO_2O_SetB + ResonanceRatio_SixthO_2O_SetB[i];
								NumOfSixthO_2O_SetB++;
							}
							if(m == 3){
								ResonanceRatio_SixthO_ThirdO_SetB[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_ThirdO_SetB[i]<<endl;
								
								OverallResonance_SixthO_ThirdO_SetB = OverallResonance_SixthO_ThirdO_SetB + ResonanceRatio_SixthO_ThirdO_SetB[i];
								NumOfSixthO_ThirdO_SetB++;
							}
							if(m == 4){
								ResonanceRatio_SixthO_ForthO_SetB[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_ForthO_SetB[i]<<endl;
								
								OverallResonance_SixthO_ForthO_SetB = OverallResonance_SixthO_ForthO_SetB + ResonanceRatio_SixthO_ForthO_SetB[i];
								NumOfSixthO_ForthO_SetB++;
							}
							if(m == 5){
								ResonanceRatio_SixthO_FifthO_SetB[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_FifthO_SetB[i]<<endl;
								
								OverallResonance_SixthO_FifthO_SetB = OverallResonance_SixthO_FifthO_SetB + ResonanceRatio_SixthO_FifthO_SetB[i];
								NumOfSixthO_FifthO_SetB++;
							}
							if(m == 6){
								ResonanceRatio_SixthO_SixthO_SetB[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_SixthO_SetB[i]<<endl;
								
								OverallResonance_SixthO_SixthO_SetB = OverallResonance_SixthO_SixthO_SetB + ResonanceRatio_SixthO_SixthO_SetB[i];
								NumOfSixthO_SixthO_SetB++;
							}
							if(m == 7){
								ResonanceRatio_SixthO_SeventhO_SetB[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_SeventhO_SetB[i]<<endl;
								
								OverallResonance_SixthO_SeventhO_SetB = OverallResonance_SixthO_SeventhO_SetB + ResonanceRatio_SixthO_SeventhO_SetB[i];
								NumOfSixthO_SeventhO_SetB++;
							}
							if(m == 8){
								ResonanceRatio_SixthO_EighthO_SetB[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_EighthO_SetB[i]<<endl;
								
								OverallResonance_SixthO_EighthO_SetB = OverallResonance_SixthO_EighthO_SetB + ResonanceRatio_SixthO_EighthO_SetB[i];
								NumOfSixthO_EighthO_SetB++;
							}
							if(m == 9){
								ResonanceRatio_SixthO_NinthO_SetB[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_NinthO_SetB[i]<<endl;
								
								OverallResonance_SixthO_NinthO_SetB = OverallResonance_SixthO_NinthO_SetB + ResonanceRatio_SixthO_NinthO_SetB[i];
								NumOfSixthO_NinthO_SetB++;
							}
						}
						//SetC
						if(j == 2){
							if(m == 0){
								ResonanceRatio_SixthO_FU_SetC[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_FU_SetC[i]<<endl;
								
								OverallResonance_SixthO_FU_SetC = OverallResonance_SixthO_FU_SetC + ResonanceRatio_SixthO_FU_SetC[i];
								NumOfSixthO_FU_SetC++;
							}
							if(m == 1){
								ResonanceRatio_SixthO_FO_SetC[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_FO_SetC[i]<<endl;
								
								OverallResonance_SixthO_FO_SetC = OverallResonance_SixthO_FO_SetC + ResonanceRatio_SixthO_FO_SetC[i];
								NumOfSixthO_FO_SetC++;
							}
							if(m == 2){
								ResonanceRatio_SixthO_2O_SetC[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_2O_SetC[i]<<endl;
								
								OverallResonance_SixthO_2O_SetC = OverallResonance_SixthO_2O_SetC + ResonanceRatio_SixthO_2O_SetC[i];
								NumOfSixthO_2O_SetC++;
							}
							if(m == 3){
								ResonanceRatio_SixthO_ThirdO_SetC[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_ThirdO_SetC[i]<<endl;
								
								OverallResonance_SixthO_ThirdO_SetC = OverallResonance_SixthO_ThirdO_SetC + ResonanceRatio_SixthO_ThirdO_SetC[i];
								NumOfSixthO_ThirdO_SetC++;
							}
							if(m == 4){
								ResonanceRatio_SixthO_ForthO_SetC[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_ForthO_SetC[i]<<endl;
								
								OverallResonance_SixthO_ForthO_SetC = OverallResonance_SixthO_ForthO_SetC + ResonanceRatio_SixthO_ForthO_SetC[i];
								NumOfSixthO_ForthO_SetC++;
							}
							if(m == 5){
								ResonanceRatio_SixthO_FifthO_SetC[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_FifthO_SetC[i]<<endl;
								
								OverallResonance_SixthO_FifthO_SetC = OverallResonance_SixthO_FifthO_SetC + ResonanceRatio_SixthO_FifthO_SetC[i];
								NumOfSixthO_FifthO_SetC++;
							}
							if(m == 6){
								ResonanceRatio_SixthO_SixthO_SetC[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_SixthO_SetC[i]<<endl;
								
								OverallResonance_SixthO_SixthO_SetC = OverallResonance_SixthO_SixthO_SetC + ResonanceRatio_SixthO_SixthO_SetC[i];
								NumOfSixthO_SixthO_SetC++;
							}
							if(m == 7){
								ResonanceRatio_SixthO_SeventhO_SetC[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_SeventhO_SetC[i]<<endl;
								
								OverallResonance_SixthO_SeventhO_SetC = OverallResonance_SixthO_SeventhO_SetC + ResonanceRatio_SixthO_SeventhO_SetC[i];
								NumOfSixthO_SeventhO_SetC++;
							}
							if(m == 8){
								ResonanceRatio_SixthO_EighthO_SetC[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_EighthO_SetC[i]<<endl;
								
								OverallResonance_SixthO_EighthO_SetC = OverallResonance_SixthO_EighthO_SetC + ResonanceRatio_SixthO_EighthO_SetC[i];
								NumOfSixthO_EighthO_SetC++;
							}
							if(m == 9){
								ResonanceRatio_SixthO_NinthO_SetC[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_NinthO_SetC[i]<<endl;
								
								OverallResonance_SixthO_NinthO_SetC = OverallResonance_SixthO_NinthO_SetC + ResonanceRatio_SixthO_NinthO_SetC[i];
								NumOfSixthO_NinthO_SetC++;
							}
						}
						//SetD
						if(j == 3){
							if(m == 0){
								ResonanceRatio_SixthO_FU_SetD[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_FU_SetD[i]<<endl;
								
								OverallResonance_SixthO_FU_SetD = OverallResonance_SixthO_FU_SetD + ResonanceRatio_SixthO_FU_SetD[i];
								NumOfSixthO_FU_SetD++;
							}
							if(m == 1){
								ResonanceRatio_SixthO_FO_SetD[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_FO_SetD[i]<<endl;
								
								OverallResonance_SixthO_FO_SetD = OverallResonance_SixthO_FO_SetD + ResonanceRatio_SixthO_FO_SetD[i];
								NumOfSixthO_FO_SetD++;
							}
							if(m == 2){
								ResonanceRatio_SixthO_2O_SetD[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_2O_SetD[i]<<endl;
								
								OverallResonance_SixthO_2O_SetD = OverallResonance_SixthO_2O_SetD + ResonanceRatio_SixthO_2O_SetD[i];
								NumOfSixthO_2O_SetD++;
							}
							if(m == 3){
								ResonanceRatio_SixthO_ThirdO_SetD[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_ThirdO_SetD[i]<<endl;
								
								OverallResonance_SixthO_ThirdO_SetD = OverallResonance_SixthO_ThirdO_SetD + ResonanceRatio_SixthO_ThirdO_SetD[i];
								NumOfSixthO_ThirdO_SetD++;
							}
							if(m == 4){
								ResonanceRatio_SixthO_ForthO_SetD[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_ForthO_SetD[i]<<endl;
								
								OverallResonance_SixthO_ForthO_SetD = OverallResonance_SixthO_ForthO_SetD + ResonanceRatio_SixthO_ForthO_SetD[i];
								NumOfSixthO_ForthO_SetD++;
							}
							if(m == 5){
								ResonanceRatio_SixthO_FifthO_SetD[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_FifthO_SetD[i]<<endl;
								
								OverallResonance_SixthO_FifthO_SetD = OverallResonance_SixthO_FifthO_SetD + ResonanceRatio_SixthO_FifthO_SetD[i];
								NumOfSixthO_FifthO_SetD++;
							}
							if(m == 6){
								ResonanceRatio_SixthO_SixthO_SetD[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_SixthO_SetD[i]<<endl;
								
								OverallResonance_SixthO_SixthO_SetD = OverallResonance_SixthO_SixthO_SetD + ResonanceRatio_SixthO_SixthO_SetD[i];
								NumOfSixthO_SixthO_SetD++;
							}
							if(m == 7){
								ResonanceRatio_SixthO_SeventhO_SetD[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_SeventhO_SetD[i]<<endl;
								
								OverallResonance_SixthO_SeventhO_SetD = OverallResonance_SixthO_SeventhO_SetD + ResonanceRatio_SixthO_SeventhO_SetD[i];
								NumOfSixthO_SeventhO_SetD++;
							}
							if(m == 8){
								ResonanceRatio_SixthO_EighthO_SetD[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_EighthO_SetD[i]<<endl;
								
								OverallResonance_SixthO_EighthO_SetD = OverallResonance_SixthO_EighthO_SetD + ResonanceRatio_SixthO_EighthO_SetD[i];
								NumOfSixthO_EighthO_SetD++;
							}
							if(m == 9){
								ResonanceRatio_SixthO_NinthO_SetD[i] = period_SixthO/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_NinthO_SetD[i]<<endl;
								
								OverallResonance_SixthO_NinthO_SetD = OverallResonance_SixthO_NinthO_SetD + ResonanceRatio_SixthO_NinthO_SetD[i];
								NumOfSixthO_NinthO_SetD++;
							}
						}
					}
					//SeventhO mode//
					if((growth > 0.0 || !DoPosGrowthCut) && (growth_SeventhO > 0)){
						cout<<"Resonance Ratios With SeventhO..."<<endl;
						//SetA
						if(j == 0){
							if(m == 0){
								ResonanceRatio_SeventhO_FU_SetA[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_FU_SetA[i]<<endl;
								
								OverallResonance_SeventhO_FU_SetA = OverallResonance_SeventhO_FU_SetA + ResonanceRatio_SeventhO_FU_SetA[i];
								NumOfSeventhO_FU_SetA++;
							}
							if(m == 1){
								ResonanceRatio_SeventhO_FO_SetA[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_FO_SetA[i]<<endl;
								
								OverallResonance_SeventhO_FO_SetA = OverallResonance_SeventhO_FO_SetA + ResonanceRatio_SeventhO_FO_SetA[i];
								NumOfSeventhO_FO_SetA++;
							}
							if(m == 2){
								ResonanceRatio_SeventhO_2O_SetA[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_2O_SetA[i]<<endl;
								
								OverallResonance_SeventhO_2O_SetA = OverallResonance_SeventhO_2O_SetA + ResonanceRatio_SeventhO_2O_SetA[i];
								NumOfSeventhO_2O_SetA++;
							}
							if(m == 3){
								ResonanceRatio_SeventhO_ThirdO_SetA[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_ThirdO_SetA[i]<<endl;
								
								OverallResonance_SeventhO_ThirdO_SetA = OverallResonance_SeventhO_ThirdO_SetA + ResonanceRatio_SeventhO_ThirdO_SetA[i];
								NumOfSeventhO_ThirdO_SetA++;
							}
							if(m == 4){
								ResonanceRatio_SeventhO_ForthO_SetA[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_ForthO_SetA[i]<<endl;
								
								OverallResonance_SeventhO_ForthO_SetA = OverallResonance_SeventhO_ForthO_SetA + ResonanceRatio_SeventhO_ForthO_SetA[i];
								NumOfSeventhO_ForthO_SetA++;
							}
							if(m == 5){
								ResonanceRatio_SeventhO_FifthO_SetA[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_FifthO_SetA[i]<<endl;
								
								OverallResonance_SeventhO_FifthO_SetA = OverallResonance_SeventhO_FifthO_SetA + ResonanceRatio_SeventhO_FifthO_SetA[i];
								NumOfSeventhO_FifthO_SetA++;
							}
							if(m == 6){
								ResonanceRatio_SeventhO_SixthO_SetA[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_SixthO_SetA[i]<<endl;
								
								OverallResonance_SeventhO_SixthO_SetA = OverallResonance_SeventhO_SixthO_SetA + ResonanceRatio_SeventhO_SixthO_SetA[i];
								NumOfSeventhO_SixthO_SetA++;
							}
							if(m == 7){
								ResonanceRatio_SeventhO_SeventhO_SetA[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_SeventhO_SetA[i]<<endl;
								
								OverallResonance_SeventhO_SeventhO_SetA = OverallResonance_SeventhO_SeventhO_SetA + ResonanceRatio_SeventhO_SeventhO_SetA[i];
								NumOfSeventhO_SeventhO_SetA++;
							}
							if(m == 8){
								ResonanceRatio_SeventhO_EighthO_SetA[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_EighthO_SetA[i]<<endl;
								
								OverallResonance_SeventhO_EighthO_SetA = OverallResonance_SeventhO_EighthO_SetA + ResonanceRatio_SeventhO_EighthO_SetA[i];
								NumOfSeventhO_EighthO_SetA++;
							}
							if(m == 9){
								ResonanceRatio_SeventhO_NinthO_SetA[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_NinthO_SetA[i]<<endl;
								
								OverallResonance_SeventhO_NinthO_SetA = OverallResonance_SeventhO_NinthO_SetA + ResonanceRatio_SeventhO_NinthO_SetA[i];
								NumOfSeventhO_NinthO_SetA++;
							}
						}
						//SetB
						if(j == 1){
							if(m == 0){
								ResonanceRatio_SeventhO_FU_SetB[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_FU_SetB[i]<<endl;
								
								OverallResonance_SeventhO_FU_SetB = OverallResonance_SeventhO_FU_SetB + ResonanceRatio_SeventhO_FU_SetB[i];
								NumOfSeventhO_FU_SetB++;
							}
							if(m == 1){
								ResonanceRatio_SeventhO_FO_SetB[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_FO_SetB[i]<<endl;
								
								OverallResonance_SeventhO_FO_SetB = OverallResonance_SeventhO_FO_SetB + ResonanceRatio_SeventhO_FO_SetB[i];
								NumOfSeventhO_FO_SetB++;
							}
							if(m == 2){
								ResonanceRatio_SeventhO_2O_SetB[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_2O_SetB[i]<<endl;
								
								OverallResonance_SeventhO_2O_SetB = OverallResonance_SeventhO_2O_SetB + ResonanceRatio_SeventhO_2O_SetB[i];
								NumOfSeventhO_2O_SetB++;
							}
							if(m == 3){
								ResonanceRatio_SeventhO_ThirdO_SetB[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_ThirdO_SetB[i]<<endl;
								
								OverallResonance_SeventhO_ThirdO_SetB = OverallResonance_SeventhO_ThirdO_SetB + ResonanceRatio_SeventhO_ThirdO_SetB[i];
								NumOfSeventhO_ThirdO_SetB++;
							}
							if(m == 4){
								ResonanceRatio_SeventhO_ForthO_SetB[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_ForthO_SetB[i]<<endl;
								
								OverallResonance_SeventhO_ForthO_SetB = OverallResonance_SeventhO_ForthO_SetB + ResonanceRatio_SeventhO_ForthO_SetB[i];
								NumOfSeventhO_ForthO_SetB++;
							}
							if(m == 5){
								ResonanceRatio_SeventhO_FifthO_SetB[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_FifthO_SetB[i]<<endl;
								
								OverallResonance_SeventhO_FifthO_SetB = OverallResonance_SeventhO_FifthO_SetB + ResonanceRatio_SeventhO_FifthO_SetB[i];
								NumOfSeventhO_FifthO_SetB++;
							}
							if(m == 6){
								ResonanceRatio_SeventhO_SixthO_SetB[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_SixthO_SetB[i]<<endl;
								
								OverallResonance_SeventhO_SixthO_SetB = OverallResonance_SeventhO_SixthO_SetB + ResonanceRatio_SeventhO_SixthO_SetB[i];
								NumOfSeventhO_SixthO_SetB++;
							}
							if(m == 7){
								ResonanceRatio_SeventhO_SeventhO_SetB[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_SeventhO_SetB[i]<<endl;
								
								OverallResonance_SeventhO_SeventhO_SetB = OverallResonance_SeventhO_SeventhO_SetB + ResonanceRatio_SeventhO_SeventhO_SetB[i];
								NumOfSeventhO_SeventhO_SetB++;
							}
							if(m == 8){
								ResonanceRatio_SeventhO_EighthO_SetB[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_EighthO_SetB[i]<<endl;
								
								OverallResonance_SeventhO_EighthO_SetB = OverallResonance_SeventhO_EighthO_SetB + ResonanceRatio_SeventhO_EighthO_SetB[i];
								NumOfSeventhO_EighthO_SetB++;
							}
							if(m == 9){
								ResonanceRatio_SeventhO_NinthO_SetB[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_NinthO_SetB[i]<<endl;
								
								OverallResonance_SeventhO_NinthO_SetB = OverallResonance_SeventhO_NinthO_SetB + ResonanceRatio_SeventhO_NinthO_SetB[i];
								NumOfSeventhO_NinthO_SetB++;
							}
						}
						//SetC
						if(j == 2){
							if(m == 0){
								ResonanceRatio_SeventhO_FU_SetC[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_FU_SetC[i]<<endl;
								
								OverallResonance_SeventhO_FU_SetC = OverallResonance_SeventhO_FU_SetC + ResonanceRatio_SeventhO_FU_SetC[i];
								NumOfSeventhO_FU_SetC++;
							}
							if(m == 1){
								ResonanceRatio_SeventhO_FO_SetC[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_FO_SetC[i]<<endl;
								
								OverallResonance_SeventhO_FO_SetC = OverallResonance_SeventhO_FO_SetC + ResonanceRatio_SeventhO_FO_SetC[i];
								NumOfSeventhO_FO_SetC++;
							}
							if(m == 2){
								ResonanceRatio_SeventhO_2O_SetC[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_2O_SetC[i]<<endl;
								
								OverallResonance_SeventhO_2O_SetC = OverallResonance_SeventhO_2O_SetC + ResonanceRatio_SeventhO_2O_SetC[i];
								NumOfSeventhO_2O_SetC++;
							}
							if(m == 3){
								ResonanceRatio_SeventhO_ThirdO_SetC[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_ThirdO_SetC[i]<<endl;
								
								OverallResonance_SeventhO_ThirdO_SetC = OverallResonance_SeventhO_ThirdO_SetC + ResonanceRatio_SeventhO_ThirdO_SetC[i];
								NumOfSeventhO_ThirdO_SetC++;
							}
							if(m == 4){
								ResonanceRatio_SeventhO_ForthO_SetC[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_ForthO_SetC[i]<<endl;
								
								OverallResonance_SeventhO_ForthO_SetC = OverallResonance_SeventhO_ForthO_SetC + ResonanceRatio_SeventhO_ForthO_SetC[i];
								NumOfSeventhO_ForthO_SetC++;
							}
							if(m == 5){
								ResonanceRatio_SeventhO_FifthO_SetC[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_FifthO_SetC[i]<<endl;
								
								OverallResonance_SeventhO_FifthO_SetC = OverallResonance_SeventhO_FifthO_SetC + ResonanceRatio_SeventhO_FifthO_SetC[i];
								NumOfSeventhO_FifthO_SetC++;
							}
							if(m == 6){
								ResonanceRatio_SeventhO_SixthO_SetC[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_SixthO_SetC[i]<<endl;
								
								OverallResonance_SeventhO_SixthO_SetC = OverallResonance_SeventhO_SixthO_SetC + ResonanceRatio_SeventhO_SixthO_SetC[i];
								NumOfSeventhO_SixthO_SetC++;
							}
							if(m == 7){
								ResonanceRatio_SeventhO_SeventhO_SetC[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_SeventhO_SetC[i]<<endl;
								
								OverallResonance_SeventhO_SeventhO_SetC = OverallResonance_SeventhO_SeventhO_SetC + ResonanceRatio_SeventhO_SeventhO_SetC[i];
								NumOfSeventhO_SeventhO_SetC++;
							}
							if(m == 8){
								ResonanceRatio_SeventhO_EighthO_SetC[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_EighthO_SetC[i]<<endl;
								
								OverallResonance_SeventhO_EighthO_SetC = OverallResonance_SeventhO_EighthO_SetC + ResonanceRatio_SeventhO_EighthO_SetC[i];
								NumOfSeventhO_EighthO_SetC++;
							}
							if(m == 9){
								ResonanceRatio_SeventhO_NinthO_SetC[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_NinthO_SetC[i]<<endl;
								
								OverallResonance_SeventhO_NinthO_SetC = OverallResonance_SeventhO_NinthO_SetC + ResonanceRatio_SeventhO_NinthO_SetC[i];
								NumOfSeventhO_NinthO_SetC++;
							}
						}
						//SetD
						if(j == 3){
							if(m == 0){
								ResonanceRatio_SeventhO_FU_SetD[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_FU_SetD[i]<<endl;
								
								OverallResonance_SeventhO_FU_SetD = OverallResonance_SeventhO_FU_SetD + ResonanceRatio_SeventhO_FU_SetD[i];
								NumOfSeventhO_FU_SetD++;
							}
							if(m == 1){
								ResonanceRatio_SeventhO_FO_SetD[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_FO_SetD[i]<<endl;
								
								OverallResonance_SeventhO_FO_SetD = OverallResonance_SeventhO_FO_SetD + ResonanceRatio_SeventhO_FO_SetD[i];
								NumOfSeventhO_FO_SetD++;
							}
							if(m == 2){
								ResonanceRatio_SeventhO_2O_SetD[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_2O_SetD[i]<<endl;
								
								OverallResonance_SeventhO_2O_SetD = OverallResonance_SeventhO_2O_SetD + ResonanceRatio_SeventhO_2O_SetD[i];
								NumOfSeventhO_2O_SetD++;
							}
							if(m == 3){
								ResonanceRatio_SeventhO_ThirdO_SetD[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_ThirdO_SetD[i]<<endl;
								
								OverallResonance_SeventhO_ThirdO_SetD = OverallResonance_SeventhO_ThirdO_SetD + ResonanceRatio_SeventhO_ThirdO_SetD[i];
								NumOfSeventhO_ThirdO_SetD++;
							}
							if(m == 4){
								ResonanceRatio_SeventhO_ForthO_SetD[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_ForthO_SetD[i]<<endl;
								
								OverallResonance_SeventhO_ForthO_SetD = OverallResonance_SeventhO_ForthO_SetD + ResonanceRatio_SeventhO_ForthO_SetD[i];
								NumOfSeventhO_ForthO_SetD++;
							}
							if(m == 5){
								ResonanceRatio_SeventhO_FifthO_SetD[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_FifthO_SetD[i]<<endl;
								
								OverallResonance_SeventhO_FifthO_SetD = OverallResonance_SeventhO_FifthO_SetD + ResonanceRatio_SeventhO_FifthO_SetD[i];
								NumOfSeventhO_FifthO_SetD++;
							}
							if(m == 6){
								ResonanceRatio_SeventhO_SixthO_SetD[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_SixthO_SetD[i]<<endl;
								
								OverallResonance_SeventhO_SixthO_SetD = OverallResonance_SeventhO_SixthO_SetD + ResonanceRatio_SeventhO_SixthO_SetD[i];
								NumOfSeventhO_SixthO_SetD++;
							}
							if(m == 7){
								ResonanceRatio_SeventhO_SeventhO_SetD[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_SeventhO_SetD[i]<<endl;
								
								OverallResonance_SeventhO_SeventhO_SetD = OverallResonance_SeventhO_SeventhO_SetD + ResonanceRatio_SeventhO_SeventhO_SetD[i];
								NumOfSeventhO_SeventhO_SetD++;
							}
							if(m == 8){
								ResonanceRatio_SeventhO_EighthO_SetD[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_EighthO_SetD[i]<<endl;
								
								OverallResonance_SeventhO_EighthO_SetD = OverallResonance_SeventhO_EighthO_SetD + ResonanceRatio_SeventhO_EighthO_SetD[i];
								NumOfSeventhO_EighthO_SetD++;
							}
							if(m == 9){
								ResonanceRatio_SeventhO_NinthO_SetD[i] = period_SeventhO/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_NinthO_SetD[i]<<endl;
								
								OverallResonance_SeventhO_NinthO_SetD = OverallResonance_SeventhO_NinthO_SetD + ResonanceRatio_SeventhO_NinthO_SetD[i];
								NumOfSeventhO_NinthO_SetD++;
							}
						}
					}
					//EighthO mode//
					if((growth > 0.0 || !DoPosGrowthCut) && (growth_EighthO > 0)){
						cout<<"Resonance Ratios With EighthO..."<<endl;
						//SetA
						if(j == 0){
							if(m == 0){
								ResonanceRatio_EighthO_FU_SetA[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_FU_SetA[i]<<endl;
								
								OverallResonance_EighthO_FU_SetA = OverallResonance_EighthO_FU_SetA + ResonanceRatio_EighthO_FU_SetA[i];
								NumOfEighthO_FU_SetA++;
							}
							if(m == 1){
								ResonanceRatio_EighthO_FO_SetA[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_FO_SetA[i]<<endl;
								
								OverallResonance_EighthO_FO_SetA = OverallResonance_EighthO_FO_SetA + ResonanceRatio_EighthO_FO_SetA[i];
								NumOfEighthO_FO_SetA++;
							}
							if(m == 2){
								ResonanceRatio_EighthO_2O_SetA[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_2O_SetA[i]<<endl;
								
								OverallResonance_EighthO_2O_SetA = OverallResonance_EighthO_2O_SetA + ResonanceRatio_EighthO_2O_SetA[i];
								NumOfEighthO_2O_SetA++;
							}
							if(m == 3){
								ResonanceRatio_EighthO_ThirdO_SetA[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_ThirdO_SetA[i]<<endl;
								
								OverallResonance_EighthO_ThirdO_SetA = OverallResonance_EighthO_ThirdO_SetA + ResonanceRatio_EighthO_ThirdO_SetA[i];
								NumOfEighthO_ThirdO_SetA++;
							}
							if(m == 4){
								ResonanceRatio_EighthO_ForthO_SetA[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_ForthO_SetA[i]<<endl;
								
								OverallResonance_EighthO_ForthO_SetA = OverallResonance_EighthO_ForthO_SetA + ResonanceRatio_EighthO_ForthO_SetA[i];
								NumOfEighthO_ForthO_SetA++;
							}
							if(m == 5){
								ResonanceRatio_EighthO_FifthO_SetA[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_FifthO_SetA[i]<<endl;
								
								OverallResonance_EighthO_FifthO_SetA = OverallResonance_EighthO_FifthO_SetA + ResonanceRatio_EighthO_FifthO_SetA[i];
								NumOfEighthO_FifthO_SetA++;
							}
							if(m == 6){
								ResonanceRatio_EighthO_SixthO_SetA[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_SixthO_SetA[i]<<endl;
								
								OverallResonance_EighthO_SixthO_SetA = OverallResonance_EighthO_SixthO_SetA + ResonanceRatio_EighthO_SixthO_SetA[i];
								NumOfEighthO_SixthO_SetA++;
							}
							if(m == 7){
								ResonanceRatio_EighthO_SeventhO_SetA[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_SeventhO_SetA[i]<<endl;
								
								OverallResonance_EighthO_SeventhO_SetA = OverallResonance_EighthO_SeventhO_SetA + ResonanceRatio_EighthO_SeventhO_SetA[i];
								NumOfEighthO_SeventhO_SetA++;
							}
							if(m == 8){
								ResonanceRatio_EighthO_EighthO_SetA[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_EighthO_SetA[i]<<endl;
								
								OverallResonance_EighthO_EighthO_SetA = OverallResonance_EighthO_EighthO_SetA + ResonanceRatio_EighthO_EighthO_SetA[i];
								NumOfEighthO_EighthO_SetA++;
							}
							if(m == 9){
								ResonanceRatio_EighthO_NinthO_SetA[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_NinthO_SetA[i]<<endl;
								
								OverallResonance_EighthO_NinthO_SetA = OverallResonance_EighthO_NinthO_SetA + ResonanceRatio_EighthO_NinthO_SetA[i];
								NumOfEighthO_NinthO_SetA++;
							}
						}
						//SetB
						if(j == 1){
							if(m == 0){
								ResonanceRatio_EighthO_FU_SetB[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_FU_SetB[i]<<endl;
								
								OverallResonance_EighthO_FU_SetB = OverallResonance_EighthO_FU_SetB + ResonanceRatio_EighthO_FU_SetB[i];
								NumOfEighthO_FU_SetB++;
							}
							if(m == 1){
								ResonanceRatio_EighthO_FO_SetB[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_FO_SetB[i]<<endl;
								
								OverallResonance_EighthO_FO_SetB = OverallResonance_EighthO_FO_SetB + ResonanceRatio_EighthO_FO_SetB[i];
								NumOfEighthO_FO_SetB++;
							}
							if(m == 2){
								ResonanceRatio_EighthO_2O_SetB[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_2O_SetB[i]<<endl;
								
								OverallResonance_EighthO_2O_SetB = OverallResonance_EighthO_2O_SetB + ResonanceRatio_EighthO_2O_SetB[i];
								NumOfEighthO_2O_SetB++;
							}
							if(m == 3){
								ResonanceRatio_EighthO_ThirdO_SetB[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_ThirdO_SetB[i]<<endl;
								
								OverallResonance_EighthO_ThirdO_SetB = OverallResonance_EighthO_ThirdO_SetB + ResonanceRatio_EighthO_ThirdO_SetB[i];
								NumOfEighthO_ThirdO_SetB++;
							}
							if(m == 4){
								ResonanceRatio_EighthO_ForthO_SetB[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_ForthO_SetB[i]<<endl;
								
								OverallResonance_EighthO_ForthO_SetB = OverallResonance_EighthO_ForthO_SetB + ResonanceRatio_EighthO_ForthO_SetB[i];
								NumOfEighthO_ForthO_SetB++;
							}
							if(m == 5){
								ResonanceRatio_EighthO_FifthO_SetB[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_FifthO_SetB[i]<<endl;
								
								OverallResonance_EighthO_FifthO_SetB = OverallResonance_EighthO_FifthO_SetB + ResonanceRatio_EighthO_FifthO_SetB[i];
								NumOfEighthO_FifthO_SetB++;
							}
							if(m == 6){
								ResonanceRatio_EighthO_SixthO_SetB[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_SixthO_SetB[i]<<endl;
								
								OverallResonance_EighthO_SixthO_SetB = OverallResonance_EighthO_SixthO_SetB + ResonanceRatio_EighthO_SixthO_SetB[i];
								NumOfEighthO_SixthO_SetB++;
							}
							if(m == 7){
								ResonanceRatio_EighthO_SeventhO_SetB[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_SeventhO_SetB[i]<<endl;
								
								OverallResonance_EighthO_SeventhO_SetB = OverallResonance_EighthO_SeventhO_SetB + ResonanceRatio_EighthO_SeventhO_SetB[i];
								NumOfEighthO_SeventhO_SetB++;
							}
							if(m == 8){
								ResonanceRatio_EighthO_EighthO_SetB[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_EighthO_SetB[i]<<endl;
								
								OverallResonance_EighthO_EighthO_SetB = OverallResonance_EighthO_EighthO_SetB + ResonanceRatio_EighthO_EighthO_SetB[i];
								NumOfEighthO_EighthO_SetB++;
							}
							if(m == 9){
								ResonanceRatio_EighthO_NinthO_SetB[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_NinthO_SetB[i]<<endl;
								
								OverallResonance_EighthO_NinthO_SetB = OverallResonance_EighthO_NinthO_SetB + ResonanceRatio_EighthO_NinthO_SetB[i];
								NumOfEighthO_NinthO_SetB++;
							}
						}
						//SetC
						if(j == 2){
							if(m == 0){
								ResonanceRatio_EighthO_FU_SetC[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_FU_SetC[i]<<endl;
								
								OverallResonance_EighthO_FU_SetC = OverallResonance_EighthO_FU_SetC + ResonanceRatio_EighthO_FU_SetC[i];
								NumOfEighthO_FU_SetC++;
							}
							if(m == 1){
								ResonanceRatio_EighthO_FO_SetC[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_FO_SetC[i]<<endl;
								
								OverallResonance_EighthO_FO_SetC = OverallResonance_EighthO_FO_SetC + ResonanceRatio_EighthO_FO_SetC[i];
								NumOfEighthO_FO_SetC++;
							}
							if(m == 2){
								ResonanceRatio_EighthO_2O_SetC[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_2O_SetC[i]<<endl;
								
								OverallResonance_EighthO_2O_SetC = OverallResonance_EighthO_2O_SetC + ResonanceRatio_EighthO_2O_SetC[i];
								NumOfEighthO_2O_SetC++;
							}
							if(m == 3){
								ResonanceRatio_EighthO_ThirdO_SetC[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_ThirdO_SetC[i]<<endl;
								
								OverallResonance_EighthO_ThirdO_SetC = OverallResonance_EighthO_ThirdO_SetC + ResonanceRatio_EighthO_ThirdO_SetC[i];
								NumOfEighthO_ThirdO_SetC++;
							}
							if(m == 4){
								ResonanceRatio_EighthO_ForthO_SetC[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_ForthO_SetC[i]<<endl;
								
								OverallResonance_EighthO_ForthO_SetC = OverallResonance_EighthO_ForthO_SetC + ResonanceRatio_EighthO_ForthO_SetC[i];
								NumOfEighthO_ForthO_SetC++;
							}
							if(m == 5){
								ResonanceRatio_EighthO_FifthO_SetC[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_FifthO_SetC[i]<<endl;
								
								OverallResonance_EighthO_FifthO_SetC = OverallResonance_EighthO_FifthO_SetC + ResonanceRatio_EighthO_FifthO_SetC[i];
								NumOfEighthO_FifthO_SetC++;
							}
							if(m == 6){
								ResonanceRatio_EighthO_SixthO_SetC[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_SixthO_SetC[i]<<endl;
								
								OverallResonance_EighthO_SixthO_SetC = OverallResonance_EighthO_SixthO_SetC + ResonanceRatio_EighthO_SixthO_SetC[i];
								NumOfEighthO_SixthO_SetC++;
							}
							if(m == 7){
								ResonanceRatio_EighthO_SeventhO_SetC[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_SeventhO_SetC[i]<<endl;
								
								OverallResonance_EighthO_SeventhO_SetC = OverallResonance_EighthO_SeventhO_SetC + ResonanceRatio_EighthO_SeventhO_SetC[i];
								NumOfEighthO_SeventhO_SetC++;
							}
							if(m == 8){
								ResonanceRatio_EighthO_EighthO_SetC[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_EighthO_SetC[i]<<endl;
								
								OverallResonance_EighthO_EighthO_SetC = OverallResonance_EighthO_EighthO_SetC + ResonanceRatio_EighthO_EighthO_SetC[i];
								NumOfEighthO_EighthO_SetC++;
							}
							if(m == 9){
								ResonanceRatio_EighthO_NinthO_SetC[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_NinthO_SetC[i]<<endl;
								
								OverallResonance_EighthO_NinthO_SetC = OverallResonance_EighthO_NinthO_SetC + ResonanceRatio_EighthO_NinthO_SetC[i];
								NumOfEighthO_NinthO_SetC++;
							}
						}
						//SetD
						if(j == 3){
							if(m == 0){
								ResonanceRatio_EighthO_FU_SetD[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_FU_SetD[i]<<endl;
								
								OverallResonance_EighthO_FU_SetD = OverallResonance_EighthO_FU_SetD + ResonanceRatio_EighthO_FU_SetD[i];
								NumOfEighthO_FU_SetD++;
							}
							if(m == 1){
								ResonanceRatio_EighthO_FO_SetD[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_FO_SetD[i]<<endl;
								
								OverallResonance_EighthO_FO_SetD = OverallResonance_EighthO_FO_SetD + ResonanceRatio_EighthO_FO_SetD[i];
								NumOfEighthO_FO_SetD++;
							}
							if(m == 2){
								ResonanceRatio_EighthO_2O_SetD[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_2O_SetD[i]<<endl;
								
								OverallResonance_EighthO_2O_SetD = OverallResonance_EighthO_2O_SetD + ResonanceRatio_EighthO_2O_SetD[i];
								NumOfEighthO_2O_SetD++;
							}
							if(m == 3){
								ResonanceRatio_EighthO_ThirdO_SetD[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_ThirdO_SetD[i]<<endl;
								
								OverallResonance_EighthO_ThirdO_SetD = OverallResonance_EighthO_ThirdO_SetD + ResonanceRatio_EighthO_ThirdO_SetD[i];
								NumOfEighthO_ThirdO_SetD++;
							}
							if(m == 4){
								ResonanceRatio_EighthO_ForthO_SetD[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_ForthO_SetD[i]<<endl;
								
								OverallResonance_EighthO_ForthO_SetD = OverallResonance_EighthO_ForthO_SetD + ResonanceRatio_EighthO_ForthO_SetD[i];
								NumOfEighthO_ForthO_SetD++;
							}
							if(m == 5){
								ResonanceRatio_EighthO_FifthO_SetD[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_FifthO_SetD[i]<<endl;
								
								OverallResonance_EighthO_FifthO_SetD = OverallResonance_EighthO_FifthO_SetD + ResonanceRatio_EighthO_FifthO_SetD[i];
								NumOfEighthO_FifthO_SetD++;
							}
							if(m == 6){
								ResonanceRatio_EighthO_SixthO_SetD[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_SixthO_SetD[i]<<endl;
								
								OverallResonance_EighthO_SixthO_SetD = OverallResonance_EighthO_SixthO_SetD + ResonanceRatio_EighthO_SixthO_SetD[i];
								NumOfEighthO_SixthO_SetD++;
							}
							if(m == 7){
								ResonanceRatio_EighthO_SeventhO_SetD[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_SeventhO_SetD[i]<<endl;
								
								OverallResonance_EighthO_SeventhO_SetD = OverallResonance_EighthO_SeventhO_SetD + ResonanceRatio_EighthO_SeventhO_SetD[i];
								NumOfEighthO_SeventhO_SetD++;
							}
							if(m == 8){
								ResonanceRatio_EighthO_EighthO_SetD[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_EighthO_SetD[i]<<endl;
								
								OverallResonance_EighthO_EighthO_SetD = OverallResonance_EighthO_EighthO_SetD + ResonanceRatio_EighthO_EighthO_SetD[i];
								NumOfEighthO_EighthO_SetD++;
							}
							if(m == 9){
								ResonanceRatio_EighthO_NinthO_SetD[i] = period_EighthO/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_NinthO_SetD[i]<<endl;
								
								OverallResonance_EighthO_NinthO_SetD = OverallResonance_EighthO_NinthO_SetD + ResonanceRatio_EighthO_NinthO_SetD[i];
								NumOfEighthO_NinthO_SetD++;
							}
						}
					}
					//NinthO mode//
					if((growth > 0.0 || !DoPosGrowthCut) && (growth_NinthO > 0)){
						cout<<"Resonance Ratios With NinthO..."<<endl;
						//SetA
						if(j == 0){
							if(m == 0){
								ResonanceRatio_NinthO_FU_SetA[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_FU_SetA[i]<<endl;
								
								OverallResonance_NinthO_FU_SetA = OverallResonance_NinthO_FU_SetA + ResonanceRatio_NinthO_FU_SetA[i];
								NumOfNinthO_FU_SetA++;
							}
							if(m == 1){
								ResonanceRatio_NinthO_FO_SetA[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_FO_SetA[i]<<endl;
								
								OverallResonance_NinthO_FO_SetA = OverallResonance_NinthO_FO_SetA + ResonanceRatio_NinthO_FO_SetA[i];
								NumOfNinthO_FO_SetA++;
							}
							if(m == 2){
								ResonanceRatio_NinthO_2O_SetA[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_2O_SetA[i]<<endl;
								
								OverallResonance_NinthO_2O_SetA = OverallResonance_NinthO_2O_SetA + ResonanceRatio_NinthO_2O_SetA[i];
								NumOfNinthO_2O_SetA++;
							}
							if(m == 3){
								ResonanceRatio_NinthO_ThirdO_SetA[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_ThirdO_SetA[i]<<endl;
								
								OverallResonance_NinthO_ThirdO_SetA = OverallResonance_NinthO_ThirdO_SetA + ResonanceRatio_NinthO_ThirdO_SetA[i];
								NumOfNinthO_ThirdO_SetA++;
							}
							if(m == 4){
								ResonanceRatio_NinthO_ForthO_SetA[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_ForthO_SetA[i]<<endl;
								
								OverallResonance_NinthO_ForthO_SetA = OverallResonance_NinthO_ForthO_SetA + ResonanceRatio_NinthO_ForthO_SetA[i];
								NumOfNinthO_ForthO_SetA++;
							}
							if(m == 5){
								ResonanceRatio_NinthO_FifthO_SetA[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_FifthO_SetA[i]<<endl;
								
								OverallResonance_NinthO_FifthO_SetA = OverallResonance_NinthO_FifthO_SetA + ResonanceRatio_NinthO_FifthO_SetA[i];
								NumOfNinthO_FifthO_SetA++;
							}
							if(m == 6){
								ResonanceRatio_NinthO_SixthO_SetA[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_SixthO_SetA[i]<<endl;
								
								OverallResonance_NinthO_SixthO_SetA = OverallResonance_NinthO_SixthO_SetA + ResonanceRatio_NinthO_SixthO_SetA[i];
								NumOfNinthO_SixthO_SetA++;
							}
							if(m == 7){
								ResonanceRatio_NinthO_SeventhO_SetA[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_SeventhO_SetA[i]<<endl;
								
								OverallResonance_NinthO_SeventhO_SetA = OverallResonance_NinthO_SeventhO_SetA + ResonanceRatio_NinthO_SeventhO_SetA[i];
								NumOfNinthO_SeventhO_SetA++;
							}
							if(m == 8){
								ResonanceRatio_NinthO_EighthO_SetA[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_EighthO_SetA[i]<<endl;
								
								OverallResonance_NinthO_EighthO_SetA = OverallResonance_NinthO_EighthO_SetA + ResonanceRatio_NinthO_EighthO_SetA[i];
								NumOfNinthO_EighthO_SetA++;
							}
							if(m == 9){
								ResonanceRatio_NinthO_NinthO_SetA[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_NinthO_SetA[i]<<endl;
								
								OverallResonance_NinthO_NinthO_SetA = OverallResonance_NinthO_NinthO_SetA + ResonanceRatio_NinthO_NinthO_SetA[i];
								NumOfNinthO_NinthO_SetA++;
							}
						}
						//SetB
						if(j == 1){
							if(m == 0){
								ResonanceRatio_NinthO_FU_SetB[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_FU_SetB[i]<<endl;
								
								OverallResonance_NinthO_FU_SetB = OverallResonance_NinthO_FU_SetB + ResonanceRatio_NinthO_FU_SetB[i];
								NumOfNinthO_FU_SetB++;
							}
							if(m == 1){
								ResonanceRatio_NinthO_FO_SetB[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_FO_SetB[i]<<endl;
								
								OverallResonance_NinthO_FO_SetB = OverallResonance_NinthO_FO_SetB + ResonanceRatio_NinthO_FO_SetB[i];
								NumOfNinthO_FO_SetB++;
							}
							if(m == 2){
								ResonanceRatio_NinthO_2O_SetB[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_2O_SetB[i]<<endl;
								
								OverallResonance_NinthO_2O_SetB = OverallResonance_NinthO_2O_SetB + ResonanceRatio_NinthO_2O_SetB[i];
								NumOfNinthO_2O_SetB++;
							}
							if(m == 3){
								ResonanceRatio_NinthO_ThirdO_SetB[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_ThirdO_SetB[i]<<endl;
								
								OverallResonance_NinthO_ThirdO_SetB = OverallResonance_NinthO_ThirdO_SetB + ResonanceRatio_NinthO_ThirdO_SetB[i];
								NumOfNinthO_ThirdO_SetB++;
							}
							if(m == 4){
								ResonanceRatio_NinthO_ForthO_SetB[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_ForthO_SetB[i]<<endl;
								
								OverallResonance_NinthO_ForthO_SetB = OverallResonance_NinthO_ForthO_SetB + ResonanceRatio_NinthO_ForthO_SetB[i];
								NumOfNinthO_ForthO_SetB++;
							}
							if(m == 5){
								ResonanceRatio_NinthO_FifthO_SetB[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_FifthO_SetB[i]<<endl;
								
								OverallResonance_NinthO_FifthO_SetB = OverallResonance_NinthO_FifthO_SetB + ResonanceRatio_NinthO_FifthO_SetB[i];
								NumOfNinthO_FifthO_SetB++;
							}
							if(m == 6){
								ResonanceRatio_NinthO_SixthO_SetB[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_SixthO_SetB[i]<<endl;
								
								OverallResonance_NinthO_SixthO_SetB = OverallResonance_NinthO_SixthO_SetB + ResonanceRatio_NinthO_SixthO_SetB[i];
								NumOfNinthO_SixthO_SetB++;
							}
							if(m == 7){
								ResonanceRatio_NinthO_SeventhO_SetB[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_SeventhO_SetB[i]<<endl;
								
								OverallResonance_NinthO_SeventhO_SetB = OverallResonance_NinthO_SeventhO_SetB + ResonanceRatio_NinthO_SeventhO_SetB[i];
								NumOfNinthO_SeventhO_SetB++;
							}
							if(m == 8){
								ResonanceRatio_NinthO_EighthO_SetB[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_EighthO_SetB[i]<<endl;
								
								OverallResonance_NinthO_EighthO_SetB = OverallResonance_NinthO_EighthO_SetB + ResonanceRatio_NinthO_EighthO_SetB[i];
								NumOfNinthO_EighthO_SetB++;
							}
							if(m == 9){
								ResonanceRatio_NinthO_NinthO_SetB[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_NinthO_SetB[i]<<endl;
								
								OverallResonance_NinthO_NinthO_SetB = OverallResonance_NinthO_NinthO_SetB + ResonanceRatio_NinthO_NinthO_SetB[i];
								NumOfNinthO_NinthO_SetB++;
							}
						}
						//SetC
						if(j == 2){
							if(m == 0){
								ResonanceRatio_NinthO_FU_SetC[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_FU_SetC[i]<<endl;
								
								OverallResonance_NinthO_FU_SetC = OverallResonance_NinthO_FU_SetC + ResonanceRatio_NinthO_FU_SetC[i];
								NumOfNinthO_FU_SetC++;
							}
							if(m == 1){
								ResonanceRatio_NinthO_FO_SetC[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_FO_SetC[i]<<endl;
								
								OverallResonance_NinthO_FO_SetC = OverallResonance_NinthO_FO_SetC + ResonanceRatio_NinthO_FO_SetC[i];
								NumOfNinthO_FO_SetC++;
							}
							if(m == 2){
								ResonanceRatio_NinthO_2O_SetC[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_2O_SetC[i]<<endl;
								
								OverallResonance_NinthO_2O_SetC = OverallResonance_NinthO_2O_SetC + ResonanceRatio_NinthO_2O_SetC[i];
								NumOfNinthO_2O_SetC++;
							}
							if(m == 3){
								ResonanceRatio_NinthO_ThirdO_SetC[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_ThirdO_SetC[i]<<endl;
								
								OverallResonance_NinthO_ThirdO_SetC = OverallResonance_NinthO_ThirdO_SetC + ResonanceRatio_NinthO_ThirdO_SetC[i];
								NumOfNinthO_ThirdO_SetC++;
							}
							if(m == 4){
								ResonanceRatio_NinthO_ForthO_SetC[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_ForthO_SetC[i]<<endl;
								
								OverallResonance_NinthO_ForthO_SetC = OverallResonance_NinthO_ForthO_SetC + ResonanceRatio_NinthO_ForthO_SetC[i];
								NumOfNinthO_ForthO_SetC++;
							}
							if(m == 5){
								ResonanceRatio_NinthO_FifthO_SetC[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_FifthO_SetC[i]<<endl;
								
								OverallResonance_NinthO_FifthO_SetC = OverallResonance_NinthO_FifthO_SetC + ResonanceRatio_NinthO_FifthO_SetC[i];
								NumOfNinthO_FifthO_SetC++;
							}
							if(m == 6){
								ResonanceRatio_NinthO_SixthO_SetC[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_SixthO_SetC[i]<<endl;
								
								OverallResonance_NinthO_SixthO_SetC = OverallResonance_NinthO_SixthO_SetC + ResonanceRatio_NinthO_SixthO_SetC[i];
								NumOfNinthO_SixthO_SetC++;
							}
							if(m == 7){
								ResonanceRatio_NinthO_SeventhO_SetC[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_SeventhO_SetC[i]<<endl;
								
								OverallResonance_NinthO_SeventhO_SetC = OverallResonance_NinthO_SeventhO_SetC + ResonanceRatio_NinthO_SeventhO_SetC[i];
								NumOfNinthO_SeventhO_SetC++;
							}
							if(m == 8){
								ResonanceRatio_NinthO_EighthO_SetC[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_EighthO_SetC[i]<<endl;
								
								OverallResonance_NinthO_EighthO_SetC = OverallResonance_NinthO_EighthO_SetC + ResonanceRatio_NinthO_EighthO_SetC[i];
								NumOfNinthO_EighthO_SetC++;
							}
							if(m == 9){
								ResonanceRatio_NinthO_NinthO_SetC[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_NinthO_SetC[i]<<endl;
								
								OverallResonance_NinthO_NinthO_SetC = OverallResonance_NinthO_NinthO_SetC + ResonanceRatio_NinthO_NinthO_SetC[i];
								NumOfNinthO_NinthO_SetC++;
							}
						}
						//SetD
						if(j == 3){
							if(m == 0){
								ResonanceRatio_NinthO_FU_SetD[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_FU_SetD[i]<<endl;
								
								OverallResonance_NinthO_FU_SetD = OverallResonance_NinthO_FU_SetD + ResonanceRatio_NinthO_FU_SetD[i];
								NumOfNinthO_FU_SetD++;
							}
							if(m == 1){
								ResonanceRatio_NinthO_FO_SetD[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_FO_SetD[i]<<endl;
								
								OverallResonance_NinthO_FO_SetD = OverallResonance_NinthO_FO_SetD + ResonanceRatio_NinthO_FO_SetD[i];
								NumOfNinthO_FO_SetD++;
							}
							if(m == 2){
								ResonanceRatio_NinthO_2O_SetD[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_2O_SetD[i]<<endl;
								
								OverallResonance_NinthO_2O_SetD = OverallResonance_NinthO_2O_SetD + ResonanceRatio_NinthO_2O_SetD[i];
								NumOfNinthO_2O_SetD++;
							}
							if(m == 3){
								ResonanceRatio_NinthO_ThirdO_SetD[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_ThirdO_SetD[i]<<endl;
								
								OverallResonance_NinthO_ThirdO_SetD = OverallResonance_NinthO_ThirdO_SetD + ResonanceRatio_NinthO_ThirdO_SetD[i];
								NumOfNinthO_ThirdO_SetD++;
							}
							if(m == 4){
								ResonanceRatio_NinthO_ForthO_SetD[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_ForthO_SetD[i]<<endl;
								
								OverallResonance_NinthO_ForthO_SetD = OverallResonance_NinthO_ForthO_SetD + ResonanceRatio_NinthO_ForthO_SetD[i];
								NumOfNinthO_ForthO_SetD++;
							}
							if(m == 5){
								ResonanceRatio_NinthO_FifthO_SetD[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_FifthO_SetD[i]<<endl;
								
								OverallResonance_NinthO_FifthO_SetD = OverallResonance_NinthO_FifthO_SetD + ResonanceRatio_NinthO_FifthO_SetD[i];
								NumOfNinthO_FifthO_SetD++;
							}
							if(m == 6){
								ResonanceRatio_NinthO_SixthO_SetD[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_SixthO_SetD[i]<<endl;
								
								OverallResonance_NinthO_SixthO_SetD = OverallResonance_NinthO_SixthO_SetD + ResonanceRatio_NinthO_SixthO_SetD[i];
								NumOfNinthO_SixthO_SetD++;
							}
							if(m == 7){
								ResonanceRatio_NinthO_SeventhO_SetD[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_SeventhO_SetD[i]<<endl;
								
								OverallResonance_NinthO_SeventhO_SetD = OverallResonance_NinthO_SeventhO_SetD + ResonanceRatio_NinthO_SeventhO_SetD[i];
								NumOfNinthO_SeventhO_SetD++;
							}
							if(m == 8){
								ResonanceRatio_NinthO_EighthO_SetD[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_EighthO_SetD[i]<<endl;
								
								OverallResonance_NinthO_EighthO_SetD = OverallResonance_NinthO_EighthO_SetD + ResonanceRatio_NinthO_EighthO_SetD[i];
								NumOfNinthO_EighthO_SetD++;
							}
							if(m == 9){
								ResonanceRatio_NinthO_NinthO_SetD[i] = period_NinthO/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_NinthO_SetD[i]<<endl;
								
								OverallResonance_NinthO_NinthO_SetD = OverallResonance_NinthO_NinthO_SetD + ResonanceRatio_NinthO_NinthO_SetD[i];
								NumOfNinthO_NinthO_SetD++;
							}
						}
					}
				}
			}
			inFile.close();
		}
		
		outFile_AverageResonance.open(AverageResonanceFileName, ios::out);
		outFile_AverageResonance<<setw(30)<<"Resonances with FU"<<setw(30)<<"Resonances with FO"<<setw(30)<<"Resonances with 2O"<<setw(30)<<"Resonances with ThirdO"<<setw(30)<<"Resonances with ForthO"<<setw(30)<<"Resonances with FifthO"<<setw(30)<<"Resonances with SixthO"<<setw(30)<<"Resonances with SeventhO"<<setw(30)<<"Resonances with EighthO"<<setw(30)<<"Resonances with NinthO"<<endl;
		if(j == 0){
			
			outFile_AverageResonance<<setw(10)<<"FU: "<<setw(20)<<(OverallResonance_FU_FU_SetA/NumOfFU_FU_SetA)<<setw(30)<<(OverallResonance_FO_FU_SetA/NumOfFO_FU_SetA)<<setw(30)<<(OverallResonance_2O_FU_SetA/NumOf2O_FU_SetA)<<setw(30)<<(OverallResonance_ThirdO_FU_SetA/NumOfThirdO_FU_SetA)<<setw(30)<<(OverallResonance_ForthO_FU_SetA/NumOfForthO_FU_SetA)<<setw(30)<<(OverallResonance_FifthO_FU_SetA/NumOfFifthO_FU_SetA)<<setw(30)<<(OverallResonance_SixthO_FU_SetA/NumOfSixthO_FU_SetA)<<setw(30)<<(OverallResonance_SeventhO_FU_SetA/NumOfSeventhO_FU_SetA)<<setw(30)<<(OverallResonance_EighthO_FU_SetA/NumOfEighthO_FU_SetA)<<setw(30)<<(OverallResonance_NinthO_FU_SetA/NumOfNinthO_FU_SetA)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"FO: "<<setw(20)<<(OverallResonance_FU_FO_SetA/NumOfFU_FO_SetA)<<setw(30)<<(OverallResonance_FO_FO_SetA/NumOfFO_FO_SetA)<<setw(30)<<(OverallResonance_2O_FO_SetA/NumOf2O_FO_SetA)<<setw(30)<<(OverallResonance_ThirdO_FO_SetA/NumOfThirdO_FO_SetA)<<setw(30)<<(OverallResonance_ForthO_FO_SetA/NumOfForthO_FO_SetA)<<setw(30)<<(OverallResonance_FifthO_FO_SetA/NumOfFifthO_FO_SetA)<<setw(30)<<(OverallResonance_SixthO_FO_SetA/NumOfSixthO_FO_SetA)<<setw(30)<<(OverallResonance_SeventhO_FO_SetA/NumOfSeventhO_FO_SetA)<<setw(30)<<(OverallResonance_EighthO_FO_SetA/NumOfEighthO_FO_SetA)<<setw(30)<<(OverallResonance_NinthO_FO_SetA/NumOfNinthO_FO_SetA)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"2O: "<<setw(20)<<(OverallResonance_FU_2O_SetA/NumOfFU_2O_SetA)<<setw(30)<<(OverallResonance_FO_2O_SetA/NumOfFO_2O_SetA)<<setw(30)<<(OverallResonance_2O_2O_SetA/NumOf2O_2O_SetA)<<setw(30)<<(OverallResonance_ThirdO_2O_SetA/NumOfThirdO_2O_SetA)<<setw(30)<<(OverallResonance_ForthO_2O_SetA/NumOfForthO_2O_SetA)<<setw(30)<<(OverallResonance_FifthO_2O_SetA/NumOfFifthO_2O_SetA)<<setw(30)<<(OverallResonance_SixthO_2O_SetA/NumOfSixthO_2O_SetA)<<setw(30)<<(OverallResonance_SeventhO_2O_SetA/NumOfSeventhO_2O_SetA)<<setw(30)<<(OverallResonance_EighthO_2O_SetA/NumOfEighthO_2O_SetA)<<setw(30)<<(OverallResonance_NinthO_2O_SetA/NumOfNinthO_2O_SetA)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"ThirdO: "<<setw(20)<<(OverallResonance_FU_ThirdO_SetA/NumOfFU_ThirdO_SetA)<<setw(30)<<(OverallResonance_FO_ThirdO_SetA/NumOfFO_ThirdO_SetA)<<setw(30)<<(OverallResonance_2O_ThirdO_SetA/NumOf2O_ThirdO_SetA)<<setw(30)<<(OverallResonance_ThirdO_ThirdO_SetA/NumOfThirdO_ThirdO_SetA)<<setw(30)<<(OverallResonance_ForthO_ThirdO_SetA/NumOfForthO_ThirdO_SetA)<<setw(30)<<(OverallResonance_FifthO_ThirdO_SetA/NumOfFifthO_ThirdO_SetA)<<setw(30)<<(OverallResonance_SixthO_ThirdO_SetA/NumOfSixthO_ThirdO_SetA)<<setw(30)<<(OverallResonance_SeventhO_ThirdO_SetA/NumOfSeventhO_ThirdO_SetA)<<setw(30)<<(OverallResonance_EighthO_ThirdO_SetA/NumOfEighthO_ThirdO_SetA)<<setw(30)<<(OverallResonance_NinthO_ThirdO_SetA/NumOfNinthO_ThirdO_SetA)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"ForthO: "<<setw(20)<<(OverallResonance_FU_ForthO_SetA/NumOfFU_ForthO_SetA)<<setw(30)<<(OverallResonance_FO_ForthO_SetA/NumOfFO_ForthO_SetA)<<setw(30)<<(OverallResonance_2O_ForthO_SetA/NumOf2O_ForthO_SetA)<<setw(30)<<(OverallResonance_ThirdO_ForthO_SetA/NumOfThirdO_ForthO_SetA)<<setw(30)<<(OverallResonance_ForthO_ForthO_SetA/NumOfForthO_ForthO_SetA)<<setw(30)<<(OverallResonance_FifthO_ForthO_SetA/NumOfFifthO_ForthO_SetA)<<setw(30)<<(OverallResonance_SixthO_ForthO_SetA/NumOfSixthO_ForthO_SetA)<<setw(30)<<(OverallResonance_SeventhO_ForthO_SetA/NumOfSeventhO_ForthO_SetA)<<setw(30)<<(OverallResonance_EighthO_ForthO_SetA/NumOfEighthO_ForthO_SetA)<<setw(30)<<(OverallResonance_NinthO_ForthO_SetA/NumOfNinthO_ForthO_SetA)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"FifthO: "<<setw(20)<<(OverallResonance_FU_FifthO_SetA/NumOfFU_FifthO_SetA)<<setw(30)<<(OverallResonance_FO_FifthO_SetA/NumOfFO_FifthO_SetA)<<setw(30)<<(OverallResonance_2O_FifthO_SetA/NumOf2O_FifthO_SetA)<<setw(30)<<(OverallResonance_ThirdO_FifthO_SetA/NumOfThirdO_FifthO_SetA)<<setw(30)<<(OverallResonance_ForthO_FifthO_SetA/NumOfForthO_FifthO_SetA)<<setw(30)<<(OverallResonance_FifthO_FifthO_SetA/NumOfFifthO_FifthO_SetA)<<setw(30)<<(OverallResonance_SixthO_FifthO_SetA/NumOfSixthO_FifthO_SetA)<<setw(30)<<(OverallResonance_SeventhO_FifthO_SetA/NumOfSeventhO_FifthO_SetA)<<setw(30)<<(OverallResonance_EighthO_FifthO_SetA/NumOfEighthO_FifthO_SetA)<<setw(30)<<(OverallResonance_NinthO_FifthO_SetA/NumOfNinthO_FifthO_SetA)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"SixthO: "<<setw(20)<<(OverallResonance_FU_SixthO_SetA/NumOfFU_SixthO_SetA)<<setw(30)<<(OverallResonance_FO_SixthO_SetA/NumOfFO_SixthO_SetA)<<setw(30)<<(OverallResonance_2O_SixthO_SetA/NumOf2O_SixthO_SetA)<<setw(30)<<(OverallResonance_ThirdO_SixthO_SetA/NumOfThirdO_SixthO_SetA)<<setw(30)<<(OverallResonance_ForthO_SixthO_SetA/NumOfForthO_SixthO_SetA)<<setw(30)<<(OverallResonance_FifthO_SixthO_SetA/NumOfFifthO_SixthO_SetA)<<setw(30)<<(OverallResonance_SixthO_SixthO_SetA/NumOfSixthO_SixthO_SetA)<<setw(30)<<(OverallResonance_SeventhO_SixthO_SetA/NumOfSeventhO_SixthO_SetA)<<setw(30)<<(OverallResonance_EighthO_SixthO_SetA/NumOfEighthO_SixthO_SetA)<<setw(30)<<(OverallResonance_NinthO_SixthO_SetA/NumOfNinthO_SixthO_SetA)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"SeventhO: "<<setw(20)<<(OverallResonance_FU_SeventhO_SetA/NumOfFU_SeventhO_SetA)<<setw(30)<<(OverallResonance_FO_SeventhO_SetA/NumOfFO_SeventhO_SetA)<<setw(30)<<(OverallResonance_2O_SeventhO_SetA/NumOf2O_SeventhO_SetA)<<setw(30)<<(OverallResonance_ThirdO_SeventhO_SetA/NumOfThirdO_SeventhO_SetA)<<setw(30)<<(OverallResonance_ForthO_SeventhO_SetA/NumOfForthO_SeventhO_SetA)<<setw(30)<<(OverallResonance_FifthO_SeventhO_SetA/NumOfFifthO_SeventhO_SetA)<<setw(30)<<(OverallResonance_SixthO_SeventhO_SetA/NumOfSixthO_SeventhO_SetA)<<setw(30)<<(OverallResonance_SeventhO_SeventhO_SetA/NumOfSeventhO_SeventhO_SetA)<<setw(30)<<(OverallResonance_EighthO_SeventhO_SetA/NumOfEighthO_SeventhO_SetA)<<setw(30)<<(OverallResonance_NinthO_SeventhO_SetA/NumOfNinthO_SeventhO_SetA)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"EighthO: "<<setw(20)<<(OverallResonance_FU_EighthO_SetA/NumOfFU_EighthO_SetA)<<setw(30)<<(OverallResonance_FO_EighthO_SetA/NumOfFO_EighthO_SetA)<<setw(30)<<(OverallResonance_2O_EighthO_SetA/NumOf2O_EighthO_SetA)<<setw(30)<<(OverallResonance_ThirdO_EighthO_SetA/NumOfThirdO_EighthO_SetA)<<setw(30)<<(OverallResonance_ForthO_EighthO_SetA/NumOfForthO_EighthO_SetA)<<setw(30)<<(OverallResonance_FifthO_EighthO_SetA/NumOfFifthO_EighthO_SetA)<<setw(30)<<(OverallResonance_SixthO_EighthO_SetA/NumOfSixthO_EighthO_SetA)<<setw(30)<<(OverallResonance_SeventhO_EighthO_SetA/NumOfSeventhO_EighthO_SetA)<<setw(30)<<(OverallResonance_EighthO_EighthO_SetA/NumOfEighthO_EighthO_SetA)<<setw(30)<<(OverallResonance_NinthO_EighthO_SetA/NumOfNinthO_EighthO_SetA)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"NinthO: "<<setw(20)<<(OverallResonance_FU_NinthO_SetA/NumOfFU_NinthO_SetA)<<setw(30)<<(OverallResonance_FO_NinthO_SetA/NumOfFO_NinthO_SetA)<<setw(30)<<(OverallResonance_2O_NinthO_SetA/NumOf2O_NinthO_SetA)<<setw(30)<<(OverallResonance_ThirdO_NinthO_SetA/NumOfThirdO_NinthO_SetA)<<setw(30)<<(OverallResonance_ForthO_NinthO_SetA/NumOfForthO_NinthO_SetA)<<setw(30)<<(OverallResonance_FifthO_NinthO_SetA/NumOfFifthO_NinthO_SetA)<<setw(30)<<(OverallResonance_SixthO_NinthO_SetA/NumOfSixthO_NinthO_SetA)<<setw(30)<<(OverallResonance_SeventhO_NinthO_SetA/NumOfSeventhO_NinthO_SetA)<<setw(30)<<(OverallResonance_EighthO_NinthO_SetA/NumOfEighthO_NinthO_SetA)<<setw(30)<<(OverallResonance_NinthO_NinthO_SetA/NumOfNinthO_NinthO_SetA)<<endl;
			
		}
		if(j == 1){
				
			outFile_AverageResonance<<setw(10)<<"FU: "<<setw(20)<<(OverallResonance_FU_FU_SetB/NumOfFU_FU_SetB)<<setw(30)<<(OverallResonance_FO_FU_SetB/NumOfFO_FU_SetB)<<setw(30)<<(OverallResonance_2O_FU_SetB/NumOf2O_FU_SetB)<<setw(30)<<(OverallResonance_ThirdO_FU_SetB/NumOfThirdO_FU_SetB)<<setw(30)<<(OverallResonance_ForthO_FU_SetB/NumOfForthO_FU_SetB)<<setw(30)<<(OverallResonance_FifthO_FU_SetB/NumOfFifthO_FU_SetB)<<setw(30)<<(OverallResonance_SixthO_FU_SetB/NumOfSixthO_FU_SetB)<<setw(30)<<(OverallResonance_SeventhO_FU_SetB/NumOfSeventhO_FU_SetB)<<setw(30)<<(OverallResonance_EighthO_FU_SetB/NumOfEighthO_FU_SetB)<<setw(30)<<(OverallResonance_NinthO_FU_SetB/NumOfNinthO_FU_SetB)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"FO: "<<setw(20)<<(OverallResonance_FU_FO_SetB/NumOfFU_FO_SetB)<<setw(30)<<(OverallResonance_FO_FO_SetB/NumOfFO_FO_SetB)<<setw(30)<<(OverallResonance_2O_FO_SetB/NumOf2O_FO_SetB)<<setw(30)<<(OverallResonance_ThirdO_FO_SetB/NumOfThirdO_FO_SetB)<<setw(30)<<(OverallResonance_ForthO_FO_SetB/NumOfForthO_FO_SetB)<<setw(30)<<(OverallResonance_FifthO_FO_SetB/NumOfFifthO_FO_SetB)<<setw(30)<<(OverallResonance_SixthO_FO_SetB/NumOfSixthO_FO_SetB)<<setw(30)<<(OverallResonance_SeventhO_FO_SetB/NumOfSeventhO_FO_SetB)<<setw(30)<<(OverallResonance_EighthO_FO_SetB/NumOfEighthO_FO_SetB)<<setw(30)<<(OverallResonance_NinthO_FO_SetB/NumOfNinthO_FO_SetB)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"2O: "<<setw(20)<<(OverallResonance_FU_2O_SetB/NumOfFU_2O_SetB)<<setw(30)<<(OverallResonance_FO_2O_SetB/NumOfFO_2O_SetB)<<setw(30)<<(OverallResonance_2O_2O_SetB/NumOf2O_2O_SetB)<<setw(30)<<(OverallResonance_ThirdO_2O_SetB/NumOfThirdO_2O_SetB)<<setw(30)<<(OverallResonance_ForthO_2O_SetB/NumOfForthO_2O_SetB)<<setw(30)<<(OverallResonance_FifthO_2O_SetB/NumOfFifthO_2O_SetB)<<setw(30)<<(OverallResonance_SixthO_2O_SetB/NumOfSixthO_2O_SetB)<<setw(30)<<(OverallResonance_SeventhO_2O_SetB/NumOfSeventhO_2O_SetB)<<setw(30)<<(OverallResonance_EighthO_2O_SetB/NumOfEighthO_2O_SetB)<<setw(30)<<(OverallResonance_NinthO_2O_SetB/NumOfNinthO_2O_SetB)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"ThirdO: "<<setw(20)<<(OverallResonance_FU_ThirdO_SetB/NumOfFU_ThirdO_SetB)<<setw(30)<<(OverallResonance_FO_ThirdO_SetB/NumOfFO_ThirdO_SetB)<<setw(30)<<(OverallResonance_2O_ThirdO_SetB/NumOf2O_ThirdO_SetB)<<setw(30)<<(OverallResonance_ThirdO_ThirdO_SetB/NumOfThirdO_ThirdO_SetB)<<setw(30)<<(OverallResonance_ForthO_ThirdO_SetB/NumOfForthO_ThirdO_SetB)<<setw(30)<<(OverallResonance_FifthO_ThirdO_SetB/NumOfFifthO_ThirdO_SetB)<<setw(30)<<(OverallResonance_SixthO_ThirdO_SetB/NumOfSixthO_ThirdO_SetB)<<setw(30)<<(OverallResonance_SeventhO_ThirdO_SetB/NumOfSeventhO_ThirdO_SetB)<<setw(30)<<(OverallResonance_EighthO_ThirdO_SetB/NumOfEighthO_ThirdO_SetB)<<setw(30)<<(OverallResonance_NinthO_ThirdO_SetB/NumOfNinthO_ThirdO_SetB)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"ForthO: "<<setw(20)<<(OverallResonance_FU_ForthO_SetB/NumOfFU_ForthO_SetB)<<setw(30)<<(OverallResonance_FO_ForthO_SetB/NumOfFO_ForthO_SetB)<<setw(30)<<(OverallResonance_2O_ForthO_SetB/NumOf2O_ForthO_SetB)<<setw(30)<<(OverallResonance_ThirdO_ForthO_SetB/NumOfThirdO_ForthO_SetB)<<setw(30)<<(OverallResonance_ForthO_ForthO_SetB/NumOfForthO_ForthO_SetB)<<setw(30)<<(OverallResonance_FifthO_ForthO_SetB/NumOfFifthO_ForthO_SetB)<<setw(30)<<(OverallResonance_SixthO_ForthO_SetB/NumOfSixthO_ForthO_SetB)<<setw(30)<<(OverallResonance_SeventhO_ForthO_SetB/NumOfSeventhO_ForthO_SetB)<<setw(30)<<(OverallResonance_EighthO_ForthO_SetB/NumOfEighthO_ForthO_SetB)<<setw(30)<<(OverallResonance_NinthO_ForthO_SetB/NumOfNinthO_ForthO_SetB)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"FifthO: "<<setw(20)<<(OverallResonance_FU_FifthO_SetB/NumOfFU_FifthO_SetB)<<setw(30)<<(OverallResonance_FO_FifthO_SetB/NumOfFO_FifthO_SetB)<<setw(30)<<(OverallResonance_2O_FifthO_SetB/NumOf2O_FifthO_SetB)<<setw(30)<<(OverallResonance_ThirdO_FifthO_SetB/NumOfThirdO_FifthO_SetB)<<setw(30)<<(OverallResonance_ForthO_FifthO_SetB/NumOfForthO_FifthO_SetB)<<setw(30)<<(OverallResonance_FifthO_FifthO_SetB/NumOfFifthO_FifthO_SetB)<<setw(30)<<(OverallResonance_SixthO_FifthO_SetB/NumOfSixthO_FifthO_SetB)<<setw(30)<<(OverallResonance_SeventhO_FifthO_SetB/NumOfSeventhO_FifthO_SetB)<<setw(30)<<(OverallResonance_EighthO_FifthO_SetB/NumOfEighthO_FifthO_SetB)<<setw(30)<<(OverallResonance_NinthO_FifthO_SetB/NumOfNinthO_FifthO_SetB)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"SixthO: "<<setw(20)<<(OverallResonance_FU_SixthO_SetB/NumOfFU_SixthO_SetB)<<setw(30)<<(OverallResonance_FO_SixthO_SetB/NumOfFO_SixthO_SetB)<<setw(30)<<(OverallResonance_2O_SixthO_SetB/NumOf2O_SixthO_SetB)<<setw(30)<<(OverallResonance_ThirdO_SixthO_SetB/NumOfThirdO_SixthO_SetB)<<setw(30)<<(OverallResonance_ForthO_SixthO_SetB/NumOfForthO_SixthO_SetB)<<setw(30)<<(OverallResonance_FifthO_SixthO_SetB/NumOfFifthO_SixthO_SetB)<<setw(30)<<(OverallResonance_SixthO_SixthO_SetB/NumOfSixthO_SixthO_SetB)<<setw(30)<<(OverallResonance_SeventhO_SixthO_SetB/NumOfSeventhO_SixthO_SetB)<<setw(30)<<(OverallResonance_EighthO_SixthO_SetB/NumOfEighthO_SixthO_SetB)<<setw(30)<<(OverallResonance_NinthO_SixthO_SetB/NumOfNinthO_SixthO_SetB)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"SeventhO: "<<setw(20)<<(OverallResonance_FU_SeventhO_SetB/NumOfFU_SeventhO_SetB)<<setw(30)<<(OverallResonance_FO_SeventhO_SetB/NumOfFO_SeventhO_SetB)<<setw(30)<<(OverallResonance_2O_SeventhO_SetB/NumOf2O_SeventhO_SetB)<<setw(30)<<(OverallResonance_ThirdO_SeventhO_SetB/NumOfThirdO_SeventhO_SetB)<<setw(30)<<(OverallResonance_ForthO_SeventhO_SetB/NumOfForthO_SeventhO_SetB)<<setw(30)<<(OverallResonance_FifthO_SeventhO_SetB/NumOfFifthO_SeventhO_SetB)<<setw(30)<<(OverallResonance_SixthO_SeventhO_SetB/NumOfSixthO_SeventhO_SetB)<<setw(30)<<(OverallResonance_SeventhO_SeventhO_SetB/NumOfSeventhO_SeventhO_SetB)<<setw(30)<<(OverallResonance_EighthO_SeventhO_SetB/NumOfEighthO_SeventhO_SetB)<<setw(30)<<(OverallResonance_NinthO_SeventhO_SetB/NumOfNinthO_SeventhO_SetB)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"EighthO: "<<setw(20)<<(OverallResonance_FU_EighthO_SetB/NumOfFU_EighthO_SetB)<<setw(30)<<(OverallResonance_FO_EighthO_SetB/NumOfFO_EighthO_SetB)<<setw(30)<<(OverallResonance_2O_EighthO_SetB/NumOf2O_EighthO_SetB)<<setw(30)<<(OverallResonance_ThirdO_EighthO_SetB/NumOfThirdO_EighthO_SetB)<<setw(30)<<(OverallResonance_ForthO_EighthO_SetB/NumOfForthO_EighthO_SetB)<<setw(30)<<(OverallResonance_FifthO_EighthO_SetB/NumOfFifthO_EighthO_SetB)<<setw(30)<<(OverallResonance_SixthO_EighthO_SetB/NumOfSixthO_EighthO_SetB)<<setw(30)<<(OverallResonance_SeventhO_EighthO_SetB/NumOfSeventhO_EighthO_SetB)<<setw(30)<<(OverallResonance_EighthO_EighthO_SetB/NumOfEighthO_EighthO_SetB)<<setw(30)<<(OverallResonance_NinthO_EighthO_SetB/NumOfNinthO_EighthO_SetB)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"NinthO: "<<setw(20)<<(OverallResonance_FU_NinthO_SetB/NumOfFU_NinthO_SetB)<<setw(30)<<(OverallResonance_FO_NinthO_SetB/NumOfFO_NinthO_SetB)<<setw(30)<<(OverallResonance_2O_NinthO_SetB/NumOf2O_NinthO_SetB)<<setw(30)<<(OverallResonance_ThirdO_NinthO_SetB/NumOfThirdO_NinthO_SetB)<<setw(30)<<(OverallResonance_ForthO_NinthO_SetB/NumOfForthO_NinthO_SetB)<<setw(30)<<(OverallResonance_FifthO_NinthO_SetB/NumOfFifthO_NinthO_SetB)<<setw(30)<<(OverallResonance_SixthO_NinthO_SetB/NumOfSixthO_NinthO_SetB)<<setw(30)<<(OverallResonance_SeventhO_NinthO_SetB/NumOfSeventhO_NinthO_SetB)<<setw(30)<<(OverallResonance_EighthO_NinthO_SetB/NumOfEighthO_NinthO_SetB)<<setw(30)<<(OverallResonance_NinthO_NinthO_SetB/NumOfNinthO_NinthO_SetB)<<endl;
			
		}
		if(j == 2){
				
			outFile_AverageResonance<<setw(10)<<"FU: "<<setw(20)<<(OverallResonance_FU_FU_SetC/NumOfFU_FU_SetC)<<setw(30)<<(OverallResonance_FO_FU_SetC/NumOfFO_FU_SetC)<<setw(30)<<(OverallResonance_2O_FU_SetC/NumOf2O_FU_SetC)<<setw(30)<<(OverallResonance_ThirdO_FU_SetC/NumOfThirdO_FU_SetC)<<setw(30)<<(OverallResonance_ForthO_FU_SetC/NumOfForthO_FU_SetC)<<setw(30)<<(OverallResonance_FifthO_FU_SetC/NumOfFifthO_FU_SetC)<<setw(30)<<(OverallResonance_SixthO_FU_SetC/NumOfSixthO_FU_SetC)<<setw(30)<<(OverallResonance_SeventhO_FU_SetC/NumOfSeventhO_FU_SetC)<<setw(30)<<(OverallResonance_EighthO_FU_SetC/NumOfEighthO_FU_SetC)<<setw(30)<<(OverallResonance_NinthO_FU_SetC/NumOfNinthO_FU_SetC)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"FO: "<<setw(20)<<(OverallResonance_FU_FO_SetC/NumOfFU_FO_SetC)<<setw(30)<<(OverallResonance_FO_FO_SetC/NumOfFO_FO_SetC)<<setw(30)<<(OverallResonance_2O_FO_SetC/NumOf2O_FO_SetC)<<setw(30)<<(OverallResonance_ThirdO_FO_SetC/NumOfThirdO_FO_SetC)<<setw(30)<<(OverallResonance_ForthO_FO_SetC/NumOfForthO_FO_SetC)<<setw(30)<<(OverallResonance_FifthO_FO_SetC/NumOfFifthO_FO_SetC)<<setw(30)<<(OverallResonance_SixthO_FO_SetC/NumOfSixthO_FO_SetC)<<setw(30)<<(OverallResonance_SeventhO_FO_SetC/NumOfSeventhO_FO_SetC)<<setw(30)<<(OverallResonance_EighthO_FO_SetC/NumOfEighthO_FO_SetC)<<setw(30)<<(OverallResonance_NinthO_FO_SetC/NumOfNinthO_FO_SetC)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"2O: "<<setw(20)<<(OverallResonance_FU_2O_SetC/NumOfFU_2O_SetC)<<setw(30)<<(OverallResonance_FO_2O_SetC/NumOfFO_2O_SetC)<<setw(30)<<(OverallResonance_2O_2O_SetC/NumOf2O_2O_SetC)<<setw(30)<<(OverallResonance_ThirdO_2O_SetC/NumOfThirdO_2O_SetC)<<setw(30)<<(OverallResonance_ForthO_2O_SetC/NumOfForthO_2O_SetC)<<setw(30)<<(OverallResonance_FifthO_2O_SetC/NumOfFifthO_2O_SetC)<<setw(30)<<(OverallResonance_SixthO_2O_SetC/NumOfSixthO_2O_SetC)<<setw(30)<<(OverallResonance_SeventhO_2O_SetC/NumOfSeventhO_2O_SetC)<<setw(30)<<(OverallResonance_EighthO_2O_SetC/NumOfEighthO_2O_SetC)<<setw(30)<<(OverallResonance_NinthO_2O_SetC/NumOfNinthO_2O_SetC)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"ThirdO: "<<setw(20)<<(OverallResonance_FU_ThirdO_SetC/NumOfFU_ThirdO_SetC)<<setw(30)<<(OverallResonance_FO_ThirdO_SetC/NumOfFO_ThirdO_SetC)<<setw(30)<<(OverallResonance_2O_ThirdO_SetC/NumOf2O_ThirdO_SetC)<<setw(30)<<(OverallResonance_ThirdO_ThirdO_SetC/NumOfThirdO_ThirdO_SetC)<<setw(30)<<(OverallResonance_ForthO_ThirdO_SetC/NumOfForthO_ThirdO_SetC)<<setw(30)<<(OverallResonance_FifthO_ThirdO_SetC/NumOfFifthO_ThirdO_SetC)<<setw(30)<<(OverallResonance_SixthO_ThirdO_SetC/NumOfSixthO_ThirdO_SetC)<<setw(30)<<(OverallResonance_SeventhO_ThirdO_SetC/NumOfSeventhO_ThirdO_SetC)<<setw(30)<<(OverallResonance_EighthO_ThirdO_SetC/NumOfEighthO_ThirdO_SetC)<<setw(30)<<(OverallResonance_NinthO_ThirdO_SetC/NumOfNinthO_ThirdO_SetC)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"ForthO: "<<setw(20)<<(OverallResonance_FU_ForthO_SetC/NumOfFU_ForthO_SetC)<<setw(30)<<(OverallResonance_FO_ForthO_SetC/NumOfFO_ForthO_SetC)<<setw(30)<<(OverallResonance_2O_ForthO_SetC/NumOf2O_ForthO_SetC)<<setw(30)<<(OverallResonance_ThirdO_ForthO_SetC/NumOfThirdO_ForthO_SetC)<<setw(30)<<(OverallResonance_ForthO_ForthO_SetC/NumOfForthO_ForthO_SetC)<<setw(30)<<(OverallResonance_FifthO_ForthO_SetC/NumOfFifthO_ForthO_SetC)<<setw(30)<<(OverallResonance_SixthO_ForthO_SetC/NumOfSixthO_ForthO_SetC)<<setw(30)<<(OverallResonance_SeventhO_ForthO_SetC/NumOfSeventhO_ForthO_SetC)<<setw(30)<<(OverallResonance_EighthO_ForthO_SetC/NumOfEighthO_ForthO_SetC)<<setw(30)<<(OverallResonance_NinthO_ForthO_SetC/NumOfNinthO_ForthO_SetC)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"FifthO: "<<setw(20)<<(OverallResonance_FU_FifthO_SetC/NumOfFU_FifthO_SetC)<<setw(30)<<(OverallResonance_FO_FifthO_SetC/NumOfFO_FifthO_SetC)<<setw(30)<<(OverallResonance_2O_FifthO_SetC/NumOf2O_FifthO_SetC)<<setw(30)<<(OverallResonance_ThirdO_FifthO_SetC/NumOfThirdO_FifthO_SetC)<<setw(30)<<(OverallResonance_ForthO_FifthO_SetC/NumOfForthO_FifthO_SetC)<<setw(30)<<(OverallResonance_FifthO_FifthO_SetC/NumOfFifthO_FifthO_SetC)<<setw(30)<<(OverallResonance_SixthO_FifthO_SetC/NumOfSixthO_FifthO_SetC)<<setw(30)<<(OverallResonance_SeventhO_FifthO_SetC/NumOfSeventhO_FifthO_SetC)<<setw(30)<<(OverallResonance_EighthO_FifthO_SetC/NumOfEighthO_FifthO_SetC)<<setw(30)<<(OverallResonance_NinthO_FifthO_SetC/NumOfNinthO_FifthO_SetC)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"SixthO: "<<setw(20)<<(OverallResonance_FU_SixthO_SetC/NumOfFU_SixthO_SetC)<<setw(30)<<(OverallResonance_FO_SixthO_SetC/NumOfFO_SixthO_SetC)<<setw(30)<<(OverallResonance_2O_SixthO_SetC/NumOf2O_SixthO_SetC)<<setw(30)<<(OverallResonance_ThirdO_SixthO_SetC/NumOfThirdO_SixthO_SetC)<<setw(30)<<(OverallResonance_ForthO_SixthO_SetC/NumOfForthO_SixthO_SetC)<<setw(30)<<(OverallResonance_FifthO_SixthO_SetC/NumOfFifthO_SixthO_SetC)<<setw(30)<<(OverallResonance_SixthO_SixthO_SetC/NumOfSixthO_SixthO_SetC)<<setw(30)<<(OverallResonance_SeventhO_SixthO_SetC/NumOfSeventhO_SixthO_SetC)<<setw(30)<<(OverallResonance_EighthO_SixthO_SetC/NumOfEighthO_SixthO_SetC)<<setw(30)<<(OverallResonance_NinthO_SixthO_SetC/NumOfNinthO_SixthO_SetC)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"SeventhO: "<<setw(20)<<(OverallResonance_FU_SeventhO_SetC/NumOfFU_SeventhO_SetC)<<setw(30)<<(OverallResonance_FO_SeventhO_SetC/NumOfFO_SeventhO_SetC)<<setw(30)<<(OverallResonance_2O_SeventhO_SetC/NumOf2O_SeventhO_SetC)<<setw(30)<<(OverallResonance_ThirdO_SeventhO_SetC/NumOfThirdO_SeventhO_SetC)<<setw(30)<<(OverallResonance_ForthO_SeventhO_SetC/NumOfForthO_SeventhO_SetC)<<setw(30)<<(OverallResonance_FifthO_SeventhO_SetC/NumOfFifthO_SeventhO_SetC)<<setw(30)<<(OverallResonance_SixthO_SeventhO_SetC/NumOfSixthO_SeventhO_SetC)<<setw(30)<<(OverallResonance_SeventhO_SeventhO_SetC/NumOfSeventhO_SeventhO_SetC)<<setw(30)<<(OverallResonance_EighthO_SeventhO_SetC/NumOfEighthO_SeventhO_SetC)<<setw(30)<<(OverallResonance_NinthO_SeventhO_SetC/NumOfNinthO_SeventhO_SetC)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"EighthO: "<<setw(20)<<(OverallResonance_FU_EighthO_SetC/NumOfFU_EighthO_SetC)<<setw(30)<<(OverallResonance_FO_EighthO_SetC/NumOfFO_EighthO_SetC)<<setw(30)<<(OverallResonance_2O_EighthO_SetC/NumOf2O_EighthO_SetC)<<setw(30)<<(OverallResonance_ThirdO_EighthO_SetC/NumOfThirdO_EighthO_SetC)<<setw(30)<<(OverallResonance_ForthO_EighthO_SetC/NumOfForthO_EighthO_SetC)<<setw(30)<<(OverallResonance_FifthO_EighthO_SetC/NumOfFifthO_EighthO_SetC)<<setw(30)<<(OverallResonance_SixthO_EighthO_SetC/NumOfSixthO_EighthO_SetC)<<setw(30)<<(OverallResonance_SeventhO_EighthO_SetC/NumOfSeventhO_EighthO_SetC)<<setw(30)<<(OverallResonance_EighthO_EighthO_SetC/NumOfEighthO_EighthO_SetC)<<setw(30)<<(OverallResonance_NinthO_EighthO_SetC/NumOfNinthO_EighthO_SetC)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"NinthO: "<<setw(20)<<(OverallResonance_FU_NinthO_SetC/NumOfFU_NinthO_SetC)<<setw(30)<<(OverallResonance_FO_NinthO_SetC/NumOfFO_NinthO_SetC)<<setw(30)<<(OverallResonance_2O_NinthO_SetC/NumOf2O_NinthO_SetC)<<setw(30)<<(OverallResonance_ThirdO_NinthO_SetC/NumOfThirdO_NinthO_SetC)<<setw(30)<<(OverallResonance_ForthO_NinthO_SetC/NumOfForthO_NinthO_SetC)<<setw(30)<<(OverallResonance_FifthO_NinthO_SetC/NumOfFifthO_NinthO_SetC)<<setw(30)<<(OverallResonance_SixthO_NinthO_SetC/NumOfSixthO_NinthO_SetC)<<setw(30)<<(OverallResonance_SeventhO_NinthO_SetC/NumOfSeventhO_NinthO_SetC)<<setw(30)<<(OverallResonance_EighthO_NinthO_SetC/NumOfEighthO_NinthO_SetC)<<setw(30)<<(OverallResonance_NinthO_NinthO_SetC/NumOfNinthO_NinthO_SetC)<<endl;
			
		}
		if(j == 3){
				
			outFile_AverageResonance<<setw(10)<<"FU: "<<setw(20)<<(OverallResonance_FU_FU_SetD/NumOfFU_FU_SetD)<<setw(30)<<(OverallResonance_FO_FU_SetD/NumOfFO_FU_SetD)<<setw(30)<<(OverallResonance_2O_FU_SetD/NumOf2O_FU_SetD)<<setw(30)<<(OverallResonance_ThirdO_FU_SetD/NumOfThirdO_FU_SetD)<<setw(30)<<(OverallResonance_ForthO_FU_SetD/NumOfForthO_FU_SetD)<<setw(30)<<(OverallResonance_FifthO_FU_SetD/NumOfFifthO_FU_SetD)<<setw(30)<<(OverallResonance_SixthO_FU_SetD/NumOfSixthO_FU_SetD)<<setw(30)<<(OverallResonance_SeventhO_FU_SetD/NumOfSeventhO_FU_SetD)<<setw(30)<<(OverallResonance_EighthO_FU_SetD/NumOfEighthO_FU_SetD)<<setw(30)<<(OverallResonance_NinthO_FU_SetD/NumOfNinthO_FU_SetD)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"FO: "<<setw(20)<<(OverallResonance_FU_FO_SetD/NumOfFU_FO_SetD)<<setw(30)<<(OverallResonance_FO_FO_SetD/NumOfFO_FO_SetD)<<setw(30)<<(OverallResonance_2O_FO_SetD/NumOf2O_FO_SetD)<<setw(30)<<(OverallResonance_ThirdO_FO_SetD/NumOfThirdO_FO_SetD)<<setw(30)<<(OverallResonance_ForthO_FO_SetD/NumOfForthO_FO_SetD)<<setw(30)<<(OverallResonance_FifthO_FO_SetD/NumOfFifthO_FO_SetD)<<setw(30)<<(OverallResonance_SixthO_FO_SetD/NumOfSixthO_FO_SetD)<<setw(30)<<(OverallResonance_SeventhO_FO_SetD/NumOfSeventhO_FO_SetD)<<setw(30)<<(OverallResonance_EighthO_FO_SetD/NumOfEighthO_FO_SetD)<<setw(30)<<(OverallResonance_NinthO_FO_SetD/NumOfNinthO_FO_SetD)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"2O: "<<setw(20)<<(OverallResonance_FU_2O_SetD/NumOfFU_2O_SetD)<<setw(30)<<(OverallResonance_FO_2O_SetD/NumOfFO_2O_SetD)<<setw(30)<<(OverallResonance_2O_2O_SetD/NumOf2O_2O_SetD)<<setw(30)<<(OverallResonance_ThirdO_2O_SetD/NumOfThirdO_2O_SetD)<<setw(30)<<(OverallResonance_ForthO_2O_SetD/NumOfForthO_2O_SetD)<<setw(30)<<(OverallResonance_FifthO_2O_SetD/NumOfFifthO_2O_SetD)<<setw(30)<<(OverallResonance_SixthO_2O_SetD/NumOfSixthO_2O_SetD)<<setw(30)<<(OverallResonance_SeventhO_2O_SetD/NumOfSeventhO_2O_SetD)<<setw(30)<<(OverallResonance_EighthO_2O_SetD/NumOfEighthO_2O_SetD)<<setw(30)<<(OverallResonance_NinthO_2O_SetD/NumOfNinthO_2O_SetD)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"ThirdO: "<<setw(20)<<(OverallResonance_FU_ThirdO_SetD/NumOfFU_ThirdO_SetD)<<setw(30)<<(OverallResonance_FO_ThirdO_SetD/NumOfFO_ThirdO_SetD)<<setw(30)<<(OverallResonance_2O_ThirdO_SetD/NumOf2O_ThirdO_SetD)<<setw(30)<<(OverallResonance_ThirdO_ThirdO_SetD/NumOfThirdO_ThirdO_SetD)<<setw(30)<<(OverallResonance_ForthO_ThirdO_SetD/NumOfForthO_ThirdO_SetD)<<setw(30)<<(OverallResonance_FifthO_ThirdO_SetD/NumOfFifthO_ThirdO_SetD)<<setw(30)<<(OverallResonance_SixthO_ThirdO_SetD/NumOfSixthO_ThirdO_SetD)<<setw(30)<<(OverallResonance_SeventhO_ThirdO_SetD/NumOfSeventhO_ThirdO_SetD)<<setw(30)<<(OverallResonance_EighthO_ThirdO_SetD/NumOfEighthO_ThirdO_SetD)<<setw(30)<<(OverallResonance_NinthO_ThirdO_SetD/NumOfNinthO_ThirdO_SetD)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"ForthO: "<<setw(20)<<(OverallResonance_FU_ForthO_SetD/NumOfFU_ForthO_SetD)<<setw(30)<<(OverallResonance_FO_ForthO_SetD/NumOfFO_ForthO_SetD)<<setw(30)<<(OverallResonance_2O_ForthO_SetD/NumOf2O_ForthO_SetD)<<setw(30)<<(OverallResonance_ThirdO_ForthO_SetD/NumOfThirdO_ForthO_SetD)<<setw(30)<<(OverallResonance_ForthO_ForthO_SetD/NumOfForthO_ForthO_SetD)<<setw(30)<<(OverallResonance_FifthO_ForthO_SetD/NumOfFifthO_ForthO_SetD)<<setw(30)<<(OverallResonance_SixthO_ForthO_SetD/NumOfSixthO_ForthO_SetD)<<setw(30)<<(OverallResonance_SeventhO_ForthO_SetD/NumOfSeventhO_ForthO_SetD)<<setw(30)<<(OverallResonance_EighthO_ForthO_SetD/NumOfEighthO_ForthO_SetD)<<setw(30)<<(OverallResonance_NinthO_ForthO_SetD/NumOfNinthO_ForthO_SetD)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"FifthO: "<<setw(20)<<(OverallResonance_FU_FifthO_SetD/NumOfFU_FifthO_SetD)<<setw(30)<<(OverallResonance_FO_FifthO_SetD/NumOfFO_FifthO_SetD)<<setw(30)<<(OverallResonance_2O_FifthO_SetD/NumOf2O_FifthO_SetD)<<setw(30)<<(OverallResonance_ThirdO_FifthO_SetD/NumOfThirdO_FifthO_SetD)<<setw(30)<<(OverallResonance_ForthO_FifthO_SetD/NumOfForthO_FifthO_SetD)<<setw(30)<<(OverallResonance_FifthO_FifthO_SetD/NumOfFifthO_FifthO_SetD)<<setw(30)<<(OverallResonance_SixthO_FifthO_SetD/NumOfSixthO_FifthO_SetD)<<setw(30)<<(OverallResonance_SeventhO_FifthO_SetD/NumOfSeventhO_FifthO_SetD)<<setw(30)<<(OverallResonance_EighthO_FifthO_SetD/NumOfEighthO_FifthO_SetD)<<setw(30)<<(OverallResonance_NinthO_FifthO_SetD/NumOfNinthO_FifthO_SetD)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"SixthO: "<<setw(20)<<(OverallResonance_FU_SixthO_SetD/NumOfFU_SixthO_SetD)<<setw(30)<<(OverallResonance_FO_SixthO_SetD/NumOfFO_SixthO_SetD)<<setw(30)<<(OverallResonance_2O_SixthO_SetD/NumOf2O_SixthO_SetD)<<setw(30)<<(OverallResonance_ThirdO_SixthO_SetD/NumOfThirdO_SixthO_SetD)<<setw(30)<<(OverallResonance_ForthO_SixthO_SetD/NumOfForthO_SixthO_SetD)<<setw(30)<<(OverallResonance_FifthO_SixthO_SetD/NumOfFifthO_SixthO_SetD)<<setw(30)<<(OverallResonance_SixthO_SixthO_SetD/NumOfSixthO_SixthO_SetD)<<setw(30)<<(OverallResonance_SeventhO_SixthO_SetD/NumOfSeventhO_SixthO_SetD)<<setw(30)<<(OverallResonance_EighthO_SixthO_SetD/NumOfEighthO_SixthO_SetD)<<setw(30)<<(OverallResonance_NinthO_SixthO_SetD/NumOfNinthO_SixthO_SetD)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"SeventhO: "<<setw(20)<<(OverallResonance_FU_SeventhO_SetD/NumOfFU_SeventhO_SetD)<<setw(30)<<(OverallResonance_FO_SeventhO_SetD/NumOfFO_SeventhO_SetD)<<setw(30)<<(OverallResonance_2O_SeventhO_SetD/NumOf2O_SeventhO_SetD)<<setw(30)<<(OverallResonance_ThirdO_SeventhO_SetD/NumOfThirdO_SeventhO_SetD)<<setw(30)<<(OverallResonance_ForthO_SeventhO_SetD/NumOfForthO_SeventhO_SetD)<<setw(30)<<(OverallResonance_FifthO_SeventhO_SetD/NumOfFifthO_SeventhO_SetD)<<setw(30)<<(OverallResonance_SixthO_SeventhO_SetD/NumOfSixthO_SeventhO_SetD)<<setw(30)<<(OverallResonance_SeventhO_SeventhO_SetD/NumOfSeventhO_SeventhO_SetD)<<setw(30)<<(OverallResonance_EighthO_SeventhO_SetD/NumOfEighthO_SeventhO_SetD)<<setw(30)<<(OverallResonance_NinthO_SeventhO_SetD/NumOfNinthO_SeventhO_SetD)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"EighthO: "<<setw(20)<<(OverallResonance_FU_EighthO_SetD/NumOfFU_EighthO_SetD)<<setw(30)<<(OverallResonance_FO_EighthO_SetD/NumOfFO_EighthO_SetD)<<setw(30)<<(OverallResonance_2O_EighthO_SetD/NumOf2O_EighthO_SetD)<<setw(30)<<(OverallResonance_ThirdO_EighthO_SetD/NumOfThirdO_EighthO_SetD)<<setw(30)<<(OverallResonance_ForthO_EighthO_SetD/NumOfForthO_EighthO_SetD)<<setw(30)<<(OverallResonance_FifthO_EighthO_SetD/NumOfFifthO_EighthO_SetD)<<setw(30)<<(OverallResonance_SixthO_EighthO_SetD/NumOfSixthO_EighthO_SetD)<<setw(30)<<(OverallResonance_SeventhO_EighthO_SetD/NumOfSeventhO_EighthO_SetD)<<setw(30)<<(OverallResonance_EighthO_EighthO_SetD/NumOfEighthO_EighthO_SetD)<<setw(30)<<(OverallResonance_NinthO_EighthO_SetD/NumOfNinthO_EighthO_SetD)<<endl;
			
			outFile_AverageResonance<<setw(10)<<"NinthO: "<<setw(20)<<(OverallResonance_FU_NinthO_SetD/NumOfFU_NinthO_SetD)<<setw(30)<<(OverallResonance_FO_NinthO_SetD/NumOfFO_NinthO_SetD)<<setw(30)<<(OverallResonance_2O_NinthO_SetD/NumOf2O_NinthO_SetD)<<setw(30)<<(OverallResonance_ThirdO_NinthO_SetD/NumOfThirdO_NinthO_SetD)<<setw(30)<<(OverallResonance_ForthO_NinthO_SetD/NumOfForthO_NinthO_SetD)<<setw(30)<<(OverallResonance_FifthO_NinthO_SetD/NumOfFifthO_NinthO_SetD)<<setw(30)<<(OverallResonance_SixthO_NinthO_SetD/NumOfSixthO_NinthO_SetD)<<setw(30)<<(OverallResonance_SeventhO_NinthO_SetD/NumOfSeventhO_NinthO_SetD)<<setw(30)<<(OverallResonance_EighthO_NinthO_SetD/NumOfEighthO_NinthO_SetD)<<setw(30)<<(OverallResonance_NinthO_NinthO_SetD/NumOfNinthO_NinthO_SetD)<<endl;
			
		}
		outFile_AverageResonance.close();
	}
	return 0;
}




double SD_calc(double x, double mean, double N){
	
	double SD;
	
	SD = sqrt((1./(N-1.))*pow((x-mean),2));
	
	return SD
}













