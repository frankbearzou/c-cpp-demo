#include <iostream>
#include <string>
#include <random>

using namespace std;

int main()
{
	int min = 1;
	int max = 100;
	int result;
	int n;
	int times = 1;
	
	uniform_int_distribution<> dis(min, max);
	result = dis(random_device());

	cout << result << endl;

	do 
	{
		cout << "the number between " << min << " and " << max << endl;
		cout << "please input the number that you guess:";
		cin >> n;
		if (n < min || n > max)
		{
			cout << "the guess number you input is not between " << min << " and " << max << endl;
		}
		else if (n == result)
		{
			cout << "congratulation! you are right" << endl;
		}
		else
		{
			if (n > result)
			{
				cout << "you guess is too bigger, please guess again" << endl;
				max = n - 1;
			}
			else if (n < result)
			{
				cout << "you guess is too smaller, please guess again" << endl;
				min = n + 1;
			}
		}
	} while (n != result);

	system("pause");
	return 0;
}
