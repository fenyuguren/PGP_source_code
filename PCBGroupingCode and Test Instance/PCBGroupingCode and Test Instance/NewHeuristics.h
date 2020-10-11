#pragma once
int Heuristic_BasedOnPCB(vector<vector<int>>& Group, vector<vector<int>>& ComponentofGroup)
{
	Group.clear();
	ComponentofGroup = Group;

	vector<int>PCBSeq;
	PCBSeq.resize(PCBType.size());
	for (int i = 0; i < PCBType.size(); i++)
		PCBSeq[i] = i;

	while (PCBSeq.size() != 0)
	{
		int RandPCBFlag = rand() % PCBSeq.size();
		vector<int>GroupSeq;
		GroupSeq.clear();
		vector<int>ComponentofGroupSeq = GroupSeq;

		GroupSeq.push_back(PCBSeq[RandPCBFlag]);
		GetComponentofAGroup(GroupSeq, ComponentofGroupSeq);
		PCBSeq.erase(PCBSeq.begin() + RandPCBFlag);

		int Flag = 1;
		while (Flag)
		{
			Flag = 0;
			vector<int>TempGroupSeq = GroupSeq;
			vector<int>TempComponentofGroupSeq = ComponentofGroupSeq;

			if (PCBSeq.size() > 0)
			{
				//计算剩下的PCB板与该Goup的相似度
				vector<float>SimilarityBetweenAPCBandAGroup;
				SimilarityBetweenAPCBandAGroup.resize(PCBSeq.size());
				for (int i = 0; i < PCBSeq.size(); i++)
					SimilarityBetweenAPCBandAGroup[i] = GetSimilarityBetweenAPCBandAGroup(PCBSeq[i], ComponentofGroupSeq);

				vector<int> PCBSeqOrder;
				GetOrder(SimilarityBetweenAPCBandAGroup, PCBSeqOrder);
				//			GetOrder(SimilarityBetweenAPCBandAGroup, PCBSeq, PCBSeqOrder);

				for (int i = 0; i < PCBSeqOrder.size(); i++)
				{
					TempGroupSeq.push_back(PCBSeq[PCBSeqOrder[i]]);
					int TempNumofComponentofGroupSeq = GetComponentofAGroup(TempGroupSeq, TempComponentofGroupSeq);
					if (TempNumofComponentofGroupSeq <= gCapacity)
					{
						GroupSeq = TempGroupSeq;
						ComponentofGroupSeq = TempComponentofGroupSeq;
						PCBSeq.erase(PCBSeq.begin() + PCBSeqOrder[i]);
						Flag = 1;
						break;
					}
				}
			}
		}
		Group.push_back(GroupSeq);
		ComponentofGroup.push_back(ComponentofGroupSeq);
	}
	int MakeSpan = GetMakeSpan(Group, ComponentofGroup);
	return MakeSpan;
}

int Heuristic_BasedOnPCBpair(vector<vector<int>>& Group, vector<vector<int>>& ComponentofGroup)
{
	Group.clear();
	ComponentofGroup = Group;

	vector<int>PCBSeq;
	PCBSeq.resize(PCBType.size());
	for (int i = 0; i < PCBType.size(); i++)
		PCBSeq[i] = i;

	while (PCBSeq.size() != 0)
	{
		vector<int>GroupSeq;
		GroupSeq.clear();
		vector<int>ComponentofGroupSeq = GroupSeq;

		vector<vector<int>>PCBPairArray;
		PCBPairArray.clear();

		vector<float>PCBPairSimilarity;
		PCBPairSimilarity.clear();

		if (PCBSeq.size() >= 2)
		{
			vector<vector<float>>SimilarityAmongPCBSeq;
			GetJaccardSimilarityAmongUcompletedPCBs(PCBType, PCBSeq, SimilarityAmongPCBSeq);

			for (int i = 0; i < SimilarityAmongPCBSeq.size(); i++)
			{
				for (int j = i + 1; j < SimilarityAmongPCBSeq.size(); j++)
				{
					vector<int>PCBPair;
					PCBPair.clear();
					PCBPairSimilarity.push_back(SimilarityAmongPCBSeq[i][j]);
					PCBPair.push_back(PCBSeq[i]);
					PCBPair.push_back(PCBSeq[j]);
					PCBPairArray.push_back(PCBPair);
				}
			}
		}
		else
		{
			GroupSeq.push_back(PCBSeq[0]);
			GetComponentofAGroup(GroupSeq, ComponentofGroupSeq);
			PCBSeq.erase(PCBSeq.begin() + 0);
		}

		if (PCBPairArray.size() != 0)
		{
			//将PCB Pair按照相似性排序
			vector<int>SimilarityofPCBPairOrder;
			GetOrder(PCBPairSimilarity, SimilarityofPCBPairOrder);

			for (int i = 0; i < SimilarityofPCBPairOrder.size(); i++)
			{
				vector<int>TempGroupSeq = PCBPairArray[SimilarityofPCBPairOrder[i]];
				vector<int>TempComponentofGroupSeq;
				int TempNumofComponentinPCBPair = GetComponentofAGroup(TempGroupSeq, TempComponentofGroupSeq);
				if (TempNumofComponentinPCBPair <= gCapacity)
				{
					GroupSeq = TempGroupSeq;
					ComponentofGroupSeq = TempComponentofGroupSeq;
					break;
				}
			}

			if (GroupSeq.size() > 0)
			{
				for (int i = 0; i < GroupSeq.size(); i++)
				{
					for (int j = 0; j < PCBSeq.size(); j++)
					{
						if (GroupSeq[i] == PCBSeq[j])
						{
							PCBSeq.erase(PCBSeq.begin() + j);
							break;
						}
					}
				}
			}
			else
			{
				int RandPCBFlag = rand() % PCBSeq.size();
				GroupSeq.push_back(PCBSeq[RandPCBFlag]);
				GetComponentofAGroup(GroupSeq, ComponentofGroupSeq);
				PCBSeq.erase(PCBSeq.begin() + RandPCBFlag);
			}

			vector<float>SimilarityBetweenAPCBandGroup;
			SimilarityBetweenAPCBandGroup.resize(PCBSeq.size());
			for (int i = 0; i < PCBSeq.size(); i++)
				SimilarityBetweenAPCBandGroup[i] = GetSimilarityBetweenAPCBandAGroup(PCBSeq[i], ComponentofGroupSeq);

			vector<int>SimilarityBetweenPCBandGroupOrder;
			GetOrder(SimilarityBetweenAPCBandGroup, SimilarityBetweenPCBandGroupOrder);

			for (int i = 0; i < SimilarityBetweenPCBandGroupOrder.size(); i++)
			{
				vector<int>TempGroupSeq = GroupSeq;
				vector<int>TempComponentofGroupSeq = ComponentofGroupSeq;

				TempGroupSeq.push_back(PCBSeq[SimilarityBetweenPCBandGroupOrder[i]]);
				int TempNumofComponentinPCBPair = GetComponentofAGroup(TempGroupSeq, TempComponentofGroupSeq);
				if (TempNumofComponentinPCBPair <= gCapacity)
				{
					GroupSeq = TempGroupSeq;
					ComponentofGroupSeq = TempComponentofGroupSeq;
				}
			}
			for (int m = 0; m < GroupSeq.size(); m++)
			{
				for (int n = 0; n < PCBSeq.size(); n++)
				{
					if (GroupSeq[m] == PCBSeq[n])
					{
						PCBSeq.erase(PCBSeq.begin() + n);
						break;
					}
				}
			}
		}
		Group.push_back(GroupSeq);
		ComponentofGroup.push_back(ComponentofGroupSeq);
	}
	int MakeSpan = GetMakeSpan(Group, ComponentofGroup);
	return MakeSpan;
}



