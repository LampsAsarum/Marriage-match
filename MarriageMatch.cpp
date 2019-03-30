#include "MarriageMatch.h"

void Match :: loadMaleData(const char *name)
{
	FILE *pf = fopen(name,"r");
	if(pf == nullptr)
	{
		cout << name << "文件不存在";
		return ;
	}
	maleVec.reserve(MAN_NUM);
	int data[7];
		
	while(!feof(pf))   //feof判断文件是否读完。在文件的末尾都会有eof
	{  
		fscanf(pf,"%d,%d,%d,%d,%d,%d,%d",&data[0],&data[1],&data[2],&data[3],&data[4],&data[5],&data[6]); 
		maleVec.push_back(Male(data));//右值引用的拷贝，或者加move
	} 
	fclose(pf);
	pf = nullptr;
}

void Match :: loadFemaleData(const char *name)
{
	FILE *pf = fopen(name,"r");
	if(pf == nullptr)
	{
		cout << name << "文件不存在";
		return ;
	}
	femaleVec.reserve(MAN_NUM);
	int data[7];
	while(!feof(pf))   //feof判断文件是否读完。在文件的末尾都会有eof
	{  
		fscanf(pf,"%d,%d,%d,%d,%d,%d,%d",&data[0],&data[1],&data[2],&data[3],&data[4],&data[5],&data[6]); 
		femaleVec.push_back(Female(data));//右值引用的拷贝，或者加move
	} 
	fclose(pf);
	pf = nullptr;
}

void Match :: beginMatch(const char *name)
{
	//计算男士对女士的满意度   5男5女   i, j分别是男女的序号
 	for(int i = 0 ; i < MAN_NUM;i++)
	{
		for(int j = 0 ; j < MAN_NUM ; j++)
		{
			maleVec[i].setsatisfyMap(femaleVec[j].id(),MaLeToFemale(i,j));
		}
	}
	//打开主角文件
	FILE *pf = fopen(name,"r");
	if(pf == nullptr)
	{
		cout << name << "文件不存在";
		return ;
	}
	int data[7];
	int nom = 1;
	int sex;
	int Num = 1;//玩家个数
	while(!feof(pf))
	{  
		fscanf(pf,"%d,%d,%d,%d,%d,%d,%d",&data[0],&data[1],&data[2],&data[3],&data[4],&data[5],&data[6]); 
		sex = data[0];
		data[0] = -1;
		if(1 == sex)//主角是男性
		{
			maleVec.push_back(Male(data));
			for(int i = 0 ; i < MAN_NUM;i++)//将最后这个入vec的主角计算满意度。
			{
				maleVec[MAN_NUM].setsatisfyMap(femaleVec[i].id(),MaLeToFemale(MAN_NUM,i));
			}
		}
		else
		{
			femaleVec.push_back(Female(data));
			//女的加进去后，男的就应该有101个人的满意度
			for(int i = 0 ; i < MAN_NUM;i++)//将最后这个入vec的主角计算满意度。
			{
				maleVec[i].setsatisfyMap(-1,MaLeToFemale(i,MAN_NUM));
			}
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int count = 0;
		while(true)
		{
			//每一个男士发送邀请
			for(int i = 0;i < maleVec.size()-count;i++)//i是序号
			{
				ToInvite(i,sex,count);
			}
			//找到受邀请最多的女士id和序号
			int Fe_id = FemaleMostInvite(count);
			int Fe_num = FemaleIDtoNum(Fe_id);
			//找到该女士最满意的男士
			int Ma_id = femaleVec[Fe_num].FindMaxID();
			int Ma_num = MaleIDtoNum(Ma_id);
			if(Fe_id == -1 || Ma_id == -1)
			{
				cout<<"第"<<Num++<<"组player加入："<<Ma_id<<":"<<Fe_id<<endl;
				//该主角匹配结束，恢复，进入下一个主角的匹配
				if(Ma_id == -1)
				{
					//maleVec.pop_back();
					for(auto it = maleVec.begin();it != maleVec.end();it++)
					{
						if((*it).id() == -1)
						{
							maleVec.erase(it);
							break;
						}
					}
					for(auto it = femaleVec.begin();it != femaleVec.end();it++)
					{
						if((*it).MapSize() > 0)
						{
							(*it).Clean();
						}
					}
				}
				else
				{
					//femaleVec.pop_back();
					for(auto it = femaleVec.begin();it != femaleVec.end();it++)
					{
						if((*it).id() == -1)
						{
							femaleVec.erase(it);
							break;
						}
					}
					for(auto it = maleVec.begin();it != maleVec.end();it++)
					{
						(*it).CleanEnd();
					}
				}
				break;
			}
			else//主角没有匹配上，将匹配上的两个人移到末尾，清除女士接受的邀请，再次匹配。
			{
				swap(maleVec[Ma_num], maleVec[maleVec.size()-count-1]);
				swap(femaleVec[Fe_num], femaleVec[femaleVec.size()-count-1]);
				//count++;
				for(auto it = femaleVec.begin();it != femaleVec.end();it++)
				{
					if((*it).MapSize() > 0)
					{
						(*it).Clean();
					}
				}
				//并且所有男士的map里，将这个女人移到最后
				for(auto it = maleVec.begin();it != maleVec.end();it++)
				{
					(*it).FemaleEnd(Fe_id,count);
				}
				count++;
			}
			//其余人都有了对象而主角没有
			if(maleVec.size()-count == 1 || femaleVec.size()-count == 1)
			{
				cout<<"第"<<count++<<"组player加入："<<endl;
				//恢复
				if(Ma_id == -1)
				{
					maleVec.pop_back();
					for(auto it = femaleVec.begin();it != femaleVec.end();it++)
					{
						if((*it).MapSize() > 0)
						{
							(*it).Clean();
						}
					}
				}
				else
				{
					femaleVec.pop_back();
					for(auto it = maleVec.begin();it != maleVec.end();it++)
					{
						(*it).CleanEnd();
					}
				}
				break;
			}
		}
		//cout<<"finish!"<<endl;

	} 
	fclose(pf);
	pf = nullptr;
}


void Match :: ToInvite(int i,int sex,int count)
{
	//找到满意度最大的女孩,并返回其id
	int max_female_id = maleVec[i].FindMaxID(sex,count);
	int max_female_num = FemaleIDtoNum(max_female_id);
	femaleVec[max_female_num].setinviteMap(maleVec[i].id(),FemaleToMale(i,max_female_num));
}
