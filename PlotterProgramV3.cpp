//Program adopted from "DataFitter", primary use for creating Figure 4 in Smolec. May be formed for general use later on?

//V1: Making program able to create figure 5 in smolec for sets A B C and D.

//V2: Adding directroy output for easier sorting. Will now automatically find size of file and make arrays also

//V3: Updating for up to ninth overtone 

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

# include "TROOT.h"
# include "TFile.h"
# include "TTree.h"
# include "TBrowser.h"
# include "TH1.h"
# include "TH2.h"
# include "TH3.h"
# include "TRandom.h"

//Gauss user defined: [0]*exp((-(x-[1])*(x-[1]))/(2*[2]*[2]))

// L50	L100	L150	L200	L250	L300

// Z0.013	Z0.00834	Z0.00424	Z0.00135	Z0.00061	Z0.00043	Z0.00014


int main(){
	
	//////Controls//////
	
	int NumOfInputFiles = 28; //Number of input files IN ONE SET, 28 is max. Total files is Sets*Files
	int NumOfSets = 4; //A, B, C, D
	
	char inputFileName[50] = "ResonanceData_Fig4"; //"Prefix" to the input files. Full suffix will be "suffix" + Label
	char outputFileName[50] = "Fig4_m06"; //"Prefix" to the input files.
	
	char OutputDirectory[100] = "D:/Research_Data_Storage/LINA/Fig4_Plotter/Fig4_All_V3"; //The directroy the plots are put into - will NOT create if not existing.
	
	char Suffix_1[50] = "_L50_Z0.013";
	char Suffix_2[50] = "_L50_Z0.00834";
	char Suffix_3[50] = "_L50_Z0.00424";
	char Suffix_4[50] = "_L50_Z0.00135";
	char Suffix_5[50] = "_L50_Z0.00061";
	char Suffix_6[50] = "_L50_Z0.00043";
	char Suffix_7[50] = "_L50_Z0.00014";
	
	char Suffix_8[50] = "_L100_Z0.013";
	char Suffix_9[50] = "_L100_Z0.00834";
	char Suffix_10[50] = "_L100_Z0.00424";
	char Suffix_11[50] = "_L100_Z0.00135";
	char Suffix_12[50] = "_L100_Z0.00061";
	char Suffix_13[50] = "_L100_Z0.00043";
	char Suffix_14[50] = "_L100_Z0.00014";
	
	char Suffix_15[50] = "_L150_Z0.013";
	char Suffix_16[50] = "_L150_Z0.00834";
	char Suffix_17[50] = "_L150_Z0.00424";
	char Suffix_18[50] = "_L150_Z0.00135";
	char Suffix_19[50] = "_L150_Z0.00061";
	char Suffix_20[50] = "_L150_Z0.00043";
	char Suffix_21[50] = "_L150_Z0.00014";
	
	char Suffix_22[50] = "_L200_Z0.013";
	char Suffix_23[50] = "_L200_Z0.00834";
	char Suffix_24[50] = "_L200_Z0.00424";
	char Suffix_25[50] = "_L200_Z0.00135";
	char Suffix_26[50] = "_L200_Z0.00061";
	char Suffix_27[50] = "_L200_Z0.00043";
	char Suffix_28[50] = "_L200_Z0.00014";
	
	char Label_1[10] = "_SetA"; //Labels associated with input files, will be used as suffix to outfile names (Without .dat)
	char Label_2[10] = "_SetB";
	char Label_3[10] = "_SetC";
	char Label_4[10] = "_SetD";
	
	char PlotTitle_1[100] = "m = 0.6, L = 50, z = 0.013"; //Title of the plot
	char PlotTitle_2[100] = "m = 0.6, L = 50, z = 0.00834"; //Title of the plot
	char PlotTitle_3[100] = "m = 0.6, L = 50, z = 0.00424"; //Title of the plot
	char PlotTitle_4[100] = "m = 0.6, L = 50, z = 0.00135"; //Title of the plot
	char PlotTitle_5[100] = "m = 0.6, L = 50, z = 0.00061"; //Title of the plot
	char PlotTitle_6[100] = "m = 0.6, L = 50, z = 0.00043"; //Title of the plot
	char PlotTitle_7[100] = "m = 0.6, L = 50, z = 0.00014"; //Title of the plot
	
	char PlotTitle_8[100] = "m = 0.6, L = 100, z = 0.013"; //Title of the plot
	char PlotTitle_9[100] = "m = 0.6, L = 100, z = 0.00834"; //Title of the plot
	char PlotTitle_10[100] = "m = 0.6, L = 100, z = 0.00424"; //Title of the plot
	char PlotTitle_11[100] = "m = 0.6, L = 100, z = 0.00135"; //Title of the plot
	char PlotTitle_12[100] = "m = 0.6, L = 100, z = 0.00061"; //Title of the plot
	char PlotTitle_13[100] = "m = 0.6, L = 100, z = 0.00043"; //Title of the plot
	char PlotTitle_14[100] = "m = 0.6, L = 100, z = 0.00014"; //Title of the plot
	
	char PlotTitle_15[50] = "m = 0.6, L = 150, z = 0.013"; //Title of the plot
	char PlotTitle_16[50] = "m = 0.6, L = 150, z = 0.00834"; //Title of the plot
	char PlotTitle_17[50] = "m = 0.6, L = 150, z = 0.00424"; //Title of the plot
	char PlotTitle_18[50] = "m = 0.6, L = 150, z = 0.00135"; //Title of the plot
	char PlotTitle_19[50] = "m = 0.6, L = 150, z = 0.00061"; //Title of the plot
	char PlotTitle_20[50] = "m = 0.6, L = 150, z = 0.00043"; //Title of the plot
	char PlotTitle_21[50] = "m = 0.6, L = 150, z = 0.00014"; //Title of the plot
	
	char PlotTitle_22[50] = "m = 0.6, L = 200, z = 0.013"; //Title of the plot
	char PlotTitle_23[50] = "m = 0.6, L = 200, z = 0.00834"; //Title of the plot
	char PlotTitle_24[50] = "m = 0.6, L = 200, z = 0.00424"; //Title of the plot
	char PlotTitle_25[50] = "m = 0.6, L = 200, z = 0.00135"; //Title of the plot
	char PlotTitle_26[50] = "m = 0.6, L = 200, z = 0.00061"; //Title of the plot
	char PlotTitle_27[50] = "m = 0.6, L = 200, z = 0.00043"; //Title of the plot
	char PlotTitle_28[50] = "m = 0.6, L = 200, z = 0.00014"; //Title of the plot
	
	//Top plot of Fig4//
	char xTitle_1[50] = "T_{eff}"; //Title for x-axis, first column of data file
	char yTitle_1[50] = "Frequency"; //Title for y-axis, second column of data file
	
	double xLowerLimit_1 = 4000; //Limits for top plot in Fig4
	double xUpperLimit_1 = 8000;
	double yLowerLimit_1 = 0;
	double yUpperLimit_1 = 6.0;
	
	//Bottom plot of Fig4//
	char xTitle_2[50] = "T_{eff}"; //Title for x-axis, first column of data file
	char yTitle_2[50] = "Growth Rate"; //Title for y-axis, second column of data file
	
	double xLowerLimit_2 = 4000; //Limits for bottom plot in Fig4
	double xUpperLimit_2 = 8000;
	double yLowerLimit_2 = -2.0;
	double yUpperLimit_2 = 0.5;
	
	bool DataWithHeaders = true; //If true, will account for headers by using getline function
	bool SavePlotAsPNG = true; //If true, will save plot with file name above.
	bool SavePlotAsRootFile = true; //If true, will save plot in root file with name above.
	
	////////////////////////////
	
	//////Variables//////
	
	ifstream inFile;
	ofstream outFile;
	
	int NumOfLines;
	
	double xMax;
	double xMin;
	double yMax;
	double yMin;
	
	char line[100];
	char header[100];
	char Full_RootFileName[100];
	char Full_PNGFileName[100];
	
	char Full_RootFilePath[100];
	char Full_PNGFilePath[100];
	
	//Dummy Variables//
	
	double dummy1, dummy2, dummy3, dummy4, dummy5, dummy6, dummy7;
	
	////////////////////////////
	
	//////Main Program//////
	for(int f = 0; f < NumOfInputFiles; f++){
		for(int s = 0; s < NumOfSets; s++){
		
			//////Variables//////
			
			char inputFileName_temp[100] = "";
			
			/////////////////////
			
			if(f == 0){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_1);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 1){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_2);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 2){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_3);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 3){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_4);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 4){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_5);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 5){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_6);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 6){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_7);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 7){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_8);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 8){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_9);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 9){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_10);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 10){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_11);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 11){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_12);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 12){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_13);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 13){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_14);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 14){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_15);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 15){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_16);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 16){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_17);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 17){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_18);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 18){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_19);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 19){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_20);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 20){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_21);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 21){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_22);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 22){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_23);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 23){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_24);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 24){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_25);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 25){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_26);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 26){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_27);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			if(f == 27){
				strcpy(inputFileName_temp,inputFileName);
				strcat(inputFileName_temp,Suffix_28);
				if(s == 0){
					strcat(inputFileName_temp,Label_1);
				}
				if(s == 1){
					strcat(inputFileName_temp,Label_2);
				}
				if(s == 2){
					strcat(inputFileName_temp,Label_3);
				}
				if(s == 3){
					strcat(inputFileName_temp,Label_4);
				}
				strcat(inputFileName_temp,".dat");
			}
			
			
			cout<<"Input file name: "<<inputFileName_temp<<endl;
			
			//Getting x,y from file//
			inFile.open(inputFileName_temp,ios::in);
			if(inFile.is_open()){
				cout<<"Input file was opened successfully"<<endl;
			
				int NumOfLines = 0;
				while(!inFile.eof()){
					getline(inFile, line);
					NumOfLines++;
				}
				if(DataWithHeaders){
					NumOfLines = NumOfLines - 2;
				}
				else{NumOfLines = NumOfLines - 1;}
				
				cout<<"Lines in input file: "<<NumOfLines<<endl;
				
				//Creating sized arrays//
				const int nArray = NumOfLines + 20;
				
				double Teff[nArray];           
				double freq_FU[nArray];
				double freq_FO[nArray];          
				double freq_2O[nArray];
				double freq_ThirdO[nArray];         
				double freq_ForthO[nArray]; 
				double freq_FifthO[nArray]; 
				double freq_SixthO[nArray]; 
				double freq_SeventhO[nArray]; 
				double freq_EighthO[nArray]; 
				double freq_NinthO[nArray]; 
				
				double growth_FU[nArray];
				double growth_FO[nArray];
				double growth_2O[nArray];
				double growth_ThirdO[nArray];
				double growth_ForthO[nArray];
				double growth_FifthO[nArray];
				double growth_SixthO[nArray];
				double growth_SeventhO[nArray];
				double growth_EighthO[nArray];
				double growth_NinthO[nArray];
				
				double freq_BG1[nArray];
				double freq_BG2[nArray];
				double freq_BG3[nArray];
				double freq_BG4[nArray];
				double freq_BG5[nArray];
				double freq_BG6[nArray];
				double freq_BG7[nArray];
				
			}
			else{cout<<"Input file not found, check name?"<<endl;}
			inFile.close();
		
			inFile.open(inputFileName_temp,ios::in);
			if(inFile.good()){
				if(DataWithHeaders){
					getline(inFile,header);
					cout<<header<<endl;
				}

				for(int i = 0; i < NumOfLines; i++){
					inFile>>Teff[i]>>freq_FU[i]>>freq_FO[i]>>freq_2O[i]>>freq_ThirdO[i]>>freq_ForthO[i]>>freq_FifthO[i]>>freq_SixthO[i]>>freq_SeventhO[i]>>freq_EighthO[i]>>freq_NinthO[i]>>growth_FU[i]>>growth_FO[i]>>growth_2O[i]>>growth_ThirdO[i]>>growth_ForthO[i]>>growth_FifthO[i]>>growth_SixthO[i]>>growth_SeventhO[i]>>growth_EighthO[i]>>growth_NinthO[i];
					//inFile.ignore(50, '\n'); //This allows for only the defined lines to be read (unless data file is 50+ columns long) 
					cout<<setw(5)<<Teff[i]<<setw(10)<<freq_FU[i]<<setw(10)<<freq_FO[i]<<setw(10)<<freq_2O[i]<<setw(10)<<freq_ThirdO[i]<<setw(10)<<freq_ForthO[i]<<setw(10)<<freq_FifthO[i]<<setw(10)<<freq_SixthO[i]<<setw(10)<<freq_SeventhO[i]<<setw(10)<<freq_EighthO[i]<<setw(10)<<freq_NinthO[i]<<setw(10)<<growth_FU[i]<<setw(10)<<growth_FO[i]<<setw(10)<<growth_2O[i]<<setw(10)<<growth_ThirdO[i]<<setw(10)<<growth_ForthO[i]<<setw(10)<<growth_FifthO[i]<<setw(10)<<growth_SixthO[i]<<setw(10)<<growth_SeventhO[i]<<setw(10)<<growth_EighthO[i]<<setw(10)<<growth_NinthO[i]<<endl;
					
					freq_BG1[i] = 1.5*freq_FU[i];
					freq_BG2[i] = 2.0*freq_FU[i];
					freq_BG3[i] = 2.5*freq_FU[i];
					freq_BG4[i] = 3.5*freq_FU[i];
					freq_BG5[i] = 4.5*freq_FU[i];
					freq_BG6[i] = 5.5*freq_FU[i];
					freq_BG7[i] = 6.5*freq_FU[i];
					
					
				}
				cout<<"************* End of file ********************"<<endl;
			
		
				//////Graphics//////
				
				//Output file name creation//
				if(f == 0){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_1);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_1);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 1){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_2);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_2);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 2){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_3);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_3);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 3){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_4);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_4);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 4){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_5);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_5);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 5){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_6);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_6);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 6){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_7);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_7);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 7){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_8);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_8);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 8){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_9);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_9);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 9){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_10);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_10);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 10){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_11);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_11);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 11){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_12);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_12);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 12){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_13);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_13);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 13){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_14);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_14);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 14){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_15);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_15);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 15){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_16);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_16);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 16){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_17);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_17);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 17){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_18);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_18);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 18){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_19);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_19);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 19){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_20);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_20);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 20){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_21);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_21);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 21){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_22);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_22);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 22){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_23);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_23);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 23){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_24);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_24);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 24){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_25);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_25);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 25){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_26);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_26);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 26){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_27);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_27);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				if(f == 27){
					strcpy(Full_RootFileName, outputFileName);
					strcat(Full_RootFileName, Suffix_28);
					if(s == 0){
						strcat(Full_RootFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_RootFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_RootFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_RootFileName, Label_4);
					}
					strcat(Full_RootFileName, ".root");
					
					strcpy(Full_RootFilePath, OutputDirectory);
					strcat(Full_RootFilePath, "/RootFiles/");
					strcat(Full_RootFilePath, Full_RootFileName);
					
					strcpy(Full_PNGFileName, outputFileName);
					strcat(Full_PNGFileName, Suffix_28);
					if(s == 0){
						strcat(Full_PNGFileName, Label_1);
					}
					if(s == 1){
						strcat(Full_PNGFileName, Label_2);
					}
					if(s == 2){
						strcat(Full_PNGFileName, Label_3);
					}
					if(s == 3){
						strcat(Full_PNGFileName, Label_4);
					}
					strcat(Full_PNGFileName, ".png");
					
					strcpy(Full_PNGFilePath, OutputDirectory);
					strcat(Full_PNGFilePath, "/PNGFiles/");
					strcat(Full_PNGFilePath, Full_PNGFileName);
					
					cout<<"Root file path: "<<Full_RootFilePath<<endl;
					cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
				}
				
				
				//Plot creation//
				TCanvas *c1 = new TCanvas("c1","Canvas 1",200,10,900,700);
					c1->Divide(1,2);
					c1->SetFillColor(10);
					c1->SetGrid();
					c1->GetFrame()->SetFillColor(10);
					c1->GetFrame()->SetBorderSize(12);
					c1->Range(0,0,1,1);

					gStyle->SetOptStat(0);
					
					cout<<"Made canvas.."<<endl;
					
				c1->cd(1);
					TH1F *hr1 = c1_1->DrawFrame(xLowerLimit_1,yLowerLimit_1,xUpperLimit_1,yUpperLimit_1);
						if(f == 0){
							hr1->SetTitle(PlotTitle_1);
						}
						if(f == 1){
							hr1->SetTitle(PlotTitle_2);
						}
						if(f == 2){
							hr1->SetTitle(PlotTitle_3);
						}
						if(f == 3){
							hr1->SetTitle(PlotTitle_4);
						}
						if(f == 4){
							hr1->SetTitle(PlotTitle_5);
						}
						if(f == 5){
							hr1->SetTitle(PlotTitle_6);
						}
						if(f == 6){
							hr1->SetTitle(PlotTitle_7);
						}
						if(f == 7){
							hr1->SetTitle(PlotTitle_8);
						}
						if(f == 8){
							hr1->SetTitle(PlotTitle_9);
						}
						if(f == 9){
							hr1->SetTitle(PlotTitle_10);
						}
						if(f == 10){
							hr1->SetTitle(PlotTitle_11);
						}
						if(f == 11){
							hr1->SetTitle(PlotTitle_12);
						}
						if(f == 12){
							hr1->SetTitle(PlotTitle_13);
						}
						if(f == 13){
							hr1->SetTitle(PlotTitle_14);
						}
						if(f == 14){
							hr1->SetTitle(PlotTitle_15);
						}
						if(f == 15){
							hr1->SetTitle(PlotTitle_16);
						}
						if(f == 16){
							hr1->SetTitle(PlotTitle_17);
						}
						if(f == 17){
							hr1->SetTitle(PlotTitle_18);
						}
						if(f == 18){
							hr1->SetTitle(PlotTitle_19);
						}
						if(f == 19){
							hr1->SetTitle(PlotTitle_20);
						}
						if(f == 20){
							hr1->SetTitle(PlotTitle_21);
						}
						if(f == 21){
							hr1->SetTitle(PlotTitle_22);
						}
						if(f == 22){
							hr1->SetTitle(PlotTitle_23);
						}
						if(f == 23){
							hr1->SetTitle(PlotTitle_24);
						}
						if(f == 24){
							hr1->SetTitle(PlotTitle_25);
						}
						if(f == 25){
							hr1->SetTitle(PlotTitle_26);
						}
						if(f == 26){
							hr1->SetTitle(PlotTitle_27);
						}
						if(f == 27){
							hr1->SetTitle(PlotTitle_28);
						}
						hr1->SetXTitle(xTitle_1);
						hr1->SetYTitle(yTitle_1);
						
					//1.5vF//
					TGraphErrors *gr6 = new TGraphErrors(NumOfLines,Teff,freq_BG1,0,0);
						gr6->SetMarkerColor(kBlack);
						gr6->SetMarkerSize(0.5);
						gr6->SetMarkerStyle(7);
						gr6->SetLineStyle(2);
						gr6->SetFillColor(kWhite);
						gr6->SetName("gr6");
						gr6->Draw("C");
						
					//2.0vF//
					TGraphErrors *gr7 = new TGraphErrors(NumOfLines,Teff,freq_BG2,0,0);
						gr7->SetMarkerColor(kBlack);
						gr7->SetMarkerSize(0.5);
						gr7->SetMarkerStyle(7);
						gr7->SetLineStyle(4);
						gr7->SetFillColor(kWhite);
						gr7->SetName("gr7");
						gr7->Draw("CSAME");
					
					//2.5vF//
					TGraphErrors *gr8 = new TGraphErrors(NumOfLines,Teff,freq_BG3,0,0);
						gr8->SetMarkerColor(kBlack);
						gr8->SetMarkerSize(0.5);
						gr8->SetMarkerStyle(7);
						gr8->SetLineStyle(5);
						gr8->SetFillColor(kWhite);
						gr8->SetName("gr8");
						gr8->Draw("CSAME");
					
					//3.5vF//
					TGraphErrors *gr9 = new TGraphErrors(NumOfLines,Teff,freq_BG4,0,0);
						gr9->SetMarkerColor(kBlack);
						gr9->SetMarkerSize(0.5);
						gr9->SetMarkerStyle(7);
						gr9->SetLineStyle(6);
						gr9->SetFillColor(kWhite);
						gr9->SetName("gr9");
						gr9->Draw("CSAME");
					
					//4.5vF//
					TGraphErrors *gr10 = new TGraphErrors(NumOfLines,Teff,freq_BG5,0,0);
						gr10->SetMarkerColor(kBlack);
						gr10->SetMarkerSize(0.5);
						gr10->SetMarkerStyle(7);
						gr10->SetLineStyle(7);
						gr10->SetFillColor(kWhite);
						gr10->SetName("gr10");
						gr10->Draw("CSAME");
						
					//5.5vF//
					TGraphErrors *gr26 = new TGraphErrors(NumOfLines,Teff,freq_BG6,0,0);
						gr26->SetMarkerColor(kBlack);
						gr26->SetMarkerSize(0.5);
						gr26->SetMarkerStyle(7);
						gr26->SetLineStyle(9);
						gr26->SetFillColor(kWhite);
						gr26->SetName("gr26");
						gr26->Draw("CSAME");
						
					//6.5vF//
					TGraphErrors *gr27 = new TGraphErrors(NumOfLines,Teff,freq_BG7,0,0);
						gr27->SetMarkerColor(kBlack);
						gr27->SetMarkerSize(0.5);
						gr27->SetMarkerStyle(7);
						gr27->SetLineStyle(10);
						gr27->SetFillColor(kWhite);
						gr27->SetName("gr27");
						gr27->Draw("CSAME");
						
					//FU//
					TGraphErrors *gr1 = new TGraphErrors(NumOfLines,Teff,freq_FU,0,0);
						gr1->SetMarkerColor(kRed);
						gr1->SetMarkerSize(0.5);
						gr1->SetMarkerStyle(20);
						gr1->SetFillColor(kWhite);
						gr1->SetName("gr1");
						gr1->Draw("P");
						
					//FO//	
					TGraphErrors *gr2 = new TGraphErrors(NumOfLines,Teff,freq_FO,0,0);
						gr2->SetMarkerColor(kGreen);
						gr2->SetMarkerSize(0.5);
						gr2->SetMarkerStyle(24);
						gr2->SetFillColor(kWhite);
						gr2->SetName("gr2");
						gr2->Draw("PSAME");
					
					//2O//
					TGraphErrors *gr3 = new TGraphErrors(NumOfLines,Teff,freq_2O,0,0);
						gr3->SetMarkerColor(kBlue);
						gr3->SetMarkerSize(0.5);
						gr3->SetMarkerStyle(21);
						gr3->SetFillColor(kWhite);
						gr3->SetName("gr3");
						gr3->Draw("PSAME");
						
					//ThirdO//
					TGraphErrors *gr4 = new TGraphErrors(NumOfLines,Teff,freq_ThirdO,0,0);
						gr4->SetMarkerColor(kMagenta);
						gr4->SetMarkerSize(0.5);
						gr4->SetMarkerStyle(25);
						gr4->SetFillColor(kWhite);
						gr4->SetName("gr4");
						gr4->Draw("PSAME");
						
					//ForthO//
					TGraphErrors *gr5 = new TGraphErrors(NumOfLines,Teff,freq_ForthO,0,0);
						gr5->SetMarkerColor(kCyan);
						gr5->SetMarkerSize(0.5);
						gr5->SetMarkerStyle(22);
						gr5->SetFillColor(kWhite);
						gr5->SetName("gr5");
						gr5->Draw("PSAME");	
						
					//FifthO//
					TGraphErrors *gr16 = new TGraphErrors(NumOfLines,Teff,freq_FifthO,0,0);
						gr16->SetMarkerColor(kRed+2);
						gr16->SetMarkerSize(0.5);
						gr16->SetMarkerStyle(20);
						gr16->SetFillColor(kWhite);
						gr16->SetName("gr16");
						gr16->Draw("PSAME");	
						
					//SixthO//
					TGraphErrors *gr17 = new TGraphErrors(NumOfLines,Teff,freq_SixthO,0,0);
						gr17->SetMarkerColor(kGreen+3);
						gr17->SetMarkerSize(0.5);
						gr17->SetMarkerStyle(24);
						gr17->SetFillColor(kWhite);
						gr17->SetName("gr17");
						gr17->Draw("PSAME");	
						
					//SeventhO//
					TGraphErrors *gr18 = new TGraphErrors(NumOfLines,Teff,freq_SeventhO,0,0);
						gr18->SetMarkerColor(kBlue+2);
						gr18->SetMarkerSize(0.5);
						gr18->SetMarkerStyle(21);
						gr18->SetFillColor(kWhite);
						gr18->SetName("gr18");
						gr18->Draw("PSAME");	
						
					//EighthO//
					TGraphErrors *gr19 = new TGraphErrors(NumOfLines,Teff,freq_EighthO,0,0);
						gr19->SetMarkerColor(kMagenta+3);
						gr19->SetMarkerSize(0.5);
						gr19->SetMarkerStyle(25);
						gr19->SetFillColor(kWhite);
						gr19->SetName("gr19");
						gr19->Draw("PSAME");	
						
					//NinthO//
					TGraphErrors *gr20 = new TGraphErrors(NumOfLines,Teff,freq_NinthO,0,0);
						gr20->SetMarkerColor(kCyan+3);
						gr20->SetMarkerSize(0.5);
						gr20->SetMarkerStyle(22);
						gr20->SetFillColor(kWhite);
						gr20->SetName("gr20");
						gr20->Draw("PSAME");	
						
					TLegend *leg = new TLegend(0.12,0.3,0.20,0.9);
						leg->AddEntry("gr1","F","p");
						leg->AddEntry("gr2","1O","p");
						leg->AddEntry("gr3","2O","p");
						leg->AddEntry("gr4","3O","p");
						leg->AddEntry("gr5","4O","p");
						leg->AddEntry("gr16","5O","p");
						leg->AddEntry("gr17","6O","p");
						leg->AddEntry("gr18","7O","p");
						leg->AddEntry("gr19","8O","p");
						leg->AddEntry("gr20","9O","p");
						
						leg->AddEntry("gr6","1.5F","l");
						leg->AddEntry("gr7","2.0F","l");
						leg->AddEntry("gr8","2.5F","l");
						leg->AddEntry("gr9","3.5F","l");
						leg->AddEntry("gr10","4.5F","l");
						leg->AddEntry("gr26","5.5F","l");
						leg->AddEntry("gr27","6.5F","l");
						leg->Draw();
						
				c1->cd(2);
					TH1F *hr2 = c1_2->DrawFrame(xLowerLimit_2,yLowerLimit_2,xUpperLimit_2,yUpperLimit_2);
						hr2->SetTitle("");
						hr2->SetXTitle(xTitle_2);
						hr2->SetYTitle(yTitle_2);
						
					TGraphErrors *gr11 = new TGraphErrors(NumOfLines,Teff,growth_FU,0,0);
						gr11->SetMarkerColor(kRed);
						gr11->SetMarkerSize(0.5);
						gr11->SetMarkerStyle(20);
						gr11->SetName("gr11");
						gr11->Draw("P");
						
					TGraphErrors *gr12 = new TGraphErrors(NumOfLines,Teff,growth_FO,0,0);
						gr12->SetMarkerColor(kGreen);
						gr12->SetMarkerSize(0.5);
						gr12->SetMarkerStyle(24);
						gr12->SetName("gr12");
						gr12->Draw("PSAME");
						
					TGraphErrors *gr13 = new TGraphErrors(NumOfLines,Teff,growth_2O,0,0);
						gr13->SetMarkerColor(kBlue);
						gr13->SetMarkerSize(0.5);
						gr13->SetMarkerStyle(21);
						gr13->SetName("gr13");
						gr13->Draw("PSAME");
						
					TGraphErrors *gr14 = new TGraphErrors(NumOfLines,Teff,growth_ThirdO,0,0);
						gr14->SetMarkerColor(kMagenta);
						gr14->SetMarkerSize(0.5);
						gr14->SetMarkerStyle(25);
						gr14->SetName("gr14");
						gr14->Draw("PSAME");
						
					TGraphErrors *gr15 = new TGraphErrors(NumOfLines,Teff,growth_ForthO,0,0);
						gr15->SetMarkerColor(kCyan);
						gr15->SetMarkerSize(0.5);
						gr15->SetMarkerStyle(22);
						gr15->SetName("gr15");
						gr15->Draw("PSAME");
						
					TGraphErrors *gr21 = new TGraphErrors(NumOfLines,Teff,growth_FifthO,0,0);
						gr21->SetMarkerColor(kRed+2);
						gr21->SetMarkerSize(0.5);
						gr21->SetMarkerStyle(20);
						gr21->SetName("gr21");
						gr21->Draw("PSAME");
						
					TGraphErrors *gr22 = new TGraphErrors(NumOfLines,Teff,growth_SixthO,0,0);
						gr22->SetMarkerColor(kGreen+3);
						gr22->SetMarkerSize(0.5);
						gr22->SetMarkerStyle(24);
						gr22->SetName("gr22");
						gr22->Draw("PSAME");
						
					TGraphErrors *gr23 = new TGraphErrors(NumOfLines,Teff,growth_SeventhO,0,0);
						gr23->SetMarkerColor(kBlue+2);
						gr23->SetMarkerSize(0.5);
						gr23->SetMarkerStyle(21);
						gr23->SetName("gr23");
						gr23->Draw("PSAME");
						
					TGraphErrors *gr24 = new TGraphErrors(NumOfLines,Teff,growth_EighthO,0,0);
						gr24->SetMarkerColor(kMagenta+3);
						gr24->SetMarkerSize(0.5);
						gr24->SetMarkerStyle(25);
						gr24->SetName("gr24");
						gr24->Draw("PSAME");
						
					TGraphErrors *gr25 = new TGraphErrors(NumOfLines,Teff,growth_NinthO,0,0);
						gr25->SetMarkerColor(kCyan+3);
						gr25->SetMarkerSize(0.5);
						gr25->SetMarkerStyle(22);
						gr25->SetName("gr25");
						gr25->Draw("PSAME");
				
				if(SavePlotAsPNG){
					c1->SaveAs(Full_PNGFilePath);
				}
				if(SavePlotAsRootFile){
					TFile outfile1(Full_RootFilePath, "RECREATE");
					c1->Write();
					outfile1.Close();
				}
				
			}
			inFile.close();
		}
	}
	
	return 0;
	
}
