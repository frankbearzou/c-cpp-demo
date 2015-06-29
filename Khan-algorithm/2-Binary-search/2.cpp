/*
Implementing binary search of an array
https://www.khanacademy.org/computing/computer-science/algorithms/binary-search/a/implementing-binary-search-of-an-array
*/

#include <iostream>
#include <random>

using namespace std;

int binarySearch(int *arr, int arr_size, int target)
{
	int min = 0;
	int max = arr_size;
	int guess;

	do 
	{
		if (max < min)
		{
			return -1;
		}
		guess = (min + max) / 2;
		if (arr[guess] == target)
		{
			return guess;
		}
		else if (arr[guess] > target)
		{
			max = guess - 1;
		}
		else if (arr[guess] < target)
		{
			min = guess + 1;
		}
	} while (guess != target);

	return -1;
}

int main()
{
	int primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
		53, 59, 61, 67, 71, 73, 79, 83, 89, 97 };
	int min = 0;
	int max = sizeof(primes) / sizeof(int);
	int guess;

	uniform_int_distribution<> dis(min, max);
	guess = dis(random_device());

	cout << "index:" << guess << ", number:" << primes[guess] << endl;

	int ret = binarySearch(primes, max, primes[guess]);
	cout << ret << endl;

	system("pause");
	return 0;
}
