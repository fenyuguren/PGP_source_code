#pragma once
//number of components used by both PCBs i and j/number of components processed by at least one of the PCBs
void GetJaccardSimilarity(vector<vector<int>>Seq, vector<vector<float>>& SimilarritySeq)  //对于给定的一组PCB，求其相似性系数
{
	SimilarritySeq.resize(Seq.size());
	for (int type = 0; type < Seq.size(); type++)
		SimilarritySeq[type].resize(Seq.size(), 0);

	for (int type = 0; type < Seq.size(); type++)
	{
		for (int SecondType = 0; SecondType < Seq.size(); SecondType++)
		{
			if (type == SecondType)
				SimilarritySeq[type][SecondType] = 1;
			else
			{
				int NumCommomComponent = 0;
				int NumAllComponent = 0;

				for (int i = 0; i < Seq[0].size(); i++)
				{
					if ((Seq[type][i] == 1) && (Seq[SecondType][i] == 1))
						NumCommomComponent++;
					else if ((Seq[type][i] == 1) || (Seq[SecondType][i] == 1))
						NumAllComponent++;
				}

				if (NumAllComponent == 0)
					SimilarritySeq[type][SecondType] = 0;
				else
					SimilarritySeq[type][SecondType] = (float)NumCommomComponent / NumAllComponent;
			}
		}
	}
}

//GetGlobalSimilarity
void GetGlobalSimilarity(vector<vector<float>>SimilaritySeq, vector<float>& GlobalSimilarity)
{
	GlobalSimilarity.resize(SimilaritySeq.size(), 0);

	for (int i = 0; i < SimilaritySeq.size(); i++)
	{
		for (int j = 0; j < SimilaritySeq.size(); j++)
		{
			if (i != j)
				GlobalSimilarity[i] = GlobalSimilarity[i] + SimilaritySeq[i][j];
		}
	}
}

//将所有的PCB板根据相似性由大到小进行排序，并将相似性最大的PCB板的编号输出
void GetPCBNum_WithMaxGlobalSimilarity(vector<float>GlobalSimilarity, vector<int>& OrderGlobalSimilarity)   //
{
	OrderGlobalSimilarity.clear();

	vector<vector<float>>Order_List;
	Order_List.resize(GlobalSimilarity.size());
	for (int i = 0; i < Order_List.size(); i++)
		Order_List[i].resize(2);

	for (int i = 0; i < Order_List.size(); i++)
	{
		Order_List[i][0] = i;
		Order_List[i][1] = GlobalSimilarity[i];
	}

	sort(Order_List.begin(), Order_List.end(), [](vector<float> t1, vector<float> t2)
		{
			return (t1[1] > t2[1]);
		});

	for (int i = 0; i < Order_List.size(); i++)
		OrderGlobalSimilarity.push_back(Order_List[i][0]);
}

//判断一个PCB板子是否已经完成
int WhetherAPCBDone(vector<int>Seq)
{
	int APCBDone = 1;
	for (int i = 0; i < Seq.size(); i++)
	{
		if (Seq[i] == 1)
		{
			APCBDone = 0;
			break;
		}
	}
	return APCBDone;
}

//找到没有完成的PCB板
void UnFinishedPCBs(vector<vector<int>>Seq, vector<int>& UnFinishedPCBsArray)
{
	UnFinishedPCBsArray.clear();
	for (int i = 0; i < Seq.size(); i++)
	{
		for (int j = 0; j < Seq[i].size(); j++)
		{
			if (Seq[i][j] == 1)
			{
				UnFinishedPCBsArray.push_back(i);
				break;
			}
		}
	}
}

//计算一个工件和一个PCBgroup的相似度
float GetSimilarityBetweenAPCBandAGroup(int AUnfinishedPCB, vector<int>ComponentInAPCBGroup)
{
	vector<int>ComponentOnAPCB;
	GetComponentOnAPCB(PCBType[AUnfinishedPCB], ComponentOnAPCB);

	//找到2个数组中一共有多少个元素，有多少个共同元素
	int CommComponnent = 0;
	int AllComponent = 0;
	for (int i = 0; i < ComponentOnAPCB.size(); i++)
	{
		for (int j = 0; j < ComponentInAPCBGroup.size(); j++)
		{
			if (ComponentOnAPCB[i] == ComponentInAPCBGroup[j])
				CommComponnent++;
		}
	}
	AllComponent = ComponentOnAPCB.size() + ComponentInAPCBGroup.size() - CommComponnent;

	float SimilarityBetweenAPCBandAGroup = (float)CommComponnent / AllComponent;
	return SimilarityBetweenAPCBandAGroup;
}

void GetOrder(vector<float>Seq, vector<int>UnfinishedPCBArray, vector<int>& Order)
{
	Order.clear();

	vector<vector<float>>Order_List;
	Order_List.resize(Seq.size());
	for (int i = 0; i < Order_List.size(); i++)
		Order_List[i].resize(2);

	for (int i = 0; i < Order_List.size(); i++)
	{
		Order_List[i][0] = UnfinishedPCBArray[i];
		Order_List[i][1] = Seq[i];
	}

	sort(Order_List.begin(), Order_List.end(), [](vector<float> t1, vector<float> t2)
		{
			return (t1[1] > t2[1]);
		});

	for (int i = 0; i < Order_List.size(); i++)
		Order.push_back(Order_List[i][0]);
}

//数组元素按降序排列，并输出相对应的编号
void GetOrder(vector<float>Seq, vector<int>& Order)   //按降序排列
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

//按升序排列
void GetIncreasingOrder(vector<float>Seq, vector<int>& Order)
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
			return (t1[1] < t2[1]);
		});

	for (int i = 0; i < Order_List.size(); i++)
		Order.push_back(Order_List[i][0]);
}

//number of components used by both PCBs i and j/number of components processed by at least one of the PCBs
void GetJaccardSimilarityAmongUcompletedPCBs(vector<vector<int>>Seq, vector<int>UcompletedPCBs, vector<vector<float>>& SimilarityAmongUcompletedPCBs)  //对于给定的一组PCB，求其相似性系数
{
	SimilarityAmongUcompletedPCBs.resize(UcompletedPCBs.size());
	for (int type = 0; type < UcompletedPCBs.size(); type++)
		SimilarityAmongUcompletedPCBs[type].resize(UcompletedPCBs.size(), 0);

	for (int type = 0; type < UcompletedPCBs.size(); type++)
	{
		for (int SecondType = 0; SecondType < UcompletedPCBs.size(); SecondType++)
		{
			if (type == SecondType)
				SimilarityAmongUcompletedPCBs[type][SecondType] = 0;
			else
			{
				int NumCommomComponent = 0;
				int NumAllComponent = 0;

				for (int i = 0; i < Seq[0].size(); i++)
				{
					if ((Seq[UcompletedPCBs[type]][i] == 1) && (Seq[UcompletedPCBs[SecondType]][i] == 1))
					{
						//			NumAllComponent++;
						NumCommomComponent++;
					}
					else if ((Seq[UcompletedPCBs[type]][i] == 1) || (Seq[UcompletedPCBs[SecondType]][i] == 1))
						NumAllComponent++;
				}
				SimilarityAmongUcompletedPCBs[type][SecondType] = (float)NumCommomComponent / NumAllComponent;
			}
		}
	}
}

//计算一个group里面有多少个元件种类
int GetNumComponentofAgroup(vector<vector<int>>TempPCBType, vector<int>group, vector<int>& ComponentofAgroup)
{
	ComponentofAgroup.clear();

	for (int i = 0; i < TempPCBType[0].size(); i++)
	{
		for (int j = 0; j < group.size(); j++)
		{
			if (TempPCBType[group[j]][i] == 1)
			{
				ComponentofAgroup.push_back(i);
				break;
			}
		}
	}

	return ComponentofAgroup.size();
}

int Heuristic_1(float Level)
{
	vector<vector<int>>TempPCBType = PCBType;

	vector<vector<int>>ComponentAssignedtoMachineArray;
	ComponentAssignedtoMachineArray.clear();

	vector<vector<int>>GroupArrangement;
	GroupArrangement.clear();

	int FlagReSartGroup = 1;
	while (FlagReSartGroup)    //    (1)
	{
		vector<int>ComponentAssignedtoMachine;
		ComponentAssignedtoMachine.clear();

		vector<int>PCBgroup;
		PCBgroup.clear();

		vector<vector<float>>Similarity;
		GetJaccardSimilarity(TempPCBType, Similarity);

		vector<float>GlobalSimilarity;
		GetGlobalSimilarity(Similarity, GlobalSimilarity);            //(2)

		int PCBNum_WithMaxGlobalSimilarity = -1;
		vector<int>OrderGlobalSimilarity;
		GetPCBNum_WithMaxGlobalSimilarity(GlobalSimilarity, OrderGlobalSimilarity);

		for (int i = 0; i < TempPCBType.size(); i++)
		{
			int APCBDone = WhetherAPCBDone(TempPCBType[OrderGlobalSimilarity[i]]);
			if (APCBDone == 0)   // APCBDone=0说明该PCB板没有完成
			{
				PCBNum_WithMaxGlobalSimilarity = OrderGlobalSimilarity[i];
				break;
			}
		}

		vector<int>ComponentOnAPCB;
		GetComponentOnAPCB(TempPCBType[PCBNum_WithMaxGlobalSimilarity], ComponentOnAPCB);        //////

//		if (ComponentAssignedtoMachine.size() <= gCapacity)      // 这个if是否需要加上
//		{
		PCBgroup.push_back(PCBNum_WithMaxGlobalSimilarity);
		for (int i = 0; i < ComponentOnAPCB.size(); i++)
		{
			if (ComponentAssignedtoMachine.size() < gCapacity)
			{
				ComponentAssignedtoMachine.push_back(ComponentOnAPCB[i]);             //(3)
				TempPCBType[PCBNum_WithMaxGlobalSimilarity][ComponentOnAPCB[i]] = 0;
			}
		}
		//		}			

		int UnusedCapacity_of_Machine = gCapacity - ComponentAssignedtoMachine.size();      //(4)

		int Flag = 1;
		while (Flag)
		{
			//	cout << "********" << endl;
				//找到所有没有完成的PCB板
			vector<int>UnFinishedPCBsArray;
			UnFinishedPCBs(TempPCBType, UnFinishedPCBsArray);

			if (UnFinishedPCBsArray.size() != 0)
			{
				//分别计算每个没有完成的板子和PCBgroup的相似性
				vector<float>SimilaritySeqBetweenAgroup;
				SimilaritySeqBetweenAgroup.resize(UnFinishedPCBsArray.size(), 0);
				for (int i = 0; i < UnFinishedPCBsArray.size(); i++)
					SimilaritySeqBetweenAgroup[i] = GetSimilarityBetweenAPCBandAGroup(UnFinishedPCBsArray[i], ComponentAssignedtoMachine);

				vector<int>Order;
				GetOrder(SimilaritySeqBetweenAgroup, UnFinishedPCBsArray, Order);

				int FlagReAssign = 1;
				while (FlagReAssign)
				{
					int FirstPCBinOrder = Order[0];    //

					//计算PCB FirstPCBinOrder 中没有加工的元件的个数
					vector<int>ComponentOnAPCB;
					GetComponentOnAPCB(TempPCBType[FirstPCBinOrder], ComponentOnAPCB);
					int NumComponentofPCB = ComponentOnAPCB.size();
					int NumCommComponent = 0;
					for (int i = 0; i < ComponentOnAPCB.size(); i++)
					{
						for (int j = 0; j < ComponentAssignedtoMachine.size(); j++)
						{
							if (ComponentOnAPCB[i] == ComponentAssignedtoMachine[j])
							{
								NumCommComponent++;
								break;
							}
						}
					}

					float pValue = abs((float)SetupTimeofAComponent * (NumComponentofPCB - NumCommComponent - UnusedCapacity_of_Machine) / SetupTimeofAPCBgroup);
					if (pValue >= Level)
					{
						if (ComponentOnAPCB.size() <= (gCapacity - ComponentAssignedtoMachine.size()))
						{
							PCBgroup.push_back(FirstPCBinOrder);
							for (int i = 0; i < ComponentOnAPCB.size(); i++)
							{
								for (int j = 0; j < ComponentAssignedtoMachine.size(); j++)
								{
									if (ComponentOnAPCB[i] == ComponentAssignedtoMachine[j])
									{
										TempPCBType[FirstPCBinOrder][ComponentOnAPCB[i]] = 0;
										break;
									}
								}
							}
							for (int i = 0; i < TempPCBType[FirstPCBinOrder].size(); i++)
							{
								if ((TempPCBType[FirstPCBinOrder][i] == 1) && (ComponentAssignedtoMachine.size() < gCapacity))
								{
									ComponentAssignedtoMachine.push_back(i);
									TempPCBType[FirstPCBinOrder][i] = 0;
								}
							}
							//(9)
							Order.erase(Order.begin() + 0);
							FlagReAssign = 0;
							break;
						}
						else
						{
							//判断是否所有的PCB板子都已经完成
							UnFinishedPCBs(TempPCBType, UnFinishedPCBsArray);
							if (UnFinishedPCBsArray.size() == 0)
							{
								FlagReAssign = 0;
								Flag = 0;
								FlagReSartGroup = 0;
							}
							else if (Order.size() == 0)
							{
								FlagReAssign = 0;
								Flag = 0;
							}
							else
								Order.erase(Order.begin() + 0);      //
						}
					}
					else
					{
						Order.erase(Order.begin() + 0);
						UnFinishedPCBs(TempPCBType, UnFinishedPCBsArray);
						if (UnFinishedPCBsArray.size() == 0)
						{
							FlagReAssign = 0;
							Flag = 0;
							FlagReSartGroup = 0;
						}
						else if (Order.size() == 0)
						{
							FlagReAssign = 0;
							Flag = 0;
						}
					}
				}
			}
			else
			{
				FlagReSartGroup = 0;
				Flag = 0;
			}
		}
		ComponentAssignedtoMachineArray.push_back(ComponentAssignedtoMachine);
		GroupArrangement.push_back(PCBgroup);
	}
	int MakeSpan = GetMakeSpan(GroupArrangement, ComponentAssignedtoMachineArray);
	CheckChrom(GroupArrangement, ComponentAssignedtoMachineArray, MakeSpan);
	return MakeSpan;
}

int Heuristic_ByMyself(vector<vector<int>>& GroupArrangement, vector<vector<int>>& ComponentAssignedtoMachineArray)
{
	GroupArrangement.clear();
	ComponentAssignedtoMachineArray.clear();

	vector<int>UnCompletedPCBs;
	UnCompletedPCBs.resize(PCBType.size());
	for (int i = 0; i < UnCompletedPCBs.size(); i++)
		UnCompletedPCBs[i] = i;

	int FlagReSartGroup = 1;
	while (FlagReSartGroup)
	{
		vector<int>ComponentAssignedtoMachine;
		ComponentAssignedtoMachine.clear();

		vector<int>PCBgroup;
		PCBgroup.clear();

		if (UnCompletedPCBs.size() >= 2)
		{
			vector<vector<int>>PCBPairArray;
			PCBPairArray.clear();

			vector<float>PCBPairSimilarity;
			PCBPairSimilarity.clear();

			vector<vector<float>>SimilarityAmongUcompletedPCBs;
			GetJaccardSimilarityAmongUcompletedPCBs(PCBType, UnCompletedPCBs, SimilarityAmongUcompletedPCBs);

			for (int i = 0; i < SimilarityAmongUcompletedPCBs.size(); i++)
			{
				for (int j = i + 1; j < SimilarityAmongUcompletedPCBs.size(); j++)
				{
					vector<int>PCBPair;
					PCBPair.clear();
					PCBPairSimilarity.push_back(SimilarityAmongUcompletedPCBs[i][j]);
					PCBPair.push_back(UnCompletedPCBs[i]);
					PCBPair.push_back(UnCompletedPCBs[j]);
					PCBPairArray.push_back(PCBPair);
				}
			}

			//将PCB Pair按照相似性排序
			vector<int>SimilarityofUncompletedPCBOrder;
			GetOrder(PCBPairSimilarity, SimilarityofUncompletedPCBOrder);

			//将排在最前面的PCB pair 加入到一个新的组里面
			int Flaggroup = 1;
			while (Flaggroup)
			{
				if (SimilarityofUncompletedPCBOrder.size() > 0)
				{
					PCBgroup = PCBPairArray[SimilarityofUncompletedPCBOrder[0]];
					SimilarityofUncompletedPCBOrder.erase(SimilarityofUncompletedPCBOrder.begin() + 0);

					//判断是否满足容量要求
					vector<int>ComponentofAgroup;
					int NumofComponentofGroup = GetNumComponentofAgroup(PCBType, PCBgroup, ComponentofAgroup);
					if (NumofComponentofGroup <= gCapacity)
					{
						for (int i = 0; i < PCBgroup.size(); i++)
						{
							for (int j = 0; j < UnCompletedPCBs.size(); j++)
							{
								if (PCBgroup[i] == UnCompletedPCBs[j])
								{
									UnCompletedPCBs.erase(UnCompletedPCBs.begin() + j);
									break;
								}
							}
						}
						ComponentAssignedtoMachine = ComponentofAgroup;
						Flaggroup = 0;
					}
					else
						PCBgroup.clear();
				}
				else     //如果所有的PCB Pair都不满足容量的要求，则按照未完成PCB 的globalsimilarity进行加工
				{
					vector<float>GlobalSimilarity;
					GetGlobalSimilarity(SimilarityAmongUcompletedPCBs, GlobalSimilarity);
					//找到未完成的PCB板中globalsimilarity的相似性系数最大的PCB板，将其放到PCBgroup中开启一个新的group，作为该group里面的第一个PCB
					float TempMax = 0;
					int TemPCBwithMAX = -1;
					for (int i = 0; i < GlobalSimilarity.size(); i++)
					{
						if (GlobalSimilarity[i] > TempMax)
						{
							TempMax = GlobalSimilarity[i];
							TemPCBwithMAX = i;
						}
					}

					if (TemPCBwithMAX != -1)
					{
						PCBgroup.push_back(UnCompletedPCBs[TemPCBwithMAX]);
						UnCompletedPCBs.erase(UnCompletedPCBs.begin() + TemPCBwithMAX);

						vector<int>ComponentOnAPCB;
						GetComponentOnAPCB(PCBType[TemPCBwithMAX], ComponentOnAPCB);
						ComponentAssignedtoMachine = ComponentOnAPCB;

						Flaggroup = 0;
					}
					else
					{
						for (int i = 0; i < UnCompletedPCBs.size(); i++)
						{
							PCBgroup.push_back(UnCompletedPCBs[i]);
							vector<int>ComponentofAgroup;
							int NumofComponentofGroup = GetNumComponentofAgroup(PCBType, PCBgroup, ComponentofAgroup);
							if (NumofComponentofGroup > gCapacity)
							{
								PCBgroup.pop_back();
								break;
							}
							else
								UnCompletedPCBs.erase(UnCompletedPCBs.begin() + i);
						}
						GetNumComponentofAgroup(PCBType, PCBgroup, ComponentAssignedtoMachine);
						Flaggroup = 0;
					}
				}
			}

			//计算未完成的PCB与已有的Group之间的相似性
			if (UnCompletedPCBs.size() > 0)
			{
				vector<float>SimilarityBetweenAPCBandAGroup;
				SimilarityBetweenAPCBandAGroup.resize(UnCompletedPCBs.size(), 0);
				for (int i = 0; i < UnCompletedPCBs.size(); i++)
					SimilarityBetweenAPCBandAGroup[i] = GetSimilarityBetweenAPCBandAGroup(UnCompletedPCBs[i], ComponentAssignedtoMachine);

				vector<int>Order;
				GetOrder(SimilarityBetweenAPCBandAGroup, UnCompletedPCBs, Order);

				int FlagAddPCB = 1;
				while (FlagAddPCB)
				{
					if (Order.size() > 0)
					{
						int FirstPCBInOrder = Order[0];

						PCBgroup.push_back(FirstPCBInOrder);
						Order.erase(Order.begin() + 0);

						vector<int>ComponentofAgroup;
						int NumofComponentofGroup = GetNumComponentofAgroup(PCBType, PCBgroup, ComponentofAgroup);
						if (NumofComponentofGroup > gCapacity)
							PCBgroup.pop_back();
						else
						{
							ComponentAssignedtoMachine = ComponentofAgroup;
							for (int i = 0; i < UnCompletedPCBs.size(); i++)
							{
								if (UnCompletedPCBs[i] == FirstPCBInOrder)
									UnCompletedPCBs.erase(UnCompletedPCBs.begin() + i);
							}
						}
					}
					else
						FlagAddPCB = 0;
				}
			}
			else
				FlagReSartGroup = 0;
		}
		else   //这里要重新验证一下//
		{
			PCBgroup = UnCompletedPCBs;
			GetNumComponentofAgroup(PCBType, PCBgroup, ComponentAssignedtoMachine);
			UnCompletedPCBs.clear();

			if (UnCompletedPCBs.size() == 0)
				FlagReSartGroup = 0;
		}
		GroupArrangement.push_back(PCBgroup);
		ComponentAssignedtoMachineArray.push_back(ComponentAssignedtoMachine);
	}

	int MakeSpan = GetMakeSpan(GroupArrangement, ComponentAssignedtoMachineArray);
	return MakeSpan;
}
