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

//V5: Fixing some things with output - Looks like thirdO was done two times too many. Need to delete these and carry that down for all below. Also changed spacing from 20 to 30.
///Note: Still outputing -1 due to illegal inf when dividing by 0. This can be fixed by making if/else statments first and asigning a varible to be outputted - lots of work though
///EighthO Check must be broken, no output when there should be Pos growth models - Fixed? Typo in assignment of growth_EighthO. Running and checking...
///Program output seems good now. Add standard deviation calc for each mode combination?? -V6?
///2O output is wrong, there is a bug somewhere

//V6: Adding SD calculation for each combination along with output to set divided files. 2O output still wrong - Can't find bug.
///Note: SD calcs arent working, need if statments that are in ratio calculator 
///SD calcs now seem correct. Still need some sort of collecter (+-0.005, +-0.05 ect?) - probably V7

//V7: Adding collection conditions in main loop. Also attempting to fix 2O, thinking it might be due to the 2 in a varible name? Also adding NumOf output 
///SD calcs seem to be broken again, shouldve been smaller with bounds but are now larger? - Could just be multiple reasonances? -Attempting to fix this with an initalization of mean within each for loop for SD calcs.

//V8: Adding graphical output for plot of P0/P1 against P0 

//V9: Adding file output of freq, growth, and Teff for FO FU Fig 5 from smolec. Need to use model grid in parallel with LINA data to do this.

//V10: Fixed V9 at midnight?


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
	
	////////////Controls///////////////////

	int NumOfSets = 4; //Number of sets, usually 4 for A, B, C, D. Max is 4 sets currently
	int NumOfModels = 20412; //Total possible number of models. 20412 is total TypeII models. 16281 is Classical Cep. 15876 is RRL.
	int NumOfModes = 10; //Number of modes investigated. Max is 10. Must have the positive growth files named accordingly with prefix and suffix below!
	
	char outputFileName[50] = "ResonanceData"; //output file name for all output files from this program (ie SD or ResonanceRatio will be done automatically). Will have prefix for mode defined below as well as the set suffix.
	
	char LINAFileName[30] = "LINA_period_growth.data"; //Normally should be "LINA_period_growth.data"
	char HistoryFileName[30] = "history"; //Normally should be "history"
	char ModelGridFileName[50] = "models_linearanalysis_BLH.dat"; //Name of input model grid, used to get values for fig5, maybe others?
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
	
	double bound = 0.005; //The bound for each ratios distance from expected ratios for resonance (resonance+-bound)
	
	//Fig5 Controls//
	double Mass_ToLookFor = 0.6; 
	double L_ToLookFor = 200;
	
	bool DoPosGrowthCut = false; //If true, will only get positive growth models (Still mode being checked with will always be positive)
	bool DoBoundsCut = true; //If true, will only output models within bounds around (resonance+-bound)
	
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
	
	double SD_FU_FU_SetA = 0;
	double SD_FU_FU_SetB = 0;
	double SD_FU_FU_SetC = 0;
	double SD_FU_FU_SetD = 0;
	
	double SD_FU_FO_SetA = 0;
	double SD_FU_FO_SetB = 0;
	double SD_FU_FO_SetC = 0;
	double SD_FU_FO_SetD = 0;
	
	double SD_FU_2O_SetA = 0;
	double SD_FU_2O_SetB = 0;
	double SD_FU_2O_SetC = 0;
	double SD_FU_2O_SetD = 0;
	
	double SD_FU_ThirdO_SetA = 0;
	double SD_FU_ThirdO_SetB = 0;
	double SD_FU_ThirdO_SetC = 0;
	double SD_FU_ThirdO_SetD = 0;
	
	double SD_FU_ForthO_SetA = 0;
	double SD_FU_ForthO_SetB = 0;
	double SD_FU_ForthO_SetC = 0;
	double SD_FU_ForthO_SetD = 0;
	
	double SD_FU_FifthO_SetA = 0;
	double SD_FU_FifthO_SetB = 0;
	double SD_FU_FifthO_SetC = 0;
	double SD_FU_FifthO_SetD = 0;
	
	double SD_FU_SixthO_SetA = 0;
	double SD_FU_SixthO_SetB = 0;
	double SD_FU_SixthO_SetC = 0;
	double SD_FU_SixthO_SetD = 0;
	
	double SD_FU_SeventhO_SetA = 0;
	double SD_FU_SeventhO_SetB = 0;
	double SD_FU_SeventhO_SetC = 0;
	double SD_FU_SeventhO_SetD = 0;
	
	double SD_FU_EighthO_SetA = 0;
	double SD_FU_EighthO_SetB = 0;
	double SD_FU_EighthO_SetC = 0;
	double SD_FU_EighthO_SetD = 0;
	
	double SD_FU_NinthO_SetA = 0;
	double SD_FU_NinthO_SetB = 0;
	double SD_FU_NinthO_SetC = 0;
	double SD_FU_NinthO_SetD = 0;
	
	double var_FU_FU_SetA = 0;
	double var_FU_FU_SetB = 0;
	double var_FU_FU_SetC = 0;
	double var_FU_FU_SetD = 0;
	
	double var_FU_FO_SetA = 0;
	double var_FU_FO_SetB = 0;
	double var_FU_FO_SetC = 0;
	double var_FU_FO_SetD = 0;
	
	double var_FU_2O_SetA = 0;
	double var_FU_2O_SetB = 0;
	double var_FU_2O_SetC = 0;
	double var_FU_2O_SetD = 0;
	
	double var_FU_ThirdO_SetA = 0;
	double var_FU_ThirdO_SetB = 0;
	double var_FU_ThirdO_SetC = 0;
	double var_FU_ThirdO_SetD = 0;
	
	double var_FU_ForthO_SetA = 0;
	double var_FU_ForthO_SetB = 0;
	double var_FU_ForthO_SetC = 0;
	double var_FU_ForthO_SetD = 0;
	
	double var_FU_FifthO_SetA = 0;
	double var_FU_FifthO_SetB = 0;
	double var_FU_FifthO_SetC = 0;
	double var_FU_FifthO_SetD = 0;
	
	double var_FU_SixthO_SetA = 0;
	double var_FU_SixthO_SetB = 0;
	double var_FU_SixthO_SetC = 0;
	double var_FU_SixthO_SetD = 0;
	
	double var_FU_SeventhO_SetA = 0;
	double var_FU_SeventhO_SetB = 0;
	double var_FU_SeventhO_SetC = 0;
	double var_FU_SeventhO_SetD = 0;
	
	double var_FU_EighthO_SetA = 0;
	double var_FU_EighthO_SetB = 0;
	double var_FU_EighthO_SetC = 0;
	double var_FU_EighthO_SetD = 0;
	
	double var_FU_NinthO_SetA = 0;
	double var_FU_NinthO_SetB = 0;
	double var_FU_NinthO_SetC = 0;
	double var_FU_NinthO_SetD = 0;
	
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
	
	bool IsSelected_FU_SetA[nArray];
	bool IsSelected_FU_SetB[nArray];
	bool IsSelected_FU_SetC[nArray];
	bool IsSelected_FU_SetD[nArray];
	
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
	
	double SD_FO_FU_SetA = 0;
	double SD_FO_FU_SetB = 0;
	double SD_FO_FU_SetC = 0;
	double SD_FO_FU_SetD = 0;
	
	double SD_FO_FO_SetA = 0;
	double SD_FO_FO_SetB = 0;
	double SD_FO_FO_SetC = 0;
	double SD_FO_FO_SetD = 0;
	
	double SD_FO_2O_SetA = 0;
	double SD_FO_2O_SetB = 0;
	double SD_FO_2O_SetC = 0;
	double SD_FO_2O_SetD = 0;
	
	double SD_FO_ThirdO_SetA = 0;
	double SD_FO_ThirdO_SetB = 0;
	double SD_FO_ThirdO_SetC = 0;
	double SD_FO_ThirdO_SetD = 0;
	
	double SD_FO_ForthO_SetA = 0;
	double SD_FO_ForthO_SetB = 0;
	double SD_FO_ForthO_SetC = 0;
	double SD_FO_ForthO_SetD = 0;
	
	double SD_FO_FifthO_SetA = 0;
	double SD_FO_FifthO_SetB = 0;
	double SD_FO_FifthO_SetC = 0;
	double SD_FO_FifthO_SetD = 0;
	
	double SD_FO_SixthO_SetA = 0;
	double SD_FO_SixthO_SetB = 0;
	double SD_FO_SixthO_SetC = 0;
	double SD_FO_SixthO_SetD = 0;
	
	double SD_FO_SeventhO_SetA = 0;
	double SD_FO_SeventhO_SetB = 0;
	double SD_FO_SeventhO_SetC = 0;
	double SD_FO_SeventhO_SetD = 0;
	
	double SD_FO_EighthO_SetA = 0;
	double SD_FO_EighthO_SetB = 0;
	double SD_FO_EighthO_SetC = 0;
	double SD_FO_EighthO_SetD = 0;
	
	double SD_FO_NinthO_SetA = 0;
	double SD_FO_NinthO_SetB = 0;
	double SD_FO_NinthO_SetC = 0;
	double SD_FO_NinthO_SetD = 0;
	
	double var_FO_FU_SetA = 0;
	double var_FO_FU_SetB = 0;
	double var_FO_FU_SetC = 0;
	double var_FO_FU_SetD = 0;
	
	double var_FO_FO_SetA = 0;
	double var_FO_FO_SetB = 0;
	double var_FO_FO_SetC = 0;
	double var_FO_FO_SetD = 0;
	
	double var_FO_2O_SetA = 0;
	double var_FO_2O_SetB = 0;
	double var_FO_2O_SetC = 0;
	double var_FO_2O_SetD = 0;
	
	double var_FO_ThirdO_SetA = 0;
	double var_FO_ThirdO_SetB = 0;
	double var_FO_ThirdO_SetC = 0;
	double var_FO_ThirdO_SetD = 0;
	
	double var_FO_ForthO_SetA = 0;
	double var_FO_ForthO_SetB = 0;
	double var_FO_ForthO_SetC = 0;
	double var_FO_ForthO_SetD = 0;
	
	double var_FO_FifthO_SetA = 0;
	double var_FO_FifthO_SetB = 0;
	double var_FO_FifthO_SetC = 0;
	double var_FO_FifthO_SetD = 0;
	
	double var_FO_SixthO_SetA = 0;
	double var_FO_SixthO_SetB = 0;
	double var_FO_SixthO_SetC = 0;
	double var_FO_SixthO_SetD = 0;
	
	double var_FO_SeventhO_SetA = 0;
	double var_FO_SeventhO_SetB = 0;
	double var_FO_SeventhO_SetC = 0;
	double var_FO_SeventhO_SetD = 0;
	
	double var_FO_EighthO_SetA = 0;
	double var_FO_EighthO_SetB = 0;
	double var_FO_EighthO_SetC = 0;
	double var_FO_EighthO_SetD = 0;
	
	double var_FO_NinthO_SetA = 0;
	double var_FO_NinthO_SetB = 0;
	double var_FO_NinthO_SetC = 0;
	double var_FO_NinthO_SetD = 0;
	
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
	
	bool IsSelected_FO_SetA[nArray];
	bool IsSelected_FO_SetB[nArray];
	bool IsSelected_FO_SetC[nArray];
	bool IsSelected_FO_SetD[nArray];
	
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
	
	double SD_2O_FU_SetA = 0;
	double SD_2O_FU_SetB = 0;
	double SD_2O_FU_SetC = 0;
	double SD_2O_FU_SetD = 0;
	
	double SD_2O_FO_SetA = 0;
	double SD_2O_FO_SetB = 0;
	double SD_2O_FO_SetC = 0;
	double SD_2O_FO_SetD = 0;
	
	double SD_2O_2O_SetA = 0;
	double SD_2O_2O_SetB = 0;
	double SD_2O_2O_SetC = 0;
	double SD_2O_2O_SetD = 0;
	
	double SD_2O_ThirdO_SetA = 0;
	double SD_2O_ThirdO_SetB = 0;
	double SD_2O_ThirdO_SetC = 0;
	double SD_2O_ThirdO_SetD = 0;
	
	double SD_2O_ForthO_SetA = 0;
	double SD_2O_ForthO_SetB = 0;
	double SD_2O_ForthO_SetC = 0;
	double SD_2O_ForthO_SetD = 0;
	
	double SD_2O_FifthO_SetA = 0;
	double SD_2O_FifthO_SetB = 0;
	double SD_2O_FifthO_SetC = 0;
	double SD_2O_FifthO_SetD = 0;
	
	double SD_2O_SixthO_SetA = 0;
	double SD_2O_SixthO_SetB = 0;
	double SD_2O_SixthO_SetC = 0;
	double SD_2O_SixthO_SetD = 0;
	
	double SD_2O_SeventhO_SetA = 0;
	double SD_2O_SeventhO_SetB = 0;
	double SD_2O_SeventhO_SetC = 0;
	double SD_2O_SeventhO_SetD = 0;
	
	double SD_2O_EighthO_SetA = 0;
	double SD_2O_EighthO_SetB = 0;
	double SD_2O_EighthO_SetC = 0;
	double SD_2O_EighthO_SetD = 0;
	
	double SD_2O_NinthO_SetA = 0;
	double SD_2O_NinthO_SetB = 0;
	double SD_2O_NinthO_SetC = 0;
	double SD_2O_NinthO_SetD = 0;
	
	double var_2O_FU_SetA = 0;
	double var_2O_FU_SetB = 0;
	double var_2O_FU_SetC = 0;
	double var_2O_FU_SetD = 0;
	
	double var_2O_FO_SetA = 0;
	double var_2O_FO_SetB = 0;
	double var_2O_FO_SetC = 0;
	double var_2O_FO_SetD = 0;
	
	double var_2O_2O_SetA = 0;
	double var_2O_2O_SetB = 0;
	double var_2O_2O_SetC = 0;
	double var_2O_2O_SetD = 0;
	
	double var_2O_ThirdO_SetA = 0;
	double var_2O_ThirdO_SetB = 0;
	double var_2O_ThirdO_SetC = 0;
	double var_2O_ThirdO_SetD = 0;
	
	double var_2O_ForthO_SetA = 0;
	double var_2O_ForthO_SetB = 0;
	double var_2O_ForthO_SetC = 0;
	double var_2O_ForthO_SetD = 0;
	
	double var_2O_FifthO_SetA = 0;
	double var_2O_FifthO_SetB = 0;
	double var_2O_FifthO_SetC = 0;
	double var_2O_FifthO_SetD = 0;
	
	double var_2O_SixthO_SetA = 0;
	double var_2O_SixthO_SetB = 0;
	double var_2O_SixthO_SetC = 0;
	double var_2O_SixthO_SetD = 0;
	
	double var_2O_SeventhO_SetA = 0;
	double var_2O_SeventhO_SetB = 0;
	double var_2O_SeventhO_SetC = 0;
	double var_2O_SeventhO_SetD = 0;
	
	double var_2O_EighthO_SetA = 0;
	double var_2O_EighthO_SetB = 0;
	double var_2O_EighthO_SetC = 0;
	double var_2O_EighthO_SetD = 0;
	
	double var_2O_NinthO_SetA = 0;
	double var_2O_NinthO_SetB = 0;
	double var_2O_NinthO_SetC = 0;
	double var_2O_NinthO_SetD = 0;
	
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
	
	bool IsSelected_2O_SetA[nArray];
	bool IsSelected_2O_SetB[nArray];
	bool IsSelected_2O_SetC[nArray];
	bool IsSelected_2O_SetD[nArray];
	
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
	
	double SD_ThirdO_FU_SetA = 0;
	double SD_ThirdO_FU_SetB = 0;
	double SD_ThirdO_FU_SetC = 0;
	double SD_ThirdO_FU_SetD = 0;
	
	double SD_ThirdO_FO_SetA = 0;
	double SD_ThirdO_FO_SetB = 0;
	double SD_ThirdO_FO_SetC = 0;
	double SD_ThirdO_FO_SetD = 0;
	
	double SD_ThirdO_2O_SetA = 0;
	double SD_ThirdO_2O_SetB = 0;
	double SD_ThirdO_2O_SetC = 0;
	double SD_ThirdO_2O_SetD = 0;
	
	double SD_ThirdO_ThirdO_SetA = 0;
	double SD_ThirdO_ThirdO_SetB = 0;
	double SD_ThirdO_ThirdO_SetC = 0;
	double SD_ThirdO_ThirdO_SetD = 0;
	
	double SD_ThirdO_ForthO_SetA = 0;
	double SD_ThirdO_ForthO_SetB = 0;
	double SD_ThirdO_ForthO_SetC = 0;
	double SD_ThirdO_ForthO_SetD = 0;
	
	double SD_ThirdO_FifthO_SetA = 0;
	double SD_ThirdO_FifthO_SetB = 0;
	double SD_ThirdO_FifthO_SetC = 0;
	double SD_ThirdO_FifthO_SetD = 0;
	
	double SD_ThirdO_SixthO_SetA = 0;
	double SD_ThirdO_SixthO_SetB = 0;
	double SD_ThirdO_SixthO_SetC = 0;
	double SD_ThirdO_SixthO_SetD = 0;
	
	double SD_ThirdO_SeventhO_SetA = 0;
	double SD_ThirdO_SeventhO_SetB = 0;
	double SD_ThirdO_SeventhO_SetC = 0;
	double SD_ThirdO_SeventhO_SetD = 0;
	
	double SD_ThirdO_EighthO_SetA = 0;
	double SD_ThirdO_EighthO_SetB = 0;
	double SD_ThirdO_EighthO_SetC = 0;
	double SD_ThirdO_EighthO_SetD = 0;
	
	double SD_ThirdO_NinthO_SetA = 0;
	double SD_ThirdO_NinthO_SetB = 0;
	double SD_ThirdO_NinthO_SetC = 0;
	double SD_ThirdO_NinthO_SetD = 0;
	
	double var_ThirdO_FU_SetA = 0;
	double var_ThirdO_FU_SetB = 0;
	double var_ThirdO_FU_SetC = 0;
	double var_ThirdO_FU_SetD = 0;
	
	double var_ThirdO_FO_SetA = 0;
	double var_ThirdO_FO_SetB = 0;
	double var_ThirdO_FO_SetC = 0;
	double var_ThirdO_FO_SetD = 0;
	
	double var_ThirdO_2O_SetA = 0;
	double var_ThirdO_2O_SetB = 0;
	double var_ThirdO_2O_SetC = 0;
	double var_ThirdO_2O_SetD = 0;
	
	double var_ThirdO_ThirdO_SetA = 0;
	double var_ThirdO_ThirdO_SetB = 0;
	double var_ThirdO_ThirdO_SetC = 0;
	double var_ThirdO_ThirdO_SetD = 0;
	
	double var_ThirdO_ForthO_SetA = 0;
	double var_ThirdO_ForthO_SetB = 0;
	double var_ThirdO_ForthO_SetC = 0;
	double var_ThirdO_ForthO_SetD = 0;
	
	double var_ThirdO_FifthO_SetA = 0;
	double var_ThirdO_FifthO_SetB = 0;
	double var_ThirdO_FifthO_SetC = 0;
	double var_ThirdO_FifthO_SetD = 0;
	
	double var_ThirdO_SixthO_SetA = 0;
	double var_ThirdO_SixthO_SetB = 0;
	double var_ThirdO_SixthO_SetC = 0;
	double var_ThirdO_SixthO_SetD = 0;
	
	double var_ThirdO_SeventhO_SetA = 0;
	double var_ThirdO_SeventhO_SetB = 0;
	double var_ThirdO_SeventhO_SetC = 0;
	double var_ThirdO_SeventhO_SetD = 0;
	
	double var_ThirdO_EighthO_SetA = 0;
	double var_ThirdO_EighthO_SetB = 0;
	double var_ThirdO_EighthO_SetC = 0;
	double var_ThirdO_EighthO_SetD = 0;
	
	double var_ThirdO_NinthO_SetA = 0;
	double var_ThirdO_NinthO_SetB = 0;
	double var_ThirdO_NinthO_SetC = 0;
	double var_ThirdO_NinthO_SetD = 0;
	
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
	
	bool IsSelected_ThirdO_SetA[nArray];
	bool IsSelected_ThirdO_SetB[nArray];
	bool IsSelected_ThirdO_SetC[nArray];
	bool IsSelected_ThirdO_SetD[nArray];
	
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
	
	double SD_ForthO_FU_SetA = 0;
	double SD_ForthO_FU_SetB = 0;
	double SD_ForthO_FU_SetC = 0;
	double SD_ForthO_FU_SetD = 0;
	
	double SD_ForthO_FO_SetA = 0;
	double SD_ForthO_FO_SetB = 0;
	double SD_ForthO_FO_SetC = 0;
	double SD_ForthO_FO_SetD = 0;
	
	double SD_ForthO_2O_SetA = 0;
	double SD_ForthO_2O_SetB = 0;
	double SD_ForthO_2O_SetC = 0;
	double SD_ForthO_2O_SetD = 0;
	
	double SD_ForthO_ThirdO_SetA = 0;
	double SD_ForthO_ThirdO_SetB = 0;
	double SD_ForthO_ThirdO_SetC = 0;
	double SD_ForthO_ThirdO_SetD = 0;
	
	double SD_ForthO_ForthO_SetA = 0;
	double SD_ForthO_ForthO_SetB = 0;
	double SD_ForthO_ForthO_SetC = 0;
	double SD_ForthO_ForthO_SetD = 0;
	
	double SD_ForthO_FifthO_SetA = 0;
	double SD_ForthO_FifthO_SetB = 0;
	double SD_ForthO_FifthO_SetC = 0;
	double SD_ForthO_FifthO_SetD = 0;
	
	double SD_ForthO_SixthO_SetA = 0;
	double SD_ForthO_SixthO_SetB = 0;
	double SD_ForthO_SixthO_SetC = 0;
	double SD_ForthO_SixthO_SetD = 0;
	
	double SD_ForthO_SeventhO_SetA = 0;
	double SD_ForthO_SeventhO_SetB = 0;
	double SD_ForthO_SeventhO_SetC = 0;
	double SD_ForthO_SeventhO_SetD = 0;
	
	double SD_ForthO_EighthO_SetA = 0;
	double SD_ForthO_EighthO_SetB = 0;
	double SD_ForthO_EighthO_SetC = 0;
	double SD_ForthO_EighthO_SetD = 0;
	
	double SD_ForthO_NinthO_SetA = 0;
	double SD_ForthO_NinthO_SetB = 0;
	double SD_ForthO_NinthO_SetC = 0;
	double SD_ForthO_NinthO_SetD = 0;
	
	double var_ForthO_FU_SetA = 0;
	double var_ForthO_FU_SetB = 0;
	double var_ForthO_FU_SetC = 0;
	double var_ForthO_FU_SetD = 0;
	
	double var_ForthO_FO_SetA = 0;
	double var_ForthO_FO_SetB = 0;
	double var_ForthO_FO_SetC = 0;
	double var_ForthO_FO_SetD = 0;
	
	double var_ForthO_2O_SetA = 0;
	double var_ForthO_2O_SetB = 0;
	double var_ForthO_2O_SetC = 0;
	double var_ForthO_2O_SetD = 0;
	
	double var_ForthO_ThirdO_SetA = 0;
	double var_ForthO_ThirdO_SetB = 0;
	double var_ForthO_ThirdO_SetC = 0;
	double var_ForthO_ThirdO_SetD = 0;
	
	double var_ForthO_ForthO_SetA = 0;
	double var_ForthO_ForthO_SetB = 0;
	double var_ForthO_ForthO_SetC = 0;
	double var_ForthO_ForthO_SetD = 0;
	
	double var_ForthO_FifthO_SetA = 0;
	double var_ForthO_FifthO_SetB = 0;
	double var_ForthO_FifthO_SetC = 0;
	double var_ForthO_FifthO_SetD = 0;
	
	double var_ForthO_SixthO_SetA = 0;
	double var_ForthO_SixthO_SetB = 0;
	double var_ForthO_SixthO_SetC = 0;
	double var_ForthO_SixthO_SetD = 0;
	
	double var_ForthO_SeventhO_SetA = 0;
	double var_ForthO_SeventhO_SetB = 0;
	double var_ForthO_SeventhO_SetC = 0;
	double var_ForthO_SeventhO_SetD = 0;
	
	double var_ForthO_EighthO_SetA = 0;
	double var_ForthO_EighthO_SetB = 0;
	double var_ForthO_EighthO_SetC = 0;
	double var_ForthO_EighthO_SetD = 0;
	
	double var_ForthO_NinthO_SetA = 0;
	double var_ForthO_NinthO_SetB = 0;
	double var_ForthO_NinthO_SetC = 0;
	double var_ForthO_NinthO_SetD = 0;
	
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
	
	bool IsSelected_ForthO_SetA[nArray];
	bool IsSelected_ForthO_SetB[nArray];
	bool IsSelected_ForthO_SetC[nArray];
	bool IsSelected_ForthO_SetD[nArray];
	
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
	
	double SD_FifthO_FU_SetA = 0;
	double SD_FifthO_FU_SetB = 0;
	double SD_FifthO_FU_SetC = 0;
	double SD_FifthO_FU_SetD = 0;
	
	double SD_FifthO_FO_SetA = 0;
	double SD_FifthO_FO_SetB = 0;
	double SD_FifthO_FO_SetC = 0;
	double SD_FifthO_FO_SetD = 0;
	
	double SD_FifthO_2O_SetA = 0;
	double SD_FifthO_2O_SetB = 0;
	double SD_FifthO_2O_SetC = 0;
	double SD_FifthO_2O_SetD = 0;
	
	double SD_FifthO_ThirdO_SetA = 0;
	double SD_FifthO_ThirdO_SetB = 0;
	double SD_FifthO_ThirdO_SetC = 0;
	double SD_FifthO_ThirdO_SetD = 0;
	
	double SD_FifthO_ForthO_SetA = 0;
	double SD_FifthO_ForthO_SetB = 0;
	double SD_FifthO_ForthO_SetC = 0;
	double SD_FifthO_ForthO_SetD = 0;
	
	double SD_FifthO_FifthO_SetA = 0;
	double SD_FifthO_FifthO_SetB = 0;
	double SD_FifthO_FifthO_SetC = 0;
	double SD_FifthO_FifthO_SetD = 0;
	
	double SD_FifthO_SixthO_SetA = 0;
	double SD_FifthO_SixthO_SetB = 0;
	double SD_FifthO_SixthO_SetC = 0;
	double SD_FifthO_SixthO_SetD = 0;
	
	double SD_FifthO_SeventhO_SetA = 0;
	double SD_FifthO_SeventhO_SetB = 0;
	double SD_FifthO_SeventhO_SetC = 0;
	double SD_FifthO_SeventhO_SetD = 0;
	
	double SD_FifthO_EighthO_SetA = 0;
	double SD_FifthO_EighthO_SetB = 0;
	double SD_FifthO_EighthO_SetC = 0;
	double SD_FifthO_EighthO_SetD = 0;
	
	double SD_FifthO_NinthO_SetA = 0;
	double SD_FifthO_NinthO_SetB = 0;
	double SD_FifthO_NinthO_SetC = 0;
	double SD_FifthO_NinthO_SetD = 0;
	
	double var_FifthO_FU_SetA = 0;
	double var_FifthO_FU_SetB = 0;
	double var_FifthO_FU_SetC = 0;
	double var_FifthO_FU_SetD = 0;
	
	double var_FifthO_FO_SetA = 0;
	double var_FifthO_FO_SetB = 0;
	double var_FifthO_FO_SetC = 0;
	double var_FifthO_FO_SetD = 0;
	
	double var_FifthO_2O_SetA = 0;
	double var_FifthO_2O_SetB = 0;
	double var_FifthO_2O_SetC = 0;
	double var_FifthO_2O_SetD = 0;
	
	double var_FifthO_ThirdO_SetA = 0;
	double var_FifthO_ThirdO_SetB = 0;
	double var_FifthO_ThirdO_SetC = 0;
	double var_FifthO_ThirdO_SetD = 0;
	
	double var_FifthO_ForthO_SetA = 0;
	double var_FifthO_ForthO_SetB = 0;
	double var_FifthO_ForthO_SetC = 0;
	double var_FifthO_ForthO_SetD = 0;
	
	double var_FifthO_FifthO_SetA = 0;
	double var_FifthO_FifthO_SetB = 0;
	double var_FifthO_FifthO_SetC = 0;
	double var_FifthO_FifthO_SetD = 0;
	
	double var_FifthO_SixthO_SetA = 0;
	double var_FifthO_SixthO_SetB = 0;
	double var_FifthO_SixthO_SetC = 0;
	double var_FifthO_SixthO_SetD = 0;
	
	double var_FifthO_SeventhO_SetA = 0;
	double var_FifthO_SeventhO_SetB = 0;
	double var_FifthO_SeventhO_SetC = 0;
	double var_FifthO_SeventhO_SetD = 0;
	
	double var_FifthO_EighthO_SetA = 0;
	double var_FifthO_EighthO_SetB = 0;
	double var_FifthO_EighthO_SetC = 0;
	double var_FifthO_EighthO_SetD = 0;
	
	double var_FifthO_NinthO_SetA = 0;
	double var_FifthO_NinthO_SetB = 0;
	double var_FifthO_NinthO_SetC = 0;
	double var_FifthO_NinthO_SetD = 0;
	
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
	
	bool IsSelected_FifthO_SetA[nArray];
	bool IsSelected_FifthO_SetB[nArray];
	bool IsSelected_FifthO_SetC[nArray];
	bool IsSelected_FifthO_SetD[nArray];
	
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
	
	double SD_SixthO_FU_SetA = 0;
	double SD_SixthO_FU_SetB = 0;
	double SD_SixthO_FU_SetC = 0;
	double SD_SixthO_FU_SetD = 0;
	
	double SD_SixthO_FO_SetA = 0;
	double SD_SixthO_FO_SetB = 0;
	double SD_SixthO_FO_SetC = 0;
	double SD_SixthO_FO_SetD = 0;
	
	double SD_SixthO_2O_SetA = 0;
	double SD_SixthO_2O_SetB = 0;
	double SD_SixthO_2O_SetC = 0;
	double SD_SixthO_2O_SetD = 0;
	
	double SD_SixthO_ThirdO_SetA = 0;
	double SD_SixthO_ThirdO_SetB = 0;
	double SD_SixthO_ThirdO_SetC = 0;
	double SD_SixthO_ThirdO_SetD = 0;
	
	double SD_SixthO_ForthO_SetA = 0;
	double SD_SixthO_ForthO_SetB = 0;
	double SD_SixthO_ForthO_SetC = 0;
	double SD_SixthO_ForthO_SetD = 0;
	
	double SD_SixthO_FifthO_SetA = 0;
	double SD_SixthO_FifthO_SetB = 0;
	double SD_SixthO_FifthO_SetC = 0;
	double SD_SixthO_FifthO_SetD = 0;
	
	double SD_SixthO_SixthO_SetA = 0;
	double SD_SixthO_SixthO_SetB = 0;
	double SD_SixthO_SixthO_SetC = 0;
	double SD_SixthO_SixthO_SetD = 0;
	
	double SD_SixthO_SeventhO_SetA = 0;
	double SD_SixthO_SeventhO_SetB = 0;
	double SD_SixthO_SeventhO_SetC = 0;
	double SD_SixthO_SeventhO_SetD = 0;
	
	double SD_SixthO_EighthO_SetA = 0;
	double SD_SixthO_EighthO_SetB = 0;
	double SD_SixthO_EighthO_SetC = 0;
	double SD_SixthO_EighthO_SetD = 0;
	
	double SD_SixthO_NinthO_SetA = 0;
	double SD_SixthO_NinthO_SetB = 0;
	double SD_SixthO_NinthO_SetC = 0;
	double SD_SixthO_NinthO_SetD = 0;
	
	double var_SixthO_FU_SetA = 0;
	double var_SixthO_FU_SetB = 0;
	double var_SixthO_FU_SetC = 0;
	double var_SixthO_FU_SetD = 0;
	
	double var_SixthO_FO_SetA = 0;
	double var_SixthO_FO_SetB = 0;
	double var_SixthO_FO_SetC = 0;
	double var_SixthO_FO_SetD = 0;
	
	double var_SixthO_2O_SetA = 0;
	double var_SixthO_2O_SetB = 0;
	double var_SixthO_2O_SetC = 0;
	double var_SixthO_2O_SetD = 0;
	
	double var_SixthO_ThirdO_SetA = 0;
	double var_SixthO_ThirdO_SetB = 0;
	double var_SixthO_ThirdO_SetC = 0;
	double var_SixthO_ThirdO_SetD = 0;
	
	double var_SixthO_ForthO_SetA = 0;
	double var_SixthO_ForthO_SetB = 0;
	double var_SixthO_ForthO_SetC = 0;
	double var_SixthO_ForthO_SetD = 0;
	
	double var_SixthO_FifthO_SetA = 0;
	double var_SixthO_FifthO_SetB = 0;
	double var_SixthO_FifthO_SetC = 0;
	double var_SixthO_FifthO_SetD = 0;
	
	double var_SixthO_SixthO_SetA = 0;
	double var_SixthO_SixthO_SetB = 0;
	double var_SixthO_SixthO_SetC = 0;
	double var_SixthO_SixthO_SetD = 0;
	
	double var_SixthO_SeventhO_SetA = 0;
	double var_SixthO_SeventhO_SetB = 0;
	double var_SixthO_SeventhO_SetC = 0;
	double var_SixthO_SeventhO_SetD = 0;
	
	double var_SixthO_EighthO_SetA = 0;
	double var_SixthO_EighthO_SetB = 0;
	double var_SixthO_EighthO_SetC = 0;
	double var_SixthO_EighthO_SetD = 0;
	
	double var_SixthO_NinthO_SetA = 0;
	double var_SixthO_NinthO_SetB = 0;
	double var_SixthO_NinthO_SetC = 0;
	double var_SixthO_NinthO_SetD = 0;
	
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
	
	bool IsSelected_SixthO_SetA[nArray];
	bool IsSelected_SixthO_SetB[nArray];
	bool IsSelected_SixthO_SetC[nArray];
	bool IsSelected_SixthO_SetD[nArray];
	
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
	
	double SD_SeventhO_FU_SetA = 0;
	double SD_SeventhO_FU_SetB = 0;
	double SD_SeventhO_FU_SetC = 0;
	double SD_SeventhO_FU_SetD = 0;
	
	double SD_SeventhO_FO_SetA = 0;
	double SD_SeventhO_FO_SetB = 0;
	double SD_SeventhO_FO_SetC = 0;
	double SD_SeventhO_FO_SetD = 0;
	
	double SD_SeventhO_2O_SetA = 0;
	double SD_SeventhO_2O_SetB = 0;
	double SD_SeventhO_2O_SetC = 0;
	double SD_SeventhO_2O_SetD = 0;
	
	double SD_SeventhO_ThirdO_SetA = 0;
	double SD_SeventhO_ThirdO_SetB = 0;
	double SD_SeventhO_ThirdO_SetC = 0;
	double SD_SeventhO_ThirdO_SetD = 0;
	
	double SD_SeventhO_ForthO_SetA = 0;
	double SD_SeventhO_ForthO_SetB = 0;
	double SD_SeventhO_ForthO_SetC = 0;
	double SD_SeventhO_ForthO_SetD = 0;
	
	double SD_SeventhO_FifthO_SetA = 0;
	double SD_SeventhO_FifthO_SetB = 0;
	double SD_SeventhO_FifthO_SetC = 0;
	double SD_SeventhO_FifthO_SetD = 0;
	
	double SD_SeventhO_SixthO_SetA = 0;
	double SD_SeventhO_SixthO_SetB = 0;
	double SD_SeventhO_SixthO_SetC = 0;
	double SD_SeventhO_SixthO_SetD = 0;
	
	double SD_SeventhO_SeventhO_SetA = 0;
	double SD_SeventhO_SeventhO_SetB = 0;
	double SD_SeventhO_SeventhO_SetC = 0;
	double SD_SeventhO_SeventhO_SetD = 0;
	
	double SD_SeventhO_EighthO_SetA = 0;
	double SD_SeventhO_EighthO_SetB = 0;
	double SD_SeventhO_EighthO_SetC = 0;
	double SD_SeventhO_EighthO_SetD = 0;
	
	double SD_SeventhO_NinthO_SetA = 0;
	double SD_SeventhO_NinthO_SetB = 0;
	double SD_SeventhO_NinthO_SetC = 0;
	double SD_SeventhO_NinthO_SetD = 0;
	
	double var_SeventhO_FU_SetA = 0;
	double var_SeventhO_FU_SetB = 0;
	double var_SeventhO_FU_SetC = 0;
	double var_SeventhO_FU_SetD = 0;
	
	double var_SeventhO_FO_SetA = 0;
	double var_SeventhO_FO_SetB = 0;
	double var_SeventhO_FO_SetC = 0;
	double var_SeventhO_FO_SetD = 0;
	
	double var_SeventhO_2O_SetA = 0;
	double var_SeventhO_2O_SetB = 0;
	double var_SeventhO_2O_SetC = 0;
	double var_SeventhO_2O_SetD = 0;
	
	double var_SeventhO_ThirdO_SetA = 0;
	double var_SeventhO_ThirdO_SetB = 0;
	double var_SeventhO_ThirdO_SetC = 0;
	double var_SeventhO_ThirdO_SetD = 0;
	
	double var_SeventhO_ForthO_SetA = 0;
	double var_SeventhO_ForthO_SetB = 0;
	double var_SeventhO_ForthO_SetC = 0;
	double var_SeventhO_ForthO_SetD = 0;
	
	double var_SeventhO_FifthO_SetA = 0;
	double var_SeventhO_FifthO_SetB = 0;
	double var_SeventhO_FifthO_SetC = 0;
	double var_SeventhO_FifthO_SetD = 0;
	
	double var_SeventhO_SixthO_SetA = 0;
	double var_SeventhO_SixthO_SetB = 0;
	double var_SeventhO_SixthO_SetC = 0;
	double var_SeventhO_SixthO_SetD = 0;
	
	double var_SeventhO_SeventhO_SetA = 0;
	double var_SeventhO_SeventhO_SetB = 0;
	double var_SeventhO_SeventhO_SetC = 0;
	double var_SeventhO_SeventhO_SetD = 0;
	
	double var_SeventhO_EighthO_SetA = 0;
	double var_SeventhO_EighthO_SetB = 0;
	double var_SeventhO_EighthO_SetC = 0;
	double var_SeventhO_EighthO_SetD = 0;
	
	double var_SeventhO_NinthO_SetA = 0;
	double var_SeventhO_NinthO_SetB = 0;
	double var_SeventhO_NinthO_SetC = 0;
	double var_SeventhO_NinthO_SetD = 0;
	
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
	
	bool IsSelected_SeventhO_SetA[nArray];
	bool IsSelected_SeventhO_SetB[nArray];
	bool IsSelected_SeventhO_SetC[nArray];
	bool IsSelected_SeventhO_SetD[nArray];
	
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
	
	double SD_EighthO_FU_SetA = 0;
	double SD_EighthO_FU_SetB = 0;
	double SD_EighthO_FU_SetC = 0;
	double SD_EighthO_FU_SetD = 0;
	
	double SD_EighthO_FO_SetA = 0;
	double SD_EighthO_FO_SetB = 0;
	double SD_EighthO_FO_SetC = 0;
	double SD_EighthO_FO_SetD = 0;
	
	double SD_EighthO_2O_SetA = 0;
	double SD_EighthO_2O_SetB = 0;
	double SD_EighthO_2O_SetC = 0;
	double SD_EighthO_2O_SetD = 0;
	
	double SD_EighthO_ThirdO_SetA = 0;
	double SD_EighthO_ThirdO_SetB = 0;
	double SD_EighthO_ThirdO_SetC = 0;
	double SD_EighthO_ThirdO_SetD = 0;
	
	double SD_EighthO_ForthO_SetA = 0;
	double SD_EighthO_ForthO_SetB = 0;
	double SD_EighthO_ForthO_SetC = 0;
	double SD_EighthO_ForthO_SetD = 0;
	
	double SD_EighthO_FifthO_SetA = 0;
	double SD_EighthO_FifthO_SetB = 0;
	double SD_EighthO_FifthO_SetC = 0;
	double SD_EighthO_FifthO_SetD = 0;
	
	double SD_EighthO_SixthO_SetA = 0;
	double SD_EighthO_SixthO_SetB = 0;
	double SD_EighthO_SixthO_SetC = 0;
	double SD_EighthO_SixthO_SetD = 0;
	
	double SD_EighthO_SeventhO_SetA = 0;
	double SD_EighthO_SeventhO_SetB = 0;
	double SD_EighthO_SeventhO_SetC = 0;
	double SD_EighthO_SeventhO_SetD = 0;
	
	double SD_EighthO_EighthO_SetA = 0;
	double SD_EighthO_EighthO_SetB = 0;
	double SD_EighthO_EighthO_SetC = 0;
	double SD_EighthO_EighthO_SetD = 0;
	
	double SD_EighthO_NinthO_SetA = 0;
	double SD_EighthO_NinthO_SetB = 0;
	double SD_EighthO_NinthO_SetC = 0;
	double SD_EighthO_NinthO_SetD = 0;
	
	double var_EighthO_FU_SetA = 0;
	double var_EighthO_FU_SetB = 0;
	double var_EighthO_FU_SetC = 0;
	double var_EighthO_FU_SetD = 0;
	
	double var_EighthO_FO_SetA = 0;
	double var_EighthO_FO_SetB = 0;
	double var_EighthO_FO_SetC = 0;
	double var_EighthO_FO_SetD = 0;
	
	double var_EighthO_2O_SetA = 0;
	double var_EighthO_2O_SetB = 0;
	double var_EighthO_2O_SetC = 0;
	double var_EighthO_2O_SetD = 0;
	
	double var_EighthO_ThirdO_SetA = 0;
	double var_EighthO_ThirdO_SetB = 0;
	double var_EighthO_ThirdO_SetC = 0;
	double var_EighthO_ThirdO_SetD = 0;
	
	double var_EighthO_ForthO_SetA = 0;
	double var_EighthO_ForthO_SetB = 0;
	double var_EighthO_ForthO_SetC = 0;
	double var_EighthO_ForthO_SetD = 0;
	
	double var_EighthO_FifthO_SetA = 0;
	double var_EighthO_FifthO_SetB = 0;
	double var_EighthO_FifthO_SetC = 0;
	double var_EighthO_FifthO_SetD = 0;
	
	double var_EighthO_SixthO_SetA = 0;
	double var_EighthO_SixthO_SetB = 0;
	double var_EighthO_SixthO_SetC = 0;
	double var_EighthO_SixthO_SetD = 0;
	
	double var_EighthO_SeventhO_SetA = 0;
	double var_EighthO_SeventhO_SetB = 0;
	double var_EighthO_SeventhO_SetC = 0;
	double var_EighthO_SeventhO_SetD = 0;
	
	double var_EighthO_EighthO_SetA = 0;
	double var_EighthO_EighthO_SetB = 0;
	double var_EighthO_EighthO_SetC = 0;
	double var_EighthO_EighthO_SetD = 0;
	
	double var_EighthO_NinthO_SetA = 0;
	double var_EighthO_NinthO_SetB = 0;
	double var_EighthO_NinthO_SetC = 0;
	double var_EighthO_NinthO_SetD = 0;
	
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
	
	bool IsSelected_EighthO_SetA[nArray];
	bool IsSelected_EighthO_SetB[nArray];
	bool IsSelected_EighthO_SetC[nArray];
	bool IsSelected_EighthO_SetD[nArray];
	
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
	
	double SD_NinthO_FU_SetA = 0;
	double SD_NinthO_FU_SetB = 0;
	double SD_NinthO_FU_SetC = 0;
	double SD_NinthO_FU_SetD = 0;
	
	double SD_NinthO_FO_SetA = 0;
	double SD_NinthO_FO_SetB = 0;
	double SD_NinthO_FO_SetC = 0;
	double SD_NinthO_FO_SetD = 0;
	
	double SD_NinthO_2O_SetA = 0;
	double SD_NinthO_2O_SetB = 0;
	double SD_NinthO_2O_SetC = 0;
	double SD_NinthO_2O_SetD = 0;
	
	double SD_NinthO_ThirdO_SetA = 0;
	double SD_NinthO_ThirdO_SetB = 0;
	double SD_NinthO_ThirdO_SetC = 0;
	double SD_NinthO_ThirdO_SetD = 0;
	
	double SD_NinthO_ForthO_SetA = 0;
	double SD_NinthO_ForthO_SetB = 0;
	double SD_NinthO_ForthO_SetC = 0;
	double SD_NinthO_ForthO_SetD = 0;
	
	double SD_NinthO_FifthO_SetA = 0;
	double SD_NinthO_FifthO_SetB = 0;
	double SD_NinthO_FifthO_SetC = 0;
	double SD_NinthO_FifthO_SetD = 0;
	
	double SD_NinthO_SixthO_SetA = 0;
	double SD_NinthO_SixthO_SetB = 0;
	double SD_NinthO_SixthO_SetC = 0;
	double SD_NinthO_SixthO_SetD = 0;
	
	double SD_NinthO_SeventhO_SetA = 0;
	double SD_NinthO_SeventhO_SetB = 0;
	double SD_NinthO_SeventhO_SetC = 0;
	double SD_NinthO_SeventhO_SetD = 0;
	
	double SD_NinthO_EighthO_SetA = 0;
	double SD_NinthO_EighthO_SetB = 0;
	double SD_NinthO_EighthO_SetC = 0;
	double SD_NinthO_EighthO_SetD = 0;
	
	double SD_NinthO_NinthO_SetA = 0;
	double SD_NinthO_NinthO_SetB = 0;
	double SD_NinthO_NinthO_SetC = 0;
	double SD_NinthO_NinthO_SetD = 0;
	
	double var_NinthO_FU_SetA = 0;
	double var_NinthO_FU_SetB = 0;
	double var_NinthO_FU_SetC = 0;
	double var_NinthO_FU_SetD = 0;
	
	double var_NinthO_FO_SetA = 0;
	double var_NinthO_FO_SetB = 0;
	double var_NinthO_FO_SetC = 0;
	double var_NinthO_FO_SetD = 0;
	
	double var_NinthO_2O_SetA = 0;
	double var_NinthO_2O_SetB = 0;
	double var_NinthO_2O_SetC = 0;
	double var_NinthO_2O_SetD = 0;
	
	double var_NinthO_ThirdO_SetA = 0;
	double var_NinthO_ThirdO_SetB = 0;
	double var_NinthO_ThirdO_SetC = 0;
	double var_NinthO_ThirdO_SetD = 0;
	
	double var_NinthO_ForthO_SetA = 0;
	double var_NinthO_ForthO_SetB = 0;
	double var_NinthO_ForthO_SetC = 0;
	double var_NinthO_ForthO_SetD = 0;
	
	double var_NinthO_FifthO_SetA = 0;
	double var_NinthO_FifthO_SetB = 0;
	double var_NinthO_FifthO_SetC = 0;
	double var_NinthO_FifthO_SetD = 0;
	
	double var_NinthO_SixthO_SetA = 0;
	double var_NinthO_SixthO_SetB = 0;
	double var_NinthO_SixthO_SetC = 0;
	double var_NinthO_SixthO_SetD = 0;
	
	double var_NinthO_SeventhO_SetA = 0;
	double var_NinthO_SeventhO_SetB = 0;
	double var_NinthO_SeventhO_SetC = 0;
	double var_NinthO_SeventhO_SetD = 0;
	
	double var_NinthO_EighthO_SetA = 0;
	double var_NinthO_EighthO_SetB = 0;
	double var_NinthO_EighthO_SetC = 0;
	double var_NinthO_EighthO_SetD = 0;
	
	double var_NinthO_NinthO_SetA = 0;
	double var_NinthO_NinthO_SetB = 0;
	double var_NinthO_NinthO_SetC = 0;
	double var_NinthO_NinthO_SetD = 0;
	
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
	
	bool IsSelected_NinthO_SetA[nArray];
	bool IsSelected_NinthO_SetB[nArray];
	bool IsSelected_NinthO_SetC[nArray];
	bool IsSelected_NinthO_SetD[nArray];
	
	/////////////////////////////////////////////////
	
	///New Varibles///
	const int nArray = NumOfModels + 1;
	
	// double mode;
	// double period;
	// double growth;
	
	double period_FU_SetA[nArray];
	double period_FU_SetB[nArray];
	double period_FU_SetC[nArray];
	double period_FU_SetD[nArray];
	
	double growth_FU_SetA[nArray];
	double growth_FU_SetB[nArray];
	double growth_FU_SetC[nArray];
	double growth_FU_SetD[nArray];
	double growth_FU;
	
	double period_FO_SetA[nArray];
	double period_FO_SetB[nArray];
	double period_FO_SetC[nArray];
	double period_FO_SetD[nArray];
	
	double growth_FO_SetA[nArray];
	double growth_FO_SetB[nArray];
	double growth_FO_SetC[nArray];
	double growth_FO_SetD[nArray];
	double growth_FO;
	
	double period_2O_SetA[nArray];
	double period_2O_SetB[nArray];
	double period_2O_SetC[nArray];
	double period_2O_SetD[nArray];
	
	double growth_2O_SetA[nArray];
	double growth_2O_SetB[nArray];
	double growth_2O_SetC[nArray];
	double growth_2O_SetD[nArray];
	double growth_2O;
	
	double period_ThirdO_SetA[nArray];
	double period_ThirdO_SetB[nArray];
	double period_ThirdO_SetC[nArray];
	double period_ThirdO_SetD[nArray];
	
	double growth_ThirdO_SetA[nArray];
	double growth_ThirdO_SetB[nArray];
	double growth_ThirdO_SetC[nArray];
	double growth_ThirdO_SetD[nArray];
	double growth_ThirdO;
	
	double period_ForthO_SetA[nArray];
	double period_ForthO_SetB[nArray];
	double period_ForthO_SetC[nArray];
	double period_ForthO_SetD[nArray];
	
	double growth_ForthO_SetA[nArray];
	double growth_ForthO_SetB[nArray];
	double growth_ForthO_SetC[nArray];
	double growth_ForthO_SetD[nArray];
	double growth_ForthO;
	
	double period_FifthO_SetA[nArray];
	double period_FifthO_SetB[nArray];
	double period_FifthO_SetC[nArray];
	double period_FifthO_SetD[nArray];
	
	double growth_FifthO_SetA[nArray];
	double growth_FifthO_SetB[nArray];
	double growth_FifthO_SetC[nArray];
	double growth_FifthO_SetD[nArray];
	double growth_FifthO;
	
	double period_SixthO_SetA[nArray];
	double period_SixthO_SetB[nArray];
	double period_SixthO_SetC[nArray];
	double period_SixthO_SetD[nArray];
	
	double growth_SixthO_SetA[nArray];
	double growth_SixthO_SetB[nArray];
	double growth_SixthO_SetC[nArray];
	double growth_SixthO_SetD[nArray];
	double growth_SixthO;
	
	double period_SeventhO_SetA[nArray];
	double period_SeventhO_SetB[nArray];
	double period_SeventhO_SetC[nArray];
	double period_SeventhO_SetD[nArray];
	
	double growth_SeventhO_SetA[nArray];
	double growth_SeventhO_SetB[nArray];
	double growth_SeventhO_SetC[nArray];
	double growth_SeventhO_SetD[nArray];
	double growth_SeventhO;
	
	double period_EighthO_SetA[nArray];
	double period_EighthO_SetB[nArray];
	double period_EighthO_SetC[nArray];
	double period_EighthO_SetD[nArray];
	
	double growth_EighthO_SetA[nArray];
	double growth_EighthO_SetB[nArray];
	double growth_EighthO_SetC[nArray];
	double growth_EighthO_SetD[nArray];
	double growth_EighthO;
	
	double period_NinthO_SetA[nArray];
	double period_NinthO_SetB[nArray];
	double period_NinthO_SetC[nArray];
	double period_NinthO_SetD[nArray];
	
	double growth_NinthO_SetA[nArray];
	double growth_NinthO_SetB[nArray];
	double growth_NinthO_SetC[nArray];
	double growth_NinthO_SetD[nArray];
	double growth_NinthO;
	
	//double mean;
	
	double mod_temp;
	double z_temp;
	double x_temp;
	double Mass_temp;
	double L_temp;
	double Teff_temp;
	
	double mod[nArray];
	double z[nArray];
	double x[nArray];
	double Mass[nArray];
	double L[nArray];
	double Teff[nArray]; 
	
	bool ModelRan_SetA[nArray];
	bool ModelRan_SetB[nArray];
	bool ModelRan_SetC[nArray];
	bool ModelRan_SetD[nArray];
	
	//////////////////////////////////////
	
	//////////Model Grid - inputing into arrays///////////////
	string ModelGrid_header;
	char ModelGridFilePath[100];
	ifstream inFile_ModelGrid;
	
	strcpy(ModelGridFilePath, ModelGridFileName);
	inFile_ModelGrid.open(ModelGridFilePath,ios::in);
	if(inFile_ModelGrid.good()){
		cout<<"Path to Model Grid file: "<<ModelGridFilePath<<endl;
		getline(inFile_ModelGrid, ModelGrid_header);
		for(int i = 0; i < NumOfModels; i++){
			inFile_ModelGrid>>mod_temp>>z_temp>>x_temp>>Mass_temp>>L_temp>>Teff_temp;
				mod[i] = mod_temp;
				z[i] = z_temp;
				x[i] = x_temp;
				Mass[i] = Mass_temp;
				L[i] = L_temp;
				Teff[i] = Teff_temp; 
		}
	}
	else{cout<<"Error opening model grid file. Check name?"<<endl;}
	inFile_ModelGrid.close();
	
	//////////////Main Set loop//////////////
	for(int j = 0; j < NumOfSets; j++){
		
		ofstream outFile_AverageResonance;
		ofstream outFile_SD;
		ofstream outFile_Num;
		ofstream outFile_Fig5_FU_FO;
		
		char AverageResonanceFileName[100];
		strcpy(AverageResonanceFileName, outputFileName);
		strcat(AverageResonanceFileName, "_AverageResonances");
		
		char SDFileName[100];
		strcpy(SDFileName, outputFileName);
		strcat(SDFileName, "_SD");
		
		char NumFileName[100];
		strcpy(NumFileName, outputFileName);
		strcat(NumFileName, "_Num");
		
		char Fig5_FU_FO_FileName[100];
		strcpy(Fig5_FU_FO_FileName, outputFileName);
		strcat(Fig5_FU_FO_FileName, "_Fig5_FU_FO");
		
		if(j == 0){
			strcat(AverageResonanceFileName, SetA_Suffix);
			strcat(SDFileName, SetA_Suffix);
			strcat(NumFileName, SetA_Suffix);
			strcat(Fig5_FU_FO_FileName, SetA_Suffix);
		}
		if(j == 1){
			strcat(AverageResonanceFileName, SetB_Suffix);
			strcat(SDFileName, SetB_Suffix);
			strcat(NumFileName, SetB_Suffix);
			strcat(Fig5_FU_FO_FileName, SetB_Suffix);
		}
		if(j == 2){
			strcat(AverageResonanceFileName, SetC_Suffix);
			strcat(SDFileName, SetC_Suffix);
			strcat(NumFileName, SetC_Suffix);
			strcat(Fig5_FU_FO_FileName, SetC_Suffix);
		}
		if(j == 3){
			strcat(AverageResonanceFileName, SetD_Suffix);
			strcat(SDFileName, SetD_Suffix);
			strcat(NumFileName, SetD_Suffix);
			strcat(Fig5_FU_FO_FileName, SetD_Suffix);
		}
		
		//cout<<"AverageResonanceFileName (output): "<<AverageResonanceFileName<<endl;
		//cout<<"SDFileName (output): "<<SDFileName<<endl;
		//cout<<"Fig5_FU_FO_FileName (output): "<<Fig5_FU_FO_FileName<<endl;
		
		//Filling bool arrays for SD and ModelRan selection//
		for(int i = 0; i < NumOfModels; i++){
			if(j == 0){
				IsSelected_FU_SetA[i] = false;
				IsSelected_FO_SetA[i] = false;
				IsSelected_2O_SetA[i] = false;
				IsSelected_ThirdO_SetA[i] = false;
				IsSelected_ForthO_SetA[i] = false;
				IsSelected_FifthO_SetA[i] = false;
				IsSelected_SixthO_SetA[i] = false;
				IsSelected_SeventhO_SetA[i] = false;
				IsSelected_EighthO_SetA[i] = false;
				IsSelected_NinthO_SetA[i] = false;
				
				ModelRan_SetA[i] = false;
			}
			if(j == 1){
				IsSelected_FU_SetB[i] = false;
				IsSelected_FO_SetB[i] = false;
				IsSelected_2O_SetB[i] = false;
				IsSelected_ThirdO_SetB[i] = false;
				IsSelected_ForthO_SetB[i] = false;
				IsSelected_FifthO_SetB[i] = false;
				IsSelected_SixthO_SetB[i] = false;
				IsSelected_SeventhO_SetB[i] = false;
				IsSelected_EighthO_SetB[i] = false;
				IsSelected_NinthO_SetB[i] = false;
				
				ModelRan_SetB[i] = false;
			}
			if(j == 2){
				IsSelected_FU_SetC[i] = false;
				IsSelected_FO_SetC[i] = false;
				IsSelected_2O_SetC[i] = false;
				IsSelected_ThirdO_SetC[i] = false;
				IsSelected_ForthO_SetC[i] = false;
				IsSelected_FifthO_SetC[i] = false;
				IsSelected_SixthO_SetC[i] = false;
				IsSelected_SeventhO_SetC[i] = false;
				IsSelected_EighthO_SetC[i] = false;
				IsSelected_NinthO_SetC[i] = false;
				
				ModelRan_SetC[i] = false;
			}
			if(j == 3){
				IsSelected_FU_SetD[i] = false;
				IsSelected_FO_SetD[i] = false;
				IsSelected_2O_SetD[i] = false;
				IsSelected_ThirdO_SetD[i] = false;
				IsSelected_ForthO_SetD[i] = false;
				IsSelected_FifthO_SetD[i] = false;
				IsSelected_SixthO_SetD[i] = false;
				IsSelected_SeventhO_SetD[i] = false;
				IsSelected_EighthO_SetD[i] = false;
				IsSelected_NinthO_SetD[i] = false;
				
				ModelRan_SetD[i] = false;
			}
		}
		
		///Main program loop///
		for(int i = 0; i < NumOfModels; i++){
			
			//////Reading LINA file////////
			
			//Getting model number into string
			stringstream stream;
				stream<<(i+1);
				string Model = stream.str();
				//cout<<"Current Model in string form: "<<Model<<endl;
				
			string readout; //String thats viewing each line
			
			string LINA_header; //Varible for header of LINA file
			char LINAFilePath[100];
			ifstream inFile;
			
			string History_header;
			char HistoryFilePath[100];
			ifstream inFile_History;
			
			if(j == 0){
				strcpy(LINAFilePath, log_directory_prefix_SetA);
				strcpy(HistoryFilePath, log_directory_prefix_SetA);
			}
			if(j == 1){
				strcpy(LINAFilePath, log_directory_prefix_SetB);
				strcpy(HistoryFilePath, log_directory_prefix_SetB);
			}
			if(j == 2){
				strcpy(LINAFilePath, log_directory_prefix_SetC);
				strcpy(HistoryFilePath, log_directory_prefix_SetC);
			}
			if(j == 3){
				strcpy(LINAFilePath, log_directory_prefix_SetD);
				strcpy(HistoryFilePath, log_directory_prefix_SetD);
			}
			strcat(LINAFilePath, Model.c_str());
			strcat(LINAFilePath, "/");
			strcat(LINAFilePath, LINAFileName);
			
			strcat(HistoryFilePath, Model.c_str());
			strcat(HistoryFilePath, "/");
			strcat(HistoryFilePath, HistoryFileName);
			
			cout<<"Path to LINA file: "<<LINAFilePath<<endl;
			//cout<<"Path to history file: "<<HistoryFilePath<<endl; //Not needed yet
			
			inFile.open(LINAFilePath,ios::in);
			//inFile_History.open(HistoryFilePath,ios::in); //Not needed yet
		
			// Warning if file cant be opened
			if(!inFile.is_open()){ 
				cout << "Error opening LINA file. Model did not run? \n";
			}
			if(inFile.is_open()){
				cout<<"LINA file was opened successfully"<<endl;			
			}
			if(inFile.good()){
				//reads LINA file into arrays
				getline(inFile,LINA_header);
				//cout<<LINA_header<<endl;
				
				for(int h = 0; h < 6; h++){
					getline(inFile_History,History_header);
					//cout<<History_header<<endl;
				}
				
				cout<<mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<Mass[i]<<setw(20)<<L[i]<<setw(20)<<Teff[i]<<setw(20)<<endl;
				
				//Setting bool arrays for ModelRan selection to true//
				if(j == 0){
					ModelRan_SetA[i] = true;
				}
				if(j == 1){
					ModelRan_SetB[i] = true;
				}
				if(j == 2){
					ModelRan_SetC[i] = true;
				}
				if(j == 3){
					ModelRan_SetD[i] = true;
				}
				
				//Loop for number of modes on LINA data. Each iteration looks at a different mode and fills periods to be used
				for(int m = 0; m < NumOfModes; m++){
					
					double mode;
					double period;
					double growth;
					
					inFile>>mode>>period>>growth;
					//cout<<mode<<" "<<period<<" "<<growth<<endl;
					
					//Storing periods (and growths?) in arrays//
					if(j == 0){
						if(m == 0){
							period_FU_SetA[i] = period;
							growth_FU_SetA[i] = growth;
							growth_FU = growth;
						}
						if(m == 1){
							period_FO_SetA[i] = period;
							growth_FO_SetA[i] = growth;
							growth_FO = growth;
						}
						if(m == 2){
							period_2O_SetA[i] = period;
							growth_2O_SetA[i] = growth;
							growth_2O = growth;
						}
						if(m == 3){
							period_ThirdO_SetA[i] = period;
							growth_ThirdO_SetA[i] = growth;
							growth_ThirdO = growth;
						}
						if(m == 4){
							period_ForthO_SetA[i] = period;
							growth_ForthO_SetA[i] = growth;
							growth_ForthO = growth;
						}
						if(m == 5){
							period_FifthO_SetA[i] = period;
							growth_FifthO_SetA[i] = growth;
							growth_FifthO = growth;
						}
						if(m == 6){
							period_SixthO_SetA[i] = period;
							growth_SixthO_SetA[i] = growth;
							growth_SixthO = growth;
						}
						if(m == 7){
							period_SeventhO_SetA[i] = period;
							growth_SeventhO_SetA[i] = growth;
							growth_SeventhO = growth;
						}
						if(m == 8){
							period_EighthO_SetA[i] = period;
							growth_EighthO_SetA[i] = growth;
							growth_EighthO = growth;
						}
						if(m == 9){
							period_NinthO_SetA[i] = period;
							growth_NinthO_SetA[i] = growth;
							growth_NinthO = growth;
						}
					}
					if(j == 1){
						if(m == 0){
							period_FU_SetB[i] = period;
							growth_FU_SetB[i] = growth;
							growth_FU = growth;
						}
						if(m == 1){
							period_FO_SetB[i] = period;
							growth_FO_SetB[i] = growth;
							growth_FO = growth;
						}
						if(m == 2){
							period_2O_SetB[i] = period;
							growth_2O_SetB[i] = growth;
							growth_2O = growth;
						}
						if(m == 3){
							period_ThirdO_SetB[i] = period;
							growth_ThirdO_SetB[i] = growth;
							growth_ThirdO = growth;
						}
						if(m == 4){
							period_ForthO_SetB[i] = period;
							growth_ForthO_SetB[i] = growth;
							growth_ForthO = growth;
						}
						if(m == 5){
							period_FifthO_SetB[i] = period;
							growth_FifthO_SetB[i] = growth;
							growth_FifthO = growth;
						}
						if(m == 6){
							period_SixthO_SetB[i] = period;
							growth_SixthO_SetB[i] = growth;
							growth_SixthO = growth;
						}
						if(m == 7){
							period_SeventhO_SetB[i] = period;
							growth_SeventhO_SetB[i] = growth;
							growth_SeventhO = growth;
						}
						if(m == 8){
							period_EighthO_SetB[i] = period;
							growth_EighthO_SetB[i] = growth;
							growth_EighthO = growth;
						}
						if(m == 9){
							period_NinthO_SetB[i] = period;
							growth_NinthO_SetB[i] = growth;
							growth_NinthO = growth;
						}
					}
					if(j == 2){
						if(m == 0){
							period_FU_SetC[i] = period;
							growth_FU_SetC[i] = growth;
							growth_FU = growth;
						}
						if(m == 1){
							period_FO_SetC[i] = period;
							growth_FO_SetC[i] = growth;
							growth_FO = growth;
						}
						if(m == 2){
							period_2O_SetC[i] = period;
							growth_2O_SetC[i] = growth;
							growth_2O = growth;
						}
						if(m == 3){
							period_ThirdO_SetC[i] = period;
							growth_ThirdO_SetC[i] = growth;
							growth_ThirdO = growth;
						}
						if(m == 4){
							period_ForthO_SetC[i] = period;
							growth_ForthO_SetC[i] = growth;
							growth_ForthO = growth;
						}
						if(m == 5){
							period_FifthO_SetC[i] = period;
							growth_FifthO_SetC[i] = growth;
							growth_FifthO = growth;
						}
						if(m == 6){
							period_SixthO_SetC[i] = period;
							growth_SixthO_SetC[i] = growth;
							growth_SixthO = growth;
						}
						if(m == 7){
							period_SeventhO_SetC[i] = period;
							growth_SeventhO_SetC[i] = growth;
							growth_SeventhO = growth;
						}
						if(m == 8){
							period_EighthO_SetC[i] = period;
							growth_EighthO_SetC[i] = growth;
							growth_EighthO = growth;
						}
						if(m == 9){
							period_NinthO_SetC[i] = period;
							growth_NinthO_SetC[i] = growth;
							growth_NinthO = growth;
						}
					}
					if(j == 3){
						if(m == 0){
							period_FU_SetD[i] = period;
							growth_FU_SetD[i] = growth;
							growth_FU = growth;
						}
						if(m == 1){
							period_FO_SetD[i] = period;
							growth_FO_SetD[i] = growth;
							growth_FO = growth;
						}
						if(m == 2){
							period_2O_SetD[i] = period;
							growth_2O_SetD[i] = growth;
							growth_2O = growth;
						}
						if(m == 3){
							period_ThirdO_SetD[i] = period;
							growth_ThirdO_SetD[i] = growth;
							growth_ThirdO = growth;
						}
						if(m == 4){
							period_ForthO_SetD[i] = period;
							growth_ForthO_SetD[i] = growth;
							growth_ForthO = growth;
						}
						if(m == 5){
							period_FifthO_SetD[i] = period;
							growth_FifthO_SetD[i] = growth;
							growth_FifthO = growth;
						}
						if(m == 6){
							period_SixthO_SetD[i] = period;
							growth_SixthO_SetD[i] = growth;
							growth_SixthO = growth;
						}
						if(m == 7){
							period_SeventhO_SetD[i] = period;
							growth_SeventhO_SetD[i] = growth;
							growth_SeventhO = growth;
						}
						if(m == 8){
							period_EighthO_SetD[i] = period;
							growth_EighthO_SetD[i] = growth;
							growth_EighthO = growth;
						}
						if(m == 9){
							period_NinthO_SetD[i] = period;
							growth_NinthO_SetD[i] = growth;
							growth_NinthO = growth;
						}
					}
				}
				inFile.close();
				inFile_ModelGrid.close();
				
				inFile.open(LINAFilePath,ios::in);
				getline(inFile,LINA_header);
				
				for(int m = 0; m < NumOfModes; m++){
					
					double mode;
					double period;
					double growth;
					
					inFile>>mode>>period>>growth;
					//cout<<mode<<" "<<period<<" "<<growth<<endl;
					
					//Filling temp varibles for conditions//
					if(j == 0){
						double ratio_temp_FU = period_FU_SetA[i]/period;
						double ratio_temp_FO = period_FO_SetA[i]/period;
						double ratio_temp_2O = period_2O_SetA[i]/period;
						double ratio_temp_ThirdO = period_ThirdO_SetA[i]/period;
						double ratio_temp_ForthO = period_ForthO_SetA[i]/period;
						double ratio_temp_FifthO = period_FifthO_SetA[i]/period;
						double ratio_temp_SixthO = period_SixthO_SetA[i]/period;
						double ratio_temp_SeventhO = period_SeventhO_SetA[i]/period;
						double ratio_temp_EighthO = period_EighthO_SetA[i]/period;
						double ratio_temp_NinthO = period_NinthO_SetA[i]/period;
					}
					if(j == 1){
						double ratio_temp_FU = period_FU_SetB[i]/period;
						double ratio_temp_FO = period_FO_SetB[i]/period;
						double ratio_temp_2O = period_2O_SetB[i]/period;
						double ratio_temp_ThirdO = period_ThirdO_SetB[i]/period;
						double ratio_temp_ForthO = period_ForthO_SetB[i]/period;
						double ratio_temp_FifthO = period_FifthO_SetB[i]/period;
						double ratio_temp_SixthO = period_SixthO_SetB[i]/period;
						double ratio_temp_SeventhO = period_SeventhO_SetB[i]/period;
						double ratio_temp_EighthO = period_EighthO_SetB[i]/period;
						double ratio_temp_NinthO = period_NinthO_SetB[i]/period;
					}
					if(j == 2){
						double ratio_temp_FU = period_FU_SetC[i]/period;
						double ratio_temp_FO = period_FO_SetC[i]/period;
						double ratio_temp_2O = period_2O_SetC[i]/period;
						double ratio_temp_ThirdO = period_ThirdO_SetC[i]/period;
						double ratio_temp_ForthO = period_ForthO_SetC[i]/period;
						double ratio_temp_FifthO = period_FifthO_SetC[i]/period;
						double ratio_temp_SixthO = period_SixthO_SetC[i]/period;
						double ratio_temp_SeventhO = period_SeventhO_SetC[i]/period;
						double ratio_temp_EighthO = period_EighthO_SetC[i]/period;
						double ratio_temp_NinthO = period_NinthO_SetC[i]/period;
					}
					if(j == 3){
						double ratio_temp_FU = period_FU_SetD[i]/period;
						double ratio_temp_FO = period_FO_SetD[i]/period;
						double ratio_temp_2O = period_2O_SetD[i]/period;
						double ratio_temp_ThirdO = period_ThirdO_SetD[i]/period;
						double ratio_temp_ForthO = period_ForthO_SetD[i]/period;
						double ratio_temp_FifthO = period_FifthO_SetD[i]/period;
						double ratio_temp_SixthO = period_SixthO_SetD[i]/period;
						double ratio_temp_SeventhO = period_SeventhO_SetD[i]/period;
						double ratio_temp_EighthO = period_EighthO_SetD[i]/period;
						double ratio_temp_NinthO = period_NinthO_SetD[i]/period;
					}
					
					///Main calculations of ratios///
					if(!DoBoundsCut || ((ratio_temp_FU <= (1. + bound)) && (ratio_temp_FU >= (1. - bound))) || ((ratio_temp_FU <= (0.5 + bound)) && (ratio_temp_FU >= (0.5 - bound))) || ((ratio_temp_FU <= (1.5 + bound)) && (ratio_temp_FU >= (1.5 - bound))) || ((ratio_temp_FU <= (2.0 + bound)) && (ratio_temp_FU >= (2.0 - bound))) || ((ratio_temp_FU <= (2.5 + bound)) && (ratio_temp_FU >= (2.5 - bound))) || ((ratio_temp_FU <= (3.0 + bound)) && (ratio_temp_FU >= (3.0 - bound))) || ((ratio_temp_FU <= (3.5 + bound)) && (ratio_temp_FU >= (3.5 - bound))) || ((ratio_temp_FU <= (4.0 + bound)) && (ratio_temp_FU >= (4.0 - bound))) || ((ratio_temp_FU <= (4.5 + bound)) && (ratio_temp_FU >= (4.5 - bound))) || ((ratio_temp_FU <= (5.0 + bound)) && (ratio_temp_FU >= (5.0 - bound))) || ((ratio_temp_FU <= (5.5 + bound)) && (ratio_temp_FU >= (5.5 - bound))) || ((ratio_temp_FU <= (6.0 + bound)) && (ratio_temp_FU >= (6.0 - bound)))){
						//FU mode//
						if((growth > 0.0 || !DoPosGrowthCut) && (growth_FU > 0)){
							cout<<"Resonance Ratios With FU..."<<endl;
							//SetA
							if(j == 0){	
								IsSelected_FU_SetA[i] = true;
								if(m == 0){
									ResonanceRatio_FU_FU_SetA[i] = period_FU_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_FU_SetA[i]<<endl;
									
									OverallResonance_FU_FU_SetA = OverallResonance_FU_FU_SetA + ResonanceRatio_FU_FU_SetA[i];
									NumOfFU_FU_SetA++;
								}
								if(m == 1){
									ResonanceRatio_FU_FO_SetA[i] = period_FU_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_FO_SetA[i]<<endl;
									
									OverallResonance_FU_FO_SetA = OverallResonance_FU_FO_SetA + ResonanceRatio_FU_FO_SetA[i];	
									NumOfFU_FO_SetA++;
								}
								if(m == 2){
									ResonanceRatio_FU_2O_SetA[i] = period_FU_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_2O_SetA[i]<<endl;
									
									OverallResonance_FU_2O_SetA = OverallResonance_FU_2O_SetA + ResonanceRatio_FU_2O_SetA[i];
									NumOfFU_2O_SetA++;
								}
								if(m == 3){
									ResonanceRatio_FU_ThirdO_SetA[i] = period_FU_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_ThirdO_SetA[i]<<endl;
									
									OverallResonance_FU_ThirdO_SetA = OverallResonance_FU_ThirdO_SetA + ResonanceRatio_FU_ThirdO_SetA[i];
									NumOfFU_ThirdO_SetA++;
								}
								if(m == 4){
									ResonanceRatio_FU_ForthO_SetA[i] = period_FU_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_ForthO_SetA[i]<<endl;
									
									OverallResonance_FU_ForthO_SetA = OverallResonance_FU_ForthO_SetA + ResonanceRatio_FU_ForthO_SetA[i];
									NumOfFU_ForthO_SetA++;
								}
								if(m == 5){
									ResonanceRatio_FU_FifthO_SetA[i] = period_FU_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_FifthO_SetA[i]<<endl;
									
									OverallResonance_FU_FifthO_SetA = OverallResonance_FU_FifthO_SetA + ResonanceRatio_FU_FifthO_SetA[i];
									NumOfFU_FifthO_SetA++;
								}
								if(m == 6){
									ResonanceRatio_FU_SixthO_SetA[i] = period_FU_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_SixthO_SetA[i]<<endl;
									
									OverallResonance_FU_SixthO_SetA = OverallResonance_FU_SixthO_SetA + ResonanceRatio_FU_SixthO_SetA[i];
									NumOfFU_SixthO_SetA++;
								}
								if(m == 7){
									ResonanceRatio_FU_SeventhO_SetA[i] = period_FU_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_SeventhO_SetA[i]<<endl;
									
									OverallResonance_FU_SeventhO_SetA = OverallResonance_FU_SeventhO_SetA + ResonanceRatio_FU_SeventhO_SetA[i];
									NumOfFU_SeventhO_SetA++;
								}
								if(m == 8){
									ResonanceRatio_FU_EighthO_SetA[i] = period_FU_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_EighthO_SetA[i]<<endl;
									
									OverallResonance_FU_EighthO_SetA = OverallResonance_FU_EighthO_SetA + ResonanceRatio_FU_EighthO_SetA[i];
									NumOfFU_EighthO_SetA++;
								}
								if(m == 9){
									ResonanceRatio_FU_NinthO_SetA[i] = period_FU_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_NinthO_SetA[i]<<endl;
									
									OverallResonance_FU_NinthO_SetA = OverallResonance_FU_NinthO_SetA + ResonanceRatio_FU_NinthO_SetA[i];
									NumOfFU_NinthO_SetA++;
								}
							}
							//SetB
							if(j == 1){
								IsSelected_FU_SetB[i] = true;
								if(m == 0){
									ResonanceRatio_FU_FU_SetB[i] = period_FU_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_FU_SetB[i]<<endl;
									
									OverallResonance_FU_FU_SetB = OverallResonance_FU_FU_SetB + ResonanceRatio_FU_FU_SetB[i];
									NumOfFU_FU_SetB++;
								}
								if(m == 1){
									ResonanceRatio_FU_FO_SetB[i] = period_FU_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_FO_SetB[i]<<endl;
									
									OverallResonance_FU_FO_SetB = OverallResonance_FU_FO_SetB + ResonanceRatio_FU_FO_SetB[i];
									NumOfFU_FO_SetB++;
								}
								if(m == 2){
									ResonanceRatio_FU_2O_SetB[i] = period_FU_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_2O_SetB[i]<<endl;
									
									OverallResonance_FU_2O_SetB = OverallResonance_FU_2O_SetB + ResonanceRatio_FU_2O_SetB[i];
									NumOfFU_2O_SetB++;
								}
								if(m == 3){
									ResonanceRatio_FU_ThirdO_SetB[i] = period_FU_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_ThirdO_SetB[i]<<endl;
									
									OverallResonance_FU_ThirdO_SetB = OverallResonance_FU_ThirdO_SetB + ResonanceRatio_FU_ThirdO_SetB[i];
									NumOfFU_ThirdO_SetB++;
								}
								if(m == 4){
									ResonanceRatio_FU_ForthO_SetB[i] = period_FU_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_ForthO_SetB[i]<<endl;
									
									OverallResonance_FU_ForthO_SetB = OverallResonance_FU_ForthO_SetB + ResonanceRatio_FU_ForthO_SetB[i];
									NumOfFU_ForthO_SetB++;
								}
								if(m == 5){
									ResonanceRatio_FU_FifthO_SetB[i] = period_FU_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_FifthO_SetB[i]<<endl;
									
									OverallResonance_FU_FifthO_SetB = OverallResonance_FU_FifthO_SetB + ResonanceRatio_FU_FifthO_SetB[i];
									NumOfFU_FifthO_SetB++;
								}
								if(m == 6){
									ResonanceRatio_FU_SixthO_SetB[i] = period_FU_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_SixthO_SetB[i]<<endl;
									
									OverallResonance_FU_SixthO_SetB = OverallResonance_FU_SixthO_SetB + ResonanceRatio_FU_SixthO_SetB[i];
									NumOfFU_SixthO_SetB++;
								}
								if(m == 7){
									ResonanceRatio_FU_SeventhO_SetB[i] = period_FU_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_SeventhO_SetB[i]<<endl;
									
									OverallResonance_FU_SeventhO_SetB = OverallResonance_FU_SeventhO_SetB + ResonanceRatio_FU_SeventhO_SetB[i];
									NumOfFU_SeventhO_SetB++;
								}
								if(m == 8){
									ResonanceRatio_FU_EighthO_SetB[i] = period_FU_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_EighthO_SetB[i]<<endl;
									
									OverallResonance_FU_EighthO_SetB = OverallResonance_FU_EighthO_SetB + ResonanceRatio_FU_EighthO_SetB[i];
									NumOfFU_EighthO_SetB++;
								}
								if(m == 9){
									ResonanceRatio_FU_NinthO_SetB[i] = period_FU_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_NinthO_SetB[i]<<endl;
									
									OverallResonance_FU_NinthO_SetB = OverallResonance_FU_NinthO_SetB + ResonanceRatio_FU_NinthO_SetB[i];
									NumOfFU_NinthO_SetB++;
								}
							}
							//SetC
							if(j == 2){
								IsSelected_FU_SetC[i] = true;
								if(m == 0){
									ResonanceRatio_FU_FU_SetC[i] = period_FU_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_FU_SetC[i]<<endl;
									
									OverallResonance_FU_FU_SetC = OverallResonance_FU_FU_SetC + ResonanceRatio_FU_FU_SetC[i];
									NumOfFU_FU_SetC++;
								}
								if(m == 1){
									ResonanceRatio_FU_FO_SetC[i] = period_FU_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_FO_SetC[i]<<endl;
									
									OverallResonance_FU_FO_SetC = OverallResonance_FU_FO_SetC + ResonanceRatio_FU_FO_SetC[i];
									NumOfFU_FO_SetC++;
								}
								if(m == 2){
									ResonanceRatio_FU_2O_SetC[i] = period_FU_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_2O_SetC[i]<<endl;
									
									OverallResonance_FU_2O_SetC = OverallResonance_FU_2O_SetC + ResonanceRatio_FU_2O_SetC[i];
									NumOfFU_2O_SetC++;
								}
								if(m == 3){
									ResonanceRatio_FU_ThirdO_SetC[i] = period_FU_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_ThirdO_SetC[i]<<endl;
									
									OverallResonance_FU_ThirdO_SetC = OverallResonance_FU_ThirdO_SetC + ResonanceRatio_FU_ThirdO_SetC[i];
									NumOfFU_ThirdO_SetC++;
								}
								if(m == 4){
									ResonanceRatio_FU_ForthO_SetC[i] = period_FU_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_ForthO_SetC[i]<<endl;
									
									OverallResonance_FU_ForthO_SetC = OverallResonance_FU_ForthO_SetC + ResonanceRatio_FU_ForthO_SetC[i];
									NumOfFU_ForthO_SetC++;
								}
								if(m == 5){
									ResonanceRatio_FU_FifthO_SetC[i] = period_FU_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_FifthO_SetC[i]<<endl;
									
									OverallResonance_FU_FifthO_SetC = OverallResonance_FU_FifthO_SetC + ResonanceRatio_FU_FifthO_SetC[i];
									NumOfFU_FifthO_SetC++;
								}
								if(m == 6){
									ResonanceRatio_FU_SixthO_SetC[i] = period_FU_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_SixthO_SetC[i]<<endl;
									
									OverallResonance_FU_SixthO_SetC = OverallResonance_FU_SixthO_SetC + ResonanceRatio_FU_SixthO_SetC[i];
									NumOfFU_SixthO_SetC++;
								}
								if(m == 7){
									ResonanceRatio_FU_SeventhO_SetC[i] = period_FU_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_SeventhO_SetC[i]<<endl;
									
									OverallResonance_FU_SeventhO_SetC = OverallResonance_FU_SeventhO_SetC + ResonanceRatio_FU_SeventhO_SetC[i];
									NumOfFU_SeventhO_SetC++;
								}
								if(m == 8){
									ResonanceRatio_FU_EighthO_SetC[i] = period_FU_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_EighthO_SetC[i]<<endl;
									
									OverallResonance_FU_EighthO_SetC = OverallResonance_FU_EighthO_SetC + ResonanceRatio_FU_EighthO_SetC[i];
									NumOfFU_EighthO_SetC++;
								}
								if(m == 9){
									ResonanceRatio_FU_NinthO_SetC[i] = period_FU_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_NinthO_SetC[i]<<endl;
									
									OverallResonance_FU_NinthO_SetC = OverallResonance_FU_NinthO_SetC + ResonanceRatio_FU_NinthO_SetC[i];
									NumOfFU_NinthO_SetC++;
								}
							}
							//SetD
							if(j == 3){
								IsSelected_FU_SetD[i] = true;
								if(m == 0){
									ResonanceRatio_FU_FU_SetD[i] = period_FU_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_FU_SetD[i]<<endl;
									
									OverallResonance_FU_FU_SetD = OverallResonance_FU_FU_SetD + ResonanceRatio_FU_FU_SetD[i];
									NumOfFU_FU_SetD++;
								}
								if(m == 1){
									ResonanceRatio_FU_FO_SetD[i] = period_FU_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_FO_SetD[i]<<endl;
									
									OverallResonance_FU_FO_SetD = OverallResonance_FU_FO_SetD + ResonanceRatio_FU_FO_SetD[i];
									NumOfFU_FO_SetD++;
								}
								if(m == 2){
									ResonanceRatio_FU_2O_SetD[i] = period_FU_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_2O_SetD[i]<<endl;
									
									OverallResonance_FU_2O_SetD = OverallResonance_FU_2O_SetD + ResonanceRatio_FU_2O_SetD[i];
									NumOfFU_2O_SetD++;
								}
								if(m == 3){
									ResonanceRatio_FU_ThirdO_SetD[i] = period_FU_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_ThirdO_SetD[i]<<endl;
									
									OverallResonance_FU_ThirdO_SetD = OverallResonance_FU_ThirdO_SetD + ResonanceRatio_FU_ThirdO_SetD[i];
									NumOfFU_ThirdO_SetD++;
								}
								if(m == 4){
									ResonanceRatio_FU_ForthO_SetD[i] = period_FU_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_ForthO_SetD[i]<<endl;
									
									OverallResonance_FU_ForthO_SetD = OverallResonance_FU_ForthO_SetD + ResonanceRatio_FU_ForthO_SetD[i];
									NumOfFU_ForthO_SetD++;
								}
								if(m == 5){
									ResonanceRatio_FU_FifthO_SetD[i] = period_FU_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_FifthO_SetD[i]<<endl;
									
									OverallResonance_FU_FifthO_SetD = OverallResonance_FU_FifthO_SetD + ResonanceRatio_FU_FifthO_SetD[i];
									NumOfFU_FifthO_SetD++;
								}
								if(m == 6){
									ResonanceRatio_FU_SixthO_SetD[i] = period_FU_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_SixthO_SetD[i]<<endl;
									
									OverallResonance_FU_SixthO_SetD = OverallResonance_FU_SixthO_SetD + ResonanceRatio_FU_SixthO_SetD[i];
									NumOfFU_SixthO_SetD++;
								}
								if(m == 7){
									ResonanceRatio_FU_SeventhO_SetD[i] = period_FU_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_SeventhO_SetD[i]<<endl;
									
									OverallResonance_FU_SeventhO_SetD = OverallResonance_FU_SeventhO_SetD + ResonanceRatio_FU_SeventhO_SetD[i];
									NumOfFU_SeventhO_SetD++;
								}
								if(m == 8){
									ResonanceRatio_FU_EighthO_SetD[i] = period_FU_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_EighthO_SetD[i]<<endl;
									
									OverallResonance_FU_EighthO_SetD = OverallResonance_FU_EighthO_SetD + ResonanceRatio_FU_EighthO_SetD[i];
									NumOfFU_EighthO_SetD++;
								}
								if(m == 9){
									ResonanceRatio_FU_NinthO_SetD[i] = period_FU_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FU_NinthO_SetD[i]<<endl;
									
									OverallResonance_FU_NinthO_SetD = OverallResonance_FU_NinthO_SetD + ResonanceRatio_FU_NinthO_SetD[i];
									NumOfFU_NinthO_SetD++;
								}
							}
						}
					}
					if(!DoBoundsCut || ((ratio_temp_FO <= (1. + bound)) && (ratio_temp_FO >= (1. - bound))) || ((ratio_temp_FO <= (0.5 + bound)) && (ratio_temp_FO >= (0.5 - bound))) || ((ratio_temp_FO <= (1.5 + bound)) && (ratio_temp_FO >= (1.5 - bound))) || ((ratio_temp_FO <= (2.0 + bound)) && (ratio_temp_FO >= (2.0 - bound))) || ((ratio_temp_FO <= (2.5 + bound)) && (ratio_temp_FO >= (2.5 - bound))) || ((ratio_temp_FO <= (3.0 + bound)) && (ratio_temp_FO >= (3.0 - bound))) || ((ratio_temp_FO <= (3.5 + bound)) && (ratio_temp_FO >= (3.5 - bound))) || ((ratio_temp_FO <= (4.0 + bound)) && (ratio_temp_FO >= (4.0 - bound))) || ((ratio_temp_FO <= (4.5 + bound)) && (ratio_temp_FO >= (4.5 - bound))) || ((ratio_temp_FO <= (5.0 + bound)) && (ratio_temp_FO >= (5.0 - bound))) || ((ratio_temp_FO <= (5.5 + bound)) && (ratio_temp_FO >= (5.5 - bound))) || ((ratio_temp_FO <= (6.0 + bound)) && (ratio_temp_FO >= (6.0 - bound)))){
						//FO mode//
						if((growth > 0.0 || !DoPosGrowthCut) && (growth_FO > 0)){
							cout<<"Resonance Ratios With FO..."<<endl;
							//SetA
							if(j == 0){
								IsSelected_FO_SetA[i] = true;
								if(m == 0){
									ResonanceRatio_FO_FU_SetA[i] = period_FO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_FU_SetA[i]<<endl;
									
									OverallResonance_FO_FU_SetA = OverallResonance_FO_FU_SetA + ResonanceRatio_FO_FU_SetA[i];
									NumOfFO_FU_SetA++;
								}
								if(m == 1){
									ResonanceRatio_FO_FO_SetA[i] = period_FO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_FO_SetA[i]<<endl;
									
									OverallResonance_FO_FO_SetA = OverallResonance_FO_FO_SetA + ResonanceRatio_FO_FO_SetA[i];
									NumOfFO_FO_SetA++;
								}
								if(m == 2){
									ResonanceRatio_FO_2O_SetA[i] = period_FO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_2O_SetA[i]<<endl;
									
									OverallResonance_FO_2O_SetA = OverallResonance_FO_2O_SetA + ResonanceRatio_FO_2O_SetA[i];
									NumOfFO_2O_SetA++;
								}
								if(m == 3){
									ResonanceRatio_FO_ThirdO_SetA[i] = period_FO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_ThirdO_SetA[i]<<endl;
									
									OverallResonance_FO_ThirdO_SetA = OverallResonance_FO_ThirdO_SetA + ResonanceRatio_FO_ThirdO_SetA[i];
									NumOfFO_ThirdO_SetA++;
								}
								if(m == 4){
									ResonanceRatio_FO_ForthO_SetA[i] = period_FO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_ForthO_SetA[i]<<endl;
									
									OverallResonance_FO_ForthO_SetA = OverallResonance_FO_ForthO_SetA + ResonanceRatio_FO_ForthO_SetA[i];
									NumOfFO_ForthO_SetA++;
								}
								if(m == 5){
									ResonanceRatio_FO_FifthO_SetA[i] = period_FO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_FifthO_SetA[i]<<endl;
									
									OverallResonance_FO_FifthO_SetA = OverallResonance_FO_FifthO_SetA + ResonanceRatio_FO_FifthO_SetA[i];
									NumOfFO_FifthO_SetA++;
								}
								if(m == 6){
									ResonanceRatio_FO_SixthO_SetA[i] = period_FO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_SixthO_SetA[i]<<endl;
									
									OverallResonance_FO_SixthO_SetA = OverallResonance_FO_SixthO_SetA + ResonanceRatio_FO_SixthO_SetA[i];
									NumOfFO_SixthO_SetA++;
								}
								if(m == 7){
									ResonanceRatio_FO_SeventhO_SetA[i] = period_FO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_SeventhO_SetA[i]<<endl;
									
									OverallResonance_FO_SeventhO_SetA = OverallResonance_FO_SeventhO_SetA + ResonanceRatio_FO_SeventhO_SetA[i];
									NumOfFO_SeventhO_SetA++;
								}
								if(m == 8){
									ResonanceRatio_FO_EighthO_SetA[i] = period_FO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_EighthO_SetA[i]<<endl;
									
									OverallResonance_FO_EighthO_SetA = OverallResonance_FO_EighthO_SetA + ResonanceRatio_FO_EighthO_SetA[i];
									NumOfFO_EighthO_SetA++;
								}
								if(m == 9){
									ResonanceRatio_FO_NinthO_SetA[i] = period_FO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_NinthO_SetA[i]<<endl;
									
									OverallResonance_FO_NinthO_SetA = OverallResonance_FO_NinthO_SetA + ResonanceRatio_FO_NinthO_SetA[i];
									NumOfFO_NinthO_SetA++;
								}
							}
							//SetB
							if(j == 1){
								IsSelected_FO_SetB[i] = true;
								if(m == 0){
									ResonanceRatio_FO_FU_SetB[i] = period_FO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_FU_SetB[i]<<endl;
									
									OverallResonance_FO_FU_SetB = OverallResonance_FO_FU_SetB + ResonanceRatio_FO_FU_SetB[i];
									NumOfFO_FU_SetB++;
								}
								if(m == 1){
									ResonanceRatio_FO_FO_SetB[i] = period_FO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_FO_SetB[i]<<endl;
									
									OverallResonance_FO_FO_SetB = OverallResonance_FO_FO_SetB + ResonanceRatio_FO_FO_SetB[i];
									NumOfFO_FO_SetB++;
								}
								if(m == 2){
									ResonanceRatio_FO_2O_SetB[i] = period_FO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_2O_SetB[i]<<endl;
									
									OverallResonance_FO_2O_SetB = OverallResonance_FO_2O_SetB + ResonanceRatio_FO_2O_SetB[i];
									NumOfFO_2O_SetB++;
								}
								if(m == 3){
									ResonanceRatio_FO_ThirdO_SetB[i] = period_FO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_ThirdO_SetB[i]<<endl;
									
									OverallResonance_FO_ThirdO_SetB = OverallResonance_FO_ThirdO_SetB + ResonanceRatio_FO_ThirdO_SetB[i];
									NumOfFO_ThirdO_SetB++;
								}
								if(m == 4){
									ResonanceRatio_FO_ForthO_SetB[i] = period_FO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_ForthO_SetB[i]<<endl;
									
									OverallResonance_FO_ForthO_SetB = OverallResonance_FO_ForthO_SetB + ResonanceRatio_FO_ForthO_SetB[i];
									NumOfFO_ForthO_SetB++;
								}
								if(m == 5){
									ResonanceRatio_FO_FifthO_SetB[i] = period_FO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_FifthO_SetB[i]<<endl;
									
									OverallResonance_FO_FifthO_SetB = OverallResonance_FO_FifthO_SetB + ResonanceRatio_FO_FifthO_SetB[i];
									NumOfFO_FifthO_SetB++;
								}
								if(m == 6){
									ResonanceRatio_FO_SixthO_SetB[i] = period_FO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_SixthO_SetB[i]<<endl;
									
									OverallResonance_FO_SixthO_SetB = OverallResonance_FO_SixthO_SetB + ResonanceRatio_FO_SixthO_SetB[i];
									NumOfFO_SixthO_SetB++;
								}
								if(m == 7){
									ResonanceRatio_FO_SeventhO_SetB[i] = period_FO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_SeventhO_SetB[i]<<endl;
									
									OverallResonance_FO_SeventhO_SetB = OverallResonance_FO_SeventhO_SetB + ResonanceRatio_FO_SeventhO_SetB[i];
									NumOfFO_SeventhO_SetB++;
								}
								if(m == 8){
									ResonanceRatio_FO_EighthO_SetB[i] = period_FO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_EighthO_SetB[i]<<endl;
									
									OverallResonance_FO_EighthO_SetB = OverallResonance_FO_EighthO_SetB + ResonanceRatio_FO_EighthO_SetB[i];
									NumOfFO_EighthO_SetB++;
								}
								if(m == 9){
									ResonanceRatio_FO_NinthO_SetB[i] = period_FO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_NinthO_SetB[i]<<endl;
									
									OverallResonance_FO_NinthO_SetB = OverallResonance_FO_NinthO_SetB + ResonanceRatio_FO_NinthO_SetB[i];
									NumOfFO_NinthO_SetB++;
								}
							}
							//SetC
							if(j == 2){
								IsSelected_FO_SetC[i] = true;
								if(m == 0){
									ResonanceRatio_FO_FU_SetC[i] = period_FO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_FU_SetC[i]<<endl;
									
									OverallResonance_FO_FU_SetC = OverallResonance_FO_FU_SetC + ResonanceRatio_FO_FU_SetC[i];
									NumOfFO_FU_SetC++;
								}
								if(m == 1){
									ResonanceRatio_FO_FO_SetC[i] = period_FO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_FO_SetC[i]<<endl;
									
									OverallResonance_FO_FO_SetC = OverallResonance_FO_FO_SetC + ResonanceRatio_FO_FO_SetC[i];
									NumOfFO_FO_SetC++;
								}
								if(m == 2){
									ResonanceRatio_FO_2O_SetC[i] = period_FO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_2O_SetC[i]<<endl;
									
									OverallResonance_FO_2O_SetC = OverallResonance_FO_2O_SetC + ResonanceRatio_FO_2O_SetC[i];
									NumOfFO_2O_SetC++;
								}
								if(m == 3){
									ResonanceRatio_FO_ThirdO_SetC[i] = period_FO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_ThirdO_SetC[i]<<endl;
									
									OverallResonance_FO_ThirdO_SetC = OverallResonance_FO_ThirdO_SetC + ResonanceRatio_FO_ThirdO_SetC[i];
									NumOfFO_ThirdO_SetC++;
								}
								if(m == 4){
									ResonanceRatio_FO_ForthO_SetC[i] = period_FO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_ForthO_SetC[i]<<endl;
									
									OverallResonance_FO_ForthO_SetC = OverallResonance_FO_ForthO_SetC + ResonanceRatio_FO_ForthO_SetC[i];
									NumOfFO_ForthO_SetC++;
								}
								if(m == 5){
									ResonanceRatio_FO_FifthO_SetC[i] = period_FO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_FifthO_SetC[i]<<endl;
									
									OverallResonance_FO_FifthO_SetC = OverallResonance_FO_FifthO_SetC + ResonanceRatio_FO_FifthO_SetC[i];
									NumOfFO_FifthO_SetC++;
								}
								if(m == 6){
									ResonanceRatio_FO_SixthO_SetC[i] = period_FO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_SixthO_SetC[i]<<endl;
									
									OverallResonance_FO_SixthO_SetC = OverallResonance_FO_SixthO_SetC + ResonanceRatio_FO_SixthO_SetC[i];
									NumOfFO_SixthO_SetC++;
								}
								if(m == 7){
									ResonanceRatio_FO_SeventhO_SetC[i] = period_FO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_SeventhO_SetC[i]<<endl;
									
									OverallResonance_FO_SeventhO_SetC = OverallResonance_FO_SeventhO_SetC + ResonanceRatio_FO_SeventhO_SetC[i];
									NumOfFO_SeventhO_SetC++;
								}
								if(m == 8){
									ResonanceRatio_FO_EighthO_SetC[i] = period_FO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_EighthO_SetC[i]<<endl;
									
									OverallResonance_FO_EighthO_SetC = OverallResonance_FO_EighthO_SetC + ResonanceRatio_FO_EighthO_SetC[i];
									NumOfFO_EighthO_SetC++;
								}
								if(m == 9){
									ResonanceRatio_FO_NinthO_SetC[i] = period_FO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_NinthO_SetC[i]<<endl;
									
									OverallResonance_FO_NinthO_SetC = OverallResonance_FO_NinthO_SetC + ResonanceRatio_FO_NinthO_SetC[i];
									NumOfFO_NinthO_SetC++;
								}
							}
							//SetD
							if(j == 3){
								IsSelected_FO_SetD[i] = true;
								if(m == 0){
									ResonanceRatio_FO_FU_SetD[i] = period_FO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_FU_SetD[i]<<endl;
									
									OverallResonance_FO_FU_SetD = OverallResonance_FO_FU_SetD + ResonanceRatio_FO_FU_SetD[i];
									NumOfFO_FU_SetD++;
								}
								if(m == 1){
									ResonanceRatio_FO_FO_SetD[i] = period_FO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_FO_SetD[i]<<endl;
									
									OverallResonance_FO_FO_SetD = OverallResonance_FO_FO_SetD + ResonanceRatio_FO_FO_SetD[i];
									NumOfFO_FO_SetD++;
								}
								if(m == 2){
									ResonanceRatio_FO_2O_SetD[i] = period_FO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_2O_SetD[i]<<endl;
									
									OverallResonance_FO_2O_SetD = OverallResonance_FO_2O_SetD + ResonanceRatio_FO_2O_SetD[i];
									NumOfFO_2O_SetD++;
								}
								if(m == 3){
									ResonanceRatio_FO_ThirdO_SetD[i] = period_FO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_ThirdO_SetD[i]<<endl;
									
									OverallResonance_FO_ThirdO_SetD = OverallResonance_FO_ThirdO_SetD + ResonanceRatio_FO_ThirdO_SetD[i];
									NumOfFO_ThirdO_SetD++;
								}
								if(m == 4){
									ResonanceRatio_FO_ForthO_SetD[i] = period_FO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_ForthO_SetD[i]<<endl;
									
									OverallResonance_FO_ForthO_SetD = OverallResonance_FO_ForthO_SetD + ResonanceRatio_FO_ForthO_SetD[i];
									NumOfFO_ForthO_SetD++;
								}
								if(m == 5){
									ResonanceRatio_FO_FifthO_SetD[i] = period_FO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_FifthO_SetD[i]<<endl;
									
									OverallResonance_FO_FifthO_SetD = OverallResonance_FO_FifthO_SetD + ResonanceRatio_FO_FifthO_SetD[i];
									NumOfFO_FifthO_SetD++;
								}
								if(m == 6){
									ResonanceRatio_FO_SixthO_SetD[i] = period_FO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_SixthO_SetD[i]<<endl;
									
									OverallResonance_FO_SixthO_SetD = OverallResonance_FO_SixthO_SetD + ResonanceRatio_FO_SixthO_SetD[i];
									NumOfFO_SixthO_SetD++;
								}
								if(m == 7){
									ResonanceRatio_FO_SeventhO_SetD[i] = period_FO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_SeventhO_SetD[i]<<endl;
									
									OverallResonance_FO_SeventhO_SetD = OverallResonance_FO_SeventhO_SetD + ResonanceRatio_FO_SeventhO_SetD[i];
									NumOfFO_SeventhO_SetD++;
								}
								if(m == 8){
									ResonanceRatio_FO_EighthO_SetD[i] = period_FO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_EighthO_SetD[i]<<endl;
									
									OverallResonance_FO_EighthO_SetD = OverallResonance_FO_EighthO_SetD + ResonanceRatio_FO_EighthO_SetD[i];
									NumOfFO_EighthO_SetD++;
								}
								if(m == 9){
									ResonanceRatio_FO_NinthO_SetD[i] = period_FO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FO_NinthO_SetD[i]<<endl;
									
									OverallResonance_FO_NinthO_SetD = OverallResonance_FO_NinthO_SetD + ResonanceRatio_FO_NinthO_SetD[i];
									NumOfFO_NinthO_SetD++;
								}
							}
						}
					}
					if(!DoBoundsCut || ((ratio_temp_2O <= (1. + bound)) && (ratio_temp_2O >= (1. - bound))) || ((ratio_temp_2O <= (0.5 + bound)) && (ratio_temp_2O >= (0.5 - bound))) || ((ratio_temp_2O <= (1.5 + bound)) && (ratio_temp_2O >= (1.5 - bound))) || ((ratio_temp_2O <= (2.0 + bound)) && (ratio_temp_2O >= (2.0 - bound))) || ((ratio_temp_2O <= (2.5 + bound)) && (ratio_temp_2O >= (2.5 - bound))) || ((ratio_temp_2O <= (3.0 + bound)) && (ratio_temp_2O >= (3.0 - bound))) || ((ratio_temp_2O <= (3.5 + bound)) && (ratio_temp_2O >= (3.5 - bound))) || ((ratio_temp_2O <= (4.0 + bound)) && (ratio_temp_2O >= (4.0 - bound))) || ((ratio_temp_2O <= (4.5 + bound)) && (ratio_temp_2O >= (4.5 - bound))) || ((ratio_temp_2O <= (5.0 + bound)) && (ratio_temp_2O >= (5.0 - bound))) || ((ratio_temp_2O <= (5.5 + bound)) && (ratio_temp_2O >= (5.5 - bound))) || ((ratio_temp_2O <= (6.0 + bound)) && (ratio_temp_2O >= (6.0 - bound)))){
						//2O mode//
						if((growth > 0.0 || !DoPosGrowthCut) && (growth_2O > 0)){
							cout<<"Resonance Ratios With 2O..."<<endl;
							//SetA
							if(j == 0){
								IsSelected_2O_SetA[i] = true;
								if(m == 0){
									ResonanceRatio_2O_FU_SetA[i] = (period_2O_SetA[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_FU_SetA[i]<<endl;
									
									OverallResonance_2O_FU_SetA = OverallResonance_2O_FU_SetA + ResonanceRatio_2O_FU_SetA[i];
									NumOf2O_FU_SetA++;
								}
								if(m == 1){
									ResonanceRatio_2O_FO_SetA[i] = (period_2O_SetA[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_FO_SetA[i]<<endl;
									
									OverallResonance_2O_FO_SetA = OverallResonance_2O_FO_SetA + ResonanceRatio_2O_FO_SetA[i];
									NumOf2O_FO_SetA++;
								}
								if(m == 2){
									ResonanceRatio_2O_2O_SetA[i] = (period_2O_SetA[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_2O_SetA[i]<<endl;
									
									OverallResonance_2O_2O_SetA = OverallResonance_2O_2O_SetA + ResonanceRatio_2O_2O_SetA[i];
									NumOf2O_2O_SetA++;
								}
								if(m == 3){
									ResonanceRatio_2O_ThirdO_SetA[i] = (period_2O_SetA[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_ThirdO_SetA[i]<<endl;
									
									OverallResonance_2O_ThirdO_SetA = OverallResonance_2O_ThirdO_SetA + ResonanceRatio_2O_ThirdO_SetA[i];
									NumOf2O_ThirdO_SetA++;
								}
								if(m == 4){
									ResonanceRatio_2O_ForthO_SetA[i] = (period_2O_SetA[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_ForthO_SetA[i]<<endl;
									
									OverallResonance_2O_ForthO_SetA = OverallResonance_2O_ForthO_SetA + ResonanceRatio_2O_ForthO_SetA[i];
									NumOf2O_ForthO_SetA++;
								}
								if(m == 5){
									ResonanceRatio_2O_FifthO_SetA[i] = (period_2O_SetA[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_FifthO_SetA[i]<<endl;
									
									OverallResonance_2O_FifthO_SetA = OverallResonance_2O_FifthO_SetA + ResonanceRatio_2O_FifthO_SetA[i];
									NumOf2O_FifthO_SetA++;
								}
								if(m == 6){
									ResonanceRatio_2O_SixthO_SetA[i] = (period_2O_SetA[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_SixthO_SetA[i]<<endl;
									
									OverallResonance_2O_SixthO_SetA = OverallResonance_2O_SixthO_SetA + ResonanceRatio_2O_SixthO_SetA[i];
									NumOf2O_SixthO_SetA++;
								}
								if(m == 7){
									ResonanceRatio_2O_SeventhO_SetA[i] = (period_2O_SetA[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_SeventhO_SetA[i]<<endl;
									
									OverallResonance_2O_SeventhO_SetA = OverallResonance_2O_SeventhO_SetA + ResonanceRatio_2O_SeventhO_SetA[i];
									NumOf2O_SeventhO_SetA++;
								}
								if(m == 8){
									ResonanceRatio_2O_EighthO_SetA[i] = (period_2O_SetA[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_EighthO_SetA[i]<<endl;
									
									OverallResonance_2O_EighthO_SetA = OverallResonance_2O_EighthO_SetA + ResonanceRatio_2O_EighthO_SetA[i];
									NumOf2O_EighthO_SetA++;
								}
								if(m == 9){
									ResonanceRatio_2O_NinthO_SetA[i] = (period_2O_SetA[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_NinthO_SetA[i]<<endl;
									
									OverallResonance_2O_NinthO_SetA = OverallResonance_2O_NinthO_SetA + ResonanceRatio_2O_NinthO_SetA[i];
									NumOf2O_NinthO_SetA++;
								}
							}
							//SetB
							if(j == 1){
								IsSelected_2O_SetB[i] = true;
								if(m == 0){
									ResonanceRatio_2O_FU_SetB[i] = (period_2O_SetB[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_FU_SetB[i]<<endl;
									
									OverallResonance_2O_FU_SetB = OverallResonance_2O_FU_SetB + ResonanceRatio_2O_FU_SetB[i];
									NumOf2O_2O_SetB++;
								}
								if(m == 1){
									ResonanceRatio_2O_FO_SetB[i] = (period_2O_SetB[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_FO_SetB[i]<<endl;
									
									OverallResonance_2O_FO_SetB = OverallResonance_2O_FO_SetB + ResonanceRatio_2O_FO_SetB[i];
									NumOf2O_FO_SetB++;
								}
								if(m == 2){
									ResonanceRatio_2O_2O_SetB[i] = (period_2O_SetB[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_2O_SetB[i]<<endl;
									
									OverallResonance_2O_2O_SetB = OverallResonance_2O_2O_SetB + ResonanceRatio_2O_2O_SetB[i];
									NumOf2O_2O_SetB++;
								}
								if(m == 3){
									ResonanceRatio_2O_ThirdO_SetB[i] = (period_2O_SetB[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_ThirdO_SetB[i]<<endl;
									
									OverallResonance_2O_ThirdO_SetB = OverallResonance_2O_ThirdO_SetB + ResonanceRatio_2O_ThirdO_SetB[i];
									NumOf2O_ThirdO_SetB++;
								}
								if(m == 4){
									ResonanceRatio_2O_ForthO_SetB[i] = (period_2O_SetB[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_ForthO_SetB[i]<<endl;
									
									OverallResonance_2O_ForthO_SetB = OverallResonance_2O_ForthO_SetB + ResonanceRatio_2O_ForthO_SetB[i];
									NumOf2O_ForthO_SetB++;
								}
								if(m == 5){
									ResonanceRatio_2O_FifthO_SetB[i] = (period_2O_SetB[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_FifthO_SetB[i]<<endl;
									
									OverallResonance_2O_FifthO_SetB = OverallResonance_2O_FifthO_SetB + ResonanceRatio_2O_FifthO_SetB[i];
									NumOf2O_FifthO_SetB++;
								}
								if(m == 6){
									ResonanceRatio_2O_SixthO_SetB[i] = (period_2O_SetB[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_SixthO_SetB[i]<<endl;
									
									OverallResonance_2O_SixthO_SetB = OverallResonance_2O_SixthO_SetB + ResonanceRatio_2O_SixthO_SetB[i];
									NumOf2O_SixthO_SetB++;
								}
								if(m == 7){
									ResonanceRatio_2O_SeventhO_SetB[i] = (period_2O_SetB[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_SeventhO_SetB[i]<<endl;
									
									OverallResonance_2O_SeventhO_SetB = OverallResonance_2O_SeventhO_SetB + ResonanceRatio_2O_SeventhO_SetB[i];
									NumOf2O_SeventhO_SetB++;
								}
								if(m == 8){
									ResonanceRatio_2O_EighthO_SetB[i] = (period_2O_SetB[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_EighthO_SetB[i]<<endl;
									
									OverallResonance_2O_EighthO_SetB = OverallResonance_2O_EighthO_SetB + ResonanceRatio_2O_EighthO_SetB[i];
									NumOf2O_EighthO_SetB++;
								}
								if(m == 9){
									ResonanceRatio_2O_NinthO_SetB[i] = (period_2O_SetB[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_NinthO_SetB[i]<<endl;
									
									OverallResonance_2O_NinthO_SetB = OverallResonance_2O_NinthO_SetB + ResonanceRatio_2O_NinthO_SetB[i];
									NumOf2O_NinthO_SetB++;
								}
							}
							//SetC
							if(j == 2){
								IsSelected_2O_SetC[i] = true;
								if(m == 0){
									ResonanceRatio_2O_FU_SetC[i] = (period_2O_SetC[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_FU_SetC[i]<<endl;
									
									OverallResonance_2O_FU_SetC = OverallResonance_2O_FU_SetC + ResonanceRatio_2O_FU_SetC[i];
									NumOf2O_FU_SetC++;
								}
								if(m == 1){
									ResonanceRatio_2O_FO_SetC[i] = (period_2O_SetC[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_FO_SetC[i]<<endl;
									
									OverallResonance_2O_FO_SetC = OverallResonance_2O_FO_SetC + ResonanceRatio_2O_FO_SetC[i];
									NumOf2O_FO_SetC++;
								}
								if(m == 2){
									ResonanceRatio_2O_2O_SetC[i] = (period_2O_SetC[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_2O_SetC[i]<<endl;
									
									OverallResonance_2O_2O_SetC = OverallResonance_2O_2O_SetC + ResonanceRatio_2O_2O_SetC[i];
									NumOf2O_2O_SetC++;
								}
								if(m == 3){
									ResonanceRatio_2O_ThirdO_SetC[i] = (period_2O_SetC[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_ThirdO_SetC[i]<<endl;
									
									OverallResonance_2O_ThirdO_SetC = OverallResonance_2O_ThirdO_SetC + ResonanceRatio_2O_ThirdO_SetC[i];
									NumOf2O_ThirdO_SetC++;
								}
								if(m == 4){
									ResonanceRatio_2O_ForthO_SetC[i] = (period_2O_SetC[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_ForthO_SetC[i]<<endl;
									
									OverallResonance_2O_ForthO_SetC = OverallResonance_2O_ForthO_SetC + ResonanceRatio_2O_ForthO_SetC[i];
									NumOf2O_ForthO_SetC++;
								}
								if(m == 5){
									ResonanceRatio_2O_FifthO_SetC[i] = (period_2O_SetC[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_FifthO_SetC[i]<<endl;
									
									OverallResonance_2O_FifthO_SetC = OverallResonance_2O_FifthO_SetC + ResonanceRatio_2O_FifthO_SetC[i];
									NumOf2O_FifthO_SetC++;
								}
								if(m == 6){
									ResonanceRatio_2O_SixthO_SetC[i] = (period_2O_SetC[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_SixthO_SetC[i]<<endl;
									
									OverallResonance_2O_SixthO_SetC = OverallResonance_2O_SixthO_SetC + ResonanceRatio_2O_SixthO_SetC[i];
									NumOf2O_SixthO_SetC++;
								}
								if(m == 7){
									ResonanceRatio_2O_SeventhO_SetC[i] = (period_2O_SetC[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_SeventhO_SetC[i]<<endl;
									
									OverallResonance_2O_SeventhO_SetC = OverallResonance_2O_SeventhO_SetC + ResonanceRatio_2O_SeventhO_SetC[i];
									NumOf2O_SeventhO_SetC++;
								}
								if(m == 8){
									ResonanceRatio_2O_EighthO_SetC[i] = (period_2O_SetC[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_EighthO_SetC[i]<<endl;
									
									OverallResonance_2O_EighthO_SetC = OverallResonance_2O_EighthO_SetC + ResonanceRatio_2O_EighthO_SetC[i];
									NumOf2O_EighthO_SetC++;
								}
								if(m == 9){
									ResonanceRatio_2O_NinthO_SetC[i] = (period_2O_SetC[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_NinthO_SetC[i]<<endl;
									
									OverallResonance_2O_NinthO_SetC = OverallResonance_2O_NinthO_SetC + ResonanceRatio_2O_NinthO_SetC[i];
									NumOf2O_NinthO_SetC++;
								}
							}
							//SetD
							if(j == 3){
								IsSelected_2O_SetD[i] = true;
								if(m == 0){
									ResonanceRatio_2O_FU_SetD[i] = (period_2O_SetD[i])/period;
									cout<<m<<": "<<ResonanceRatio_FU_2O_SetD[i]<<endl;
									
									OverallResonance_2O_FU_SetD = OverallResonance_2O_FU_SetD + ResonanceRatio_2O_FU_SetD[i];
									NumOf2O_FU_SetD++;
								}
								if(m == 1){
									ResonanceRatio_2O_FO_SetD[i] = (period_2O_SetD[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_FO_SetD[i]<<endl;
									
									OverallResonance_2O_FO_SetD = OverallResonance_2O_FO_SetD + ResonanceRatio_2O_FO_SetD[i];
									NumOf2O_FO_SetD++;
								}
								if(m == 2){
									ResonanceRatio_2O_2O_SetD[i] = (period_2O_SetD[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_2O_SetD[i]<<endl;
									
									OverallResonance_2O_2O_SetD = OverallResonance_2O_2O_SetD + ResonanceRatio_2O_2O_SetD[i];
									NumOf2O_2O_SetD++;
								}
								if(m == 3){
									ResonanceRatio_2O_ThirdO_SetD[i] = (period_2O_SetD[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_ThirdO_SetD[i]<<endl;
									
									OverallResonance_2O_ThirdO_SetD = OverallResonance_2O_ThirdO_SetD + ResonanceRatio_2O_ThirdO_SetD[i];
									NumOf2O_ThirdO_SetD++;
								}
								if(m == 4){
									ResonanceRatio_2O_ForthO_SetD[i] = (period_2O_SetD[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_ForthO_SetD[i]<<endl;
									
									OverallResonance_2O_ForthO_SetD = OverallResonance_2O_ForthO_SetD + ResonanceRatio_2O_ForthO_SetD[i];
									NumOf2O_ForthO_SetD++;
								}
								if(m == 5){
									ResonanceRatio_2O_FifthO_SetD[i] = (period_2O_SetD[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_FifthO_SetD[i]<<endl;
									
									OverallResonance_2O_FifthO_SetD = OverallResonance_2O_FifthO_SetD + ResonanceRatio_2O_FifthO_SetD[i];
									NumOf2O_FifthO_SetD++;
								}
								if(m == 6){
									ResonanceRatio_2O_SixthO_SetD[i] = (period_2O_SetD[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_SixthO_SetD[i]<<endl;
									
									OverallResonance_2O_SixthO_SetD = OverallResonance_2O_SixthO_SetD + ResonanceRatio_2O_SixthO_SetD[i];
									NumOf2O_SixthO_SetD++;
								}
								if(m == 7){
									ResonanceRatio_2O_SeventhO_SetD[i] = (period_2O_SetD[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_SeventhO_SetD[i]<<endl;
									
									OverallResonance_2O_SeventhO_SetD = OverallResonance_2O_SeventhO_SetD + ResonanceRatio_2O_SeventhO_SetD[i];
									NumOf2O_SeventhO_SetD++;
								}
								if(m == 8){
									ResonanceRatio_2O_EighthO_SetD[i] = (period_2O_SetD[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_EighthO_SetD[i]<<endl;
									
									OverallResonance_2O_EighthO_SetD = OverallResonance_2O_EighthO_SetD + ResonanceRatio_2O_EighthO_SetD[i];
									NumOf2O_EighthO_SetD++;
								}
								if(m == 9){
									ResonanceRatio_2O_NinthO_SetD[i] = (period_2O_SetD[i])/period;
									cout<<m<<": "<<ResonanceRatio_2O_NinthO_SetD[i]<<endl;
									
									OverallResonance_2O_NinthO_SetD = OverallResonance_2O_NinthO_SetD + ResonanceRatio_2O_NinthO_SetD[i];
									NumOf2O_NinthO_SetD++;
								}
							}
						}
					}
					if(!DoBoundsCut || ((ratio_temp_ThirdO <= (1. + bound)) && (ratio_temp_ThirdO >= (1. - bound))) || ((ratio_temp_ThirdO <= (0.5 + bound)) && (ratio_temp_ThirdO >= (0.5 - bound))) || ((ratio_temp_ThirdO <= (1.5 + bound)) && (ratio_temp_ThirdO >= (1.5 - bound))) || ((ratio_temp_ThirdO <= (2.0 + bound)) && (ratio_temp_ThirdO >= (2.0 - bound))) || ((ratio_temp_ThirdO <= (2.5 + bound)) && (ratio_temp_ThirdO >= (2.5 - bound))) || ((ratio_temp_ThirdO <= (3.0 + bound)) && (ratio_temp_ThirdO >= (3.0 - bound))) || ((ratio_temp_ThirdO <= (3.5 + bound)) && (ratio_temp_ThirdO >= (3.5 - bound))) || ((ratio_temp_ThirdO <= (4.0 + bound)) && (ratio_temp_ThirdO >= (4.0 - bound))) || ((ratio_temp_ThirdO <= (4.5 + bound)) && (ratio_temp_ThirdO >= (4.5 - bound))) || ((ratio_temp_ThirdO <= (5.0 + bound)) && (ratio_temp_ThirdO >= (5.0 - bound))) || ((ratio_temp_ThirdO <= (5.5 + bound)) && (ratio_temp_ThirdO >= (5.5 - bound))) || ((ratio_temp_ThirdO <= (6.0 + bound)) && (ratio_temp_ThirdO >= (6.0 - bound)))){
						//ThirdO mode//
						if((growth > 0.0 || !DoPosGrowthCut) && (growth_ThirdO > 0)){
							cout<<"Resonance Ratios With ThirdO..."<<endl;
							//SetA
							if(j == 0){
								IsSelected_ThirdO_SetA[i] = true;
								if(m == 0){
									ResonanceRatio_ThirdO_FU_SetA[i] = period_ThirdO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_FU_SetA[i]<<endl;
									
									OverallResonance_ThirdO_FU_SetA = OverallResonance_ThirdO_FU_SetA + ResonanceRatio_ThirdO_FU_SetA[i];
									NumOfThirdO_FU_SetA++;
								}
								if(m == 1){
									ResonanceRatio_ThirdO_FO_SetA[i] = period_ThirdO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_FO_SetA[i]<<endl;
									
									OverallResonance_ThirdO_FO_SetA = OverallResonance_ThirdO_FO_SetA + ResonanceRatio_ThirdO_FO_SetA[i];
									NumOfThirdO_FO_SetA++;
								}
								if(m == 2){
									ResonanceRatio_ThirdO_2O_SetA[i] = period_ThirdO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_2O_SetA[i]<<endl;
									
									OverallResonance_ThirdO_2O_SetA = OverallResonance_ThirdO_2O_SetA + ResonanceRatio_ThirdO_2O_SetA[i];
									NumOfThirdO_2O_SetA++;
								}
								if(m == 3){
									ResonanceRatio_ThirdO_ThirdO_SetA[i] = period_ThirdO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_ThirdO_SetA[i]<<endl;
									
									OverallResonance_ThirdO_ThirdO_SetA = OverallResonance_ThirdO_ThirdO_SetA + ResonanceRatio_ThirdO_ThirdO_SetA[i];
									NumOfThirdO_ThirdO_SetA++;
								}
								if(m == 4){
									ResonanceRatio_ThirdO_ForthO_SetA[i] = period_ThirdO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_ForthO_SetA[i]<<endl;
									
									OverallResonance_ThirdO_ForthO_SetA = OverallResonance_ThirdO_ForthO_SetA + ResonanceRatio_ThirdO_ForthO_SetA[i];
									NumOfThirdO_ForthO_SetA++;
								}
								if(m == 5){
									ResonanceRatio_ThirdO_FifthO_SetA[i] = period_ThirdO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_FifthO_SetA[i]<<endl;
									
									OverallResonance_ThirdO_FifthO_SetA = OverallResonance_ThirdO_FifthO_SetA + ResonanceRatio_ThirdO_FifthO_SetA[i];
									NumOfThirdO_FifthO_SetA++;
								}
								if(m == 6){
									ResonanceRatio_ThirdO_SixthO_SetA[i] = period_ThirdO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_SixthO_SetA[i]<<endl;
									
									OverallResonance_ThirdO_SixthO_SetA = OverallResonance_ThirdO_SixthO_SetA + ResonanceRatio_ThirdO_SixthO_SetA[i];
									NumOfThirdO_SixthO_SetA++;
								}
								if(m == 7){
									ResonanceRatio_ThirdO_SeventhO_SetA[i] = period_ThirdO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_SeventhO_SetA[i]<<endl;
									
									OverallResonance_ThirdO_SeventhO_SetA = OverallResonance_ThirdO_SeventhO_SetA + ResonanceRatio_ThirdO_SeventhO_SetA[i];
									NumOfThirdO_SeventhO_SetA++;
								}
								if(m == 8){
									ResonanceRatio_ThirdO_EighthO_SetA[i] = period_ThirdO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_EighthO_SetA[i]<<endl;
									
									OverallResonance_ThirdO_EighthO_SetA = OverallResonance_ThirdO_EighthO_SetA + ResonanceRatio_ThirdO_EighthO_SetA[i];
									NumOfThirdO_EighthO_SetA++;
								}
								if(m == 9){
									ResonanceRatio_ThirdO_NinthO_SetA[i] = period_ThirdO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_NinthO_SetA[i]<<endl;
									
									OverallResonance_ThirdO_NinthO_SetA = OverallResonance_ThirdO_NinthO_SetA + ResonanceRatio_ThirdO_NinthO_SetA[i];
									NumOfThirdO_NinthO_SetA++;
								}
							}
							//SetB
							if(j == 1){
								IsSelected_ThirdO_SetB[i] = true;
								if(m == 0){
									ResonanceRatio_ThirdO_FU_SetB[i] = period_ThirdO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_FU_SetB[i]<<endl;
									
									OverallResonance_ThirdO_FU_SetB = OverallResonance_ThirdO_FU_SetB + ResonanceRatio_ThirdO_FU_SetB[i];
									NumOfThirdO_FU_SetB++;
								}
								if(m == 1){
									ResonanceRatio_ThirdO_FO_SetB[i] = period_ThirdO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_FO_SetB[i]<<endl;
									
									OverallResonance_ThirdO_FO_SetB = OverallResonance_ThirdO_FO_SetB + ResonanceRatio_ThirdO_FO_SetB[i];
									NumOfThirdO_FO_SetB++;
								}
								if(m == 2){
									ResonanceRatio_ThirdO_2O_SetB[i] = period_ThirdO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_2O_SetB[i]<<endl;
									
									OverallResonance_ThirdO_2O_SetB = OverallResonance_ThirdO_2O_SetB + ResonanceRatio_ThirdO_2O_SetB[i];
									NumOfThirdO_2O_SetB++;
								}
								if(m == 3){
									ResonanceRatio_ThirdO_ThirdO_SetB[i] = period_ThirdO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_ThirdO_SetB[i]<<endl;
									
									OverallResonance_ThirdO_ThirdO_SetB = OverallResonance_ThirdO_ThirdO_SetB + ResonanceRatio_ThirdO_ThirdO_SetB[i];
									NumOfThirdO_ThirdO_SetB++;
								}
								if(m == 4){
									ResonanceRatio_ThirdO_ForthO_SetB[i] = period_ThirdO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_ForthO_SetB[i]<<endl;
									
									OverallResonance_ThirdO_ForthO_SetB = OverallResonance_ThirdO_ForthO_SetB + ResonanceRatio_ThirdO_ForthO_SetB[i];
									NumOfThirdO_ForthO_SetB++;
								}
								if(m == 5){
									ResonanceRatio_ThirdO_FifthO_SetB[i] = period_ThirdO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_FifthO_SetB[i]<<endl;
									
									OverallResonance_ThirdO_FifthO_SetB = OverallResonance_ThirdO_FifthO_SetB + ResonanceRatio_ThirdO_FifthO_SetB[i];
									NumOfThirdO_FifthO_SetB++;
								}
								if(m == 6){
									ResonanceRatio_ThirdO_SixthO_SetB[i] = period_ThirdO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_SixthO_SetB[i]<<endl;
									
									OverallResonance_ThirdO_SixthO_SetB = OverallResonance_ThirdO_SixthO_SetB + ResonanceRatio_ThirdO_SixthO_SetB[i];
									NumOfThirdO_SixthO_SetB++;
								}
								if(m == 7){
									ResonanceRatio_ThirdO_SeventhO_SetB[i] = period_ThirdO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_SeventhO_SetB[i]<<endl;
									
									OverallResonance_ThirdO_SeventhO_SetB = OverallResonance_ThirdO_SeventhO_SetB + ResonanceRatio_ThirdO_SeventhO_SetB[i];
									NumOfThirdO_SeventhO_SetB++;
								}
								if(m == 8){
									ResonanceRatio_ThirdO_EighthO_SetB[i] = period_ThirdO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_EighthO_SetB[i]<<endl;
									
									OverallResonance_ThirdO_EighthO_SetB = OverallResonance_ThirdO_EighthO_SetB + ResonanceRatio_ThirdO_EighthO_SetB[i];
									NumOfThirdO_EighthO_SetB++;
								}
								if(m == 9){
									ResonanceRatio_ThirdO_NinthO_SetB[i] = period_ThirdO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_NinthO_SetB[i]<<endl;
									
									OverallResonance_ThirdO_NinthO_SetB = OverallResonance_ThirdO_NinthO_SetB + ResonanceRatio_ThirdO_NinthO_SetB[i];
									NumOfThirdO_NinthO_SetB++;
								}
							}
							//SetC
							if(j == 2){
								IsSelected_ThirdO_SetC[i] = true;
								if(m == 0){
									ResonanceRatio_ThirdO_FU_SetC[i] = period_ThirdO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_FU_SetC[i]<<endl;
									
									OverallResonance_ThirdO_FU_SetC = OverallResonance_ThirdO_FU_SetC + ResonanceRatio_ThirdO_FU_SetC[i];
									NumOfThirdO_FU_SetC++;
								}
								if(m == 1){
									ResonanceRatio_ThirdO_FO_SetC[i] = period_ThirdO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_FO_SetC[i]<<endl;
									
									OverallResonance_ThirdO_FO_SetC = OverallResonance_ThirdO_FO_SetC + ResonanceRatio_ThirdO_FO_SetC[i];
									NumOfThirdO_FO_SetC++;
								}
								if(m == 2){
									ResonanceRatio_ThirdO_2O_SetC[i] = period_ThirdO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_2O_SetC[i]<<endl;
									
									OverallResonance_ThirdO_2O_SetC = OverallResonance_ThirdO_2O_SetC + ResonanceRatio_ThirdO_2O_SetC[i];
									NumOfThirdO_2O_SetC++;
								}
								if(m == 3){
									ResonanceRatio_ThirdO_ThirdO_SetC[i] = period_ThirdO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_ThirdO_SetC[i]<<endl;
									
									OverallResonance_ThirdO_ThirdO_SetC = OverallResonance_ThirdO_ThirdO_SetC + ResonanceRatio_ThirdO_ThirdO_SetC[i];
									NumOfThirdO_ThirdO_SetC++;
								}
								if(m == 4){
									ResonanceRatio_ThirdO_ForthO_SetC[i] = period_ThirdO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_ForthO_SetC[i]<<endl;
									
									OverallResonance_ThirdO_ForthO_SetC = OverallResonance_ThirdO_ForthO_SetC + ResonanceRatio_ThirdO_ForthO_SetC[i];
									NumOfThirdO_ForthO_SetC++;
								}
								if(m == 5){
									ResonanceRatio_ThirdO_FifthO_SetC[i] = period_ThirdO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_FifthO_SetC[i]<<endl;
									
									OverallResonance_ThirdO_FifthO_SetC = OverallResonance_ThirdO_FifthO_SetC + ResonanceRatio_ThirdO_FifthO_SetC[i];
									NumOfThirdO_FifthO_SetC++;
								}
								if(m == 6){
									ResonanceRatio_ThirdO_SixthO_SetC[i] = period_ThirdO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_SixthO_SetC[i]<<endl;
									
									OverallResonance_ThirdO_SixthO_SetC = OverallResonance_ThirdO_SixthO_SetC + ResonanceRatio_ThirdO_SixthO_SetC[i];
									NumOfThirdO_SixthO_SetC++;
								}
								if(m == 7){
									ResonanceRatio_ThirdO_SeventhO_SetC[i] = period_ThirdO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_SeventhO_SetC[i]<<endl;
									
									OverallResonance_ThirdO_SeventhO_SetC = OverallResonance_ThirdO_SeventhO_SetC + ResonanceRatio_ThirdO_SeventhO_SetC[i];
									NumOfThirdO_SeventhO_SetC++;
								}
								if(m == 8){
									ResonanceRatio_ThirdO_EighthO_SetC[i] = period_ThirdO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_EighthO_SetC[i]<<endl;
									
									OverallResonance_ThirdO_EighthO_SetC = OverallResonance_ThirdO_EighthO_SetC + ResonanceRatio_ThirdO_EighthO_SetC[i];
									NumOfThirdO_EighthO_SetC++;
								}
								if(m == 9){
									ResonanceRatio_ThirdO_NinthO_SetC[i] = period_ThirdO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_NinthO_SetC[i]<<endl;
									
									OverallResonance_ThirdO_NinthO_SetC = OverallResonance_ThirdO_NinthO_SetC + ResonanceRatio_ThirdO_NinthO_SetC[i];
									NumOfThirdO_NinthO_SetC++;
								}
							}
							//SetD
							if(j == 3){
								IsSelected_ThirdO_SetD[i] = true;
								if(m == 0){
									ResonanceRatio_ThirdO_FU_SetD[i] = period_ThirdO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_FU_SetD[i]<<endl;
									
									OverallResonance_ThirdO_FU_SetD = OverallResonance_ThirdO_FU_SetD + ResonanceRatio_ThirdO_FU_SetD[i];
									NumOfThirdO_FU_SetD++;
								}
								if(m == 1){
									ResonanceRatio_ThirdO_FO_SetD[i] = period_ThirdO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_FO_SetD[i]<<endl;
									
									OverallResonance_ThirdO_FO_SetD = OverallResonance_ThirdO_FO_SetD + ResonanceRatio_ThirdO_FO_SetD[i];
									NumOfThirdO_FO_SetD++;
								}
								if(m == 2){
									ResonanceRatio_ThirdO_2O_SetD[i] = period_ThirdO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_2O_SetD[i]<<endl;
									
									OverallResonance_ThirdO_2O_SetD = OverallResonance_ThirdO_2O_SetD + ResonanceRatio_ThirdO_2O_SetD[i];
									NumOfThirdO_2O_SetD++;
								}
								if(m == 3){
									ResonanceRatio_ThirdO_ThirdO_SetD[i] = period_ThirdO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_ThirdO_SetD[i]<<endl;
									
									OverallResonance_ThirdO_ThirdO_SetD = OverallResonance_ThirdO_ThirdO_SetD + ResonanceRatio_ThirdO_ThirdO_SetD[i];
									NumOfThirdO_ThirdO_SetD++;
								}
								if(m == 4){
									ResonanceRatio_ThirdO_ForthO_SetD[i] = period_ThirdO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_ForthO_SetD[i]<<endl;
									
									OverallResonance_ThirdO_ForthO_SetD = OverallResonance_ThirdO_ForthO_SetD + ResonanceRatio_ThirdO_ForthO_SetD[i];
									NumOfThirdO_ForthO_SetD++;
								}
								if(m == 5){
									ResonanceRatio_ThirdO_FifthO_SetD[i] = period_ThirdO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_FifthO_SetD[i]<<endl;
									
									OverallResonance_ThirdO_FifthO_SetD = OverallResonance_ThirdO_FifthO_SetD + ResonanceRatio_ThirdO_FifthO_SetD[i];
									NumOfThirdO_FifthO_SetD++;
								}
								if(m == 6){
									ResonanceRatio_ThirdO_SixthO_SetD[i] = period_ThirdO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_SixthO_SetD[i]<<endl;
									
									OverallResonance_ThirdO_SixthO_SetD = OverallResonance_ThirdO_SixthO_SetD + ResonanceRatio_ThirdO_SixthO_SetD[i];
									NumOfThirdO_SixthO_SetD++;
								}
								if(m == 7){
									ResonanceRatio_ThirdO_SeventhO_SetD[i] = period_ThirdO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_SeventhO_SetD[i]<<endl;
									
									OverallResonance_ThirdO_SeventhO_SetD = OverallResonance_ThirdO_SeventhO_SetD + ResonanceRatio_ThirdO_SeventhO_SetD[i];
									NumOfThirdO_SeventhO_SetD++;
								}
								if(m == 8){
									ResonanceRatio_ThirdO_EighthO_SetD[i] = period_ThirdO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_EighthO_SetD[i]<<endl;
									
									OverallResonance_ThirdO_EighthO_SetD = OverallResonance_ThirdO_EighthO_SetD + ResonanceRatio_ThirdO_EighthO_SetD[i];
									NumOfThirdO_EighthO_SetD++;
								}
								if(m == 9){
									ResonanceRatio_ThirdO_NinthO_SetD[i] = period_ThirdO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ThirdO_NinthO_SetD[i]<<endl;
									
									OverallResonance_ThirdO_NinthO_SetD = OverallResonance_ThirdO_NinthO_SetD + ResonanceRatio_ThirdO_NinthO_SetD[i];
									NumOfThirdO_NinthO_SetD++;
								}
							}
						}
					}
					if(!DoBoundsCut || ((ratio_temp_ForthO <= (1. + bound)) && (ratio_temp_ForthO >= (1. - bound))) || ((ratio_temp_ForthO <= (0.5 + bound)) && (ratio_temp_ForthO >= (0.5 - bound))) || ((ratio_temp_ForthO <= (1.5 + bound)) && (ratio_temp_ForthO >= (1.5 - bound))) || ((ratio_temp_ForthO <= (2.0 + bound)) && (ratio_temp_ForthO >= (2.0 - bound))) || ((ratio_temp_ForthO <= (2.5 + bound)) && (ratio_temp_ForthO >= (2.5 - bound))) || ((ratio_temp_ForthO <= (3.0 + bound)) && (ratio_temp_ForthO >= (3.0 - bound))) || ((ratio_temp_ForthO <= (3.5 + bound)) && (ratio_temp_ForthO >= (3.5 - bound))) || ((ratio_temp_ForthO <= (4.0 + bound)) && (ratio_temp_ForthO >= (4.0 - bound))) || ((ratio_temp_ForthO <= (4.5 + bound)) && (ratio_temp_ForthO >= (4.5 - bound))) || ((ratio_temp_ForthO <= (5.0 + bound)) && (ratio_temp_ForthO >= (5.0 - bound))) || ((ratio_temp_ForthO <= (5.5 + bound)) && (ratio_temp_ForthO >= (5.5 - bound))) || ((ratio_temp_ForthO <= (6.0 + bound)) && (ratio_temp_ForthO >= (6.0 - bound)))){
						//ForthO mode//
						if((growth > 0.0 || !DoPosGrowthCut) && (growth_ForthO > 0)){
							cout<<"Resonance Ratios With ForthO..."<<endl;
							//SetA
							if(j == 0){
								IsSelected_ForthO_SetA[i] = true;
								if(m == 0){
									ResonanceRatio_ForthO_FU_SetA[i] = period_ForthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_FU_SetA[i]<<endl;
									
									OverallResonance_ForthO_FU_SetA = OverallResonance_ForthO_FU_SetA + ResonanceRatio_ForthO_FU_SetA[i];
									NumOfForthO_FU_SetA++;
								}
								if(m == 1){
									ResonanceRatio_ForthO_FO_SetA[i] = period_ForthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_FO_SetA[i]<<endl;
									
									OverallResonance_ForthO_FO_SetA = OverallResonance_ForthO_FO_SetA + ResonanceRatio_ForthO_FO_SetA[i];
									NumOfForthO_FO_SetA++;
								}
								if(m == 2){
									ResonanceRatio_ForthO_2O_SetA[i] = period_ForthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_2O_SetA[i]<<endl;
									
									OverallResonance_ForthO_2O_SetA = OverallResonance_ForthO_2O_SetA + ResonanceRatio_ForthO_2O_SetA[i];
									NumOfForthO_2O_SetA++;
								}
								if(m == 3){
									ResonanceRatio_ForthO_ThirdO_SetA[i] = period_ForthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_ThirdO_SetA[i]<<endl;
									
									OverallResonance_ForthO_ThirdO_SetA = OverallResonance_ForthO_ThirdO_SetA + ResonanceRatio_ForthO_ThirdO_SetA[i];
									NumOfForthO_ThirdO_SetA++;
								}
								if(m == 4){
									ResonanceRatio_ForthO_ForthO_SetA[i] = period_ForthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_ForthO_SetA[i]<<endl;
									
									OverallResonance_ForthO_ForthO_SetA = OverallResonance_ForthO_ForthO_SetA + ResonanceRatio_ForthO_ForthO_SetA[i];
									NumOfForthO_ForthO_SetA++;
								}
								if(m == 5){
									ResonanceRatio_ForthO_FifthO_SetA[i] = period_ForthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_FifthO_SetA[i]<<endl;
									
									OverallResonance_ForthO_FifthO_SetA = OverallResonance_ForthO_FifthO_SetA + ResonanceRatio_ForthO_FifthO_SetA[i];
									NumOfForthO_FifthO_SetA++;
								}
								if(m == 6){
									ResonanceRatio_ForthO_SixthO_SetA[i] = period_ForthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_SixthO_SetA[i]<<endl;
									
									OverallResonance_ForthO_SixthO_SetA = OverallResonance_ForthO_SixthO_SetA + ResonanceRatio_ForthO_SixthO_SetA[i];
									NumOfForthO_SixthO_SetA++;
								}
								if(m == 7){
									ResonanceRatio_ForthO_SeventhO_SetA[i] = period_ForthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_SeventhO_SetA[i]<<endl;
									
									OverallResonance_ForthO_SeventhO_SetA = OverallResonance_ForthO_SeventhO_SetA + ResonanceRatio_ForthO_SeventhO_SetA[i];
									NumOfForthO_SeventhO_SetA++;
								}
								if(m == 8){
									ResonanceRatio_ForthO_EighthO_SetA[i] = period_ForthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_EighthO_SetA[i]<<endl;
									
									OverallResonance_ForthO_EighthO_SetA = OverallResonance_ForthO_EighthO_SetA + ResonanceRatio_ForthO_EighthO_SetA[i];
									NumOfForthO_EighthO_SetA++;
								}
								if(m == 9){
									ResonanceRatio_ForthO_NinthO_SetA[i] = period_ForthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_NinthO_SetA[i]<<endl;
									
									OverallResonance_ForthO_NinthO_SetA = OverallResonance_ForthO_NinthO_SetA + ResonanceRatio_ForthO_NinthO_SetA[i];
									NumOfForthO_NinthO_SetA++;
								}
							}
							//SetB
							if(j == 1){
								IsSelected_ForthO_SetB[i] = true;
								if(m == 0){
									ResonanceRatio_ForthO_FU_SetB[i] = period_ForthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_FU_SetB[i]<<endl;
									
									OverallResonance_ForthO_FU_SetB = OverallResonance_ForthO_FU_SetB + ResonanceRatio_ForthO_FU_SetB[i];
									NumOfForthO_FU_SetB++;
								}
								if(m == 1){
									ResonanceRatio_ForthO_FO_SetB[i] = period_ForthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_FO_SetB[i]<<endl;
									
									OverallResonance_ForthO_FO_SetB = OverallResonance_ForthO_FO_SetB + ResonanceRatio_ForthO_FO_SetB[i];
									NumOfForthO_FO_SetB++;
								}
								if(m == 2){
									ResonanceRatio_ForthO_2O_SetB[i] = period_ForthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_2O_SetB[i]<<endl;
									
									OverallResonance_ForthO_2O_SetB = OverallResonance_ForthO_2O_SetB + ResonanceRatio_ForthO_2O_SetB[i];
									NumOfForthO_2O_SetB++;
								}
								if(m == 3){
									ResonanceRatio_ForthO_ThirdO_SetB[i] = period_ForthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_ThirdO_SetB[i]<<endl;
									
									OverallResonance_ForthO_ThirdO_SetB = OverallResonance_ForthO_ThirdO_SetB + ResonanceRatio_ForthO_ThirdO_SetB[i];
									NumOfForthO_ThirdO_SetB++;
								}
								if(m == 4){
									ResonanceRatio_ForthO_ForthO_SetB[i] = period_ForthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_ForthO_SetB[i]<<endl;
									
									OverallResonance_ForthO_ForthO_SetB = OverallResonance_ForthO_ForthO_SetB + ResonanceRatio_ForthO_ForthO_SetB[i];
									NumOfForthO_ForthO_SetB++;
								}
								if(m == 5){
									ResonanceRatio_ForthO_FifthO_SetB[i] = period_ForthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_FifthO_SetB[i]<<endl;
									
									OverallResonance_ForthO_FifthO_SetB = OverallResonance_ForthO_FifthO_SetB + ResonanceRatio_ForthO_FifthO_SetB[i];
									NumOfForthO_FifthO_SetB++;
								}
								if(m == 6){
									ResonanceRatio_ForthO_SixthO_SetB[i] = period_ForthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_SixthO_SetB[i]<<endl;
									
									OverallResonance_ForthO_SixthO_SetB = OverallResonance_ForthO_SixthO_SetB + ResonanceRatio_ForthO_SixthO_SetB[i];
									NumOfForthO_SixthO_SetB++;
								}
								if(m == 7){
									ResonanceRatio_ForthO_SeventhO_SetB[i] = period_ForthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_SeventhO_SetB[i]<<endl;
									
									OverallResonance_ForthO_SeventhO_SetB = OverallResonance_ForthO_SeventhO_SetB + ResonanceRatio_ForthO_SeventhO_SetB[i];
									NumOfForthO_SeventhO_SetB++;
								}
								if(m == 8){
									ResonanceRatio_ForthO_EighthO_SetB[i] = period_ForthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_EighthO_SetB[i]<<endl;
									
									OverallResonance_ForthO_EighthO_SetB = OverallResonance_ForthO_EighthO_SetB + ResonanceRatio_ForthO_EighthO_SetB[i];
									NumOfForthO_EighthO_SetB++;
								}
								if(m == 9){
									ResonanceRatio_ForthO_NinthO_SetB[i] = period_ForthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_NinthO_SetB[i]<<endl;
									
									OverallResonance_ForthO_NinthO_SetB = OverallResonance_ForthO_NinthO_SetB + ResonanceRatio_ForthO_NinthO_SetB[i];
									NumOfForthO_NinthO_SetB++;
								}
							}
							//SetC
							if(j == 2){
								IsSelected_ForthO_SetC[i] = true;
								if(m == 0){
									ResonanceRatio_ForthO_FU_SetC[i] = period_ForthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_FU_SetC[i]<<endl;
									
									OverallResonance_ForthO_FU_SetC = OverallResonance_ForthO_FU_SetC + ResonanceRatio_ForthO_FU_SetC[i];
									NumOfForthO_FU_SetC++;
								}
								if(m == 1){
									ResonanceRatio_ForthO_FO_SetC[i] = period_ForthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_FO_SetC[i]<<endl;
									
									OverallResonance_ForthO_FO_SetC = OverallResonance_ForthO_FO_SetC + ResonanceRatio_ForthO_FO_SetC[i];
									NumOfForthO_FO_SetC++;
								}
								if(m == 2){
									ResonanceRatio_ForthO_2O_SetC[i] = period_ForthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_2O_SetC[i]<<endl;
									
									OverallResonance_ForthO_2O_SetC = OverallResonance_ForthO_2O_SetC + ResonanceRatio_ForthO_2O_SetC[i];
									NumOfForthO_2O_SetC++;
								}
								if(m == 3){
									ResonanceRatio_ForthO_ThirdO_SetC[i] = period_ForthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_ThirdO_SetC[i]<<endl;
									
									OverallResonance_ForthO_ThirdO_SetC = OverallResonance_ForthO_ThirdO_SetC + ResonanceRatio_ForthO_ThirdO_SetC[i];
									NumOfForthO_ThirdO_SetC++;
								}
								if(m == 4){
									ResonanceRatio_ForthO_ForthO_SetC[i] = period_ForthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_ForthO_SetC[i]<<endl;
									
									OverallResonance_ForthO_ForthO_SetC = OverallResonance_ForthO_ForthO_SetC + ResonanceRatio_ForthO_ForthO_SetC[i];
									NumOfForthO_ForthO_SetC++;
								}
								if(m == 5){
									ResonanceRatio_ForthO_FifthO_SetC[i] = period_ForthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_FifthO_SetC[i]<<endl;
									
									OverallResonance_ForthO_FifthO_SetC = OverallResonance_ForthO_FifthO_SetC + ResonanceRatio_ForthO_FifthO_SetC[i];
									NumOfForthO_FifthO_SetC++;
								}
								if(m == 6){
									ResonanceRatio_ForthO_SixthO_SetC[i] = period_ForthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_SixthO_SetC[i]<<endl;
									
									OverallResonance_ForthO_SixthO_SetC = OverallResonance_ForthO_SixthO_SetC + ResonanceRatio_ForthO_SixthO_SetC[i];
									NumOfForthO_SixthO_SetC++;
								}
								if(m == 7){
									ResonanceRatio_ForthO_SeventhO_SetC[i] = period_ForthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_SeventhO_SetC[i]<<endl;
									
									OverallResonance_ForthO_SeventhO_SetC = OverallResonance_ForthO_SeventhO_SetC + ResonanceRatio_ForthO_SeventhO_SetC[i];
									NumOfForthO_SeventhO_SetC++;
								}
								if(m == 8){
									ResonanceRatio_ForthO_EighthO_SetC[i] = period_ForthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_EighthO_SetC[i]<<endl;
									
									OverallResonance_ForthO_EighthO_SetC = OverallResonance_ForthO_EighthO_SetC + ResonanceRatio_ForthO_EighthO_SetC[i];
									NumOfForthO_EighthO_SetC++;
								}
								if(m == 9){
									ResonanceRatio_ForthO_NinthO_SetC[i] = period_ForthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_NinthO_SetC[i]<<endl;
									
									OverallResonance_ForthO_NinthO_SetC = OverallResonance_ForthO_NinthO_SetC + ResonanceRatio_ForthO_NinthO_SetC[i];
									NumOfForthO_NinthO_SetC++;
								}
							}
							//SetD
							if(j == 3){
								IsSelected_ForthO_SetD[i] = true;
								if(m == 0){
									ResonanceRatio_ForthO_FU_SetD[i] = period_ForthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_FU_SetD[i]<<endl;
									
									OverallResonance_ForthO_FU_SetD = OverallResonance_ForthO_FU_SetD + ResonanceRatio_ForthO_FU_SetD[i];
									NumOfForthO_FU_SetD++;
								}
								if(m == 1){
									ResonanceRatio_ForthO_FO_SetD[i] = period_ForthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_FO_SetD[i]<<endl;
									
									OverallResonance_ForthO_FO_SetD = OverallResonance_ForthO_FO_SetD + ResonanceRatio_ForthO_FO_SetD[i];
									NumOfForthO_FO_SetD++;
								}
								if(m == 2){
									ResonanceRatio_ForthO_2O_SetD[i] = period_ForthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_2O_SetD[i]<<endl;
									
									OverallResonance_ForthO_2O_SetD = OverallResonance_ForthO_2O_SetD + ResonanceRatio_ForthO_2O_SetD[i];
									NumOfForthO_2O_SetD++;
								}
								if(m == 3){
									ResonanceRatio_ForthO_ThirdO_SetD[i] = period_ForthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_ThirdO_SetD[i]<<endl;
									
									OverallResonance_ForthO_ThirdO_SetD = OverallResonance_ForthO_ThirdO_SetD + ResonanceRatio_ForthO_ThirdO_SetD[i];
									NumOfForthO_ThirdO_SetD++;
								}
								if(m == 4){
									ResonanceRatio_ForthO_ForthO_SetD[i] = period_ForthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_ForthO_SetD[i]<<endl;
									
									OverallResonance_ForthO_ForthO_SetD = OverallResonance_ForthO_ForthO_SetD + ResonanceRatio_ForthO_ForthO_SetD[i];
									NumOfForthO_ForthO_SetD++;
								}
								if(m == 5){
									ResonanceRatio_ForthO_FifthO_SetD[i] = period_ForthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_FifthO_SetD[i]<<endl;
									
									OverallResonance_ForthO_FifthO_SetD = OverallResonance_ForthO_FifthO_SetD + ResonanceRatio_ForthO_FifthO_SetD[i];
									NumOfForthO_FifthO_SetD++;
								}
								if(m == 6){
									ResonanceRatio_ForthO_SixthO_SetD[i] = period_ForthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_SixthO_SetD[i]<<endl;
									
									OverallResonance_ForthO_SixthO_SetD = OverallResonance_ForthO_SixthO_SetD + ResonanceRatio_ForthO_SixthO_SetD[i];
									NumOfForthO_SixthO_SetD++;
								}
								if(m == 7){
									ResonanceRatio_ForthO_SeventhO_SetD[i] = period_ForthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_SeventhO_SetD[i]<<endl;
									
									OverallResonance_ForthO_SeventhO_SetD = OverallResonance_ForthO_SeventhO_SetD + ResonanceRatio_ForthO_SeventhO_SetD[i];
									NumOfForthO_SeventhO_SetD++;
								}
								if(m == 8){
									ResonanceRatio_ForthO_EighthO_SetD[i] = period_ForthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_EighthO_SetD[i]<<endl;
									
									OverallResonance_ForthO_EighthO_SetD = OverallResonance_ForthO_EighthO_SetD + ResonanceRatio_ForthO_EighthO_SetD[i];
									NumOfForthO_EighthO_SetD++;
								}
								if(m == 9){
									ResonanceRatio_ForthO_NinthO_SetD[i] = period_ForthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_ForthO_NinthO_SetD[i]<<endl;
									
									OverallResonance_ForthO_NinthO_SetD = OverallResonance_ForthO_NinthO_SetD + ResonanceRatio_ForthO_NinthO_SetD[i];
									NumOfForthO_NinthO_SetD++;
								}
							}
						}
					}
					if(!DoBoundsCut || ((ratio_temp_FifthO <= (1. + bound)) && (ratio_temp_FifthO >= (1. - bound))) || ((ratio_temp_FifthO <= (0.5 + bound)) && (ratio_temp_FifthO >= (0.5 - bound))) || ((ratio_temp_FifthO <= (1.5 + bound)) && (ratio_temp_FifthO >= (1.5 - bound))) || ((ratio_temp_FifthO <= (2.0 + bound)) && (ratio_temp_FifthO >= (2.0 - bound))) || ((ratio_temp_FifthO <= (2.5 + bound)) && (ratio_temp_FifthO >= (2.5 - bound))) || ((ratio_temp_FifthO <= (3.0 + bound)) && (ratio_temp_FifthO >= (3.0 - bound))) || ((ratio_temp_FifthO <= (3.5 + bound)) && (ratio_temp_FifthO >= (3.5 - bound))) || ((ratio_temp_FifthO <= (4.0 + bound)) && (ratio_temp_FifthO >= (4.0 - bound))) || ((ratio_temp_FifthO <= (4.5 + bound)) && (ratio_temp_FifthO >= (4.5 - bound))) || ((ratio_temp_FifthO <= (5.0 + bound)) && (ratio_temp_FifthO >= (5.0 - bound))) || ((ratio_temp_FifthO <= (5.5 + bound)) && (ratio_temp_FifthO >= (5.5 - bound))) || ((ratio_temp_FifthO <= (6.0 + bound)) && (ratio_temp_FifthO >= (6.0 - bound)))){
						//FifthO mode//
						if((growth > 0.0 || !DoPosGrowthCut) && (growth_FifthO > 0)){
							cout<<"Resonance Ratios With FifthO..."<<endl;
							//SetA
							if(j == 0){
								IsSelected_FifthO_SetA[i] = true;
								if(m == 0){
									ResonanceRatio_FifthO_FU_SetA[i] = period_FifthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_FU_SetA[i]<<endl;
									
									OverallResonance_FifthO_FU_SetA = OverallResonance_FifthO_FU_SetA + ResonanceRatio_FifthO_FU_SetA[i];
									NumOfFifthO_FU_SetA++;
								}
								if(m == 1){
									ResonanceRatio_FifthO_FO_SetA[i] = period_FifthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_FO_SetA[i]<<endl;
									
									OverallResonance_FifthO_FO_SetA = OverallResonance_FifthO_FO_SetA + ResonanceRatio_FifthO_FO_SetA[i];
									NumOfFifthO_FO_SetA++;
								}
								if(m == 2){
									ResonanceRatio_FifthO_2O_SetA[i] = period_FifthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_2O_SetA[i]<<endl;
									
									OverallResonance_FifthO_2O_SetA = OverallResonance_FifthO_2O_SetA + ResonanceRatio_FifthO_2O_SetA[i];
									NumOfFifthO_2O_SetA++;
								}
								if(m == 3){
									ResonanceRatio_FifthO_ThirdO_SetA[i] = period_FifthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_ThirdO_SetA[i]<<endl;
									
									OverallResonance_FifthO_ThirdO_SetA = OverallResonance_FifthO_ThirdO_SetA + ResonanceRatio_FifthO_ThirdO_SetA[i];
									NumOfFifthO_ThirdO_SetA++;
								}
								if(m == 4){
									ResonanceRatio_FifthO_ForthO_SetA[i] = period_FifthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_ForthO_SetA[i]<<endl;
									
									OverallResonance_FifthO_ForthO_SetA = OverallResonance_FifthO_ForthO_SetA + ResonanceRatio_FifthO_ForthO_SetA[i];
									NumOfFifthO_ForthO_SetA++;
								}
								if(m == 5){
									ResonanceRatio_FifthO_FifthO_SetA[i] = period_FifthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_FifthO_SetA[i]<<endl;
									
									OverallResonance_FifthO_FifthO_SetA = OverallResonance_FifthO_FifthO_SetA + ResonanceRatio_FifthO_FifthO_SetA[i];
									NumOfFifthO_FifthO_SetA++;
								}
								if(m == 6){
									ResonanceRatio_FifthO_SixthO_SetA[i] = period_FifthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_SixthO_SetA[i]<<endl;
									
									OverallResonance_FifthO_SixthO_SetA = OverallResonance_FifthO_SixthO_SetA + ResonanceRatio_FifthO_SixthO_SetA[i];
									NumOfFifthO_SixthO_SetA++;
								}
								if(m == 7){
									ResonanceRatio_FifthO_SeventhO_SetA[i] = period_FifthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_SeventhO_SetA[i]<<endl;
									
									OverallResonance_FifthO_SeventhO_SetA = OverallResonance_FifthO_SeventhO_SetA + ResonanceRatio_FifthO_SeventhO_SetA[i];
									NumOfFifthO_SeventhO_SetA++;
								}
								if(m == 8){
									ResonanceRatio_FifthO_EighthO_SetA[i] = period_FifthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_EighthO_SetA[i]<<endl;
									
									OverallResonance_FifthO_EighthO_SetA = OverallResonance_FifthO_EighthO_SetA + ResonanceRatio_FifthO_EighthO_SetA[i];
									NumOfFifthO_EighthO_SetA++;
								}
								if(m == 9){
									ResonanceRatio_FifthO_NinthO_SetA[i] = period_FifthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_NinthO_SetA[i]<<endl;
									
									OverallResonance_FifthO_NinthO_SetA = OverallResonance_FifthO_NinthO_SetA + ResonanceRatio_FifthO_NinthO_SetA[i];
									NumOfFifthO_NinthO_SetA++;
								}
							}
							//SetB
							if(j == 1){
								IsSelected_FifthO_SetB[i] = true;
								if(m == 0){
									ResonanceRatio_FifthO_FU_SetB[i] = period_FifthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_FU_SetB[i]<<endl;
									
									OverallResonance_FifthO_FU_SetB = OverallResonance_FifthO_FU_SetB + ResonanceRatio_FifthO_FU_SetB[i];
									NumOfFifthO_FU_SetB++;
								}
								if(m == 1){
									ResonanceRatio_FifthO_FO_SetB[i] = period_FifthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_FO_SetB[i]<<endl;
									
									OverallResonance_FifthO_FO_SetB = OverallResonance_FifthO_FO_SetB + ResonanceRatio_FifthO_FO_SetB[i];
									NumOfFifthO_FO_SetB++;
								}
								if(m == 2){
									ResonanceRatio_FifthO_2O_SetB[i] = period_FifthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_2O_SetB[i]<<endl;
									
									OverallResonance_FifthO_2O_SetB = OverallResonance_FifthO_2O_SetB + ResonanceRatio_FifthO_2O_SetB[i];
									NumOfFifthO_2O_SetB++;
								}
								if(m == 3){
									ResonanceRatio_FifthO_ThirdO_SetB[i] = period_FifthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_ThirdO_SetB[i]<<endl;
									
									OverallResonance_FifthO_ThirdO_SetB = OverallResonance_FifthO_ThirdO_SetB + ResonanceRatio_FifthO_ThirdO_SetB[i];
									NumOfFifthO_ThirdO_SetB++;
								}
								if(m == 4){
									ResonanceRatio_FifthO_ForthO_SetB[i] = period_FifthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_ForthO_SetB[i]<<endl;
									
									OverallResonance_FifthO_ForthO_SetB = OverallResonance_FifthO_ForthO_SetB + ResonanceRatio_FifthO_ForthO_SetB[i];
									NumOfFifthO_ForthO_SetB++;
								}
								if(m == 5){
									ResonanceRatio_FifthO_FifthO_SetB[i] = period_FifthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_FifthO_SetB[i]<<endl;
									
									OverallResonance_FifthO_FifthO_SetB = OverallResonance_FifthO_FifthO_SetB + ResonanceRatio_FifthO_FifthO_SetB[i];
									NumOfFifthO_FifthO_SetB++;
								}
								if(m == 6){
									ResonanceRatio_FifthO_SixthO_SetB[i] = period_FifthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_SixthO_SetB[i]<<endl;
									
									OverallResonance_FifthO_SixthO_SetB = OverallResonance_FifthO_SixthO_SetB + ResonanceRatio_FifthO_SixthO_SetB[i];
									NumOfFifthO_SixthO_SetB++;
								}
								if(m == 7){
									ResonanceRatio_FifthO_SeventhO_SetB[i] = period_FifthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_SeventhO_SetB[i]<<endl;
									
									OverallResonance_FifthO_SeventhO_SetB = OverallResonance_FifthO_SeventhO_SetB + ResonanceRatio_FifthO_SeventhO_SetB[i];
									NumOfFifthO_SeventhO_SetB++;
								}
								if(m == 8){
									ResonanceRatio_FifthO_EighthO_SetB[i] = period_FifthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_EighthO_SetB[i]<<endl;
									
									OverallResonance_FifthO_EighthO_SetB = OverallResonance_FifthO_EighthO_SetB + ResonanceRatio_FifthO_EighthO_SetB[i];
									NumOfFifthO_EighthO_SetB++;
								}
								if(m == 9){
									ResonanceRatio_FifthO_NinthO_SetB[i] = period_FifthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_NinthO_SetB[i]<<endl;
									
									OverallResonance_FifthO_NinthO_SetB = OverallResonance_FifthO_NinthO_SetB + ResonanceRatio_FifthO_NinthO_SetB[i];
									NumOfFifthO_NinthO_SetB++;
								}
							}
							//SetC
							if(j == 2){
								IsSelected_FifthO_SetC[i] = true;
								if(m == 0){
									ResonanceRatio_FifthO_FU_SetC[i] = period_FifthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_FU_SetC[i]<<endl;
									
									OverallResonance_FifthO_FU_SetC = OverallResonance_FifthO_FU_SetC + ResonanceRatio_FifthO_FU_SetC[i];
									NumOfFifthO_FU_SetC++;
								}
								if(m == 1){
									ResonanceRatio_FifthO_FO_SetC[i] = period_FifthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_FO_SetC[i]<<endl;
									
									OverallResonance_FifthO_FO_SetC = OverallResonance_FifthO_FO_SetC + ResonanceRatio_FifthO_FO_SetC[i];
									NumOfFifthO_FO_SetC++;
								}
								if(m == 2){
									ResonanceRatio_FifthO_2O_SetC[i] = period_FifthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_2O_SetC[i]<<endl;
									
									OverallResonance_FifthO_2O_SetC = OverallResonance_FifthO_2O_SetC + ResonanceRatio_FifthO_2O_SetC[i];
									NumOfFifthO_2O_SetC++;
								}
								if(m == 3){
									ResonanceRatio_FifthO_ThirdO_SetC[i] = period_FifthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_ThirdO_SetC[i]<<endl;
									
									OverallResonance_FifthO_ThirdO_SetC = OverallResonance_FifthO_ThirdO_SetC + ResonanceRatio_FifthO_ThirdO_SetC[i];
									NumOfFifthO_ThirdO_SetC++;
								}
								if(m == 4){
									ResonanceRatio_FifthO_ForthO_SetC[i] = period_FifthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_ForthO_SetC[i]<<endl;
									
									OverallResonance_FifthO_ForthO_SetC = OverallResonance_FifthO_ForthO_SetC + ResonanceRatio_FifthO_ForthO_SetC[i];
									NumOfFifthO_ForthO_SetC++;
								}
								if(m == 5){
									ResonanceRatio_FifthO_FifthO_SetC[i] = period_FifthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_FifthO_SetC[i]<<endl;
									
									OverallResonance_FifthO_FifthO_SetC = OverallResonance_FifthO_FifthO_SetC + ResonanceRatio_FifthO_FifthO_SetC[i];
									NumOfFifthO_FifthO_SetC++;
								}
								if(m == 6){
									ResonanceRatio_FifthO_SixthO_SetC[i] = period_FifthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_SixthO_SetC[i]<<endl;
									
									OverallResonance_FifthO_SixthO_SetC = OverallResonance_FifthO_SixthO_SetC + ResonanceRatio_FifthO_SixthO_SetC[i];
									NumOfFifthO_SixthO_SetC++;
								}
								if(m == 7){
									ResonanceRatio_FifthO_SeventhO_SetC[i] = period_FifthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_SeventhO_SetC[i]<<endl;
									
									OverallResonance_FifthO_SeventhO_SetC = OverallResonance_FifthO_SeventhO_SetC + ResonanceRatio_FifthO_SeventhO_SetC[i];
									NumOfFifthO_SeventhO_SetC++;
								}
								if(m == 8){
									ResonanceRatio_FifthO_EighthO_SetC[i] = period_FifthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_EighthO_SetC[i]<<endl;
									
									OverallResonance_FifthO_EighthO_SetC = OverallResonance_FifthO_EighthO_SetC + ResonanceRatio_FifthO_EighthO_SetC[i];
									NumOfFifthO_EighthO_SetC++;
								}
								if(m == 9){
									ResonanceRatio_FifthO_NinthO_SetC[i] = period_FifthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_NinthO_SetC[i]<<endl;
									
									OverallResonance_FifthO_NinthO_SetC = OverallResonance_FifthO_NinthO_SetC + ResonanceRatio_FifthO_NinthO_SetC[i];
									NumOfFifthO_NinthO_SetC++;
								}
							}
							//SetD
							if(j == 3){
								IsSelected_FifthO_SetD[i] = true;
								if(m == 0){
									ResonanceRatio_FifthO_FU_SetD[i] = period_FifthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_FU_SetD[i]<<endl;
									
									OverallResonance_FifthO_FU_SetD = OverallResonance_FifthO_FU_SetD + ResonanceRatio_FifthO_FU_SetD[i];
									NumOfFifthO_FU_SetD++;
								}
								if(m == 1){
									ResonanceRatio_FifthO_FO_SetD[i] = period_FifthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_FO_SetD[i]<<endl;
									
									OverallResonance_FifthO_FO_SetD = OverallResonance_FifthO_FO_SetD + ResonanceRatio_FifthO_FO_SetD[i];
									NumOfFifthO_FO_SetD++;
								}
								if(m == 2){
									ResonanceRatio_FifthO_2O_SetD[i] = period_FifthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_2O_SetD[i]<<endl;
									
									OverallResonance_FifthO_2O_SetD = OverallResonance_FifthO_2O_SetD + ResonanceRatio_FifthO_2O_SetD[i];
									NumOfFifthO_2O_SetD++;
								}
								if(m == 3){
									ResonanceRatio_FifthO_ThirdO_SetD[i] = period_FifthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_ThirdO_SetD[i]<<endl;
									
									OverallResonance_FifthO_ThirdO_SetD = OverallResonance_FifthO_ThirdO_SetD + ResonanceRatio_FifthO_ThirdO_SetD[i];
									NumOfFifthO_ThirdO_SetD++;
								}
								if(m == 4){
									ResonanceRatio_FifthO_ForthO_SetD[i] = period_FifthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_ForthO_SetD[i]<<endl;
									
									OverallResonance_FifthO_ForthO_SetD = OverallResonance_FifthO_ForthO_SetD + ResonanceRatio_FifthO_ForthO_SetD[i];
									NumOfFifthO_ForthO_SetD++;
								}
								if(m == 5){
									ResonanceRatio_FifthO_FifthO_SetD[i] = period_FifthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_FifthO_SetD[i]<<endl;
									
									OverallResonance_FifthO_FifthO_SetD = OverallResonance_FifthO_FifthO_SetD + ResonanceRatio_FifthO_FifthO_SetD[i];
									NumOfFifthO_FifthO_SetD++;
								}
								if(m == 6){
									ResonanceRatio_FifthO_SixthO_SetD[i] = period_FifthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_SixthO_SetD[i]<<endl;
									
									OverallResonance_FifthO_SixthO_SetD = OverallResonance_FifthO_SixthO_SetD + ResonanceRatio_FifthO_SixthO_SetD[i];
									NumOfFifthO_SixthO_SetD++;
								}
								if(m == 7){
									ResonanceRatio_FifthO_SeventhO_SetD[i] = period_FifthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_SeventhO_SetD[i]<<endl;
									
									OverallResonance_FifthO_SeventhO_SetD = OverallResonance_FifthO_SeventhO_SetD + ResonanceRatio_FifthO_SeventhO_SetD[i];
									NumOfFifthO_SeventhO_SetD++;
								}
								if(m == 8){
									ResonanceRatio_FifthO_EighthO_SetD[i] = period_FifthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_EighthO_SetD[i]<<endl;
									
									OverallResonance_FifthO_EighthO_SetD = OverallResonance_FifthO_EighthO_SetD + ResonanceRatio_FifthO_EighthO_SetD[i];
									NumOfFifthO_EighthO_SetD++;
								}
								if(m == 9){
									ResonanceRatio_FifthO_NinthO_SetD[i] = period_FifthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_FifthO_NinthO_SetD[i]<<endl;
									
									OverallResonance_FifthO_NinthO_SetD = OverallResonance_FifthO_NinthO_SetD + ResonanceRatio_FifthO_NinthO_SetD[i];
									NumOfFifthO_NinthO_SetD++;
								}
							}
						}
					}
					if(!DoBoundsCut || ((ratio_temp_SixthO <= (1. + bound)) && (ratio_temp_SixthO >= (1. - bound))) || ((ratio_temp_SixthO <= (0.5 + bound)) && (ratio_temp_SixthO >= (0.5 - bound))) || ((ratio_temp_SixthO <= (1.5 + bound)) && (ratio_temp_SixthO >= (1.5 - bound))) || ((ratio_temp_SixthO <= (2.0 + bound)) && (ratio_temp_SixthO >= (2.0 - bound))) || ((ratio_temp_SixthO <= (2.5 + bound)) && (ratio_temp_SixthO >= (2.5 - bound))) || ((ratio_temp_SixthO <= (3.0 + bound)) && (ratio_temp_SixthO >= (3.0 - bound))) || ((ratio_temp_SixthO <= (3.5 + bound)) && (ratio_temp_SixthO >= (3.5 - bound))) || ((ratio_temp_SixthO <= (4.0 + bound)) && (ratio_temp_SixthO >= (4.0 - bound))) || ((ratio_temp_SixthO <= (4.5 + bound)) && (ratio_temp_SixthO >= (4.5 - bound))) || ((ratio_temp_SixthO <= (5.0 + bound)) && (ratio_temp_SixthO >= (5.0 - bound))) || ((ratio_temp_SixthO <= (5.5 + bound)) && (ratio_temp_SixthO >= (5.5 - bound))) || ((ratio_temp_SixthO <= (6.0 + bound)) && (ratio_temp_SixthO >= (6.0 - bound)))){
						//SixthO mode//
						if((growth > 0.0 || !DoPosGrowthCut) && (growth_SixthO > 0)){
							cout<<"Resonance Ratios With SixthO..."<<endl;
							//SetA
							if(j == 0){
								IsSelected_SixthO_SetA[i] = true;
								if(m == 0){
									ResonanceRatio_SixthO_FU_SetA[i] = period_SixthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_FU_SetA[i]<<endl;
									
									OverallResonance_SixthO_FU_SetA = OverallResonance_SixthO_FU_SetA + ResonanceRatio_SixthO_FU_SetA[i];
									NumOfSixthO_FU_SetA++;
								}
								if(m == 1){
									ResonanceRatio_SixthO_FO_SetA[i] = period_SixthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_FO_SetA[i]<<endl;
									
									OverallResonance_SixthO_FO_SetA = OverallResonance_SixthO_FO_SetA + ResonanceRatio_SixthO_FO_SetA[i];
									NumOfSixthO_FO_SetA++;
								}
								if(m == 2){
									ResonanceRatio_SixthO_2O_SetA[i] = period_SixthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_2O_SetA[i]<<endl;
									
									OverallResonance_SixthO_2O_SetA = OverallResonance_SixthO_2O_SetA + ResonanceRatio_SixthO_2O_SetA[i];
									NumOfSixthO_2O_SetA++;
								}
								if(m == 3){
									ResonanceRatio_SixthO_ThirdO_SetA[i] = period_SixthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_ThirdO_SetA[i]<<endl;
									
									OverallResonance_SixthO_ThirdO_SetA = OverallResonance_SixthO_ThirdO_SetA + ResonanceRatio_SixthO_ThirdO_SetA[i];
									NumOfSixthO_ThirdO_SetA++;
								}
								if(m == 4){
									ResonanceRatio_SixthO_ForthO_SetA[i] = period_SixthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_ForthO_SetA[i]<<endl;
									
									OverallResonance_SixthO_ForthO_SetA = OverallResonance_SixthO_ForthO_SetA + ResonanceRatio_SixthO_ForthO_SetA[i];
									NumOfSixthO_ForthO_SetA++;
								}
								if(m == 5){
									ResonanceRatio_SixthO_FifthO_SetA[i] = period_SixthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_FifthO_SetA[i]<<endl;
									
									OverallResonance_SixthO_FifthO_SetA = OverallResonance_SixthO_FifthO_SetA + ResonanceRatio_SixthO_FifthO_SetA[i];
									NumOfSixthO_FifthO_SetA++;
								}
								if(m == 6){
									ResonanceRatio_SixthO_SixthO_SetA[i] = period_SixthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_SixthO_SetA[i]<<endl;
									
									OverallResonance_SixthO_SixthO_SetA = OverallResonance_SixthO_SixthO_SetA + ResonanceRatio_SixthO_SixthO_SetA[i];
									NumOfSixthO_SixthO_SetA++;
								}
								if(m == 7){
									ResonanceRatio_SixthO_SeventhO_SetA[i] = period_SixthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_SeventhO_SetA[i]<<endl;
									
									OverallResonance_SixthO_SeventhO_SetA = OverallResonance_SixthO_SeventhO_SetA + ResonanceRatio_SixthO_SeventhO_SetA[i];
									NumOfSixthO_SeventhO_SetA++;
								}
								if(m == 8){
									ResonanceRatio_SixthO_EighthO_SetA[i] = period_SixthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_EighthO_SetA[i]<<endl;
									
									OverallResonance_SixthO_EighthO_SetA = OverallResonance_SixthO_EighthO_SetA + ResonanceRatio_SixthO_EighthO_SetA[i];
									NumOfSixthO_EighthO_SetA++;
								}
								if(m == 9){
									ResonanceRatio_SixthO_NinthO_SetA[i] = period_SixthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_NinthO_SetA[i]<<endl;
									
									OverallResonance_SixthO_NinthO_SetA = OverallResonance_SixthO_NinthO_SetA + ResonanceRatio_SixthO_NinthO_SetA[i];
									NumOfSixthO_NinthO_SetA++;
								}
							}
							//SetB
							if(j == 1){
								IsSelected_SixthO_SetB[i] = true;
								if(m == 0){
									ResonanceRatio_SixthO_FU_SetB[i] = period_SixthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_FU_SetB[i]<<endl;
									
									OverallResonance_SixthO_FU_SetB = OverallResonance_SixthO_FU_SetB + ResonanceRatio_SixthO_FU_SetB[i];
									NumOfSixthO_FU_SetB++;
								}
								if(m == 1){
									ResonanceRatio_SixthO_FO_SetB[i] = period_SixthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_FO_SetB[i]<<endl;
									
									OverallResonance_SixthO_FO_SetB = OverallResonance_SixthO_FO_SetB + ResonanceRatio_SixthO_FO_SetB[i];
									NumOfSixthO_FO_SetB++;
								}
								if(m == 2){
									ResonanceRatio_SixthO_2O_SetB[i] = period_SixthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_2O_SetB[i]<<endl;
									
									OverallResonance_SixthO_2O_SetB = OverallResonance_SixthO_2O_SetB + ResonanceRatio_SixthO_2O_SetB[i];
									NumOfSixthO_2O_SetB++;
								}
								if(m == 3){
									ResonanceRatio_SixthO_ThirdO_SetB[i] = period_SixthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_ThirdO_SetB[i]<<endl;
									
									OverallResonance_SixthO_ThirdO_SetB = OverallResonance_SixthO_ThirdO_SetB + ResonanceRatio_SixthO_ThirdO_SetB[i];
									NumOfSixthO_ThirdO_SetB++;
								}
								if(m == 4){
									ResonanceRatio_SixthO_ForthO_SetB[i] = period_SixthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_ForthO_SetB[i]<<endl;
									
									OverallResonance_SixthO_ForthO_SetB = OverallResonance_SixthO_ForthO_SetB + ResonanceRatio_SixthO_ForthO_SetB[i];
									NumOfSixthO_ForthO_SetB++;
								}
								if(m == 5){
									ResonanceRatio_SixthO_FifthO_SetB[i] = period_SixthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_FifthO_SetB[i]<<endl;
									
									OverallResonance_SixthO_FifthO_SetB = OverallResonance_SixthO_FifthO_SetB + ResonanceRatio_SixthO_FifthO_SetB[i];
									NumOfSixthO_FifthO_SetB++;
								}
								if(m == 6){
									ResonanceRatio_SixthO_SixthO_SetB[i] = period_SixthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_SixthO_SetB[i]<<endl;
									
									OverallResonance_SixthO_SixthO_SetB = OverallResonance_SixthO_SixthO_SetB + ResonanceRatio_SixthO_SixthO_SetB[i];
									NumOfSixthO_SixthO_SetB++;
								}
								if(m == 7){
									ResonanceRatio_SixthO_SeventhO_SetB[i] = period_SixthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_SeventhO_SetB[i]<<endl;
									
									OverallResonance_SixthO_SeventhO_SetB = OverallResonance_SixthO_SeventhO_SetB + ResonanceRatio_SixthO_SeventhO_SetB[i];
									NumOfSixthO_SeventhO_SetB++;
								}
								if(m == 8){
									ResonanceRatio_SixthO_EighthO_SetB[i] = period_SixthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_EighthO_SetB[i]<<endl;
									
									OverallResonance_SixthO_EighthO_SetB = OverallResonance_SixthO_EighthO_SetB + ResonanceRatio_SixthO_EighthO_SetB[i];
									NumOfSixthO_EighthO_SetB++;
								}
								if(m == 9){
									ResonanceRatio_SixthO_NinthO_SetB[i] = period_SixthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_NinthO_SetB[i]<<endl;
									
									OverallResonance_SixthO_NinthO_SetB = OverallResonance_SixthO_NinthO_SetB + ResonanceRatio_SixthO_NinthO_SetB[i];
									NumOfSixthO_NinthO_SetB++;
								}
							}
							//SetC
							if(j == 2){
								IsSelected_SixthO_SetC[i] = true;
								if(m == 0){
									ResonanceRatio_SixthO_FU_SetC[i] = period_SixthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_FU_SetC[i]<<endl;
									
									OverallResonance_SixthO_FU_SetC = OverallResonance_SixthO_FU_SetC + ResonanceRatio_SixthO_FU_SetC[i];
									NumOfSixthO_FU_SetC++;
								}
								if(m == 1){
									ResonanceRatio_SixthO_FO_SetC[i] = period_SixthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_FO_SetC[i]<<endl;
									
									OverallResonance_SixthO_FO_SetC = OverallResonance_SixthO_FO_SetC + ResonanceRatio_SixthO_FO_SetC[i];
									NumOfSixthO_FO_SetC++;
								}
								if(m == 2){
									ResonanceRatio_SixthO_2O_SetC[i] = period_SixthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_2O_SetC[i]<<endl;
									
									OverallResonance_SixthO_2O_SetC = OverallResonance_SixthO_2O_SetC + ResonanceRatio_SixthO_2O_SetC[i];
									NumOfSixthO_2O_SetC++;
								}
								if(m == 3){
									ResonanceRatio_SixthO_ThirdO_SetC[i] = period_SixthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_ThirdO_SetC[i]<<endl;
									
									OverallResonance_SixthO_ThirdO_SetC = OverallResonance_SixthO_ThirdO_SetC + ResonanceRatio_SixthO_ThirdO_SetC[i];
									NumOfSixthO_ThirdO_SetC++;
								}
								if(m == 4){
									ResonanceRatio_SixthO_ForthO_SetC[i] = period_SixthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_ForthO_SetC[i]<<endl;
									
									OverallResonance_SixthO_ForthO_SetC = OverallResonance_SixthO_ForthO_SetC + ResonanceRatio_SixthO_ForthO_SetC[i];
									NumOfSixthO_ForthO_SetC++;
								}
								if(m == 5){
									ResonanceRatio_SixthO_FifthO_SetC[i] = period_SixthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_FifthO_SetC[i]<<endl;
									
									OverallResonance_SixthO_FifthO_SetC = OverallResonance_SixthO_FifthO_SetC + ResonanceRatio_SixthO_FifthO_SetC[i];
									NumOfSixthO_FifthO_SetC++;
								}
								if(m == 6){
									ResonanceRatio_SixthO_SixthO_SetC[i] = period_SixthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_SixthO_SetC[i]<<endl;
									
									OverallResonance_SixthO_SixthO_SetC = OverallResonance_SixthO_SixthO_SetC + ResonanceRatio_SixthO_SixthO_SetC[i];
									NumOfSixthO_SixthO_SetC++;
								}
								if(m == 7){
									ResonanceRatio_SixthO_SeventhO_SetC[i] = period_SixthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_SeventhO_SetC[i]<<endl;
									
									OverallResonance_SixthO_SeventhO_SetC = OverallResonance_SixthO_SeventhO_SetC + ResonanceRatio_SixthO_SeventhO_SetC[i];
									NumOfSixthO_SeventhO_SetC++;
								}
								if(m == 8){
									ResonanceRatio_SixthO_EighthO_SetC[i] = period_SixthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_EighthO_SetC[i]<<endl;
									
									OverallResonance_SixthO_EighthO_SetC = OverallResonance_SixthO_EighthO_SetC + ResonanceRatio_SixthO_EighthO_SetC[i];
									NumOfSixthO_EighthO_SetC++;
								}
								if(m == 9){
									ResonanceRatio_SixthO_NinthO_SetC[i] = period_SixthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_NinthO_SetC[i]<<endl;
									
									OverallResonance_SixthO_NinthO_SetC = OverallResonance_SixthO_NinthO_SetC + ResonanceRatio_SixthO_NinthO_SetC[i];
									NumOfSixthO_NinthO_SetC++;
								}
							}
							//SetD
							if(j == 3){
								IsSelected_SixthO_SetD[i] = true;
								if(m == 0){
									ResonanceRatio_SixthO_FU_SetD[i] = period_SixthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_FU_SetD[i]<<endl;
									
									OverallResonance_SixthO_FU_SetD = OverallResonance_SixthO_FU_SetD + ResonanceRatio_SixthO_FU_SetD[i];
									NumOfSixthO_FU_SetD++;
								}
								if(m == 1){
									ResonanceRatio_SixthO_FO_SetD[i] = period_SixthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_FO_SetD[i]<<endl;
									
									OverallResonance_SixthO_FO_SetD = OverallResonance_SixthO_FO_SetD + ResonanceRatio_SixthO_FO_SetD[i];
									NumOfSixthO_FO_SetD++;
								}
								if(m == 2){
									ResonanceRatio_SixthO_2O_SetD[i] = period_SixthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_2O_SetD[i]<<endl;
									
									OverallResonance_SixthO_2O_SetD = OverallResonance_SixthO_2O_SetD + ResonanceRatio_SixthO_2O_SetD[i];
									NumOfSixthO_2O_SetD++;
								}
								if(m == 3){
									ResonanceRatio_SixthO_ThirdO_SetD[i] = period_SixthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_ThirdO_SetD[i]<<endl;
									
									OverallResonance_SixthO_ThirdO_SetD = OverallResonance_SixthO_ThirdO_SetD + ResonanceRatio_SixthO_ThirdO_SetD[i];
									NumOfSixthO_ThirdO_SetD++;
								}
								if(m == 4){
									ResonanceRatio_SixthO_ForthO_SetD[i] = period_SixthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_ForthO_SetD[i]<<endl;
									
									OverallResonance_SixthO_ForthO_SetD = OverallResonance_SixthO_ForthO_SetD + ResonanceRatio_SixthO_ForthO_SetD[i];
									NumOfSixthO_ForthO_SetD++;
								}
								if(m == 5){
									ResonanceRatio_SixthO_FifthO_SetD[i] = period_SixthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_FifthO_SetD[i]<<endl;
									
									OverallResonance_SixthO_FifthO_SetD = OverallResonance_SixthO_FifthO_SetD + ResonanceRatio_SixthO_FifthO_SetD[i];
									NumOfSixthO_FifthO_SetD++;
								}
								if(m == 6){
									ResonanceRatio_SixthO_SixthO_SetD[i] = period_SixthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_SixthO_SetD[i]<<endl;
									
									OverallResonance_SixthO_SixthO_SetD = OverallResonance_SixthO_SixthO_SetD + ResonanceRatio_SixthO_SixthO_SetD[i];
									NumOfSixthO_SixthO_SetD++;
								}
								if(m == 7){
									ResonanceRatio_SixthO_SeventhO_SetD[i] = period_SixthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_SeventhO_SetD[i]<<endl;
									
									OverallResonance_SixthO_SeventhO_SetD = OverallResonance_SixthO_SeventhO_SetD + ResonanceRatio_SixthO_SeventhO_SetD[i];
									NumOfSixthO_SeventhO_SetD++;
								}
								if(m == 8){
									ResonanceRatio_SixthO_EighthO_SetD[i] = period_SixthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_EighthO_SetD[i]<<endl;
									
									OverallResonance_SixthO_EighthO_SetD = OverallResonance_SixthO_EighthO_SetD + ResonanceRatio_SixthO_EighthO_SetD[i];
									NumOfSixthO_EighthO_SetD++;
								}
								if(m == 9){
									ResonanceRatio_SixthO_NinthO_SetD[i] = period_SixthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SixthO_NinthO_SetD[i]<<endl;
									
									OverallResonance_SixthO_NinthO_SetD = OverallResonance_SixthO_NinthO_SetD + ResonanceRatio_SixthO_NinthO_SetD[i];
									NumOfSixthO_NinthO_SetD++;
								}
							}
						}
					}
					if(!DoBoundsCut || ((ratio_temp_SeventhO <= (1. + bound)) && (ratio_temp_SeventhO >= (1. - bound))) || ((ratio_temp_SeventhO <= (0.5 + bound)) && (ratio_temp_SeventhO >= (0.5 - bound))) || ((ratio_temp_SeventhO <= (1.5 + bound)) && (ratio_temp_SeventhO >= (1.5 - bound))) || ((ratio_temp_SeventhO <= (2.0 + bound)) && (ratio_temp_SeventhO >= (2.0 - bound))) || ((ratio_temp_SeventhO <= (2.5 + bound)) && (ratio_temp_SeventhO >= (2.5 - bound))) || ((ratio_temp_SeventhO <= (3.0 + bound)) && (ratio_temp_SeventhO >= (3.0 - bound))) || ((ratio_temp_SeventhO <= (3.5 + bound)) && (ratio_temp_SeventhO >= (3.5 - bound))) || ((ratio_temp_SeventhO <= (4.0 + bound)) && (ratio_temp_SeventhO >= (4.0 - bound))) || ((ratio_temp_SeventhO <= (4.5 + bound)) && (ratio_temp_SeventhO >= (4.5 - bound))) || ((ratio_temp_SeventhO <= (5.0 + bound)) && (ratio_temp_SeventhO >= (5.0 - bound))) || ((ratio_temp_SeventhO <= (5.5 + bound)) && (ratio_temp_SeventhO >= (5.5 - bound))) || ((ratio_temp_SeventhO <= (6.0 + bound)) && (ratio_temp_SeventhO >= (6.0 - bound)))){
						//SeventhO mode//
						if((growth > 0.0 || !DoPosGrowthCut) && (growth_SeventhO > 0)){
							cout<<"Resonance Ratios With SeventhO..."<<endl;
							//SetA
							if(j == 0){
								IsSelected_SeventhO_SetA[i] = true;
								if(m == 0){
									ResonanceRatio_SeventhO_FU_SetA[i] = period_SeventhO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_FU_SetA[i]<<endl;
									
									OverallResonance_SeventhO_FU_SetA = OverallResonance_SeventhO_FU_SetA + ResonanceRatio_SeventhO_FU_SetA[i];
									NumOfSeventhO_FU_SetA++;
								}
								if(m == 1){
									ResonanceRatio_SeventhO_FO_SetA[i] = period_SeventhO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_FO_SetA[i]<<endl;
									
									OverallResonance_SeventhO_FO_SetA = OverallResonance_SeventhO_FO_SetA + ResonanceRatio_SeventhO_FO_SetA[i];
									NumOfSeventhO_FO_SetA++;
								}
								if(m == 2){
									ResonanceRatio_SeventhO_2O_SetA[i] = period_SeventhO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_2O_SetA[i]<<endl;
									
									OverallResonance_SeventhO_2O_SetA = OverallResonance_SeventhO_2O_SetA + ResonanceRatio_SeventhO_2O_SetA[i];
									NumOfSeventhO_2O_SetA++;
								}
								if(m == 3){
									ResonanceRatio_SeventhO_ThirdO_SetA[i] = period_SeventhO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_ThirdO_SetA[i]<<endl;
									
									OverallResonance_SeventhO_ThirdO_SetA = OverallResonance_SeventhO_ThirdO_SetA + ResonanceRatio_SeventhO_ThirdO_SetA[i];
									NumOfSeventhO_ThirdO_SetA++;
								}
								if(m == 4){
									ResonanceRatio_SeventhO_ForthO_SetA[i] = period_SeventhO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_ForthO_SetA[i]<<endl;
									
									OverallResonance_SeventhO_ForthO_SetA = OverallResonance_SeventhO_ForthO_SetA + ResonanceRatio_SeventhO_ForthO_SetA[i];
									NumOfSeventhO_ForthO_SetA++;
								}
								if(m == 5){
									ResonanceRatio_SeventhO_FifthO_SetA[i] = period_SeventhO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_FifthO_SetA[i]<<endl;
									
									OverallResonance_SeventhO_FifthO_SetA = OverallResonance_SeventhO_FifthO_SetA + ResonanceRatio_SeventhO_FifthO_SetA[i];
									NumOfSeventhO_FifthO_SetA++;
								}
								if(m == 6){
									ResonanceRatio_SeventhO_SixthO_SetA[i] = period_SeventhO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_SixthO_SetA[i]<<endl;
									
									OverallResonance_SeventhO_SixthO_SetA = OverallResonance_SeventhO_SixthO_SetA + ResonanceRatio_SeventhO_SixthO_SetA[i];
									NumOfSeventhO_SixthO_SetA++;
								}
								if(m == 7){
									ResonanceRatio_SeventhO_SeventhO_SetA[i] = period_SeventhO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_SeventhO_SetA[i]<<endl;
									
									OverallResonance_SeventhO_SeventhO_SetA = OverallResonance_SeventhO_SeventhO_SetA + ResonanceRatio_SeventhO_SeventhO_SetA[i];
									NumOfSeventhO_SeventhO_SetA++;
								}
								if(m == 8){
									ResonanceRatio_SeventhO_EighthO_SetA[i] = period_SeventhO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_EighthO_SetA[i]<<endl;
									
									OverallResonance_SeventhO_EighthO_SetA = OverallResonance_SeventhO_EighthO_SetA + ResonanceRatio_SeventhO_EighthO_SetA[i];
									NumOfSeventhO_EighthO_SetA++;
								}
								if(m == 9){
									ResonanceRatio_SeventhO_NinthO_SetA[i] = period_SeventhO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_NinthO_SetA[i]<<endl;
									
									OverallResonance_SeventhO_NinthO_SetA = OverallResonance_SeventhO_NinthO_SetA + ResonanceRatio_SeventhO_NinthO_SetA[i];
									NumOfSeventhO_NinthO_SetA++;
								}
							}
							//SetB
							if(j == 1){
								IsSelected_SeventhO_SetB[i] = true;
								if(m == 0){
									ResonanceRatio_SeventhO_FU_SetB[i] = period_SeventhO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_FU_SetB[i]<<endl;
									
									OverallResonance_SeventhO_FU_SetB = OverallResonance_SeventhO_FU_SetB + ResonanceRatio_SeventhO_FU_SetB[i];
									NumOfSeventhO_FU_SetB++;
								}
								if(m == 1){
									ResonanceRatio_SeventhO_FO_SetB[i] = period_SeventhO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_FO_SetB[i]<<endl;
									
									OverallResonance_SeventhO_FO_SetB = OverallResonance_SeventhO_FO_SetB + ResonanceRatio_SeventhO_FO_SetB[i];
									NumOfSeventhO_FO_SetB++;
								}
								if(m == 2){
									ResonanceRatio_SeventhO_2O_SetB[i] = period_SeventhO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_2O_SetB[i]<<endl;
									
									OverallResonance_SeventhO_2O_SetB = OverallResonance_SeventhO_2O_SetB + ResonanceRatio_SeventhO_2O_SetB[i];
									NumOfSeventhO_2O_SetB++;
								}
								if(m == 3){
									ResonanceRatio_SeventhO_ThirdO_SetB[i] = period_SeventhO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_ThirdO_SetB[i]<<endl;
									
									OverallResonance_SeventhO_ThirdO_SetB = OverallResonance_SeventhO_ThirdO_SetB + ResonanceRatio_SeventhO_ThirdO_SetB[i];
									NumOfSeventhO_ThirdO_SetB++;
								}
								if(m == 4){
									ResonanceRatio_SeventhO_ForthO_SetB[i] = period_SeventhO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_ForthO_SetB[i]<<endl;
									
									OverallResonance_SeventhO_ForthO_SetB = OverallResonance_SeventhO_ForthO_SetB + ResonanceRatio_SeventhO_ForthO_SetB[i];
									NumOfSeventhO_ForthO_SetB++;
								}
								if(m == 5){
									ResonanceRatio_SeventhO_FifthO_SetB[i] = period_SeventhO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_FifthO_SetB[i]<<endl;
									
									OverallResonance_SeventhO_FifthO_SetB = OverallResonance_SeventhO_FifthO_SetB + ResonanceRatio_SeventhO_FifthO_SetB[i];
									NumOfSeventhO_FifthO_SetB++;
								}
								if(m == 6){
									ResonanceRatio_SeventhO_SixthO_SetB[i] = period_SeventhO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_SixthO_SetB[i]<<endl;
									
									OverallResonance_SeventhO_SixthO_SetB = OverallResonance_SeventhO_SixthO_SetB + ResonanceRatio_SeventhO_SixthO_SetB[i];
									NumOfSeventhO_SixthO_SetB++;
								}
								if(m == 7){
									ResonanceRatio_SeventhO_SeventhO_SetB[i] = period_SeventhO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_SeventhO_SetB[i]<<endl;
									
									OverallResonance_SeventhO_SeventhO_SetB = OverallResonance_SeventhO_SeventhO_SetB + ResonanceRatio_SeventhO_SeventhO_SetB[i];
									NumOfSeventhO_SeventhO_SetB++;
								}
								if(m == 8){
									ResonanceRatio_SeventhO_EighthO_SetB[i] = period_SeventhO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_EighthO_SetB[i]<<endl;
									
									OverallResonance_SeventhO_EighthO_SetB = OverallResonance_SeventhO_EighthO_SetB + ResonanceRatio_SeventhO_EighthO_SetB[i];
									NumOfSeventhO_EighthO_SetB++;
								}
								if(m == 9){
									ResonanceRatio_SeventhO_NinthO_SetB[i] = period_SeventhO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_NinthO_SetB[i]<<endl;
									
									OverallResonance_SeventhO_NinthO_SetB = OverallResonance_SeventhO_NinthO_SetB + ResonanceRatio_SeventhO_NinthO_SetB[i];
									NumOfSeventhO_NinthO_SetB++;
								}
							}
							//SetC
							if(j == 2){
								IsSelected_SeventhO_SetC[i] = true;
								if(m == 0){
									ResonanceRatio_SeventhO_FU_SetC[i] = period_SeventhO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_FU_SetC[i]<<endl;
									
									OverallResonance_SeventhO_FU_SetC = OverallResonance_SeventhO_FU_SetC + ResonanceRatio_SeventhO_FU_SetC[i];
									NumOfSeventhO_FU_SetC++;
								}
								if(m == 1){
									ResonanceRatio_SeventhO_FO_SetC[i] = period_SeventhO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_FO_SetC[i]<<endl;
									
									OverallResonance_SeventhO_FO_SetC = OverallResonance_SeventhO_FO_SetC + ResonanceRatio_SeventhO_FO_SetC[i];
									NumOfSeventhO_FO_SetC++;
								}
								if(m == 2){
									ResonanceRatio_SeventhO_2O_SetC[i] = period_SeventhO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_2O_SetC[i]<<endl;
									
									OverallResonance_SeventhO_2O_SetC = OverallResonance_SeventhO_2O_SetC + ResonanceRatio_SeventhO_2O_SetC[i];
									NumOfSeventhO_2O_SetC++;
								}
								if(m == 3){
									ResonanceRatio_SeventhO_ThirdO_SetC[i] = period_SeventhO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_ThirdO_SetC[i]<<endl;
									
									OverallResonance_SeventhO_ThirdO_SetC = OverallResonance_SeventhO_ThirdO_SetC + ResonanceRatio_SeventhO_ThirdO_SetC[i];
									NumOfSeventhO_ThirdO_SetC++;
								}
								if(m == 4){
									ResonanceRatio_SeventhO_ForthO_SetC[i] = period_SeventhO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_ForthO_SetC[i]<<endl;
									
									OverallResonance_SeventhO_ForthO_SetC = OverallResonance_SeventhO_ForthO_SetC + ResonanceRatio_SeventhO_ForthO_SetC[i];
									NumOfSeventhO_ForthO_SetC++;
								}
								if(m == 5){
									ResonanceRatio_SeventhO_FifthO_SetC[i] = period_SeventhO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_FifthO_SetC[i]<<endl;
									
									OverallResonance_SeventhO_FifthO_SetC = OverallResonance_SeventhO_FifthO_SetC + ResonanceRatio_SeventhO_FifthO_SetC[i];
									NumOfSeventhO_FifthO_SetC++;
								}
								if(m == 6){
									ResonanceRatio_SeventhO_SixthO_SetC[i] = period_SeventhO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_SixthO_SetC[i]<<endl;
									
									OverallResonance_SeventhO_SixthO_SetC = OverallResonance_SeventhO_SixthO_SetC + ResonanceRatio_SeventhO_SixthO_SetC[i];
									NumOfSeventhO_SixthO_SetC++;
								}
								if(m == 7){
									ResonanceRatio_SeventhO_SeventhO_SetC[i] = period_SeventhO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_SeventhO_SetC[i]<<endl;
									
									OverallResonance_SeventhO_SeventhO_SetC = OverallResonance_SeventhO_SeventhO_SetC + ResonanceRatio_SeventhO_SeventhO_SetC[i];
									NumOfSeventhO_SeventhO_SetC++;
								}
								if(m == 8){
									ResonanceRatio_SeventhO_EighthO_SetC[i] = period_SeventhO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_EighthO_SetC[i]<<endl;
									
									OverallResonance_SeventhO_EighthO_SetC = OverallResonance_SeventhO_EighthO_SetC + ResonanceRatio_SeventhO_EighthO_SetC[i];
									NumOfSeventhO_EighthO_SetC++;
								}
								if(m == 9){
									ResonanceRatio_SeventhO_NinthO_SetC[i] = period_SeventhO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_NinthO_SetC[i]<<endl;
									
									OverallResonance_SeventhO_NinthO_SetC = OverallResonance_SeventhO_NinthO_SetC + ResonanceRatio_SeventhO_NinthO_SetC[i];
									NumOfSeventhO_NinthO_SetC++;
								}
							}
							//SetD
							if(j == 3){
								IsSelected_SeventhO_SetD[i] = true;
								if(m == 0){
									ResonanceRatio_SeventhO_FU_SetD[i] = period_SeventhO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_FU_SetD[i]<<endl;
									
									OverallResonance_SeventhO_FU_SetD = OverallResonance_SeventhO_FU_SetD + ResonanceRatio_SeventhO_FU_SetD[i];
									NumOfSeventhO_FU_SetD++;
								}
								if(m == 1){
									ResonanceRatio_SeventhO_FO_SetD[i] = period_SeventhO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_FO_SetD[i]<<endl;
									
									OverallResonance_SeventhO_FO_SetD = OverallResonance_SeventhO_FO_SetD + ResonanceRatio_SeventhO_FO_SetD[i];
									NumOfSeventhO_FO_SetD++;
								}
								if(m == 2){
									ResonanceRatio_SeventhO_2O_SetD[i] = period_SeventhO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_2O_SetD[i]<<endl;
									
									OverallResonance_SeventhO_2O_SetD = OverallResonance_SeventhO_2O_SetD + ResonanceRatio_SeventhO_2O_SetD[i];
									NumOfSeventhO_2O_SetD++;
								}
								if(m == 3){
									ResonanceRatio_SeventhO_ThirdO_SetD[i] = period_SeventhO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_ThirdO_SetD[i]<<endl;
									
									OverallResonance_SeventhO_ThirdO_SetD = OverallResonance_SeventhO_ThirdO_SetD + ResonanceRatio_SeventhO_ThirdO_SetD[i];
									NumOfSeventhO_ThirdO_SetD++;
								}
								if(m == 4){
									ResonanceRatio_SeventhO_ForthO_SetD[i] = period_SeventhO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_ForthO_SetD[i]<<endl;
									
									OverallResonance_SeventhO_ForthO_SetD = OverallResonance_SeventhO_ForthO_SetD + ResonanceRatio_SeventhO_ForthO_SetD[i];
									NumOfSeventhO_ForthO_SetD++;
								}
								if(m == 5){
									ResonanceRatio_SeventhO_FifthO_SetD[i] = period_SeventhO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_FifthO_SetD[i]<<endl;
									
									OverallResonance_SeventhO_FifthO_SetD = OverallResonance_SeventhO_FifthO_SetD + ResonanceRatio_SeventhO_FifthO_SetD[i];
									NumOfSeventhO_FifthO_SetD++;
								}
								if(m == 6){
									ResonanceRatio_SeventhO_SixthO_SetD[i] = period_SeventhO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_SixthO_SetD[i]<<endl;
									
									OverallResonance_SeventhO_SixthO_SetD = OverallResonance_SeventhO_SixthO_SetD + ResonanceRatio_SeventhO_SixthO_SetD[i];
									NumOfSeventhO_SixthO_SetD++;
								}
								if(m == 7){
									ResonanceRatio_SeventhO_SeventhO_SetD[i] = period_SeventhO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_SeventhO_SetD[i]<<endl;
									
									OverallResonance_SeventhO_SeventhO_SetD = OverallResonance_SeventhO_SeventhO_SetD + ResonanceRatio_SeventhO_SeventhO_SetD[i];
									NumOfSeventhO_SeventhO_SetD++;
								}
								if(m == 8){
									ResonanceRatio_SeventhO_EighthO_SetD[i] = period_SeventhO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_EighthO_SetD[i]<<endl;
									
									OverallResonance_SeventhO_EighthO_SetD = OverallResonance_SeventhO_EighthO_SetD + ResonanceRatio_SeventhO_EighthO_SetD[i];
									NumOfSeventhO_EighthO_SetD++;
								}
								if(m == 9){
									ResonanceRatio_SeventhO_NinthO_SetD[i] = period_SeventhO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_SeventhO_NinthO_SetD[i]<<endl;
									
									OverallResonance_SeventhO_NinthO_SetD = OverallResonance_SeventhO_NinthO_SetD + ResonanceRatio_SeventhO_NinthO_SetD[i];
									NumOfSeventhO_NinthO_SetD++;
								}
							}
						}
					}
					if(!DoBoundsCut || ((ratio_temp_EighthO <= (1. + bound)) && (ratio_temp_EighthO >= (1. - bound))) || ((ratio_temp_EighthO <= (0.5 + bound)) && (ratio_temp_EighthO >= (0.5 - bound))) || ((ratio_temp_EighthO <= (1.5 + bound)) && (ratio_temp_EighthO >= (1.5 - bound))) || ((ratio_temp_EighthO <= (2.0 + bound)) && (ratio_temp_EighthO >= (2.0 - bound))) || ((ratio_temp_EighthO <= (2.5 + bound)) && (ratio_temp_EighthO >= (2.5 - bound))) || ((ratio_temp_EighthO <= (3.0 + bound)) && (ratio_temp_EighthO >= (3.0 - bound))) || ((ratio_temp_EighthO <= (3.5 + bound)) && (ratio_temp_EighthO >= (3.5 - bound))) || ((ratio_temp_EighthO <= (4.0 + bound)) && (ratio_temp_EighthO >= (4.0 - bound))) || ((ratio_temp_EighthO <= (4.5 + bound)) && (ratio_temp_EighthO >= (4.5 - bound))) || ((ratio_temp_EighthO <= (5.0 + bound)) && (ratio_temp_EighthO >= (5.0 - bound))) || ((ratio_temp_EighthO <= (5.5 + bound)) && (ratio_temp_EighthO >= (5.5 - bound))) || ((ratio_temp_EighthO <= (6.0 + bound)) && (ratio_temp_EighthO >= (6.0 - bound)))){
						//EighthO mode//
						if((growth > 0.0 || !DoPosGrowthCut) && (growth_EighthO > 0)){
							cout<<"Resonance Ratios With EighthO..."<<endl;
							//SetA
							if(j == 0){
								IsSelected_EighthO_SetA[i] = true;
								if(m == 0){
									ResonanceRatio_EighthO_FU_SetA[i] = period_EighthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_FU_SetA[i]<<endl;
									
									OverallResonance_EighthO_FU_SetA = OverallResonance_EighthO_FU_SetA + ResonanceRatio_EighthO_FU_SetA[i];
									NumOfEighthO_FU_SetA++;
								}
								if(m == 1){
									ResonanceRatio_EighthO_FO_SetA[i] = period_EighthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_FO_SetA[i]<<endl;
									
									OverallResonance_EighthO_FO_SetA = OverallResonance_EighthO_FO_SetA + ResonanceRatio_EighthO_FO_SetA[i];
									NumOfEighthO_FO_SetA++;
								}
								if(m == 2){
									ResonanceRatio_EighthO_2O_SetA[i] = period_EighthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_2O_SetA[i]<<endl;
									
									OverallResonance_EighthO_2O_SetA = OverallResonance_EighthO_2O_SetA + ResonanceRatio_EighthO_2O_SetA[i];
									NumOfEighthO_2O_SetA++;
								}
								if(m == 3){
									ResonanceRatio_EighthO_ThirdO_SetA[i] = period_EighthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_ThirdO_SetA[i]<<endl;
									
									OverallResonance_EighthO_ThirdO_SetA = OverallResonance_EighthO_ThirdO_SetA + ResonanceRatio_EighthO_ThirdO_SetA[i];
									NumOfEighthO_ThirdO_SetA++;
								}
								if(m == 4){
									ResonanceRatio_EighthO_ForthO_SetA[i] = period_EighthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_ForthO_SetA[i]<<endl;
									
									OverallResonance_EighthO_ForthO_SetA = OverallResonance_EighthO_ForthO_SetA + ResonanceRatio_EighthO_ForthO_SetA[i];
									NumOfEighthO_ForthO_SetA++;
								}
								if(m == 5){
									ResonanceRatio_EighthO_FifthO_SetA[i] = period_EighthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_FifthO_SetA[i]<<endl;
									
									OverallResonance_EighthO_FifthO_SetA = OverallResonance_EighthO_FifthO_SetA + ResonanceRatio_EighthO_FifthO_SetA[i];
									NumOfEighthO_FifthO_SetA++;
								}
								if(m == 6){
									ResonanceRatio_EighthO_SixthO_SetA[i] = period_EighthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_SixthO_SetA[i]<<endl;
									
									OverallResonance_EighthO_SixthO_SetA = OverallResonance_EighthO_SixthO_SetA + ResonanceRatio_EighthO_SixthO_SetA[i];
									NumOfEighthO_SixthO_SetA++;
								}
								if(m == 7){
									ResonanceRatio_EighthO_SeventhO_SetA[i] = period_EighthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_SeventhO_SetA[i]<<endl;
									
									OverallResonance_EighthO_SeventhO_SetA = OverallResonance_EighthO_SeventhO_SetA + ResonanceRatio_EighthO_SeventhO_SetA[i];
									NumOfEighthO_SeventhO_SetA++;
								}
								if(m == 8){
									ResonanceRatio_EighthO_EighthO_SetA[i] = period_EighthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_EighthO_SetA[i]<<endl;
									
									OverallResonance_EighthO_EighthO_SetA = OverallResonance_EighthO_EighthO_SetA + ResonanceRatio_EighthO_EighthO_SetA[i];
									NumOfEighthO_EighthO_SetA++;
								}
								if(m == 9){
									ResonanceRatio_EighthO_NinthO_SetA[i] = period_EighthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_NinthO_SetA[i]<<endl;
									
									OverallResonance_EighthO_NinthO_SetA = OverallResonance_EighthO_NinthO_SetA + ResonanceRatio_EighthO_NinthO_SetA[i];
									NumOfEighthO_NinthO_SetA++;
								}
							}
							//SetB
							if(j == 1){
								IsSelected_EighthO_SetB[i] = true;
								if(m == 0){
									ResonanceRatio_EighthO_FU_SetB[i] = period_EighthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_FU_SetB[i]<<endl;
									
									OverallResonance_EighthO_FU_SetB = OverallResonance_EighthO_FU_SetB + ResonanceRatio_EighthO_FU_SetB[i];
									NumOfEighthO_FU_SetB++;
								}
								if(m == 1){
									ResonanceRatio_EighthO_FO_SetB[i] = period_EighthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_FO_SetB[i]<<endl;
									
									OverallResonance_EighthO_FO_SetB = OverallResonance_EighthO_FO_SetB + ResonanceRatio_EighthO_FO_SetB[i];
									NumOfEighthO_FO_SetB++;
								}
								if(m == 2){
									ResonanceRatio_EighthO_2O_SetB[i] = period_EighthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_2O_SetB[i]<<endl;
									
									OverallResonance_EighthO_2O_SetB = OverallResonance_EighthO_2O_SetB + ResonanceRatio_EighthO_2O_SetB[i];
									NumOfEighthO_2O_SetB++;
								}
								if(m == 3){
									ResonanceRatio_EighthO_ThirdO_SetB[i] = period_EighthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_ThirdO_SetB[i]<<endl;
									
									OverallResonance_EighthO_ThirdO_SetB = OverallResonance_EighthO_ThirdO_SetB + ResonanceRatio_EighthO_ThirdO_SetB[i];
									NumOfEighthO_ThirdO_SetB++;
								}
								if(m == 4){
									ResonanceRatio_EighthO_ForthO_SetB[i] = period_EighthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_ForthO_SetB[i]<<endl;
									
									OverallResonance_EighthO_ForthO_SetB = OverallResonance_EighthO_ForthO_SetB + ResonanceRatio_EighthO_ForthO_SetB[i];
									NumOfEighthO_ForthO_SetB++;
								}
								if(m == 5){
									ResonanceRatio_EighthO_FifthO_SetB[i] = period_EighthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_FifthO_SetB[i]<<endl;
									
									OverallResonance_EighthO_FifthO_SetB = OverallResonance_EighthO_FifthO_SetB + ResonanceRatio_EighthO_FifthO_SetB[i];
									NumOfEighthO_FifthO_SetB++;
								}
								if(m == 6){
									ResonanceRatio_EighthO_SixthO_SetB[i] = period_EighthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_SixthO_SetB[i]<<endl;
									
									OverallResonance_EighthO_SixthO_SetB = OverallResonance_EighthO_SixthO_SetB + ResonanceRatio_EighthO_SixthO_SetB[i];
									NumOfEighthO_SixthO_SetB++;
								}
								if(m == 7){
									ResonanceRatio_EighthO_SeventhO_SetB[i] = period_EighthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_SeventhO_SetB[i]<<endl;
									
									OverallResonance_EighthO_SeventhO_SetB = OverallResonance_EighthO_SeventhO_SetB + ResonanceRatio_EighthO_SeventhO_SetB[i];
									NumOfEighthO_SeventhO_SetB++;
								}
								if(m == 8){
									ResonanceRatio_EighthO_EighthO_SetB[i] = period_EighthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_EighthO_SetB[i]<<endl;
									
									OverallResonance_EighthO_EighthO_SetB = OverallResonance_EighthO_EighthO_SetB + ResonanceRatio_EighthO_EighthO_SetB[i];
									NumOfEighthO_EighthO_SetB++;
								}
								if(m == 9){
									ResonanceRatio_EighthO_NinthO_SetB[i] = period_EighthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_NinthO_SetB[i]<<endl;
									
									OverallResonance_EighthO_NinthO_SetB = OverallResonance_EighthO_NinthO_SetB + ResonanceRatio_EighthO_NinthO_SetB[i];
									NumOfEighthO_NinthO_SetB++;
								}
							}
							//SetC
							if(j == 2){
								IsSelected_EighthO_SetC[i] = true;
								if(m == 0){
									ResonanceRatio_EighthO_FU_SetC[i] = period_EighthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_FU_SetC[i]<<endl;
									
									OverallResonance_EighthO_FU_SetC = OverallResonance_EighthO_FU_SetC + ResonanceRatio_EighthO_FU_SetC[i];
									NumOfEighthO_FU_SetC++;
								}
								if(m == 1){
									ResonanceRatio_EighthO_FO_SetC[i] = period_EighthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_FO_SetC[i]<<endl;
									
									OverallResonance_EighthO_FO_SetC = OverallResonance_EighthO_FO_SetC + ResonanceRatio_EighthO_FO_SetC[i];
									NumOfEighthO_FO_SetC++;
								}
								if(m == 2){
									ResonanceRatio_EighthO_2O_SetC[i] = period_EighthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_2O_SetC[i]<<endl;
									
									OverallResonance_EighthO_2O_SetC = OverallResonance_EighthO_2O_SetC + ResonanceRatio_EighthO_2O_SetC[i];
									NumOfEighthO_2O_SetC++;
								}
								if(m == 3){
									ResonanceRatio_EighthO_ThirdO_SetC[i] = period_EighthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_ThirdO_SetC[i]<<endl;
									
									OverallResonance_EighthO_ThirdO_SetC = OverallResonance_EighthO_ThirdO_SetC + ResonanceRatio_EighthO_ThirdO_SetC[i];
									NumOfEighthO_ThirdO_SetC++;
								}
								if(m == 4){
									ResonanceRatio_EighthO_ForthO_SetC[i] = period_EighthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_ForthO_SetC[i]<<endl;
									
									OverallResonance_EighthO_ForthO_SetC = OverallResonance_EighthO_ForthO_SetC + ResonanceRatio_EighthO_ForthO_SetC[i];
									NumOfEighthO_ForthO_SetC++;
								}
								if(m == 5){
									ResonanceRatio_EighthO_FifthO_SetC[i] = period_EighthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_FifthO_SetC[i]<<endl;
									
									OverallResonance_EighthO_FifthO_SetC = OverallResonance_EighthO_FifthO_SetC + ResonanceRatio_EighthO_FifthO_SetC[i];
									NumOfEighthO_FifthO_SetC++;
								}
								if(m == 6){
									ResonanceRatio_EighthO_SixthO_SetC[i] = period_EighthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_SixthO_SetC[i]<<endl;
									
									OverallResonance_EighthO_SixthO_SetC = OverallResonance_EighthO_SixthO_SetC + ResonanceRatio_EighthO_SixthO_SetC[i];
									NumOfEighthO_SixthO_SetC++;
								}
								if(m == 7){
									ResonanceRatio_EighthO_SeventhO_SetC[i] = period_EighthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_SeventhO_SetC[i]<<endl;
									
									OverallResonance_EighthO_SeventhO_SetC = OverallResonance_EighthO_SeventhO_SetC + ResonanceRatio_EighthO_SeventhO_SetC[i];
									NumOfEighthO_SeventhO_SetC++;
								}
								if(m == 8){
									ResonanceRatio_EighthO_EighthO_SetC[i] = period_EighthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_EighthO_SetC[i]<<endl;
									
									OverallResonance_EighthO_EighthO_SetC = OverallResonance_EighthO_EighthO_SetC + ResonanceRatio_EighthO_EighthO_SetC[i];
									NumOfEighthO_EighthO_SetC++;
								}
								if(m == 9){
									ResonanceRatio_EighthO_NinthO_SetC[i] = period_EighthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_NinthO_SetC[i]<<endl;
									
									OverallResonance_EighthO_NinthO_SetC = OverallResonance_EighthO_NinthO_SetC + ResonanceRatio_EighthO_NinthO_SetC[i];
									NumOfEighthO_NinthO_SetC++;
								}
							}
							//SetD
							if(j == 3){
								IsSelected_EighthO_SetD[i] = true;
								if(m == 0){
									ResonanceRatio_EighthO_FU_SetD[i] = period_EighthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_FU_SetD[i]<<endl;
									
									OverallResonance_EighthO_FU_SetD = OverallResonance_EighthO_FU_SetD + ResonanceRatio_EighthO_FU_SetD[i];
									NumOfEighthO_FU_SetD++;
								}
								if(m == 1){
									ResonanceRatio_EighthO_FO_SetD[i] = period_EighthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_FO_SetD[i]<<endl;
									
									OverallResonance_EighthO_FO_SetD = OverallResonance_EighthO_FO_SetD + ResonanceRatio_EighthO_FO_SetD[i];
									NumOfEighthO_FO_SetD++;
								}
								if(m == 2){
									ResonanceRatio_EighthO_2O_SetD[i] = period_EighthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_2O_SetD[i]<<endl;
									
									OverallResonance_EighthO_2O_SetD = OverallResonance_EighthO_2O_SetD + ResonanceRatio_EighthO_2O_SetD[i];
									NumOfEighthO_2O_SetD++;
								}
								if(m == 3){
									ResonanceRatio_EighthO_ThirdO_SetD[i] = period_EighthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_ThirdO_SetD[i]<<endl;
									
									OverallResonance_EighthO_ThirdO_SetD = OverallResonance_EighthO_ThirdO_SetD + ResonanceRatio_EighthO_ThirdO_SetD[i];
									NumOfEighthO_ThirdO_SetD++;
								}
								if(m == 4){
									ResonanceRatio_EighthO_ForthO_SetD[i] = period_EighthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_ForthO_SetD[i]<<endl;
									
									OverallResonance_EighthO_ForthO_SetD = OverallResonance_EighthO_ForthO_SetD + ResonanceRatio_EighthO_ForthO_SetD[i];
									NumOfEighthO_ForthO_SetD++;
								}
								if(m == 5){
									ResonanceRatio_EighthO_FifthO_SetD[i] = period_EighthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_FifthO_SetD[i]<<endl;
									
									OverallResonance_EighthO_FifthO_SetD = OverallResonance_EighthO_FifthO_SetD + ResonanceRatio_EighthO_FifthO_SetD[i];
									NumOfEighthO_FifthO_SetD++;
								}
								if(m == 6){
									ResonanceRatio_EighthO_SixthO_SetD[i] = period_EighthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_SixthO_SetD[i]<<endl;
									
									OverallResonance_EighthO_SixthO_SetD = OverallResonance_EighthO_SixthO_SetD + ResonanceRatio_EighthO_SixthO_SetD[i];
									NumOfEighthO_SixthO_SetD++;
								}
								if(m == 7){
									ResonanceRatio_EighthO_SeventhO_SetD[i] = period_EighthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_SeventhO_SetD[i]<<endl;
									
									OverallResonance_EighthO_SeventhO_SetD = OverallResonance_EighthO_SeventhO_SetD + ResonanceRatio_EighthO_SeventhO_SetD[i];
									NumOfEighthO_SeventhO_SetD++;
								}
								if(m == 8){
									ResonanceRatio_EighthO_EighthO_SetD[i] = period_EighthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_EighthO_SetD[i]<<endl;
									
									OverallResonance_EighthO_EighthO_SetD = OverallResonance_EighthO_EighthO_SetD + ResonanceRatio_EighthO_EighthO_SetD[i];
									NumOfEighthO_EighthO_SetD++;
								}
								if(m == 9){
									ResonanceRatio_EighthO_NinthO_SetD[i] = period_EighthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_EighthO_NinthO_SetD[i]<<endl;
									
									OverallResonance_EighthO_NinthO_SetD = OverallResonance_EighthO_NinthO_SetD + ResonanceRatio_EighthO_NinthO_SetD[i];
									NumOfEighthO_NinthO_SetD++;
								}
							}
						}
					}	
					if(!DoBoundsCut || ((ratio_temp_NinthO <= (1. + bound)) && (ratio_temp_NinthO >= (1. - bound))) || ((ratio_temp_NinthO <= (0.5 + bound)) && (ratio_temp_NinthO >= (0.5 - bound))) || ((ratio_temp_NinthO <= (1.5 + bound)) && (ratio_temp_NinthO >= (1.5 - bound))) || ((ratio_temp_NinthO <= (2.0 + bound)) && (ratio_temp_NinthO >= (2.0 - bound))) || ((ratio_temp_NinthO <= (2.5 + bound)) && (ratio_temp_NinthO >= (2.5 - bound))) || ((ratio_temp_NinthO <= (3.0 + bound)) && (ratio_temp_NinthO >= (3.0 - bound))) || ((ratio_temp_NinthO <= (3.5 + bound)) && (ratio_temp_NinthO >= (3.5 - bound))) || ((ratio_temp_NinthO <= (4.0 + bound)) && (ratio_temp_NinthO >= (4.0 - bound))) || ((ratio_temp_NinthO <= (4.5 + bound)) && (ratio_temp_NinthO >= (4.5 - bound))) || ((ratio_temp_NinthO <= (5.0 + bound)) && (ratio_temp_NinthO >= (5.0 - bound))) || ((ratio_temp_NinthO <= (5.5 + bound)) && (ratio_temp_NinthO >= (5.5 - bound))) || ((ratio_temp_NinthO <= (6.0 + bound)) && (ratio_temp_NinthO >= (6.0 - bound)))){
						//NinthO mode//
						if((growth > 0.0 || !DoPosGrowthCut) && (growth_NinthO > 0)){
							cout<<"Resonance Ratios With NinthO..."<<endl;
							//SetA
							if(j == 0){
								IsSelected_NinthO_SetA[i] = true;
								if(m == 0){
									ResonanceRatio_NinthO_FU_SetA[i] = period_NinthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_FU_SetA[i]<<endl;
									
									OverallResonance_NinthO_FU_SetA = OverallResonance_NinthO_FU_SetA + ResonanceRatio_NinthO_FU_SetA[i];
									NumOfNinthO_FU_SetA++;
								}
								if(m == 1){
									ResonanceRatio_NinthO_FO_SetA[i] = period_NinthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_FO_SetA[i]<<endl;
									
									OverallResonance_NinthO_FO_SetA = OverallResonance_NinthO_FO_SetA + ResonanceRatio_NinthO_FO_SetA[i];
									NumOfNinthO_FO_SetA++;
								}
								if(m == 2){
									ResonanceRatio_NinthO_2O_SetA[i] = period_NinthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_2O_SetA[i]<<endl;
									
									OverallResonance_NinthO_2O_SetA = OverallResonance_NinthO_2O_SetA + ResonanceRatio_NinthO_2O_SetA[i];
									NumOfNinthO_2O_SetA++;
								}
								if(m == 3){
									ResonanceRatio_NinthO_ThirdO_SetA[i] = period_NinthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_ThirdO_SetA[i]<<endl;
									
									OverallResonance_NinthO_ThirdO_SetA = OverallResonance_NinthO_ThirdO_SetA + ResonanceRatio_NinthO_ThirdO_SetA[i];
									NumOfNinthO_ThirdO_SetA++;
								}
								if(m == 4){
									ResonanceRatio_NinthO_ForthO_SetA[i] = period_NinthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_ForthO_SetA[i]<<endl;
									
									OverallResonance_NinthO_ForthO_SetA = OverallResonance_NinthO_ForthO_SetA + ResonanceRatio_NinthO_ForthO_SetA[i];
									NumOfNinthO_ForthO_SetA++;
								}
								if(m == 5){
									ResonanceRatio_NinthO_FifthO_SetA[i] = period_NinthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_FifthO_SetA[i]<<endl;
									
									OverallResonance_NinthO_FifthO_SetA = OverallResonance_NinthO_FifthO_SetA + ResonanceRatio_NinthO_FifthO_SetA[i];
									NumOfNinthO_FifthO_SetA++;
								}
								if(m == 6){
									ResonanceRatio_NinthO_SixthO_SetA[i] = period_NinthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_SixthO_SetA[i]<<endl;
									
									OverallResonance_NinthO_SixthO_SetA = OverallResonance_NinthO_SixthO_SetA + ResonanceRatio_NinthO_SixthO_SetA[i];
									NumOfNinthO_SixthO_SetA++;
								}
								if(m == 7){
									ResonanceRatio_NinthO_SeventhO_SetA[i] = period_NinthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_SeventhO_SetA[i]<<endl;
									
									OverallResonance_NinthO_SeventhO_SetA = OverallResonance_NinthO_SeventhO_SetA + ResonanceRatio_NinthO_SeventhO_SetA[i];
									NumOfNinthO_SeventhO_SetA++;
								}
								if(m == 8){
									ResonanceRatio_NinthO_EighthO_SetA[i] = period_NinthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_EighthO_SetA[i]<<endl;
									
									OverallResonance_NinthO_EighthO_SetA = OverallResonance_NinthO_EighthO_SetA + ResonanceRatio_NinthO_EighthO_SetA[i];
									NumOfNinthO_EighthO_SetA++;
								}
								if(m == 9){
									ResonanceRatio_NinthO_NinthO_SetA[i] = period_NinthO_SetA[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_NinthO_SetA[i]<<endl;
									
									OverallResonance_NinthO_NinthO_SetA = OverallResonance_NinthO_NinthO_SetA + ResonanceRatio_NinthO_NinthO_SetA[i];
									NumOfNinthO_NinthO_SetA++;
								}
							}
							//SetB
							if(j == 1){
								IsSelected_NinthO_SetB[i] = true;
								if(m == 0){
									ResonanceRatio_NinthO_FU_SetB[i] = period_NinthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_FU_SetB[i]<<endl;
									
									OverallResonance_NinthO_FU_SetB = OverallResonance_NinthO_FU_SetB + ResonanceRatio_NinthO_FU_SetB[i];
									NumOfNinthO_FU_SetB++;
								}
								if(m == 1){
									ResonanceRatio_NinthO_FO_SetB[i] = period_NinthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_FO_SetB[i]<<endl;
									
									OverallResonance_NinthO_FO_SetB = OverallResonance_NinthO_FO_SetB + ResonanceRatio_NinthO_FO_SetB[i];
									NumOfNinthO_FO_SetB++;
								}
								if(m == 2){
									ResonanceRatio_NinthO_2O_SetB[i] = period_NinthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_2O_SetB[i]<<endl;
									
									OverallResonance_NinthO_2O_SetB = OverallResonance_NinthO_2O_SetB + ResonanceRatio_NinthO_2O_SetB[i];
									NumOfNinthO_2O_SetB++;
								}
								if(m == 3){
									ResonanceRatio_NinthO_ThirdO_SetB[i] = period_NinthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_ThirdO_SetB[i]<<endl;
									
									OverallResonance_NinthO_ThirdO_SetB = OverallResonance_NinthO_ThirdO_SetB + ResonanceRatio_NinthO_ThirdO_SetB[i];
									NumOfNinthO_ThirdO_SetB++;
								}
								if(m == 4){
									ResonanceRatio_NinthO_ForthO_SetB[i] = period_NinthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_ForthO_SetB[i]<<endl;
									
									OverallResonance_NinthO_ForthO_SetB = OverallResonance_NinthO_ForthO_SetB + ResonanceRatio_NinthO_ForthO_SetB[i];
									NumOfNinthO_ForthO_SetB++;
								}
								if(m == 5){
									ResonanceRatio_NinthO_FifthO_SetB[i] = period_NinthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_FifthO_SetB[i]<<endl;
									
									OverallResonance_NinthO_FifthO_SetB = OverallResonance_NinthO_FifthO_SetB + ResonanceRatio_NinthO_FifthO_SetB[i];
									NumOfNinthO_FifthO_SetB++;
								}
								if(m == 6){
									ResonanceRatio_NinthO_SixthO_SetB[i] = period_NinthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_SixthO_SetB[i]<<endl;
									
									OverallResonance_NinthO_SixthO_SetB = OverallResonance_NinthO_SixthO_SetB + ResonanceRatio_NinthO_SixthO_SetB[i];
									NumOfNinthO_SixthO_SetB++;
								}
								if(m == 7){
									ResonanceRatio_NinthO_SeventhO_SetB[i] = period_NinthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_SeventhO_SetB[i]<<endl;
									
									OverallResonance_NinthO_SeventhO_SetB = OverallResonance_NinthO_SeventhO_SetB + ResonanceRatio_NinthO_SeventhO_SetB[i];
									NumOfNinthO_SeventhO_SetB++;
								}
								if(m == 8){
									ResonanceRatio_NinthO_EighthO_SetB[i] = period_NinthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_EighthO_SetB[i]<<endl;
									
									OverallResonance_NinthO_EighthO_SetB = OverallResonance_NinthO_EighthO_SetB + ResonanceRatio_NinthO_EighthO_SetB[i];
									NumOfNinthO_EighthO_SetB++;
								}
								if(m == 9){
									ResonanceRatio_NinthO_NinthO_SetB[i] = period_NinthO_SetB[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_NinthO_SetB[i]<<endl;
									
									OverallResonance_NinthO_NinthO_SetB = OverallResonance_NinthO_NinthO_SetB + ResonanceRatio_NinthO_NinthO_SetB[i];
									NumOfNinthO_NinthO_SetB++;
								}
							}
							//SetC
							if(j == 2){
								IsSelected_NinthO_SetC[i] = true;
								if(m == 0){
									ResonanceRatio_NinthO_FU_SetC[i] = period_NinthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_FU_SetC[i]<<endl;
									
									OverallResonance_NinthO_FU_SetC = OverallResonance_NinthO_FU_SetC + ResonanceRatio_NinthO_FU_SetC[i];
									NumOfNinthO_FU_SetC++;
								}
								if(m == 1){
									ResonanceRatio_NinthO_FO_SetC[i] = period_NinthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_FO_SetC[i]<<endl;
									
									OverallResonance_NinthO_FO_SetC = OverallResonance_NinthO_FO_SetC + ResonanceRatio_NinthO_FO_SetC[i];
									NumOfNinthO_FO_SetC++;
								}
								if(m == 2){
									ResonanceRatio_NinthO_2O_SetC[i] = period_NinthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_2O_SetC[i]<<endl;
									
									OverallResonance_NinthO_2O_SetC = OverallResonance_NinthO_2O_SetC + ResonanceRatio_NinthO_2O_SetC[i];
									NumOfNinthO_2O_SetC++;
								}
								if(m == 3){
									ResonanceRatio_NinthO_ThirdO_SetC[i] = period_NinthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_ThirdO_SetC[i]<<endl;
									
									OverallResonance_NinthO_ThirdO_SetC = OverallResonance_NinthO_ThirdO_SetC + ResonanceRatio_NinthO_ThirdO_SetC[i];
									NumOfNinthO_ThirdO_SetC++;
								}
								if(m == 4){
									ResonanceRatio_NinthO_ForthO_SetC[i] = period_NinthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_ForthO_SetC[i]<<endl;
									
									OverallResonance_NinthO_ForthO_SetC = OverallResonance_NinthO_ForthO_SetC + ResonanceRatio_NinthO_ForthO_SetC[i];
									NumOfNinthO_ForthO_SetC++;
								}
								if(m == 5){
									ResonanceRatio_NinthO_FifthO_SetC[i] = period_NinthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_FifthO_SetC[i]<<endl;
									
									OverallResonance_NinthO_FifthO_SetC = OverallResonance_NinthO_FifthO_SetC + ResonanceRatio_NinthO_FifthO_SetC[i];
									NumOfNinthO_FifthO_SetC++;
								}
								if(m == 6){
									ResonanceRatio_NinthO_SixthO_SetC[i] = period_NinthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_SixthO_SetC[i]<<endl;
									
									OverallResonance_NinthO_SixthO_SetC = OverallResonance_NinthO_SixthO_SetC + ResonanceRatio_NinthO_SixthO_SetC[i];
									NumOfNinthO_SixthO_SetC++;
								}
								if(m == 7){
									ResonanceRatio_NinthO_SeventhO_SetC[i] = period_NinthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_SeventhO_SetC[i]<<endl;
									
									OverallResonance_NinthO_SeventhO_SetC = OverallResonance_NinthO_SeventhO_SetC + ResonanceRatio_NinthO_SeventhO_SetC[i];
									NumOfNinthO_SeventhO_SetC++;
								}
								if(m == 8){
									ResonanceRatio_NinthO_EighthO_SetC[i] = period_NinthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_EighthO_SetC[i]<<endl;
									
									OverallResonance_NinthO_EighthO_SetC = OverallResonance_NinthO_EighthO_SetC + ResonanceRatio_NinthO_EighthO_SetC[i];
									NumOfNinthO_EighthO_SetC++;
								}
								if(m == 9){
									ResonanceRatio_NinthO_NinthO_SetC[i] = period_NinthO_SetC[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_NinthO_SetC[i]<<endl;
									
									OverallResonance_NinthO_NinthO_SetC = OverallResonance_NinthO_NinthO_SetC + ResonanceRatio_NinthO_NinthO_SetC[i];
									NumOfNinthO_NinthO_SetC++;
								}
							}
							//SetD
							if(j == 3){
								IsSelected_NinthO_SetD[i] = true;
								if(m == 0){
									ResonanceRatio_NinthO_FU_SetD[i] = period_NinthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_FU_SetD[i]<<endl;
									
									OverallResonance_NinthO_FU_SetD = OverallResonance_NinthO_FU_SetD + ResonanceRatio_NinthO_FU_SetD[i];
									NumOfNinthO_FU_SetD++;
								}
								if(m == 1){
									ResonanceRatio_NinthO_FO_SetD[i] = period_NinthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_FO_SetD[i]<<endl;
									
									OverallResonance_NinthO_FO_SetD = OverallResonance_NinthO_FO_SetD + ResonanceRatio_NinthO_FO_SetD[i];
									NumOfNinthO_FO_SetD++;
								}
								if(m == 2){
									ResonanceRatio_NinthO_2O_SetD[i] = period_NinthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_2O_SetD[i]<<endl;
									
									OverallResonance_NinthO_2O_SetD = OverallResonance_NinthO_2O_SetD + ResonanceRatio_NinthO_2O_SetD[i];
									NumOfNinthO_2O_SetD++;
								}
								if(m == 3){
									ResonanceRatio_NinthO_ThirdO_SetD[i] = period_NinthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_ThirdO_SetD[i]<<endl;
									
									OverallResonance_NinthO_ThirdO_SetD = OverallResonance_NinthO_ThirdO_SetD + ResonanceRatio_NinthO_ThirdO_SetD[i];
									NumOfNinthO_ThirdO_SetD++;
								}
								if(m == 4){
									ResonanceRatio_NinthO_ForthO_SetD[i] = period_NinthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_ForthO_SetD[i]<<endl;
									
									OverallResonance_NinthO_ForthO_SetD = OverallResonance_NinthO_ForthO_SetD + ResonanceRatio_NinthO_ForthO_SetD[i];
									NumOfNinthO_ForthO_SetD++;
								}
								if(m == 5){
									ResonanceRatio_NinthO_FifthO_SetD[i] = period_NinthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_FifthO_SetD[i]<<endl;
									
									OverallResonance_NinthO_FifthO_SetD = OverallResonance_NinthO_FifthO_SetD + ResonanceRatio_NinthO_FifthO_SetD[i];
									NumOfNinthO_FifthO_SetD++;
								}
								if(m == 6){
									ResonanceRatio_NinthO_SixthO_SetD[i] = period_NinthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_SixthO_SetD[i]<<endl;
									
									OverallResonance_NinthO_SixthO_SetD = OverallResonance_NinthO_SixthO_SetD + ResonanceRatio_NinthO_SixthO_SetD[i];
									NumOfNinthO_SixthO_SetD++;
								}
								if(m == 7){
									ResonanceRatio_NinthO_SeventhO_SetD[i] = period_NinthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_SeventhO_SetD[i]<<endl;
									
									OverallResonance_NinthO_SeventhO_SetD = OverallResonance_NinthO_SeventhO_SetD + ResonanceRatio_NinthO_SeventhO_SetD[i];
									NumOfNinthO_SeventhO_SetD++;
								}
								if(m == 8){
									ResonanceRatio_NinthO_EighthO_SetD[i] = period_NinthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_EighthO_SetD[i]<<endl;
									
									OverallResonance_NinthO_EighthO_SetD = OverallResonance_NinthO_EighthO_SetD + ResonanceRatio_NinthO_EighthO_SetD[i];
									NumOfNinthO_EighthO_SetD++;
								}
								if(m == 9){
									ResonanceRatio_NinthO_NinthO_SetD[i] = period_NinthO_SetD[i]/period;
									cout<<m<<": "<<ResonanceRatio_NinthO_NinthO_SetD[i]<<endl;
									
									OverallResonance_NinthO_NinthO_SetD = OverallResonance_NinthO_NinthO_SetD + ResonanceRatio_NinthO_NinthO_SetD[i];
									NumOfNinthO_NinthO_SetD++;
								}
							}
						}
					}
				}
			}
			inFile.close();
		}
		
		////Output////
		outFile_AverageResonance.open(AverageResonanceFileName, ios::out);
		outFile_AverageResonance<<setw(30)<<"Resonances with FU"<<setw(30)<<"Resonances with FO"<<setw(30)<<"Resonances with 2O"<<setw(30)<<"Resonances with ThirdO"<<setw(30)<<"Resonances with ForthO"<<setw(30)<<"Resonances with FifthO"<<setw(30)<<"Resonances with SixthO"<<setw(30)<<"Resonances with SeventhO"<<setw(30)<<"Resonances with EighthO"<<setw(30)<<"Resonances with NinthO"<<endl;
		
		outFile_SD.open(SDFileName, ios::out);
		outFile_SD<<setw(30)<<"Resonances with FU"<<setw(30)<<"Resonances with FO"<<setw(30)<<"Resonances with 2O"<<setw(30)<<"Resonances with ThirdO"<<setw(30)<<"Resonances with ForthO"<<setw(30)<<"Resonances with FifthO"<<setw(30)<<"Resonances with SixthO"<<setw(30)<<"Resonances with SeventhO"<<setw(30)<<"Resonances with EighthO"<<setw(30)<<"Resonances with NinthO"<<endl;
		
		outFile_Num.open(NumFileName, ios::out);
		outFile_Num<<setw(30)<<"Resonances with FU"<<setw(30)<<"Resonances with FO"<<setw(30)<<"Resonances with 2O"<<setw(30)<<"Resonances with ThirdO"<<setw(30)<<"Resonances with ForthO"<<setw(30)<<"Resonances with FifthO"<<setw(30)<<"Resonances with SixthO"<<setw(30)<<"Resonances with SeventhO"<<setw(30)<<"Resonances with EighthO"<<setw(30)<<"Resonances with NinthO"<<endl;
		
		outFile_Fig5_FU_FO.open(Fig5_FU_FO_FileName, ios::out);
		outFile_Fig5_FU_FO<<setw(10)<<"Teff"<<setw(20)<<"freq_FU"<<setw(20)<<"freq_FO"<<setw(20)<<"freq_2O"<<setw(20)<<"freq_ThirdO"<<setw(20)<<"freq_ForthO"<<setw(20)<<"growth_FU"<<setw(20)<<"growth_FO"<<setw(20)<<"growth_2O"<<setw(20)<<"growth_ThirdO"<<setw(20)<<"growth_ForthO"<<endl;
		
		if(j == 0){
			////SD Calc Part I (To get var)////
			for(int i = 0; i < NumOfModels; i++){
				double mean;
				//FU as check//
				if(IsSelected_FU_SetA[i]){
					mean = (OverallResonance_FU_FU_SetA/NumOfFU_FU_SetA);
					var_FU_FU_SetA = var_FU_FU_SetA + pow((ResonanceRatio_FU_FU_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FU_FO_SetA/NumOfFU_FO_SetA);
					var_FU_FO_SetA = var_FU_FO_SetA + pow((ResonanceRatio_FU_FO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FU_2O_SetA/NumOfFU_2O_SetA);
					var_FU_2O_SetA = var_FU_2O_SetA + pow((ResonanceRatio_FU_2O_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FU_ThirdO_SetA/NumOfFU_ThirdO_SetA);
					var_FU_ThirdO_SetA = var_FU_ThirdO_SetA + pow((ResonanceRatio_FU_ThirdO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FU_ForthO_SetA/NumOfFU_ForthO_SetA);
					var_FU_ForthO_SetA = var_FU_ForthO_SetA + pow((ResonanceRatio_FU_ForthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FU_FifthO_SetA/NumOfFU_FifthO_SetA);
					var_FU_FifthO_SetA = var_FU_FifthO_SetA + pow((ResonanceRatio_FU_FifthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FU_SixthO_SetA/NumOfFU_SixthO_SetA);
					var_FU_SixthO_SetA = var_FU_SixthO_SetA + pow((ResonanceRatio_FU_SixthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FU_SeventhO_SetA/NumOfFU_SeventhO_SetA);
					var_FU_SeventhO_SetA = var_FU_SeventhO_SetA + pow((ResonanceRatio_FU_SeventhO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FU_EighthO_SetA/NumOfFU_EighthO_SetA);
					var_FU_EighthO_SetA = var_FU_EighthO_SetA + pow((ResonanceRatio_FU_EighthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FU_NinthO_SetA/NumOfFU_NinthO_SetA);
					var_FU_NinthO_SetA = var_FU_NinthO_SetA + pow((ResonanceRatio_FU_NinthO_SetA[i] - mean),2.);
				}
							
				//FO//			
				if(IsSelected_FO_SetA[i]){
					mean = (OverallResonance_FO_FU_SetA/NumOfFO_FU_SetA);
					var_FO_FU_SetA = var_FO_FU_SetA + pow((ResonanceRatio_FO_FU_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FO_FO_SetA/NumOfFO_FO_SetA);
					var_FO_FO_SetA = var_FO_FO_SetA + pow((ResonanceRatio_FO_FO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FO_2O_SetA/NumOfFO_2O_SetA);
					var_FO_2O_SetA = var_FO_2O_SetA + pow((ResonanceRatio_FO_2O_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FO_ThirdO_SetA/NumOfFO_ThirdO_SetA);
					var_FO_ThirdO_SetA = var_FO_ThirdO_SetA + pow((ResonanceRatio_FO_ThirdO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FO_ForthO_SetA/NumOfFO_ForthO_SetA);
					var_FO_ForthO_SetA = var_FO_ForthO_SetA + pow((ResonanceRatio_FO_ForthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FO_FifthO_SetA/NumOfFO_FifthO_SetA);
					var_FO_FifthO_SetA = var_FO_FifthO_SetA + pow((ResonanceRatio_FO_FifthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FO_SixthO_SetA/NumOfFO_SixthO_SetA);
					var_FO_SixthO_SetA = var_FO_SixthO_SetA + pow((ResonanceRatio_FO_SixthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FO_SeventhO_SetA/NumOfFO_SeventhO_SetA);
					var_FO_SeventhO_SetA = var_FO_SeventhO_SetA + pow((ResonanceRatio_FO_SeventhO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FO_EighthO_SetA/NumOfFO_EighthO_SetA);
					var_FO_EighthO_SetA = var_FO_EighthO_SetA + pow((ResonanceRatio_FO_EighthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FO_NinthO_SetA/NumOfFO_NinthO_SetA);
					var_FO_NinthO_SetA = var_FO_NinthO_SetA + pow((ResonanceRatio_FO_NinthO_SetA[i] - mean),2.);
				}
			
				//2O//
				if(IsSelected_2O_SetA[i]){
					mean = (OverallResonance_2O_FU_SetA/NumOf2O_FU_SetA);
					var_2O_FU_SetA = var_2O_FU_SetA + pow((ResonanceRatio_2O_FU_SetA[i] - mean),2.);
					
					mean = (OverallResonance_2O_FO_SetA/NumOf2O_FO_SetA);
					var_2O_FO_SetA = var_2O_FO_SetA + pow((ResonanceRatio_2O_FO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_2O_2O_SetA/NumOf2O_2O_SetA);
					var_2O_2O_SetA = var_2O_2O_SetA + pow((ResonanceRatio_2O_2O_SetA[i] - mean),2.);
					
					mean = (OverallResonance_2O_ThirdO_SetA/NumOf2O_ThirdO_SetA);
					var_2O_ThirdO_SetA = var_2O_ThirdO_SetA + pow((ResonanceRatio_2O_ThirdO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_2O_ForthO_SetA/NumOf2O_ForthO_SetA);
					var_2O_ForthO_SetA = var_2O_ForthO_SetA + pow((ResonanceRatio_2O_ForthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_2O_FifthO_SetA/NumOf2O_FifthO_SetA);
					var_2O_FifthO_SetA = var_2O_FifthO_SetA + pow((ResonanceRatio_2O_FifthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_2O_SixthO_SetA/NumOf2O_SixthO_SetA);
					var_2O_SixthO_SetA = var_2O_SixthO_SetA + pow((ResonanceRatio_2O_SixthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_2O_SeventhO_SetA/NumOf2O_SeventhO_SetA);
					var_2O_SeventhO_SetA = var_2O_SeventhO_SetA + pow((ResonanceRatio_2O_SeventhO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_2O_EighthO_SetA/NumOf2O_EighthO_SetA);
					var_2O_EighthO_SetA = var_2O_EighthO_SetA + pow((ResonanceRatio_2O_EighthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_2O_NinthO_SetA/NumOf2O_NinthO_SetA);
					var_2O_NinthO_SetA = var_2O_NinthO_SetA + pow((ResonanceRatio_2O_NinthO_SetA[i] - mean),2.);
				}
				
				//ThirdO//
				if(IsSelected_ThirdO_SetA[i]){
					mean = (OverallResonance_ThirdO_FU_SetA/NumOfThirdO_FU_SetA);
					var_ThirdO_FU_SetA = var_ThirdO_FU_SetA + pow((ResonanceRatio_ThirdO_FU_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_FO_SetA/NumOfThirdO_FO_SetA);
					var_ThirdO_FO_SetA = var_ThirdO_FO_SetA + pow((ResonanceRatio_ThirdO_FO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_2O_SetA/NumOfThirdO_2O_SetA);
					var_ThirdO_2O_SetA = var_ThirdO_2O_SetA + pow((ResonanceRatio_ThirdO_2O_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_ThirdO_SetA/NumOfThirdO_ThirdO_SetA);
					var_ThirdO_ThirdO_SetA = var_ThirdO_ThirdO_SetA + pow((ResonanceRatio_ThirdO_ThirdO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_ForthO_SetA/NumOfThirdO_ForthO_SetA);
					var_ThirdO_ForthO_SetA = var_ThirdO_ForthO_SetA + pow((ResonanceRatio_ThirdO_ForthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_FifthO_SetA/NumOfThirdO_FifthO_SetA);
					var_ThirdO_FifthO_SetA = var_ThirdO_FifthO_SetA + pow((ResonanceRatio_ThirdO_FifthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_SixthO_SetA/NumOfThirdO_SixthO_SetA);
					var_ThirdO_SixthO_SetA = var_ThirdO_SixthO_SetA + pow((ResonanceRatio_ThirdO_SixthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_SeventhO_SetA/NumOfThirdO_SeventhO_SetA);
					var_ThirdO_SeventhO_SetA = var_ThirdO_SeventhO_SetA + pow((ResonanceRatio_ThirdO_SeventhO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_EighthO_SetA/NumOfThirdO_EighthO_SetA);
					var_ThirdO_EighthO_SetA = var_ThirdO_EighthO_SetA + pow((ResonanceRatio_ThirdO_EighthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_NinthO_SetA/NumOfThirdO_NinthO_SetA);
					var_ThirdO_NinthO_SetA = var_ThirdO_NinthO_SetA + pow((ResonanceRatio_ThirdO_NinthO_SetA[i] - mean),2.);
				}
			
				//ForthO//
				if(IsSelected_ForthO_SetA[i]){
					mean = (OverallResonance_ForthO_FU_SetA/NumOfForthO_FU_SetA);
					var_ForthO_FU_SetA = var_ForthO_FU_SetA + pow((ResonanceRatio_ForthO_FU_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_FO_SetA/NumOfForthO_FO_SetA);
					var_ForthO_FO_SetA = var_ForthO_FO_SetA + pow((ResonanceRatio_ForthO_FO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_2O_SetA/NumOfForthO_2O_SetA);
					var_ForthO_2O_SetA = var_ForthO_2O_SetA + pow((ResonanceRatio_ForthO_2O_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_ThirdO_SetA/NumOfForthO_ThirdO_SetA);
					var_ForthO_ThirdO_SetA = var_ForthO_ThirdO_SetA + pow((ResonanceRatio_ForthO_ThirdO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_ForthO_SetA/NumOfForthO_ForthO_SetA);
					var_ForthO_ForthO_SetA = var_ForthO_ForthO_SetA + pow((ResonanceRatio_ForthO_ForthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_FifthO_SetA/NumOfForthO_FifthO_SetA);
					var_ForthO_FifthO_SetA = var_ForthO_FifthO_SetA + pow((ResonanceRatio_ForthO_FifthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_SixthO_SetA/NumOfForthO_SixthO_SetA);
					var_ForthO_SixthO_SetA = var_ForthO_SixthO_SetA + pow((ResonanceRatio_ForthO_SixthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_SeventhO_SetA/NumOfForthO_SeventhO_SetA);
					var_ForthO_SeventhO_SetA = var_ForthO_SeventhO_SetA + pow((ResonanceRatio_ForthO_SeventhO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_EighthO_SetA/NumOfForthO_EighthO_SetA);
					var_ForthO_EighthO_SetA = var_ForthO_EighthO_SetA + pow((ResonanceRatio_ForthO_EighthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_NinthO_SetA/NumOfForthO_NinthO_SetA);
					var_ForthO_NinthO_SetA = var_ForthO_NinthO_SetA + pow((ResonanceRatio_ForthO_NinthO_SetA[i] - mean),2.);
				}
			
				//FifthO//
				if(IsSelected_FifthO_SetA[i]){
					mean = (OverallResonance_FifthO_FU_SetA/NumOfFifthO_FU_SetA);
					var_FifthO_FU_SetA = var_FifthO_FU_SetA + pow((ResonanceRatio_FifthO_FU_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_FO_SetA/NumOfFifthO_FO_SetA);
					var_FifthO_FO_SetA = var_FifthO_FO_SetA + pow((ResonanceRatio_FifthO_FO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_2O_SetA/NumOfFifthO_2O_SetA);
					var_FifthO_2O_SetA = var_FifthO_2O_SetA + pow((ResonanceRatio_FifthO_2O_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_ThirdO_SetA/NumOfFifthO_ThirdO_SetA);
					var_FifthO_ThirdO_SetA = var_FifthO_ThirdO_SetA + pow((ResonanceRatio_FifthO_ThirdO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_ForthO_SetA/NumOfFifthO_ForthO_SetA);
					var_FifthO_ForthO_SetA = var_FifthO_ForthO_SetA + pow((ResonanceRatio_FifthO_ForthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_FifthO_SetA/NumOfFifthO_FifthO_SetA);
					var_FifthO_FifthO_SetA = var_FifthO_FifthO_SetA + pow((ResonanceRatio_FifthO_FifthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_SixthO_SetA/NumOfFifthO_SixthO_SetA);
					var_FifthO_SixthO_SetA = var_FifthO_SixthO_SetA + pow((ResonanceRatio_FifthO_SixthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_SeventhO_SetA/NumOfFifthO_SeventhO_SetA);
					var_FifthO_SeventhO_SetA = var_FifthO_SeventhO_SetA + pow((ResonanceRatio_FifthO_SeventhO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_EighthO_SetA/NumOfFifthO_EighthO_SetA);
					var_FifthO_EighthO_SetA = var_FifthO_EighthO_SetA + pow((ResonanceRatio_FifthO_EighthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_NinthO_SetA/NumOfFifthO_NinthO_SetA);
					var_FifthO_NinthO_SetA = var_FifthO_NinthO_SetA + pow((ResonanceRatio_FifthO_NinthO_SetA[i] - mean),2.);
				}
			
				//SixthO//
				if(IsSelected_SixthO_SetA[i]){
					mean = (OverallResonance_SixthO_FU_SetA/NumOfSixthO_FU_SetA);
					var_SixthO_FU_SetA = var_SixthO_FU_SetA + pow((ResonanceRatio_SixthO_FU_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_FO_SetA/NumOfSixthO_FO_SetA);
					var_SixthO_FO_SetA = var_SixthO_FO_SetA + pow((ResonanceRatio_SixthO_FO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_2O_SetA/NumOfSixthO_2O_SetA);
					var_SixthO_2O_SetA = var_SixthO_2O_SetA + pow((ResonanceRatio_SixthO_2O_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_ThirdO_SetA/NumOfSixthO_ThirdO_SetA);
					var_SixthO_ThirdO_SetA = var_SixthO_ThirdO_SetA + pow((ResonanceRatio_SixthO_ThirdO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_ForthO_SetA/NumOfSixthO_ForthO_SetA);
					var_SixthO_ForthO_SetA = var_SixthO_ForthO_SetA + pow((ResonanceRatio_SixthO_ForthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_FifthO_SetA/NumOfSixthO_FifthO_SetA);
					var_SixthO_FifthO_SetA = var_SixthO_FifthO_SetA + pow((ResonanceRatio_SixthO_FifthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_SixthO_SetA/NumOfSixthO_SixthO_SetA);
					var_SixthO_SixthO_SetA = var_SixthO_SixthO_SetA + pow((ResonanceRatio_SixthO_SixthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_SeventhO_SetA/NumOfSixthO_SeventhO_SetA);
					var_SixthO_SeventhO_SetA = var_SixthO_SeventhO_SetA + pow((ResonanceRatio_SixthO_SeventhO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_EighthO_SetA/NumOfSixthO_EighthO_SetA);
					var_SixthO_EighthO_SetA = var_SixthO_EighthO_SetA + pow((ResonanceRatio_SixthO_EighthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_NinthO_SetA/NumOfSixthO_NinthO_SetA);
					var_SixthO_NinthO_SetA = var_SixthO_NinthO_SetA + pow((ResonanceRatio_SixthO_NinthO_SetA[i] - mean),2.);
				}
			
				//SeventhO//
				if(IsSelected_SeventhO_SetA[i]){
					mean = (OverallResonance_SeventhO_FU_SetA/NumOfSeventhO_FU_SetA);
					var_SeventhO_FU_SetA = var_SeventhO_FU_SetA + pow((ResonanceRatio_SeventhO_FU_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_FO_SetA/NumOfSeventhO_FO_SetA);
					var_SeventhO_FO_SetA = var_SeventhO_FO_SetA + pow((ResonanceRatio_SeventhO_FO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_2O_SetA/NumOfSeventhO_2O_SetA);
					var_SeventhO_2O_SetA = var_SeventhO_2O_SetA + pow((ResonanceRatio_SeventhO_2O_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_ThirdO_SetA/NumOfSeventhO_ThirdO_SetA);
					var_SeventhO_ThirdO_SetA = var_SeventhO_ThirdO_SetA + pow((ResonanceRatio_SeventhO_ThirdO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_ForthO_SetA/NumOfSeventhO_ForthO_SetA);
					var_SeventhO_ForthO_SetA = var_SeventhO_ForthO_SetA + pow((ResonanceRatio_SeventhO_ForthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_FifthO_SetA/NumOfSeventhO_FifthO_SetA);
					var_SeventhO_FifthO_SetA = var_SeventhO_FifthO_SetA + pow((ResonanceRatio_SeventhO_FifthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_SixthO_SetA/NumOfSeventhO_SixthO_SetA);
					var_SeventhO_SixthO_SetA = var_SeventhO_SixthO_SetA + pow((ResonanceRatio_SeventhO_SixthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_SeventhO_SetA/NumOfSeventhO_SeventhO_SetA);
					var_SeventhO_SeventhO_SetA = var_SeventhO_SeventhO_SetA + pow((ResonanceRatio_SeventhO_SeventhO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_EighthO_SetA/NumOfSeventhO_EighthO_SetA);
					var_SeventhO_EighthO_SetA = var_SeventhO_EighthO_SetA + pow((ResonanceRatio_SeventhO_EighthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_NinthO_SetA/NumOfSeventhO_NinthO_SetA);
					var_SeventhO_NinthO_SetA = var_SeventhO_NinthO_SetA + pow((ResonanceRatio_SeventhO_NinthO_SetA[i] - mean),2.);
				}
			
				//EighthO//
				if(IsSelected_EighthO_SetA[i]){
					mean = (OverallResonance_EighthO_FU_SetA/NumOfEighthO_FU_SetA);
					var_EighthO_FU_SetA = var_EighthO_FU_SetA + pow((ResonanceRatio_EighthO_FU_SetA[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_FO_SetA/NumOfEighthO_FO_SetA);
					var_EighthO_FO_SetA = var_EighthO_FO_SetA + pow((ResonanceRatio_EighthO_FO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_2O_SetA/NumOfEighthO_2O_SetA);
					var_EighthO_2O_SetA = var_EighthO_2O_SetA + pow((ResonanceRatio_EighthO_2O_SetA[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_ThirdO_SetA/NumOfEighthO_ThirdO_SetA);
					var_EighthO_ThirdO_SetA = var_EighthO_ThirdO_SetA + pow((ResonanceRatio_EighthO_ThirdO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_ForthO_SetA/NumOfEighthO_ForthO_SetA);
					var_EighthO_ForthO_SetA = var_EighthO_ForthO_SetA + pow((ResonanceRatio_EighthO_ForthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_FifthO_SetA/NumOfEighthO_FifthO_SetA);
					var_EighthO_FifthO_SetA = var_EighthO_FifthO_SetA + pow((ResonanceRatio_EighthO_FifthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_SixthO_SetA/NumOfEighthO_SixthO_SetA);
					var_EighthO_SixthO_SetA = var_EighthO_SixthO_SetA + pow((ResonanceRatio_EighthO_SixthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_SeventhO_SetA/NumOfEighthO_SeventhO_SetA);
					var_EighthO_SeventhO_SetA = var_EighthO_SeventhO_SetA + pow((ResonanceRatio_EighthO_SeventhO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_EighthO_SetA/NumOfEighthO_EighthO_SetA);
					var_EighthO_EighthO_SetA = var_EighthO_EighthO_SetA + pow((ResonanceRatio_EighthO_EighthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_NinthO_SetA/NumOfEighthO_NinthO_SetA);
					var_EighthO_NinthO_SetA = var_EighthO_NinthO_SetA + pow((ResonanceRatio_EighthO_NinthO_SetA[i] - mean),2.);
				}
				
				//NinthO//
				if(IsSelected_NinthO_SetA[i]){
					mean = (OverallResonance_NinthO_FU_SetA/NumOfNinthO_FU_SetA);
					var_NinthO_FU_SetA = var_NinthO_FU_SetA + pow((ResonanceRatio_NinthO_FU_SetA[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_FO_SetA/NumOfNinthO_FO_SetA);
					var_NinthO_FO_SetA = var_NinthO_FO_SetA + pow((ResonanceRatio_NinthO_FO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_2O_SetA/NumOfNinthO_2O_SetA);
					var_NinthO_2O_SetA = var_NinthO_2O_SetA + pow((ResonanceRatio_NinthO_2O_SetA[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_ThirdO_SetA/NumOfNinthO_ThirdO_SetA);
					var_NinthO_ThirdO_SetA = var_NinthO_ThirdO_SetA + pow((ResonanceRatio_NinthO_ThirdO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_ForthO_SetA/NumOfNinthO_ForthO_SetA);
					var_NinthO_ForthO_SetA = var_NinthO_ForthO_SetA + pow((ResonanceRatio_NinthO_ForthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_FifthO_SetA/NumOfNinthO_FifthO_SetA);
					var_NinthO_FifthO_SetA = var_NinthO_FifthO_SetA + pow((ResonanceRatio_NinthO_FifthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_SixthO_SetA/NumOfNinthO_SixthO_SetA);
					var_NinthO_SixthO_SetA = var_NinthO_SixthO_SetA + pow((ResonanceRatio_NinthO_SixthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_SeventhO_SetA/NumOfNinthO_SeventhO_SetA);
					var_NinthO_SeventhO_SetA = var_NinthO_SeventhO_SetA + pow((ResonanceRatio_NinthO_SeventhO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_EighthO_SetA/NumOfNinthO_EighthO_SetA);
					var_NinthO_EighthO_SetA = var_NinthO_EighthO_SetA + pow((ResonanceRatio_NinthO_EighthO_SetA[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_NinthO_SetA/NumOfNinthO_NinthO_SetA);
					var_NinthO_NinthO_SetA = var_NinthO_NinthO_SetA + pow((ResonanceRatio_NinthO_NinthO_SetA[i] - mean),2.);
				}
					
			}
			
			////SD Calc Part II (To get final SD)////
			//FU as check mode//
			SD_FU_FU_SetA = sqrt((1./(NumOfFU_FU_SetA - 1.))*var_FU_FU_SetA);
			SD_FU_FO_SetA = sqrt((1./(NumOfFU_FO_SetA - 1.))*var_FU_FO_SetA);
			SD_FU_2O_SetA = sqrt((1./(NumOfFU_2O_SetA - 1.))*var_FU_2O_SetA);
			SD_FU_ThirdO_SetA = sqrt((1./(NumOfFU_ThirdO_SetA - 1.))*var_FU_ThirdO_SetA);
			SD_FU_ForthO_SetA = sqrt((1./(NumOfFU_ForthO_SetA - 1.))*var_FU_ForthO_SetA);
			SD_FU_FifthO_SetA = sqrt((1./(NumOfFU_FifthO_SetA - 1.))*var_FU_FifthO_SetA);
			SD_FU_SixthO_SetA = sqrt((1./(NumOfFU_SixthO_SetA - 1.))*var_FU_SixthO_SetA);
			SD_FU_SeventhO_SetA = sqrt((1./(NumOfFU_SeventhO_SetA - 1.))*var_FU_SeventhO_SetA);
			SD_FU_EighthO_SetA = sqrt((1./(NumOfFU_EighthO_SetA - 1.))*var_FU_EighthO_SetA);
			SD_FU_NinthO_SetA = sqrt((1./(NumOfFU_NinthO_SetA - 1.))*var_FU_NinthO_SetA);
			
			//FO as check mode//
			SD_FO_FU_SetA = sqrt((1./(NumOfFO_FU_SetA - 1.))*var_FO_FU_SetA);
			SD_FO_FO_SetA = sqrt((1./(NumOfFO_FO_SetA - 1.))*var_FO_FO_SetA);
			SD_FO_2O_SetA = sqrt((1./(NumOfFO_2O_SetA - 1.))*var_FO_2O_SetA);
			SD_FO_ThirdO_SetA = sqrt((1./(NumOfFO_ThirdO_SetA - 1.))*var_FO_ThirdO_SetA);
			SD_FO_ForthO_SetA = sqrt((1./(NumOfFO_ForthO_SetA - 1.))*var_FO_ForthO_SetA);
			SD_FO_FifthO_SetA = sqrt((1./(NumOfFO_FifthO_SetA - 1.))*var_FO_FifthO_SetA);
			SD_FO_SixthO_SetA = sqrt((1./(NumOfFO_SixthO_SetA - 1.))*var_FO_SixthO_SetA);
			SD_FO_SeventhO_SetA = sqrt((1./(NumOfFO_SeventhO_SetA - 1.))*var_FO_SeventhO_SetA);
			SD_FO_EighthO_SetA = sqrt((1./(NumOfFO_EighthO_SetA - 1.))*var_FO_EighthO_SetA);
			SD_FO_NinthO_SetA = sqrt((1./(NumOfFO_NinthO_SetA - 1.))*var_FO_NinthO_SetA);
			
			
			//2O as check mode//
			SD_2O_FU_SetA = sqrt((1./(NumOf2O_FU_SetA - 1.))*var_2O_FU_SetA);
			SD_2O_FO_SetA = sqrt((1./(NumOf2O_FO_SetA - 1.))*var_2O_FO_SetA);
			SD_2O_2O_SetA = sqrt((1./(NumOf2O_2O_SetA - 1.))*var_2O_2O_SetA);
			SD_2O_ThirdO_SetA = sqrt((1./(NumOf2O_ThirdO_SetA - 1.))*var_2O_ThirdO_SetA);
			SD_2O_ForthO_SetA = sqrt((1./(NumOf2O_ForthO_SetA - 1.))*var_2O_ForthO_SetA);
			SD_2O_FifthO_SetA = sqrt((1./(NumOf2O_FifthO_SetA - 1.))*var_2O_FifthO_SetA);
			SD_2O_SixthO_SetA = sqrt((1./(NumOf2O_SixthO_SetA - 1.))*var_2O_SixthO_SetA);
			SD_2O_SeventhO_SetA = sqrt((1./(NumOf2O_SeventhO_SetA - 1.))*var_2O_SeventhO_SetA);
			SD_2O_EighthO_SetA = sqrt((1./(NumOf2O_EighthO_SetA - 1.))*var_2O_EighthO_SetA);
			SD_2O_NinthO_SetA = sqrt((1./(NumOf2O_NinthO_SetA - 1.))*var_2O_NinthO_SetA);
			
			//ThirdO as check mode//
			SD_ThirdO_FU_SetA = sqrt((1./(NumOfThirdO_FU_SetA - 1.))*var_ThirdO_FU_SetA);
			SD_ThirdO_FO_SetA = sqrt((1./(NumOfThirdO_FO_SetA - 1.))*var_ThirdO_FO_SetA);
			SD_ThirdO_2O_SetA = sqrt((1./(NumOfThirdO_2O_SetA - 1.))*var_ThirdO_2O_SetA);
			SD_ThirdO_ThirdO_SetA = sqrt((1./(NumOfThirdO_ThirdO_SetA - 1.))*var_ThirdO_ThirdO_SetA);
			SD_ThirdO_ForthO_SetA = sqrt((1./(NumOfThirdO_ForthO_SetA - 1.))*var_ThirdO_ForthO_SetA);
			SD_ThirdO_FifthO_SetA = sqrt((1./(NumOfThirdO_FifthO_SetA - 1.))*var_ThirdO_FifthO_SetA);
			SD_ThirdO_SixthO_SetA = sqrt((1./(NumOfThirdO_SixthO_SetA - 1.))*var_ThirdO_SixthO_SetA);
			SD_ThirdO_SeventhO_SetA = sqrt((1./(NumOfThirdO_SeventhO_SetA - 1.))*var_ThirdO_SeventhO_SetA);
			SD_ThirdO_EighthO_SetA = sqrt((1./(NumOfThirdO_EighthO_SetA - 1.))*var_ThirdO_EighthO_SetA);
			SD_ThirdO_NinthO_SetA = sqrt((1./(NumOfThirdO_NinthO_SetA - 1.))*var_ThirdO_NinthO_SetA);
			
			//ForthO as check mode//
			SD_ForthO_FU_SetA = sqrt((1./(NumOfForthO_FU_SetA - 1.))*var_ForthO_FU_SetA);
			SD_ForthO_FO_SetA = sqrt((1./(NumOfForthO_FO_SetA - 1.))*var_ForthO_FO_SetA);
			SD_ForthO_2O_SetA = sqrt((1./(NumOfForthO_2O_SetA - 1.))*var_ForthO_2O_SetA);
			SD_ForthO_ThirdO_SetA = sqrt((1./(NumOfForthO_ThirdO_SetA - 1.))*var_ForthO_ThirdO_SetA);
			SD_ForthO_ForthO_SetA = sqrt((1./(NumOfForthO_ForthO_SetA - 1.))*var_ForthO_ForthO_SetA);
			SD_ForthO_FifthO_SetA = sqrt((1./(NumOfForthO_FifthO_SetA - 1.))*var_ForthO_FifthO_SetA);
			SD_ForthO_SixthO_SetA = sqrt((1./(NumOfForthO_SixthO_SetA - 1.))*var_ForthO_SixthO_SetA);
			SD_ForthO_SeventhO_SetA = sqrt((1./(NumOfForthO_SeventhO_SetA - 1.))*var_ForthO_SeventhO_SetA);
			SD_ForthO_EighthO_SetA = sqrt((1./(NumOfForthO_EighthO_SetA - 1.))*var_ForthO_EighthO_SetA);
			SD_ForthO_NinthO_SetA = sqrt((1./(NumOfForthO_NinthO_SetA - 1.))*var_ForthO_NinthO_SetA);
			
			//FifthO as check mode//
			SD_FifthO_FU_SetA = sqrt((1./(NumOfFifthO_FU_SetA - 1.))*var_FifthO_FU_SetA);
			SD_FifthO_FO_SetA = sqrt((1./(NumOfFifthO_FO_SetA - 1.))*var_FifthO_FO_SetA);
			SD_FifthO_2O_SetA = sqrt((1./(NumOfFifthO_2O_SetA - 1.))*var_FifthO_2O_SetA);
			SD_FifthO_ThirdO_SetA = sqrt((1./(NumOfFifthO_ThirdO_SetA - 1.))*var_FifthO_ThirdO_SetA);
			SD_FifthO_ForthO_SetA = sqrt((1./(NumOfFifthO_ForthO_SetA - 1.))*var_FifthO_ForthO_SetA);
			SD_FifthO_FifthO_SetA = sqrt((1./(NumOfFifthO_FifthO_SetA - 1.))*var_FifthO_FifthO_SetA);
			SD_FifthO_SixthO_SetA = sqrt((1./(NumOfFifthO_SixthO_SetA - 1.))*var_FifthO_SixthO_SetA);
			SD_FifthO_SeventhO_SetA = sqrt((1./(NumOfFifthO_SeventhO_SetA - 1.))*var_FifthO_SeventhO_SetA);
			SD_FifthO_EighthO_SetA = sqrt((1./(NumOfFifthO_EighthO_SetA - 1.))*var_FifthO_EighthO_SetA);
			SD_FifthO_NinthO_SetA = sqrt((1./(NumOfFifthO_NinthO_SetA - 1.))*var_FifthO_NinthO_SetA);
			
			//SixthO as check mode//
			SD_SixthO_FU_SetA = sqrt((1./(NumOfSixthO_FU_SetA - 1.))*var_SixthO_FU_SetA);
			SD_SixthO_FO_SetA = sqrt((1./(NumOfSixthO_FO_SetA - 1.))*var_SixthO_FO_SetA);
			SD_SixthO_2O_SetA = sqrt((1./(NumOfSixthO_2O_SetA - 1.))*var_SixthO_2O_SetA);
			SD_SixthO_ThirdO_SetA = sqrt((1./(NumOfSixthO_ThirdO_SetA - 1.))*var_SixthO_ThirdO_SetA);
			SD_SixthO_ForthO_SetA = sqrt((1./(NumOfSixthO_ForthO_SetA - 1.))*var_SixthO_ForthO_SetA);
			SD_SixthO_FifthO_SetA = sqrt((1./(NumOfSixthO_FifthO_SetA - 1.))*var_SixthO_FifthO_SetA);
			SD_SixthO_SixthO_SetA = sqrt((1./(NumOfSixthO_SixthO_SetA - 1.))*var_SixthO_SixthO_SetA);
			SD_SixthO_SeventhO_SetA = sqrt((1./(NumOfSixthO_SeventhO_SetA - 1.))*var_SixthO_SeventhO_SetA);
			SD_SixthO_EighthO_SetA = sqrt((1./(NumOfSixthO_EighthO_SetA - 1.))*var_SixthO_EighthO_SetA);
			SD_SixthO_NinthO_SetA = sqrt((1./(NumOfSixthO_NinthO_SetA - 1.))*var_SixthO_NinthO_SetA);
			
			//SeventhO as check mode//
			SD_SeventhO_FU_SetA = sqrt((1./(NumOfSeventhO_FU_SetA - 1.))*var_SeventhO_FU_SetA);
			SD_SeventhO_FO_SetA = sqrt((1./(NumOfSeventhO_FO_SetA - 1.))*var_SeventhO_FO_SetA);
			SD_SeventhO_2O_SetA = sqrt((1./(NumOfSeventhO_2O_SetA - 1.))*var_SeventhO_2O_SetA);
			SD_SeventhO_ThirdO_SetA = sqrt((1./(NumOfSeventhO_ThirdO_SetA - 1.))*var_SeventhO_ThirdO_SetA);
			SD_SeventhO_ForthO_SetA = sqrt((1./(NumOfSeventhO_ForthO_SetA - 1.))*var_SeventhO_ForthO_SetA);
			SD_SeventhO_FifthO_SetA = sqrt((1./(NumOfSeventhO_FifthO_SetA - 1.))*var_SeventhO_FifthO_SetA);
			SD_SeventhO_SixthO_SetA = sqrt((1./(NumOfSeventhO_SixthO_SetA - 1.))*var_SeventhO_SixthO_SetA);
			SD_SeventhO_SeventhO_SetA = sqrt((1./(NumOfSeventhO_SeventhO_SetA - 1.))*var_SeventhO_SeventhO_SetA);
			SD_SeventhO_EighthO_SetA = sqrt((1./(NumOfSeventhO_EighthO_SetA - 1.))*var_SeventhO_EighthO_SetA);
			SD_SeventhO_NinthO_SetA = sqrt((1./(NumOfSeventhO_NinthO_SetA - 1.))*var_SeventhO_NinthO_SetA);
			
			//EighthO as check mode//
			SD_EighthO_FU_SetA = sqrt((1./(NumOfEighthO_FU_SetA - 1.))*var_EighthO_FU_SetA);
			SD_EighthO_FO_SetA = sqrt((1./(NumOfEighthO_FO_SetA - 1.))*var_EighthO_FO_SetA);
			SD_EighthO_2O_SetA = sqrt((1./(NumOfEighthO_2O_SetA - 1.))*var_EighthO_2O_SetA);
			SD_EighthO_ThirdO_SetA = sqrt((1./(NumOfEighthO_ThirdO_SetA - 1.))*var_EighthO_ThirdO_SetA);
			SD_EighthO_ForthO_SetA = sqrt((1./(NumOfEighthO_ForthO_SetA - 1.))*var_EighthO_ForthO_SetA);
			SD_EighthO_FifthO_SetA = sqrt((1./(NumOfEighthO_FifthO_SetA - 1.))*var_EighthO_FifthO_SetA);
			SD_EighthO_SixthO_SetA = sqrt((1./(NumOfEighthO_SixthO_SetA - 1.))*var_EighthO_SixthO_SetA);
			SD_EighthO_SeventhO_SetA = sqrt((1./(NumOfEighthO_SeventhO_SetA - 1.))*var_EighthO_SeventhO_SetA);
			SD_EighthO_EighthO_SetA = sqrt((1./(NumOfEighthO_EighthO_SetA - 1.))*var_EighthO_EighthO_SetA);
			SD_EighthO_NinthO_SetA = sqrt((1./(NumOfEighthO_NinthO_SetA - 1.))*var_EighthO_NinthO_SetA);
			
			//NinthO as check mode//
			SD_NinthO_FU_SetA = sqrt((1./(NumOfNinthO_FU_SetA - 1.))*var_NinthO_FU_SetA);
			SD_NinthO_FO_SetA = sqrt((1./(NumOfNinthO_FO_SetA - 1.))*var_NinthO_FO_SetA);
			SD_NinthO_2O_SetA = sqrt((1./(NumOfNinthO_2O_SetA - 1.))*var_NinthO_2O_SetA);
			SD_NinthO_ThirdO_SetA = sqrt((1./(NumOfNinthO_ThirdO_SetA - 1.))*var_NinthO_ThirdO_SetA);
			SD_NinthO_ForthO_SetA = sqrt((1./(NumOfNinthO_ForthO_SetA - 1.))*var_NinthO_ForthO_SetA);
			SD_NinthO_FifthO_SetA = sqrt((1./(NumOfNinthO_FifthO_SetA - 1.))*var_NinthO_FifthO_SetA);
			SD_NinthO_SixthO_SetA = sqrt((1./(NumOfNinthO_SixthO_SetA - 1.))*var_NinthO_SixthO_SetA);
			SD_NinthO_SeventhO_SetA = sqrt((1./(NumOfNinthO_SeventhO_SetA - 1.))*var_NinthO_SeventhO_SetA);
			SD_NinthO_EighthO_SetA = sqrt((1./(NumOfNinthO_EighthO_SetA - 1.))*var_NinthO_EighthO_SetA);
			SD_NinthO_NinthO_SetA = sqrt((1./(NumOfNinthO_NinthO_SetA - 1.))*var_NinthO_NinthO_SetA);
			
			////Output for Num////
			outFile_Num<<setw(10)<<"FU: "<<setw(20)<<NumOfFU_FU_SetA<<setw(30)<<NumOfFO_FU_SetA<<setw(30)<<NumOf2O_FU_SetA<<setw(30)<<NumOfThirdO_FU_SetA<<setw(30)<<NumOfForthO_FU_SetA<<setw(30)<<NumOfFifthO_FU_SetA<<setw(30)<<NumOfSixthO_FU_SetA<<setw(30)<<NumOfSeventhO_FU_SetA<<setw(30)<<NumOfEighthO_FU_SetA<<setw(30)<<NumOfNinthO_FU_SetA<<endl;
			
			outFile_Num<<setw(10)<<"FO: "<<setw(20)<<NumOfFU_FO_SetA<<setw(30)<<NumOfFO_FO_SetA<<setw(30)<<NumOf2O_FO_SetA<<setw(30)<<NumOfThirdO_FO_SetA<<setw(30)<<NumOfForthO_FO_SetA<<setw(30)<<NumOfFifthO_FO_SetA<<setw(30)<<NumOfSixthO_FO_SetA<<setw(30)<<NumOfSeventhO_FO_SetA<<setw(30)<<NumOfEighthO_FO_SetA<<setw(30)<<NumOfNinthO_FO_SetA<<endl;
			
			outFile_Num<<setw(10)<<"2O: "<<setw(20)<<NumOfFU_2O_SetA<<setw(30)<<NumOfFO_2O_SetA<<setw(30)<<NumOf2O_2O_SetA<<setw(30)<<NumOfThirdO_2O_SetA<<setw(30)<<NumOfForthO_2O_SetA<<setw(30)<<NumOfFifthO_2O_SetA<<setw(30)<<NumOfSixthO_2O_SetA<<setw(30)<<NumOfSeventhO_2O_SetA<<setw(30)<<NumOfEighthO_2O_SetA<<setw(30)<<NumOfNinthO_2O_SetA<<endl;
			
			outFile_Num<<setw(10)<<"ThirdO: "<<setw(20)<<NumOfFU_ThirdO_SetA<<setw(30)<<NumOfFO_ThirdO_SetA<<setw(30)<<NumOf2O_ThirdO_SetA<<setw(30)<<NumOfThirdO_ThirdO_SetA<<setw(30)<<NumOfForthO_ThirdO_SetA<<setw(30)<<NumOfFifthO_ThirdO_SetA<<setw(30)<<NumOfSixthO_ThirdO_SetA<<setw(30)<<NumOfSeventhO_ThirdO_SetA<<setw(30)<<NumOfEighthO_ThirdO_SetA<<setw(30)<<NumOfNinthO_ThirdO_SetA<<endl;
			
			outFile_Num<<setw(10)<<"ForthO: "<<setw(20)<<NumOfFU_ForthO_SetA<<setw(30)<<NumOfFO_ForthO_SetA<<setw(30)<<NumOf2O_ForthO_SetA<<setw(30)<<NumOfThirdO_ForthO_SetA<<setw(30)<<NumOfForthO_ForthO_SetA<<setw(30)<<NumOfFifthO_ForthO_SetA<<setw(30)<<NumOfSixthO_ForthO_SetA<<setw(30)<<NumOfSeventhO_ForthO_SetA<<setw(30)<<NumOfEighthO_ForthO_SetA<<setw(30)<<NumOfNinthO_ForthO_SetA<<endl;
			
			outFile_Num<<setw(10)<<"FifthO: "<<setw(20)<<NumOfFU_FifthO_SetA<<setw(30)<<NumOfFO_FifthO_SetA<<setw(30)<<NumOf2O_FifthO_SetA<<setw(30)<<NumOfThirdO_FifthO_SetA<<setw(30)<<NumOfForthO_FifthO_SetA<<setw(30)<<NumOfFifthO_FifthO_SetA<<setw(30)<<NumOfSixthO_FifthO_SetA<<setw(30)<<NumOfSeventhO_FifthO_SetA<<setw(30)<<NumOfEighthO_FifthO_SetA<<setw(30)<<NumOfNinthO_FifthO_SetA<<endl;
			
			outFile_Num<<setw(10)<<"SixthO: "<<setw(20)<<NumOfFU_SixthO_SetA<<setw(30)<<NumOfFO_SixthO_SetA<<setw(30)<<NumOf2O_SixthO_SetA<<setw(30)<<NumOfThirdO_SixthO_SetA<<setw(30)<<NumOfForthO_SixthO_SetA<<setw(30)<<NumOfFifthO_SixthO_SetA<<setw(30)<<NumOfSixthO_SixthO_SetA<<setw(30)<<NumOfSeventhO_SixthO_SetA<<setw(30)<<NumOfEighthO_SixthO_SetA<<setw(30)<<NumOfNinthO_SixthO_SetA<<endl;
			
			outFile_Num<<setw(10)<<"SeventhO: "<<setw(20)<<NumOfFU_SeventhO_SetA<<setw(30)<<NumOfFO_SeventhO_SetA<<setw(30)<<NumOf2O_SeventhO_SetA<<setw(30)<<NumOfThirdO_SeventhO_SetA<<setw(30)<<NumOfForthO_SeventhO_SetA<<setw(30)<<NumOfFifthO_SeventhO_SetA<<setw(30)<<NumOfSixthO_SeventhO_SetA<<setw(30)<<NumOfSeventhO_SeventhO_SetA<<setw(30)<<NumOfEighthO_SeventhO_SetA<<setw(30)<<NumOfNinthO_SeventhO_SetA<<endl;
			
			outFile_Num<<setw(10)<<"EighthO: "<<setw(20)<<NumOfFU_EighthO_SetA<<setw(30)<<NumOfFO_EighthO_SetA<<setw(30)<<NumOf2O_EighthO_SetA<<setw(30)<<NumOfThirdO_EighthO_SetA<<setw(30)<<NumOfForthO_EighthO_SetA<<setw(30)<<NumOfFifthO_EighthO_SetA<<setw(30)<<NumOfSixthO_EighthO_SetA<<setw(30)<<NumOfSeventhO_EighthO_SetA<<setw(30)<<NumOfEighthO_EighthO_SetA<<setw(30)<<NumOfNinthO_EighthO_SetA<<endl;
			
			outFile_Num<<setw(10)<<"NinthO: "<<setw(20)<<NumOfFU_NinthO_SetA<<setw(30)<<NumOfFO_NinthO_SetA<<setw(30)<<NumOf2O_NinthO_SetA<<setw(30)<<NumOfThirdO_NinthO_SetA<<setw(30)<<NumOfForthO_NinthO_SetA<<setw(30)<<NumOfFifthO_NinthO_SetA<<setw(30)<<NumOfSixthO_NinthO_SetA<<setw(30)<<NumOfSeventhO_NinthO_SetA<<setw(30)<<NumOfEighthO_NinthO_SetA<<setw(30)<<NumOfNinthO_NinthO_SetA<<endl;
			
			
			////Output for SD calcs////
			outFile_SD<<setw(10)<<"FU: "<<setw(20)<<SD_FU_FU_SetA<<setw(30)<<SD_FO_FU_SetA<<setw(30)<<SD_2O_FU_SetA<<setw(30)<<SD_ThirdO_FU_SetA<<setw(30)<<SD_ForthO_FU_SetA<<setw(30)<<SD_FifthO_FU_SetA<<setw(30)<<SD_SixthO_FU_SetA<<setw(30)<<SD_SeventhO_FU_SetA<<setw(30)<<SD_EighthO_FU_SetA<<setw(30)<<SD_NinthO_FU_SetA<<endl;
			
			outFile_SD<<setw(10)<<"FO: "<<setw(20)<<SD_FU_FO_SetA<<setw(30)<<SD_FO_FO_SetA<<setw(30)<<SD_2O_FO_SetA<<setw(30)<<SD_ThirdO_FO_SetA<<setw(30)<<SD_ForthO_FO_SetA<<setw(30)<<SD_FifthO_FO_SetA<<setw(30)<<SD_SixthO_FO_SetA<<setw(30)<<SD_SeventhO_FO_SetA<<setw(30)<<SD_EighthO_FO_SetA<<setw(30)<<SD_NinthO_FO_SetA<<endl;
			
			outFile_SD<<setw(10)<<"2O: "<<setw(20)<<SD_FU_2O_SetA<<setw(30)<<SD_FO_2O_SetA<<setw(30)<<SD_2O_2O_SetA<<setw(30)<<SD_ThirdO_2O_SetA<<setw(30)<<SD_ForthO_2O_SetA<<setw(30)<<SD_FifthO_2O_SetA<<setw(30)<<SD_SixthO_2O_SetA<<setw(30)<<SD_SeventhO_2O_SetA<<setw(30)<<SD_EighthO_2O_SetA<<setw(30)<<SD_NinthO_2O_SetA<<endl;
			
			outFile_SD<<setw(10)<<"ThirdO: "<<setw(20)<<SD_FU_ThirdO_SetA<<setw(30)<<SD_FO_ThirdO_SetA<<setw(30)<<SD_2O_ThirdO_SetA<<setw(30)<<SD_ThirdO_ThirdO_SetA<<setw(30)<<SD_ForthO_ThirdO_SetA<<setw(30)<<SD_FifthO_ThirdO_SetA<<setw(30)<<SD_SixthO_ThirdO_SetA<<setw(30)<<SD_SeventhO_ThirdO_SetA<<setw(30)<<SD_EighthO_ThirdO_SetA<<setw(30)<<SD_NinthO_ThirdO_SetA<<endl;
			
			outFile_SD<<setw(10)<<"ForthO: "<<setw(20)<<SD_FU_ForthO_SetA<<setw(30)<<SD_FO_ForthO_SetA<<setw(30)<<SD_2O_ForthO_SetA<<setw(30)<<SD_ThirdO_ForthO_SetA<<setw(30)<<SD_ForthO_ForthO_SetA<<setw(30)<<SD_FifthO_ForthO_SetA<<setw(30)<<SD_SixthO_ForthO_SetA<<setw(30)<<SD_SeventhO_ForthO_SetA<<setw(30)<<SD_EighthO_ForthO_SetA<<setw(30)<<SD_NinthO_ForthO_SetA<<endl;
			
			outFile_SD<<setw(10)<<"FifthO: "<<setw(20)<<SD_FU_FifthO_SetA<<setw(30)<<SD_FO_FifthO_SetA<<setw(30)<<SD_2O_FifthO_SetA<<setw(30)<<SD_ThirdO_FifthO_SetA<<setw(30)<<SD_ForthO_FifthO_SetA<<setw(30)<<SD_FifthO_FifthO_SetA<<setw(30)<<SD_SixthO_FifthO_SetA<<setw(30)<<SD_SeventhO_FifthO_SetA<<setw(30)<<SD_EighthO_FifthO_SetA<<setw(30)<<SD_NinthO_FifthO_SetA<<endl;
			
			outFile_SD<<setw(10)<<"SixthO: "<<setw(20)<<SD_FU_SixthO_SetA<<setw(30)<<SD_FO_SixthO_SetA<<setw(30)<<SD_2O_SixthO_SetA<<setw(30)<<SD_ThirdO_SixthO_SetA<<setw(30)<<SD_ForthO_SixthO_SetA<<setw(30)<<SD_FifthO_SixthO_SetA<<setw(30)<<SD_SixthO_SixthO_SetA<<setw(30)<<SD_SeventhO_SixthO_SetA<<setw(30)<<SD_EighthO_SixthO_SetA<<setw(30)<<SD_NinthO_SixthO_SetA<<endl;
			
			outFile_SD<<setw(10)<<"SeventhO: "<<setw(20)<<SD_FU_SeventhO_SetA<<setw(30)<<SD_FO_SeventhO_SetA<<setw(30)<<SD_2O_SeventhO_SetA<<setw(30)<<SD_ThirdO_SeventhO_SetA<<setw(30)<<SD_ForthO_SeventhO_SetA<<setw(30)<<SD_FifthO_SeventhO_SetA<<setw(30)<<SD_SixthO_SeventhO_SetA<<setw(30)<<SD_SeventhO_SeventhO_SetA<<setw(30)<<SD_EighthO_SeventhO_SetA<<setw(30)<<SD_NinthO_SeventhO_SetA<<endl;
			
			outFile_SD<<setw(10)<<"EighthO: "<<setw(20)<<SD_FU_EighthO_SetA<<setw(30)<<SD_FO_EighthO_SetA<<setw(30)<<SD_2O_EighthO_SetA<<setw(30)<<SD_ThirdO_EighthO_SetA<<setw(30)<<SD_ForthO_EighthO_SetA<<setw(30)<<SD_FifthO_EighthO_SetA<<setw(30)<<SD_SixthO_EighthO_SetA<<setw(30)<<SD_SeventhO_EighthO_SetA<<setw(30)<<SD_EighthO_EighthO_SetA<<setw(30)<<SD_NinthO_EighthO_SetA<<endl;
			
			outFile_SD<<setw(10)<<"NinthO: "<<setw(20)<<SD_FU_NinthO_SetA<<setw(30)<<SD_FO_NinthO_SetA<<setw(30)<<SD_2O_NinthO_SetA<<setw(30)<<SD_ThirdO_NinthO_SetA<<setw(30)<<SD_ForthO_NinthO_SetA<<setw(30)<<SD_FifthO_NinthO_SetA<<setw(30)<<SD_SixthO_NinthO_SetA<<setw(30)<<SD_SeventhO_NinthO_SetA<<setw(30)<<SD_EighthO_NinthO_SetA<<setw(30)<<SD_NinthO_NinthO_SetA<<endl;
			
			
			////Output for resonance ratios////
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
			
			
			////Output for Fig5////
			for(int i = 0; i < NumOfModels; i++){
				if(Mass[i] == Mass_ToLookFor && L[i] == L_ToLookFor && ModelRan_SetA[i]){
					double frequency_FU = 1./period_FU_SetA[i];
					double frequency_FO = 1./period_FO_SetA[i];
					double frequency_2O = 1./period_2O_SetA[i];
					double frequency_ThirdO = 1./period_ThirdO_SetA[i];
					double frequency_ForthO = 1./period_ForthO_SetA[i];
					outFile_Fig5_FU_FO<<setw(10)<<Teff[i]<<setw(20)<<frequency_FU<<setw(20)<<frequency_FO<<setw(20)<<frequency_2O<<setw(20)<<frequency_ThirdO<<setw(20)<<frequency_ForthO<<setw(20)<<growth_FU_SetA[i]<<setw(20)<<growth_FO_SetA[i]<<setw(20)<<growth_2O_SetA[i]<<setw(20)<<growth_ThirdO_SetA[i]<<setw(20)<<growth_ForthO_SetA[i]<<endl;
				}
				inFile.close();
			}
		}
		if(j == 1){
			////SD Calc Part I (To get var)////
			for(int i = 0; i < NumOfModels; i++){
				double mean;
				//FU as check//
				if(IsSelected_FU_SetB[i]){
					mean = (OverallResonance_FU_FU_SetB/NumOfFU_FU_SetB);
					var_FU_FU_SetB = var_FU_FU_SetB + pow((ResonanceRatio_FU_FU_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FU_FO_SetB/NumOfFU_FO_SetB);
					var_FU_FO_SetB = var_FU_FO_SetB + pow((ResonanceRatio_FU_FO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FU_2O_SetB/NumOfFU_2O_SetB);
					var_FU_2O_SetB = var_FU_2O_SetB + pow((ResonanceRatio_FU_2O_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FU_ThirdO_SetB/NumOfFU_ThirdO_SetB);
					var_FU_ThirdO_SetB = var_FU_ThirdO_SetB + pow((ResonanceRatio_FU_ThirdO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FU_ForthO_SetB/NumOfFU_ForthO_SetB);
					var_FU_ForthO_SetB = var_FU_ForthO_SetB + pow((ResonanceRatio_FU_ForthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FU_FifthO_SetB/NumOfFU_FifthO_SetB);
					var_FU_FifthO_SetB = var_FU_FifthO_SetB + pow((ResonanceRatio_FU_FifthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FU_SixthO_SetB/NumOfFU_SixthO_SetB);
					var_FU_SixthO_SetB = var_FU_SixthO_SetB + pow((ResonanceRatio_FU_SixthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FU_SeventhO_SetB/NumOfFU_SeventhO_SetB);
					var_FU_SeventhO_SetB = var_FU_SeventhO_SetB + pow((ResonanceRatio_FU_SeventhO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FU_EighthO_SetB/NumOfFU_EighthO_SetB);
					var_FU_EighthO_SetB = var_FU_EighthO_SetB + pow((ResonanceRatio_FU_EighthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FU_NinthO_SetB/NumOfFU_NinthO_SetB);
					var_FU_NinthO_SetB = var_FU_NinthO_SetB + pow((ResonanceRatio_FU_NinthO_SetB[i] - mean),2.);
				}
							
				//FO//			
				if(IsSelected_FO_SetB[i]){
					mean = (OverallResonance_FO_FU_SetB/NumOfFO_FU_SetB);
					var_FO_FU_SetB = var_FO_FU_SetB + pow((ResonanceRatio_FO_FU_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FO_FO_SetB/NumOfFO_FO_SetB);
					var_FO_FO_SetB = var_FO_FO_SetB + pow((ResonanceRatio_FO_FO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FO_2O_SetB/NumOfFO_2O_SetB);
					var_FO_2O_SetB = var_FO_2O_SetB + pow((ResonanceRatio_FO_2O_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FO_ThirdO_SetB/NumOfFO_ThirdO_SetB);
					var_FO_ThirdO_SetB = var_FO_ThirdO_SetB + pow((ResonanceRatio_FO_ThirdO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FO_ForthO_SetB/NumOfFO_ForthO_SetB);
					var_FO_ForthO_SetB = var_FO_ForthO_SetB + pow((ResonanceRatio_FO_ForthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FO_FifthO_SetB/NumOfFO_FifthO_SetB);
					var_FO_FifthO_SetB = var_FO_FifthO_SetB + pow((ResonanceRatio_FO_FifthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FO_SixthO_SetB/NumOfFO_SixthO_SetB);
					var_FO_SixthO_SetB = var_FO_SixthO_SetB + pow((ResonanceRatio_FO_SixthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FO_SeventhO_SetB/NumOfFO_SeventhO_SetB);
					var_FO_SeventhO_SetB = var_FO_SeventhO_SetB + pow((ResonanceRatio_FO_SeventhO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FO_EighthO_SetB/NumOfFO_EighthO_SetB);
					var_FO_EighthO_SetB = var_FO_EighthO_SetB + pow((ResonanceRatio_FO_EighthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FO_NinthO_SetB/NumOfFO_NinthO_SetB);
					var_FO_NinthO_SetB = var_FO_NinthO_SetB + pow((ResonanceRatio_FO_NinthO_SetB[i] - mean),2.);
				}
			
				//2O//
				if(IsSelected_2O_SetB[i]){
					mean = (OverallResonance_2O_FU_SetB/NumOf2O_FU_SetB);
					var_2O_FU_SetB = var_2O_FU_SetB + pow((ResonanceRatio_2O_FU_SetB[i] - mean),2.);
					
					mean = (OverallResonance_2O_FO_SetB/NumOf2O_FO_SetB);
					var_2O_FO_SetB = var_2O_FO_SetB + pow((ResonanceRatio_2O_FO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_2O_2O_SetB/NumOf2O_2O_SetB);
					var_2O_2O_SetB = var_2O_2O_SetB + pow((ResonanceRatio_2O_2O_SetB[i] - mean),2.);
					
					mean = (OverallResonance_2O_ThirdO_SetB/NumOf2O_ThirdO_SetB);
					var_2O_ThirdO_SetB = var_2O_ThirdO_SetB + pow((ResonanceRatio_2O_ThirdO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_2O_ForthO_SetB/NumOf2O_ForthO_SetB);
					var_2O_ForthO_SetB = var_2O_ForthO_SetB + pow((ResonanceRatio_2O_ForthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_2O_FifthO_SetB/NumOf2O_FifthO_SetB);
					var_2O_FifthO_SetB = var_2O_FifthO_SetB + pow((ResonanceRatio_2O_FifthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_2O_SixthO_SetB/NumOf2O_SixthO_SetB);
					var_2O_SixthO_SetB = var_2O_SixthO_SetB + pow((ResonanceRatio_2O_SixthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_2O_SeventhO_SetB/NumOf2O_SeventhO_SetB);
					var_2O_SeventhO_SetB = var_2O_SeventhO_SetB + pow((ResonanceRatio_2O_SeventhO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_2O_EighthO_SetB/NumOf2O_EighthO_SetB);
					var_2O_EighthO_SetB = var_2O_EighthO_SetB + pow((ResonanceRatio_2O_EighthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_2O_NinthO_SetB/NumOf2O_NinthO_SetB);
					var_2O_NinthO_SetB = var_2O_NinthO_SetB + pow((ResonanceRatio_2O_NinthO_SetB[i] - mean),2.);
				}
							
				//ThirdO//
				if(IsSelected_ThirdO_SetB[i]){
					mean = (OverallResonance_ThirdO_FU_SetB/NumOfThirdO_FU_SetB);
					var_ThirdO_FU_SetB = var_ThirdO_FU_SetB + pow((ResonanceRatio_ThirdO_FU_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_FO_SetB/NumOfThirdO_FO_SetB);
					var_ThirdO_FO_SetB = var_ThirdO_FO_SetB + pow((ResonanceRatio_ThirdO_FO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_2O_SetB/NumOfThirdO_2O_SetB);
					var_ThirdO_2O_SetB = var_ThirdO_2O_SetB + pow((ResonanceRatio_ThirdO_2O_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_ThirdO_SetB/NumOfThirdO_ThirdO_SetB);
					var_ThirdO_ThirdO_SetB = var_ThirdO_ThirdO_SetB + pow((ResonanceRatio_ThirdO_ThirdO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_ForthO_SetB/NumOfThirdO_ForthO_SetB);
					var_ThirdO_ForthO_SetB = var_ThirdO_ForthO_SetB + pow((ResonanceRatio_ThirdO_ForthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_FifthO_SetB/NumOfThirdO_FifthO_SetB);
					var_ThirdO_FifthO_SetB = var_ThirdO_FifthO_SetB + pow((ResonanceRatio_ThirdO_FifthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_SixthO_SetB/NumOfThirdO_SixthO_SetB);
					var_ThirdO_SixthO_SetB = var_ThirdO_SixthO_SetB + pow((ResonanceRatio_ThirdO_SixthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_SeventhO_SetB/NumOfThirdO_SeventhO_SetB);
					var_ThirdO_SeventhO_SetB = var_ThirdO_SeventhO_SetB + pow((ResonanceRatio_ThirdO_SeventhO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_EighthO_SetB/NumOfThirdO_EighthO_SetB);
					var_ThirdO_EighthO_SetB = var_ThirdO_EighthO_SetB + pow((ResonanceRatio_ThirdO_EighthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_NinthO_SetB/NumOfThirdO_NinthO_SetB);
					var_ThirdO_NinthO_SetB = var_ThirdO_NinthO_SetB + pow((ResonanceRatio_ThirdO_NinthO_SetB[i] - mean),2.);
				}
			
				//ForthO//
				if(IsSelected_ForthO_SetB[i]){
					mean = (OverallResonance_ForthO_FU_SetB/NumOfForthO_FU_SetB);
					var_ForthO_FU_SetB = var_ForthO_FU_SetB + pow((ResonanceRatio_ForthO_FU_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_FO_SetB/NumOfForthO_FO_SetB);
					var_ForthO_FO_SetB = var_ForthO_FO_SetB + pow((ResonanceRatio_ForthO_FO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_2O_SetB/NumOfForthO_2O_SetB);
					var_ForthO_2O_SetB = var_ForthO_2O_SetB + pow((ResonanceRatio_ForthO_2O_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_ThirdO_SetB/NumOfForthO_ThirdO_SetB);
					var_ForthO_ThirdO_SetB = var_ForthO_ThirdO_SetB + pow((ResonanceRatio_ForthO_ThirdO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_ForthO_SetB/NumOfForthO_ForthO_SetB);
					var_ForthO_ForthO_SetB = var_ForthO_ForthO_SetB + pow((ResonanceRatio_ForthO_ForthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_FifthO_SetB/NumOfForthO_FifthO_SetB);
					var_ForthO_FifthO_SetB = var_ForthO_FifthO_SetB + pow((ResonanceRatio_ForthO_FifthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_SixthO_SetB/NumOfForthO_SixthO_SetB);
					var_ForthO_SixthO_SetB = var_ForthO_SixthO_SetB + pow((ResonanceRatio_ForthO_SixthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_SeventhO_SetB/NumOfForthO_SeventhO_SetB);
					var_ForthO_SeventhO_SetB = var_ForthO_SeventhO_SetB + pow((ResonanceRatio_ForthO_SeventhO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_EighthO_SetB/NumOfForthO_EighthO_SetB);
					var_ForthO_EighthO_SetB = var_ForthO_EighthO_SetB + pow((ResonanceRatio_ForthO_EighthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_NinthO_SetB/NumOfForthO_NinthO_SetB);
					var_ForthO_NinthO_SetB = var_ForthO_NinthO_SetB + pow((ResonanceRatio_ForthO_NinthO_SetB[i] - mean),2.);
				}
				
				//FifthO//
				if(IsSelected_FifthO_SetB[i]){
					mean = (OverallResonance_FifthO_FU_SetB/NumOfFifthO_FU_SetB);
					var_FifthO_FU_SetB = var_FifthO_FU_SetB + pow((ResonanceRatio_FifthO_FU_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_FO_SetB/NumOfFifthO_FO_SetB);
					var_FifthO_FO_SetB = var_FifthO_FO_SetB + pow((ResonanceRatio_FifthO_FO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_2O_SetB/NumOfFifthO_2O_SetB);
					var_FifthO_2O_SetB = var_FifthO_2O_SetB + pow((ResonanceRatio_FifthO_2O_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_ThirdO_SetB/NumOfFifthO_ThirdO_SetB);
					var_FifthO_ThirdO_SetB = var_FifthO_ThirdO_SetB + pow((ResonanceRatio_FifthO_ThirdO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_ForthO_SetB/NumOfFifthO_ForthO_SetB);
					var_FifthO_ForthO_SetB = var_FifthO_ForthO_SetB + pow((ResonanceRatio_FifthO_ForthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_FifthO_SetB/NumOfFifthO_FifthO_SetB);
					var_FifthO_FifthO_SetB = var_FifthO_FifthO_SetB + pow((ResonanceRatio_FifthO_FifthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_SixthO_SetB/NumOfFifthO_SixthO_SetB);
					var_FifthO_SixthO_SetB = var_FifthO_SixthO_SetB + pow((ResonanceRatio_FifthO_SixthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_SeventhO_SetB/NumOfFifthO_SeventhO_SetB);
					var_FifthO_SeventhO_SetB = var_FifthO_SeventhO_SetB + pow((ResonanceRatio_FifthO_SeventhO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_EighthO_SetB/NumOfFifthO_EighthO_SetB);
					var_FifthO_EighthO_SetB = var_FifthO_EighthO_SetB + pow((ResonanceRatio_FifthO_EighthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_NinthO_SetB/NumOfFifthO_NinthO_SetB);
					var_FifthO_NinthO_SetB = var_FifthO_NinthO_SetB + pow((ResonanceRatio_FifthO_NinthO_SetB[i] - mean),2.);
				}
			
				//SixthO//
				if(IsSelected_SixthO_SetB[i]){
					mean = (OverallResonance_SixthO_FU_SetB/NumOfSixthO_FU_SetB);
					var_SixthO_FU_SetB = var_SixthO_FU_SetB + pow((ResonanceRatio_SixthO_FU_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_FO_SetB/NumOfSixthO_FO_SetB);
					var_SixthO_FO_SetB = var_SixthO_FO_SetB + pow((ResonanceRatio_SixthO_FO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_2O_SetB/NumOfSixthO_2O_SetB);
					var_SixthO_2O_SetB = var_SixthO_2O_SetB + pow((ResonanceRatio_SixthO_2O_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_ThirdO_SetB/NumOfSixthO_ThirdO_SetB);
					var_SixthO_ThirdO_SetB = var_SixthO_ThirdO_SetB + pow((ResonanceRatio_SixthO_ThirdO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_ForthO_SetB/NumOfSixthO_ForthO_SetB);
					var_SixthO_ForthO_SetB = var_SixthO_ForthO_SetB + pow((ResonanceRatio_SixthO_ForthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_FifthO_SetB/NumOfSixthO_FifthO_SetB);
					var_SixthO_FifthO_SetB = var_SixthO_FifthO_SetB + pow((ResonanceRatio_SixthO_FifthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_SixthO_SetB/NumOfSixthO_SixthO_SetB);
					var_SixthO_SixthO_SetB = var_SixthO_SixthO_SetB + pow((ResonanceRatio_SixthO_SixthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_SeventhO_SetB/NumOfSixthO_SeventhO_SetB);
					var_SixthO_SeventhO_SetB = var_SixthO_SeventhO_SetB + pow((ResonanceRatio_SixthO_SeventhO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_EighthO_SetB/NumOfSixthO_EighthO_SetB);
					var_SixthO_EighthO_SetB = var_SixthO_EighthO_SetB + pow((ResonanceRatio_SixthO_EighthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_NinthO_SetB/NumOfSixthO_NinthO_SetB);
					var_SixthO_NinthO_SetB = var_SixthO_NinthO_SetB + pow((ResonanceRatio_SixthO_NinthO_SetB[i] - mean),2.);
				}
			
				//SeventhO//
				if(IsSelected_SeventhO_SetB[i]){
					mean = (OverallResonance_SeventhO_FU_SetB/NumOfSeventhO_FU_SetB);
					var_SeventhO_FU_SetB = var_SeventhO_FU_SetB + pow((ResonanceRatio_SeventhO_FU_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_FO_SetB/NumOfSeventhO_FO_SetB);
					var_SeventhO_FO_SetB = var_SeventhO_FO_SetB + pow((ResonanceRatio_SeventhO_FO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_2O_SetB/NumOfSeventhO_2O_SetB);
					var_SeventhO_2O_SetB = var_SeventhO_2O_SetB + pow((ResonanceRatio_SeventhO_2O_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_ThirdO_SetB/NumOfSeventhO_ThirdO_SetB);
					var_SeventhO_ThirdO_SetB = var_SeventhO_ThirdO_SetB + pow((ResonanceRatio_SeventhO_ThirdO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_ForthO_SetB/NumOfSeventhO_ForthO_SetB);
					var_SeventhO_ForthO_SetB = var_SeventhO_ForthO_SetB + pow((ResonanceRatio_SeventhO_ForthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_FifthO_SetB/NumOfSeventhO_FifthO_SetB);
					var_SeventhO_FifthO_SetB = var_SeventhO_FifthO_SetB + pow((ResonanceRatio_SeventhO_FifthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_SixthO_SetB/NumOfSeventhO_SixthO_SetB);
					var_SeventhO_SixthO_SetB = var_SeventhO_SixthO_SetB + pow((ResonanceRatio_SeventhO_SixthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_SeventhO_SetB/NumOfSeventhO_SeventhO_SetB);
					var_SeventhO_SeventhO_SetB = var_SeventhO_SeventhO_SetB + pow((ResonanceRatio_SeventhO_SeventhO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_EighthO_SetB/NumOfSeventhO_EighthO_SetB);
					var_SeventhO_EighthO_SetB = var_SeventhO_EighthO_SetB + pow((ResonanceRatio_SeventhO_EighthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_NinthO_SetB/NumOfSeventhO_NinthO_SetB);
					var_SeventhO_NinthO_SetB = var_SeventhO_NinthO_SetB + pow((ResonanceRatio_SeventhO_NinthO_SetB[i] - mean),2.);
				}
			
				//EighthO//
				if(IsSelected_EighthO_SetB[i]){
					mean = (OverallResonance_EighthO_FU_SetB/NumOfEighthO_FU_SetB);
					var_EighthO_FU_SetB = var_EighthO_FU_SetB + pow((ResonanceRatio_EighthO_FU_SetB[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_FO_SetB/NumOfEighthO_FO_SetB);
					var_EighthO_FO_SetB = var_EighthO_FO_SetB + pow((ResonanceRatio_EighthO_FO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_2O_SetB/NumOfEighthO_2O_SetB);
					var_EighthO_2O_SetB = var_EighthO_2O_SetB + pow((ResonanceRatio_EighthO_2O_SetB[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_ThirdO_SetB/NumOfEighthO_ThirdO_SetB);
					var_EighthO_ThirdO_SetB = var_EighthO_ThirdO_SetB + pow((ResonanceRatio_EighthO_ThirdO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_ForthO_SetB/NumOfEighthO_ForthO_SetB);
					var_EighthO_ForthO_SetB = var_EighthO_ForthO_SetB + pow((ResonanceRatio_EighthO_ForthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_FifthO_SetB/NumOfEighthO_FifthO_SetB);
					var_EighthO_FifthO_SetB = var_EighthO_FifthO_SetB + pow((ResonanceRatio_EighthO_FifthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_SixthO_SetB/NumOfEighthO_SixthO_SetB);
					var_EighthO_SixthO_SetB = var_EighthO_SixthO_SetB + pow((ResonanceRatio_EighthO_SixthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_SeventhO_SetB/NumOfEighthO_SeventhO_SetB);
					var_EighthO_SeventhO_SetB = var_EighthO_SeventhO_SetB + pow((ResonanceRatio_EighthO_SeventhO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_EighthO_SetB/NumOfEighthO_EighthO_SetB);
					var_EighthO_EighthO_SetB = var_EighthO_EighthO_SetB + pow((ResonanceRatio_EighthO_EighthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_NinthO_SetB/NumOfEighthO_NinthO_SetB);
					var_EighthO_NinthO_SetB = var_EighthO_NinthO_SetB + pow((ResonanceRatio_EighthO_NinthO_SetB[i] - mean),2.);
				}
				
				//NinthO//
				if(IsSelected_NinthO_SetB[i]){
					mean = (OverallResonance_NinthO_FU_SetB/NumOfNinthO_FU_SetB);
					var_NinthO_FU_SetB = var_NinthO_FU_SetB + pow((ResonanceRatio_NinthO_FU_SetB[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_FO_SetB/NumOfNinthO_FO_SetB);
					var_NinthO_FO_SetB = var_NinthO_FO_SetB + pow((ResonanceRatio_NinthO_FO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_2O_SetB/NumOfNinthO_2O_SetB);
					var_NinthO_2O_SetB = var_NinthO_2O_SetB + pow((ResonanceRatio_NinthO_2O_SetB[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_ThirdO_SetB/NumOfNinthO_ThirdO_SetB);
					var_NinthO_ThirdO_SetB = var_NinthO_ThirdO_SetB + pow((ResonanceRatio_NinthO_ThirdO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_ForthO_SetB/NumOfNinthO_ForthO_SetB);
					var_NinthO_ForthO_SetB = var_NinthO_ForthO_SetB + pow((ResonanceRatio_NinthO_ForthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_FifthO_SetB/NumOfNinthO_FifthO_SetB);
					var_NinthO_FifthO_SetB = var_NinthO_FifthO_SetB + pow((ResonanceRatio_NinthO_FifthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_SixthO_SetB/NumOfNinthO_SixthO_SetB);
					var_NinthO_SixthO_SetB = var_NinthO_SixthO_SetB + pow((ResonanceRatio_NinthO_SixthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_SeventhO_SetB/NumOfNinthO_SeventhO_SetB);
					var_NinthO_SeventhO_SetB = var_NinthO_SeventhO_SetB + pow((ResonanceRatio_NinthO_SeventhO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_EighthO_SetB/NumOfNinthO_EighthO_SetB);
					var_NinthO_EighthO_SetB = var_NinthO_EighthO_SetB + pow((ResonanceRatio_NinthO_EighthO_SetB[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_NinthO_SetB/NumOfNinthO_NinthO_SetB);
					var_NinthO_NinthO_SetB = var_NinthO_NinthO_SetB + pow((ResonanceRatio_NinthO_NinthO_SetB[i] - mean),2.);
				}
			}
			
			////SD Calc Part II (To get final SD)////
			//FU as check mode//
			SD_FU_FU_SetB = sqrt((1./(NumOfFU_FU_SetB - 1.))*var_FU_FU_SetB);
			SD_FU_FO_SetB = sqrt((1./(NumOfFU_FO_SetB - 1.))*var_FU_FO_SetB);
			SD_FU_2O_SetB = sqrt((1./(NumOfFU_2O_SetB - 1.))*var_FU_2O_SetB);
			SD_FU_ThirdO_SetB = sqrt((1./(NumOfFU_ThirdO_SetB - 1.))*var_FU_ThirdO_SetB);
			SD_FU_ForthO_SetB = sqrt((1./(NumOfFU_ForthO_SetB - 1.))*var_FU_ForthO_SetB);
			SD_FU_FifthO_SetB = sqrt((1./(NumOfFU_FifthO_SetB - 1.))*var_FU_FifthO_SetB);
			SD_FU_SixthO_SetB = sqrt((1./(NumOfFU_SixthO_SetB - 1.))*var_FU_SixthO_SetB);
			SD_FU_SeventhO_SetB = sqrt((1./(NumOfFU_SeventhO_SetB - 1.))*var_FU_SeventhO_SetB);
			SD_FU_EighthO_SetB = sqrt((1./(NumOfFU_EighthO_SetB - 1.))*var_FU_EighthO_SetB);
			SD_FU_NinthO_SetB = sqrt((1./(NumOfFU_NinthO_SetB - 1.))*var_FU_NinthO_SetB);
			
			//FO as check mode//
			SD_FO_FU_SetB = sqrt((1./(NumOfFO_FU_SetB - 1.))*var_FO_FU_SetB);
			SD_FO_FO_SetB = sqrt((1./(NumOfFO_FO_SetB - 1.))*var_FO_FO_SetB);
			SD_FO_2O_SetB = sqrt((1./(NumOfFO_2O_SetB - 1.))*var_FO_2O_SetB);
			SD_FO_ThirdO_SetB = sqrt((1./(NumOfFO_ThirdO_SetB - 1.))*var_FO_ThirdO_SetB);
			SD_FO_ForthO_SetB = sqrt((1./(NumOfFO_ForthO_SetB - 1.))*var_FO_ForthO_SetB);
			SD_FO_FifthO_SetB = sqrt((1./(NumOfFO_FifthO_SetB - 1.))*var_FO_FifthO_SetB);
			SD_FO_SixthO_SetB = sqrt((1./(NumOfFO_SixthO_SetB - 1.))*var_FO_SixthO_SetB);
			SD_FO_SeventhO_SetB = sqrt((1./(NumOfFO_SeventhO_SetB - 1.))*var_FO_SeventhO_SetB);
			SD_FO_EighthO_SetB = sqrt((1./(NumOfFO_EighthO_SetB - 1.))*var_FO_EighthO_SetB);
			SD_FO_NinthO_SetB = sqrt((1./(NumOfFO_NinthO_SetB - 1.))*var_FO_NinthO_SetB);
			
			
			//2O as check mode//
			SD_2O_FU_SetB = sqrt((1./(NumOf2O_FU_SetB - 1.))*var_2O_FU_SetB);
			SD_2O_FO_SetB = sqrt((1./(NumOf2O_FO_SetB - 1.))*var_2O_FO_SetB);
			SD_2O_2O_SetB = sqrt((1./(NumOf2O_2O_SetB - 1.))*var_2O_2O_SetB);
			SD_2O_ThirdO_SetB = sqrt((1./(NumOf2O_ThirdO_SetB - 1.))*var_2O_ThirdO_SetB);
			SD_2O_ForthO_SetB = sqrt((1./(NumOf2O_ForthO_SetB - 1.))*var_2O_ForthO_SetB);
			SD_2O_FifthO_SetB = sqrt((1./(NumOf2O_FifthO_SetB - 1.))*var_2O_FifthO_SetB);
			SD_2O_SixthO_SetB = sqrt((1./(NumOf2O_SixthO_SetB - 1.))*var_2O_SixthO_SetB);
			SD_2O_SeventhO_SetB = sqrt((1./(NumOf2O_SeventhO_SetB - 1.))*var_2O_SeventhO_SetB);
			SD_2O_EighthO_SetB = sqrt((1./(NumOf2O_EighthO_SetB - 1.))*var_2O_EighthO_SetB);
			SD_2O_NinthO_SetB = sqrt((1./(NumOf2O_NinthO_SetB - 1.))*var_2O_NinthO_SetB);
			
			//ThirdO as check mode//
			SD_ThirdO_FU_SetB = sqrt((1./(NumOfThirdO_FU_SetB - 1.))*var_ThirdO_FU_SetB);
			SD_ThirdO_FO_SetB = sqrt((1./(NumOfThirdO_FO_SetB - 1.))*var_ThirdO_FO_SetB);
			SD_ThirdO_2O_SetB = sqrt((1./(NumOfThirdO_2O_SetB - 1.))*var_ThirdO_2O_SetB);
			SD_ThirdO_ThirdO_SetB = sqrt((1./(NumOfThirdO_ThirdO_SetB - 1.))*var_ThirdO_ThirdO_SetB);
			SD_ThirdO_ForthO_SetB = sqrt((1./(NumOfThirdO_ForthO_SetB - 1.))*var_ThirdO_ForthO_SetB);
			SD_ThirdO_FifthO_SetB = sqrt((1./(NumOfThirdO_FifthO_SetB - 1.))*var_ThirdO_FifthO_SetB);
			SD_ThirdO_SixthO_SetB = sqrt((1./(NumOfThirdO_SixthO_SetB - 1.))*var_ThirdO_SixthO_SetB);
			SD_ThirdO_SeventhO_SetB = sqrt((1./(NumOfThirdO_SeventhO_SetB - 1.))*var_ThirdO_SeventhO_SetB);
			SD_ThirdO_EighthO_SetB = sqrt((1./(NumOfThirdO_EighthO_SetB - 1.))*var_ThirdO_EighthO_SetB);
			SD_ThirdO_NinthO_SetB = sqrt((1./(NumOfThirdO_NinthO_SetB - 1.))*var_ThirdO_NinthO_SetB);
			
			//ForthO as check mode//
			SD_ForthO_FU_SetB = sqrt((1./(NumOfForthO_FU_SetB - 1.))*var_ForthO_FU_SetB);
			SD_ForthO_FO_SetB = sqrt((1./(NumOfForthO_FO_SetB - 1.))*var_ForthO_FO_SetB);
			SD_ForthO_2O_SetB = sqrt((1./(NumOfForthO_2O_SetB - 1.))*var_ForthO_2O_SetB);
			SD_ForthO_ThirdO_SetB = sqrt((1./(NumOfForthO_ThirdO_SetB - 1.))*var_ForthO_ThirdO_SetB);
			SD_ForthO_ForthO_SetB = sqrt((1./(NumOfForthO_ForthO_SetB - 1.))*var_ForthO_ForthO_SetB);
			SD_ForthO_FifthO_SetB = sqrt((1./(NumOfForthO_FifthO_SetB - 1.))*var_ForthO_FifthO_SetB);
			SD_ForthO_SixthO_SetB = sqrt((1./(NumOfForthO_SixthO_SetB - 1.))*var_ForthO_SixthO_SetB);
			SD_ForthO_SeventhO_SetB = sqrt((1./(NumOfForthO_SeventhO_SetB - 1.))*var_ForthO_SeventhO_SetB);
			SD_ForthO_EighthO_SetB = sqrt((1./(NumOfForthO_EighthO_SetB - 1.))*var_ForthO_EighthO_SetB);
			SD_ForthO_NinthO_SetB = sqrt((1./(NumOfForthO_NinthO_SetB - 1.))*var_ForthO_NinthO_SetB);
			
			//FifthO as check mode//
			SD_FifthO_FU_SetB = sqrt((1./(NumOfFifthO_FU_SetB - 1.))*var_FifthO_FU_SetB);
			SD_FifthO_FO_SetB = sqrt((1./(NumOfFifthO_FO_SetB - 1.))*var_FifthO_FO_SetB);
			SD_FifthO_2O_SetB = sqrt((1./(NumOfFifthO_2O_SetB - 1.))*var_FifthO_2O_SetB);
			SD_FifthO_ThirdO_SetB = sqrt((1./(NumOfFifthO_ThirdO_SetB - 1.))*var_FifthO_ThirdO_SetB);
			SD_FifthO_ForthO_SetB = sqrt((1./(NumOfFifthO_ForthO_SetB - 1.))*var_FifthO_ForthO_SetB);
			SD_FifthO_FifthO_SetB = sqrt((1./(NumOfFifthO_FifthO_SetB - 1.))*var_FifthO_FifthO_SetB);
			SD_FifthO_SixthO_SetB = sqrt((1./(NumOfFifthO_SixthO_SetB - 1.))*var_FifthO_SixthO_SetB);
			SD_FifthO_SeventhO_SetB = sqrt((1./(NumOfFifthO_SeventhO_SetB - 1.))*var_FifthO_SeventhO_SetB);
			SD_FifthO_EighthO_SetB = sqrt((1./(NumOfFifthO_EighthO_SetB - 1.))*var_FifthO_EighthO_SetB);
			SD_FifthO_NinthO_SetB = sqrt((1./(NumOfFifthO_NinthO_SetB - 1.))*var_FifthO_NinthO_SetB);
			
			//SixthO as check mode//
			SD_SixthO_FU_SetB = sqrt((1./(NumOfSixthO_FU_SetB - 1.))*var_SixthO_FU_SetB);
			SD_SixthO_FO_SetB = sqrt((1./(NumOfSixthO_FO_SetB - 1.))*var_SixthO_FO_SetB);
			SD_SixthO_2O_SetB = sqrt((1./(NumOfSixthO_2O_SetB - 1.))*var_SixthO_2O_SetB);
			SD_SixthO_ThirdO_SetB = sqrt((1./(NumOfSixthO_ThirdO_SetB - 1.))*var_SixthO_ThirdO_SetB);
			SD_SixthO_ForthO_SetB = sqrt((1./(NumOfSixthO_ForthO_SetB - 1.))*var_SixthO_ForthO_SetB);
			SD_SixthO_FifthO_SetB = sqrt((1./(NumOfSixthO_FifthO_SetB - 1.))*var_SixthO_FifthO_SetB);
			SD_SixthO_SixthO_SetB = sqrt((1./(NumOfSixthO_SixthO_SetB - 1.))*var_SixthO_SixthO_SetB);
			SD_SixthO_SeventhO_SetB = sqrt((1./(NumOfSixthO_SeventhO_SetB - 1.))*var_SixthO_SeventhO_SetB);
			SD_SixthO_EighthO_SetB = sqrt((1./(NumOfSixthO_EighthO_SetB - 1.))*var_SixthO_EighthO_SetB);
			SD_SixthO_NinthO_SetB = sqrt((1./(NumOfSixthO_NinthO_SetB - 1.))*var_SixthO_NinthO_SetB);
			
			//SeventhO as check mode//
			SD_SeventhO_FU_SetB = sqrt((1./(NumOfSeventhO_FU_SetB - 1.))*var_SeventhO_FU_SetB);
			SD_SeventhO_FO_SetB = sqrt((1./(NumOfSeventhO_FO_SetB - 1.))*var_SeventhO_FO_SetB);
			SD_SeventhO_2O_SetB = sqrt((1./(NumOfSeventhO_2O_SetB - 1.))*var_SeventhO_2O_SetB);
			SD_SeventhO_ThirdO_SetB = sqrt((1./(NumOfSeventhO_ThirdO_SetB - 1.))*var_SeventhO_ThirdO_SetB);
			SD_SeventhO_ForthO_SetB = sqrt((1./(NumOfSeventhO_ForthO_SetB - 1.))*var_SeventhO_ForthO_SetB);
			SD_SeventhO_FifthO_SetB = sqrt((1./(NumOfSeventhO_FifthO_SetB - 1.))*var_SeventhO_FifthO_SetB);
			SD_SeventhO_SixthO_SetB = sqrt((1./(NumOfSeventhO_SixthO_SetB - 1.))*var_SeventhO_SixthO_SetB);
			SD_SeventhO_SeventhO_SetB = sqrt((1./(NumOfSeventhO_SeventhO_SetB - 1.))*var_SeventhO_SeventhO_SetB);
			SD_SeventhO_EighthO_SetB = sqrt((1./(NumOfSeventhO_EighthO_SetB - 1.))*var_SeventhO_EighthO_SetB);
			SD_SeventhO_NinthO_SetB = sqrt((1./(NumOfSeventhO_NinthO_SetB - 1.))*var_SeventhO_NinthO_SetB);
			
			//EighthO as check mode//
			SD_EighthO_FU_SetB = sqrt((1./(NumOfEighthO_FU_SetB - 1.))*var_EighthO_FU_SetB);
			SD_EighthO_FO_SetB = sqrt((1./(NumOfEighthO_FO_SetB - 1.))*var_EighthO_FO_SetB);
			SD_EighthO_2O_SetB = sqrt((1./(NumOfEighthO_2O_SetB - 1.))*var_EighthO_2O_SetB);
			SD_EighthO_ThirdO_SetB = sqrt((1./(NumOfEighthO_ThirdO_SetB - 1.))*var_EighthO_ThirdO_SetB);
			SD_EighthO_ForthO_SetB = sqrt((1./(NumOfEighthO_ForthO_SetB - 1.))*var_EighthO_ForthO_SetB);
			SD_EighthO_FifthO_SetB = sqrt((1./(NumOfEighthO_FifthO_SetB - 1.))*var_EighthO_FifthO_SetB);
			SD_EighthO_SixthO_SetB = sqrt((1./(NumOfEighthO_SixthO_SetB - 1.))*var_EighthO_SixthO_SetB);
			SD_EighthO_SeventhO_SetB = sqrt((1./(NumOfEighthO_SeventhO_SetB - 1.))*var_EighthO_SeventhO_SetB);
			SD_EighthO_EighthO_SetB = sqrt((1./(NumOfEighthO_EighthO_SetB - 1.))*var_EighthO_EighthO_SetB);
			SD_EighthO_NinthO_SetB = sqrt((1./(NumOfEighthO_NinthO_SetB - 1.))*var_EighthO_NinthO_SetB);
			
			//NinthO as check mode//
			SD_NinthO_FU_SetB = sqrt((1./(NumOfNinthO_FU_SetB - 1.))*var_NinthO_FU_SetB);
			SD_NinthO_FO_SetB = sqrt((1./(NumOfNinthO_FO_SetB - 1.))*var_NinthO_FO_SetB);
			SD_NinthO_2O_SetB = sqrt((1./(NumOfNinthO_2O_SetB - 1.))*var_NinthO_2O_SetB);
			SD_NinthO_ThirdO_SetB = sqrt((1./(NumOfNinthO_ThirdO_SetB - 1.))*var_NinthO_ThirdO_SetB);
			SD_NinthO_ForthO_SetB = sqrt((1./(NumOfNinthO_ForthO_SetB - 1.))*var_NinthO_ForthO_SetB);
			SD_NinthO_FifthO_SetB = sqrt((1./(NumOfNinthO_FifthO_SetB - 1.))*var_NinthO_FifthO_SetB);
			SD_NinthO_SixthO_SetB = sqrt((1./(NumOfNinthO_SixthO_SetB - 1.))*var_NinthO_SixthO_SetB);
			SD_NinthO_SeventhO_SetB = sqrt((1./(NumOfNinthO_SeventhO_SetB - 1.))*var_NinthO_SeventhO_SetB);
			SD_NinthO_EighthO_SetB = sqrt((1./(NumOfNinthO_EighthO_SetB - 1.))*var_NinthO_EighthO_SetB);
			SD_NinthO_NinthO_SetB = sqrt((1./(NumOfNinthO_NinthO_SetB - 1.))*var_NinthO_NinthO_SetB);
			
			////Output for Num////
			outFile_Num<<setw(10)<<"FU: "<<setw(20)<<NumOfFU_FU_SetB<<setw(30)<<NumOfFO_FU_SetB<<setw(30)<<NumOf2O_FU_SetB<<setw(30)<<NumOfThirdO_FU_SetB<<setw(30)<<NumOfForthO_FU_SetB<<setw(30)<<NumOfFifthO_FU_SetB<<setw(30)<<NumOfSixthO_FU_SetB<<setw(30)<<NumOfSeventhO_FU_SetB<<setw(30)<<NumOfEighthO_FU_SetB<<setw(30)<<NumOfNinthO_FU_SetB<<endl;
			
			outFile_Num<<setw(10)<<"FO: "<<setw(20)<<NumOfFU_FO_SetB<<setw(30)<<NumOfFO_FO_SetB<<setw(30)<<NumOf2O_FO_SetB<<setw(30)<<NumOfThirdO_FO_SetB<<setw(30)<<NumOfForthO_FO_SetB<<setw(30)<<NumOfFifthO_FO_SetB<<setw(30)<<NumOfSixthO_FO_SetB<<setw(30)<<NumOfSeventhO_FO_SetB<<setw(30)<<NumOfEighthO_FO_SetB<<setw(30)<<NumOfNinthO_FO_SetB<<endl;
			
			outFile_Num<<setw(10)<<"2O: "<<setw(20)<<NumOfFU_2O_SetB<<setw(30)<<NumOfFO_2O_SetB<<setw(30)<<NumOf2O_2O_SetB<<setw(30)<<NumOfThirdO_2O_SetB<<setw(30)<<NumOfForthO_2O_SetB<<setw(30)<<NumOfFifthO_2O_SetB<<setw(30)<<NumOfSixthO_2O_SetB<<setw(30)<<NumOfSeventhO_2O_SetB<<setw(30)<<NumOfEighthO_2O_SetB<<setw(30)<<NumOfNinthO_2O_SetB<<endl;
			
			outFile_Num<<setw(10)<<"ThirdO: "<<setw(20)<<NumOfFU_ThirdO_SetB<<setw(30)<<NumOfFO_ThirdO_SetB<<setw(30)<<NumOf2O_ThirdO_SetB<<setw(30)<<NumOfThirdO_ThirdO_SetB<<setw(30)<<NumOfForthO_ThirdO_SetB<<setw(30)<<NumOfFifthO_ThirdO_SetB<<setw(30)<<NumOfSixthO_ThirdO_SetB<<setw(30)<<NumOfSeventhO_ThirdO_SetB<<setw(30)<<NumOfEighthO_ThirdO_SetB<<setw(30)<<NumOfNinthO_ThirdO_SetB<<endl;
			
			outFile_Num<<setw(10)<<"ForthO: "<<setw(20)<<NumOfFU_ForthO_SetB<<setw(30)<<NumOfFO_ForthO_SetB<<setw(30)<<NumOf2O_ForthO_SetB<<setw(30)<<NumOfThirdO_ForthO_SetB<<setw(30)<<NumOfForthO_ForthO_SetB<<setw(30)<<NumOfFifthO_ForthO_SetB<<setw(30)<<NumOfSixthO_ForthO_SetB<<setw(30)<<NumOfSeventhO_ForthO_SetB<<setw(30)<<NumOfEighthO_ForthO_SetB<<setw(30)<<NumOfNinthO_ForthO_SetB<<endl;
			
			outFile_Num<<setw(10)<<"FifthO: "<<setw(20)<<NumOfFU_FifthO_SetB<<setw(30)<<NumOfFO_FifthO_SetB<<setw(30)<<NumOf2O_FifthO_SetB<<setw(30)<<NumOfThirdO_FifthO_SetB<<setw(30)<<NumOfForthO_FifthO_SetB<<setw(30)<<NumOfFifthO_FifthO_SetB<<setw(30)<<NumOfSixthO_FifthO_SetB<<setw(30)<<NumOfSeventhO_FifthO_SetB<<setw(30)<<NumOfEighthO_FifthO_SetB<<setw(30)<<NumOfNinthO_FifthO_SetB<<endl;
			
			outFile_Num<<setw(10)<<"SixthO: "<<setw(20)<<NumOfFU_SixthO_SetB<<setw(30)<<NumOfFO_SixthO_SetB<<setw(30)<<NumOf2O_SixthO_SetB<<setw(30)<<NumOfThirdO_SixthO_SetB<<setw(30)<<NumOfForthO_SixthO_SetB<<setw(30)<<NumOfFifthO_SixthO_SetB<<setw(30)<<NumOfSixthO_SixthO_SetB<<setw(30)<<NumOfSeventhO_SixthO_SetB<<setw(30)<<NumOfEighthO_SixthO_SetB<<setw(30)<<NumOfNinthO_SixthO_SetB<<endl;
			
			outFile_Num<<setw(10)<<"SeventhO: "<<setw(20)<<NumOfFU_SeventhO_SetB<<setw(30)<<NumOfFO_SeventhO_SetB<<setw(30)<<NumOf2O_SeventhO_SetB<<setw(30)<<NumOfThirdO_SeventhO_SetB<<setw(30)<<NumOfForthO_SeventhO_SetB<<setw(30)<<NumOfFifthO_SeventhO_SetB<<setw(30)<<NumOfSixthO_SeventhO_SetB<<setw(30)<<NumOfSeventhO_SeventhO_SetB<<setw(30)<<NumOfEighthO_SeventhO_SetB<<setw(30)<<NumOfNinthO_SeventhO_SetB<<endl;
			
			outFile_Num<<setw(10)<<"EighthO: "<<setw(20)<<NumOfFU_EighthO_SetB<<setw(30)<<NumOfFO_EighthO_SetB<<setw(30)<<NumOf2O_EighthO_SetB<<setw(30)<<NumOfThirdO_EighthO_SetB<<setw(30)<<NumOfForthO_EighthO_SetB<<setw(30)<<NumOfFifthO_EighthO_SetB<<setw(30)<<NumOfSixthO_EighthO_SetB<<setw(30)<<NumOfSeventhO_EighthO_SetB<<setw(30)<<NumOfEighthO_EighthO_SetB<<setw(30)<<NumOfNinthO_EighthO_SetB<<endl;
			
			outFile_Num<<setw(10)<<"NinthO: "<<setw(20)<<NumOfFU_NinthO_SetB<<setw(30)<<NumOfFO_NinthO_SetB<<setw(30)<<NumOf2O_NinthO_SetB<<setw(30)<<NumOfThirdO_NinthO_SetB<<setw(30)<<NumOfForthO_NinthO_SetB<<setw(30)<<NumOfFifthO_NinthO_SetB<<setw(30)<<NumOfSixthO_NinthO_SetB<<setw(30)<<NumOfSeventhO_NinthO_SetB<<setw(30)<<NumOfEighthO_NinthO_SetB<<setw(30)<<NumOfNinthO_NinthO_SetB<<endl;
			
			
			////Output for SD calcs////
			outFile_SD<<setw(10)<<"FU: "<<setw(20)<<SD_FU_FU_SetB<<setw(30)<<SD_FO_FU_SetB<<setw(30)<<SD_2O_FU_SetB<<setw(30)<<SD_ThirdO_FU_SetB<<setw(30)<<SD_ForthO_FU_SetB<<setw(30)<<SD_FifthO_FU_SetB<<setw(30)<<SD_SixthO_FU_SetB<<setw(30)<<SD_SeventhO_FU_SetB<<setw(30)<<SD_EighthO_FU_SetB<<setw(30)<<SD_NinthO_FU_SetB<<endl;
			
			outFile_SD<<setw(10)<<"FO: "<<setw(20)<<SD_FU_FO_SetB<<setw(30)<<SD_FO_FO_SetB<<setw(30)<<SD_2O_FO_SetB<<setw(30)<<SD_ThirdO_FO_SetB<<setw(30)<<SD_ForthO_FO_SetB<<setw(30)<<SD_FifthO_FO_SetB<<setw(30)<<SD_SixthO_FO_SetB<<setw(30)<<SD_SeventhO_FO_SetB<<setw(30)<<SD_EighthO_FO_SetB<<setw(30)<<SD_NinthO_FO_SetB<<endl;
			
			outFile_SD<<setw(10)<<"2O: "<<setw(20)<<SD_FU_2O_SetB<<setw(30)<<SD_FO_2O_SetB<<setw(30)<<SD_2O_2O_SetB<<setw(30)<<SD_ThirdO_2O_SetB<<setw(30)<<SD_ForthO_2O_SetB<<setw(30)<<SD_FifthO_2O_SetB<<setw(30)<<SD_SixthO_2O_SetB<<setw(30)<<SD_SeventhO_2O_SetB<<setw(30)<<SD_EighthO_2O_SetB<<setw(30)<<SD_NinthO_2O_SetB<<endl;
			
			outFile_SD<<setw(10)<<"ThirdO: "<<setw(20)<<SD_FU_ThirdO_SetB<<setw(30)<<SD_FO_ThirdO_SetB<<setw(30)<<SD_2O_ThirdO_SetB<<setw(30)<<SD_ThirdO_ThirdO_SetB<<setw(30)<<SD_ForthO_ThirdO_SetB<<setw(30)<<SD_FifthO_ThirdO_SetB<<setw(30)<<SD_SixthO_ThirdO_SetB<<setw(30)<<SD_SeventhO_ThirdO_SetB<<setw(30)<<SD_EighthO_ThirdO_SetB<<setw(30)<<SD_NinthO_ThirdO_SetB<<endl;
			
			outFile_SD<<setw(10)<<"ForthO: "<<setw(20)<<SD_FU_ForthO_SetB<<setw(30)<<SD_FO_ForthO_SetB<<setw(30)<<SD_2O_ForthO_SetB<<setw(30)<<SD_ThirdO_ForthO_SetB<<setw(30)<<SD_ForthO_ForthO_SetB<<setw(30)<<SD_FifthO_ForthO_SetB<<setw(30)<<SD_SixthO_ForthO_SetB<<setw(30)<<SD_SeventhO_ForthO_SetB<<setw(30)<<SD_EighthO_ForthO_SetB<<setw(30)<<SD_NinthO_ForthO_SetB<<endl;
			
			outFile_SD<<setw(10)<<"FifthO: "<<setw(20)<<SD_FU_FifthO_SetB<<setw(30)<<SD_FO_FifthO_SetB<<setw(30)<<SD_2O_FifthO_SetB<<setw(30)<<SD_ThirdO_FifthO_SetB<<setw(30)<<SD_ForthO_FifthO_SetB<<setw(30)<<SD_FifthO_FifthO_SetB<<setw(30)<<SD_SixthO_FifthO_SetB<<setw(30)<<SD_SeventhO_FifthO_SetB<<setw(30)<<SD_EighthO_FifthO_SetB<<setw(30)<<SD_NinthO_FifthO_SetB<<endl;
			
			outFile_SD<<setw(10)<<"SixthO: "<<setw(20)<<SD_FU_SixthO_SetB<<setw(30)<<SD_FO_SixthO_SetB<<setw(30)<<SD_2O_SixthO_SetB<<setw(30)<<SD_ThirdO_SixthO_SetB<<setw(30)<<SD_ForthO_SixthO_SetB<<setw(30)<<SD_FifthO_SixthO_SetB<<setw(30)<<SD_SixthO_SixthO_SetB<<setw(30)<<SD_SeventhO_SixthO_SetB<<setw(30)<<SD_EighthO_SixthO_SetB<<setw(30)<<SD_NinthO_SixthO_SetB<<endl;
			
			outFile_SD<<setw(10)<<"SeventhO: "<<setw(20)<<SD_FU_SeventhO_SetB<<setw(30)<<SD_FO_SeventhO_SetB<<setw(30)<<SD_2O_SeventhO_SetB<<setw(30)<<SD_ThirdO_SeventhO_SetB<<setw(30)<<SD_ForthO_SeventhO_SetB<<setw(30)<<SD_FifthO_SeventhO_SetB<<setw(30)<<SD_SixthO_SeventhO_SetB<<setw(30)<<SD_SeventhO_SeventhO_SetB<<setw(30)<<SD_EighthO_SeventhO_SetB<<setw(30)<<SD_NinthO_SeventhO_SetB<<endl;
			
			outFile_SD<<setw(10)<<"EighthO: "<<setw(20)<<SD_FU_EighthO_SetB<<setw(30)<<SD_FO_EighthO_SetB<<setw(30)<<SD_2O_EighthO_SetB<<setw(30)<<SD_ThirdO_EighthO_SetB<<setw(30)<<SD_ForthO_EighthO_SetB<<setw(30)<<SD_FifthO_EighthO_SetB<<setw(30)<<SD_SixthO_EighthO_SetB<<setw(30)<<SD_SeventhO_EighthO_SetB<<setw(30)<<SD_EighthO_EighthO_SetB<<setw(30)<<SD_NinthO_EighthO_SetB<<endl;
			
			outFile_SD<<setw(10)<<"NinthO: "<<setw(20)<<SD_FU_NinthO_SetB<<setw(30)<<SD_FO_NinthO_SetB<<setw(30)<<SD_2O_NinthO_SetB<<setw(30)<<SD_ThirdO_NinthO_SetB<<setw(30)<<SD_ForthO_NinthO_SetB<<setw(30)<<SD_FifthO_NinthO_SetB<<setw(30)<<SD_SixthO_NinthO_SetB<<setw(30)<<SD_SeventhO_NinthO_SetB<<setw(30)<<SD_EighthO_NinthO_SetB<<setw(30)<<SD_NinthO_NinthO_SetB<<endl;
			
			
			////Output for resonance ratios////	
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
			
			////output for fig5////
			for(int i = 0; i < NumOfModels; i++){
				if(Mass[i] == Mass_ToLookFor && L[i] == L_ToLookFor && ModelRan_SetB[i]){
					double frequency_FU = 1./period_FU_SetB[i];
					double frequency_FO = 1./period_FO_SetB[i];
					double frequency_2O = 1./period_2O_SetB[i];
					double frequency_ThirdO = 1./period_ThirdO_SetB[i];
					double frequency_ForthO = 1./period_ForthO_SetB[i];
					outFile_Fig5_FU_FO<<setw(10)<<Teff[i]<<setw(20)<<frequency_FU<<setw(20)<<frequency_FO<<setw(20)<<frequency_2O<<setw(20)<<frequency_ThirdO<<setw(20)<<frequency_ForthO<<setw(20)<<growth_FU_SetB[i]<<setw(20)<<growth_FO_SetB[i]<<setw(20)<<growth_2O_SetB[i]<<setw(20)<<growth_ThirdO_SetB[i]<<setw(20)<<growth_ForthO_SetB[i]<<endl;
				}
			}
		}
		if(j == 2){
			////SD Calc Part I (To get var)////
			for(int i = 0; i < NumOfModels; i++){
				double mean;
				//FU as check//
				if(IsSelected_FU_SetC[i]){
					mean = (OverallResonance_FU_FU_SetC/NumOfFU_FU_SetC);
					var_FU_FU_SetC = var_FU_FU_SetC + pow((ResonanceRatio_FU_FU_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FU_FO_SetC/NumOfFU_FO_SetC);
					var_FU_FO_SetC = var_FU_FO_SetC + pow((ResonanceRatio_FU_FO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FU_2O_SetC/NumOfFU_2O_SetC);
					var_FU_2O_SetC = var_FU_2O_SetC + pow((ResonanceRatio_FU_2O_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FU_ThirdO_SetC/NumOfFU_ThirdO_SetC);
					var_FU_ThirdO_SetC = var_FU_ThirdO_SetC + pow((ResonanceRatio_FU_ThirdO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FU_ForthO_SetC/NumOfFU_ForthO_SetC);
					var_FU_ForthO_SetC = var_FU_ForthO_SetC + pow((ResonanceRatio_FU_ForthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FU_FifthO_SetC/NumOfFU_FifthO_SetC);
					var_FU_FifthO_SetC = var_FU_FifthO_SetC + pow((ResonanceRatio_FU_FifthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FU_SixthO_SetC/NumOfFU_SixthO_SetC);
					var_FU_SixthO_SetC = var_FU_SixthO_SetC + pow((ResonanceRatio_FU_SixthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FU_SeventhO_SetC/NumOfFU_SeventhO_SetC);
					var_FU_SeventhO_SetC = var_FU_SeventhO_SetC + pow((ResonanceRatio_FU_SeventhO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FU_EighthO_SetC/NumOfFU_EighthO_SetC);
					var_FU_EighthO_SetC = var_FU_EighthO_SetC + pow((ResonanceRatio_FU_EighthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FU_NinthO_SetC/NumOfFU_NinthO_SetC);
					var_FU_NinthO_SetC = var_FU_NinthO_SetC + pow((ResonanceRatio_FU_NinthO_SetC[i] - mean),2.);
				}
							
				//FO//			
				if(IsSelected_FO_SetC[i]){
					mean = (OverallResonance_FO_FU_SetC/NumOfFO_FU_SetC);
					var_FO_FU_SetC = var_FO_FU_SetC + pow((ResonanceRatio_FO_FU_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FO_FO_SetC/NumOfFO_FO_SetC);
					var_FO_FO_SetC = var_FO_FO_SetC + pow((ResonanceRatio_FO_FO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FO_2O_SetC/NumOfFO_2O_SetC);
					var_FO_2O_SetC = var_FO_2O_SetC + pow((ResonanceRatio_FO_2O_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FO_ThirdO_SetC/NumOfFO_ThirdO_SetC);
					var_FO_ThirdO_SetC = var_FO_ThirdO_SetC + pow((ResonanceRatio_FO_ThirdO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FO_ForthO_SetC/NumOfFO_ForthO_SetC);
					var_FO_ForthO_SetC = var_FO_ForthO_SetC + pow((ResonanceRatio_FO_ForthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FO_FifthO_SetC/NumOfFO_FifthO_SetC);
					var_FO_FifthO_SetC = var_FO_FifthO_SetC + pow((ResonanceRatio_FO_FifthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FO_SixthO_SetC/NumOfFO_SixthO_SetC);
					var_FO_SixthO_SetC = var_FO_SixthO_SetC + pow((ResonanceRatio_FO_SixthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FO_SeventhO_SetC/NumOfFO_SeventhO_SetC);
					var_FO_SeventhO_SetC = var_FO_SeventhO_SetC + pow((ResonanceRatio_FO_SeventhO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FO_EighthO_SetC/NumOfFO_EighthO_SetC);
					var_FO_EighthO_SetC = var_FO_EighthO_SetC + pow((ResonanceRatio_FO_EighthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FO_NinthO_SetC/NumOfFO_NinthO_SetC);
					var_FO_NinthO_SetC = var_FO_NinthO_SetC + pow((ResonanceRatio_FO_NinthO_SetC[i] - mean),2.);
				}
			
				//2O//
				if(IsSelected_2O_SetC[i]){
					mean = (OverallResonance_2O_FU_SetC/NumOf2O_FU_SetC);
					var_2O_FU_SetC = var_2O_FU_SetC + pow((ResonanceRatio_2O_FU_SetC[i] - mean),2.);
					
					mean = (OverallResonance_2O_FO_SetC/NumOf2O_FO_SetC);
					var_2O_FO_SetC = var_2O_FO_SetC + pow((ResonanceRatio_2O_FO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_2O_2O_SetC/NumOf2O_2O_SetC);
					var_2O_2O_SetC = var_2O_2O_SetC + pow((ResonanceRatio_2O_2O_SetC[i] - mean),2.);
					
					mean = (OverallResonance_2O_ThirdO_SetC/NumOf2O_ThirdO_SetC);
					var_2O_ThirdO_SetC = var_2O_ThirdO_SetC + pow((ResonanceRatio_2O_ThirdO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_2O_ForthO_SetC/NumOf2O_ForthO_SetC);
					var_2O_ForthO_SetC = var_2O_ForthO_SetC + pow((ResonanceRatio_2O_ForthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_2O_FifthO_SetC/NumOf2O_FifthO_SetC);
					var_2O_FifthO_SetC = var_2O_FifthO_SetC + pow((ResonanceRatio_2O_FifthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_2O_SixthO_SetC/NumOf2O_SixthO_SetC);
					var_2O_SixthO_SetC = var_2O_SixthO_SetC + pow((ResonanceRatio_2O_SixthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_2O_SeventhO_SetC/NumOf2O_SeventhO_SetC);
					var_2O_SeventhO_SetC = var_2O_SeventhO_SetC + pow((ResonanceRatio_2O_SeventhO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_2O_EighthO_SetC/NumOf2O_EighthO_SetC);
					var_2O_EighthO_SetC = var_2O_EighthO_SetC + pow((ResonanceRatio_2O_EighthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_2O_NinthO_SetC/NumOf2O_NinthO_SetC);
					var_2O_NinthO_SetC = var_2O_NinthO_SetC + pow((ResonanceRatio_2O_NinthO_SetC[i] - mean),2.);
				}
			
				//ThirdO//
				if(IsSelected_ThirdO_SetC[i]){
					mean = (OverallResonance_ThirdO_FU_SetC/NumOfThirdO_FU_SetC);
					var_ThirdO_FU_SetC = var_ThirdO_FU_SetC + pow((ResonanceRatio_ThirdO_FU_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_FO_SetC/NumOfThirdO_FO_SetC);
					var_ThirdO_FO_SetC = var_ThirdO_FO_SetC + pow((ResonanceRatio_ThirdO_FO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_2O_SetC/NumOfThirdO_2O_SetC);
					var_ThirdO_2O_SetC = var_ThirdO_2O_SetC + pow((ResonanceRatio_ThirdO_2O_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_ThirdO_SetC/NumOfThirdO_ThirdO_SetC);
					var_ThirdO_ThirdO_SetC = var_ThirdO_ThirdO_SetC + pow((ResonanceRatio_ThirdO_ThirdO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_ForthO_SetC/NumOfThirdO_ForthO_SetC);
					var_ThirdO_ForthO_SetC = var_ThirdO_ForthO_SetC + pow((ResonanceRatio_ThirdO_ForthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_FifthO_SetC/NumOfThirdO_FifthO_SetC);
					var_ThirdO_FifthO_SetC = var_ThirdO_FifthO_SetC + pow((ResonanceRatio_ThirdO_FifthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_SixthO_SetC/NumOfThirdO_SixthO_SetC);
					var_ThirdO_SixthO_SetC = var_ThirdO_SixthO_SetC + pow((ResonanceRatio_ThirdO_SixthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_SeventhO_SetC/NumOfThirdO_SeventhO_SetC);
					var_ThirdO_SeventhO_SetC = var_ThirdO_SeventhO_SetC + pow((ResonanceRatio_ThirdO_SeventhO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_EighthO_SetC/NumOfThirdO_EighthO_SetC);
					var_ThirdO_EighthO_SetC = var_ThirdO_EighthO_SetC + pow((ResonanceRatio_ThirdO_EighthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_NinthO_SetC/NumOfThirdO_NinthO_SetC);
					var_ThirdO_NinthO_SetC = var_ThirdO_NinthO_SetC + pow((ResonanceRatio_ThirdO_NinthO_SetC[i] - mean),2.);
				}
			
				//ForthO//
				if(IsSelected_ForthO_SetC[i]){
					mean = (OverallResonance_ForthO_FU_SetC/NumOfForthO_FU_SetC);
					var_ForthO_FU_SetC = var_ForthO_FU_SetC + pow((ResonanceRatio_ForthO_FU_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_FO_SetC/NumOfForthO_FO_SetC);
					var_ForthO_FO_SetC = var_ForthO_FO_SetC + pow((ResonanceRatio_ForthO_FO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_2O_SetC/NumOfForthO_2O_SetC);
					var_ForthO_2O_SetC = var_ForthO_2O_SetC + pow((ResonanceRatio_ForthO_2O_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_ThirdO_SetC/NumOfForthO_ThirdO_SetC);
					var_ForthO_ThirdO_SetC = var_ForthO_ThirdO_SetC + pow((ResonanceRatio_ForthO_ThirdO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_ForthO_SetC/NumOfForthO_ForthO_SetC);
					var_ForthO_ForthO_SetC = var_ForthO_ForthO_SetC + pow((ResonanceRatio_ForthO_ForthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_FifthO_SetC/NumOfForthO_FifthO_SetC);
					var_ForthO_FifthO_SetC = var_ForthO_FifthO_SetC + pow((ResonanceRatio_ForthO_FifthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_SixthO_SetC/NumOfForthO_SixthO_SetC);
					var_ForthO_SixthO_SetC = var_ForthO_SixthO_SetC + pow((ResonanceRatio_ForthO_SixthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_SeventhO_SetC/NumOfForthO_SeventhO_SetC);
					var_ForthO_SeventhO_SetC = var_ForthO_SeventhO_SetC + pow((ResonanceRatio_ForthO_SeventhO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_EighthO_SetC/NumOfForthO_EighthO_SetC);
					var_ForthO_EighthO_SetC = var_ForthO_EighthO_SetC + pow((ResonanceRatio_ForthO_EighthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_NinthO_SetC/NumOfForthO_NinthO_SetC);
					var_ForthO_NinthO_SetC = var_ForthO_NinthO_SetC + pow((ResonanceRatio_ForthO_NinthO_SetC[i] - mean),2.);
				}
				
				//FifthO//
				if(IsSelected_FifthO_SetC[i]){
					mean = (OverallResonance_FifthO_FU_SetC/NumOfFifthO_FU_SetC);
					var_FifthO_FU_SetC = var_FifthO_FU_SetC + pow((ResonanceRatio_FifthO_FU_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_FO_SetC/NumOfFifthO_FO_SetC);
					var_FifthO_FO_SetC = var_FifthO_FO_SetC + pow((ResonanceRatio_FifthO_FO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_2O_SetC/NumOfFifthO_2O_SetC);
					var_FifthO_2O_SetC = var_FifthO_2O_SetC + pow((ResonanceRatio_FifthO_2O_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_ThirdO_SetC/NumOfFifthO_ThirdO_SetC);
					var_FifthO_ThirdO_SetC = var_FifthO_ThirdO_SetC + pow((ResonanceRatio_FifthO_ThirdO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_ForthO_SetC/NumOfFifthO_ForthO_SetC);
					var_FifthO_ForthO_SetC = var_FifthO_ForthO_SetC + pow((ResonanceRatio_FifthO_ForthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_FifthO_SetC/NumOfFifthO_FifthO_SetC);
					var_FifthO_FifthO_SetC = var_FifthO_FifthO_SetC + pow((ResonanceRatio_FifthO_FifthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_SixthO_SetC/NumOfFifthO_SixthO_SetC);
					var_FifthO_SixthO_SetC = var_FifthO_SixthO_SetC + pow((ResonanceRatio_FifthO_SixthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_SeventhO_SetC/NumOfFifthO_SeventhO_SetC);
					var_FifthO_SeventhO_SetC = var_FifthO_SeventhO_SetC + pow((ResonanceRatio_FifthO_SeventhO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_EighthO_SetC/NumOfFifthO_EighthO_SetC);
					var_FifthO_EighthO_SetC = var_FifthO_EighthO_SetC + pow((ResonanceRatio_FifthO_EighthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_NinthO_SetC/NumOfFifthO_NinthO_SetC);
					var_FifthO_NinthO_SetC = var_FifthO_NinthO_SetC + pow((ResonanceRatio_FifthO_NinthO_SetC[i] - mean),2.);
				}
			
				//SixthO//
				if(IsSelected_SixthO_SetC[i]){
					mean = (OverallResonance_SixthO_FU_SetC/NumOfSixthO_FU_SetC);
					var_SixthO_FU_SetC = var_SixthO_FU_SetC + pow((ResonanceRatio_SixthO_FU_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_FO_SetC/NumOfSixthO_FO_SetC);
					var_SixthO_FO_SetC = var_SixthO_FO_SetC + pow((ResonanceRatio_SixthO_FO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_2O_SetC/NumOfSixthO_2O_SetC);
					var_SixthO_2O_SetC = var_SixthO_2O_SetC + pow((ResonanceRatio_SixthO_2O_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_ThirdO_SetC/NumOfSixthO_ThirdO_SetC);
					var_SixthO_ThirdO_SetC = var_SixthO_ThirdO_SetC + pow((ResonanceRatio_SixthO_ThirdO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_ForthO_SetC/NumOfSixthO_ForthO_SetC);
					var_SixthO_ForthO_SetC = var_SixthO_ForthO_SetC + pow((ResonanceRatio_SixthO_ForthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_FifthO_SetC/NumOfSixthO_FifthO_SetC);
					var_SixthO_FifthO_SetC = var_SixthO_FifthO_SetC + pow((ResonanceRatio_SixthO_FifthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_SixthO_SetC/NumOfSixthO_SixthO_SetC);
					var_SixthO_SixthO_SetC = var_SixthO_SixthO_SetC + pow((ResonanceRatio_SixthO_SixthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_SeventhO_SetC/NumOfSixthO_SeventhO_SetC);
					var_SixthO_SeventhO_SetC = var_SixthO_SeventhO_SetC + pow((ResonanceRatio_SixthO_SeventhO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_EighthO_SetC/NumOfSixthO_EighthO_SetC);
					var_SixthO_EighthO_SetC = var_SixthO_EighthO_SetC + pow((ResonanceRatio_SixthO_EighthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_NinthO_SetC/NumOfSixthO_NinthO_SetC);
					var_SixthO_NinthO_SetC = var_SixthO_NinthO_SetC + pow((ResonanceRatio_SixthO_NinthO_SetC[i] - mean),2.);
				}
			
				//SeventhO//
				if(IsSelected_SeventhO_SetC[i]){
					mean = (OverallResonance_SeventhO_FU_SetC/NumOfSeventhO_FU_SetC);
					var_SeventhO_FU_SetC = var_SeventhO_FU_SetC + pow((ResonanceRatio_SeventhO_FU_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_FO_SetC/NumOfSeventhO_FO_SetC);
					var_SeventhO_FO_SetC = var_SeventhO_FO_SetC + pow((ResonanceRatio_SeventhO_FO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_2O_SetC/NumOfSeventhO_2O_SetC);
					var_SeventhO_2O_SetC = var_SeventhO_2O_SetC + pow((ResonanceRatio_SeventhO_2O_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_ThirdO_SetC/NumOfSeventhO_ThirdO_SetC);
					var_SeventhO_ThirdO_SetC = var_SeventhO_ThirdO_SetC + pow((ResonanceRatio_SeventhO_ThirdO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_ForthO_SetC/NumOfSeventhO_ForthO_SetC);
					var_SeventhO_ForthO_SetC = var_SeventhO_ForthO_SetC + pow((ResonanceRatio_SeventhO_ForthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_FifthO_SetC/NumOfSeventhO_FifthO_SetC);
					var_SeventhO_FifthO_SetC = var_SeventhO_FifthO_SetC + pow((ResonanceRatio_SeventhO_FifthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_SixthO_SetC/NumOfSeventhO_SixthO_SetC);
					var_SeventhO_SixthO_SetC = var_SeventhO_SixthO_SetC + pow((ResonanceRatio_SeventhO_SixthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_SeventhO_SetC/NumOfSeventhO_SeventhO_SetC);
					var_SeventhO_SeventhO_SetC = var_SeventhO_SeventhO_SetC + pow((ResonanceRatio_SeventhO_SeventhO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_EighthO_SetC/NumOfSeventhO_EighthO_SetC);
					var_SeventhO_EighthO_SetC = var_SeventhO_EighthO_SetC + pow((ResonanceRatio_SeventhO_EighthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_NinthO_SetC/NumOfSeventhO_NinthO_SetC);
					var_SeventhO_NinthO_SetC = var_SeventhO_NinthO_SetC + pow((ResonanceRatio_SeventhO_NinthO_SetC[i] - mean),2.);
				}
			
				//EighthO//
				if(IsSelected_EighthO_SetC[i]){
					mean = (OverallResonance_EighthO_FU_SetC/NumOfEighthO_FU_SetC);
					var_EighthO_FU_SetC = var_EighthO_FU_SetC + pow((ResonanceRatio_EighthO_FU_SetC[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_FO_SetC/NumOfEighthO_FO_SetC);
					var_EighthO_FO_SetC = var_EighthO_FO_SetC + pow((ResonanceRatio_EighthO_FO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_2O_SetC/NumOfEighthO_2O_SetC);
					var_EighthO_2O_SetC = var_EighthO_2O_SetC + pow((ResonanceRatio_EighthO_2O_SetC[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_ThirdO_SetC/NumOfEighthO_ThirdO_SetC);
					var_EighthO_ThirdO_SetC = var_EighthO_ThirdO_SetC + pow((ResonanceRatio_EighthO_ThirdO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_ForthO_SetC/NumOfEighthO_ForthO_SetC);
					var_EighthO_ForthO_SetC = var_EighthO_ForthO_SetC + pow((ResonanceRatio_EighthO_ForthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_FifthO_SetC/NumOfEighthO_FifthO_SetC);
					var_EighthO_FifthO_SetC = var_EighthO_FifthO_SetC + pow((ResonanceRatio_EighthO_FifthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_SixthO_SetC/NumOfEighthO_SixthO_SetC);
					var_EighthO_SixthO_SetC = var_EighthO_SixthO_SetC + pow((ResonanceRatio_EighthO_SixthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_SeventhO_SetC/NumOfEighthO_SeventhO_SetC);
					var_EighthO_SeventhO_SetC = var_EighthO_SeventhO_SetC + pow((ResonanceRatio_EighthO_SeventhO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_EighthO_SetC/NumOfEighthO_EighthO_SetC);
					var_EighthO_EighthO_SetC = var_EighthO_EighthO_SetC + pow((ResonanceRatio_EighthO_EighthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_NinthO_SetC/NumOfEighthO_NinthO_SetC);
					var_EighthO_NinthO_SetC = var_EighthO_NinthO_SetC + pow((ResonanceRatio_EighthO_NinthO_SetC[i] - mean),2.);
				}
				
				//NinthO//
				if(IsSelected_NinthO_SetC[i]){
					mean = (OverallResonance_NinthO_FU_SetC/NumOfNinthO_FU_SetC);
					var_NinthO_FU_SetC = var_NinthO_FU_SetC + pow((ResonanceRatio_NinthO_FU_SetC[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_FO_SetC/NumOfNinthO_FO_SetC);
					var_NinthO_FO_SetC = var_NinthO_FO_SetC + pow((ResonanceRatio_NinthO_FO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_2O_SetC/NumOfNinthO_2O_SetC);
					var_NinthO_2O_SetC = var_NinthO_2O_SetC + pow((ResonanceRatio_NinthO_2O_SetC[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_ThirdO_SetC/NumOfNinthO_ThirdO_SetC);
					var_NinthO_ThirdO_SetC = var_NinthO_ThirdO_SetC + pow((ResonanceRatio_NinthO_ThirdO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_ForthO_SetC/NumOfNinthO_ForthO_SetC);
					var_NinthO_ForthO_SetC = var_NinthO_ForthO_SetC + pow((ResonanceRatio_NinthO_ForthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_FifthO_SetC/NumOfNinthO_FifthO_SetC);
					var_NinthO_FifthO_SetC = var_NinthO_FifthO_SetC + pow((ResonanceRatio_NinthO_FifthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_SixthO_SetC/NumOfNinthO_SixthO_SetC);
					var_NinthO_SixthO_SetC = var_NinthO_SixthO_SetC + pow((ResonanceRatio_NinthO_SixthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_SeventhO_SetC/NumOfNinthO_SeventhO_SetC);
					var_NinthO_SeventhO_SetC = var_NinthO_SeventhO_SetC + pow((ResonanceRatio_NinthO_SeventhO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_EighthO_SetC/NumOfNinthO_EighthO_SetC);
					var_NinthO_EighthO_SetC = var_NinthO_EighthO_SetC + pow((ResonanceRatio_NinthO_EighthO_SetC[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_NinthO_SetC/NumOfNinthO_NinthO_SetC);
					var_NinthO_NinthO_SetC = var_NinthO_NinthO_SetC + pow((ResonanceRatio_NinthO_NinthO_SetC[i] - mean),2.);
				}
			}
			
			////SD Calc Part II (To get final SD)////
			//FU as check mode//
			SD_FU_FU_SetC = sqrt((1./(NumOfFU_FU_SetC - 1.))*var_FU_FU_SetC);
			SD_FU_FO_SetC = sqrt((1./(NumOfFU_FO_SetC - 1.))*var_FU_FO_SetC);
			SD_FU_2O_SetC = sqrt((1./(NumOfFU_2O_SetC - 1.))*var_FU_2O_SetC);
			SD_FU_ThirdO_SetC = sqrt((1./(NumOfFU_ThirdO_SetC - 1.))*var_FU_ThirdO_SetC);
			SD_FU_ForthO_SetC = sqrt((1./(NumOfFU_ForthO_SetC - 1.))*var_FU_ForthO_SetC);
			SD_FU_FifthO_SetC = sqrt((1./(NumOfFU_FifthO_SetC - 1.))*var_FU_FifthO_SetC);
			SD_FU_SixthO_SetC = sqrt((1./(NumOfFU_SixthO_SetC - 1.))*var_FU_SixthO_SetC);
			SD_FU_SeventhO_SetC = sqrt((1./(NumOfFU_SeventhO_SetC - 1.))*var_FU_SeventhO_SetC);
			SD_FU_EighthO_SetC = sqrt((1./(NumOfFU_EighthO_SetC - 1.))*var_FU_EighthO_SetC);
			SD_FU_NinthO_SetC = sqrt((1./(NumOfFU_NinthO_SetC - 1.))*var_FU_NinthO_SetC);
			
			//FO as check mode//
			SD_FO_FU_SetC = sqrt((1./(NumOfFO_FU_SetC - 1.))*var_FO_FU_SetC);
			SD_FO_FO_SetC = sqrt((1./(NumOfFO_FO_SetC - 1.))*var_FO_FO_SetC);
			SD_FO_2O_SetC = sqrt((1./(NumOfFO_2O_SetC - 1.))*var_FO_2O_SetC);
			SD_FO_ThirdO_SetC = sqrt((1./(NumOfFO_ThirdO_SetC - 1.))*var_FO_ThirdO_SetC);
			SD_FO_ForthO_SetC = sqrt((1./(NumOfFO_ForthO_SetC - 1.))*var_FO_ForthO_SetC);
			SD_FO_FifthO_SetC = sqrt((1./(NumOfFO_FifthO_SetC - 1.))*var_FO_FifthO_SetC);
			SD_FO_SixthO_SetC = sqrt((1./(NumOfFO_SixthO_SetC - 1.))*var_FO_SixthO_SetC);
			SD_FO_SeventhO_SetC = sqrt((1./(NumOfFO_SeventhO_SetC - 1.))*var_FO_SeventhO_SetC);
			SD_FO_EighthO_SetC = sqrt((1./(NumOfFO_EighthO_SetC - 1.))*var_FO_EighthO_SetC);
			SD_FO_NinthO_SetC = sqrt((1./(NumOfFO_NinthO_SetC - 1.))*var_FO_NinthO_SetC);
			
			//2O as check mode//
			SD_2O_FU_SetC = sqrt((1./(NumOf2O_FU_SetC - 1.))*var_2O_FU_SetC);
			SD_2O_FO_SetC = sqrt((1./(NumOf2O_FO_SetC - 1.))*var_2O_FO_SetC);
			SD_2O_2O_SetC = sqrt((1./(NumOf2O_2O_SetC - 1.))*var_2O_2O_SetC);
			SD_2O_ThirdO_SetC = sqrt((1./(NumOf2O_ThirdO_SetC - 1.))*var_2O_ThirdO_SetC);
			SD_2O_ForthO_SetC = sqrt((1./(NumOf2O_ForthO_SetC - 1.))*var_2O_ForthO_SetC);
			SD_2O_FifthO_SetC = sqrt((1./(NumOf2O_FifthO_SetC - 1.))*var_2O_FifthO_SetC);
			SD_2O_SixthO_SetC = sqrt((1./(NumOf2O_SixthO_SetC - 1.))*var_2O_SixthO_SetC);
			SD_2O_SeventhO_SetC = sqrt((1./(NumOf2O_SeventhO_SetC - 1.))*var_2O_SeventhO_SetC);
			SD_2O_EighthO_SetC = sqrt((1./(NumOf2O_EighthO_SetC - 1.))*var_2O_EighthO_SetC);
			SD_2O_NinthO_SetC = sqrt((1./(NumOf2O_NinthO_SetC - 1.))*var_2O_NinthO_SetC);
			
			//ThirdO as check mode//
			SD_ThirdO_FU_SetC = sqrt((1./(NumOfThirdO_FU_SetC - 1.))*var_ThirdO_FU_SetC);
			SD_ThirdO_FO_SetC = sqrt((1./(NumOfThirdO_FO_SetC - 1.))*var_ThirdO_FO_SetC);
			SD_ThirdO_2O_SetC = sqrt((1./(NumOfThirdO_2O_SetC - 1.))*var_ThirdO_2O_SetC);
			SD_ThirdO_ThirdO_SetC = sqrt((1./(NumOfThirdO_ThirdO_SetC - 1.))*var_ThirdO_ThirdO_SetC);
			SD_ThirdO_ForthO_SetC = sqrt((1./(NumOfThirdO_ForthO_SetC - 1.))*var_ThirdO_ForthO_SetC);
			SD_ThirdO_FifthO_SetC = sqrt((1./(NumOfThirdO_FifthO_SetC - 1.))*var_ThirdO_FifthO_SetC);
			SD_ThirdO_SixthO_SetC = sqrt((1./(NumOfThirdO_SixthO_SetC - 1.))*var_ThirdO_SixthO_SetC);
			SD_ThirdO_SeventhO_SetC = sqrt((1./(NumOfThirdO_SeventhO_SetC - 1.))*var_ThirdO_SeventhO_SetC);
			SD_ThirdO_EighthO_SetC = sqrt((1./(NumOfThirdO_EighthO_SetC - 1.))*var_ThirdO_EighthO_SetC);
			SD_ThirdO_NinthO_SetC = sqrt((1./(NumOfThirdO_NinthO_SetC - 1.))*var_ThirdO_NinthO_SetC);
			
			//ForthO as check mode//
			SD_ForthO_FU_SetC = sqrt((1./(NumOfForthO_FU_SetC - 1.))*var_ForthO_FU_SetC);
			SD_ForthO_FO_SetC = sqrt((1./(NumOfForthO_FO_SetC - 1.))*var_ForthO_FO_SetC);
			SD_ForthO_2O_SetC = sqrt((1./(NumOfForthO_2O_SetC - 1.))*var_ForthO_2O_SetC);
			SD_ForthO_ThirdO_SetC = sqrt((1./(NumOfForthO_ThirdO_SetC - 1.))*var_ForthO_ThirdO_SetC);
			SD_ForthO_ForthO_SetC = sqrt((1./(NumOfForthO_ForthO_SetC - 1.))*var_ForthO_ForthO_SetC);
			SD_ForthO_FifthO_SetC = sqrt((1./(NumOfForthO_FifthO_SetC - 1.))*var_ForthO_FifthO_SetC);
			SD_ForthO_SixthO_SetC = sqrt((1./(NumOfForthO_SixthO_SetC - 1.))*var_ForthO_SixthO_SetC);
			SD_ForthO_SeventhO_SetC = sqrt((1./(NumOfForthO_SeventhO_SetC - 1.))*var_ForthO_SeventhO_SetC);
			SD_ForthO_EighthO_SetC = sqrt((1./(NumOfForthO_EighthO_SetC - 1.))*var_ForthO_EighthO_SetC);
			SD_ForthO_NinthO_SetC = sqrt((1./(NumOfForthO_NinthO_SetC - 1.))*var_ForthO_NinthO_SetC);
			
			//FifthO as check mode//
			SD_FifthO_FU_SetC = sqrt((1./(NumOfFifthO_FU_SetC - 1.))*var_FifthO_FU_SetC);
			SD_FifthO_FO_SetC = sqrt((1./(NumOfFifthO_FO_SetC - 1.))*var_FifthO_FO_SetC);
			SD_FifthO_2O_SetC = sqrt((1./(NumOfFifthO_2O_SetC - 1.))*var_FifthO_2O_SetC);
			SD_FifthO_ThirdO_SetC = sqrt((1./(NumOfFifthO_ThirdO_SetC - 1.))*var_FifthO_ThirdO_SetC);
			SD_FifthO_ForthO_SetC = sqrt((1./(NumOfFifthO_ForthO_SetC - 1.))*var_FifthO_ForthO_SetC);
			SD_FifthO_FifthO_SetC = sqrt((1./(NumOfFifthO_FifthO_SetC - 1.))*var_FifthO_FifthO_SetC);
			SD_FifthO_SixthO_SetC = sqrt((1./(NumOfFifthO_SixthO_SetC - 1.))*var_FifthO_SixthO_SetC);
			SD_FifthO_SeventhO_SetC = sqrt((1./(NumOfFifthO_SeventhO_SetC - 1.))*var_FifthO_SeventhO_SetC);
			SD_FifthO_EighthO_SetC = sqrt((1./(NumOfFifthO_EighthO_SetC - 1.))*var_FifthO_EighthO_SetC);
			SD_FifthO_NinthO_SetC = sqrt((1./(NumOfFifthO_NinthO_SetC - 1.))*var_FifthO_NinthO_SetC);
			
			//SixthO as check mode//
			SD_SixthO_FU_SetC = sqrt((1./(NumOfSixthO_FU_SetC - 1.))*var_SixthO_FU_SetC);
			SD_SixthO_FO_SetC = sqrt((1./(NumOfSixthO_FO_SetC - 1.))*var_SixthO_FO_SetC);
			SD_SixthO_2O_SetC = sqrt((1./(NumOfSixthO_2O_SetC - 1.))*var_SixthO_2O_SetC);
			SD_SixthO_ThirdO_SetC = sqrt((1./(NumOfSixthO_ThirdO_SetC - 1.))*var_SixthO_ThirdO_SetC);
			SD_SixthO_ForthO_SetC = sqrt((1./(NumOfSixthO_ForthO_SetC - 1.))*var_SixthO_ForthO_SetC);
			SD_SixthO_FifthO_SetC = sqrt((1./(NumOfSixthO_FifthO_SetC - 1.))*var_SixthO_FifthO_SetC);
			SD_SixthO_SixthO_SetC = sqrt((1./(NumOfSixthO_SixthO_SetC - 1.))*var_SixthO_SixthO_SetC);
			SD_SixthO_SeventhO_SetC = sqrt((1./(NumOfSixthO_SeventhO_SetC - 1.))*var_SixthO_SeventhO_SetC);
			SD_SixthO_EighthO_SetC = sqrt((1./(NumOfSixthO_EighthO_SetC - 1.))*var_SixthO_EighthO_SetC);
			SD_SixthO_NinthO_SetC = sqrt((1./(NumOfSixthO_NinthO_SetC - 1.))*var_SixthO_NinthO_SetC);
			
			//SeventhO as check mode//
			SD_SeventhO_FU_SetC = sqrt((1./(NumOfSeventhO_FU_SetC - 1.))*var_SeventhO_FU_SetC);
			SD_SeventhO_FO_SetC = sqrt((1./(NumOfSeventhO_FO_SetC - 1.))*var_SeventhO_FO_SetC);
			SD_SeventhO_2O_SetC = sqrt((1./(NumOfSeventhO_2O_SetC - 1.))*var_SeventhO_2O_SetC);
			SD_SeventhO_ThirdO_SetC = sqrt((1./(NumOfSeventhO_ThirdO_SetC - 1.))*var_SeventhO_ThirdO_SetC);
			SD_SeventhO_ForthO_SetC = sqrt((1./(NumOfSeventhO_ForthO_SetC - 1.))*var_SeventhO_ForthO_SetC);
			SD_SeventhO_FifthO_SetC = sqrt((1./(NumOfSeventhO_FifthO_SetC - 1.))*var_SeventhO_FifthO_SetC);
			SD_SeventhO_SixthO_SetC = sqrt((1./(NumOfSeventhO_SixthO_SetC - 1.))*var_SeventhO_SixthO_SetC);
			SD_SeventhO_SeventhO_SetC = sqrt((1./(NumOfSeventhO_SeventhO_SetC - 1.))*var_SeventhO_SeventhO_SetC);
			SD_SeventhO_EighthO_SetC = sqrt((1./(NumOfSeventhO_EighthO_SetC - 1.))*var_SeventhO_EighthO_SetC);
			SD_SeventhO_NinthO_SetC = sqrt((1./(NumOfSeventhO_NinthO_SetC - 1.))*var_SeventhO_NinthO_SetC);
			
			//EighthO as check mode//
			SD_EighthO_FU_SetC = sqrt((1./(NumOfEighthO_FU_SetC - 1.))*var_EighthO_FU_SetC);
			SD_EighthO_FO_SetC = sqrt((1./(NumOfEighthO_FO_SetC - 1.))*var_EighthO_FO_SetC);
			SD_EighthO_2O_SetC = sqrt((1./(NumOfEighthO_2O_SetC - 1.))*var_EighthO_2O_SetC);
			SD_EighthO_ThirdO_SetC = sqrt((1./(NumOfEighthO_ThirdO_SetC - 1.))*var_EighthO_ThirdO_SetC);
			SD_EighthO_ForthO_SetC = sqrt((1./(NumOfEighthO_ForthO_SetC - 1.))*var_EighthO_ForthO_SetC);
			SD_EighthO_FifthO_SetC = sqrt((1./(NumOfEighthO_FifthO_SetC - 1.))*var_EighthO_FifthO_SetC);
			SD_EighthO_SixthO_SetC = sqrt((1./(NumOfEighthO_SixthO_SetC - 1.))*var_EighthO_SixthO_SetC);
			SD_EighthO_SeventhO_SetC = sqrt((1./(NumOfEighthO_SeventhO_SetC - 1.))*var_EighthO_SeventhO_SetC);
			SD_EighthO_EighthO_SetC = sqrt((1./(NumOfEighthO_EighthO_SetC - 1.))*var_EighthO_EighthO_SetC);
			SD_EighthO_NinthO_SetC = sqrt((1./(NumOfEighthO_NinthO_SetC - 1.))*var_EighthO_NinthO_SetC);
			
			//NinthO as check mode//
			SD_NinthO_FU_SetC = sqrt((1./(NumOfNinthO_FU_SetC - 1.))*var_NinthO_FU_SetC);
			SD_NinthO_FO_SetC = sqrt((1./(NumOfNinthO_FO_SetC - 1.))*var_NinthO_FO_SetC);
			SD_NinthO_2O_SetC = sqrt((1./(NumOfNinthO_2O_SetC - 1.))*var_NinthO_2O_SetC);
			SD_NinthO_ThirdO_SetC = sqrt((1./(NumOfNinthO_ThirdO_SetC - 1.))*var_NinthO_ThirdO_SetC);
			SD_NinthO_ForthO_SetC = sqrt((1./(NumOfNinthO_ForthO_SetC - 1.))*var_NinthO_ForthO_SetC);
			SD_NinthO_FifthO_SetC = sqrt((1./(NumOfNinthO_FifthO_SetC - 1.))*var_NinthO_FifthO_SetC);
			SD_NinthO_SixthO_SetC = sqrt((1./(NumOfNinthO_SixthO_SetC - 1.))*var_NinthO_SixthO_SetC);
			SD_NinthO_SeventhO_SetC = sqrt((1./(NumOfNinthO_SeventhO_SetC - 1.))*var_NinthO_SeventhO_SetC);
			SD_NinthO_EighthO_SetC = sqrt((1./(NumOfNinthO_EighthO_SetC - 1.))*var_NinthO_EighthO_SetC);
			SD_NinthO_NinthO_SetC = sqrt((1./(NumOfNinthO_NinthO_SetC - 1.))*var_NinthO_NinthO_SetC);
			
			////Output for Num////
			outFile_Num<<setw(10)<<"FU: "<<setw(20)<<NumOfFU_FU_SetC<<setw(30)<<NumOfFO_FU_SetC<<setw(30)<<NumOf2O_FU_SetC<<setw(30)<<NumOfThirdO_FU_SetC<<setw(30)<<NumOfForthO_FU_SetC<<setw(30)<<NumOfFifthO_FU_SetC<<setw(30)<<NumOfSixthO_FU_SetC<<setw(30)<<NumOfSeventhO_FU_SetC<<setw(30)<<NumOfEighthO_FU_SetC<<setw(30)<<NumOfNinthO_FU_SetC<<endl;
			
			outFile_Num<<setw(10)<<"FO: "<<setw(20)<<NumOfFU_FO_SetC<<setw(30)<<NumOfFO_FO_SetC<<setw(30)<<NumOf2O_FO_SetC<<setw(30)<<NumOfThirdO_FO_SetC<<setw(30)<<NumOfForthO_FO_SetC<<setw(30)<<NumOfFifthO_FO_SetC<<setw(30)<<NumOfSixthO_FO_SetC<<setw(30)<<NumOfSeventhO_FO_SetC<<setw(30)<<NumOfEighthO_FO_SetC<<setw(30)<<NumOfNinthO_FO_SetC<<endl;
			
			outFile_Num<<setw(10)<<"2O: "<<setw(20)<<NumOfFU_2O_SetC<<setw(30)<<NumOfFO_2O_SetC<<setw(30)<<NumOf2O_2O_SetC<<setw(30)<<NumOfThirdO_2O_SetC<<setw(30)<<NumOfForthO_2O_SetC<<setw(30)<<NumOfFifthO_2O_SetC<<setw(30)<<NumOfSixthO_2O_SetC<<setw(30)<<NumOfSeventhO_2O_SetC<<setw(30)<<NumOfEighthO_2O_SetC<<setw(30)<<NumOfNinthO_2O_SetC<<endl;
			
			outFile_Num<<setw(10)<<"ThirdO: "<<setw(20)<<NumOfFU_ThirdO_SetC<<setw(30)<<NumOfFO_ThirdO_SetC<<setw(30)<<NumOf2O_ThirdO_SetC<<setw(30)<<NumOfThirdO_ThirdO_SetC<<setw(30)<<NumOfForthO_ThirdO_SetC<<setw(30)<<NumOfFifthO_ThirdO_SetC<<setw(30)<<NumOfSixthO_ThirdO_SetC<<setw(30)<<NumOfSeventhO_ThirdO_SetC<<setw(30)<<NumOfEighthO_ThirdO_SetC<<setw(30)<<NumOfNinthO_ThirdO_SetC<<endl;
			
			outFile_Num<<setw(10)<<"ForthO: "<<setw(20)<<NumOfFU_ForthO_SetC<<setw(30)<<NumOfFO_ForthO_SetC<<setw(30)<<NumOf2O_ForthO_SetC<<setw(30)<<NumOfThirdO_ForthO_SetC<<setw(30)<<NumOfForthO_ForthO_SetC<<setw(30)<<NumOfFifthO_ForthO_SetC<<setw(30)<<NumOfSixthO_ForthO_SetC<<setw(30)<<NumOfSeventhO_ForthO_SetC<<setw(30)<<NumOfEighthO_ForthO_SetC<<setw(30)<<NumOfNinthO_ForthO_SetC<<endl;
			
			outFile_Num<<setw(10)<<"FifthO: "<<setw(20)<<NumOfFU_FifthO_SetC<<setw(30)<<NumOfFO_FifthO_SetC<<setw(30)<<NumOf2O_FifthO_SetC<<setw(30)<<NumOfThirdO_FifthO_SetC<<setw(30)<<NumOfForthO_FifthO_SetC<<setw(30)<<NumOfFifthO_FifthO_SetC<<setw(30)<<NumOfSixthO_FifthO_SetC<<setw(30)<<NumOfSeventhO_FifthO_SetC<<setw(30)<<NumOfEighthO_FifthO_SetC<<setw(30)<<NumOfNinthO_FifthO_SetC<<endl;
			
			outFile_Num<<setw(10)<<"SixthO: "<<setw(20)<<NumOfFU_SixthO_SetC<<setw(30)<<NumOfFO_SixthO_SetC<<setw(30)<<NumOf2O_SixthO_SetC<<setw(30)<<NumOfThirdO_SixthO_SetC<<setw(30)<<NumOfForthO_SixthO_SetC<<setw(30)<<NumOfFifthO_SixthO_SetC<<setw(30)<<NumOfSixthO_SixthO_SetC<<setw(30)<<NumOfSeventhO_SixthO_SetC<<setw(30)<<NumOfEighthO_SixthO_SetC<<setw(30)<<NumOfNinthO_SixthO_SetC<<endl;
			
			outFile_Num<<setw(10)<<"SeventhO: "<<setw(20)<<NumOfFU_SeventhO_SetC<<setw(30)<<NumOfFO_SeventhO_SetC<<setw(30)<<NumOf2O_SeventhO_SetC<<setw(30)<<NumOfThirdO_SeventhO_SetC<<setw(30)<<NumOfForthO_SeventhO_SetC<<setw(30)<<NumOfFifthO_SeventhO_SetC<<setw(30)<<NumOfSixthO_SeventhO_SetC<<setw(30)<<NumOfSeventhO_SeventhO_SetC<<setw(30)<<NumOfEighthO_SeventhO_SetC<<setw(30)<<NumOfNinthO_SeventhO_SetC<<endl;
			
			outFile_Num<<setw(10)<<"EighthO: "<<setw(20)<<NumOfFU_EighthO_SetC<<setw(30)<<NumOfFO_EighthO_SetC<<setw(30)<<NumOf2O_EighthO_SetC<<setw(30)<<NumOfThirdO_EighthO_SetC<<setw(30)<<NumOfForthO_EighthO_SetC<<setw(30)<<NumOfFifthO_EighthO_SetC<<setw(30)<<NumOfSixthO_EighthO_SetC<<setw(30)<<NumOfSeventhO_EighthO_SetC<<setw(30)<<NumOfEighthO_EighthO_SetC<<setw(30)<<NumOfNinthO_EighthO_SetC<<endl;
			
			outFile_Num<<setw(10)<<"NinthO: "<<setw(20)<<NumOfFU_NinthO_SetC<<setw(30)<<NumOfFO_NinthO_SetC<<setw(30)<<NumOf2O_NinthO_SetC<<setw(30)<<NumOfThirdO_NinthO_SetC<<setw(30)<<NumOfForthO_NinthO_SetC<<setw(30)<<NumOfFifthO_NinthO_SetC<<setw(30)<<NumOfSixthO_NinthO_SetC<<setw(30)<<NumOfSeventhO_NinthO_SetC<<setw(30)<<NumOfEighthO_NinthO_SetC<<setw(30)<<NumOfNinthO_NinthO_SetC<<endl;
			
			
			////Output for SD calcs////
			outFile_SD<<setw(10)<<"FU: "<<setw(20)<<SD_FU_FU_SetC<<setw(30)<<SD_FO_FU_SetC<<setw(30)<<SD_2O_FU_SetC<<setw(30)<<SD_ThirdO_FU_SetC<<setw(30)<<SD_ForthO_FU_SetC<<setw(30)<<SD_FifthO_FU_SetC<<setw(30)<<SD_SixthO_FU_SetC<<setw(30)<<SD_SeventhO_FU_SetC<<setw(30)<<SD_EighthO_FU_SetC<<setw(30)<<SD_NinthO_FU_SetC<<endl;
			
			outFile_SD<<setw(10)<<"FO: "<<setw(20)<<SD_FU_FO_SetC<<setw(30)<<SD_FO_FO_SetC<<setw(30)<<SD_2O_FO_SetC<<setw(30)<<SD_ThirdO_FO_SetC<<setw(30)<<SD_ForthO_FO_SetC<<setw(30)<<SD_FifthO_FO_SetC<<setw(30)<<SD_SixthO_FO_SetC<<setw(30)<<SD_SeventhO_FO_SetC<<setw(30)<<SD_EighthO_FO_SetC<<setw(30)<<SD_NinthO_FO_SetC<<endl;
			
			outFile_SD<<setw(10)<<"2O: "<<setw(20)<<SD_FU_2O_SetC<<setw(30)<<SD_FO_2O_SetC<<setw(30)<<SD_2O_2O_SetC<<setw(30)<<SD_ThirdO_2O_SetC<<setw(30)<<SD_ForthO_2O_SetC<<setw(30)<<SD_FifthO_2O_SetC<<setw(30)<<SD_SixthO_2O_SetC<<setw(30)<<SD_SeventhO_2O_SetC<<setw(30)<<SD_EighthO_2O_SetC<<setw(30)<<SD_NinthO_2O_SetC<<endl;
			
			outFile_SD<<setw(10)<<"ThirdO: "<<setw(20)<<SD_FU_ThirdO_SetC<<setw(30)<<SD_FO_ThirdO_SetC<<setw(30)<<SD_2O_ThirdO_SetC<<setw(30)<<SD_ThirdO_ThirdO_SetC<<setw(30)<<SD_ForthO_ThirdO_SetC<<setw(30)<<SD_FifthO_ThirdO_SetC<<setw(30)<<SD_SixthO_ThirdO_SetC<<setw(30)<<SD_SeventhO_ThirdO_SetC<<setw(30)<<SD_EighthO_ThirdO_SetC<<setw(30)<<SD_NinthO_ThirdO_SetC<<endl;
			
			outFile_SD<<setw(10)<<"ForthO: "<<setw(20)<<SD_FU_ForthO_SetC<<setw(30)<<SD_FO_ForthO_SetC<<setw(30)<<SD_2O_ForthO_SetC<<setw(30)<<SD_ThirdO_ForthO_SetC<<setw(30)<<SD_ForthO_ForthO_SetC<<setw(30)<<SD_FifthO_ForthO_SetC<<setw(30)<<SD_SixthO_ForthO_SetC<<setw(30)<<SD_SeventhO_ForthO_SetC<<setw(30)<<SD_EighthO_ForthO_SetC<<setw(30)<<SD_NinthO_ForthO_SetC<<endl;
			
			outFile_SD<<setw(10)<<"FifthO: "<<setw(20)<<SD_FU_FifthO_SetC<<setw(30)<<SD_FO_FifthO_SetC<<setw(30)<<SD_2O_FifthO_SetC<<setw(30)<<SD_ThirdO_FifthO_SetC<<setw(30)<<SD_ForthO_FifthO_SetC<<setw(30)<<SD_FifthO_FifthO_SetC<<setw(30)<<SD_SixthO_FifthO_SetC<<setw(30)<<SD_SeventhO_FifthO_SetC<<setw(30)<<SD_EighthO_FifthO_SetC<<setw(30)<<SD_NinthO_FifthO_SetC<<endl;
			
			outFile_SD<<setw(10)<<"SixthO: "<<setw(20)<<SD_FU_SixthO_SetC<<setw(30)<<SD_FO_SixthO_SetC<<setw(30)<<SD_2O_SixthO_SetC<<setw(30)<<SD_ThirdO_SixthO_SetC<<setw(30)<<SD_ForthO_SixthO_SetC<<setw(30)<<SD_FifthO_SixthO_SetC<<setw(30)<<SD_SixthO_SixthO_SetC<<setw(30)<<SD_SeventhO_SixthO_SetC<<setw(30)<<SD_EighthO_SixthO_SetC<<setw(30)<<SD_NinthO_SixthO_SetC<<endl;
			
			outFile_SD<<setw(10)<<"SeventhO: "<<setw(20)<<SD_FU_SeventhO_SetC<<setw(30)<<SD_FO_SeventhO_SetC<<setw(30)<<SD_2O_SeventhO_SetC<<setw(30)<<SD_ThirdO_SeventhO_SetC<<setw(30)<<SD_ForthO_SeventhO_SetC<<setw(30)<<SD_FifthO_SeventhO_SetC<<setw(30)<<SD_SixthO_SeventhO_SetC<<setw(30)<<SD_SeventhO_SeventhO_SetC<<setw(30)<<SD_EighthO_SeventhO_SetC<<setw(30)<<SD_NinthO_SeventhO_SetC<<endl;
			
			outFile_SD<<setw(10)<<"EighthO: "<<setw(20)<<SD_FU_EighthO_SetC<<setw(30)<<SD_FO_EighthO_SetC<<setw(30)<<SD_2O_EighthO_SetC<<setw(30)<<SD_ThirdO_EighthO_SetC<<setw(30)<<SD_ForthO_EighthO_SetC<<setw(30)<<SD_FifthO_EighthO_SetC<<setw(30)<<SD_SixthO_EighthO_SetC<<setw(30)<<SD_SeventhO_EighthO_SetC<<setw(30)<<SD_EighthO_EighthO_SetC<<setw(30)<<SD_NinthO_EighthO_SetC<<endl;
			
			outFile_SD<<setw(10)<<"NinthO: "<<setw(20)<<SD_FU_NinthO_SetC<<setw(30)<<SD_FO_NinthO_SetC<<setw(30)<<SD_2O_NinthO_SetC<<setw(30)<<SD_ThirdO_NinthO_SetC<<setw(30)<<SD_ForthO_NinthO_SetC<<setw(30)<<SD_FifthO_NinthO_SetC<<setw(30)<<SD_SixthO_NinthO_SetC<<setw(30)<<SD_SeventhO_NinthO_SetC<<setw(30)<<SD_EighthO_NinthO_SetC<<setw(30)<<SD_NinthO_NinthO_SetC<<endl;
			
			
			////Output for resonance ratios////		
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
			
			////output for fig5////
			for(int i = 0; i < NumOfModels; i++){
				if(Mass[i] == Mass_ToLookFor && L[i] == L_ToLookFor && ModelRan_SetC[i]){
					double frequency_FU = 1./period_FU_SetC[i];
					double frequency_FO = 1./period_FO_SetC[i];
					double frequency_2O = 1./period_2O_SetC[i];
					double frequency_ThirdO = 1./period_ThirdO_SetC[i];
					double frequency_ForthO = 1./period_ForthO_SetC[i];
					outFile_Fig5_FU_FO<<setw(10)<<Teff[i]<<setw(20)<<frequency_FU<<setw(20)<<frequency_FO<<setw(20)<<frequency_2O<<setw(20)<<frequency_ThirdO<<setw(20)<<frequency_ForthO<<setw(20)<<growth_FU_SetC[i]<<setw(20)<<growth_FO_SetC[i]<<setw(20)<<growth_2O_SetC[i]<<setw(20)<<growth_ThirdO_SetC[i]<<setw(20)<<growth_ForthO_SetC[i]<<endl;
				}
			}
		}
		if(j == 3){
			////SD Calc Part I (To get var)////
			for(int i = 0; i < NumOfModels; i++){
				double mean;
				//FU as check//
				if(IsSelected_FU_SetD[i]){
					mean = (OverallResonance_FU_FU_SetD/NumOfFU_FU_SetD);
					var_FU_FU_SetD = var_FU_FU_SetD + pow((ResonanceRatio_FU_FU_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FU_FO_SetD/NumOfFU_FO_SetD);
					var_FU_FO_SetD = var_FU_FO_SetD + pow((ResonanceRatio_FU_FO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FU_2O_SetD/NumOfFU_2O_SetD);
					var_FU_2O_SetD = var_FU_2O_SetD + pow((ResonanceRatio_FU_2O_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FU_ThirdO_SetD/NumOfFU_ThirdO_SetD);
					var_FU_ThirdO_SetD = var_FU_ThirdO_SetD + pow((ResonanceRatio_FU_ThirdO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FU_ForthO_SetD/NumOfFU_ForthO_SetD);
					var_FU_ForthO_SetD = var_FU_ForthO_SetD + pow((ResonanceRatio_FU_ForthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FU_FifthO_SetD/NumOfFU_FifthO_SetD);
					var_FU_FifthO_SetD = var_FU_FifthO_SetD + pow((ResonanceRatio_FU_FifthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FU_SixthO_SetD/NumOfFU_SixthO_SetD);
					var_FU_SixthO_SetD = var_FU_SixthO_SetD + pow((ResonanceRatio_FU_SixthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FU_SeventhO_SetD/NumOfFU_SeventhO_SetD);
					var_FU_SeventhO_SetD = var_FU_SeventhO_SetD + pow((ResonanceRatio_FU_SeventhO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FU_EighthO_SetD/NumOfFU_EighthO_SetD);
					var_FU_EighthO_SetD = var_FU_EighthO_SetD + pow((ResonanceRatio_FU_EighthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FU_NinthO_SetD/NumOfFU_NinthO_SetD);
					var_FU_NinthO_SetD = var_FU_NinthO_SetD + pow((ResonanceRatio_FU_NinthO_SetD[i] - mean),2.);
				}
							
				//FO//			
				if(IsSelected_FO_SetD[i]){
					mean = (OverallResonance_FO_FU_SetD/NumOfFO_FU_SetD);
					var_FO_FU_SetD = var_FO_FU_SetD + pow((ResonanceRatio_FO_FU_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FO_FO_SetD/NumOfFO_FO_SetD);
					var_FO_FO_SetD = var_FO_FO_SetD + pow((ResonanceRatio_FO_FO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FO_2O_SetD/NumOfFO_2O_SetD);
					var_FO_2O_SetD = var_FO_2O_SetD + pow((ResonanceRatio_FO_2O_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FO_ThirdO_SetD/NumOfFO_ThirdO_SetD);
					var_FO_ThirdO_SetD = var_FO_ThirdO_SetD + pow((ResonanceRatio_FO_ThirdO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FO_ForthO_SetD/NumOfFO_ForthO_SetD);
					var_FO_ForthO_SetD = var_FO_ForthO_SetD + pow((ResonanceRatio_FO_ForthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FO_FifthO_SetD/NumOfFO_FifthO_SetD);
					var_FO_FifthO_SetD = var_FO_FifthO_SetD + pow((ResonanceRatio_FO_FifthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FO_SixthO_SetD/NumOfFO_SixthO_SetD);
					var_FO_SixthO_SetD = var_FO_SixthO_SetD + pow((ResonanceRatio_FO_SixthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FO_SeventhO_SetD/NumOfFO_SeventhO_SetD);
					var_FO_SeventhO_SetD = var_FO_SeventhO_SetD + pow((ResonanceRatio_FO_SeventhO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FO_EighthO_SetD/NumOfFO_EighthO_SetD);
					var_FO_EighthO_SetD = var_FO_EighthO_SetD + pow((ResonanceRatio_FO_EighthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FO_NinthO_SetD/NumOfFO_NinthO_SetD);
					var_FO_NinthO_SetD = var_FO_NinthO_SetD + pow((ResonanceRatio_FO_NinthO_SetD[i] - mean),2.);
				}
			
				//2O//
				if(IsSelected_2O_SetD[i]){
					mean = (OverallResonance_2O_FU_SetD/NumOf2O_FU_SetD);
					var_2O_FU_SetD = var_2O_FU_SetD + pow((ResonanceRatio_2O_FU_SetD[i] - mean),2.);
					
					mean = (OverallResonance_2O_FO_SetD/NumOf2O_FO_SetD);
					var_2O_FO_SetD = var_2O_FO_SetD + pow((ResonanceRatio_2O_FO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_2O_2O_SetD/NumOf2O_2O_SetD);
					var_2O_2O_SetD = var_2O_2O_SetD + pow((ResonanceRatio_2O_2O_SetD[i] - mean),2.);
					
					mean = (OverallResonance_2O_ThirdO_SetD/NumOf2O_ThirdO_SetD);
					var_2O_ThirdO_SetD = var_2O_ThirdO_SetD + pow((ResonanceRatio_2O_ThirdO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_2O_ForthO_SetD/NumOf2O_ForthO_SetD);
					var_2O_ForthO_SetD = var_2O_ForthO_SetD + pow((ResonanceRatio_2O_ForthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_2O_FifthO_SetD/NumOf2O_FifthO_SetD);
					var_2O_FifthO_SetD = var_2O_FifthO_SetD + pow((ResonanceRatio_2O_FifthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_2O_SixthO_SetD/NumOf2O_SixthO_SetD);
					var_2O_SixthO_SetD = var_2O_SixthO_SetD + pow((ResonanceRatio_2O_SixthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_2O_SeventhO_SetD/NumOf2O_SeventhO_SetD);
					var_2O_SeventhO_SetD = var_2O_SeventhO_SetD + pow((ResonanceRatio_2O_SeventhO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_2O_EighthO_SetD/NumOf2O_EighthO_SetD);
					var_2O_EighthO_SetD = var_2O_EighthO_SetD + pow((ResonanceRatio_2O_EighthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_2O_NinthO_SetD/NumOf2O_NinthO_SetD);
					var_2O_NinthO_SetD = var_2O_NinthO_SetD + pow((ResonanceRatio_2O_NinthO_SetD[i] - mean),2.);
				}
				
				//ThirdO//
				if(IsSelected_ThirdO_SetD[i]){
					mean = (OverallResonance_ThirdO_FU_SetD/NumOfThirdO_FU_SetD);
					var_ThirdO_FU_SetD = var_ThirdO_FU_SetD + pow((ResonanceRatio_ThirdO_FU_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_FO_SetD/NumOfThirdO_FO_SetD);
					var_ThirdO_FO_SetD = var_ThirdO_FO_SetD + pow((ResonanceRatio_ThirdO_FO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_2O_SetD/NumOfThirdO_2O_SetD);
					var_ThirdO_2O_SetD = var_ThirdO_2O_SetD + pow((ResonanceRatio_ThirdO_2O_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_ThirdO_SetD/NumOfThirdO_ThirdO_SetD);
					var_ThirdO_ThirdO_SetD = var_ThirdO_ThirdO_SetD + pow((ResonanceRatio_ThirdO_ThirdO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_ForthO_SetD/NumOfThirdO_ForthO_SetD);
					var_ThirdO_ForthO_SetD = var_ThirdO_ForthO_SetD + pow((ResonanceRatio_ThirdO_ForthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_FifthO_SetD/NumOfThirdO_FifthO_SetD);
					var_ThirdO_FifthO_SetD = var_ThirdO_FifthO_SetD + pow((ResonanceRatio_ThirdO_FifthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_SixthO_SetD/NumOfThirdO_SixthO_SetD);
					var_ThirdO_SixthO_SetD = var_ThirdO_SixthO_SetD + pow((ResonanceRatio_ThirdO_SixthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_SeventhO_SetD/NumOfThirdO_SeventhO_SetD);
					var_ThirdO_SeventhO_SetD = var_ThirdO_SeventhO_SetD + pow((ResonanceRatio_ThirdO_SeventhO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_EighthO_SetD/NumOfThirdO_EighthO_SetD);
					var_ThirdO_EighthO_SetD = var_ThirdO_EighthO_SetD + pow((ResonanceRatio_ThirdO_EighthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ThirdO_NinthO_SetD/NumOfThirdO_NinthO_SetD);
					var_ThirdO_NinthO_SetD = var_ThirdO_NinthO_SetD + pow((ResonanceRatio_ThirdO_NinthO_SetD[i] - mean),2.);
				}
			
				//ForthO//
				if(IsSelected_ForthO_SetD[i]){
					mean = (OverallResonance_ForthO_FU_SetD/NumOfForthO_FU_SetD);
					var_ForthO_FU_SetD = var_ForthO_FU_SetD + pow((ResonanceRatio_ForthO_FU_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_FO_SetD/NumOfForthO_FO_SetD);
					var_ForthO_FO_SetD = var_ForthO_FO_SetD + pow((ResonanceRatio_ForthO_FO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_2O_SetD/NumOfForthO_2O_SetD);
					var_ForthO_2O_SetD = var_ForthO_2O_SetD + pow((ResonanceRatio_ForthO_2O_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_ThirdO_SetD/NumOfForthO_ThirdO_SetD);
					var_ForthO_ThirdO_SetD = var_ForthO_ThirdO_SetD + pow((ResonanceRatio_ForthO_ThirdO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_ForthO_SetD/NumOfForthO_ForthO_SetD);
					var_ForthO_ForthO_SetD = var_ForthO_ForthO_SetD + pow((ResonanceRatio_ForthO_ForthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_FifthO_SetD/NumOfForthO_FifthO_SetD);
					var_ForthO_FifthO_SetD = var_ForthO_FifthO_SetD + pow((ResonanceRatio_ForthO_FifthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_SixthO_SetD/NumOfForthO_SixthO_SetD);
					var_ForthO_SixthO_SetD = var_ForthO_SixthO_SetD + pow((ResonanceRatio_ForthO_SixthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_SeventhO_SetD/NumOfForthO_SeventhO_SetD);
					var_ForthO_SeventhO_SetD = var_ForthO_SeventhO_SetD + pow((ResonanceRatio_ForthO_SeventhO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_EighthO_SetD/NumOfForthO_EighthO_SetD);
					var_ForthO_EighthO_SetD = var_ForthO_EighthO_SetD + pow((ResonanceRatio_ForthO_EighthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_ForthO_NinthO_SetD/NumOfForthO_NinthO_SetD);
					var_ForthO_NinthO_SetD = var_ForthO_NinthO_SetD + pow((ResonanceRatio_ForthO_NinthO_SetD[i] - mean),2.);
				}
					
				//FifthO//
				if(IsSelected_FifthO_SetD[i]){
					mean = (OverallResonance_FifthO_FU_SetD/NumOfFifthO_FU_SetD);
					var_FifthO_FU_SetD = var_FifthO_FU_SetD + pow((ResonanceRatio_FifthO_FU_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_FO_SetD/NumOfFifthO_FO_SetD);
					var_FifthO_FO_SetD = var_FifthO_FO_SetD + pow((ResonanceRatio_FifthO_FO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_2O_SetD/NumOfFifthO_2O_SetD);
					var_FifthO_2O_SetD = var_FifthO_2O_SetD + pow((ResonanceRatio_FifthO_2O_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_ThirdO_SetD/NumOfFifthO_ThirdO_SetD);
					var_FifthO_ThirdO_SetD = var_FifthO_ThirdO_SetD + pow((ResonanceRatio_FifthO_ThirdO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_ForthO_SetD/NumOfFifthO_ForthO_SetD);
					var_FifthO_ForthO_SetD = var_FifthO_ForthO_SetD + pow((ResonanceRatio_FifthO_ForthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_FifthO_SetD/NumOfFifthO_FifthO_SetD);
					var_FifthO_FifthO_SetD = var_FifthO_FifthO_SetD + pow((ResonanceRatio_FifthO_FifthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_SixthO_SetD/NumOfFifthO_SixthO_SetD);
					var_FifthO_SixthO_SetD = var_FifthO_SixthO_SetD + pow((ResonanceRatio_FifthO_SixthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_SeventhO_SetD/NumOfFifthO_SeventhO_SetD);
					var_FifthO_SeventhO_SetD = var_FifthO_SeventhO_SetD + pow((ResonanceRatio_FifthO_SeventhO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_EighthO_SetD/NumOfFifthO_EighthO_SetD);
					var_FifthO_EighthO_SetD = var_FifthO_EighthO_SetD + pow((ResonanceRatio_FifthO_EighthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_FifthO_NinthO_SetD/NumOfFifthO_NinthO_SetD);
					var_FifthO_NinthO_SetD = var_FifthO_NinthO_SetD + pow((ResonanceRatio_FifthO_NinthO_SetD[i] - mean),2.);
				}
			
				//SixthO//
				if(IsSelected_SixthO_SetD[i]){
					mean = (OverallResonance_SixthO_FU_SetD/NumOfSixthO_FU_SetD);
					var_SixthO_FU_SetD = var_SixthO_FU_SetD + pow((ResonanceRatio_SixthO_FU_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_FO_SetD/NumOfSixthO_FO_SetD);
					var_SixthO_FO_SetD = var_SixthO_FO_SetD + pow((ResonanceRatio_SixthO_FO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_2O_SetD/NumOfSixthO_2O_SetD);
					var_SixthO_2O_SetD = var_SixthO_2O_SetD + pow((ResonanceRatio_SixthO_2O_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_ThirdO_SetD/NumOfSixthO_ThirdO_SetD);
					var_SixthO_ThirdO_SetD = var_SixthO_ThirdO_SetD + pow((ResonanceRatio_SixthO_ThirdO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_ForthO_SetD/NumOfSixthO_ForthO_SetD);
					var_SixthO_ForthO_SetD = var_SixthO_ForthO_SetD + pow((ResonanceRatio_SixthO_ForthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_FifthO_SetD/NumOfSixthO_FifthO_SetD);
					var_SixthO_FifthO_SetD = var_SixthO_FifthO_SetD + pow((ResonanceRatio_SixthO_FifthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_SixthO_SetD/NumOfSixthO_SixthO_SetD);
					var_SixthO_SixthO_SetD = var_SixthO_SixthO_SetD + pow((ResonanceRatio_SixthO_SixthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_SeventhO_SetD/NumOfSixthO_SeventhO_SetD);
					var_SixthO_SeventhO_SetD = var_SixthO_SeventhO_SetD + pow((ResonanceRatio_SixthO_SeventhO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_EighthO_SetD/NumOfSixthO_EighthO_SetD);
					var_SixthO_EighthO_SetD = var_SixthO_EighthO_SetD + pow((ResonanceRatio_SixthO_EighthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SixthO_NinthO_SetD/NumOfSixthO_NinthO_SetD);
					var_SixthO_NinthO_SetD = var_SixthO_NinthO_SetD + pow((ResonanceRatio_SixthO_NinthO_SetD[i] - mean),2.);
				}
			
				//SeventhO//
				if(IsSelected_SeventhO_SetD[i]){
					mean = (OverallResonance_SeventhO_FU_SetD/NumOfSeventhO_FU_SetD);
					var_SeventhO_FU_SetD = var_SeventhO_FU_SetD + pow((ResonanceRatio_SeventhO_FU_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_FO_SetD/NumOfSeventhO_FO_SetD);
					var_SeventhO_FO_SetD = var_SeventhO_FO_SetD + pow((ResonanceRatio_SeventhO_FO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_2O_SetD/NumOfSeventhO_2O_SetD);
					var_SeventhO_2O_SetD = var_SeventhO_2O_SetD + pow((ResonanceRatio_SeventhO_2O_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_ThirdO_SetD/NumOfSeventhO_ThirdO_SetD);
					var_SeventhO_ThirdO_SetD = var_SeventhO_ThirdO_SetD + pow((ResonanceRatio_SeventhO_ThirdO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_ForthO_SetD/NumOfSeventhO_ForthO_SetD);
					var_SeventhO_ForthO_SetD = var_SeventhO_ForthO_SetD + pow((ResonanceRatio_SeventhO_ForthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_FifthO_SetD/NumOfSeventhO_FifthO_SetD);
					var_SeventhO_FifthO_SetD = var_SeventhO_FifthO_SetD + pow((ResonanceRatio_SeventhO_FifthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_SixthO_SetD/NumOfSeventhO_SixthO_SetD);
					var_SeventhO_SixthO_SetD = var_SeventhO_SixthO_SetD + pow((ResonanceRatio_SeventhO_SixthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_SeventhO_SetD/NumOfSeventhO_SeventhO_SetD);
					var_SeventhO_SeventhO_SetD = var_SeventhO_SeventhO_SetD + pow((ResonanceRatio_SeventhO_SeventhO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_EighthO_SetD/NumOfSeventhO_EighthO_SetD);
					var_SeventhO_EighthO_SetD = var_SeventhO_EighthO_SetD + pow((ResonanceRatio_SeventhO_EighthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_SeventhO_NinthO_SetD/NumOfSeventhO_NinthO_SetD);
					var_SeventhO_NinthO_SetD = var_SeventhO_NinthO_SetD + pow((ResonanceRatio_SeventhO_NinthO_SetD[i] - mean),2.);
				}
			
				//EighthO//
				if(IsSelected_EighthO_SetD[i]){
					mean = (OverallResonance_EighthO_FU_SetD/NumOfEighthO_FU_SetD);
					var_EighthO_FU_SetD = var_EighthO_FU_SetD + pow((ResonanceRatio_EighthO_FU_SetD[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_FO_SetD/NumOfEighthO_FO_SetD);
					var_EighthO_FO_SetD = var_EighthO_FO_SetD + pow((ResonanceRatio_EighthO_FO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_2O_SetD/NumOfEighthO_2O_SetD);
					var_EighthO_2O_SetD = var_EighthO_2O_SetD + pow((ResonanceRatio_EighthO_2O_SetD[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_ThirdO_SetD/NumOfEighthO_ThirdO_SetD);
					var_EighthO_ThirdO_SetD = var_EighthO_ThirdO_SetD + pow((ResonanceRatio_EighthO_ThirdO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_ForthO_SetD/NumOfEighthO_ForthO_SetD);
					var_EighthO_ForthO_SetD = var_EighthO_ForthO_SetD + pow((ResonanceRatio_EighthO_ForthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_FifthO_SetD/NumOfEighthO_FifthO_SetD);
					var_EighthO_FifthO_SetD = var_EighthO_FifthO_SetD + pow((ResonanceRatio_EighthO_FifthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_SixthO_SetD/NumOfEighthO_SixthO_SetD);
					var_EighthO_SixthO_SetD = var_EighthO_SixthO_SetD + pow((ResonanceRatio_EighthO_SixthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_SeventhO_SetD/NumOfEighthO_SeventhO_SetD);
					var_EighthO_SeventhO_SetD = var_EighthO_SeventhO_SetD + pow((ResonanceRatio_EighthO_SeventhO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_EighthO_SetD/NumOfEighthO_EighthO_SetD);
					var_EighthO_EighthO_SetD = var_EighthO_EighthO_SetD + pow((ResonanceRatio_EighthO_EighthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_EighthO_NinthO_SetD/NumOfEighthO_NinthO_SetD);
					var_EighthO_NinthO_SetD = var_EighthO_NinthO_SetD + pow((ResonanceRatio_EighthO_NinthO_SetD[i] - mean),2.);
				}
				
				//NinthO//
				if(IsSelected_NinthO_SetD[i]){
					mean = (OverallResonance_NinthO_FU_SetD/NumOfNinthO_FU_SetD);
					var_NinthO_FU_SetD = var_NinthO_FU_SetD + pow((ResonanceRatio_NinthO_FU_SetD[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_FO_SetD/NumOfNinthO_FO_SetD);
					var_NinthO_FO_SetD = var_NinthO_FO_SetD + pow((ResonanceRatio_NinthO_FO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_2O_SetD/NumOfNinthO_2O_SetD);
					var_NinthO_2O_SetD = var_NinthO_2O_SetD + pow((ResonanceRatio_NinthO_2O_SetD[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_ThirdO_SetD/NumOfNinthO_ThirdO_SetD);
					var_NinthO_ThirdO_SetD = var_NinthO_ThirdO_SetD + pow((ResonanceRatio_NinthO_ThirdO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_ForthO_SetD/NumOfNinthO_ForthO_SetD);
					var_NinthO_ForthO_SetD = var_NinthO_ForthO_SetD + pow((ResonanceRatio_NinthO_ForthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_FifthO_SetD/NumOfNinthO_FifthO_SetD);
					var_NinthO_FifthO_SetD = var_NinthO_FifthO_SetD + pow((ResonanceRatio_NinthO_FifthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_SixthO_SetD/NumOfNinthO_SixthO_SetD);
					var_NinthO_SixthO_SetD = var_NinthO_SixthO_SetD + pow((ResonanceRatio_NinthO_SixthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_SeventhO_SetD/NumOfNinthO_SeventhO_SetD);
					var_NinthO_SeventhO_SetD = var_NinthO_SeventhO_SetD + pow((ResonanceRatio_NinthO_SeventhO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_EighthO_SetD/NumOfNinthO_EighthO_SetD);
					var_NinthO_EighthO_SetD = var_NinthO_EighthO_SetD + pow((ResonanceRatio_NinthO_EighthO_SetD[i] - mean),2.);
					
					mean = (OverallResonance_NinthO_NinthO_SetD/NumOfNinthO_NinthO_SetD);
					var_NinthO_NinthO_SetD = var_NinthO_NinthO_SetD + pow((ResonanceRatio_NinthO_NinthO_SetD[i] - mean),2.);
				}
					
			}
			
			////SD Calc Part II (To get final SD)////
			//FU as check mode//
			SD_FU_FU_SetD = sqrt((1./(NumOfFU_FU_SetD - 1.))*var_FU_FU_SetD);
			SD_FU_FO_SetD = sqrt((1./(NumOfFU_FO_SetD - 1.))*var_FU_FO_SetD);
			SD_FU_2O_SetD = sqrt((1./(NumOfFU_2O_SetD - 1.))*var_FU_2O_SetD);
			SD_FU_ThirdO_SetD = sqrt((1./(NumOfFU_ThirdO_SetD - 1.))*var_FU_ThirdO_SetD);
			SD_FU_ForthO_SetD = sqrt((1./(NumOfFU_ForthO_SetD - 1.))*var_FU_ForthO_SetD);
			SD_FU_FifthO_SetD = sqrt((1./(NumOfFU_FifthO_SetD - 1.))*var_FU_FifthO_SetD);
			SD_FU_SixthO_SetD = sqrt((1./(NumOfFU_SixthO_SetD - 1.))*var_FU_SixthO_SetD);
			SD_FU_SeventhO_SetD = sqrt((1./(NumOfFU_SeventhO_SetD - 1.))*var_FU_SeventhO_SetD);
			SD_FU_EighthO_SetD = sqrt((1./(NumOfFU_EighthO_SetD - 1.))*var_FU_EighthO_SetD);
			SD_FU_NinthO_SetD = sqrt((1./(NumOfFU_NinthO_SetD - 1.))*var_FU_NinthO_SetD);
			
			//FO as check mode//
			SD_FO_FU_SetD = sqrt((1./(NumOfFO_FU_SetD - 1.))*var_FO_FU_SetD);
			SD_FO_FO_SetD = sqrt((1./(NumOfFO_FO_SetD - 1.))*var_FO_FO_SetD);
			SD_FO_2O_SetD = sqrt((1./(NumOfFO_2O_SetD - 1.))*var_FO_2O_SetD);
			SD_FO_ThirdO_SetD = sqrt((1./(NumOfFO_ThirdO_SetD - 1.))*var_FO_ThirdO_SetD);
			SD_FO_ForthO_SetD = sqrt((1./(NumOfFO_ForthO_SetD - 1.))*var_FO_ForthO_SetD);
			SD_FO_FifthO_SetD = sqrt((1./(NumOfFO_FifthO_SetD - 1.))*var_FO_FifthO_SetD);
			SD_FO_SixthO_SetD = sqrt((1./(NumOfFO_SixthO_SetD - 1.))*var_FO_SixthO_SetD);
			SD_FO_SeventhO_SetD = sqrt((1./(NumOfFO_SeventhO_SetD - 1.))*var_FO_SeventhO_SetD);
			SD_FO_EighthO_SetD = sqrt((1./(NumOfFO_EighthO_SetD - 1.))*var_FO_EighthO_SetD);
			SD_FO_NinthO_SetD = sqrt((1./(NumOfFO_NinthO_SetD - 1.))*var_FO_NinthO_SetD);
			
			//2O as check mode//
			SD_2O_FU_SetD = sqrt((1./(NumOf2O_FU_SetD - 1.))*var_2O_FU_SetD);
			SD_2O_FO_SetD = sqrt((1./(NumOf2O_FO_SetD - 1.))*var_2O_FO_SetD);
			SD_2O_2O_SetD = sqrt((1./(NumOf2O_2O_SetD - 1.))*var_2O_2O_SetD);
			SD_2O_ThirdO_SetD = sqrt((1./(NumOf2O_ThirdO_SetD - 1.))*var_2O_ThirdO_SetD);
			SD_2O_ForthO_SetD = sqrt((1./(NumOf2O_ForthO_SetD - 1.))*var_2O_ForthO_SetD);
			SD_2O_FifthO_SetD = sqrt((1./(NumOf2O_FifthO_SetD - 1.))*var_2O_FifthO_SetD);
			SD_2O_SixthO_SetD = sqrt((1./(NumOf2O_SixthO_SetD - 1.))*var_2O_SixthO_SetD);
			SD_2O_SeventhO_SetD = sqrt((1./(NumOf2O_SeventhO_SetD - 1.))*var_2O_SeventhO_SetD);
			SD_2O_EighthO_SetD = sqrt((1./(NumOf2O_EighthO_SetD - 1.))*var_2O_EighthO_SetD);
			SD_2O_NinthO_SetD = sqrt((1./(NumOf2O_NinthO_SetD - 1.))*var_2O_NinthO_SetD);
			
			//ThirdO as check mode//
			SD_ThirdO_FU_SetD = sqrt((1./(NumOfThirdO_FU_SetD - 1.))*var_ThirdO_FU_SetD);
			SD_ThirdO_FO_SetD = sqrt((1./(NumOfThirdO_FO_SetD - 1.))*var_ThirdO_FO_SetD);
			SD_ThirdO_2O_SetD = sqrt((1./(NumOfThirdO_2O_SetD - 1.))*var_ThirdO_2O_SetD);
			SD_ThirdO_ThirdO_SetD = sqrt((1./(NumOfThirdO_ThirdO_SetD - 1.))*var_ThirdO_ThirdO_SetD);
			SD_ThirdO_ForthO_SetD = sqrt((1./(NumOfThirdO_ForthO_SetD - 1.))*var_ThirdO_ForthO_SetD);
			SD_ThirdO_FifthO_SetD = sqrt((1./(NumOfThirdO_FifthO_SetD - 1.))*var_ThirdO_FifthO_SetD);
			SD_ThirdO_SixthO_SetD = sqrt((1./(NumOfThirdO_SixthO_SetD - 1.))*var_ThirdO_SixthO_SetD);
			SD_ThirdO_SeventhO_SetD = sqrt((1./(NumOfThirdO_SeventhO_SetD - 1.))*var_ThirdO_SeventhO_SetD);
			SD_ThirdO_EighthO_SetD = sqrt((1./(NumOfThirdO_EighthO_SetD - 1.))*var_ThirdO_EighthO_SetD);
			SD_ThirdO_NinthO_SetD = sqrt((1./(NumOfThirdO_NinthO_SetD - 1.))*var_ThirdO_NinthO_SetD);
			
			//ForthO as check mode//
			SD_ForthO_FU_SetD = sqrt((1./(NumOfForthO_FU_SetD - 1.))*var_ForthO_FU_SetD);
			SD_ForthO_FO_SetD = sqrt((1./(NumOfForthO_FO_SetD - 1.))*var_ForthO_FO_SetD);
			SD_ForthO_2O_SetD = sqrt((1./(NumOfForthO_2O_SetD - 1.))*var_ForthO_2O_SetD);
			SD_ForthO_ThirdO_SetD = sqrt((1./(NumOfForthO_ThirdO_SetD - 1.))*var_ForthO_ThirdO_SetD);
			SD_ForthO_ForthO_SetD = sqrt((1./(NumOfForthO_ForthO_SetD - 1.))*var_ForthO_ForthO_SetD);
			SD_ForthO_FifthO_SetD = sqrt((1./(NumOfForthO_FifthO_SetD - 1.))*var_ForthO_FifthO_SetD);
			SD_ForthO_SixthO_SetD = sqrt((1./(NumOfForthO_SixthO_SetD - 1.))*var_ForthO_SixthO_SetD);
			SD_ForthO_SeventhO_SetD = sqrt((1./(NumOfForthO_SeventhO_SetD - 1.))*var_ForthO_SeventhO_SetD);
			SD_ForthO_EighthO_SetD = sqrt((1./(NumOfForthO_EighthO_SetD - 1.))*var_ForthO_EighthO_SetD);
			SD_ForthO_NinthO_SetD = sqrt((1./(NumOfForthO_NinthO_SetD - 1.))*var_ForthO_NinthO_SetD);
			
			//FifthO as check mode//
			SD_FifthO_FU_SetD = sqrt((1./(NumOfFifthO_FU_SetD - 1.))*var_FifthO_FU_SetD);
			SD_FifthO_FO_SetD = sqrt((1./(NumOfFifthO_FO_SetD - 1.))*var_FifthO_FO_SetD);
			SD_FifthO_2O_SetD = sqrt((1./(NumOfFifthO_2O_SetD - 1.))*var_FifthO_2O_SetD);
			SD_FifthO_ThirdO_SetD = sqrt((1./(NumOfFifthO_ThirdO_SetD - 1.))*var_FifthO_ThirdO_SetD);
			SD_FifthO_ForthO_SetD = sqrt((1./(NumOfFifthO_ForthO_SetD - 1.))*var_FifthO_ForthO_SetD);
			SD_FifthO_FifthO_SetD = sqrt((1./(NumOfFifthO_FifthO_SetD - 1.))*var_FifthO_FifthO_SetD);
			SD_FifthO_SixthO_SetD = sqrt((1./(NumOfFifthO_SixthO_SetD - 1.))*var_FifthO_SixthO_SetD);
			SD_FifthO_SeventhO_SetD = sqrt((1./(NumOfFifthO_SeventhO_SetD - 1.))*var_FifthO_SeventhO_SetD);
			SD_FifthO_EighthO_SetD = sqrt((1./(NumOfFifthO_EighthO_SetD - 1.))*var_FifthO_EighthO_SetD);
			SD_FifthO_NinthO_SetD = sqrt((1./(NumOfFifthO_NinthO_SetD - 1.))*var_FifthO_NinthO_SetD);
			
			//SixthO as check mode//
			SD_SixthO_FU_SetD = sqrt((1./(NumOfSixthO_FU_SetD - 1.))*var_SixthO_FU_SetD);
			SD_SixthO_FO_SetD = sqrt((1./(NumOfSixthO_FO_SetD - 1.))*var_SixthO_FO_SetD);
			SD_SixthO_2O_SetD = sqrt((1./(NumOfSixthO_2O_SetD - 1.))*var_SixthO_2O_SetD);
			SD_SixthO_ThirdO_SetD = sqrt((1./(NumOfSixthO_ThirdO_SetD - 1.))*var_SixthO_ThirdO_SetD);
			SD_SixthO_ForthO_SetD = sqrt((1./(NumOfSixthO_ForthO_SetD - 1.))*var_SixthO_ForthO_SetD);
			SD_SixthO_FifthO_SetD = sqrt((1./(NumOfSixthO_FifthO_SetD - 1.))*var_SixthO_FifthO_SetD);
			SD_SixthO_SixthO_SetD = sqrt((1./(NumOfSixthO_SixthO_SetD - 1.))*var_SixthO_SixthO_SetD);
			SD_SixthO_SeventhO_SetD = sqrt((1./(NumOfSixthO_SeventhO_SetD - 1.))*var_SixthO_SeventhO_SetD);
			SD_SixthO_EighthO_SetD = sqrt((1./(NumOfSixthO_EighthO_SetD - 1.))*var_SixthO_EighthO_SetD);
			SD_SixthO_NinthO_SetD = sqrt((1./(NumOfSixthO_NinthO_SetD - 1.))*var_SixthO_NinthO_SetD);
			
			//SeventhO as check mode//
			SD_SeventhO_FU_SetD = sqrt((1./(NumOfSeventhO_FU_SetD - 1.))*var_SeventhO_FU_SetD);
			SD_SeventhO_FO_SetD = sqrt((1./(NumOfSeventhO_FO_SetD - 1.))*var_SeventhO_FO_SetD);
			SD_SeventhO_2O_SetD = sqrt((1./(NumOfSeventhO_2O_SetD - 1.))*var_SeventhO_2O_SetD);
			SD_SeventhO_ThirdO_SetD = sqrt((1./(NumOfSeventhO_ThirdO_SetD - 1.))*var_SeventhO_ThirdO_SetD);
			SD_SeventhO_ForthO_SetD = sqrt((1./(NumOfSeventhO_ForthO_SetD - 1.))*var_SeventhO_ForthO_SetD);
			SD_SeventhO_FifthO_SetD = sqrt((1./(NumOfSeventhO_FifthO_SetD - 1.))*var_SeventhO_FifthO_SetD);
			SD_SeventhO_SixthO_SetD = sqrt((1./(NumOfSeventhO_SixthO_SetD - 1.))*var_SeventhO_SixthO_SetD);
			SD_SeventhO_SeventhO_SetD = sqrt((1./(NumOfSeventhO_SeventhO_SetD - 1.))*var_SeventhO_SeventhO_SetD);
			SD_SeventhO_EighthO_SetD = sqrt((1./(NumOfSeventhO_EighthO_SetD - 1.))*var_SeventhO_EighthO_SetD);
			SD_SeventhO_NinthO_SetD = sqrt((1./(NumOfSeventhO_NinthO_SetD - 1.))*var_SeventhO_NinthO_SetD);
			
			//EighthO as check mode//
			SD_EighthO_FU_SetD = sqrt((1./(NumOfEighthO_FU_SetD - 1.))*var_EighthO_FU_SetD);
			SD_EighthO_FO_SetD = sqrt((1./(NumOfEighthO_FO_SetD - 1.))*var_EighthO_FO_SetD);
			SD_EighthO_2O_SetD = sqrt((1./(NumOfEighthO_2O_SetD - 1.))*var_EighthO_2O_SetD);
			SD_EighthO_ThirdO_SetD = sqrt((1./(NumOfEighthO_ThirdO_SetD - 1.))*var_EighthO_ThirdO_SetD);
			SD_EighthO_ForthO_SetD = sqrt((1./(NumOfEighthO_ForthO_SetD - 1.))*var_EighthO_ForthO_SetD);
			SD_EighthO_FifthO_SetD = sqrt((1./(NumOfEighthO_FifthO_SetD - 1.))*var_EighthO_FifthO_SetD);
			SD_EighthO_SixthO_SetD = sqrt((1./(NumOfEighthO_SixthO_SetD - 1.))*var_EighthO_SixthO_SetD);
			SD_EighthO_SeventhO_SetD = sqrt((1./(NumOfEighthO_SeventhO_SetD - 1.))*var_EighthO_SeventhO_SetD);
			SD_EighthO_EighthO_SetD = sqrt((1./(NumOfEighthO_EighthO_SetD - 1.))*var_EighthO_EighthO_SetD);
			SD_EighthO_NinthO_SetD = sqrt((1./(NumOfEighthO_NinthO_SetD - 1.))*var_EighthO_NinthO_SetD);
			
			//NinthO as check mode//
			SD_NinthO_FU_SetD = sqrt((1./(NumOfNinthO_FU_SetD - 1.))*var_NinthO_FU_SetD);
			SD_NinthO_FO_SetD = sqrt((1./(NumOfNinthO_FO_SetD - 1.))*var_NinthO_FO_SetD);
			SD_NinthO_2O_SetD = sqrt((1./(NumOfNinthO_2O_SetD - 1.))*var_NinthO_2O_SetD);
			SD_NinthO_ThirdO_SetD = sqrt((1./(NumOfNinthO_ThirdO_SetD - 1.))*var_NinthO_ThirdO_SetD);
			SD_NinthO_ForthO_SetD = sqrt((1./(NumOfNinthO_ForthO_SetD - 1.))*var_NinthO_ForthO_SetD);
			SD_NinthO_FifthO_SetD = sqrt((1./(NumOfNinthO_FifthO_SetD - 1.))*var_NinthO_FifthO_SetD);
			SD_NinthO_SixthO_SetD = sqrt((1./(NumOfNinthO_SixthO_SetD - 1.))*var_NinthO_SixthO_SetD);
			SD_NinthO_SeventhO_SetD = sqrt((1./(NumOfNinthO_SeventhO_SetD - 1.))*var_NinthO_SeventhO_SetD);
			SD_NinthO_EighthO_SetD = sqrt((1./(NumOfNinthO_EighthO_SetD - 1.))*var_NinthO_EighthO_SetD);
			SD_NinthO_NinthO_SetD = sqrt((1./(NumOfNinthO_NinthO_SetD - 1.))*var_NinthO_NinthO_SetD);
			
			////Output for Num////
			outFile_Num<<setw(10)<<"FU: "<<setw(20)<<NumOfFU_FU_SetD<<setw(30)<<NumOfFO_FU_SetD<<setw(30)<<NumOf2O_FU_SetD<<setw(30)<<NumOfThirdO_FU_SetD<<setw(30)<<NumOfForthO_FU_SetD<<setw(30)<<NumOfFifthO_FU_SetD<<setw(30)<<NumOfSixthO_FU_SetD<<setw(30)<<NumOfSeventhO_FU_SetD<<setw(30)<<NumOfEighthO_FU_SetD<<setw(30)<<NumOfNinthO_FU_SetD<<endl;
			
			outFile_Num<<setw(10)<<"FO: "<<setw(20)<<NumOfFU_FO_SetD<<setw(30)<<NumOfFO_FO_SetD<<setw(30)<<NumOf2O_FO_SetD<<setw(30)<<NumOfThirdO_FO_SetD<<setw(30)<<NumOfForthO_FO_SetD<<setw(30)<<NumOfFifthO_FO_SetD<<setw(30)<<NumOfSixthO_FO_SetD<<setw(30)<<NumOfSeventhO_FO_SetD<<setw(30)<<NumOfEighthO_FO_SetD<<setw(30)<<NumOfNinthO_FO_SetD<<endl;
			
			outFile_Num<<setw(10)<<"2O: "<<setw(20)<<NumOfFU_2O_SetD<<setw(30)<<NumOfFO_2O_SetD<<setw(30)<<NumOf2O_2O_SetD<<setw(30)<<NumOfThirdO_2O_SetD<<setw(30)<<NumOfForthO_2O_SetD<<setw(30)<<NumOfFifthO_2O_SetD<<setw(30)<<NumOfSixthO_2O_SetD<<setw(30)<<NumOfSeventhO_2O_SetD<<setw(30)<<NumOfEighthO_2O_SetD<<setw(30)<<NumOfNinthO_2O_SetD<<endl;
			
			outFile_Num<<setw(10)<<"ThirdO: "<<setw(20)<<NumOfFU_ThirdO_SetD<<setw(30)<<NumOfFO_ThirdO_SetD<<setw(30)<<NumOf2O_ThirdO_SetD<<setw(30)<<NumOfThirdO_ThirdO_SetD<<setw(30)<<NumOfForthO_ThirdO_SetD<<setw(30)<<NumOfFifthO_ThirdO_SetD<<setw(30)<<NumOfSixthO_ThirdO_SetD<<setw(30)<<NumOfSeventhO_ThirdO_SetD<<setw(30)<<NumOfEighthO_ThirdO_SetD<<setw(30)<<NumOfNinthO_ThirdO_SetD<<endl;
			
			outFile_Num<<setw(10)<<"ForthO: "<<setw(20)<<NumOfFU_ForthO_SetD<<setw(30)<<NumOfFO_ForthO_SetD<<setw(30)<<NumOf2O_ForthO_SetD<<setw(30)<<NumOfThirdO_ForthO_SetD<<setw(30)<<NumOfForthO_ForthO_SetD<<setw(30)<<NumOfFifthO_ForthO_SetD<<setw(30)<<NumOfSixthO_ForthO_SetD<<setw(30)<<NumOfSeventhO_ForthO_SetD<<setw(30)<<NumOfEighthO_ForthO_SetD<<setw(30)<<NumOfNinthO_ForthO_SetD<<endl;
			
			outFile_Num<<setw(10)<<"FifthO: "<<setw(20)<<NumOfFU_FifthO_SetD<<setw(30)<<NumOfFO_FifthO_SetD<<setw(30)<<NumOf2O_FifthO_SetD<<setw(30)<<NumOfThirdO_FifthO_SetD<<setw(30)<<NumOfForthO_FifthO_SetD<<setw(30)<<NumOfFifthO_FifthO_SetD<<setw(30)<<NumOfSixthO_FifthO_SetD<<setw(30)<<NumOfSeventhO_FifthO_SetD<<setw(30)<<NumOfEighthO_FifthO_SetD<<setw(30)<<NumOfNinthO_FifthO_SetD<<endl;
			
			outFile_Num<<setw(10)<<"SixthO: "<<setw(20)<<NumOfFU_SixthO_SetD<<setw(30)<<NumOfFO_SixthO_SetD<<setw(30)<<NumOf2O_SixthO_SetD<<setw(30)<<NumOfThirdO_SixthO_SetD<<setw(30)<<NumOfForthO_SixthO_SetD<<setw(30)<<NumOfFifthO_SixthO_SetD<<setw(30)<<NumOfSixthO_SixthO_SetD<<setw(30)<<NumOfSeventhO_SixthO_SetD<<setw(30)<<NumOfEighthO_SixthO_SetD<<setw(30)<<NumOfNinthO_SixthO_SetD<<endl;
			
			outFile_Num<<setw(10)<<"SeventhO: "<<setw(20)<<NumOfFU_SeventhO_SetD<<setw(30)<<NumOfFO_SeventhO_SetD<<setw(30)<<NumOf2O_SeventhO_SetD<<setw(30)<<NumOfThirdO_SeventhO_SetD<<setw(30)<<NumOfForthO_SeventhO_SetD<<setw(30)<<NumOfFifthO_SeventhO_SetD<<setw(30)<<NumOfSixthO_SeventhO_SetD<<setw(30)<<NumOfSeventhO_SeventhO_SetD<<setw(30)<<NumOfEighthO_SeventhO_SetD<<setw(30)<<NumOfNinthO_SeventhO_SetD<<endl;
			
			outFile_Num<<setw(10)<<"EighthO: "<<setw(20)<<NumOfFU_EighthO_SetD<<setw(30)<<NumOfFO_EighthO_SetD<<setw(30)<<NumOf2O_EighthO_SetD<<setw(30)<<NumOfThirdO_EighthO_SetD<<setw(30)<<NumOfForthO_EighthO_SetD<<setw(30)<<NumOfFifthO_EighthO_SetD<<setw(30)<<NumOfSixthO_EighthO_SetD<<setw(30)<<NumOfSeventhO_EighthO_SetD<<setw(30)<<NumOfEighthO_EighthO_SetD<<setw(30)<<NumOfNinthO_EighthO_SetD<<endl;
			
			outFile_Num<<setw(10)<<"NinthO: "<<setw(20)<<NumOfFU_NinthO_SetD<<setw(30)<<NumOfFO_NinthO_SetD<<setw(30)<<NumOf2O_NinthO_SetD<<setw(30)<<NumOfThirdO_NinthO_SetD<<setw(30)<<NumOfForthO_NinthO_SetD<<setw(30)<<NumOfFifthO_NinthO_SetD<<setw(30)<<NumOfSixthO_NinthO_SetD<<setw(30)<<NumOfSeventhO_NinthO_SetD<<setw(30)<<NumOfEighthO_NinthO_SetD<<setw(30)<<NumOfNinthO_NinthO_SetD<<endl;
			
			
			////Output for SD calcs////
			outFile_SD<<setw(10)<<"FU: "<<setw(20)<<SD_FU_FU_SetD<<setw(30)<<SD_FO_FU_SetD<<setw(30)<<SD_2O_FU_SetD<<setw(30)<<SD_ThirdO_FU_SetD<<setw(30)<<SD_ForthO_FU_SetD<<setw(30)<<SD_FifthO_FU_SetD<<setw(30)<<SD_SixthO_FU_SetD<<setw(30)<<SD_SeventhO_FU_SetD<<setw(30)<<SD_EighthO_FU_SetD<<setw(30)<<SD_NinthO_FU_SetD<<endl;
			
			outFile_SD<<setw(10)<<"FO: "<<setw(20)<<SD_FU_FO_SetD<<setw(30)<<SD_FO_FO_SetD<<setw(30)<<SD_2O_FO_SetD<<setw(30)<<SD_ThirdO_FO_SetD<<setw(30)<<SD_ForthO_FO_SetD<<setw(30)<<SD_FifthO_FO_SetD<<setw(30)<<SD_SixthO_FO_SetD<<setw(30)<<SD_SeventhO_FO_SetD<<setw(30)<<SD_EighthO_FO_SetD<<setw(30)<<SD_NinthO_FO_SetD<<endl;
			
			outFile_SD<<setw(10)<<"2O: "<<setw(20)<<SD_FU_2O_SetD<<setw(30)<<SD_FO_2O_SetD<<setw(30)<<SD_2O_2O_SetD<<setw(30)<<SD_ThirdO_2O_SetD<<setw(30)<<SD_ForthO_2O_SetD<<setw(30)<<SD_FifthO_2O_SetD<<setw(30)<<SD_SixthO_2O_SetD<<setw(30)<<SD_SeventhO_2O_SetD<<setw(30)<<SD_EighthO_2O_SetD<<setw(30)<<SD_NinthO_2O_SetD<<endl;
			
			outFile_SD<<setw(10)<<"ThirdO: "<<setw(20)<<SD_FU_ThirdO_SetD<<setw(30)<<SD_FO_ThirdO_SetD<<setw(30)<<SD_2O_ThirdO_SetD<<setw(30)<<SD_ThirdO_ThirdO_SetD<<setw(30)<<SD_ForthO_ThirdO_SetD<<setw(30)<<SD_FifthO_ThirdO_SetD<<setw(30)<<SD_SixthO_ThirdO_SetD<<setw(30)<<SD_SeventhO_ThirdO_SetD<<setw(30)<<SD_EighthO_ThirdO_SetD<<setw(30)<<SD_NinthO_ThirdO_SetD<<endl;
			
			outFile_SD<<setw(10)<<"ForthO: "<<setw(20)<<SD_FU_ForthO_SetD<<setw(30)<<SD_FO_ForthO_SetD<<setw(30)<<SD_2O_ForthO_SetD<<setw(30)<<SD_ThirdO_ForthO_SetD<<setw(30)<<SD_ForthO_ForthO_SetD<<setw(30)<<SD_FifthO_ForthO_SetD<<setw(30)<<SD_SixthO_ForthO_SetD<<setw(30)<<SD_SeventhO_ForthO_SetD<<setw(30)<<SD_EighthO_ForthO_SetD<<setw(30)<<SD_NinthO_ForthO_SetD<<endl;
			
			outFile_SD<<setw(10)<<"FifthO: "<<setw(20)<<SD_FU_FifthO_SetD<<setw(30)<<SD_FO_FifthO_SetD<<setw(30)<<SD_2O_FifthO_SetD<<setw(30)<<SD_ThirdO_FifthO_SetD<<setw(30)<<SD_ForthO_FifthO_SetD<<setw(30)<<SD_FifthO_FifthO_SetD<<setw(30)<<SD_SixthO_FifthO_SetD<<setw(30)<<SD_SeventhO_FifthO_SetD<<setw(30)<<SD_EighthO_FifthO_SetD<<setw(30)<<SD_NinthO_FifthO_SetD<<endl;
			
			outFile_SD<<setw(10)<<"SixthO: "<<setw(20)<<SD_FU_SixthO_SetD<<setw(30)<<SD_FO_SixthO_SetD<<setw(30)<<SD_2O_SixthO_SetD<<setw(30)<<SD_ThirdO_SixthO_SetD<<setw(30)<<SD_ForthO_SixthO_SetD<<setw(30)<<SD_FifthO_SixthO_SetD<<setw(30)<<SD_SixthO_SixthO_SetD<<setw(30)<<SD_SeventhO_SixthO_SetD<<setw(30)<<SD_EighthO_SixthO_SetD<<setw(30)<<SD_NinthO_SixthO_SetD<<endl;
			
			outFile_SD<<setw(10)<<"SeventhO: "<<setw(20)<<SD_FU_SeventhO_SetD<<setw(30)<<SD_FO_SeventhO_SetD<<setw(30)<<SD_2O_SeventhO_SetD<<setw(30)<<SD_ThirdO_SeventhO_SetD<<setw(30)<<SD_ForthO_SeventhO_SetD<<setw(30)<<SD_FifthO_SeventhO_SetD<<setw(30)<<SD_SixthO_SeventhO_SetD<<setw(30)<<SD_SeventhO_SeventhO_SetD<<setw(30)<<SD_EighthO_SeventhO_SetD<<setw(30)<<SD_NinthO_SeventhO_SetD<<endl;
			
			outFile_SD<<setw(10)<<"EighthO: "<<setw(20)<<SD_FU_EighthO_SetD<<setw(30)<<SD_FO_EighthO_SetD<<setw(30)<<SD_2O_EighthO_SetD<<setw(30)<<SD_ThirdO_EighthO_SetD<<setw(30)<<SD_ForthO_EighthO_SetD<<setw(30)<<SD_FifthO_EighthO_SetD<<setw(30)<<SD_SixthO_EighthO_SetD<<setw(30)<<SD_SeventhO_EighthO_SetD<<setw(30)<<SD_EighthO_EighthO_SetD<<setw(30)<<SD_NinthO_EighthO_SetD<<endl;
			
			outFile_SD<<setw(10)<<"NinthO: "<<setw(20)<<SD_FU_NinthO_SetD<<setw(30)<<SD_FO_NinthO_SetD<<setw(30)<<SD_2O_NinthO_SetD<<setw(30)<<SD_ThirdO_NinthO_SetD<<setw(30)<<SD_ForthO_NinthO_SetD<<setw(30)<<SD_FifthO_NinthO_SetD<<setw(30)<<SD_SixthO_NinthO_SetD<<setw(30)<<SD_SeventhO_NinthO_SetD<<setw(30)<<SD_EighthO_NinthO_SetD<<setw(30)<<SD_NinthO_NinthO_SetD<<endl;
			
			
			////Output for resonance ratios////	
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
			
			////output for fig5////
			for(int i = 0; i < NumOfModels; i++){
				if(Mass[i] == Mass_ToLookFor && L[i] == L_ToLookFor && ModelRan_SetD[i]){
					double frequency_FU = 1./period_FU_SetD[i];
					double frequency_FO = 1./period_FO_SetD[i];
					double frequency_2O = 1./period_2O_SetD[i];
					double frequency_ThirdO = 1./period_ThirdO_SetD[i];
					double frequency_ForthO = 1./period_ForthO_SetD[i];
					outFile_Fig5_FU_FO<<setw(10)<<Teff[i]<<setw(20)<<frequency_FU<<setw(20)<<frequency_FO<<setw(20)<<frequency_2O<<setw(20)<<frequency_ThirdO<<setw(20)<<frequency_ForthO<<setw(20)<<growth_FU_SetD[i]<<setw(20)<<growth_FO_SetD[i]<<setw(20)<<growth_2O_SetD[i]<<setw(20)<<growth_ThirdO_SetD[i]<<setw(20)<<growth_ForthO_SetD[i]<<endl;
				}
			}
		}
		
		outFile_AverageResonance.close();
		outFile_SD.close();
		outFile_Num.close();
		outFile_Fig5_FU_FO.close();
		
		
		////// Graphics /////
		if(j == 0){
			TCanvas *c1 = new TCanvas("c1","Canvas 1",200,10,900,700);
				c1->SetFillColor(10);
				c1->SetGrid();
				c1->GetFrame()->SetFillColor(10);
				c1->GetFrame()->SetBorderSize(12);
				c1->Range(0,0,1,1);

				gStyle->SetOptStat(0);
				
				TH1F *hr1 = c1->DrawFrame(0.0,0.0,20.0,8.0);
					hr1->SetTitle("p0/p1 SetA");
					hr1->SetXTitle("period_FU");
					hr1->SetYTitle("period_FU/period_FO");
				
				TGraphErrors *gr1 = new TGraphErrors(NumOfModels,period_FU_SetA,ResonanceRatio_FU_FO_SetA,0,0);
					gr1->SetMarkerColor(kBlue);
					gr1->SetMarkerSize(0.75);
					gr1->SetMarkerStyle(21);
					gr1->SetName("gr1");
					gr1->Draw("P");
					
				TFile outfile1("FU_FO_SetA.root", "RECREATE");
				c1->Write();
				outfile1.Close();
		}
		if(j == 1){
			TCanvas *c1 = new TCanvas("c1","Canvas 1",200,10,900,700);
				c1->SetFillColor(10);
				c1->SetGrid();
				c1->GetFrame()->SetFillColor(10);
				c1->GetFrame()->SetBorderSize(12);
				c1->Range(0,0,1,1);

				gStyle->SetOptStat(0);
				
				TH1F *hr1 = c1->DrawFrame(0.0,0.0,20.0,8.0);
					hr1->SetTitle("p0/p1 SetB");
					hr1->SetXTitle("period_FU");
					hr1->SetYTitle("period_FU/period_FO");
				
				TGraphErrors *gr1 = new TGraphErrors(NumOfModels,period_FU_SetB,ResonanceRatio_FU_FO_SetB,0,0);
					gr1->SetMarkerColor(kBlue);
					gr1->SetMarkerSize(0.75);
					gr1->SetMarkerStyle(21);
					gr1->SetName("gr1");
					gr1->Draw("P");
					
				TFile outfile1("FU_FO_SetB.root", "RECREATE");
				c1->Write();
				outfile1.Close();
		}
		if(j == 2){
			TCanvas *c1 = new TCanvas("c1","Canvas 1",200,10,900,700);
				c1->SetFillColor(10);
				c1->SetGrid();
				c1->GetFrame()->SetFillColor(10);
				c1->GetFrame()->SetBorderSize(12);
				c1->Range(0,0,1,1);

				gStyle->SetOptStat(0);
				
				TH1F *hr1 = c1->DrawFrame(0.0,0.0,20.0,8.0);
					hr1->SetTitle("p0/p1 SetC");
					hr1->SetXTitle("period_FU");
					hr1->SetYTitle("period_FU/period_FO");
				
				TGraphErrors *gr1 = new TGraphErrors(NumOfModels,period_FU_SetC,ResonanceRatio_FU_FO_SetC,0,0);
					gr1->SetMarkerColor(kBlue);
					gr1->SetMarkerSize(0.75);
					gr1->SetMarkerStyle(21);
					gr1->SetName("gr1");
					gr1->Draw("P");
					
				TFile outfile1("FU_FO_SetC.root", "RECREATE");
				c1->Write();
				outfile1.Close();
		}
		if(j == 3){
			TCanvas *c1 = new TCanvas("c1","Canvas 1",200,10,900,700);
				c1->SetFillColor(10);
				c1->SetGrid();
				c1->GetFrame()->SetFillColor(10);
				c1->GetFrame()->SetBorderSize(12);
				c1->Range(0,0,1,1);

				gStyle->SetOptStat(0);
				
				TH1F *hr1 = c1->DrawFrame(0.0,0.0,20.0,8.0);
					hr1->SetTitle("p0/p1 SetD");
					hr1->SetXTitle("period_FU");
					hr1->SetYTitle("period_FU/period_FO");
				
				TGraphErrors *gr1 = new TGraphErrors(NumOfModels,period_FU_SetD,ResonanceRatio_FU_FO_SetD,0,0);
					gr1->SetMarkerColor(kBlue);
					gr1->SetMarkerSize(0.75);
					gr1->SetMarkerStyle(21);
					gr1->SetName("gr1");
					gr1->Draw("P");
					
				TFile outfile1("FU_FO_SetD.root", "RECREATE");
				c1->Write();
				outfile1.Close();
		}
		
		
	}
	return 0;
}













