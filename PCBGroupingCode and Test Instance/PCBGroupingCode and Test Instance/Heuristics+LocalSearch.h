#pragma once
void Repair(int IllegalGroupFlag, vector<vector<int>>& Group, vector<vector<int>>& ComponentGroup)
{
	while (ComponentGroup[IllegalGroupFlag].size() > gCapacity)
	{
		//在GoupSeq1中找出一个元素与该group中的所有元素相似性最小的元素
		vector<float>SimilarityBetweenAPCBandAgroup;
		SimilarityBetweenAPCBandAgroup.resize(Group[IllegalGroupFlag].size());
		for (int i = 0; i < SimilarityBetweenAPCBandAgroup.size(); i++)
		{
			vector<int>TempGroupSeq = Group[IllegalGroupFlag];
			int TempPCB = TempGroupSeq[i];
			TempGroupSeq.erase(TempGroupSeq.begin() + i);
			vector<int>ComponentofTempGroupSeq;
			GetComponentofAGroup(TempGroupSeq, ComponentofTempGroupSeq);
			SimilarityBetweenAPCBandAgroup[i] = GetSimilarityBetweenAPCBandAGroup(TempPCB, ComponentofTempGroupSeq);
		}
		vector<int>IncreasingOrder;
		GetIncreasingOrder(SimilarityBetweenAPCBandAgroup, IncreasingOrder);

		int Flag = 0;
		vector<int>TempIncreasingOrder = IncreasingOrder;

		while (TempIncreasingOrder.size() != 0)
		{
			int PCBwithMinSimilarity = Group[IllegalGroupFlag][TempIncreasingOrder[0]];
			TempIncreasingOrder.erase(TempIncreasingOrder.begin() + 0);
			for (int i = 0; i < Group.size(); i++)
			{
				if (i != IllegalGroupFlag)
				{
					vector<int>TempGroupSeq = Group[i];
					TempGroupSeq.push_back(PCBwithMinSimilarity);
					vector<int>ComponentofTempGroupSeq;
					int TempNumofComponent = GetComponentofAGroup(TempGroupSeq, ComponentofTempGroupSeq);
					if (TempNumofComponent <= gCapacity)
					{
						Flag = 1;
						Group[i] = TempGroupSeq;
						ComponentGroup[i] = ComponentofTempGroupSeq;
						for (int j = 0; j < Group[IllegalGroupFlag].size(); j++)
						{
							if (Group[IllegalGroupFlag][j] == PCBwithMinSimilarity)
								Group[IllegalGroupFlag].erase(Group[IllegalGroupFlag].begin() + j);
						}
						GetComponentofAGroup(Group[IllegalGroupFlag], ComponentGroup[IllegalGroupFlag]);
						break;
					}
				}
			}
			if (Flag == 1)
				break;
		}

		if ((Flag != 1) && (TempIncreasingOrder.size() == 0))
		{
			vector<int>TempGroupSeq;
			TempGroupSeq.clear();
			TempGroupSeq.push_back(Group[IllegalGroupFlag][IncreasingOrder[0]]);
			vector<int>ComponentofTempGroupSeq;
			int TempNumofComponent = GetComponentofAGroup(TempGroupSeq, ComponentofTempGroupSeq);
			Group.push_back(TempGroupSeq);
			ComponentGroup.push_back(ComponentofTempGroupSeq);

			for (int j = 0; j < Group[IllegalGroupFlag].size(); j++)
			{
				if (Group[IllegalGroupFlag][j] == Group[IllegalGroupFlag][IncreasingOrder[0]])
					Group[IllegalGroupFlag].erase(Group[IllegalGroupFlag].begin() + j);
			}
			GetComponentofAGroup(Group[IllegalGroupFlag], ComponentGroup[IllegalGroupFlag]);
		}
	}
}

int Merge_3(vector<vector<int>>& Group, vector<vector<int>>& ComponentGroup)
{
	//Get GroupPair
	int GroupFlag1 = -1;
	int GroupFlag2 = -1;
	int TempNumofComponent = 0;
	for (int i = 0; i < Group.size(); i++)
	{
		for (int j = i + 1; j < Group.size(); j++)
		{
			vector<int>ComComponentofTwoGroup;
			int NumofComponent = GetComComponentofTwoGroup(Group[i], Group[j], ComComponentofTwoGroup);
			if (NumofComponent > TempNumofComponent)
			{
				TempNumofComponent = NumofComponent;
				GroupFlag1 = i;
				GroupFlag2 = j;
			}
		}
	}

	vector<int>TempGroup1 = Group[GroupFlag1];
	vector<int>TempGroup2 = Group[GroupFlag2];

	for (int i = 0; i < TempGroup2.size(); i++)
		TempGroup1.push_back(TempGroup2[i]);

	vector<int>TempComponentGroup1;
	GetComponentofAGroup(TempGroup1, TempComponentGroup1);
	if (TempComponentGroup1.size() <= gCapacity)
	{
		Group[GroupFlag1] = TempGroup1;
		ComponentGroup[GroupFlag1] = TempComponentGroup1;

		Group.erase(Group.begin() + GroupFlag2);
		ComponentGroup.erase(ComponentGroup.begin() + GroupFlag2);
	}
	int Span = GetMakeSpan(Group, ComponentGroup);
	return Span;
}

int Move_3(vector<vector<int>>& Group, vector<vector<int>>& ComponentofGroup)
{
	vector<int>EmptyGroupSeq;
	for (int i = 0; i < Group.size(); i++)
	{
		if (Group[i].size() == 0)
			EmptyGroupSeq.push_back(i);
	}

	for (int i = EmptyGroupSeq.size() - 1; i >= 0; i--)
	{
		Group.erase(Group.begin() + EmptyGroupSeq[i]);
		ComponentofGroup.erase(ComponentofGroup.begin() + EmptyGroupSeq[i]);
	}

	vector<vector<int>>TempGroup = Group;
	vector<vector<int>>TempComponentofGroup = ComponentofGroup;

	for (int i = 0; i < TempGroup.size(); i++)
	{
		for (int j = 0; j < TempGroup[i].size(); j++)
		{
			int TempPCB = TempGroup[i][j];
			int GroupPt = -1;
			for (int g = 0; g < TempGroup.size(); g++)
			{
				if (g != i)
				{
					TempGroup[g].push_back(TempPCB);
					TempGroup[i].erase(TempGroup[i].begin() + j);

					vector<int>TempComponentGroup1;
					int TempNumComponent1 = GetComponentofAGroup(TempGroup[i], TempComponentGroup1);
					vector<int>TempComponentGroup2;
					int TempNumComponent2 = GetComponentofAGroup(TempGroup[g], TempComponentGroup2);

					if (TempNumComponent2 <= gCapacity && (TempNumComponent1 + TempNumComponent2) <= (TempComponentofGroup[i].size() + TempComponentofGroup[g].size()))
						GroupPt = g;
				//	if (TempNumComponent2 <= gCapacity && (TempNumComponent1 + TempNumComponent2) <= Tempnum)
				//	{
				//		GroupPt = g;
				//		Tempnum = TempNumComponent1 + TempNumComponent2;
				//	}
					TempGroup[i] = Group[i];
					TempGroup[g] = Group[g];
				}
			}
			if (GroupPt != -1)
			{
				TempGroup[GroupPt].push_back(TempPCB);
				TempGroup[i].erase(TempGroup[i].begin() + j);

				GetComponentofAGroup(TempGroup[GroupPt], TempComponentofGroup[GroupPt]);
				GetComponentofAGroup(TempGroup[i], TempComponentofGroup[i]);

				EmptyGroupSeq.clear();
				for (int u = 0; u < TempGroup.size(); u++)
				{
					if (TempGroup[u].size() == 0)
						EmptyGroupSeq.push_back(u);
				}

				for (int u = EmptyGroupSeq.size() - 1; u >= 0; u--)
				{
					TempGroup.erase(TempGroup.begin() + EmptyGroupSeq[u]);
					TempComponentofGroup.erase(TempComponentofGroup.begin() + EmptyGroupSeq[u]);
				}
				break;
			}
		}
		Group = TempGroup;
		ComponentofGroup = TempComponentofGroup;
	}
	int MakeSpan = GetMakeSpan(Group, ComponentofGroup);
	return MakeSpan;
}

int Swap_3(vector<vector<int>>& Group, vector<vector<int>>& ComponentofGroup)
{
	vector<int>EmptyGroupSeq;
	for (int i = 0; i < Group.size(); i++)
	{
		if (Group[i].size() == 0)
			EmptyGroupSeq.push_back(i);
	}

	for (int i = EmptyGroupSeq.size() - 1; i >= 0; i--)
	{
		Group.erase(Group.begin() + EmptyGroupSeq[i]);
		ComponentofGroup.erase(ComponentofGroup.begin() + EmptyGroupSeq[i]);
	}

	vector<vector<int>>TempGroup = Group;
	vector<vector<int>>TempComponentofGroup = ComponentofGroup;

	for (int i = 0; i < TempGroup.size(); i++)
	{
		for (int j = 0; j < TempGroup[i].size(); j++)
		{
	//		int TempNum = INT_MAX;
			int GroupPt = -1;
			int PCBPt = -1;
			for (int g = 0; g < TempGroup.size(); g++)
			{
				if (g != i)
				{
					for (int p = 0; p < TempGroup[g].size(); p++)
					{
						int TempPCB = TempGroup[g][p];
						TempGroup[g][p] = TempGroup[i][j];
						TempGroup[i][j] = TempPCB;
						
						vector<int>TempComponentGroup1;
						int TempNumComponent1 = GetComponentofAGroup(TempGroup[i], TempComponentGroup1);
						vector<int>TempComponentGroup2;
						int TempNumComponent2 = GetComponentofAGroup(TempGroup[g], TempComponentGroup2);

				//		if ((TempNumComponent1 <= gCapacity) && (TempNumComponent2 <= gCapacity) && (TempNumComponent1 + TempNumComponent2) <= TempNum)
						if ((TempNumComponent1 <= gCapacity) && (TempNumComponent2 <= gCapacity) && (TempNumComponent1 + TempNumComponent2) <= TempComponentofGroup[i].size()+ TempComponentofGroup[g].size())
						{
							GroupPt = g;
							PCBPt = p;
						}
						TempGroup[i] = Group[i];
						TempGroup[g] = Group[g];
					}
				}
			}

			if (GroupPt != -1 && PCBPt != -1)
			{
				int TempPCB = TempGroup[GroupPt][PCBPt];
				TempGroup[GroupPt][PCBPt] = TempGroup[i][j];
				TempGroup[i][j] = TempPCB;
				break;
			}
		}
		Group = TempGroup;
		ComponentofGroup = TempComponentofGroup;
	}
	int MakeSpan = GetMakeSpan(Group, ComponentofGroup);
	return MakeSpan;
}