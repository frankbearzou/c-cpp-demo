#include <iostream>

using namespace std;

template<typename T>
class ResizingArrayStack
{
public:
	ResizingArrayStack() : N{ 0 }, _capacity{ 1 }
	{
		arr = new T[1];
	}

	~ResizingArrayStack()
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
		if (N == _capacity)
		{
			resize(2 * _capacity);
		}
		arr[N++] = item;
	}

	T pop()
	{
		if (N > 0 && N == _capacity / 4)
			resize(_capacity / 2);
		return arr[--N];
	}
private:
	void resize(int capacity)
	{
		T *temp = new T[capacity];
		for (int i = 0; i < N; ++i)
		{
			temp[i] = arr[i];
		}
		delete arr;
		arr = temp;
		_capacity = capacity;
	}
private:
	T *arr;
	int N;
	int _capacity;
};

int main()
{
	ResizingArrayStack<int> sta;
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
