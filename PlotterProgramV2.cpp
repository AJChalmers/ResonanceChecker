//Program adopted from "DataFitter", primary use for creating Figure 4 in Smolec. May be formed for general use later on?

//V1: Making program able to create figure 5 in smolec for sets A B C and D.

//V2: Adding directroy output for easier sorting. Will now automatically find size of file and make arrays also

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
	
	int NumOfInputFiles = 4; //Number of input files plots are made for, useful for SetA through D.
	
	char PNGFileName[250] = "Fig4_m06_L250_Z0.00834"; //PNG output file name, do not include .png
	char RootFileName[250] = "Fig4_m06_L250_Z0.00834"; //Root output file name, do not include .root
	
	char inputFileName1[250] = "ResonanceData_Fig4_L250_Z0.00834_SetA.dat"; 
	char inputFileName2[250] = "ResonanceData_Fig4_L250_Z0.00834_SetB.dat"; 
	char inputFileName3[250] = "ResonanceData_Fig4_L250_Z0.00834_SetC.dat"; 
	char inputFileName4[250] = "ResonanceData_Fig4_L250_Z0.00834_SetD.dat"; 
	
	char OutputDirectory[250] = "D:/Research_Data_Storage/LINA/Fig5_Plotter/Fig4_All_V2/Fig4_L250"; //The directroy the plots are put into - will NOT create if not existing.
	
	char Label_1[10] = "SetA"; //Labels associated with input files, will be used as suffix to outfile names
	char Label_2[10] = "SetB";
	char Label_3[10] = "SetC";
	char Label_4[10] = "SetD";
	
	//Top plot of Fig4//
	char PlotTitle_1[250] = "m = 0.6, L = 250, z = 0.00834"; //Title of the plot
	char xTitle_1[50] = "T_{eff}"; //Title for x-axis, first column of data file
	char yTitle_1[50] = "Frequency"; //Title for y-axis, second column of data file
	
	double xLowerLimit_1 = 4000; //Limits for top plot in Fig4
	double xUpperLimit_1 = 8000;
	double yLowerLimit_1 = 0;
	double yUpperLimit_1 = 5.0;
	
	//Bottom plot of Fig4//
	char PlotTitle_2[50] = ""; //Title of the plot
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
		
		//////Variables//////
		
		char inputFileName[50] = "";
		
		/////////////////////
		
		if(f == 0){
			strcpy(inputFileName,inputFileName1);
			// if(DataWithHeaders){
				// NumOfLines = NumOfLines_1 - 1;
			// }
			// else{NumOfLines = NumOfLines_1;}
		}
		if(f == 1){
			strcpy(inputFileName,inputFileName2);
			// if(DataWithHeaders){
				// NumOfLines = NumOfLines_2 - 1;
			// }
			// else{NumOfLines = NumOfLines_2;}
		}
		if(f == 2){
			strcpy(inputFileName,inputFileName3);
			// if(DataWithHeaders){
				// NumOfLines = NumOfLines_3 - 1;
			// }
			// else{NumOfLines = NumOfLines_3;}
		}
		if(f == 3){
			strcpy(inputFileName,inputFileName4);
			// if(DataWithHeaders){
				// NumOfLines = NumOfLines_4 - 1;
			// }
			// else{NumOfLines = NumOfLines_4;}
		}
		
		cout<<"Input file name: "<<inputFileName<<endl;
		
		//Getting x,y from file//
		inFile.open(inputFileName,ios::in);
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
			const int nArray = NumOfLines + 5;
			
			double Teff[nArray];           
			double freq_FU[nArray];
			double freq_FO[nArray];          
			double freq_2O[nArray];
			double freq_ThirdO[nArray];         
			double freq_ForthO[nArray];          
			double growth_FU[nArray];
			double growth_FO[nArray];
			double growth_2O[nArray];
			double growth_ThirdO[nArray];
			double growth_ForthO[nArray];
			
			double freq_BG1[nArray];
			double freq_BG2[nArray];
			double freq_BG3[nArray];
			double freq_BG4[nArray];
			double freq_BG5[nArray];
			
		}
		else{cout<<"Input file not found, check name?"<<endl;}
		inFile.close();
	
		inFile.open(inputFileName,ios::in);
		if(inFile.good()){
			if(DataWithHeaders){
				getline(inFile,header);
				cout<<header<<endl;
			}

			for(int i = 0; i < NumOfLines; i++){
				inFile>>Teff[i]>>freq_FU[i]>>freq_FO[i]>>freq_2O[i]>>freq_ThirdO[i]>>freq_ForthO[i]>>growth_FU[i]>>growth_FO[i]>>growth_2O[i]>>growth_ThirdO[i]>>growth_ForthO[i];
				//inFile.ignore(50, '\n'); //This allows for only the defined lines to be read (unless data file is 50+ columns long) 
				cout<<setw(5)<<Teff[i]<<setw(10)<<freq_FU[i]<<setw(10)<<freq_FO[i]<<setw(10)<<freq_2O[i]<<setw(10)<<freq_ThirdO[i]<<setw(10)<<freq_ForthO[i]<<setw(10)<<growth_FU[i]<<setw(10)<<growth_FO[i]<<setw(10)<<growth_2O[i]<<setw(10)<<growth_ThirdO[i]<<setw(10)<<growth_ForthO[i]<<endl;
				
				freq_BG1[i] = 1.5*freq_FU[i];
				freq_BG2[i] = 2.0*freq_FU[i];
				freq_BG3[i] = 2.5*freq_FU[i];
				freq_BG4[i] = 3.5*freq_FU[i];
				freq_BG5[i] = 4.5*freq_FU[i];
				
				
			}
			cout<<"************* End of file ********************"<<endl;
		
	
			//////Graphics//////
			
			//Output file name creation//
			if(f == 0){
				strcpy(Full_RootFileName, RootFileName);
				strcat(Full_RootFileName, "_");
				strcat(Full_RootFileName, Label_1);
				strcat(Full_RootFileName, ".root");
				
				strcpy(Full_RootFilePath, OutputDirectory);
				strcat(Full_RootFilePath, "/RootFiles/");
				strcat(Full_RootFilePath, Full_RootFileName);
				
				strcpy(Full_PNGFileName, PNGFileName);
				strcat(Full_PNGFileName, "_");
				strcat(Full_PNGFileName, Label_1);
				strcat(Full_PNGFileName, ".png");
				
				strcpy(Full_PNGFilePath, OutputDirectory);
				strcat(Full_PNGFilePath, "/PNGFiles/");
				strcat(Full_PNGFilePath, Full_PNGFileName);
				
				cout<<"Root file path: "<<Full_RootFilePath<<endl;
				cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
			}
			if(f == 1){
				strcpy(Full_RootFileName, RootFileName);
				strcat(Full_RootFileName, "_");
				strcat(Full_RootFileName, Label_2);
				strcat(Full_RootFileName, ".root");
				
				strcpy(Full_RootFilePath, OutputDirectory);
				strcat(Full_RootFilePath, "/RootFiles/");
				strcat(Full_RootFilePath, Full_RootFileName);
				
				strcpy(Full_PNGFileName, PNGFileName);
				strcat(Full_PNGFileName, "_");
				strcat(Full_PNGFileName, Label_2);
				strcat(Full_PNGFileName, ".png");
				
				strcpy(Full_PNGFilePath, OutputDirectory);
				strcat(Full_PNGFilePath, "/PNGFiles/");
				strcat(Full_PNGFilePath, Full_PNGFileName);
				
				cout<<"Root file path: "<<Full_RootFilePath<<endl;
				cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
			}
			if(f == 2){
				strcpy(Full_RootFileName, RootFileName);
				strcat(Full_RootFileName, "_");
				strcat(Full_RootFileName, Label_3);
				strcat(Full_RootFileName, ".root");
				
				strcpy(Full_RootFilePath, OutputDirectory);
				strcat(Full_RootFilePath, "/RootFiles/");
				strcat(Full_RootFilePath, Full_RootFileName);
				
				strcpy(Full_PNGFileName, PNGFileName);
				strcat(Full_PNGFileName, "_");
				strcat(Full_PNGFileName, Label_3);
				strcat(Full_PNGFileName, ".png");
				
				strcpy(Full_PNGFilePath, OutputDirectory);
				strcat(Full_PNGFilePath, "/PNGFiles/");
				strcat(Full_PNGFilePath, Full_PNGFileName);
				
				cout<<"Root file path: "<<Full_RootFilePath<<endl;
				cout<<"PNG file path: "<<Full_PNGFilePath<<endl;
			}
			if(f == 3){
				strcpy(Full_RootFileName, RootFileName);
				strcat(Full_RootFileName, "_");
				strcat(Full_RootFileName, Label_4);
				strcat(Full_RootFileName, ".root");
				
				strcpy(Full_RootFilePath, OutputDirectory);
				strcat(Full_RootFilePath, "/RootFiles/");
				strcat(Full_RootFilePath, Full_RootFileName);
				
				strcpy(Full_PNGFileName, PNGFileName);
				strcat(Full_PNGFileName, "_");
				strcat(Full_PNGFileName, Label_4);
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
					hr1->SetTitle(PlotTitle_1);
					hr1->SetXTitle(xTitle_1);
					hr1->SetYTitle(yTitle_1);
					
				//1.5vF//
				TGraphErrors *gr6 = new TGraphErrors(NumOfLines,Teff,freq_BG1,0,0);
					gr6->SetMarkerColor(kBlack);
					gr6->SetMarkerSize(0.5);
					gr6->SetMarkerStyle(7);
					gr6->SetFillColor(kWhite);
					gr6->SetName("gr6");
					gr6->Draw("P");
					
				//2.0vF//
				TGraphErrors *gr7 = new TGraphErrors(NumOfLines,Teff,freq_BG2,0,0);
					gr7->SetMarkerColor(kBlack);
					gr7->SetMarkerSize(0.5);
					gr7->SetMarkerStyle(7);
					gr7->SetFillColor(kWhite);
					gr7->SetName("gr7");
					gr7->Draw("PSAME");
				
				//2.5vF//
				TGraphErrors *gr8 = new TGraphErrors(NumOfLines,Teff,freq_BG3,0,0);
					gr8->SetMarkerColor(kBlack);
					gr8->SetMarkerSize(0.5);
					gr8->SetMarkerStyle(7);
					gr8->SetName("gr8");
					gr8->Draw("PSAME");
				
				//3.5vF//
				TGraphErrors *gr9 = new TGraphErrors(NumOfLines,Teff,freq_BG4,0,0);
					gr9->SetMarkerColor(kBlack);
					gr9->SetMarkerSize(0.5);
					gr9->SetMarkerStyle(7);
					gr9->SetFillColor(kWhite);
					gr9->SetName("gr9");
					gr9->Draw("PSAME");
				
				//4.5vF//
				TGraphErrors *gr10 = new TGraphErrors(NumOfLines,Teff,freq_BG5,0,0);
					gr10->SetMarkerColor(kBlack);
					gr10->SetMarkerSize(0.5);
					gr10->SetMarkerStyle(7);
					gr10->SetFillColor(kWhite);
					gr10->SetName("gr10");
					gr10->Draw("PSAME");
					
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
					
				TLegend *leg = new TLegend(0.1,0.7,0.28,0.9);
					leg->AddEntry("gr1","F","p");
					leg->AddEntry("gr2","1O","p");
					leg->AddEntry("gr3","2O","p");
					leg->AddEntry("gr4","3O","p");
					leg->AddEntry("gr5","4O","p");
					leg->Draw();
					
				
			c1->cd(2);
				TH1F *hr2 = c1_2->DrawFrame(xLowerLimit_2,yLowerLimit_2,xUpperLimit_2,yUpperLimit_2);
					hr2->SetTitle(PlotTitle_2);
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
	
	return 0;
	
}
