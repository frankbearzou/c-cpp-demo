#include <iostream>

using namespace std;

class Operations
{
public:
	Operations() :_a(0), _b(0)
	{}
	Operations(int a, int b) : _a(a), _b(b)
	{}
	void set_value(int a, int b)
	{
		_a = a;
		_b = b;
	}
	virtual int get_result() = 0;
protected:
	int _a;
	int _b;
};

class Add : public Operations
{
private:
	Add()
	{}
	int get_result() override
	{
		return _a + _b;
	}
	friend class OpeFactory;
};

class Minus : public Operations
{
private:
	Minus()
	{}
	int get_result() override
	{
		return _a - _b;
	}
	friend class OpeFactory;
};

class OpeFactory
{
public:
	static enum class op
	{
		add, minus
	};
	static Operations* getOper(op o)
	{
		if (o == op::add) {
			return new Add;
		}
		else if (o == op::minus) {
			return new Minus;
		}
		else {
			return nullptr;
		}
	}
};

void main()
{
	Operations *opt = OpeFactory::getOper(OpeFactory::op::add);
	if (opt == nullptr) {
		cerr << "opt err" << endl;
	}
	opt->set_value(3, 5);
	auto a = opt->get_result();
	cout << a << endl;
	delete opt;

	cout << static_cast<int>(OpeFactory::op::minus) << endl;

	system("pause");
}
