/*
Merge sort
https://www.khanacademy.org/computing/computer-science/algorithms/merge-sort/a/divide-and-conquer-algorithms
*/

#include <iostream>
#include <memory>

using namespace std;

void merge(int *array, int left, int mid, int right)
{
	int i, j, k;
	int left_length = mid - left + 1;
	int right_length = right - mid;
	unique_ptr<int[]> array_left(new int[left_length]);
	unique_ptr<int[]> array_right(new int[right_length]);

	// 1. copy left array
	for (i = 0; i < left_length; ++i)
	{
		array_left[i] = array[left + i];
	}

	// 2. copy right array
	for (j = 0; j < right_length; ++j)
	{
		array_right[j] = array[mid + 1 + j];
	}

	// 3. merge
	for (i = j = 0, k = left; i < left_length && j < right_length;)
	{
		if (array_left[i] < array_right[j])
		{
			array[k] = array_left[i];
			++i;
			++k;
		}
		else
		{
			array[k] = array_right[j];
			++j;
			++k;
		}
	}

	// 4. copy left remains to array
	for (; i < left_length; ++i)
	{
		array[k] = array_left[i];
		++k;
	}

	// 5. copy right remains to array
	for (; j < right_length; ++j)
	{
		array[k] = array_right[j];
		++k;
	}

}

void mergeSort(int *array, int left, int right)
{
	int mid;
	if (left < right)
	{
		mid = (left + right) / 2;
		mergeSort(array, left, mid);
		mergeSort(array, mid + 1, right);
		merge(array, left, mid, right);
	}
}

int main()
{
	int array[] = { 14, 7, 3, 12, 9, 11, 6, 2};

	for (const auto p : array)
	{
		cout << p << " ";
	}
	cout << endl;

	mergeSort(array, 0, sizeof(array) / sizeof(int) - 1);

	for (const auto p : array)
	{
		cout << p << " ";
	}
	cout << endl;

	system("pause");
	return 0;
}
