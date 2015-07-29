#include <iostream>

using namespace std;

template<typename T>
class FixedCapacityStack
{
public:
	FixedCapacityStack(int capacity = 0) : N{ 0 }, _capacity{capacity}
	{
		arr = new T[capacity];
	}

	~FixedCapacityStack()
	{
		if (arr != nullptr)
		{
			delete arr;
			arr = nullptr;
		}
	}

	bool isEmpty()
	{
		return N == 0;
	}

	void push(T item)
	{
		arr[N++] = item;
	}

	T pop()
	{
		return arr[--N];
	}
private:
	T *arr;
	int N;
	int _capacity;
};

int main()
{
	FixedCapacityStack<int> sta(10);
	cout << "is empty: " << sta.isEmpty() << endl;

	sta.push(1);
	sta.push(3);
	sta.push(5);
	sta.push(8);
	sta.push(9);

	cout << sta.pop() << endl;
	cout << "is empty: " << sta.isEmpty() << endl;
	cout << sta.pop() << endl;

	cout << "is empty: " << sta.isEmpty() << endl;

	while (!sta.isEmpty())
	{
		cout << sta.pop() << endl;
	}

	cout << "is empty: " << sta.isEmpty() << endl;

	cin.get();
	return 0;
}
