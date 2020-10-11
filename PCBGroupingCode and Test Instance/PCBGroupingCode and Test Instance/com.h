#pragma once

//Globle variables
int gCapacity, gNumPCBtype, gNumComponent;
int SetupTimeofAPCBgroup = 10;
int SetupTimeofAComponent = 1;

vector<vector<int>>PCBType;  //the elements in the row represent that the components is occupied in which PCB

void GetInstance(int NO)
{
	for (int m = 0; m < 10; m++)
	{
		PCBType.resize(gNumPCBtype);
		for (int i = 0; i < PCBType.size(); i++)
		{
			PCBType[i].resize(gNumComponent);
			for (int j = 0; j < PCBType[i].size(); j++)
				PCBType[i][j] = 0;
		}

		for (int i = 0; i < PCBType.size(); i++)
		{
			int NumComponentOnOnePCB = -1;
			do
			{
				NumComponentOnOnePCB = rand() % 12 + 5;
			} while (NumComponentOnOnePCB > 12);

			for (int j = 0; j < NumComponentOnOnePCB; j++)
				PCBType[i][j] = 1;

			random_shuffle(PCBType[i].begin(), PCBType[i].end());
		}


		//print the instance
		char FileName[100] = "PCBType_", Buf[100] = "";
		_itoa_s(NO, Buf, 10);
		strcat_s(FileName, Buf);
		strcat_s(FileName, "_");
		_itoa_s(m, Buf, 10);
		strcat_s(FileName, Buf);
		strcat_s(FileName, ".txt");

		ofstream ofile;
		ofile.open(FileName);
		ofile << gNumPCBtype << "\t" << gNumComponent << "\t" << gCapacity << endl;
		for (int type = 0; type < PCBType.size(); type++)
		{
			for (int component = 0; component < PCBType[type].size(); component++)
				ofile << PCBType[type][component] << "\t";

			ofile << endl;
		}
	}
}

void Read(int NO)
{
	int Size = NO / 10;
	int Num = NO % 10;

	//print the instance
	char FName[100] = "PCBType_", Buf[100] = "";
	_itoa_s(Size, Buf, 10);
	strcat_s(FName, Buf);
	strcat_s(FName, "_");
	_itoa_s(Num, Buf, 10);
	strcat_s(FName, Buf);
	strcat_s(FName, ".txt");

	ifstream ifile;
	ifile.open(FName);
	ifile >> gNumPCBtype >> gNumComponent >> gCapacity;

	PCBType.resize(gNumPCBtype);
	for (int i = 0; i < PCBType.size(); i++)
		PCBType[i].resize(gNumComponent);

	for (int type = 0; type < PCBType.size(); type++)
	{
		for (int component = 0; component < PCBType[type].size(); component++)
		{
			ifile >> PCBType[type][component];
		}
	}
	ifile.close();

	/*	for (int i = 0; i < PCBType.size(); i++)
		{
			for (int j = 0; j < PCBType[i].size(); j++)
				cout << PCBType[i][j] << "\t";
			cout << endl;
		}*/
}

//获取时间函数
long GetElapsedProcessTime()
{
	FILETIME createTime;
	FILETIME exitTime;
	FILETIME kernelTime;
	FILETIME userTime;

	if (GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTime, &userTime) != 0)
	{
		//  Returns total user time.
		SYSTEMTIME userSystemTime;
		if (FileTimeToSystemTime(&userTime, &userSystemTime) != -1)
			return (userSystemTime.wDay - 1) * 24 * 3600 * 1000
			+ userSystemTime.wHour * 3600 * 1000 +
			userSystemTime.wMinute * 60 * 1000 +
			userSystemTime.wSecond * 1000 +
			userSystemTime.wMilliseconds;
		else
			return 0;
	}
	else
		return 0;
}

int GetMakeSpan(vector<vector<int>>Group, vector<vector<int>>ComComponentofGroup)
{
	int ChangeOver = 0;
	for (int i = 0; i < ComComponentofGroup.size(); i++)
		ChangeOver = ChangeOver + ComComponentofGroup[i].size();

	ChangeOver = ChangeOver + Group.size() * 10;

	return ChangeOver;
}

void ReadCalFile(string FName, int Instances, int Reps, vector<vector<float>>& Results)
{
	Results.clear();
	Results.resize(Instances);
	for (int j = 0; j < Instances; j++)
		Results[j].resize(Reps);
	ifstream ifile;
	ifile.open(FName);
	for (int i = 0; i < Instances; i++)
		for (int j = 0; j < Reps; j++)
			ifile >> Results[i][j];
	ifile.close();
}

void CheckChrom(vector<vector<int>>Group, vector<vector<int>>ComponentofGroup, int Span)
{
	vector <bool> bExist(gNumPCBtype, false);

	for (int f = 0; f < Group.size(); f++)
	{
		for (int j = 0; j < Group[f].size(); j++)
			bExist[Group[f][j]] = true;
	}

	for (int i = 0; i < bExist.size(); i++)
	{
		if (!bExist[i]) printf("/nchrom is error! PCB %d is lost", i);
	}

	int SolSize = 0;
	for (int i = 0; i < Group.size(); i++)
	{
		for (int j = 0; j < Group[i].size(); j++)
			SolSize++;
	}

	if (SolSize != gNumPCBtype)
		printf("Permutation is error");

	if (Span > 0)
	{
		int TempSpan = GetMakeSpan(Group, ComponentofGroup);
		if (TempSpan != Span)
			printf("\n Span is Error! %d,%d", TempSpan, Span);
	}

	for (int i = 0; i < ComponentofGroup.size(); i++)
	{
		if (ComponentofGroup[i].size() > gCapacity)
			cout << "Group " << i << "\t" << "is Out of Capacity" << endl;
	}
}

//找到一个数组中的所有非零元素所在的位置,即找出一个PCB板子上没有被加工的元件
void GetComponentOnAPCB(vector<int>Seq, vector<int>& ComponentOnAPCB)
{
	ComponentOnAPCB.clear();
	for (int i = 0; i < Seq.size(); i++)
	{
		if (Seq[i] != 0)
			ComponentOnAPCB.push_back(i);
	}
}

//找到2个PCB板上共同的元素
int GetComComponentofTwoPCB(vector<int>PCBSeq1, vector<int>PCBSeq2, vector<int>& ComComponent)
{
	ComComponent.clear();
	for (int i = 0; i < PCBSeq1.size(); i++)
	{
		if ((PCBSeq1[i] == PCBSeq2[i]) && (PCBSeq1[i] == 1))
			ComComponent.push_back(i);
	}
	return ComComponent.size();
}

//计算1个Group中的Component
int GetComponentofAGroup(vector<int>OneGroupSeq, vector<int>& ComponentofAGroup)
{
	ComponentofAGroup.clear();
	for (int i = 0; i < PCBType[0].size(); i++)
	{
		for (int j = 0; j < OneGroupSeq.size(); j++)
		{
			if (PCBType[OneGroupSeq[j]][i] == 1)
			{
				ComponentofAGroup.push_back(i);
				break;
			}
		}
	}
	return ComponentofAGroup.size();
}

//计算2个Group中的共同元素
int GetComComponentofTwoGroup(vector<int>OneGroupSeq1, vector<int>OneGroupSeq2, vector<int>& ComComponentofTwoGroup)
{
	ComComponentofTwoGroup.clear();
	vector<int>ComponentInGroupSeq1;
	ComponentInGroupSeq1.clear();
	vector<int>ComponentInGroupSeq2;
	ComponentInGroupSeq2.clear();

	for (int i = 0; i < PCBType[0].size(); i++)
	{
		for (int j = 0; j < OneGroupSeq1.size(); j++)
		{
			if (PCBType[OneGroupSeq1[j]][i] == 1)
			{
				ComponentInGroupSeq1.push_back(i);
				break;
			}
		}
	}

	for (int i = 0; i < PCBType[0].size(); i++)
	{
		for (int j = 0; j < OneGroupSeq2.size(); j++)
		{
			if (PCBType[OneGroupSeq2[j]][i] == 1)
			{
				ComponentInGroupSeq2.push_back(i);
				break;
			}
		}
	}

	for (int i = 0; i < ComponentInGroupSeq1.size(); i++)
	{
		for (int j = 0; j < ComponentInGroupSeq2.size(); j++)
		{
			if (ComponentInGroupSeq1[i] == ComponentInGroupSeq2[j])
			{
				ComComponentofTwoGroup.push_back(ComponentInGroupSeq1[i]);
				break;
			}
		}
	}

	return ComComponentofTwoGroup.size();
}

//计算2个Group所包含的所有的元素
int GetTotalComponentofTwoGroup(vector<int>OneGroupSeq1, vector<int>OneGroupSeq2, vector<int>& TotalComponentofTwoGroup)
{
	TotalComponentofTwoGroup.clear();
	vector<int>ComponentInGroupSeq1;
	ComponentInGroupSeq1.clear();
	vector<int>ComponentInGroupSeq2;
	ComponentInGroupSeq2.clear();

	for (int i = 0; i < PCBType[0].size(); i++)
	{
		for (int j = 0; j < OneGroupSeq1.size(); j++)
		{
			if (PCBType[OneGroupSeq1[j]][i] == 1)
			{
				ComponentInGroupSeq1.push_back(i);
				break;
			}
		}
	}

	for (int i = 0; i < PCBType[0].size(); i++)
	{
		for (int j = 0; j < OneGroupSeq2.size(); j++)
		{
			if (PCBType[OneGroupSeq2[j]][i] == 1)
			{
				ComponentInGroupSeq2.push_back(i);
				break;
			}
		}
	}

	for (int i = 0; i < ComponentInGroupSeq1.size(); i++)
	{
		int Flag = 0;
		for (int j = 0; j < ComponentInGroupSeq2.size(); j++)
		{
			if (ComponentInGroupSeq1[i] == ComponentInGroupSeq2[j])
			{
				Flag = 0;
				break;
			}
			else
				Flag = 1;
		}
		if (Flag == 1)
			TotalComponentofTwoGroup.push_back(ComponentInGroupSeq1[i]);
	}

	for (int i = 0; i < ComponentInGroupSeq2.size(); i++)
		TotalComponentofTwoGroup.push_back(ComponentInGroupSeq2[i]);

	return TotalComponentofTwoGroup.size();
}

int Merge(vector<vector<int>>& Group, int GroupSeqFlag1, int GroupSeqFlag2, vector<vector<int>>& ComponentofGroup)
{
	for (int i = 0; i < Group[GroupSeqFlag2].size(); i++)
		Group[GroupSeqFlag1].push_back(Group[GroupSeqFlag2][i]);

	vector<int>NewComponentofGroupSeq;
	NewComponentofGroupSeq.clear();
	for (int i = 0; i < PCBType[0].size(); i++)
	{
		for (int j = 0; j < Group[GroupSeqFlag1].size(); j++)
		{
			if (PCBType[Group[GroupSeqFlag1][j]][i] == 1)
			{
				NewComponentofGroupSeq.push_back(i);
				break;
			}
		}
	}
	ComponentofGroup[GroupSeqFlag1] = NewComponentofGroupSeq;
	Group.erase(Group.begin() + GroupSeqFlag2);
	ComponentofGroup.erase(ComponentofGroup.begin() + GroupSeqFlag2);

	int MakeSpan = GetMakeSpan(Group, ComponentofGroup);
	return MakeSpan;
}

int Swap(vector<vector<int>>& Group, vector<vector<int>>& ComponentofGroup)
{
	int SwapFlag = 1;
	while (SwapFlag)
	{
		SwapFlag = 0;

		for (int i = 0; i < Group.size(); i++)
		{
			for (int j = i + 1; j < Group.size(); j++)
			{
				for (int m = 0; m < Group[i].size(); m++)
				{
					for (int n = 0; n < Group[j].size(); n++)
					{
						vector<int>GroupSeq1 = Group[i];
						vector<int>GroupSeq2 = Group[j];
						int PCB1 = Group[i][m];
						int PCB2 = Group[j][n];

						GroupSeq1.erase(GroupSeq1.begin() + m);
						GroupSeq2.erase(GroupSeq2.begin() + n);
						GroupSeq1.push_back(PCB2);
						GroupSeq2.push_back(PCB1);

						int InitialNumofComponentofGroup1 = ComponentofGroup[i].size();
						int InitialNumofComponentofGroup2 = ComponentofGroup[j].size();

						vector<int>NewComponentofGroup1;
						vector<int>NewComponentofGroup2;
						int NumofComponentofNewGroup1 = GetComponentofAGroup(GroupSeq1, NewComponentofGroup1);
						int NumofComponentofNewGroup2 = GetComponentofAGroup(GroupSeq2, NewComponentofGroup2);

						if ((NumofComponentofNewGroup1 <= gCapacity) && (NumofComponentofNewGroup2 <= gCapacity))
						{
							if ((InitialNumofComponentofGroup1 + InitialNumofComponentofGroup2) > (NumofComponentofNewGroup1 + NumofComponentofNewGroup2))
							{
								SwapFlag = 1;
								Group[i] = GroupSeq1;
								Group[j] = GroupSeq2;

								ComponentofGroup[i] = NewComponentofGroup1;
								ComponentofGroup[j] = NewComponentofGroup2;
							}
						}
					}
				}
			}
		}
	}
	int MakeSpan = GetMakeSpan(Group, ComponentofGroup);
	return MakeSpan;
}
void SelectGroupPair(vector<vector<int>>Group, vector<vector<int>>ComponentofGroup, int MergeRule, int k, int& GroupSeqFlag1, int& GroupSeqFlag2)
{
	switch (MergeRule)
	{
	case 0: {
		int NumofComComponent = INT_MIN;
		for (int i = 0; i < Group.size(); i++)
		{
			for (int j = i + 1; j < Group.size(); j++)
			{
				vector<int>ComComponentofTwoGroup;
				int TempNumofComComponent = GetComComponentofTwoGroup(Group[i], Group[j], ComComponentofTwoGroup);
				if (TempNumofComComponent > NumofComComponent)
				{
					NumofComComponent = TempNumofComComponent;
					GroupSeqFlag1 = i;
					GroupSeqFlag2 = j;
				}
			}
		}
		break;
	}
	case 1: {
		int NumofTotalComponent = INT_MAX;
		for (int i = 0; i < Group.size(); i++)
		{
			for (int j = i + 1; j < Group.size(); j++)
			{
				vector<int>TotalComponentofTwoGroup;
				int TempNumofTotalComponent = GetTotalComponentofTwoGroup(Group[i], Group[j], TotalComponentofTwoGroup);
				if (TempNumofTotalComponent - gCapacity < NumofTotalComponent)
				{
					NumofTotalComponent = TempNumofTotalComponent - gCapacity;
					GroupSeqFlag1 = i;
					GroupSeqFlag2 = j;
				}
			}
		}
		break;
	}
	case 2: {
		int NumofComComponent = INT_MIN;
		for (int i = 0; i < Group.size(); i++)
		{
			for (int j = i + 1; j < Group.size(); j++)
			{
				vector<int>ComComponentofTwoGroup;
				int TempNumofComComponent = GetComComponentofTwoGroup(Group[i], Group[j], ComComponentofTwoGroup);
				vector<int>TotalComponentofTwoGroup;
				int TempNumofTotalComponent = GetTotalComponentofTwoGroup(Group[i], Group[j], TotalComponentofTwoGroup);

				if ((TempNumofComComponent > NumofComComponent) && (TempNumofTotalComponent - gCapacity < k))
				{
					NumofComComponent = TempNumofComComponent;
					GroupSeqFlag1 = i;
					GroupSeqFlag2 = j;
				}
			}
		}
		break;
	}
	}
}

void SelectGroupandPCB(vector<vector<int>>Group, vector<vector<int>>ComponentofGroup, int RepairRule, int IlligalGroupFlag, int& GroupFlag, int& PCBFlag)
{
	switch (RepairRule)
	{
	case 0: {
		int NumofIncreaseComponent = INT_MAX;
		for (int i = 0; i < Group[IlligalGroupFlag].size(); i++)
		{
			for (int j = 0; j < Group.size(); j++)
			{
				vector<int>IncreaseComponent;
				IncreaseComponent.clear();

				if (j != IlligalGroupFlag)
				{
					vector<int>ComponentOnAPCB;
					GetComponentOnAPCB(PCBType[Group[IlligalGroupFlag][i]], ComponentOnAPCB);

					for (int m = 0; m < ComponentOnAPCB.size(); m++)
					{
						for (int n = 0; n < ComponentofGroup[IlligalGroupFlag].size(); n++)
						{
							if (ComponentOnAPCB[m] == ComponentofGroup[IlligalGroupFlag][n])
							{
								IncreaseComponent.push_back(ComponentOnAPCB[m]);
								break;
							}
						}
					}
				}

				if (IncreaseComponent.size() < NumofIncreaseComponent)
				{
					NumofIncreaseComponent = IncreaseComponent.size();
					GroupFlag = j;
					PCBFlag = i;
				}
			}
		}
		break;
	}
	case 1: {
		int NumofDecreaseComponent = INT_MIN;
		for (int i = 0; i < Group[IlligalGroupFlag].size(); i++)
		{
			for (int j = 0; j < Group.size(); j++)
			{
				if (j != IlligalGroupFlag)
				{
					int InitialNumofComponent = ComponentofGroup[IlligalGroupFlag].size();
					vector<int>NewGroup = Group[IlligalGroupFlag];
					NewGroup.erase(NewGroup.begin() + i);
					vector<int>NewComponentofAGroup;
					int NewNumofComponent = GetComponentofAGroup(NewGroup, NewComponentofAGroup);
					if (InitialNumofComponent - NewNumofComponent > NumofDecreaseComponent)
					{
						NumofDecreaseComponent = InitialNumofComponent - NewNumofComponent;
						GroupFlag = j;
						PCBFlag = i;
					}
				}
			}
		}
		break;
	}
	case 2: {
		int NumofIncreaseComponent = INT_MIN;
		for (int i = 0; i < Group[IlligalGroupFlag].size(); i++)
		{
			for (int j = 0; j < Group.size(); j++)
			{
				vector<int>IncreaseComponent;
				IncreaseComponent.clear();

				if (j != IlligalGroupFlag)
				{
					int InitialNumofComponent = ComponentofGroup[IlligalGroupFlag].size();
					vector<int>NewGroup = Group[IlligalGroupFlag];
					NewGroup.erase(NewGroup.begin() + i);
					vector<int>NewComponentofAGroup;
					int NewNumofComponent = GetComponentofAGroup(NewGroup, NewComponentofAGroup);

					vector<int>ComponentOnAPCB;
					GetComponentOnAPCB(PCBType[Group[IlligalGroupFlag][i]], ComponentOnAPCB);

					for (int m = 0; m < ComponentOnAPCB.size(); m++)
					{
						for (int n = 0; n < ComponentofGroup[IlligalGroupFlag].size(); n++)
						{							
							if (ComponentOnAPCB[m] == ComponentofGroup[IlligalGroupFlag][n])
							{
								IncreaseComponent.push_back(ComponentOnAPCB[m]);
								break;								
							}
						}
					}
					int TempFlag = InitialNumofComponent - NewNumofComponent - IncreaseComponent.size();
					if (TempFlag > NumofIncreaseComponent)
					{
						NumofIncreaseComponent = TempFlag;
						GroupFlag = j;
						PCBFlag = i;
					}
				}
			}
		}
		break;
	}
	}
}

int TryMerge(int MergeRule, int k, int RepairRule, vector<vector<int>>& Group, vector<vector<int>>& ComponentofGroup)
{
	vector<vector<int>>TempGroup = Group;
	vector<vector<int>>TempComponentofGroup = ComponentofGroup;

	int GroupSeqFlag1 = -1;
	int GroupSeqFlag2 = -1;
	SelectGroupPair(Group, ComponentofGroup, MergeRule, k, GroupSeqFlag1, GroupSeqFlag2);

	Merge(TempGroup, GroupSeqFlag1, GroupSeqFlag2, TempComponentofGroup);

	int Flag = 0;
	if (TempComponentofGroup[GroupSeqFlag1].size() > gCapacity)
		Flag = 1;


	if (Flag == 1)
	{
		int TargetGroupFlag = -1;
		int PCBFlag = -1;
		SelectGroupandPCB(TempGroup, TempComponentofGroup, RepairRule, GroupSeqFlag1, TargetGroupFlag, PCBFlag);

		vector<int>TempTargetGroup = TempGroup[TargetGroupFlag];
		vector<int>TempComponentofTargetGroup;
		TempTargetGroup.push_back(TempGroup[GroupSeqFlag1][PCBFlag]);
		int TempNumofComponent = GetComponentofAGroup(TempTargetGroup, TempComponentofTargetGroup);

		if (TempNumofComponent <= gCapacity)
		{
			TempGroup[TargetGroupFlag].push_back(TempGroup[GroupSeqFlag1][PCBFlag]);
			TempGroup[GroupSeqFlag1].erase(TempGroup[GroupSeqFlag1].begin() + PCBFlag);

			GetComponentofAGroup(TempGroup[GroupSeqFlag1], TempComponentofGroup[GroupSeqFlag1]);
			GetComponentofAGroup(TempGroup[TargetGroupFlag], TempComponentofGroup[TargetGroupFlag]);

			Group = TempGroup;
			ComponentofGroup = TempComponentofGroup;

		}
	}
	int MakeSpan = GetMakeSpan(Group, ComponentofGroup);
	return MakeSpan;
}

void Move(vector<vector<int>>& Group, vector<vector<int>>& ComponentofGroup)
{
	int Flag = 1;
	while (Flag)
	{
		Flag = 0;
		for (int i = 0; i < Group.size(); i++)
		{
			for (int j = i + 1; j < Group.size(); j++)
			{
				for (int m = 0; m < Group[i].size(); m++)
				{
					int TempPCB = Group[i][m];
					vector<int>TempGroupSeq1 = Group[i];
					vector<int>TempGroupSeq2 = Group[j];
					TempGroupSeq1.erase(TempGroupSeq1.begin() + m);
					TempGroupSeq2.push_back(TempPCB);

					vector<int>ComponentofTempGroupSeq1;
					vector<int>ComponentofTempGroupSeq2;
					int NumComponentofGroupSeq1 = GetComponentofAGroup(TempGroupSeq1, ComponentofTempGroupSeq1);
					int NumComponentofGroupSeq2 = GetComponentofAGroup(TempGroupSeq2, ComponentofTempGroupSeq2);

					if ((NumComponentofGroupSeq1 <= gCapacity) && (NumComponentofGroupSeq2 <= gCapacity))
					{
						if ((NumComponentofGroupSeq1 + NumComponentofGroupSeq2) < (ComponentofGroup[i].size() + ComponentofGroup[j].size()))
						{
							Group[i] = TempGroupSeq1;
							Group[j] = TempGroupSeq2;

							ComponentofGroup[i] = ComponentofTempGroupSeq1;
							ComponentofGroup[j] = ComponentofTempGroupSeq2;

							Flag = 1;
							break;
						}
					}
				}
			}
		}
	}
}

void DoMoveSwap(vector<vector<int>>& Group, vector<vector<int>>& ComponentofGroup)
{
	int Flag = 1;
	while (Flag)
	{
		Flag = 0;

		//Move
		for (int i = 0; i < Group.size(); i++)
		{
			for (int j = i + 1; j < Group.size(); j++)
			{
				for (int m = 0; m < Group[i].size(); m++)
				{
					int TempPCB = Group[i][m];
					vector<int>TempGroupSeq1 = Group[i];
					vector<int>TempGroupSeq2 = Group[j];
					TempGroupSeq1.erase(TempGroupSeq1.begin() + m);
					TempGroupSeq2.push_back(TempPCB);

					vector<int>ComponentofTempGroupSeq1;
					vector<int>ComponentofTempGroupSeq2;
					int NumComponentofGroupSeq1 = GetComponentofAGroup(TempGroupSeq1, ComponentofTempGroupSeq1);
					int NumComponentofGroupSeq2 = GetComponentofAGroup(TempGroupSeq2, ComponentofTempGroupSeq2);
					if ((NumComponentofGroupSeq1 <= gCapacity) && (NumComponentofGroupSeq2 <= gCapacity))
					{
						if ((NumComponentofGroupSeq1 + NumComponentofGroupSeq2) < (ComponentofGroup[i].size() + ComponentofGroup[j].size()))
						{
							Group[i] = TempGroupSeq1;
							Group[j] = TempGroupSeq2;

							ComponentofGroup[i] = ComponentofTempGroupSeq1;
							ComponentofGroup[j] = ComponentofTempGroupSeq2;

							Flag = 1;
							break;
						}
					}
				}
			}
		}

		//Swap
		for (int i = 0; i < Group.size(); i++)
		{
			for (int j = i + 1; j < Group.size(); j++)
			{
				for (int m = 0; m < Group[i].size(); m++)
				{
					for (int n = 0; n < Group[j].size(); n++)
					{
						int TempPCB1 = Group[i][m];
						int TempPCB2 = Group[j][n];

						vector<int>TempGroupSeq1 = Group[i];
						vector<int>TempGroupSeq2 = Group[j];

						TempGroupSeq1.erase(TempGroupSeq1.begin() + m);
						TempGroupSeq2.erase(TempGroupSeq2.begin() + n);
						TempGroupSeq1.push_back(TempPCB2);
						TempGroupSeq2.push_back(TempPCB1);

						vector<int>ComponentofTempGroupSeq1;
						vector<int>ComponentofTempGroupSeq2;
						int NumComponentofGroupSeq1 = GetComponentofAGroup(TempGroupSeq1, ComponentofTempGroupSeq1);
						int NumComponentofGroupSeq2 = GetComponentofAGroup(TempGroupSeq2, ComponentofTempGroupSeq2);

						if ((NumComponentofGroupSeq1 <= gCapacity) && (NumComponentofGroupSeq2 <= gCapacity))
						{
							if ((NumComponentofGroupSeq1 + NumComponentofGroupSeq2) < (ComponentofGroup[i].size() + ComponentofGroup[j].size()))
							{
								Group[i] = TempGroupSeq1;
								Group[j] = TempGroupSeq2;

								ComponentofGroup[i] = ComponentofTempGroupSeq1;
								ComponentofGroup[j] = ComponentofTempGroupSeq2;

								m = 0;
								Flag = 1;
								break;
							}
						}
					}
				}
			}
		}
	}
}

void GetDecreasingOrder(vector<int>Seq, vector<int>& Order)   //按降序排列
{
	Order.clear();

	vector<vector<float>>Order_List;
	Order_List.resize(Seq.size());
	for (int i = 0; i < Order_List.size(); i++)
		Order_List[i].resize(2);

	for (int i = 0; i < Order_List.size(); i++)
	{
		Order_List[i][0] = i;
		Order_List[i][1] = Seq[i];
	}

	sort(Order_List.begin(), Order_List.end(), [](vector<float> t1, vector<float> t2)
		{
			return (t1[1] > t2[1]);
		});

	for (int i = 0; i < Order_List.size(); i++)
		Order.push_back(Order_List[i][0]);
}


void GetDescendingOrderofGroupPair(vector<vector<int>>Group, vector<vector<int>>& GroupPair)
{
	GroupPair.clear();

	vector<vector<int>>TempGroupPair;
	TempGroupPair.clear();

	for (int i = 0; i < Group.size(); i++)
	{
		for (int j = i + 1; j < Group.size(); j++)
		{
			vector<int>TempGroupPairSeq;
			TempGroupPairSeq.clear();
			TempGroupPairSeq.push_back(i);
			TempGroupPairSeq.push_back(j);
			TempGroupPair.push_back(TempGroupPairSeq);
		}
	}

	vector<int>NumofComComponentofGroupPair;
	NumofComComponentofGroupPair.resize(TempGroupPair.size());

	for (int i = 0; i < TempGroupPair.size(); i++)
	{
		vector<int>ComComponentofGroupPair;
		NumofComComponentofGroupPair[i] = GetComComponentofTwoGroup(Group[TempGroupPair[i][0]], Group[TempGroupPair[i][1]], ComComponentofGroupPair);
	}

	vector<int>DecreasingOrder;
	GetDecreasingOrder(NumofComComponentofGroupPair, DecreasingOrder);

	for (int i = 0; i < DecreasingOrder.size(); i++)
		GroupPair.push_back(TempGroupPair[DecreasingOrder[i]]);
}

