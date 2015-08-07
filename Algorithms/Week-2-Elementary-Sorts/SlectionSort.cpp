#include <iostream>

using namespace std;

template<typename T>
bool lessthan(T a, T b)
{
	return a < b;
}

template<typename T>
void exchange(T &a, T &b)
{
	T t;
	t = a;
	a = b;
	b = t;
}

template<typename T>
void selection_sort(T arr[], int size)
{
	int min;
	for (int i = 0; i < size - 1; ++i)
	{
		min = i;
		for (int j = i + 1; j < size; ++j)
		{
			if (lessthan(arr[j], arr[min]))
			{
				min = j;
			}
		}
		if (min != i)
		{
			exchange(arr[i], arr[min]);
		}
	}
}

int main()
{
	int arr[] = { 9, 7, 5, 3, 1, 4, 8, 6, 2 };

	for (auto i : arr)
	{
		cout << i << " ";
	}
	cout << endl;

	selection_sort(arr, sizeof(arr) / sizeof(*arr));

	for (auto i : arr)
	{
		cout << i << " ";
	}
	cout << endl;

	cin.get();
	return 0;
}
