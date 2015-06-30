#include <iostream>
#include <algorithm>
#include <iterator>
#include <random>

using namespace std;

void InsertionSort(int *arr, int size)
{
	int key;
	int j;
	for (int i = 1; i < size; ++i)
	{
		key = arr[i];
		for (j = i - 1; j >= 0; --j)
		{
			if (arr[j] > key)
			{
				arr[j + 1] = arr[j];
			}
			else
			{
				arr[j + 1] = key;
				break;
			}
		}
		arr[j + 1] = key;
	}
}

int main()
{
	int arr[10];
	int size = sizeof(arr) / sizeof(int);
	uniform_int_distribution<> dis(1, 100);

	for (int i = 0; i < size; ++i)
	{
		arr[i] = dis(random_device());
	}
	
	copy(begin(arr), end(arr), ostream_iterator<int>(cout, " "));
	cout << endl;

	InsertionSort(arr, size);

	cout << "after sorting" << endl;
	copy(begin(arr), end(arr), ostream_iterator<int>(cout, " "));
	cout << endl;

	system("pause");
	return 0;
}
