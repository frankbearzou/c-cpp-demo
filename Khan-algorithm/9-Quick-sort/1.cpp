/*
Quick sort
https://www.khanacademy.org/computing/computer-science/algorithms/quick-sort/a/overview-of-quicksort
*/
#include <iostream>

using namespace std;

void swap(int &a, int &b)
{
	int t = a;
	a = b;
	b = t;
}

int partition(int *array, int p, int r)
{
	int i = p;
	int q = p;

	for (i = p; i < r; ++i)
	{
		if (array[i] <= array[r])
		{
			swap(array[i], array[q]);
			++q;
		}
	}

	swap(array[r], array[q]);
	return q;
}

void quickSort(int *array, int p, int r)
{
	if (p < r)
	{
		int pivot = partition(array, p, r);
		quickSort(array, p, pivot - 1);
		quickSort(array, pivot + 1, r);
	}
}



int main()
{
	int array[] = { 9, 7, 5, 11, 12, 2, 14, 3, 10, 6 };

	for (const auto p : array)
	{
		cout << p << " ";
	}
	cout << endl;

	quickSort(array, 0, sizeof(array) / sizeof(int) - 1);

	for (const auto p : array)
	{
		cout << p << " ";
	}
	cout << endl;

	

	system("pause");
	return 0;
}
