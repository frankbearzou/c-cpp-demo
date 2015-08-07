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
void insertion_sort(T arr[], int size)
{
	for (int i = 1; i < size; ++i)
	{
		for (int j = i; j > 0; --j)
		{
			if (lessthan(arr[j], arr[j - 1]))
			{
				exchange(arr[j], arr[j - 1]);
			}
			else
			{
				break;
			}
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

	insertion_sort(arr, sizeof(arr) / sizeof(*arr));

	for (auto i : arr)
	{
		cout << i << " ";
	}
	cout << endl;

	cin.get();
	return 0;
}
