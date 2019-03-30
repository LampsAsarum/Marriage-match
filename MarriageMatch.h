#include <iostream>
#include <unordered_map>
#include <algorithm>
using namespace std;
#define MAN_NUM 100

class Man
{
public:
	Man(int arr[7]):_id(arr[0]),_a1(arr[1]),_a2(arr[2]),_a3(arr[3]),_b1(arr[4]),_b2(arr[5]),_b3(arr[6])
	{}
	int id(){ return _id; }
	int a1(){ return _a1; }
	int a2(){ return _a2; }
	int a3(){ return _a3; }
	int b1(){ return _b1; }
	int b2(){ return _b2; }
	int b3(){ return _b3; }
private:
	int _id;
	int _a1;
	int _a2;
	int _a3;
	int _b1;
	int _b2;
	int _b3;
};

//男士样本类
class Male : public Man
{
public:
	Male(int arr[7]):Man(arr){}

	void setsatisfyMap(int id,int sat)
	{
		satisfyMap.insert(make_pair(id,sat));
	}

	int FindMaxID(int sex,int count)
	{
		int max = 0;
		int id = 0;
		if(sex == 1)
		{
			auto it = satisfyMap.begin();
			for(int i = 0; i < MAN_NUM-1-count && it != satisfyMap.end();i++)
			{
				if((*it).second > max)
				{
					max = (*it).second;
					id = (*it).first;
				}
				it++;
			}
		}
		else
		{
			auto it = satisfyMap.begin();
			for(int i = 0; i < MAN_NUM-count;i++)
			{
				if((*it).second > max)
				{
					max = (*it).second;
					id = (*it).first;
				}
				it++;
			}
		}
		return id;
	}

	void CleanEnd()
	{
		auto it = satisfyMap.begin();
		for(;it != satisfyMap.end();it ++)
		{
			if((*it).first == -1)
			{
				break;
			}
		}
		satisfyMap.erase(it);
	}

	void FemaleEnd(int Fe_id,int count)
	{
		//swap(satisfyMap[Fe_id], satisfyMap[id]);//map无法调用swap
		int num1 = satisfyMap[Fe_id];
		for(auto it = satisfyMap.begin();it != satisfyMap.end();it ++)
		{
			if((*it).first == Fe_id)
			{
				satisfyMap.erase(it);
				break;
			}
		}
		satisfyMap.insert(make_pair(Fe_id,num1));
	}
private:
	unordered_map<int, int> satisfyMap;//存每一个男的对每一个女人的满意度
};

//女士样本类
class Female : public Man
{
public:
	Female(int arr[7]):Man(arr){}
	void setinviteMap(int id,int sat)
	{
		inviteMap.insert(make_pair(id,sat));
	}

	int MapSize()
	{
		return inviteMap.size();
	}

	int FindMaxID()
	{
		int max = 0;int id = 0;
		for(auto it = inviteMap.begin();it!=inviteMap.end();++it)
		{
			if((*it).second > max)
			{
				max = (*it).second;
				id = (*it).first;
			}
		}
		return id;
	}

	void Clean()
	{
		inviteMap.clear();
	}
private:
	unordered_map<int, int> inviteMap;
};

//匹配类
class Match
{
public:
	void loadMaleData(const char *name);
	void loadFemaleData(const char *name);

	//每次载入一个主角的信息，开始匹配
	void beginMatch(const char *name);
	//男士给女士发送邀请
	void ToInvite(int i,int sex,int count);
private:
	vector<Male> maleVec; // 存储样本男性的容器
	vector<Female> femaleVec; // 存储样本女性的容器

	int MaLeToFemale(int i ,int j)
	{
		return maleVec[i].b1() * femaleVec[j].a1() + maleVec[i].b2() * femaleVec[j].a2() + maleVec[i].b3() * femaleVec[j].a3();
	}

	int FemaleToMale(int i ,int j)
	{
		return maleVec[i].a1() * femaleVec[j].b1() + maleVec[i].a2() * femaleVec[j].b2() + maleVec[i].a3() * femaleVec[j].b3();
	}

	int FemaleIDtoNum(int id)
	{
		int num = 0;
		for(auto it = femaleVec.begin();it != femaleVec.end();it++)
		{
			if(id == (*it).id())
			{
				return num;
			}
			num++;
		}
	}

	int MaleIDtoNum(int id)
	{
		int num = 0;
		for(auto it = maleVec.begin();it != maleVec.end();it++)
		{
			if(id == (*it).id())
			{
				return num;
			}
			num++;
		}
	}

	int FemaleMostInvite(int count)
	{
		int max = 0;
		int id = 0;
		for(auto it = femaleVec.begin();it != femaleVec.end()-count;it++)
		{
			if((*it).MapSize() > max)
			{
				max = (*it).MapSize();
				id = (*it).id();
			}
		}
		return id;
	}
};