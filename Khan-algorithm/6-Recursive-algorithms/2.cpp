/*
Recursive factorial
https://www.khanacademy.org/computing/computer-science/algorithms/recursive-algorithms/a/recursive-factorial
*/

#include <iostream>

using namespace std;

double recursive_factorial(int n)
{
	if (n < 0)
	{
		return 0;
	}
	else if (n == 0)
	{
		return 1;
	}
	
	return n * recursive_factorial(n - 1);
}

int main()
{
	cout << recursive_factorial(5) << endl;

	system("pause");
	return 0;
}
