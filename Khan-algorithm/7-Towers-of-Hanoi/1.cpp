/*
Towers of Hanoi
https://www.khanacademy.org/computing/computer-science/algorithms/towers-of-hanoi/a/towers-of-hanoi
*/

#include <iostream>
#include <cctype>
#include <string>

using namespace std;

char getSparePeg(char fromPeg, char toPeg)
{
	int sum = 'A' + 'B' + 'C';
	string s = "ABC";
	bool is_valid = false;
	fromPeg = toupper(fromPeg);
	toPeg = toupper(toPeg);

	// validate fromPeg
	for (const auto c : s)
	{
		if (is_valid == false && fromPeg == c)
		{
			is_valid = true;
			break;
		}
	}
	if (is_valid == false)
	{
		return '\0';
	}

	// validate toPeg
	is_valid = false;
	for (const auto c : s)
	{
		if (is_valid == false && toPeg == c)
		{
			is_valid = true;
			break;
		}
	}
	if (is_valid == false)
	{
		return '\0';
	}

	// validate if fromPeg equals toPeg
	if (fromPeg == toPeg)
		return '\0';

	return sum - fromPeg - toPeg;
}

void moveHanoi(int disk, char fromPeg, char toPeg, char sparePeg)
{
	if (disk > 0)
	{
		moveHanoi(disk - 1, fromPeg, sparePeg, toPeg);
		
		cout << "move " << disk << " from " << fromPeg << " to " << toPeg << endl;

		moveHanoi(disk - 1, sparePeg, toPeg, fromPeg);
	}
	
}

int solveHanoi(int disk, char fromPeg, char toPeg)
{
	char sparePeg = getSparePeg(fromPeg, toPeg);
	if (sparePeg == '\0')
	{
		cout << "error" << endl;
		return -1;
	}
	
	moveHanoi(disk, fromPeg, toPeg, sparePeg);
	return 0;
}

int main()
{
	solveHanoi(3, 'A', 'B');


	system("pause");
	return 0;
}
