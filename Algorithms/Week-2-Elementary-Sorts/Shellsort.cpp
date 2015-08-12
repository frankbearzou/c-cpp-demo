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
void shell_sort(T arr[], int size)
{
	int h = 1;
	while (h < size / 3)
	{
		h = 3 * h + 1; // formula: 3 * x + 1. ex: 1, 4, 13, 40, 121...
	}

	while (h >= 1)
	{
		for (int i = h; i < size; ++i)
		{
			for (int j = i; j >= h && lessthan(arr[j], arr[j - h]); j -= h)
			{
				exchange(arr[j], arr[j - h]);

			}
		}
		h /= 3;
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

	shell_sort(arr, sizeof(arr) / sizeof(*arr));

	for (auto i : arr)
	{
		cout << i << " ";
	}
	cout << endl;

	cin.get();
	return 0;
}
