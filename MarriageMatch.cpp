#include "MarriageMatch.h"

void Match :: loadMaleData(const char *name)
{
	FILE *pf = fopen(name,"r");
	if(pf == nullptr)
	{
		cout << name << "�ļ�������";
		return ;
	}
	maleVec.reserve(MAN_NUM);
	int data[7];
		
	while(!feof(pf))   //feof�ж��ļ��Ƿ���ꡣ���ļ���ĩβ������eof
	{  
		fscanf(pf,"%d,%d,%d,%d,%d,%d,%d",&data[0],&data[1],&data[2],&data[3],&data[4],&data[5],&data[6]); 
		maleVec.push_back(Male(data));//��ֵ���õĿ��������߼�move
	} 
	fclose(pf);
	pf = nullptr;
}

void Match :: loadFemaleData(const char *name)
{
	FILE *pf = fopen(name,"r");
	if(pf == nullptr)
	{
		cout << name << "�ļ�������";
		return ;
	}
	femaleVec.reserve(MAN_NUM);
	int data[7];
	while(!feof(pf))   //feof�ж��ļ��Ƿ���ꡣ���ļ���ĩβ������eof
	{  
		fscanf(pf,"%d,%d,%d,%d,%d,%d,%d",&data[0],&data[1],&data[2],&data[3],&data[4],&data[5],&data[6]); 
		femaleVec.push_back(Female(data));//��ֵ���õĿ��������߼�move
	} 
	fclose(pf);
	pf = nullptr;
}

void Match :: beginMatch(const char *name)
{
	//������ʿ��Ůʿ�������   5��5Ů   i, j�ֱ�����Ů�����
 	for(int i = 0 ; i < MAN_NUM;i++)
	{
		for(int j = 0 ; j < MAN_NUM ; j++)
		{
			maleVec[i].setsatisfyMap(femaleVec[j].id(),MaLeToFemale(i,j));
		}
	}
	//�������ļ�
	FILE *pf = fopen(name,"r");
	if(pf == nullptr)
	{
		cout << name << "�ļ�������";
		return ;
	}
	int data[7];
	int nom = 1;
	int sex;
	int Num = 1;//��Ҹ���
	while(!feof(pf))
	{  
		fscanf(pf,"%d,%d,%d,%d,%d,%d,%d",&data[0],&data[1],&data[2],&data[3],&data[4],&data[5],&data[6]); 
		sex = data[0];
		data[0] = -1;
		if(1 == sex)//����������
		{
			maleVec.push_back(Male(data));
			for(int i = 0 ; i < MAN_NUM;i++)//����������vec�����Ǽ�������ȡ�
			{
				maleVec[MAN_NUM].setsatisfyMap(femaleVec[i].id(),MaLeToFemale(MAN_NUM,i));
			}
		}
		else
		{
			femaleVec.push_back(Female(data));
			//Ů�ļӽ�ȥ���еľ�Ӧ����101���˵������
			for(int i = 0 ; i < MAN_NUM;i++)//����������vec�����Ǽ�������ȡ�
			{
				maleVec[i].setsatisfyMap(-1,MaLeToFemale(i,MAN_NUM));
			}
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int count = 0;
		while(true)
		{
			//ÿһ����ʿ��������
			for(int i = 0;i < maleVec.size()-count;i++)//i�����
			{
				ToInvite(i,sex,count);
			}
			//�ҵ�����������Ůʿid�����
			int Fe_id = FemaleMostInvite(count);
			int Fe_num = FemaleIDtoNum(Fe_id);
			//�ҵ���Ůʿ���������ʿ
			int Ma_id = femaleVec[Fe_num].FindMaxID();
			int Ma_num = MaleIDtoNum(Ma_id);
			if(Fe_id == -1 || Ma_id == -1)
			{
				cout<<"��"<<Num++<<"��player���룺"<<Ma_id<<":"<<Fe_id<<endl;
				//������ƥ��������ָ���������һ�����ǵ�ƥ��
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
			else//����û��ƥ���ϣ���ƥ���ϵ��������Ƶ�ĩβ�����Ůʿ���ܵ����룬�ٴ�ƥ�䡣
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
				//����������ʿ��map������Ů���Ƶ����
				for(auto it = maleVec.begin();it != maleVec.end();it++)
				{
					(*it).FemaleEnd(Fe_id,count);
				}
				count++;
			}
			//�����˶����˶��������û��
			if(maleVec.size()-count == 1 || femaleVec.size()-count == 1)
			{
				cout<<"��"<<count++<<"��player���룺"<<endl;
				//�ָ�
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
	//�ҵ����������Ů��,��������id
	int max_female_id = maleVec[i].FindMaxID(sex,count);
	int max_female_num = FemaleIDtoNum(max_female_id);
	femaleVec[max_female_num].setinviteMap(maleVec[i].id(),FemaleToMale(i,max_female_num));
}
