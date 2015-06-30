#include <iostream>
#include <algorithm>
#include <iterator>
#include <random>

using namespace std;

void SelectionSort(int *arr, int size)
{
	int index;


	for (int i = 0; i < size; ++i)
	{
		index = i;
		for (int j = i + 1; j < size; ++j)
		{
			if (arr[index] > arr[j])
			{
				index = j;
			}
		}
		if (i != index)
		{
			swap(arr[i], arr[index]);
		}
	}
}

int main()
{
	int arr[100000];
	int size = sizeof(arr) / sizeof(int);
	uniform_int_distribution<> dis(1, 100000);

	for (int i = 0; i < size; ++i)
	{
		arr[i] = dis(random_device());
	}
	
	copy(begin(arr), end(arr), ostream_iterator<int>(cout, " "));
	cout << endl;

	SelectionSort(arr, size);

	cout << "after sorting" << endl;
	copy(begin(arr), end(arr), ostream_iterator<int>(cout, " "));
	cout << endl;

	system("pause");
	return 0;
}
