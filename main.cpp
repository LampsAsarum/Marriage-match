#include "MarriageMatch.h"

int main()
{
	Match match;
	//1、载入100男士与100女士的信息，载入100主角
	match.loadMaleData("male.txt");
	match.loadFemaleData("female.txt");

	//2、每次载入一个主角的信息然后进行匹配
	match.beginMatch("players.txt");

	return 0;
}