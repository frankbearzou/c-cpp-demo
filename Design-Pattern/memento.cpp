#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <iterator>

using namespace std;

template<class T>
class Memento
{
public:
	
private:
	stack<T> _sta;
	Memento()
	{}
	Memento(const stack<T>& sta) : _sta(sta)
	{}

	template<class T>
	friend class Orig;
};

template<class T>
class Orig
{
public:

	Orig()
	{}

	void record(T re)
	{
		_sta.push(re);
	}

	T remove()
	{
		T t = _sta.top();
		_sta.pop();
		return t;
	}

	bool is_empty()
	{
		return _sta.empty();
	}

	Memento<T> check_point()
	{
		// copy constructor, we still need to use _sta
		return Memento<T>(_sta);
	}

	void rollback(Memento<T> &memento)
	{
		_sta = memento._sta;
	}

	// print the first value firstly
	friend ostream& operator<<(ostream &os, const Orig<T> &o)
	{
		stack<T> t(o._sta);
		vector<T> v;
		os << "[";
		while (!t.empty())
		{
			v.push_back(move(t.top()));
			t.pop();
		}
		if (v.size() > 1)
			copy(v.crbegin(), v.crend() - 1, ostream_iterator<T>(os, ", "));
		if (v.size() > 0)
			os << *v.cbegin();
		os << "]";
		return os;
	}
private:
	stack<T> _sta;
};

void main()
{
	Orig<int> o;
	for (int i = 0; i < 5; ++i) {
		o.record(i);
	}

	Memento<int> m1 = o.check_point();
	cout << "check point 1: ";
	cout << o << endl;

	cout << "insert 9: ";
	o.record(9);
	cout << o << endl;

	cout << "rollback to cp1: ";
	o.rollback(m1);
	cout << o << endl;

	cout << "clear: ";
	while (!o.is_empty()) {
		o.remove();
	}
	cout << o << endl;

	system("pause");
}
