#pragma once
//int HG_1(long InitialTime, long TimeLimit, vector<vector<int>>& Group, vector<vector<int>>& ComponentofGroup)
int HG_1(vector<vector<int>>& Group, vector<vector<int>>& ComponentofGroup)
{
	int MakeSpan = 0;
	Group.resize(PCBType.size());
	ComponentofGroup = Group;

	for (int i = 0; i < Group.size(); i++)
	{
		Group[i].clear();
		Group[i].push_back(i);
	}

	for (int i = 0; i < ComponentofGroup.size(); i++)
		GetComponentofAGroup(Group[i], ComponentofGroup[i]);

	int Flag = 1;
	while (Flag > 0)
	{
		Flag = -1;
//		if (GetElapsedProcessTime() - InitialTime > TimeLimit)
//			break;

		int GroupSeqFlag1 = -1;
		int GroupSeqFlag2 = -1;
		for (int i = 0; i < Group.size(); i++)
		{
			for (int j = i + 1; j < Group.size(); j++)
			{
				vector<int>ComComponentofTwoGroup;
				int NuMofComComponent = GetComComponentofTwoGroup(Group[i], Group[j], ComComponentofTwoGroup);
				vector<int>TotalComponentofTwoGroup;
				int NumofTotalComponent = GetTotalComponentofTwoGroup(Group[i], Group[j], TotalComponentofTwoGroup);

				if ((NumofTotalComponent <= gCapacity) && (NuMofComComponent > Flag))
				{
					Flag = NuMofComComponent;
					GroupSeqFlag1 = i;
					GroupSeqFlag2 = j;
				}
			}
		}
		if (Flag > 0)
			MakeSpan = Merge(Group, GroupSeqFlag1, GroupSeqFlag2, ComponentofGroup);
	}
	CheckChrom(Group, ComponentofGroup, MakeSpan);
	return MakeSpan;
}

int HG_2(int MergeRule, int k, int RepairRule, vector<vector<int>>& Group, vector<vector<int>>& ComponentofGroup)
{
	long InitialTime = GetElapsedProcessTime();
	long TimeLimit = 120 * 1000;
//	HG_1(InitialTime, TimeLimit, Group, ComponentofGroup);
	HG_1(Group, ComponentofGroup);

	int Flag = 1;
	while (Flag)
	{
		int FirstMakeSpan = Swap(Group, ComponentofGroup);

		vector<vector<int>>TempGroup = Group;
		vector<vector<int>>TempComponentofGroup = ComponentofGroup;

		int TryMergeSpan = TryMerge(MergeRule, k, RepairRule, TempGroup, TempComponentofGroup);

		if (TryMergeSpan < FirstMakeSpan)
		{
			FirstMakeSpan = TryMergeSpan;
			Group = TempGroup;
			ComponentofGroup = TempComponentofGroup;
		}
		else
			Flag = 0;
	}

	int MakeSpan = GetMakeSpan(Group, ComponentofGroup);
	return MakeSpan;
}

int HG_3(int MergeRule, int k, int RepairRule, vector<vector<int>>& Group, vector<vector<int>>& ComponentofGroup)
{
	int MakeSpanofHG_2 = HG_2(MergeRule, k, RepairRule, Group, ComponentofGroup);

	vector<vector<int>>TempGroup = Group;
	vector<vector<int>>TempComponentofGroup = ComponentofGroup;

	int GroupSeqFlag1 = -1;
	int GroupSeqFlag2 = -1;
	SelectGroupPair(TempGroup, TempComponentofGroup, MergeRule, k, GroupSeqFlag1, GroupSeqFlag2);

	Merge(TempGroup, GroupSeqFlag1, GroupSeqFlag2, TempComponentofGroup);

	long InitialTimeforRepeat = GetElapsedProcessTime();
	long TimelimitforRepeat = 60 * 1000;

	while (GetElapsedProcessTime() - InitialTimeforRepeat < TimelimitforRepeat)
	{
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
			else
			{
				DoMoveSwap(TempGroup, TempComponentofGroup);

				int Flag2 = 0;
				for (int i = 0; i < TempComponentofGroup.size(); i++)
				{
					if (TempComponentofGroup[i].size() > gCapacity)
						Flag2++;
				}
				if (Flag2 == 0)
				{
					Group = TempGroup;
					ComponentofGroup = TempComponentofGroup;
				}
			}
		}
	}
	int MakeSpan = GetMakeSpan(Group, ComponentofGroup);
	return MakeSpan;
}

//int HGS_1(long InitialTime, long TimeLimit, vector<vector<int>>& Group, vector<vector<int>>& ComponentofGroup)
int HGS_1(vector<vector<int>>& Group, vector<vector<int>>& ComponentofGroup)
{
	Group.resize(PCBType.size());
	ComponentofGroup = Group;

	for (int i = 0; i < Group.size(); i++)
	{
		Group[i].clear();
		Group[i].push_back(i);
	}

	for (int i = 0; i < ComponentofGroup.size(); i++)
		GetComponentofAGroup(Group[i], ComponentofGroup[i]);

	int Flag = 1;
	while (Flag)
	{
//		if (GetElapsedProcessTime() - InitialTime > TimeLimit)
//		{
//			Flag = 0;
//			break;
//		}

		Flag = 0;
		vector<vector<int>>GroupPair;
		GetDescendingOrderofGroupPair(Group, GroupPair);

		for (int i = 0; i < GroupPair.size(); i++)
		{
			vector<vector<int>>TempGroup = Group;
			vector<vector<int>>TempComponentofGroup = ComponentofGroup;
			Merge(TempGroup, GroupPair[i][0], GroupPair[i][1], TempComponentofGroup);
			if (TempComponentofGroup[GroupPair[i][0]].size() <= gCapacity)
			{
				Group = TempGroup;
				ComponentofGroup = TempComponentofGroup;
				Flag = 1;
				break;
			}
		}
	}
	int MakeSpan = GetMakeSpan(Group, ComponentofGroup);
	return MakeSpan;
}

int HGS_2(vector<vector<int>>& Group, vector<vector<int>>& ComponentofGroup, int MergeRule, int k, int RepairRule)
{
	HGS_1(Group, ComponentofGroup);
	int MakeSpan = HG_2(MergeRule, k, RepairRule, Group, ComponentofGroup);
	return MakeSpan;
}