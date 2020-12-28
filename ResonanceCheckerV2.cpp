//This code is not the same as v1. Remaking the resonence code to be able to act alone so pos growth models and non pos growth may be looked at.

//V2: Attempting to make code output resonence data. Unfinished. Need to work out way to output ResonenceRatios to correct file with or without cut on data. Also want overall average ratio?
///Note: So far its finished?
///Note: Add collection of near resonances to smolec? Also eventually want bulk data output?
///Note: Must add conditions for things only working when FU period is positive as well. Also need condition that does not output 1 to file if no pos growth rate models were found for a mode

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
	
	bool DoPosGrowthCut = true; //If true, will only get positive growth models
	
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
	
	double ResonanceRatio_FU_SetA[nArray];
	double ResonanceRatio_FO_SetA[nArray];
	double ResonanceRatio_2O_SetA[nArray];
	double ResonanceRatio_ThirdO_SetA[nArray];
	double ResonanceRatio_ForthO_SetA[nArray];
	double ResonanceRatio_FifthO_SetA[nArray];
	double ResonanceRatio_SixthO_SetA[nArray];
	double ResonanceRatio_SeventhO_SetA[nArray];
	double ResonanceRatio_EighthO_SetA[nArray];
	double ResonanceRatio_NinthO_SetA[nArray];
	
	double ResonanceRatio_FU_SetB[nArray];
	double ResonanceRatio_FO_SetB[nArray];
	double ResonanceRatio_2O_SetB[nArray];
	double ResonanceRatio_ThirdO_SetB[nArray];
	double ResonanceRatio_ForthO_SetB[nArray];
	double ResonanceRatio_FifthO_SetB[nArray];
	double ResonanceRatio_SixthO_SetB[nArray];
	double ResonanceRatio_SeventhO_SetB[nArray];
	double ResonanceRatio_EighthO_SetB[nArray];
	double ResonanceRatio_NinthO_SetB[nArray];
	
	double ResonanceRatio_FU_SetC[nArray];
	double ResonanceRatio_FO_SetC[nArray];
	double ResonanceRatio_2O_SetC[nArray];
	double ResonanceRatio_ThirdO_SetC[nArray];
	double ResonanceRatio_ForthO_SetC[nArray];
	double ResonanceRatio_FifthO_SetC[nArray];
	double ResonanceRatio_SixthO_SetC[nArray];
	double ResonanceRatio_SeventhO_SetC[nArray];
	double ResonanceRatio_EighthO_SetC[nArray];
	double ResonanceRatio_NinthO_SetC[nArray];
	
	double ResonanceRatio_FU_SetD[nArray];
	double ResonanceRatio_FO_SetD[nArray];
	double ResonanceRatio_2O_SetD[nArray];
	double ResonanceRatio_ThirdO_SetD[nArray];
	double ResonanceRatio_ForthO_SetD[nArray];
	double ResonanceRatio_FifthO_SetD[nArray];
	double ResonanceRatio_SixthO_SetD[nArray];
	double ResonanceRatio_SeventhO_SetD[nArray];
	double ResonanceRatio_EighthO_SetD[nArray];
	double ResonanceRatio_NinthO_SetD[nArray];
	
	double OverallResonance_FU_SetA = 0;
	double AverageResonance_FU_SetA;
	double OverallResonance_FU_SetB = 0;
	double AverageResonance_FU_SetB;
	double OverallResonance_FU_SetC = 0;
	double AverageResonance_FU_SetC;
	double OverallResonance_FU_SetD = 0;
	double AverageResonance_FU_SetD;
	
	double OverallResonance_FO_SetA = 0;
	double AverageResonance_FO_SetA;
	double OverallResonance_FO_SetB = 0;
	double AverageResonance_FO_SetB;
	double OverallResonance_FO_SetC = 0;
	double AverageResonance_FO_SetC;
	double OverallResonance_FO_SetD = 0;
	double AverageResonance_FO_SetD;
	
	double OverallResonance_2O_SetA = 0;
	double AverageResonance_2O_SetA;
	double OverallResonance_2O_SetB = 0;
	double AverageResonance_2O_SetB;
	double OverallResonance_2O_SetC = 0;
	double AverageResonance_2O_SetC;
	double OverallResonance_2O_SetD = 0;
	double AverageResonance_2O_SetD;
	
	double OverallResonance_ThirdO_SetA = 0;
	double AverageResonance_ThirdO_SetA;
	double OverallResonance_ThirdO_SetB = 0;
	double AverageResonance_ThirdO_SetB;
	double OverallResonance_ThirdO_SetC = 0;
	double AverageResonance_ThirdO_SetC;
	double OverallResonance_ThirdO_SetD = 0;
	double AverageResonance_ThirdO_SetD;
	
	double OverallResonance_ForthO_SetA = 0;
	double AverageResonance_ForthO_SetA;
	double OverallResonance_ForthO_SetB = 0;
	double AverageResonance_ForthO_SetB;
	double OverallResonance_ForthO_SetC = 0;
	double AverageResonance_ForthO_SetC;
	double OverallResonance_ForthO_SetD = 0;
	double AverageResonance_ForthO_SetD;
	
	double OverallResonance_FifthO_SetA = 0;
	double AverageResonance_FifthO_SetA;
	double OverallResonance_FifthO_SetB = 0;
	double AverageResonance_FifthO_SetB;
	double OverallResonance_FifthO_SetC = 0;
	double AverageResonance_FifthO_SetC;
	double OverallResonance_FifthO_SetD = 0;
	double AverageResonance_FifthO_SetD;
	
	double OverallResonance_SixthO_SetA = 0;
	double AverageResonance_SixthO_SetA;
	double OverallResonance_SixthO_SetB = 0;
	double AverageResonance_SixthO_SetB;
	double OverallResonance_SixthO_SetC = 0;
	double AverageResonance_SixthO_SetC;
	double OverallResonance_SixthO_SetD = 0;
	double AverageResonance_SixthO_SetD;
	
	double OverallResonance_SeventhO_SetA = 0;
	double AverageResonance_SeventhO_SetA;
	double OverallResonance_SeventhO_SetB = 0;
	double AverageResonance_SeventhO_SetB;
	double OverallResonance_SeventhO_SetC = 0;
	double AverageResonance_SeventhO_SetC;
	double OverallResonance_SeventhO_SetD = 0;
	double AverageResonance_SeventhO_SetD;
	
	double OverallResonance_EighthO_SetA = 0;
	double AverageResonance_EighthO_SetA;
	double OverallResonance_EighthO_SetB = 0;
	double AverageResonance_EighthO_SetB;
	double OverallResonance_EighthO_SetC = 0;
	double AverageResonance_EighthO_SetC;
	double OverallResonance_EighthO_SetD = 0;
	double AverageResonance_EighthO_SetD;
	
	double OverallResonance_NinthO_SetA = 0;
	double AverageResonance_NinthO_SetA;
	double OverallResonance_NinthO_SetB = 0;
	double AverageResonance_NinthO_SetB;
	double OverallResonance_NinthO_SetC = 0;
	double AverageResonance_NinthO_SetC;
	double OverallResonance_NinthO_SetD = 0;
	double AverageResonance_NinthO_SetD;
	
	int NumOfFU_SetA = 0;
	int NumOfFU_SetB = 0;
	int NumOfFU_SetC = 0;
	int NumOfFU_SetD = 0;
	
	int NumOfFO_SetA = 0;
	int NumOfFO_SetB = 0;
	int NumOfFO_SetC = 0;
	int NumOfFO_SetD = 0;
	
	int NumOf2O_SetA = 0;
	int NumOf2O_SetB = 0;
	int NumOf2O_SetC = 0;
	int NumOf2O_SetD = 0;
	
	int NumOfThirdO_SetA = 0;
	int NumOfThirdO_SetB = 0;
	int NumOfThirdO_SetC = 0;
	int NumOfThirdO_SetD = 0;
	
	int NumOfForthO_SetA = 0;
	int NumOfForthO_SetB = 0;
	int NumOfForthO_SetC = 0;
	int NumOfForthO_SetD = 0;
	
	int NumOfFifthO_SetA = 0;
	int NumOfFifthO_SetB = 0;
	int NumOfFifthO_SetC = 0;
	int NumOfFifthO_SetD = 0;
	
	int NumOfSixthO_SetA = 0;
	int NumOfSixthO_SetB = 0;
	int NumOfSixthO_SetC = 0;
	int NumOfSixthO_SetD = 0;
	
	int NumOfSeventhO_SetA = 0;
	int NumOfSeventhO_SetB = 0;
	int NumOfSeventhO_SetC = 0;
	int NumOfSeventhO_SetD = 0;
	
	int NumOfEighthO_SetA = 0;
	int NumOfEighthO_SetB = 0;
	int NumOfEighthO_SetC = 0;
	int NumOfEighthO_SetD = 0;
	
	int NumOfNinthO_SetA = 0;
	int NumOfNinthO_SetB = 0;
	int NumOfNinthO_SetC = 0;
	int NumOfNinthO_SetD = 0;
	
	///New Varibles///
	const int nArray = NumOfModels + 1;
	
	double mode;
	double period;
	double growth;
	
	
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
		outFile_AverageResonance.open(AverageResonanceFileName, ios::out);
		
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
				
				//Loop for number of modes on LINA data. Each iteration looks at a different mode and fills array to be used.
				cout<<"Resonance Ratios..."<<endl;
				for(int m = 0; m < NumOfModes; m++){
					
					double mode;
					double period;
					double growth;
					
					double period_FU;
					
					inFile>>mode>>period>>growth;
					cout<<mode<<" "<<period<<" "<<growth<<endl;
					
					if(m == 0){
						period_FU = period;
					}
					
					if(growth > 0.0 || !DoPosGrowthCut){
						//SetA
						if(j == 0){
							if(m == 0){
								ResonanceRatio_FU_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_SetA[i]<<endl;
								
								OverallResonance_FU_SetA = OverallResonance_FU_SetA + ResonanceRatio_FU_SetA[i];
								NumOfFU_SetA++;
							}
							if(m == 1){
								ResonanceRatio_FO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FO_SetA[i]<<endl;
								
								OverallResonance_FO_SetA = OverallResonance_FO_SetA + ResonanceRatio_FO_SetA[i];
								NumOfFO_SetA++;
							}
							if(m == 2){
								ResonanceRatio_2O_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_2O_SetA[i]<<endl;
								
								OverallResonance_2O_SetA = OverallResonance_2O_SetA + ResonanceRatio_2O_SetA[i];
								NumOf2O_SetA++;
							}
							if(m == 3){
								ResonanceRatio_ThirdO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_SetA[i]<<endl;
								
								OverallResonance_ThirdO_SetA = OverallResonance_ThirdO_SetA + ResonanceRatio_ThirdO_SetA[i];
								NumOfThirdO_SetA++;
							}
							if(m == 4){
								ResonanceRatio_ForthO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_SetA[i]<<endl;
								
								OverallResonance_ForthO_SetA = OverallResonance_ForthO_SetA + ResonanceRatio_ForthO_SetA[i];
								NumOfForthO_SetA++;
							}
							if(m == 5){
								ResonanceRatio_FifthO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_SetA[i]<<endl;
								
								OverallResonance_FifthO_SetA = OverallResonance_FifthO_SetA + ResonanceRatio_FifthO_SetA[i];
								NumOfFifthO_SetA++;
							}
							if(m == 6){
								ResonanceRatio_SixthO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_SetA[i]<<endl;
								
								OverallResonance_SixthO_SetA = OverallResonance_SixthO_SetA + ResonanceRatio_SixthO_SetA[i];
								NumOfSixthO_SetA++;
							}
							if(m == 7){
								ResonanceRatio_SeventhO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_SetA[i]<<endl;
								
								OverallResonance_SeventhO_SetA = OverallResonance_SeventhO_SetA + ResonanceRatio_SeventhO_SetA[i];
								NumOfSeventhO_SetA++;
							}
							if(m == 8){
								ResonanceRatio_EighthO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_SetA[i]<<endl;
								
								OverallResonance_EighthO_SetA = OverallResonance_EighthO_SetA + ResonanceRatio_EighthO_SetA[i];
								NumOfEighthO_SetA++;
							}
							if(m == 9){
								ResonanceRatio_NinthO_SetA[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_SetA[i]<<endl;
								
								OverallResonance_NinthO_SetA = OverallResonance_NinthO_SetA + ResonanceRatio_NinthO_SetA[i];
								NumOfNinthO_SetA++;
							}
						}
						//SetB
						if(j == 1){
							if(m == 0){
								ResonanceRatio_FU_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_SetB[i]<<endl;
								
								OverallResonance_FU_SetB = OverallResonance_FU_SetB + ResonanceRatio_FU_SetB[i];
								NumOfFU_SetB++;
							}
							if(m == 1){
								ResonanceRatio_FO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FO_SetB[i]<<endl;
								
								OverallResonance_FO_SetB = OverallResonance_FO_SetB + ResonanceRatio_FO_SetB[i];
								NumOfFO_SetB++;
							}
							if(m == 2){
								ResonanceRatio_2O_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_2O_SetB[i]<<endl;
								
								OverallResonance_2O_SetB = OverallResonance_2O_SetB + ResonanceRatio_2O_SetB[i];
								NumOf2O_SetB++;
							}
							if(m == 3){
								ResonanceRatio_ThirdO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_SetB[i]<<endl;
								
								OverallResonance_ThirdO_SetB = OverallResonance_ThirdO_SetB + ResonanceRatio_ThirdO_SetB[i];
								NumOfThirdO_SetB++;
							}
							if(m == 4){
								ResonanceRatio_ForthO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_SetB[i]<<endl;
								
								OverallResonance_ForthO_SetB = OverallResonance_ForthO_SetB + ResonanceRatio_ForthO_SetB[i];
								NumOfForthO_SetB++;
							}
							if(m == 5){
								ResonanceRatio_FifthO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_SetB[i]<<endl;
								
								OverallResonance_FifthO_SetB = OverallResonance_FifthO_SetB + ResonanceRatio_FifthO_SetB[i];
								NumOfFifthO_SetB++;
							}
							if(m == 6){
								ResonanceRatio_SixthO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_SetB[i]<<endl;
								
								OverallResonance_SixthO_SetB = OverallResonance_SixthO_SetB + ResonanceRatio_SixthO_SetB[i];
								NumOfSixthO_SetB++;
							}
							if(m == 7){
								ResonanceRatio_SeventhO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_SetB[i]<<endl;
								
								OverallResonance_SeventhO_SetB = OverallResonance_SeventhO_SetB + ResonanceRatio_SeventhO_SetB[i];
								NumOfSeventhO_SetB++;
							}
							if(m == 8){
								ResonanceRatio_EighthO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_SetB[i]<<endl;
								
								OverallResonance_EighthO_SetB = OverallResonance_EighthO_SetB + ResonanceRatio_EighthO_SetB[i];
								NumOfEighthO_SetB++;
							}
							if(m == 9){
								ResonanceRatio_NinthO_SetB[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_SetB[i]<<endl;
								
								OverallResonance_NinthO_SetB = OverallResonance_NinthO_SetB + ResonanceRatio_NinthO_SetB[i];
								NumOfNinthO_SetB++;
							}
						}
						//SetC
						if(j == 2){
							if(m == 0){
								ResonanceRatio_FU_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_SetC[i]<<endl;
								
								OverallResonance_FU_SetC = OverallResonance_FU_SetC + ResonanceRatio_FU_SetC[i];
								NumOfFU_SetC++;
							}
							if(m == 1){
								ResonanceRatio_FO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FO_SetC[i]<<endl;
								
								OverallResonance_FO_SetC = OverallResonance_FO_SetC + ResonanceRatio_FO_SetC[i];
								NumOfFO_SetC++;
							}
							if(m == 2){
								ResonanceRatio_2O_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_2O_SetC[i]<<endl;
								
								OverallResonance_2O_SetC = OverallResonance_2O_SetC + ResonanceRatio_2O_SetC[i];
								NumOf2O_SetC++;
							}
							if(m == 3){
								ResonanceRatio_ThirdO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_SetC[i]<<endl;
								
								OverallResonance_ThirdO_SetC = OverallResonance_ThirdO_SetC + ResonanceRatio_ThirdO_SetC[i];
								NumOfThirdO_SetC++;
							}
							if(m == 4){
								ResonanceRatio_ForthO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_SetC[i]<<endl;
								
								OverallResonance_ForthO_SetC = OverallResonance_ForthO_SetC + ResonanceRatio_ForthO_SetC[i];
								NumOfForthO_SetC++;
							}
							if(m == 5){
								ResonanceRatio_FifthO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_SetC[i]<<endl;
								
								OverallResonance_FifthO_SetC = OverallResonance_FifthO_SetC + ResonanceRatio_FifthO_SetC[i];
								NumOfFifthO_SetC++;
							}
							if(m == 6){
								ResonanceRatio_SixthO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_SetC[i]<<endl;
								
								OverallResonance_SixthO_SetC = OverallResonance_SixthO_SetC + ResonanceRatio_SixthO_SetC[i];
								NumOfSixthO_SetC++;
							}
							if(m == 7){
								ResonanceRatio_SeventhO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_SetC[i]<<endl;
								
								OverallResonance_SeventhO_SetC = OverallResonance_SeventhO_SetC + ResonanceRatio_SeventhO_SetC[i];
								NumOfSeventhO_SetC++;
							}
							if(m == 8){
								ResonanceRatio_EighthO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_SetC[i]<<endl;
								
								OverallResonance_EighthO_SetC = OverallResonance_EighthO_SetC + ResonanceRatio_EighthO_SetC[i];
								NumOfEighthO_SetC++;
							}
							if(m == 9){
								ResonanceRatio_NinthO_SetC[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_SetC[i]<<endl;
								
								OverallResonance_NinthO_SetC = OverallResonance_NinthO_SetC + ResonanceRatio_NinthO_SetC[i];
								NumOfNinthO_SetC++;
							}
						}
						//SetD
						if(j == 3){
							if(m == 0){
								ResonanceRatio_FU_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FU_SetD[i]<<endl;
								
								OverallResonance_FU_SetD = OverallResonance_FU_SetD + ResonanceRatio_FU_SetD[i];
								NumOfFU_SetD++;
							}
							if(m == 1){
								ResonanceRatio_FO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FO_SetD[i]<<endl;
								
								OverallResonance_FO_SetD = OverallResonance_FO_SetD + ResonanceRatio_FO_SetD[i];
								NumOfFO_SetD++;
							}
							if(m == 2){
								ResonanceRatio_2O_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_2O_SetD[i]<<endl;
								
								OverallResonance_2O_SetD = OverallResonance_2O_SetD + ResonanceRatio_2O_SetD[i];
								NumOf2O_SetD++;
							}
							if(m == 3){
								ResonanceRatio_ThirdO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_ThirdO_SetD[i]<<endl;
								
								OverallResonance_ThirdO_SetD = OverallResonance_ThirdO_SetD + ResonanceRatio_ThirdO_SetD[i];
								NumOfThirdO_SetD++;
							}
							if(m == 4){
								ResonanceRatio_ForthO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_ForthO_SetD[i]<<endl;
								
								OverallResonance_ForthO_SetD = OverallResonance_ForthO_SetD + ResonanceRatio_ForthO_SetD[i];
								NumOfForthO_SetD++;
							}
							if(m == 5){
								ResonanceRatio_FifthO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_FifthO_SetD[i]<<endl;
								
								OverallResonance_FifthO_SetD = OverallResonance_FifthO_SetD + ResonanceRatio_FifthO_SetD[i];
								NumOfFifthO_SetD++;
							}
							if(m == 6){
								ResonanceRatio_SixthO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_SixthO_SetD[i]<<endl;
								
								OverallResonance_SixthO_SetD = OverallResonance_SixthO_SetD + ResonanceRatio_SixthO_SetD[i];
								NumOfSixthO_SetD++;
							}
							if(m == 7){
								ResonanceRatio_SeventhO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_SeventhO_SetD[i]<<endl;
								
								OverallResonance_SeventhO_SetD = OverallResonance_SeventhO_SetD + ResonanceRatio_SeventhO_SetD[i];
								NumOfSeventhO_SetD++;
							}
							if(m == 8){
								ResonanceRatio_EighthO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_EighthO_SetD[i]<<endl;
								
								OverallResonance_EighthO_SetD = OverallResonance_EighthO_SetD + ResonanceRatio_EighthO_SetD[i];
								NumOfEighthO_SetD++;
							}
							if(m == 9){
								ResonanceRatio_NinthO_SetD[i] = period_FU/period;
								cout<<m<<": "<<ResonanceRatio_NinthO_SetD[i]<<endl;
								
								OverallResonance_NinthO_SetD = OverallResonance_NinthO_SetD + ResonanceRatio_NinthO_SetD[i];
								NumOfNinthO_SetD++;
							}
						}
					}
				}
			}
			inFile.close();
		}
		if(j == 0){
			outFile_AverageResonance<<"FU Ratio: "<<(OverallResonance_FU_SetA/NumOfFU_SetA)<<endl;
			outFile_AverageResonance<<"FO Ratio: "<<(OverallResonance_FO_SetA/NumOfFO_SetA)<<endl;
			outFile_AverageResonance<<"2O Ratio: "<<(OverallResonance_2O_SetA/NumOf2O_SetA)<<endl;
			outFile_AverageResonance<<"ThirdO Ratio: "<<(OverallResonance_ThirdO_SetA/NumOfThirdO_SetA)<<endl;
			outFile_AverageResonance<<"ForthO Ratio: "<<(OverallResonance_ForthO_SetA/NumOfForthO_SetA)<<endl;
			outFile_AverageResonance<<"FifthO Ratio: "<<(OverallResonance_FifthO_SetA/NumOfFifthO_SetA)<<endl;
			outFile_AverageResonance<<"SixthO Ratio: "<<(OverallResonance_SixthO_SetA/NumOfSixthO_SetA)<<endl;
			outFile_AverageResonance<<"SeventhO Ratio: "<<(OverallResonance_SeventhO_SetA/NumOfSeventhO_SetA)<<endl;
			outFile_AverageResonance<<"EighthO Ratio: "<<(OverallResonance_EighthO_SetA/NumOfEighthO_SetA)<<endl;
			outFile_AverageResonance<<"NinthO Ratio: "<<(OverallResonance_NinthO_SetA/NumOfNinthO_SetA)<<endl;
		}
		if(j == 1){
			outFile_AverageResonance<<"FU Ratio: "<<(OverallResonance_FU_SetB/NumOfFU_SetB)<<endl;
			outFile_AverageResonance<<"FO Ratio: "<<(OverallResonance_FO_SetB/NumOfFO_SetB)<<endl;
			outFile_AverageResonance<<"2O Ratio: "<<(OverallResonance_2O_SetB/NumOf2O_SetB)<<endl;
			outFile_AverageResonance<<"ThirdO Ratio: "<<(OverallResonance_ThirdO_SetB/NumOfThirdO_SetB)<<endl;
			outFile_AverageResonance<<"ForthO Ratio: "<<(OverallResonance_ForthO_SetB/NumOfForthO_SetB)<<endl;
			outFile_AverageResonance<<"FifthO Ratio: "<<(OverallResonance_FifthO_SetB/NumOfFifthO_SetB)<<endl;
			outFile_AverageResonance<<"SixthO Ratio: "<<(OverallResonance_SixthO_SetB/NumOfSixthO_SetB)<<endl;
			outFile_AverageResonance<<"SeventhO Ratio: "<<(OverallResonance_SeventhO_SetB/NumOfSeventhO_SetB)<<endl;
			outFile_AverageResonance<<"EighthO Ratio: "<<(OverallResonance_EighthO_SetB/NumOfEighthO_SetB)<<endl;
			outFile_AverageResonance<<"NinthO Ratio: "<<(OverallResonance_NinthO_SetB/NumOfNinthO_SetB)<<endl;
		}
		if(j == 2){
			outFile_AverageResonance<<"FU Ratio: "<<(OverallResonance_FU_SetC/NumOfFU_SetC)<<endl;
			outFile_AverageResonance<<"FO Ratio: "<<(OverallResonance_FO_SetC/NumOfFO_SetC)<<endl;
			outFile_AverageResonance<<"2O Ratio: "<<(OverallResonance_2O_SetC/NumOf2O_SetC)<<endl;
			outFile_AverageResonance<<"ThirdO Ratio: "<<(OverallResonance_ThirdO_SetC/NumOfThirdO_SetC)<<endl;
			outFile_AverageResonance<<"ForthO Ratio: "<<(OverallResonance_ForthO_SetC/NumOfForthO_SetC)<<endl;
			outFile_AverageResonance<<"FifthO Ratio: "<<(OverallResonance_FifthO_SetC/NumOfFifthO_SetC)<<endl;
			outFile_AverageResonance<<"SixthO Ratio: "<<(OverallResonance_SixthO_SetC/NumOfSixthO_SetC)<<endl;
			outFile_AverageResonance<<"SeventhO Ratio: "<<(OverallResonance_SeventhO_SetC/NumOfSeventhO_SetC)<<endl;
			outFile_AverageResonance<<"EighthO Ratio: "<<(OverallResonance_EighthO_SetC/NumOfEighthO_SetC)<<endl;
			outFile_AverageResonance<<"NinthO Ratio: "<<(OverallResonance_NinthO_SetC/NumOfNinthO_SetC)<<endl;
		}
		if(j == 3){
			outFile_AverageResonance<<"FU Ratio: "<<(OverallResonance_FU_SetD/NumOfFU_SetD)<<endl;
			outFile_AverageResonance<<"FO Ratio: "<<(OverallResonance_FO_SetD/NumOfFO_SetD)<<endl;
			outFile_AverageResonance<<"2O Ratio: "<<(OverallResonance_2O_SetD/NumOf2O_SetD)<<endl;
			outFile_AverageResonance<<"ThirdO Ratio: "<<(OverallResonance_ThirdO_SetD/NumOfThirdO_SetD)<<endl;
			outFile_AverageResonance<<"ForthO Ratio: "<<(OverallResonance_ForthO_SetD/NumOfForthO_SetD)<<endl;
			outFile_AverageResonance<<"FifthO Ratio: "<<(OverallResonance_FifthO_SetD/NumOfFifthO_SetD)<<endl;
			outFile_AverageResonance<<"SixthO Ratio: "<<(OverallResonance_SixthO_SetD/NumOfSixthO_SetD)<<endl;
			outFile_AverageResonance<<"SeventhO Ratio: "<<(OverallResonance_SeventhO_SetD/NumOfSeventhO_SetD)<<endl;
			outFile_AverageResonance<<"EighthO Ratio: "<<(OverallResonance_EighthO_SetD/NumOfEighthO_SetD)<<endl;
			outFile_AverageResonance<<"NinthO Ratio: "<<(OverallResonance_NinthO_SetD/NumOfNinthO_SetD)<<endl;
		}
		outFile_AverageResonance.close();
	}
	return 0;
}


















