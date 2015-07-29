#include <iostream>

using namespace std;

template<class T>
struct Node
{
	T item;
	Node *next;
};

template<typename T>
class LinkedStack
{
public:
	LinkedStack()
	{
		first = nullptr;
	}

	~LinkedStack()
	{
		while (first != nullptr)
		{
			Node<T> *oldfirst = first;
			first = first->next;
			delete oldfirst;
			oldfirst = nullptr;
		}
	}

	bool isEmpty()
	{
		if (first == nullptr)
			return true;
		return false;
	}

	void push(T item)
	{
		if (isEmpty())
		{
			first = new Node<T>;
			first->item = item;
			first->next = nullptr;
		}
		else
		{
			Node<T> *oldfirst = first;
			first = new Node<T>;
			first->item = item;
			first->next = oldfirst;
		}
	}

	T pop()
	{
		Node<T> *oldfirst = first;
		T item = oldfirst->item;
		first = first->next;
		delete oldfirst;
		oldfirst = nullptr;
		return item;
	}
private:
	Node<T> *first;
};

int main1()
{
	LinkedStack<int> sta;
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
