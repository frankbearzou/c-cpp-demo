/*
Using recursion to determine whether a word is a palindrome
https://www.khanacademy.org/computing/computer-science/algorithms/recursive-algorithms/a/using-recursion-to-determine-whether-a-word-is-a-palindrome
*/

#include <iostream>
#include <string>

using namespace std;

bool isPalindrome(string str)
{
	if (str.length() == 1 || str.length() == 0)
	{
		return true;
	}
	if (str.at(0) == str.at(str.length() - 1))
	{
		return isPalindrome(str.substr(1, str.length() - 2));
	}
	else
	{
		return false;
	}
}

int main()
{
	cout << boolalpha << isPalindrome("a") << endl;
	cout << boolalpha << isPalindrome("motor") << endl;
	cout << boolalpha << isPalindrome("rotor") << endl;

	system("pause");
	return 0;
}
