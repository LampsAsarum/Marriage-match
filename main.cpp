#include "MarriageMatch.h"

int main()
{
	Match match;
	//1������100��ʿ��100Ůʿ����Ϣ������100����
	match.loadMaleData("male.txt");
	match.loadFemaleData("female.txt");

	//2��ÿ������һ�����ǵ���ϢȻ�����ƥ��
	match.beginMatch("players.txt");

	return 0;
}