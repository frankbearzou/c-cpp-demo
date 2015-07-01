/*
The factorial function
https://www.khanacademy.org/computing/computer-science/algorithms/recursive-algorithms/a/the-factorial-function
*/

#include <iostream>

using namespace std;

double factorial(int n)
{
	double result = 1;

	if (n < 0)
	{
		return 0;
	}

	for (; n > 1; --n)
	{
		result *= n;
	}

	return result;
}

int main()
{
	cout << factorial(5) << endl;

	system("pause");
	return 0;
}
