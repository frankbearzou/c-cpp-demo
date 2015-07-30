#include <iostream>

using namespace std;

template<typename T>
struct Node
{
	T item;
	Node<T> *next;
};

template<typename T>
class LinkedQueue
{
public:
	LinkedQueue() :first{ nullptr }, last{ nullptr }
	{
	}

	~LinkedQueue()
	{
	}

	bool isEmpty()
	{
		return first == nullptr;
	}

	void enqueue(T item)
	{
		Node<T> *oldlast = last;
		last = new Node<T>;
		last->item = item;
		last->next = nullptr;
		if (isEmpty())
			first = last;
		else
			oldlast->next = last;
	}

	T dequeue()
	{
		T item = first->item;
		Node<T> *oldfirst = first;
		first = first->next;
		delete oldfirst;
		oldfirst = nullptr;

		return item;
	}
private:
	Node<T> *first;
	Node<T> *last;
};

int main()
{
	LinkedQueue<int> que;
	que.enqueue(1);
	que.enqueue(3);
	que.enqueue(5);
	que.enqueue(7);
	que.enqueue(9);

	cout << "dequeue: " << que.dequeue() << endl;
	while (!que.isEmpty())
	{
		cout << "dequeue: " << que.dequeue() << endl;
	}

	cin.get();
	return 0;
}
