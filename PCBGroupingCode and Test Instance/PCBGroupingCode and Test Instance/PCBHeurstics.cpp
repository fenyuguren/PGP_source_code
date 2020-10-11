#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <windows.h>  
#include <conio.h>  
#include "math.h"
#include "limits.h"
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

using namespace std;
#include "Com.h"
#include "HG.h"
#include "Heuristic.h"
#include "NewHeuristics.h"
#include "Heuristics+LocalSearch.h"

/*int main()
{
	for (int ins = 99;ins < 100;ins++)
	{
		Read(ins);
		long InitialTime = GetElapsedProcessTime();
		vector<vector<int>>Group;
		vector<vector<int>>ComponentofGroup;
//		int Span = HG_1(Group, ComponentofGroup);
//		int Span = HG_2(1, 0, 2, Group, ComponentofGroup);
//		int Span = HG_3(1, 0, 2, Group, ComponentofGroup);
//		int Span = HGS_1(Group, ComponentofGroup);
		int Span = HGS_2(Group, ComponentofGroup, 1, 0, 2);
//		int Span = Heuristic_1(0.7);
//		int Span = Heuristic_BasedOnPCB(PCBType, Group, ComponentofGroup);
//		int Span = Heuristic_BasedOnPCBpair(PCBType, Group, ComponentofGroup);
//		int Span = HeuristicLocalSearch_BasedOnPCB(PCBType, Group, ComponentofGroup);
//		int Span = HeuristicLocalSearch_BasedOnPCBpair(PCBType, Group, ComponentofGroup);
//		int Span = HeuristicLocalSearch_Basedonswapandmove_BasedOnPCB(PCBType, Group, ComponentofGroup);
//		int Span = HeuristicLocalSearch_Basedonswapandmove_BasedOnPCBPair(PCBType, Group, ComponentofGroup);
		cout << ins << "\t" << Span << "\t" << (GetElapsedProcessTime() - InitialTime) / 1000.0 << endl;
		CheckChrom(Group, ComponentofGroup, Span);
	}
}*/

/*int main()
{
	ostringstream str1;
	str1 << "Heuristic_BasedOnPCB_3_Time_96_99" << ".txt";               //
	ofstream ofile1;
	ofile1.open(str1.str());

	ostringstream str2;
	str2 << "Heuristic_BasedOnPCB_LS_3_Time_96_99" << ".txt";               //
	ofstream ofile2;
	ofile2.open(str2.str());

	ostringstream str3;
	str3 << "Heuristic_BasedOnPCB_3_96_99" << ".txt";               //
	ofstream ofile3;
	ofile3.open(str3.str());

	ostringstream str4;
	str4 << "Heuristic_BasedOnPCB_LS_3_96_99" << ".txt";               //
	ofstream ofile4;
	ofile4.open(str4.str());

	for (int ins = 69; ins < 100; ins++)
	{
		Read(ins);
		for (int time = 0; time < 30; time++)
		{
			long InitialTime = GetElapsedProcessTime();
			srand(1994 + time);

			vector<vector<int>>Group;
			vector<vector<int>>ComponentofGroup;

			int Span1 = Heuristic_BasedOnPCB(Group, ComponentofGroup);
			long ProcessingTime1 = (GetElapsedProcessTime() - InitialTime) / 1000.0;
			ofile1 << ProcessingTime1 << "\t";
			ofile3 << Span1 << "\t";
			cout << Span1 << "\t";


			vector<vector<int>>TempGroup = Group;
			vector<vector<int>>TempComponentofGroup = ComponentofGroup;
			int TempMakeSpan = Merge_3(TempGroup, TempComponentofGroup);
			if (TempMakeSpan <= Span1)
			{
				Span1 = TempMakeSpan;
				Group = TempGroup;
				ComponentofGroup = TempComponentofGroup;
			}

			Span1 = Swap_3(Group, ComponentofGroup);

			bool Flag = true;
			while (Flag)
			{
				Flag = false;
				TempGroup = Group;
				TempComponentofGroup = ComponentofGroup;
				int TempSpan = Move_3(TempGroup, TempComponentofGroup);
				if (TempSpan < Span1)
				{
					Span1 = TempSpan;
					Group = TempGroup;
					ComponentofGroup = TempComponentofGroup;
					Flag = true;
				}
			}
			long ProcessingTime2 = (GetElapsedProcessTime() - InitialTime) / 1000.0;

			ofile2 << ProcessingTime2 << "\t";
			ofile4 << Span1 << "\t";
			cout << Span1 << "\t";
			CheckChrom(Group, ComponentofGroup, Span1);
		}
		cout << endl;
		ofile1 << endl;
		ofile2 << endl;
		ofile3 << endl;
		ofile4 << endl;
		
	}
	ofile1.close();
	ofile2.close();
	ofile3.close();
	ofile4.close();

	return 0;
}*/

//HG_1
/*int main()
{
	cout << "HG_1" << endl;
	ostringstream str1;
	str1 << "HG_1" << ".txt";               //
	ofstream ofile1;
	ofile1.open(str1.str());

	for (int ins = 0; ins < 100; ins++)
	{
		cout << "ins:" << "\t" << ins << endl;
		long InitialTime = GetElapsedProcessTime();

		Read(ins);
		vector<vector<int>>Group;
		vector<vector<int>>ComponentofGroup;

		int Span1 = HG_1(Group, ComponentofGroup);
		long ProcessingTime1 = (GetElapsedProcessTime() - InitialTime) / 1000.0;

		ofile1 << ProcessingTime1 << "\t" << Span1 << "\t" << endl;
		cout << ProcessingTime1 << "\t" << Span1 << endl;
	}
	ofile1.close();
}*/

//HG_2
/*int main()
{
	cout << "HG_2" << endl;

	ostringstream str1;
	str1 << "HG_2" << ".txt";               //
	ofstream ofile1;
	ofile1.open(str1.str());

	for (int ins = 0; ins < 100; ins++)
	{
    	cout << "ins:" << "\t" << ins << endl;
		long InitialTime = GetElapsedProcessTime();

		Read(ins);
		vector<vector<int>>Group;
		vector<vector<int>>ComponentofGroup;

		int Span1 = HG_2(1, 0, 2, Group, ComponentofGroup);
		long ProcessingTime1 = (GetElapsedProcessTime() - InitialTime) / 1000.0;

		ofile1 << ProcessingTime1 << "\t" << Span1 << "\t" << endl;
		cout << ProcessingTime1 << "\t" << Span1 << endl;
	}
	ofile1.close();
}*/

// HG_3
/*int main()
{
	cout << "HG_3" << endl;
	ostringstream str1;
	str1 << "HG_3" << ".txt";               //
	ofstream ofile1;
	ofile1.open(str1.str());

	for (int ins = 0; ins < 100; ins++)
	{
     	cout << "ins:" << "\t" << ins << endl;
		long InitialTime = GetElapsedProcessTime();

		Read(ins);
		vector<vector<int>>Group;
		vector<vector<int>>ComponentofGroup;

		int Span1 = HG_3(1, 0, 2, Group, ComponentofGroup);
		long ProcessingTime1 = (GetElapsedProcessTime() - InitialTime) / 1000.0;

		ofile1 << ProcessingTime1 << "\t" << Span1 << "\t" << endl;
		cout << ProcessingTime1 << "\t" << Span1 << endl;
	}
	ofile1.close();
}*/

//HGS_1
/*int main()
{
	cout << "HGS_1" << endl;

	ostringstream str1;
	str1 << "HGS_1" << ".txt";               //
	ofstream ofile1;
	ofile1.open(str1.str());

	for (int ins = 0; ins < 100; ins++)
	{
		cout << "ins:" << "\t" << ins << endl;
		long InitialTime = GetElapsedProcessTime();

		Read(ins);
		vector<vector<int>>Group;
		vector<vector<int>>ComponentofGroup;

		int Span1 = HGS_1(Group, ComponentofGroup);;
		long ProcessingTime1 = (GetElapsedProcessTime() - InitialTime) / 1000.0;

		ofile1 << ProcessingTime1 << "\t" << Span1 << "\t" << endl;
		cout << ProcessingTime1 << "\t" << Span1 << endl;
	}
	ofile1.close();
}*/

//HGS_2
/*int main()
{
	cout << "HGS_2" << endl;

	ostringstream str1;
	str1 << "HGS_2" << ".txt";               //
	ofstream ofile1;
	ofile1.open(str1.str());

	for (int ins = 0; ins < 100; ins++)
	{
		cout << "ins:" << "\t" << ins << endl;
		long InitialTime = GetElapsedProcessTime();

		Read(ins);
		vector<vector<int>>Group;
		vector<vector<int>>ComponentofGroup;

		int Span1 = HGS_2(Group, ComponentofGroup, 1, 0, 2);
		long ProcessingTime1 = (GetElapsedProcessTime() - InitialTime) / 1000.0;

		ofile1 << ProcessingTime1 << "\t" << Span1 << "\t" << endl;
		cout << ProcessingTime1 << "\t" << Span1 << endl;
	}
	ofile1.close();
}*/

//会议论文里的启发式
/*int main()
{
	cout << "HEU_Old" << endl;

	ostringstream str1;
	str1 << "HEU_Old" << ".txt";               //
	ofstream ofile1;
	ofile1.open(str1.str());

	for (int ins = 0; ins < 100; ins++)
	{
		cout << "ins:" << "\t" << ins << endl;
		long InitialTime = GetElapsedProcessTime();

		Read(ins);
		vector<vector<int>>Group;
		vector<vector<int>>ComponentofGroup;

		int Span1 = Heuristic_ByMyself(Group, ComponentofGroup);
		long ProcessingTime1 = (GetElapsedProcessTime() - InitialTime) / 1000.0;

		ofile1 << ProcessingTime1 << "\t" << Span1 << "\t" << endl;
		cout << ProcessingTime1 << "\t" << Span1 << endl;
	}
	ofile1.close();
}*/

void main()
{
	vector<vector<float>> TotalResults(100);      //
	vector<vector<float>> Results_ind;
	string FileDirectory = "Result\\";
	int Instances = 100, Reps = 1;

	for (int sf = 0; sf < 2; sf++)
	{
		string sfname;
		switch (sf)
		{
		case 0:sfname = "Heuristic_BasedOnPCBAV"; break;
//		case 1:sfname = "Heuristic_BasedOnPCB_LSAV"; break;
		case 1:sfname = "Heuristic_BasedOnPCBPairAV"; break;
//		case 1:sfname = "Heuristic_BasedOnPCBPair_LSAV"; break;
	//	case 4:sfname = "HEU_byMyselfAV"; break;
	//	case 5:sfname = "HG_1AV"; break;
	//	case 6:sfname = "HG_2AV"; break;
	//	case 7:sfname = "HGS_1AV"; break;
	//	case 8:sfname = "HGS_2AV"; break;
	//	case 9:sfname = "HEUAV"; break;
		}
		string Filename;
		ostringstream str;
		str << sfname << ".txt";
		Filename = str.str();
		ReadCalFile(FileDirectory + Filename, Instances, Reps, Results_ind);

		for (int ins = 0; ins < Results_ind.size(); ins++)
		{
			for (int Rep = 0; Rep < Results_ind[ins].size(); Rep++)
				TotalResults[ins].push_back(Results_ind[ins][Rep]);
		}
	}

	vector <int> minValue(Instances);

	for (int ins = 0; ins < TotalResults.size(); ins++)
	{
		minValue[ins] = *min_element(TotalResults[ins].begin(), TotalResults[ins].end());
		cout << ins << "\t" << minValue[ins] << endl;
	}

	vector<vector<double>> RDI(Instances);
	for (int ins = 0; ins < Instances; ins++)
		RDI[ins].resize(TotalResults[ins].size());
	for (int ins = 0; ins < TotalResults.size(); ins++)
	{
		for (int j = 0; j < TotalResults[ins].size(); j++)
			RDI[ins][j] = 100.00 * (TotalResults[ins][j] - minValue[ins]) / minValue[ins];
	}

	ofstream ofile;
	ostringstream strof;
	strof << "Single_Pair_HEU_RPI" << ".txt";
	//		strof << "table_" << ".txt";
	ofile.open(FileDirectory + strof.str());
	ofile << fixed << setprecision(2);
	ofile << "ins" << "\t" << "sf" << "\t" << "NumPCBtype" << "\t" << "NumComponent" << "\t" << "TotalResults" << "\t" << "RDI" << endl;

	for (int ins = 0; ins < TotalResults.size(); ins++)
	{
		Read(ins);
		Reps = 1;
//		int sfArray[] = { 0, 1,2,3,4,5,6,7,8,9 };   //
//		int sfArray[] = { 0, 1,2,3 };   //
		int sfArray[] = { 0, 1 };   //

		double sum5 = 0;
		int sump = 0;
		for (int j = 0; j < TotalResults[ins].size(); j++)
		{
			int Temp = j / Reps;
			int sf = sfArray[Temp];

			string sfname;
			switch (sf)
			{
			case 0:sfname = "HP"; break;
//			case 1:sfname = "HPL"; break;
			case 1:sfname = "HPP"; break;
//			case 1:sfname = "HPPL"; break;
//			case 4:sfname = "HEU1"; break;
//			case 5:sfname = "HG1"; break;
//			case 6:sfname = "HG2"; break;
//			case 7:sfname = "HGS1"; break;
//			case 8:sfname = "HGS2"; break;
//			case 9:sfname = "HEU"; break;
			}
			ofile << ins << "\t" << sfname << "\t" << gNumPCBtype << "\t" << gNumComponent << "\t" << TotalResults[ins][j] << "\t" << RDI[ins][j] << endl;
		}
	}
	ofile.close();
}

/*int main()
{
	cout << "HEU" << endl;
	ostringstream str1;
	str1 << "HEU" << ".txt";               //
	ofstream ofile1;
	ofile1.open(str1.str());

	for (int ins = 0; ins < 100; ins++)
	{
		cout << "ins:" << "\t" << ins << endl;
		long InitialTime = GetElapsedProcessTime();

		Read(ins);
		float Level = 0.0;
		int Span1 = Heuristic_1(Level);
		long ProcessingTime1 = (GetElapsedProcessTime() - InitialTime) / 1000.0;

		ofile1 << ProcessingTime1 << "\t" << Span1 << "\t" << endl;
		cout << ProcessingTime1 << "\t" << Span1 << endl;
	}
	ofile1.close();
}*/