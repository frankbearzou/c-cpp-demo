/*
Computing powers of a number
https://www.khanacademy.org/computing/computer-science/algorithms/recursive-algorithms/a/computing-powers-of-a-number
*/

#include <iostream>

using namespace std;

double power(double x, int n)
{
	if (n == 0)
	{
		return 1;
	}
	else if (n == 1)
	{
		return x;
	}
	else if (n < 0) // n is negative
	{
		return 1 / power(x, -n);
	}
	else if (n % 2 == 1) // n is odd
	{
		return x * power(x, n - 1);
	}
	else if (n % 2 == 0) // n is even
	{
		return power(x, n / 2) * power(x, n / 2);
	}

	return 0;
}

int main()
{
	cout << power(3, 0) << endl;
	cout << power(3, 1) << endl;
	cout << power(3, 2) << endl;
	cout << power(3, -1) << endl;
	cout << power(3, 3) << endl; 


	system("pause");
	return 0;
}
